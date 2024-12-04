SET(EXEC_NAME task04_p02)

add_executable(${EXEC_NAME} main.cpp)

add_custom_command(
        TARGET ${EXEC_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/task04/part02/input
        $<TARGET_FILE_DIR:${EXEC_NAME}>/input
)