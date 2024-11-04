
message("ToolChain: Gnu")

add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Werror)
add_compile_options("$<$<CONFIG:DEBUG>:-Og>")
add_compile_options("$<$<CONFIG:RELEASE>:-O3>")
add_link_options($<$<CONFIG:RELEASE>:-s>)

#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -Bstatic -lpthread")
