#include "connectPool.h"
#include <chrono>
#include <fstream>
#include <thread>
#include <jsoncpp/json/json.h>
using namespace Json;
using namespace std;

ConnectPool* ConnectPool::getConnectPool() {
    static ConnectPool pool;
    return &pool;
}

bool ConnectPool::parseJsonFile() {
    std::ifstream ifs("dbconf.json");
    Reader rd;
    Value root;
    rd.parse(ifs, root);
    if (root.isObject()) {
        m_ip = root["ip"].asString();
        m_port = root["port"].asInt();
        m_user = root["userName"].asString();
        m_passwd = root["password"].asString();
        m_dbName = root["dbName"].asString();
        m_minSize = root["minSize"].asInt();
        m_maxSize = root["maxSize"].asInt();
        m_maxIdTime = root["maxIdTime"].asInt();
        m_timeout = root["timeout"].asInt();
        return true;
    }
    return false;
}

void ConnectPool::produceConnection() {
    while (!stop) {
        std::unique_lock<mutex> locker(m_mutexQ);
        while (static_cast<int>(m_connectQ.size()) >= m_minSize) {
            m_cond.wait(locker);
            if (stop) {
                printf("produceConnection exit\n");
                return;
            }
        }
        addConnection();
        printf("produceConnection\n");
        m_cond.notify_all();
    }
}

void ConnectPool::recycleConnection() {
    while (!stop) {
        this_thread::sleep_for(chrono::milliseconds(500));
        printf("m_minSize:%d m_m_maxSize:%d m_connectQ.size():%d\n", m_minSize, m_maxSize,
               static_cast<int>(m_connectQ.size()));
        // ReSharper disable once CppDFAUnusedValue
        lock_guard<mutex> locker(m_mutexQ);
        //应该让连接池里面的数量为一个区间，而不是固定值
        while (static_cast<int>(m_connectQ.size()) > 2 * m_minSize) {
            const auto conn = m_connectQ.front();
            if (conn->getAliveTime() >= m_maxIdTime) {
                m_connectQ.pop();
                delete conn;
                printf("recycleConnection\n");
            }
            else break;
        }
    }
    //这个退出的显示可能不实时，不过对于单例模式，退出资源也会释放
    printf("recycleConnection exit\n");
}

void ConnectPool::addConnection() {
    const auto conn = new DatabaseConnect;
    if (conn->connect(m_user, m_passwd, m_dbName, m_ip, m_port)) {
        //printf("addConnection success\n");
    }
    else {
        printf("addConnection fail\n");
    }
    conn->refreshAliveTime();
    m_connectQ.push(conn);
}

shared_ptr<DatabaseConnect> ConnectPool::getConnection() {
    std::unique_lock<mutex> locker(m_mutexQ);
    while (m_connectQ.empty()) {
        if (cv_status::timeout == m_cond.wait_for(locker, chrono::milliseconds(m_timeout))) {
        }
    }
    shared_ptr<DatabaseConnect> connptr(m_connectQ.front(), [this](DatabaseConnect* conn) {
        lock_guard<mutex> locker1(m_mutexQ);
        conn->refreshAliveTime();
        m_connectQ.push(conn);
        //printf("recyle connection success\n");
    });
    m_connectQ.pop();
    m_cond.notify_all();
    return connptr;
}

ConnectPool::~ConnectPool() {
    stop = true;
    m_cond.notify_all();
    while (!m_connectQ.empty()) {
        const DatabaseConnect* conn = m_connectQ.front();
        m_connectQ.pop();
        delete conn;
    }
}


ConnectPool::ConnectPool(): m_minSize(96), m_maxSize(1000), m_timeout(1000), m_maxIdTime(5000), stop(false) {
    //加载配置文件
    if (!parseJsonFile()) {
        printf("parseJsonFile ERROR\n");
        return;
    }

    for (int i = 0; i < m_minSize; i++) {
        addConnection();
    }
    thread prodecer(&ConnectPool::produceConnection, this);
    thread recycler(&ConnectPool::recycleConnection, this);
    prodecer.detach();
    recycler.detach();
}
