#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm-c/Target.h>
#include <llvm/Support/CommandLine.h>
#include "checkers/TemplateCheckerMultiplePath.h"
#include "checkers/TemplateCheckerSinglePath.h"
#include "framework/Common.h"
#include "framework/Config.h"
#include "checkers/All_Detector.h"
#include<iostream>
using namespace std;
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
 if(argc==4&&strcmp(argv[3],"-m")==0)
  {
TemplateCheckerMultiplePath *tc = new TemplateCheckerMultiplePath(ASTs, configure);
  tc->check();
 Detector *dt;
  
  dt->detector();
}
else if(argc==4&&strcmp(argv[3],"-s")==0)
  {
TemplateCheckerSinglePath *tc = new TemplateCheckerSinglePath(ASTs, configure);
  tc->check();
}
else
{
cout<<"Please add Arg \"-s\" for single path or \"-m\" for multiple path!\nDefault:SinglePath"<<endl;
TemplateCheckerSinglePath *tc = new TemplateCheckerSinglePath(ASTs, configure);
  tc->check();
}	

  return 0;
}
