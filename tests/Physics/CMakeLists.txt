
add_executable(TestPhysics TestPhysics.cpp)
target_include_directories(TestPhysics PRIVATE
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(TestPhysics PRIVATE
                      glm::glm
                      )

add_test(NAME TestPhysics COMMAND TestPhysics)

