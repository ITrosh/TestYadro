#ifndef YADROTASK_ERROR_H
#define YADROTASK_ERROR_H

enum class Error {
    Success,
    IncorrectQuantityOfArguments,
    IncorrectFileExtension,
    FileIsNotOpen,
    IncorrectTitle,
    IncorrectLineNumber,
    NumberOfCellsMore,
    NumberOfCellsLess,
    IncorrectCellExpression,
    InterrelatedCells,
    DivisionByZero,
};

#endif //YADROTASK_ERROR_H
