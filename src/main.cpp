#include "TaskSolver.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    if (argc == 2) {
        TaskSolver taskSolver;
        return taskSolver.run(argv[argc - 1]);
    }
    else {
        cout << "The number of command line arguments is less or more than required!" << endl;
        return static_cast<int>(Error::IncorrectQuantityOfArguments);
    }
}
