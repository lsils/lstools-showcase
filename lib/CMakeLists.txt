set(ABC_USE_NAMESPACE "abc")
add_subdirectory(abc EXCLUDE_FROM_ALL)
target_compile_definitions(libabc PUBLIC "LIN64")

add_subdirectory(mockturtle)

add_subdirectory(alice)
if (NOT TARGET kitty)
add_subdirectory(kitty)
endif()
if (NOT TARGET lorina)
add_subdirectory(lorina)
endif()

if (NOT TARGET percy)
set(PERCY_BUILD_KITTY OFF CACHE BOOL "Build kitty for percy" FORCE)
add_subdirectory(percy)
endif()

add_subdirectory(tweedledum)
