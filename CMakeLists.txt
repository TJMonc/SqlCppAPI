cmake_minimum_required(VERSION 3.21)
include(FetchContent)

option(BUILD_SHARED_LIBS "makes shared libraries" ON)
set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/deps)
make_directory(${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(COMPILE_OPTIONS -std=c++20 -O2 -Werror -Wno-free-nonheap-object)


FetchContent_Declare(
    sqlite-amalgamation
    GIT_REPOSITORY https://github.com/TJMonc/sqlite-amalgamation.git
    GIT_TAG v1
)
FetchContent_MakeAvailable(sqlite-amalgamation)

project(sqlite3cpp)

set(INCLUDE_DIRECTORIES ${FETCHCONTENT_BASE_DIR}/sqlite-amalgamation-src/include ${CMAKE_SOURCE_DIR}/include)
set(LINK_DIRECTORIES ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(LINKED_LIBS sqlite3)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_library(${PROJECT_NAME} ${SOURCES})

target_link_directories(${PROJECT_NAME} PRIVATE ${LINK_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} ${LINKED_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRECTORIES})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTIONS})


set(LINKED_LIBS sqlite3 sqlite3cpp)
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/test_src/*.cpp)
add_executable(${PROJECT_NAME}_test ${SOURCES})

target_link_directories(${PROJECT_NAME}_test PRIVATE ${LINK_DIRECTORIES})
target_link_libraries(${PROJECT_NAME}_test ${LINKED_LIBS})
target_include_directories(${PROJECT_NAME}_test PRIVATE ${INCLUDE_DIRECTORIES})
target_compile_options(${PROJECT_NAME}_test PRIVATE ${COMPILE_OPTIONS})

add_dependencies(${PROJECT_NAME} sqlite3)
add_dependencies(${PROJECT_NAME}_test ${PROJECT_NAME})
