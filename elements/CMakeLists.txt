set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wextra -g")

add_library(elements SHARED 
            dmem.cpp
            imem.cpp
            mux.cpp
            regfile.cpp
            hazard_unit.cpp
            branch_predictor.cpp
            )

target_include_directories(elements PRIVATE ${PROJECT_SOURCE_DIR})

# target_include_directories(elements ...)
# target_link_libraries(elements ...)
