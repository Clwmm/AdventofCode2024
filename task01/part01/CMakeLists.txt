SET(EXEC_NAME task01_p01)

add_executable(${EXEC_NAME} main.cpp)

add_custom_command(
        TARGET ${EXEC_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/input
        $<TARGET_FILE_DIR:${EXEC_NAME}>/input
)