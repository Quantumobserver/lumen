
add_executable(Lumen main.cpp)

target_include_directories(Lumen PRIVATE
                           ${DEPENDENCIES_DIR}/glm/
                           ${DEPENDENCIES_DIR}/SFML/include/
                           )

target_link_libraries(Lumen PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      SFML::Main
                      )
