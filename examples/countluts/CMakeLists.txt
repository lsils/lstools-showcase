add_executable(countluts countluts.cpp)
target_link_libraries(countluts kitty lorina)
target_compile_options(countluts PUBLIC -Wall -Wextra)
