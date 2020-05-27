#include "checkers/All_Detector.h"
#include "checkers/Memory_Leak_Detector.h"
#include "checkers/Detector.h"
/*step0:建立自己的.h，在自己的.h里面加入Detector.h的报错*/
/*step1:添加自己的.h*/


SourceManager*srcMgr=NULL;
extern defuse_node all_node;
std::map<FunctionDecl *, std::map<int, std::vector<CFGBlock *>>> path_tree;

//I modified the kuangjia a little zhujiao gave us, save all the cfg&FunctionDecl into the fowllowing cfg_path.
std::map<FunctionDecl *, std::unique_ptr<clang::CFG>> cfg_path;

void Detector::detector() {
  std::cout << "Detector Funtion: " << "\n";
  for (auto fd = path_tree.begin(); fd != path_tree.end(); ++fd) {
    //function
    if(srcMgr==NULL)
     srcMgr=&(fd->first->getASTContext().getSourceManager());
      
    //you can get SourceManager from FunctionDecl(fd) here.
    std::cout << "FunctionName: " << (*fd).first->getQualifiedNameAsString() << "\n";
    for (auto idx = (*fd).second.begin(); idx != (*fd).second.end(); ++idx) {
      //path index
      /*step2:在每一个分支下，创建自己的对象*/
      Memory_Leak_Detector MLD;

      int path_idx = (*idx).first;
      std::cout << "path: " << path_idx << "\n";
      auto i = cfg_path[(*fd).first]->end();
      for (auto cfg_bk = ++((*idx).second.begin()); cfg_bk != (*idx).second.end(); ++cfg_bk) {
          //cfg_block of the path
          CFGBlock *cur_bk = (*cfg_bk);
          for (auto i_bk = cur_bk->begin(); i_bk != cur_bk->end(); ++i_bk) {
            if (Optional<CFGStmt> CS = (*i_bk).getAs<CFGStmt>()) {
              Stmt *S = const_cast<Stmt *>(CS->getStmt());
              assert(S != nullptr && "Expecting non-null Stmt");
              std::cout << S->getStmtClassName() << "\n";
               MLD.ML_Entry(*srcMgr,S,path_idx,&all_node);

              /*
              step3:调用入口函数
              for every statement.
              add your detect here.
              */
            }
          }
          
      }
      /*step4:调用检测函数*/
      MLD.ML_Detect();
    }
  }
  
  Get_SourceCode(*srcMgr);//获取源码
   print_result();//打印结果
}

