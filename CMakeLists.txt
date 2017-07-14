cmake_minimum_required(VERSION 3.5)
project(example_protobuf)

set(CMAKE_CXX_STANDARD 14)

set(TARGET_CLIENT example_protobuf_client)

# Need this for libasio
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)

set(PROTO_IN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/proto/example.proto)
message(STATUS "PROTO_IN_FILE: " ${PROTO_IN_FILE})

execute_process(COMMAND protoc -I=${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${CMAKE_CURRENT_SOURCE_DIR} ${PROTO_IN_FILE}
                ERROR_VARIABLE ERR)

if (ERR)
    message(WARNING ${ERR})
endif(ERR)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE_FILES_CLIENT client.cpp)

add_executable(${TARGET_CLIENT}
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/example.pb.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/example.pb.h
        ${SOURCE_FILES_CLIENT})

target_link_libraries(${TARGET_CLIENT} ${CMAKE_THREAD_LIBS_INIT} ${PROTOBUF_LIBRARY})

get_property(PROP_VAL TARGET ${TARGET_CLIENT} PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "INCLUDE_DIRECTORIES: " ${PROP_VAL})

add_executable(example_protobuf_server
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/example.pb.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/example.pb.h
        server.cpp)

target_link_libraries(example_protobuf_server ${CMAKE_THREAD_LIBS_INIT} ${PROTOBUF_LIBRARY})