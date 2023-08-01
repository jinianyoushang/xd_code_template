#include <iostream>
#include "FileData2.h"

using namespace std;

int main() {
    Filedata main("main.cpp");
    //测试内容
//    for (int i = 0; i < main.getFileLen(); ++i){
//        cout<<main.getFileData()[i];
//    }

    Filedata filedata("FileData.h");

    Filedata main2(main);
    Filedata main21 = main2;
    Filedata main22("CMakeLists.txt");
    main22=main2;
    Filedata main3(std::move(main));
    Filedata main4("main.cpp");
    main4=std::move(main3);
    //测试内容
    for (int i = 0; i < main4.getFileLen(); ++i){
        cout<<main4.getFileData()[i];
    }


    return 0;
}