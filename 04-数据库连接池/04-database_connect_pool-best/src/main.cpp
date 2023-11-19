#include "connectPool.h"
#include "databaseConnect.h"
#include <thread>
/*
1.单线程：使用/不使用连接池
    不使用：非连接池，单线程，用时：42693912629纳秒，42693毫秒
    使用：连接池，单线程，用时：9150030844纳秒，9150毫秒
2.多线程：使用/不使用连接池
    不使用：非连接池，多线程，用时：60792322338纳秒，60792毫秒
    使用：连接池，多线程，用时：3522916191纳秒，3522毫秒
*/

using namespace std;
using namespace chrono;
void op1(const int begin, const int end) {
    for (int i = begin; i < end; i++) {
        DatabaseConnect conn;
        conn.connect("root", "root", "testdb", "127.0.0.1");
        char sql[1024] = {0};
        sprintf(sql, "insert into person values(%d, 18, 'man', 'starry')", i);
        conn.update(sql);
    }
}

void op2(ConnectPool* pool, int begin, int end) {
    for (int i = begin; i < end; i++) {
        shared_ptr<DatabaseConnect> conn = pool->getConnection();
        char sql[1024] = {0};
        sprintf(sql, "insert into person values(%d, 18, 'man', 'starry')", i);
        //        string sql = "insert into person values(" + to_string(i) + ", 18, 'man', 'yll')";
        //      conn->update(sql.c_str());
        conn->update(sql);
    }
}

void test1() {
#if 0
    steady_clock::time_point begin = chrono::steady_clock::now();
    op1(0, 5000);
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    cout << "非连接池，单线程，用时：" << length.count() << "纳秒，" << length.count() / 1000000 << "毫秒" << endl;
#else
    ConnectPool* pool = ConnectPool::getConnectPool();
    steady_clock::time_point begin = steady_clock::now();
    op2(pool, 0, 5000);
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    cout << "连接池，单线程，用时：" << length.count() << "纳秒，" << length.count() / 1000000 << "毫秒" << endl;
#endif
}

void test2() {
#if 0
    steady_clock::time_point begin = steady_clock::now();
    thread t1(op1, 0, 1000);
    thread t2(op1, 1000, 2000);
    thread t3(op1, 2000, 3000);
    thread t4(op1, 3000, 4000);
    thread t5(op1, 4000, 5000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    cout << "非连接池，多线程，用时：" << length.count() << "纳秒，" << length.count() / 1000000 << "毫秒" << endl;
#else
    steady_clock::time_point begin = steady_clock::now();
    ConnectPool* pool = ConnectPool::getConnectPool();
    thread t1(op2, pool, 0, 1000);
    thread t2(op2, pool, 1000, 2000);
    thread t3(op2, pool, 2000, 3000);
    thread t4(op2, pool, 3000, 4000);
    thread t5(op2, pool, 4000, 5000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    cout << "连接池，多线程，用时：" << length.count() << "纳秒，" << length.count() / 1000000 << "毫秒" << endl;
#endif
}

int query() {
    DatabaseConnect conn;
    conn.connect("root", "root", "testdb", "127.0.0.1");
    string sql = "insert into person values(6, 18, 'man', 'starry')";
    bool flag = conn.update(sql);
    cout << "flag value" << (flag ? "success" : "fail") << endl;

    sql = "select * from person";
    conn.query(sql);
    while (conn.next()) {
        cout << conn.value(0) << ", "
                << conn.value(1) << ", "
                << conn.value(2) << ", "
                << conn.value(3) << endl;
    }
    return 0;
}

void clearDataBase() {
    DatabaseConnect conn;
    conn.connect("root", "root", "testdb", "127.0.0.1");
    const string sql = "delete from person";
    const bool flag = conn.update(sql);
    cout << "clear person  " << (flag ? "success" : "fail") << endl;
}

int main() {
    clearDataBase();
    // query(); //连接测试
    // test1();//单线程测试
    test2(); //多线程测试

    std::this_thread::sleep_for(3s);
    return 0;
}
