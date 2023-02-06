#ifndef YADROTASK_FILEHANDLER_H
#define YADROTASK_FILEHANDLER_H

#include "ParsingData.h"
#include <fstream>

class FileHandler {
public:
    FileHandler(const std::string& fileName, ParsingFileData& parsingFileData);

private:
    static constexpr char DELIMITER = ',';
    std::vector<std::string> _keys;

    static void parseTitle(const std::string& title, std::vector<std::string>& columnNames, Error& parsingError);
    static bool isCorrectColumnName(const std::string& columnName);

    void parseTableLines(std::ifstream&& csvFile, const std::vector<std::string>& columnNames, TableLines& tableLines);
    static bool isCorrectLineNumber(const std::string& lineNumber);
    static bool isCorrectCellExpression(const std::string& stringCellValue,
                                        std::variant<std::string,int>& parsingValue,
                                        const std::vector<std::string>& keys);

    static bool isNumber(const std::string& stringCellValue);
    static bool isCorrectExpression(const std::string& stringCellValue, const std::vector<std::string>& keys);
    static bool isCorrectCellName(const std::string& argument, const std::vector<std::string>& keys);
    void fillContainer(table_struct_t& tableStruct, const std::vector<std::string>& cellValues, Error& parsingError);
};

#endif //YADROTASK_FILEHANDLER_H
