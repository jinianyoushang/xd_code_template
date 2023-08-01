//
// Created by root on 6/8/23.
// 一个文件的内存对象

#ifndef WEBSERVER_FILEDATA_H
#define WEBSERVER_FILEDATA_H


#include <string>
#include <ostream>

class Filedata {
    char *fileData = nullptr;
    size_t m_len=0;
public:
    explicit Filedata(const std::string &fileName);

    Filedata(const Filedata &filedata);

    Filedata(Filedata &&filedata) noexcept;

    Filedata &operator=(const Filedata &filedata);

    Filedata &operator=(Filedata &&filedata) noexcept;

    ~Filedata();

    char *getFileData() const;
    size_t getFileLen() const;
};


#endif //WEBSERVER_FILEDATA_H
