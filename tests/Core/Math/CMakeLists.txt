
add_executable(TestVector TestVector.cpp)
target_include_directories(TestVector PRIVATE
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(TestVector PRIVATE
                      glm::glm
                      )

add_test(NAME TestVector COMMAND TestVector)
