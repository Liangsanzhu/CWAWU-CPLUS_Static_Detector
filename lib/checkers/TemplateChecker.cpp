#include "checkers/TemplateChecker.h"
#include "checkers/Memory_Leak_Detector.h"
#include "checkers/Null_Pointer_Detector.h"
#include "checkers/Var_Uninitialized.h"
#include "checkers/Out_Index.h"
#include "checkers/Detector.h"
#include <iostream>
using namespace std;

void TemplateChecker::check() {
  // for example
  auto astr_iter = getASTRsBegin();
  VU_Entry1(astr_iter);//变量未初始化
  while (astr_iter != getASTRsEnd()) {
    OI_Entry(astr_iter);//数组越界
    auto fds = astr_iter->second.GetFunctionDecls();
    for (auto fd : fds) {
      
      Get_SourceCode(fd);//获取源码
      VU_Entry2(fd);//变量未初始化
      NPD_Entry(fd);//空指针解引用
      ML_Entry(fd);//内存泄漏
      }

    ++astr_iter;
  }
  OI_Dectect();
  VU_Detect();
  NPD_Detect();
  ML_Detect();
  print_result();//打印所有出错信息
}
