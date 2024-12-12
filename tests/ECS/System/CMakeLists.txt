
add_subdirectory(Debug)

add_executable(TestMovement TestMovement.cpp)
target_include_directories(TestMovement PRIVATE
                           ${DEPENDENCIES_DIR}/SFML/include/
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(TestMovement PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      SFML::Main
                      )

add_test(NAME TestMovement COMMAND TestMovement)

##############################################################
add_executable(TestLifespanUpdate TestLifespanUpdate.cpp)
target_include_directories(TestLifespanUpdate PRIVATE
                           ${DEPENDENCIES_DIR}/SFML/include/
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(TestLifespanUpdate PRIVATE
                      glm::glm
                      SFML::Window
                      SFML::Graphics
                      SFML::Main
                      )

add_test(NAME TestLifespanUpdate COMMAND TestLifespanUpdate)
