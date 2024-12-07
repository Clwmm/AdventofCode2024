SET(EXEC_NAME task07_p02)

include_directories(/usr/include) # Adjust path as found
link_directories(/usr/lib/x86_64-linux-gnu) # Adjust path as found

add_executable(${EXEC_NAME} main.cpp)


target_link_libraries(${EXEC_NAME} gmp gmpxx)

add_custom_command(
        TARGET ${EXEC_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../input
        $<TARGET_FILE_DIR:${EXEC_NAME}>/input
)