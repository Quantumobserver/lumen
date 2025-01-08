
add_executable(TestReadJson TestReadJson.cpp ${LUMEN_SRC_DIR}/Log/DebugSignal.cpp
                            ${LUMEN_SRC_DIR}/Utility/Json/JsonDef.cpp)
target_include_directories(TestReadJson PRIVATE
                           ${DEPENDENCIES_DIR}/SFML/include/
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(TestReadJson PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      #SFML::Main
                      )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME TestReadJson COMMAND TestReadJson)