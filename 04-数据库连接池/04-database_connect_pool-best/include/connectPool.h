#pragma once
#include <queue>
#include "databaseConnect.h"
#include <mutex>
#include <condition_variable>
#include <atomic>

class ConnectPool {
public:
    static ConnectPool* getConnectPool();

    ConnectPool(const ConnectPool&obj) = delete;

    ConnectPool& operator=(const ConnectPool&obj) = delete;

    std::shared_ptr<DatabaseConnect> getConnection();

    ~ConnectPool();

private:
    ConnectPool(); //懒汉模式——用到时创建
    bool parseJsonFile();

    void produceConnection();

    void recycleConnection();

    void addConnection();

private:
    std::string m_ip;
    std::string m_user;
    std::string m_passwd;
    std::string m_dbName;
    unsigned short m_port{};
    //连接池中，可连接数范围
    int m_minSize;
    int m_maxSize;
    //超时时间
    int m_timeout;
    int m_maxIdTime;

    std::queue<DatabaseConnect *> m_connectQ; //队列存放数据库连接
    std::mutex m_mutexQ; //操作连接池锁住
    std::condition_variable m_cond; //生产者消费者，控制连接池连接个数
    std::atomic_bool stop;
};
