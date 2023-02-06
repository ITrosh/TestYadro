#include "ParsingData.h"
#include "FileHandler.h"
#include "TaskSolver.h"
#include <catch2/catch_all.hpp>
#include <source_location>
#include <filesystem>

namespace fs = std::filesystem;

constexpr const char* CURRENT_SOURCE_FILE_PATH = std::source_location::current().file_name();

constexpr const char* RESOURCE_FOLDER = "resource";
constexpr const char* TEST_OPEN_FILE_FOLDER = "test_open_file";
constexpr const char* TEST_FILE_PARSE_FOLDER = "test_file_parse";
constexpr const char* TEST_WRONG_CALCULATION_FOLDER = "test_wrong_calculation";
constexpr const char* TEST_SUCCESS_CALCULATION_FOLDER = "test_success_calculation";
constexpr const char* TEST_PRINT_FILE_RESULT_FOLDER = "test_print_file_result";

void CheckFileParsing(fs::path& testFolderPath, const std::string& filePath, Error errorType) {
    ParsingFileData parsingFileData;
    parsingFileData.tableLines.error = Error::Success;
    FileHandler fileHandler(testFolderPath.append(filePath).string(), parsingFileData);
    REQUIRE(parsingFileData.tableLines.error == errorType);
}

TEST_CASE("OpeningCsvFile", "[YadroTaskTest]") {
    auto testOpenFileResourcePath = fs::path(CURRENT_SOURCE_FILE_PATH).parent_path().append(RESOURCE_FOLDER).append(TEST_OPEN_FILE_FOLDER);

    SECTION("IncorrectFileExtension") {
        CheckFileParsing(testOpenFileResourcePath, "IncorrectFileExtension.txt", Error::IncorrectFileExtension);
    }

    SECTION("FileIsNotOpen") {
        CheckFileParsing(testOpenFileResourcePath, "NonExistentCsvFile.csv", Error::FileIsNotOpen);
    }
}

TEST_CASE("ParsingCsvFile", "[YadroTaskTest]") {
    auto testFileParseResourcePath = fs::path(CURRENT_SOURCE_FILE_PATH).parent_path().append(RESOURCE_FOLDER).append(TEST_FILE_PARSE_FOLDER);

    SECTION("EmptyFile") {
        CheckFileParsing(testFileParseResourcePath, "EmptyFile.csv", Error::IncorrectTitle);
    }

    SECTION("IncorrectTitle") {
        CheckFileParsing(testFileParseResourcePath, "IncorrectTitle.csv", Error::IncorrectTitle);
    }

    SECTION("NoColumnName") {
        CheckFileParsing(testFileParseResourcePath, "NoColumnName.csv", Error::IncorrectTitle);
    }

    SECTION("IncorrectColumnName") {
        CheckFileParsing(testFileParseResourcePath, "IncorrectColumnName.csv", Error::IncorrectTitle);
    }

    SECTION("NoLineNumber") {
        CheckFileParsing(testFileParseResourcePath, "NoLineNumber.csv", Error::IncorrectLineNumber);
    }

    SECTION("IncorrectLineNumber") {
        CheckFileParsing(testFileParseResourcePath, "IncorrectLineNumber.csv", Error::IncorrectLineNumber);
    }

    SECTION("LessNumberOfCells") {
        CheckFileParsing(testFileParseResourcePath, "LessNumberOfCells.csv", Error::NumberOfCellsLess);
    }

    SECTION("MoreNumberOfCells") {
        CheckFileParsing(testFileParseResourcePath, "MoreNumberOfCells.csv", Error::NumberOfCellsMore);
    }

    SECTION("NoCellExpression") {
        CheckFileParsing(testFileParseResourcePath, "NoCellExpression.csv", Error::IncorrectCellExpression);
    }

    SECTION("IncorrectCellExpression") {
        CheckFileParsing(testFileParseResourcePath, "IncorrectCellExpression.csv", Error::IncorrectCellExpression);
    }

    SECTION("IncorrectCellExpressionFormat") {
        CheckFileParsing(testFileParseResourcePath, "IncorrectCellExpressionFormat.csv", Error::IncorrectCellExpression);
    }

    // Example from the test task definition
    SECTION("CorrectFileParse") {
        CheckFileParsing(testFileParseResourcePath, "CorrectFileParse.csv", Error::Success);
    }
}

TEST_CASE("WrongCalculation", "[YadroTaskTest]") {
    auto testIncorrectCalculateResourcePath = fs::path(CURRENT_SOURCE_FILE_PATH).parent_path().append(RESOURCE_FOLDER).append(TEST_WRONG_CALCULATION_FOLDER);
    TaskSolver taskSolver;

    SECTION("DivisionByZero") {
        const auto taskSolverResult = taskSolver.run(testIncorrectCalculateResourcePath.append("DivisionByZero.csv").string());
        REQUIRE(taskSolverResult == static_cast<int>(Error::DivisionByZero));
    }

    SECTION("InterrelatedCells") {
        const auto taskSolverResult = taskSolver.run(testIncorrectCalculateResourcePath.append("InterrelatedCells.csv").string());
        REQUIRE(taskSolverResult == static_cast<int>(Error::InterrelatedCells));
    }
}

