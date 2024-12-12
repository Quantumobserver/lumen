
add_executable(TestDrawBoundingBox TestDrawBoundingBox.cpp)
target_include_directories(TestDrawBoundingBox PRIVATE
                           ${DEPENDENCIES_DIR}/SFML/include/
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(TestDrawBoundingBox PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      SFML::Main
                      )

add_test(NAME TestDrawBoundingBox COMMAND TestDrawBoundingBox)
