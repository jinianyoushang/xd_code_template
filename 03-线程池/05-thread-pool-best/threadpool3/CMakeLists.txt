add_library(threadpool3
        STATIC
        WorkThread.cpp
        ThreadPool.cpp
)

if (NOT WIN32)
    target_link_libraries(threadpool3 pthread)
endif ()
