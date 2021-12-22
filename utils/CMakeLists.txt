set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wextra -g")

add_library(utils SHARED ins.cpp)

set(TEST_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/tests)

set(TESTS_INSTRUCTIONS 
    ${TEST_FOLDER}/tests_instructions.cpp 
    ${TEST_FOLDER}/tests_instructions_R.cpp
    ${TEST_FOLDER}/tests_instructions_I.cpp
    ${TEST_FOLDER}/tests_instructions_S.cpp
    ${TEST_FOLDER}/tests_instructions_B.cpp
    ${TEST_FOLDER}/tests_instructions_U.cpp
    ${TEST_FOLDER}/tests_instructions_J.cpp
    )

add_executable(tests_instructions ${TESTS_INSTRUCTIONS})
target_link_libraries(tests_instructions utils ${GTEST_LIBRARIES} pthread)
target_include_directories(tests_instructions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
