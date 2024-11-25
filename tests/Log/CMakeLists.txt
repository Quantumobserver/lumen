
add_executable(TestDebugLog TestDebugLog.cpp)
target_include_directories(TestDebugLog PRIVATE
                           ${LUMEN_SRC_DIR}/
                           )

add_test(NAME TestDebugLog COMMAND TestDebugLog)