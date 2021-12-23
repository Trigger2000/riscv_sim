set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wextra -g")

add_library(elements SHARED 
            alu.cpp
            dmem.cpp
            imm.cpp
            mux.cpp
            Regfile.cpp
            )

# target_include_directories(elements ...)
# target_link_libraries(elements ...)
