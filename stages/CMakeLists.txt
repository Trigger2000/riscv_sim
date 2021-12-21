set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wextra -g")

add_library(stages SHARED 
            stage_decode.cpp
            stage_execute.cpp
            stage_fetch.cpp
            stage_memory.cpp
            stage_writeback.cpp
            stage.cpp
            )


# target_include_directories(stages ...)
# target_link_libraries(stages ...)