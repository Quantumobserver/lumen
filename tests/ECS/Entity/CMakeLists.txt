
add_executable(TestEntityManager TestEntityManager.cpp)
target_include_directories(TestEntityManager PRIVATE
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           ${DEPENDENCIES_DIR}/SFML/include/
                           )

target_link_libraries(TestEntityManager PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      #SFML::Main
                      )

add_test(NAME TestEntityManager COMMAND TestEntityManager)
