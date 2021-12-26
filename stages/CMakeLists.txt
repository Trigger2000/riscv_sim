set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wextra -g")

add_library(stages SHARED 
            stage_decode.cpp
            stage_execute.cpp
            stage_fetch.cpp
            stage_memory.cpp
            stage_writeback.cpp
            )

set(TEST_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/tests)

set(TESTS_STAGES 
    ${TEST_FOLDER}/tests_decode.cpp 
    ${TEST_FOLDER}/tests_execute.cpp
    ${TEST_FOLDER}/tests_fetch.cpp
    ${TEST_FOLDER}/tests_memory.cpp
    ${TEST_FOLDER}/tests_stages.cpp
    ${TEST_FOLDER}/tests_writeback.cpp
    )

add_executable(tests_stages ${TESTS_STAGES})
target_include_directories(tests_stages PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(tests_stages stages ${GTEST_LIBRARIES} pthread)
