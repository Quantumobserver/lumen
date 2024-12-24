
add_executable(TestActionManager TestActionManager.cpp)
target_include_directories(TestActionManager PRIVATE
                           ${DEPENDENCIES_DIR}/SFML/include/
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(TestActionManager PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      #SFML::Main
                      )

add_test(NAME TestActionManager COMMAND TestActionManager)

