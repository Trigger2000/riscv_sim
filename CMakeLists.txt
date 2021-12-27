cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror" CACHE STRING "Default CXX options" FORCE)
set(CMAKE_CXX_STANDARD 20)

project(simulator CXX)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(elements)
add_subdirectory(stages)
add_subdirectory(utils)

add_executable(sim simulation.cpp sim.cpp)

target_include_directories(sim PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(sim utils)
target_link_libraries(sim stages)
target_link_libraries(sim elements)

set(TEST_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/tests)

set(TESTS_PIPELINE 
    ${TEST_FOLDER}/tests_pipeline.cpp
    )

add_executable(tests_pipeline ${TESTS_PIPELINE})
target_link_libraries(tests_pipeline utils stages elements ${GTEST_LIBRARIES} pthread)
target_include_directories(tests_pipeline PRIVATE ${PROJECT_SOURCE_DIR})
