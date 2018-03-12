add_subdirectory(alice)
add_subdirectory(kitty)
add_subdirectory(lorina)

set(PERCY_BUILD_KITTY OFF CACHE BOOL "Build kitty for percy" FORCE)
add_subdirectory(percy)
