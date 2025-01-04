add_subdirectory(Components)

add_executable(TestUIManager TestUIManager.cpp)
target_include_directories(TestUIManager PRIVATE
                           ${DEPENDENCIES_DIR}/SFML/include/
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(TestUIManager PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      #SFML::Main
                      )

add_test(NAME TestUIManager COMMAND TestUIManager)