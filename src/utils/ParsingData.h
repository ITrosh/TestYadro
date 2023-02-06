#ifndef YADROTASK_PARSINGDATA_H
#define YADROTASK_PARSINGDATA_H

#include "Error.h"
#include <map>
#include <string>
#include <variant>
#include <vector>

using table_struct_t = std::map<std::string, std::variant<std::string, int>>;

struct TableLines {
    std::vector<std::string> lineNumbers;
    std::vector<std::string> cellValues;
    Error error;
};

struct ParsingFileData {
    std::vector<std::string> columnNames;
    table_struct_t tableStruct;
    TableLines tableLines;
};

#endif //YADROTASK_PARSINGDATA_H
