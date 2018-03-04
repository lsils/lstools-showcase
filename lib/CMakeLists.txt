set(ABC_USE_NAMESPACE "abc")
add_subdirectory(abc)
target_compile_definitions(libabc-pic PUBLIC "LIN64")

add_subdirectory(alice)
add_subdirectory(kitty)
