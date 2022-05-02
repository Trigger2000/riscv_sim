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

set(TESTS 
    ${TEST_FOLDER}/tests_control_flow_conflict.cpp
    ${TEST_FOLDER}/tests_data_flow_conflict.cpp
    ${TEST_FOLDER}/tests_single_op.cpp
    ${TEST_FOLDER}/tests_sequence.cpp
    )

add_executable(tests ${TESTS})
target_link_libraries(tests utils stages elements ${GTEST_LIBRARIES} pthread)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR})