TEST_CASE("SuccessCalculation", "[YadroTaskTest]") {
    auto testCorrectCalculateResourcePath = fs::path(CURRENT_SOURCE_FILE_PATH).parent_path().append(RESOURCE_FOLDER).append(TEST_SUCCESS_CALCULATION_FOLDER);
    TaskSolver taskSolver;

    auto calculate = [&testCorrectCalculateResourcePath, &taskSolver](const std::string& filePath) {
        const auto taskSolverResult = taskSolver.run(testCorrectCalculateResourcePath.append(filePath).string());
        REQUIRE(taskSolverResult == static_cast<int>(Error::Success));
    };

    SECTION("DifficultExample") {
        calculate("DifficultExample.csv");

        REQUIRE(taskSolver.getCellValue("A1") == 1);
        REQUIRE(taskSolver.getCellValue("B1") == 0);
        REQUIRE(taskSolver.getCellValue("C1") == -13);
        REQUIRE(taskSolver.getCellValue("D1") == -9);

        REQUIRE(taskSolver.getCellValue("A2") == 2);
        REQUIRE(taskSolver.getCellValue("B2") == 14);
        REQUIRE(taskSolver.getCellValue("C2") == 0);
        REQUIRE(taskSolver.getCellValue("D2") == -18);

        REQUIRE(taskSolver.getCellValue("A30") == 0);
        REQUIRE(taskSolver.getCellValue("B30") == 14);
        REQUIRE(taskSolver.getCellValue("C30") == 5);
        REQUIRE(taskSolver.getCellValue("D30") == 6);

        REQUIRE(taskSolver.getCellValue("A4") == 13);
        REQUIRE(taskSolver.getCellValue("B4") == -14);
        REQUIRE(taskSolver.getCellValue("C4") == 10);
        REQUIRE(taskSolver.getCellValue("D4") == 7);
    }

    SECTION("OnlyNumbers") {
        calculate("OnlyNumbers.csv");

        REQUIRE(taskSolver.getCellValue("A1") == 1);
        REQUIRE(taskSolver.getCellValue("B1") == 0);
        REQUIRE(taskSolver.getCellValue("C1") == 7);
        REQUIRE(taskSolver.getCellValue("D1") == 10);

        REQUIRE(taskSolver.getCellValue("A2") == 2);
        REQUIRE(taskSolver.getCellValue("B2") == 15);
        REQUIRE(taskSolver.getCellValue("C2") == 0);
        REQUIRE(taskSolver.getCellValue("D2") == 1);

        REQUIRE(taskSolver.getCellValue("A30") == 0);
        REQUIRE(taskSolver.getCellValue("B30") == 6);
        REQUIRE(taskSolver.getCellValue("C30") == 5);
        REQUIRE(taskSolver.getCellValue("D30") == 3);

        REQUIRE(taskSolver.getCellValue("A4") == 1);
        REQUIRE(taskSolver.getCellValue("B4") == -8);
        REQUIRE(taskSolver.getCellValue("C4") == 10);
        REQUIRE(taskSolver.getCellValue("D4") == 2);
    }

    SECTION("Expressions") {
        calculate("Expressions.csv");

        REQUIRE(taskSolver.getCellValue("A1") == 14);
        REQUIRE(taskSolver.getCellValue("B1") == -7);
        REQUIRE(taskSolver.getCellValue("C1") == 7);
        REQUIRE(taskSolver.getCellValue("D1") == 14);

        REQUIRE(taskSolver.getCellValue("A2") == 4);
        REQUIRE(taskSolver.getCellValue("B2") == 4);
        REQUIRE(taskSolver.getCellValue("C2") == -5);
        REQUIRE(taskSolver.getCellValue("D2") == 5);

        REQUIRE(taskSolver.getCellValue("A30") == 0);
        REQUIRE(taskSolver.getCellValue("B30") == 15);
        REQUIRE(taskSolver.getCellValue("C30") == 20);
        REQUIRE(taskSolver.getCellValue("D30") == 8);

        REQUIRE(taskSolver.getCellValue("A4") == 12);
        REQUIRE(taskSolver.getCellValue("B4") == 16);
        REQUIRE(taskSolver.getCellValue("C4") == -2);
        REQUIRE(taskSolver.getCellValue("D4") == -10);
    }

    // Example from the test task definition
    SECTION("FromTestTask") {
        calculate("FromTestTask.csv");

        REQUIRE(taskSolver.getCellValue("A1") == 1);
        REQUIRE(taskSolver.getCellValue("B1") == 0);
        REQUIRE(taskSolver.getCellValue("Cell1") == 1);

        REQUIRE(taskSolver.getCellValue("A2") == 2);
        REQUIRE(taskSolver.getCellValue("B2") == 6);
        REQUIRE(taskSolver.getCellValue("Cell2") == 0);

        REQUIRE(taskSolver.getCellValue("A30") == 0);
        REQUIRE(taskSolver.getCellValue("B30") == 1);
        REQUIRE(taskSolver.getCellValue("Cell30") == 5);
    }
}

TEST_CASE("PrintFileResult", "[YadroTaskTest]") {
    auto testPrintFileResultResourcePath = fs::path(CURRENT_SOURCE_FILE_PATH).parent_path().append(RESOURCE_FOLDER).append(TEST_PRINT_FILE_RESULT_FOLDER);

    TaskSolver taskSolver;
    taskSolver.run(testPrintFileResultResourcePath.append("FromTestTask.csv").string());

    REQUIRE(taskSolver.getResultTable() == ",A,B,Cell\n"
                                           "1,1,0,1\n"
                                           "2,2,6,0\n"
                                           "30,0,1,5\n");
}
