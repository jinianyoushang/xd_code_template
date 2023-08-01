//
// Created by root on 6/8/23.
// 一个文件的内存对象 使用智能指针管理

#ifndef WEBSERVER_FILEDATA_H
#define WEBSERVER_FILEDATA_H


#include <string>
#include <ostream>
#include <memory>

class Filedata {

    std::shared_ptr<char []> m_fileData;
    size_t m_len=0;
public:
    explicit Filedata(const std::string &fileName);
    ~Filedata();

    char *getFileData() const;
    size_t getFileLen() const;
};


#endif //WEBSERVER_FILEDATA_H
