set(ABC_USE_NAMESPACE "abc")
add_subdirectory(abc EXCLUDE_FROM_ALL)
target_compile_definitions(libabc PUBLIC "LIN64")

add_subdirectory(alice)
add_subdirectory(kitty)
add_subdirectory(lorina)


set(PERCY_BUILD_KITTY OFF CACHE BOOL "Build kitty for percy" FORCE)
add_subdirectory(percy)

add_subdirectory(tweedledum)
