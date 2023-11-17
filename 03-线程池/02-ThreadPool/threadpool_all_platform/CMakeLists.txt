
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(threadpool_all_platform main.cpp
        TaskInterface.h
        Task.h
        Task.cpp
        Semaphore.h
        Threadpool.cpp
)