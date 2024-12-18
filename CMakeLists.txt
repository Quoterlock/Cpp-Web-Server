cmake_minimum_required(VERSION 3.22.1)
project(http_server)

add_executable(server main.cpp)
target_link_libraries(server server_modules)

add_executable(tests tests.cpp)
target_link_libraries(tests server_modules)

add_subdirectory(modules)
