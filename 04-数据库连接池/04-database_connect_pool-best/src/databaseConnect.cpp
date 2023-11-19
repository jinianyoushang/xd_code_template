#include "databaseConnect.h"

DatabaseConnect::DatabaseConnect(){
    //mysql 环境初始化
    m_conn = mysql_init(nullptr);
    //mysql 字符集设置 utf8
    mysql_set_character_set(m_conn, "utf8");
}

DatabaseConnect::~DatabaseConnect() {
    //mysql 环境关闭
    if (m_conn != nullptr) {
        mysql_close(m_conn);
    }
    freeResult();
}

bool DatabaseConnect::connect(const std::string&user, const std::string&passwd, const std::string&dbName,
                              const std::string&ip, const unsigned short port) const {
    const MYSQL* ptr = mysql_real_connect(m_conn, ip.c_str(), user.c_str(), passwd.c_str(), dbName.c_str(), port,
                                          nullptr, 0);
    return ptr != nullptr;
}

bool DatabaseConnect::update(const std::string&sql) const {
    if (mysql_query(m_conn, sql.c_str())) return false;
    return true;
}

bool DatabaseConnect::query(const std::string&sql) {
    freeResult(); //保存结果时，清空上一次的结果
    if (mysql_query(m_conn, sql.c_str())) return false;
    m_result = mysql_store_result(m_conn);
    return true;
}

bool DatabaseConnect::next() {
    if (m_result != nullptr) {
        m_row = mysql_fetch_row(m_result);
        if (m_row != nullptr) return true;
    }
    return false;
}

std::string DatabaseConnect::value(const int index) const {
    const int listCount = mysql_num_fields(m_result);
    if (index >= listCount || index < 0) return std::string();
    char* val = m_row[index]; //返回值为char* 有'\0'
    unsigned long length = mysql_fetch_lengths(m_result)[index]; //取出第index列的属性长度
    return std::string(val, length); //去除'\0'
}

bool DatabaseConnect::transaction() const {
    return mysql_autocommit(m_conn, false);
}

bool DatabaseConnect::commit() const {
    return mysql_commit(m_conn);
}

bool DatabaseConnect::rollback() const {
    return mysql_rollback(m_conn);
}

void DatabaseConnect::freeResult() {
    if (m_result) {
        mysql_free_result(m_result);
        m_result = nullptr;
    }
}

void DatabaseConnect::refreshAliveTime() {
    m_alivetime = std::chrono::steady_clock::now();
}

long long DatabaseConnect::getAliveTime() const {
    const std::chrono::nanoseconds res = std::chrono::steady_clock::now() - m_alivetime;
    const std::chrono::milliseconds millisec = std::chrono::duration_cast<std::chrono::milliseconds>(res);
    return millisec.count();
}
