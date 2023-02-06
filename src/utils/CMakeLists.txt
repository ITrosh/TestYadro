add_library(Utils INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/Error.h
    ${CMAKE_CURRENT_LIST_DIR}/ParsingData.h
)

target_include_directories(Utils INTERFACE ${CMAKE_CURRENT_LIST_DIR})
