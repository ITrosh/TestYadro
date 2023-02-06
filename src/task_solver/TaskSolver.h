#ifndef YADROTASK_TASKSOLVER_H
#define YADROTASK_TASKSOLVER_H

#include "ParsingData.h"

class TaskSolver {
public:
    TaskSolver() { _parsingFileData.tableLines.error = Error::Success; }
    int run(const std::string& fileName);

    const std::string& getResultTable() const { return _result; }
    int getCellValue(const std::string& cellName);

private:
    ParsingFileData _parsingFileData;
    std::string _result;

    void collectResultInfo();
};


#endif //YADROTASK_TASKSOLVER_H
