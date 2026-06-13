cmake_minimum_required(VERSION 3.21)
include(FetchContent)

option(BUILD_SHARED_LIBS "makes shared libraries" ON)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/deps)
make_directory(${FETCHCONTENT_BASE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${FETCHCONTENT_BASE_DIR}/lib)
set(COMPILE_OPTIONS -std=c++20 -O2 -Werror)


FetchContent_Declare(
    sqlite-amalgamation
    GIT_REPOSITORY https://github.com/TJMonc/sqlite-amalgamation.git
    GIT_TAG v1
)
FetchContent_MakeAvailable(sqlite-amalgamation)

project(sqlite3cpp)

set(INCLUDE_DIRECTORIES ${FETCHCONTENT_BASE_DIR}/sqlite-amalgamation-src/include ${CMAKE_SOURCE_DIR}/include)
set(LINK_DIRECTORIES ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
if(WIN32)
    set(LINKED_LIBS sqlite3.dll.a)
else()
    set(LINKED_LIBS sqlite3)
endif()

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(sqlite3cpp ${SOURCES})

target_link_directories(${PROJECT_NAME} PRIVATE ${LINK_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} ${LINKED_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRECTORIES})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTIONS})

add_dependencies(sqlite3)















