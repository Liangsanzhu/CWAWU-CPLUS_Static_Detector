#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm-c/Target.h>
#include <llvm/Support/CommandLine.h>

#include "checkers/TemplateChecker.h"
#include "framework/Common.h"
#include "framework/Config.h"

using namespace clang;
using namespace llvm;
using namespace clang::tooling;

int main(int argc, const char *argv[]) {
  LLVMInitializeNativeTarget();
  LLVMInitializeNativeAsmParser();

  std::vector<std::string> ASTs = std::move(initialize(argv[1]));

  // TODO
  Config configure(argv[2]);
  // auto enable = configure.getOptionBlock("CheckerEnable");

  TemplateChecker *tc = new TemplateChecker(ASTs, configure);
  tc->check();

  return 0;
}