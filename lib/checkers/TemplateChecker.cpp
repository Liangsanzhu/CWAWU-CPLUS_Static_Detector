#include "checkers/TemplateChecker.h"
#include "checkers/Memory_Leak_Detector.h"
#include "checkers/Null_Pointer_Detector.h"
#include "checkers/Var_Uninitialized.h"
#include "checkers/Out_Index.h"
#include "checkers/Detector.h"
#include "checkers/Buffer_Overflow.h"
#include <iostream>
using namespace std;

void TemplateChecker::check() {
  // for example
  Buffer_Overflow BOF;
  Var_Uninitialized VU;
  Null_Pointer_Detector NPD;
  Memory_Leak_Detector MLD;
  Out_Index OI;
  auto astr_iter = getASTRsBegin();
  VU.VU_Entry1(astr_iter);//变量未初始化
  while (astr_iter != getASTRsEnd()) {
    OI.OI_Entry(astr_iter);//数组越界
    auto fds = astr_iter->second.GetFunctionDecls();
    for (auto fd : fds) {
    
      Get_SourceCode(fd);//获取源码
      VU.VU_Entry2(fd);//变量未初始化
      NPD.NPD_Entry(fd);//空指针解引用
      MLD.ML_Entry(fd);//内存泄漏
      BOF.BOF_Entry(fd);
      }

    ++astr_iter;
  }
  OI.OI_Dectect();
  VU.VU_Detect();
  NPD.NPD_Detect();
  MLD.ML_Detect();
  print_result();//打印所有出错信息
}
