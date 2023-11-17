
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")



add_executable(threadpool_BlockingConcurrentQueue main.cpp
        TaskInterface.h
        Task.h
        Task.cpp
        Threadpool.cpp
)

include_directories(../01-concurrentqueue-master)