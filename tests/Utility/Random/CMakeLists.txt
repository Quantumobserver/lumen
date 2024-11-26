
add_executable(TestRandomSeedGenerator TestRandomSeedGenerator.cpp ${LUMEN_SRC_DIR}/Log/DebugSignal.cpp)

target_include_directories(TestRandomSeedGenerator PRIVATE
                           ${LUMEN_SRC_DIR}/
                           )

add_test(NAME TestRandomSeedGenerator COMMAND TestRandomSeedGenerator)

##########################################################
add_executable(TestLehmer TestLehmer.cpp)

target_include_directories(TestLehmer PRIVATE
                           ${LUMEN_SRC_DIR}/
                           )

add_test(NAME TestLehmer COMMAND TestLehmer)