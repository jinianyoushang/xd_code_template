//
// Created by root on 6/8/23.
//
#include <cstring>
#include <iostream>
#include "Filedata.h"

Filedata::~Filedata() {
    if (fileData) {
        delete[] fileData;
    }
}

Filedata::Filedata(const std::string &fileName) {
    std::cout << "构造函数" << std::endl;
    // 打开文件
    FILE *fp = std::fopen(fileName.c_str(), "rb");
    if (!fp) {
        std::perror("fopen");
        exit(-1);
    }

    // 读取文件内容
    std::fseek(fp, 0, SEEK_END);
    long fileSize = std::ftell(fp);
    m_len = fileSize;
    std::rewind(fp);
    fileData = new char[fileSize];
    std::fread(fileData, 1, fileSize, fp);
//    // 输出文件内容
//    std::printf("%.*s", (int)fileSize, fileData);
    // 关闭文件
    std::fclose(fp);
}


Filedata::Filedata(const Filedata &filedata) {
    std::cout << "拷贝构造函数" << std::endl;
    fileData = new char[filedata.m_len];
    memcpy(fileData, filedata.fileData, filedata.m_len);
    m_len = filedata.m_len;
}

Filedata &Filedata::operator=(const Filedata &filedata) {
    std::cout << "拷贝构造函数赋值" << std::endl;
    if (this == &filedata) {
        return *this;
    }
    delete[]fileData;
    fileData = new char[filedata.m_len];
    memcpy(fileData, filedata.fileData, filedata.m_len);
    m_len = filedata.m_len;
    return *this;
}

//移动构造
Filedata::Filedata(Filedata &&filedata) noexcept: fileData(filedata.fileData), m_len(filedata.m_len) {
    std::cout << "移动构造函数" << std::endl;
    filedata.fileData = nullptr;
    filedata.m_len = 0;
}

Filedata &Filedata::operator=(Filedata &&filedata) noexcept {
    std::cout << "移动构造函数赋值" << std::endl;
    if (this != &filedata) {//防止自我移动
        if (this->fileData) {
            delete[] this->fileData;
            this->fileData = filedata.fileData;
            this->m_len = filedata.m_len;
            filedata.m_len = 0;
            filedata.fileData = nullptr;
        }
    }
    return *this;
}

char *Filedata::getFileData() const {
    return fileData;
}

size_t Filedata::getFileLen() const {
    return m_len;
}
