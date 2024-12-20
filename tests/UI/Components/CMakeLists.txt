add_executable(TestButton TestButton.cpp)
target_include_directories(TestButton PRIVATE
                           ${DEPENDENCIES_DIR}/SFML/include/
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(TestButton PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      SFML::Main
                      )

add_test(NAME TestButton COMMAND TestButton)


add_executable(TestMenu TestMenu.cpp)
target_include_directories(TestMenu PRIVATE
                           ${DEPENDENCIES_DIR}/SFML/include/
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(TestMenu PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      SFML::Main
                      )

add_test(NAME TestMenu COMMAND TestMenu)