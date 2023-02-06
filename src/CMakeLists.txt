add_subdirectory(file_handler)
add_subdirectory(cell_calculator)
add_subdirectory(task_solver)
add_subdirectory(utils)

set(PROJECT_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE TaskSolver)
