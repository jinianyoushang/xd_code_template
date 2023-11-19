#pragma once
#include <iostream>
#include <mysql.h>
#include <chrono>
using namespace std;
using namespace chrono;
class MysqlConn
{
public:
    // ��ʼ�����ݿ�����
    MysqlConn();
    // �ͷ����ݿ�����
    ~MysqlConn();
    // �������ݿ�
    bool connect(const string& user, const string& passwd, const string& dbName, const string& ip, unsigned short port = 3306) const;
    // �������ݿ�: insert, update, delete
    bool update(const string& sql) const;
    // ��ѯ���ݿ�
    bool query(const string& sql);
    // ������ѯ�õ��Ľ����
    bool next();
    // �õ�������е��ֶ�ֵ
    string value(int index) const;
    // �������
    bool transaction() const;
    // �ύ����
    bool commit() const;
    // ����ع� 
    bool rollback() const;
    // ˢ����ʼ�Ŀ���ʱ���
    void refreshAliveTime();
    // �������Ӵ�����ʱ��
    long long getAliveTime() const;
private:
    void freeResult();

public:

private:
    MYSQL* m_conn = nullptr;
    MYSQL_RES* m_result = nullptr;
    MYSQL_ROW m_row = nullptr;
    steady_clock::time_point m_alivetime;
};

