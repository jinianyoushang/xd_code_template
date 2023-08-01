cmake_minimum_required(VERSION 3.26)
set(VCPKG_ROOT "C:/softwares/vcpkg-master")            # 手动设置到你的vcpkg根目录
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
project(clion_demo)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
set(CMAKE_CXX_STANDARD 17)

add_executable(clion_demo main.cpp)

#这里添加包
find_package(OpenSSL REQUIRED)
target_link_libraries(clion_demo PRIVATE OpenSSL::SSL OpenSSL::Crypto)
