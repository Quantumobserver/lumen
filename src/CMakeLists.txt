
add_executable(Lumen main.cpp ${LUMEN_SRC_DIR}/Log/DebugSignal.cpp)

target_include_directories(Lumen PRIVATE
                           ${DEPENDENCIES_DIR}/glm/
                           ${DEPENDENCIES_DIR}/SFML/include/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(Lumen PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      #SFML::Main
                      )
