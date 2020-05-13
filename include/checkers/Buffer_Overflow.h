#ifndef B_O_F_H
#define B_O_F_H
#include"Detector.h"

class Buffer_Overflow
{
private:
//void handleDeclStmt(const Stmt *S);                     //处理DeclStmt
//void handleCallExpr(const Stmt *S);                     //处理CallExpr
//int strLen(string type);                                //得到数组变量char[]的长度
//void detect_bcopy(const Stmt *S, FunctionDecl *callee); //检测bcopy()函数

map<std::string, int> Var;
map<std::string, SourceLocation> FuncLocation;



void handleDeclStmt(const Stmt *S)
{
  DeclStmt *declstmt = (DeclStmt *)S;

  auto it = declstmt->decl_begin();
  VarDecl *vdecl = static_cast<VarDecl *>(*it);
  string var_tpye = vdecl->getType().getAsString();
  string var_name = vdecl->getIdentifier()->getName().data();
  int len = strLen(var_tpye);

  //cout << "var tpye:   " << var_tpye << endl;
  //cout << "var name:   " << var_name << endl;
  //cout << "char len= " << len << endl;
  //cout << endl;

  Var.insert(pair<string, int>(var_name, len));

  for (auto i = S->child_begin(); i != S->child_end(); i++)
  {
    //std::cout << "StmtClass:  " << i->getStmtClassName() << std::endl;
    if (strcmp(i->getStmtClassName(), "IntegerLiteral") == 0)
    {
      IntegerLiteral *intltr = (IntegerLiteral *)*i;
      int int_val = intltr->getValue().getSExtValue();
      //cout << "var value: " << int_val << endl;
      Var[var_name] = int_val;
      //cout << var_name << "=" << Var[var_name] << endl;
    }

    if (strcmp(i->getStmtClassName(), "StringLiteral") == 0)
    {
      StringLiteral *strltr = (StringLiteral *)*i;
      //cout << "string data: " << strltr->getString().data() << endl;
    }
    //cout << endl;
  }
}

void handleCallExpr(const Stmt *S)
{
  CallExpr *callexpr = (CallExpr *)S;
  FunctionDecl *callee = callexpr->getDirectCallee();
  string callee_name = callee->getNameAsString();

  if (callee_name == "bcopy")
  {
    FuncLocation.insert(pair<string, SourceLocation>(callee_name, S->getBeginLoc()));
    detect_bcopy(S, callee); //检测bcopy()函数
  }
}

int strLen(string type)
{
  string t;
  int len = 0;
  for (int i = 0; i < 6; i++)
    t = t + type[i];

  if (t == "char [")
    for (int i = 6; type[i] != ']'; i++)
      len = len * 10 + type[i] - '0';
  else
    len = -1;
  return len;
}

void detect_bcopy(const Stmt *S, FunctionDecl *callee)
{
  int len_dest = -1;
  int len_src = -1;
  int len_n = -1;

  int param_order[3]; //0-src,1-dest,2-n

  auto param = callee->param_begin();
  int num = 1;
  for (; param != callee->param_end(); param++)
  {
    string param_name = (*param)->getIdentifier()->getName().data();

    //cout << "param name:   " << param_name << endl;
    //std::cout << "param type:   " << (*param)->getType().getAsString() << std::endl;
    if (param_name == "__src")
      param_order[0] = num;
    else if (param_name == "__dest")
      param_order[1] = num;
    else if (param_name == "__n")
      param_order[2] = num;
    num++;
  }
  //cout << param_order[0] << " " << param_order[1] << " " << param_order[2] << endl;
  //cout << endl;

  num = 0;
  //auto i = S->child_begin();
  for (auto i = S->child_begin(); i != S->child_end(); i++)
  {

    auto j = i;
    while (strcmp(j->getStmtClassName(), "DeclRefExpr") != 0)
    {
      j = j->child_begin();
      //cout << "j: " << j->getStmtClassName() << endl;
    }
    DeclRefExpr *declre = (DeclRefExpr *)*j;
    string param_name = declre->getNameInfo().getAsString();
    string param_type = declre->getType().getAsString();

    //cout << "param name:  " << param_name << endl;
    //cout << "param type:  " << param_type << endl;
    //cout << "num=" << num << endl;

    if (num == param_order[1])
    {
      len_dest = Var[param_name];
      //cout << "len_dest=" << len_dest << endl;
    }
    else if (num == param_order[2])
    {
      len_n = Var[param_name];
      //cout << "len_n=" << len_n << endl;
    }
    num++;
    //td::cout << std::endl;
  }

  //
  if (len_n == -1)
  {
    //warning:maybe right or wrong
  }
  else
  {
    if (len_dest <= len_n)
    {
      clang::SourceManager &srcmgr = callee->getASTContext().getSourceManager();
      //error
      //cout << "error:" << endl;
      string Func_name = callee->getNameAsString();
      const string filename = srcmgr.getFilename(FuncLocation[Func_name]).str();
      int line = srcmgr.getSpellingLineNumber(FuncLocation[Func_name]);
      int col = srcmgr.getSpellingColumnNumber(FuncLocation[Func_name]);
      string info = "Buffer overflow caused by use " + Func_name + " to copy data";
      error_info*e=new_error_info(NULL,filename,line,col,TYPE_ERROR,info);
      result.push(e);
      //cout << "Func_name=" << Func_name << endl;
      //cout << "filename=" << filename << ensdl;
      //cout << "line=" << line << endl;
      //cout << "col=" << col << endl;
      //cout << info << endl;

      /*
      error_info* e=new_error_info(NULL,filename,line,col,TYPE_ERROR,info);
       result.push(e);*/
    }
    else
    {
      //no error or warning
    }
  }
}
public:
void BOF_Entry(clang::FunctionDecl*fd){
 
      //std::cout << common::getFullName(fd) << std::endl;
      auto fd_cfg = common::buildCFG(fd);
      // Traverse CFG

      //////////////////////////////////////////////////////
      //std::cout << "- - - - - - Start - - - - - -" << std::endl;
      auto it_block = fd_cfg->begin();
      for (; it_block != fd_cfg->end(); it_block++)
      {
        auto it_element = (*it_block)->begin();
        for (; it_element != (*it_block)->end(); it_element++)
        {
          Optional<CFGStmt> it_stmt = it_element->getAs<CFGStmt>();
          const Stmt *S = it_stmt->getStmt();
          char const *StmtClassName = S->getStmtClassName();
          //std::cout << "\033[32m" << StmtClassName << "\033[0m" << std::endl;

          int isDeclStmt = strcmp(StmtClassName, "DeclStmt");
          int isCallExpr = strcmp(StmtClassName, "CallExpr");

          if (isDeclStmt == 0)
            handleDeclStmt(S); //处理DeclStmt

          if (isCallExpr == 0)
            handleCallExpr(S); //处理CallExpr
        }
      }
      //std::cout << "- - - - - - End - - - - - -" << std::endl;   
}
};
#endif