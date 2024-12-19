
add_executable(TestPhysics TestPhysics.cpp)
target_include_directories(TestPhysics PRIVATE
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           ${DEPENDENCIES_DIR}/SFML/include/
                           )

target_link_libraries(TestPhysics PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      SFML::Main
                      )

add_test(NAME TestPhysics COMMAND TestPhysics)

