

add_executable(TestCellularAutomata TestCellularAutomata.cpp)
target_include_directories(TestCellularAutomata PRIVATE
                           ${DEPENDENCIES_DIR}/SFML/include/
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(TestCellularAutomata PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      #SFML::Main
                      )

add_test(NAME TestCellularAutomata COMMAND TestCellularAutomata)
