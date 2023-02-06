add_library(CellCalculator STATIC
    ${CMAKE_CURRENT_LIST_DIR}/CellCalculator.cpp
)

target_include_directories(CellCalculator PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(CellCalculator PUBLIC Utils)
