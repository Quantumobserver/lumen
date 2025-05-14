
add_executable(AssetGenerator AssetGenerator.cpp ${LUMEN_SRC_DIR}/Log/DebugSignal.cpp)
target_include_directories(AssetGenerator PRIVATE
                           ${DEPENDENCIES_DIR}/SFML/include/
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(AssetGenerator PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      #SFML::Main
                      )

