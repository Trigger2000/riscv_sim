set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wextra -g")

add_library(utils SHARED ins.cpp)

add_executable(test_instructions ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_ins.cpp)
target_link_libraries(test_instructions utils ${GTEST_LIBRARIES} pthread)
target_include_directories(test_instructions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})