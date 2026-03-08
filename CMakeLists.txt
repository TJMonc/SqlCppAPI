cmake_minimum_required(VERSION 3.21)
project(sqlite3cpp)

set(SQLiteAPI_DIR deps/SqlCAPI/)
add_subdirectory(${SQLiteAPI_DIR})

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/include)
set(LIBRARY_DIRECTORIES ${SQLiteAPI_DIR}/bin)
set(LINK_LIBRARIES sqlite3)
set(COMPILE_OPTIONS -std=c++20 -Werror -O3)
set(EXECUTABLE_OUTPUT_PATH ${SOURCE_DIR}/bin)


file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTIONS})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRECTORIES})
target_link_directories(${PROJECT_NAME} PRIVATE ${LIBRARY_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})
