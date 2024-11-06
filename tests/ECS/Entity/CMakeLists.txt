
add_executable(TestEntityManager TestEntityManager.cpp)
target_include_directories(TestEntityManager PRIVATE
                           ${DEPENDENCIES_DIR}/glm/
                           ${LUMEN_SRC_DIR}/
                           )

target_link_libraries(TestEntityManager PRIVATE
                      glm::glm
                      )

add_test(NAME TestEntityManager COMMAND TestEntityManager)
