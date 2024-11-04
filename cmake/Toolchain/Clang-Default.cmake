
message("ToolChain: Clang")

add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Werror)
add_compile_options("$<$<CONFIG:DEBUG>:-Og>")
add_compile_options("$<$<CONFIG:RELEASE>:-O3>")
add_link_options($<$<CONFIG:RELEASE>:-s>)
