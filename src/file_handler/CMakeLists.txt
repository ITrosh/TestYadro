add_library(FileHandler STATIC
    ${CMAKE_CURRENT_LIST_DIR}/FileHandler.cpp
)

target_include_directories(FileHandler PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(FileHandler PUBLIC Utils)
