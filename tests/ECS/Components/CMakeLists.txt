
add_executable(TestAnimation TestAnimation.cpp)
target_include_directories(TestAnimation PRIVATE
                           ${DEPENDENCIES_DIR}/SFML/include/
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(TestAnimation PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      #SFML::Main
                      )

add_test(NAME TestAnimation COMMAND TestAnimation)