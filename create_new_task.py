import os

def count_task_folders(directory):
    if not os.path.isdir(directory):
        raise ValueError(f"The provided path '{directory}' is not a valid directory.")

    # Count folders starting with 'task'
    task_folders = [
        folder for folder in os.listdir(directory)
        if folder.startswith("task") and os.path.isdir(os.path.join(directory, folder))
    ]

    return len(task_folders)

def create_task_folder(directory, task_num):
    task_folder_name = f"task{task_num}"
    task_folder_path = os.path.join(directory, task_folder_name)

    sample_cpp_path = os.path.join(directory, "template.cpp")
    if not os.path.exists(sample_cpp_path):
        raise FileNotFoundError(f"The sample file '{sample_cpp_path}' does not exist.")

    # Create the task folder
    os.makedirs(task_folder_path, exist_ok=True)

    # Create the CMakeLists.txt file
    cmake_file_path = os.path.join(task_folder_path, "CMakeLists.txt")
    with open(cmake_file_path, "w") as cmake_file:
        cmake_file.write("add_subdirectory(part1)\n")
        cmake_file.write("add_subdirectory(part2)\n")

    input_path = os.path.join(task_folder_path, "input")
    part1_path = os.path.join(task_folder_path, "part1")
    part2_path = os.path.join(task_folder_path, "part2")
    os.makedirs(input_path, exist_ok=True)
    os.makedirs(part1_path, exist_ok=True)
    os.makedirs(part2_path, exist_ok=True)

    input_file_path = os.path.join(input_path, "input")
    open(input_file_path, "w")

    cmake_part1_path = os.path.join(part1_path, "CMakeLists.txt")
    with open(cmake_part1_path, "w") as cmake_part1_file:
        cmake_part1_file.write(f"SET(EXEC_NAME task{task_num}_p01)\n\n")
        cmake_part1_file.write("add_executable(${EXEC_NAME} main.cpp)\n\n")
        cmake_part1_file.write("add_custom_command(\n")
        cmake_part1_file.write("        TARGET ${EXEC_NAME} POST_BUILD\n")
        cmake_part1_file.write("        COMMAND ${CMAKE_COMMAND} -E copy_directory\n")
        cmake_part1_file.write("        ${CMAKE_CURRENT_SOURCE_DIR}/../input\n")
        cmake_part1_file.write("        $<TARGET_FILE_DIR:${EXEC_NAME}>/input\n")
        cmake_part1_file.write(")\n")

    cmake_part2_path = os.path.join(part2_path, "CMakeLists.txt")
    with open(cmake_part2_path, "w") as cmake_part2_file:
        cmake_part2_file.write(f"SET(EXEC_NAME task{task_num}_p02)\n\n")
        cmake_part2_file.write("add_executable(${EXEC_NAME} main.cpp)\n\n")
        cmake_part2_file.write("add_custom_command(\n")
        cmake_part2_file.write("        TARGET ${EXEC_NAME} POST_BUILD\n")
        cmake_part2_file.write("        COMMAND ${CMAKE_COMMAND} -E copy_directory\n")
        cmake_part2_file.write("        ${CMAKE_CURRENT_SOURCE_DIR}/../input\n")
        cmake_part2_file.write("        $<TARGET_FILE_DIR:${EXEC_NAME}>/input\n")
        cmake_part2_file.write(")\n")

    main_cpp_part1_path = os.path.join(part1_path, "main.cpp")
    main_cpp_part2_path = os.path.join(part2_path, "main.cpp")

    if os.path.exists(sample_cpp_path):
        with open(sample_cpp_path, "r") as sample_cpp_file, open(main_cpp_part1_path, "w") as main_cpp_part1_file:
            main_cpp_part1_file.write(sample_cpp_file.read())
        with open(sample_cpp_path, "r") as sample_cpp_file, open(main_cpp_part2_path, "w") as main_cpp_part2_file:
            main_cpp_part2_file.write(sample_cpp_file.read())
    else:
        raise FileNotFoundError(f"The sample file '{sample_cpp_path}' does not exist.")

    return task_folder_path

def add_task_to_cmake(directory, task_num):
    cmake_file_path = os.path.join(directory, "CMakeLists.txt")
    if not os.path.exists(cmake_file_path):
        raise FileNotFoundError(f"CMakeLists.txt not found in the directory '{directory}'")

    with open(cmake_file_path, "a") as cmake_file:
        cmake_file.write(f"add_subdirectory(task{task_num})\n")

if __name__ == "__main__":
    try:
        current_directory = os.getcwd()
        taskNum = count_task_folders(current_directory) + 1
        create_task_folder(current_directory, taskNum)
        add_task_to_cmake(current_directory, taskNum)
        print(f"Created task{taskNum}")
    except Exception as e:
        print(e)
