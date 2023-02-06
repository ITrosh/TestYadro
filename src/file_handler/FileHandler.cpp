#include "FileHandler.h"
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace std;

FileHandler::FileHandler(const string& fileName, ParsingFileData& parsingFileData) {
    size_t dotPos = fileName.find_last_of('.');
    if (dotPos != string::npos && fileName.substr(dotPos + 1, fileName.length() - dotPos - 1) != "csv") {
        parsingFileData.tableLines.error = Error::IncorrectFileExtension;
        return;
    }

    ifstream csvFile(fileName);
    
    if (csvFile.is_open()) {
        string title;
        
        getline(csvFile, title);
        parseTitle(title, parsingFileData.columnNames, parsingFileData.tableLines.error);
        
        if (parsingFileData.tableLines.error != Error::Success) {
            csvFile.close();
            return;
        }

        parseTableLines(std::move(csvFile), parsingFileData.columnNames, parsingFileData.tableLines);
        if (parsingFileData.tableLines.error != Error::Success)
            return;

        fillContainer(parsingFileData.tableStruct, parsingFileData.tableLines.cellValues,
                      parsingFileData.tableLines.error);
    } else {
        parsingFileData.tableLines.error = Error::FileIsNotOpen;
    }
}

void FileHandler::parseTitle(const string& title, vector<string>& columnNames, Error& parsingError) {
    if (title.empty() || title.at(title.length() - 1) == ',') {
        parsingError = Error::IncorrectTitle;
        return;
    }

    stringstream titleStream(title);
    string temp;
    getline(titleStream, temp, DELIMITER);

    if (temp.empty()) {
        while (getline(titleStream, temp, DELIMITER)) {
            if (isCorrectColumnName(temp)) {
                columnNames.push_back(std::move(temp));
            }
            else {
                columnNames.clear();
                parsingError = Error::IncorrectTitle;
                return;
            }
        }
    }
    else {
        parsingError = Error::IncorrectTitle;
    }
}

bool FileHandler::isCorrectColumnName(const string& columnName) {
    return !columnName.empty() && std::ranges::all_of(columnName, [](char symbol) {
        return (symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z');
    });
}

void FileHandler::parseTableLines(ifstream&& csvFile, const vector<string>& columnNames, TableLines& tableLines) {
    string line;

    while (getline(csvFile, line)) {
        if (line.at(line.length() - 1) == ',') {
            tableLines.error = Error::IncorrectCellExpression;
            csvFile.close();
            return;
        }

        stringstream lineStream(line);
        string lineNumber;
        
        getline(lineStream, lineNumber, DELIMITER);
        
        if (isCorrectLineNumber(lineNumber)) {
            tableLines.lineNumbers.push_back(lineNumber);
            string cellValue;
            size_t i = 0;
            
            while (getline(lineStream, cellValue, DELIMITER)) {
                if (i == columnNames.size()) {
                    tableLines.error = Error::NumberOfCellsMore;
                    csvFile.close();
                    return;
                }
                else {
                    _keys.push_back(columnNames[i] + lineNumber);
                    tableLines.cellValues.push_back(std::move(cellValue));
                    i++;
                }
            }
            if (i < columnNames.size()) {
                tableLines.error = Error::NumberOfCellsLess;
                csvFile.close();
                return;
            }
        }
        else {
            tableLines.error = Error::IncorrectLineNumber;
            csvFile.close();
            return;
        }
    }

    csvFile.close();
}

bool FileHandler::isCorrectLineNumber(const string& lineNumber) {
    if (!lineNumber.empty() && lineNumber.at(0) >= '1' && lineNumber.at(0) <= '9') {
        return std::all_of(lineNumber.begin() + 1, lineNumber.end(), [](char symbol) {
            return symbol >= '0' && symbol <= '9';
        });
    }

    return false;
}

void FileHandler::fillContainer(table_struct_t& tableStruct, const vector<string>& cellValues, Error& parsingError) {
    size_t i = 0;
    variant<string,int> parsingValue;

    for (const string& cellValue : cellValues) {
        if (isCorrectCellExpression(cellValue, parsingValue, _keys)) {
            tableStruct[_keys.at(i)] = parsingValue;
            i++;
        }
        else {
            parsingError = Error::IncorrectCellExpression;
            return;
        }
    }
}

bool FileHandler::isCorrectCellExpression(const string& stringCellValue, variant<string, int>& parsingValue,
                                          const vector<string>& keys) {
    if (stringCellValue.empty())
        return false;

    if (stringCellValue.at(0) >= '0' && stringCellValue.at(0) <= '9' || stringCellValue.at(0) == '-') {
        if (isNumber(stringCellValue))
            parsingValue = stoi(stringCellValue);
        else
            return false;
    }
    else if (stringCellValue.length() > 1 && stringCellValue.at(0) == '=') {
        if (isCorrectExpression(stringCellValue, keys))
            parsingValue = stringCellValue;
        else
            return false;
    }
    else {
        return false;
    }

    return true;
}

bool FileHandler::isNumber(const string& stringCellValue) {
    if (stringCellValue.length() == 1) {
        if (stringCellValue.at(0) >= '0' && stringCellValue.at(0) <= '9')
            return true;
        else
            return false;
    }
    else {
        size_t pos = 0;

        if (stringCellValue.at(pos) == '-') {
            if (stringCellValue.at(pos + 1) >= '1' && stringCellValue.at(pos + 1) <= '9')
                pos++;
            else
                return false;
        }
        else if (stringCellValue.at(pos) >= '1' && stringCellValue.at(pos) <= '9') {
            pos++;
        }
        else {
            return false;
        }

        while (pos != stringCellValue.length()) {
            if (stringCellValue.at(pos) >= '0' && stringCellValue.at(pos) <= '9')
                pos++;
            else
                return false;
        }

        return true;
    }
}

bool FileHandler::isCorrectExpression(const string& stringCellValue, const vector<string>& keys) {
    size_t beginPos = 1, endPos = 1;
    bool isFindOperation = false;

    if (stringCellValue.at(endPos) == '-')
        endPos++;

    while (endPos != stringCellValue.length()) {
        if (stringCellValue.at(endPos) == '+' || stringCellValue.at(endPos) == '-' ||
            stringCellValue.at(endPos) == '*' || stringCellValue.at(endPos) == '/') {
            isFindOperation = true;
            break;
        }

        endPos++;
    }

    if (isFindOperation) {
        string arg1 = stringCellValue.substr(beginPos, endPos - beginPos);
        string arg2 = stringCellValue.substr(endPos + 1, stringCellValue.size() - endPos - 1);

        if (isNumber(arg1) || isCorrectCellName(arg1, keys)) {
            if (isNumber(arg2) || isCorrectCellName(arg2, keys))
                return true;
            else
                return false;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool FileHandler::isCorrectCellName(const string& argument, const vector<string>& keys) {
    return std::ranges::any_of(keys, [&](const string& key) { return argument == key; });
}
