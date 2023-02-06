#include "CellCalculator.h"
#include "FileHandler.h"
#include "TaskSolver.h"
#include <iostream>
#include <sstream>

using namespace std;

int TaskSolver::run(const string& fileName) {
    FileHandler fileHandler(fileName, _parsingFileData);

    if (_parsingFileData.tableLines.error != Error::Success)
        switch (_parsingFileData.tableLines.error) {
            case Error::IncorrectFileExtension:
                cout << "The file has an incorrect extension!" << endl;
                return static_cast<int>(Error::IncorrectFileExtension);
            case Error::FileIsNotOpen:
                cout << "The file is not open!" << endl;
                return static_cast<int>(Error::FileIsNotOpen);
            case Error::IncorrectTitle:
                cout << "There is an error in column name of CSV file!" << endl;
                return static_cast<int>(Error::IncorrectTitle);
            case Error::IncorrectLineNumber:
                cout << "There is an error in line number of CSV file!" << endl;
                return static_cast<int>(Error::IncorrectLineNumber);
            case Error::NumberOfCellsMore:
                cout << "The number of cells more than number of columns!" << endl;
                return static_cast<int>(Error::NumberOfCellsMore);
            case Error::NumberOfCellsLess:
                cout << "The number of cells less than number of columns!" << endl;
                return static_cast<int>(Error::NumberOfCellsLess);
            default:
                cout << "There is an incorrect expression in cell!" << endl;
                return static_cast<int>(Error::IncorrectCellExpression);
        }

    CellCalculator cellCalculator(_parsingFileData.tableStruct);
    cellCalculator.calculateCells(_parsingFileData.tableLines.error);

    if (_parsingFileData.tableLines.error != Error::Success) {
        switch (_parsingFileData.tableLines.error) {
            case Error::InterrelatedCells:
                cout << "The expressions in one or more cells refer to each other by circle!" << endl;
                return static_cast<int>(Error::InterrelatedCells);
            default:
                cout << "The second argument is zero in division operation!" << endl;
                return static_cast<int>(Error::DivisionByZero);
        }
    }

    collectResultInfo();
    cout << _result << endl;
    return static_cast<int>(_parsingFileData.tableLines.error);
}

void TaskSolver::collectResultInfo() {
    ostringstream oss;

    for (const auto& columnName : _parsingFileData.columnNames) {
        oss << "," << columnName;
    }

    oss << '\n';

    for (const auto& lineNumber: _parsingFileData.tableLines.lineNumbers) {
        oss << lineNumber;

        for (const auto& columnName : _parsingFileData.columnNames) {
                auto it = _parsingFileData.tableStruct.find(columnName + lineNumber);
                oss << "," << std::get<int>(it->second);
        }

        oss << '\n';
    }

    _result = oss.str();
}

// Artificial method for testing
int TaskSolver::getCellValue(const string& cellName) {
    auto it = _parsingFileData.tableStruct.find(cellName);
    return std::get<int>(it->second);
}
