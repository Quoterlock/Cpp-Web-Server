add_library(server_modules STATIC
    httpParser.cpp 
    router.cpp
    logger.cpp
    server.cpp
    pagesManager.cpp
    staticFilesManager.cpp)
    
target_include_directories(server_modules
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})