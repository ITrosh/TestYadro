#ifndef YADROTASK_CELLCALCULATOR_H
#define YADROTASK_CELLCALCULATOR_H

#include "ParsingData.h"

struct OperationProcess {
    const std::string& expression;
    size_t pos;
    int firstNumber;
    int& result;
    Error& error;
};

class CellCalculator {
public:
    explicit CellCalculator(table_struct_t& tableStruct)
                            : _tableStruct(tableStruct) {}

    void calculateCells(Error& error);

private:
    table_struct_t& _tableStruct;
    std::string _chainOfCellNames;

    void calculateExpression(const std::string& expression, int& result, Error& error);
    static void doOperation(OperationProcess& operationProcess, int secondNumber);

    void calculateSecondArgument(OperationProcess& operationProcess);

    bool isUniqueCellName(const std::string& cellName);
    static bool isNumber(const std::string& expression, size_t pos);
};


#endif //YADROTASK_CELLCALCULATOR_H
