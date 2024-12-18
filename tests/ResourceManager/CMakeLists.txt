
add_executable(TestResourceManager TestResourceManager.cpp)
target_include_directories(TestResourceManager PRIVATE
                           ${DEPENDENCIES_DIR}/SFML/include/
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(TestResourceManager PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      SFML::Main
                      )

add_test(NAME TestResourceManager COMMAND TestResourceManager)