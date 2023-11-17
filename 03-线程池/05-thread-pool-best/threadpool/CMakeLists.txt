add_library(threadpool
        STATIC
        WorkThread.cpp
        ThreadPool.cpp
)

if (NOT WIN32)
    target_link_libraries(threadpool pthread)
endif ()
