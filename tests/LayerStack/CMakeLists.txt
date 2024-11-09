
add_executable(TestLayerStack TestLayerStack.cpp)
target_include_directories(TestLayerStack PRIVATE
                           ${DEPENDENCIES_DIR}/SFML/include/
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(TestLayerStack PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      SFML::Main
                      )

add_test(NAME TestLayerStack COMMAND TestLayerStack)

