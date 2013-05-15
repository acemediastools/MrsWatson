#include <stdlib.h>
#if WINDOWS
#include <io.h>
#endif

#include "unit/TestRunner.h"
#include "logging/LogPrinter.h"

// In both the TestSuite and TestCase objects we assume that we do not need ownership of
// the strings passed in, since they should be allocated on the heap and live for the
// lifetime of the program.
TestSuite newTestSuite(char* name, TestCaseSetupFunc setup, TestCaseTeardownFunc teardown) {
  TestSuite testSuite = malloc(sizeof(TestSuiteMembers));
  testSuite->name = name;
  testSuite->numSuccess = 0;
  testSuite->numFail = 0;
  testSuite->numSkips = 0;
  testSuite->testCases = newLinkedList();
  testSuite->setup = setup;
  testSuite->teardown = teardown;
  testSuite->onlyPrintFailing = false;
  testSuite->keepFiles = false;
  return testSuite;
}

TestCase newTestCase(char* name, char* filename, int lineNumber, TestCaseExecFunc testCaseFunc) {
  TestCase testCase = malloc(sizeof(TestCaseMembers));
  testCase->name = name;
  testCase->filename = filename;
  testCase->lineNumber = lineNumber;
  testCase->testCaseFunc = testCaseFunc;
  return testCase;
}

void addTestToTestSuite(TestSuite testSuite, TestCase testCase) {
  appendItemToList(testSuite->testCases, testCase);
}

void printTestSuccess(void) {
  if(isatty(1)) {
    printToLog(COLOR_FG_GREEN, NULL, "OK");
  }
  else {
    printToLog(COLOR_RESET, NULL, "OK");
  }
  flushLog(NULL);
}

void printTestFail(void) {
  if(isatty(1)) {
    printToLog(COLOR_BG_MAROON, NULL, "FAIL");
  }
  else {
    printToLog(COLOR_RESET, NULL, "FAIL");
  }
  flushLog(NULL);
}

static void _printTestSkipped(void) {
  if(isatty(1)) {
    printToLog(COLOR_FG_YELLOW, NULL, "Skipped");
  }
  else {
    printToLog(COLOR_RESET, NULL, "Skipped");
  }
  flushLog(NULL);
}

void printTestName(const char* testName) {
  fprintf(stderr, "  %s: ", testName);
  // Flush standard output in case the test crashes. That way at least the
  // crashing test name is seen.
  fflush(stderr);
}

void runTestCase(void* item, void* extraData) {
  TestCase testCase = (TestCase)item;
  TestSuite testSuite = (TestSuite)extraData;
  int result;
  if(!testSuite->onlyPrintFailing) {
    printTestName(testCase->name);
  }

  if(testCase->testCaseFunc != NULL) {
    if(testSuite->setup != NULL) {
      testSuite->setup();
    }
    result = testCase->testCaseFunc();
    if(result == 0) {
      if(!testSuite->onlyPrintFailing) {
        printTestSuccess();
      }
      testSuite->numSuccess++;
    }
    else {
      printTestFail();
      testSuite->numFail++;
    }

    if(testSuite->teardown != NULL) {
      testSuite->teardown();
    }
  }
  else {
    if(!testSuite->onlyPrintFailing) {
      _printTestSkipped();
    }
    testSuite->numSkips++;
  }
}

void runTestSuite(void* testSuitePtr, void* extraData) {
  TestSuite testSuite = (TestSuite)testSuitePtr;

  printToLog(COLOR_RESET, NULL, "Running tests in ");
  if(isatty(1)) {
    printToLog(COLOR_FG_CYAN, NULL, testSuite->name);
  }
  else {
    printToLog(COLOR_RESET, NULL, testSuite->name);
  }
  flushLog(NULL);

  foreachItemInList(testSuite->testCases, runTestCase, testSuite);
}
