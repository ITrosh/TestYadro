#include "CellCalculator.h"

using namespace std;

void CellCalculator::calculateCells(Error& error) {
    int result;

    for (auto& [cellName, cellValue] : _tableStruct) {
        if (holds_alternative<string>(cellValue)) {
            _chainOfCellNames = cellName;
            calculateExpression(std::get<string>(cellValue), result, error);

            if (error == Error::Success) {
                _chainOfCellNames.clear();
                _tableStruct[cellName] = result;
            }
            else {
                return;
            }
        }
    }
}

void CellCalculator::calculateExpression(const string& expression, int& result, Error& error) {
    size_t beginPos = 1, endPos = 1;

    if (expression.at(endPos) == '-')
        endPos++;

    while ((expression.at(endPos) != '+') && (expression.at(endPos) != '-') &&
           (expression.at(endPos) != '*') && (expression.at(endPos) != '/'))
        endPos++;

    string arg1 = expression.substr(beginPos, endPos - beginPos);
    int firstNumber;

    if (isNumber(expression, beginPos)) {
        firstNumber = stoi(arg1);

        OperationProcess operationProcess = {expression, endPos, firstNumber, result, error};
        calculateSecondArgument(operationProcess);
    }
    else {
        auto it1 = _tableStruct.find(arg1);

        if (holds_alternative<int>(it1->second)) {
            firstNumber = std::get<int>(it1->second);

            OperationProcess operationProcess = {expression, endPos, firstNumber, result, error};
            calculateSecondArgument(operationProcess);
        }
        else {
            if (isUniqueCellName(it1->first)) {
                _chainOfCellNames += it1->first;
                calculateExpression(std::get<string>(it1->second), result, error);

                if (error != Error::Success)
                    return;

                _tableStruct[it1->first] = result;
                firstNumber = result;

                OperationProcess operationProcess = {expression, endPos, firstNumber, result, error};
                calculateSecondArgument(operationProcess);
            }
            else {
                error = Error::InterrelatedCells;
            }
        }
    }
}

void CellCalculator::doOperation(OperationProcess& operationProcess, int secondNumber) {
    switch (operationProcess.expression.at(operationProcess.pos)) {
        case '+':
            operationProcess.result = operationProcess.firstNumber + secondNumber;
            break;
        case '-':
            operationProcess.result = operationProcess.firstNumber - secondNumber;
            break;
        case '*':
            operationProcess.result = operationProcess.firstNumber * secondNumber;
            break;
        case '/':
            if (secondNumber == 0) {
                operationProcess.error = Error::DivisionByZero;
                return;
            }

            operationProcess.result = operationProcess.firstNumber / secondNumber;
            break;
    }
}

bool CellCalculator::isUniqueCellName(const string& cellName) {
    return (_chainOfCellNames.find(cellName) == string::npos);
}

bool CellCalculator::isNumber(const string& expression, size_t pos) {
    return (expression.at(pos) == '-' || (expression.at(pos) >= '0' && expression.at(pos) <= '9'));
}

void CellCalculator::calculateSecondArgument(OperationProcess& operationProcess) {
    string secondArgument = operationProcess.expression.substr(operationProcess.pos + 1, operationProcess.expression.length() - operationProcess.pos - 1);
    int secondNumber;

    if (isNumber(operationProcess.expression, operationProcess.pos + 1)) {
        secondNumber = stoi(secondArgument);
        doOperation(operationProcess, secondNumber);
    }
    else {
        auto it = _tableStruct.find(secondArgument);

        if (holds_alternative<int>(it->second)) {
            secondNumber = std::get<int>(it->second);
            doOperation(operationProcess, secondNumber);
        }
        else {
            if (isUniqueCellName(it->first)) {
                _chainOfCellNames += it->first;
                calculateExpression(std::get<string>(it->second), operationProcess.result, operationProcess.error);

                if (operationProcess.error != Error::Success)
                    return;

                _tableStruct[it->first] = operationProcess.result;
                secondNumber = operationProcess.result;
                doOperation(operationProcess, secondNumber);
            }
            else {
                operationProcess.error = Error::InterrelatedCells;
            }
        }
    }
}
