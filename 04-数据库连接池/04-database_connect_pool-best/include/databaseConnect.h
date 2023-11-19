#pragma once
#include <iostream>
#include <chrono>
#include <string>
#include <mysql.h>

class DatabaseConnect {
public:
    //初始化数据库连接
    DatabaseConnect();

    //释放数据库连接
    ~DatabaseConnect();

    //连接数据库
    bool connect(const std::string&user, const std::string&passwd, const std::string&dbName, const std::string&ip, unsigned short port = 3306) const;

    //更新数据库连接
    bool update(const std::string&sql) const;

    //查询数据库连接
    bool query(const std::string&sql);

    //遍历查询得到的结果集
    bool next();

    //得到结果集中的字段值
    std::string value(int index) const;

    //事物操作
    bool transaction() const;

    //提交事物
    bool commit() const;

    //事物回滚
    bool rollback() const;

    //刷新起始的空闲时间点
    void refreshAliveTime();

    //计算连接存活的总时长
    long long getAliveTime() const;

private:
    void freeResult(); //释放结果集
private:
    MYSQL* m_conn = nullptr; //数据库对象
    MYSQL_RES* m_result = nullptr; //数据库结果集
    MYSQL_ROW m_row = nullptr; //结构为MYSQL_ROW的下一行结果
    std::chrono::steady_clock::time_point m_alivetime;
};
