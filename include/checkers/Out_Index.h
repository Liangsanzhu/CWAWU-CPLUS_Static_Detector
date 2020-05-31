#ifndef O_I_H
#define O_I_H

#include "Detector.h"
using namespace std;


class Out_Index{
private:
  map<std::string,std::vector<int>> ArrayUse;//数组名->访问下标队列
  map<std::string,std::vector<int>> ArrayDefn;//数组名->定义下标队列
  map<std::string,SourceLocation> ArrayLocation;//数组名->位置


  map<int,int> VarUse;//VarID,值

  string useName;//使用的数组名
  string defnName;//定义的数组名
  std::vector<int> DefnTemp;//定义的数组长度
  std::vector<int> UseTemp;//使用的数组长度

  SourceManager* scMg;
private:
  Stmt* Order(Stmt* node);
  void OrderDeclStmt(Stmt* node);

  void FindArrayDefine(VarDecl *  vd);
  Stmt* FindArrayUse(Stmt* node);
  int  FindVarDefine(Stmt* node);

  int ifIndex;//分支序号
public:
  void OI_Entry(std::unordered_map<std::__cxx11::string, ASTResource>::iterator astr_iter);
  void OI_Dectect();
  void OI_Entry_New(SourceManager&SrcMgr,Stmt*S,int index);//test
};
Stmt* Out_Index::Order(Stmt* node){
  if (node==nullptr)
    return nullptr;
  if(strcmp(node->getStmtClassName(),"DeclStmt")==0){//变量、数组定义检测
    //下含VarDecl
    OrderDeclStmt(node);
  }
  
  if (strcmp(node->getStmtClassName(),"ArraySubscriptExpr")==0)//数组使用检测
  {
    UseTemp.clear();
    FindArrayUse(node);
    ArrayUse.insert(make_pair(useName,UseTemp));
  }
  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    Order(*i);
  }
}

void Out_Index::OrderDeclStmt(Stmt* node){
  DeclStmt *dlstmt = (DeclStmt *)node;
    for (auto j = dlstmt->decl_begin(); j != dlstmt->decl_end(); ++j) {
        Decl *anydecl = *j;
				unsigned kind = anydecl->getKind();
        if(kind==57){//VarDecl
          VarDecl* vardecl = (VarDecl *)anydecl;
          auto arrayType=vardecl->getType().getTypePtrOrNull();
          if(arrayType->isArrayType()){//数组结构
            FindArrayDefine(vardecl);//数组定义检测
          }          
          if(arrayType->isIntegerType()){//识别整形变量
            auto varInit =vardecl->ensureEvaluatedStmt()->Value;
            if(varInit != nullptr){//有赋值
              //FindVarDefine(varInit);
              VarUse.insert(make_pair(vardecl->getID(),FindVarDefine(varInit)));
            }
          }
        }
		}

}

void Out_Index::FindArrayDefine(VarDecl *  vd){
  //存储数组定义名称 -> 数组大小
    defnName = vd->getQualifiedNameAsString();
    DefnTemp.clear();
    auto arrayType=vd->getType().getTypePtrOrNull();
    while(strcmp(arrayType->getTypeClassName(),"ConstantArray")==0){
        const ArrayType* arraytp=arrayType->getAsArrayTypeUnsafe();
        ConstantArrayType *cstArraytp=(ConstantArrayType*)arraytp;
        //std::cout<<cstArraytp->getSize().getSExtValue()<<"*";//多维数组长度
        DefnTemp.push_back(cstArraytp->getSize().getSExtValue());
        arrayType=arrayType->getArrayElementTypeNoTypeQual();
    }
    ArrayDefn.insert(make_pair(defnName,DefnTemp));
  
  
}

Stmt* Out_Index::FindArrayUse(Stmt* node){
  for (auto i = node->child_begin(); i != node->child_end(); i++)
    {    
      if(strcmp(i->getStmtClassName(),"IntegerLiteral")==0){ //second
        IntegerLiteral* itl=(IntegerLiteral*)*i;
        //std::cout<<itl->getValue().getSExtValue()<<std::endl;//访问下标
        UseTemp.push_back(itl->getValue().getSExtValue());
      }
      if(strcmp(i->getStmtClassName(),"ImplicitCastExpr")==0){ //first
        if(strcmp(i->child_begin()->getStmtClassName(),"ArraySubscriptExpr")==0){
          FindArrayUse(*(i->child_begin()));
        }
        else{
          DeclRefExpr* itl=(DeclRefExpr*)*i->child_begin();//DeclRefExpr
          VarDecl *it2vd=dyn_cast<VarDecl>(itl->getDecl());
          if(VarUse.count(it2vd->getID())>0){//变量访问，VarUse存储有值的<变量ID,变脸值>
            UseTemp.push_back(VarUse[it2vd->getID()]);
          }
          else{
          //std::cout<<it2vd->getQualifiedNameAsString()<<std::endl;//数组名
          useName=it2vd->getQualifiedNameAsString();
          ArrayLocation.insert(pair<string,SourceLocation>(useName,itl->getBeginLoc()));
          }
        }
      }
    }
}

int  Out_Index::FindVarDefine(Stmt* node){
  if(strcmp(node->getStmtClassName(),"IntegerLiteral")==0){ //second
        IntegerLiteral* itl=(IntegerLiteral*)node;
        //std::cout<<itl->getValue().getSExtValue()<<std::endl;//变量的值
        return itl->getValue().getSExtValue();
  }
  for (auto i = node->child_begin(); i != node->child_end(); i++)
    {    
      FindVarDefine(*i);
    }
}

void Out_Index::OI_Entry(std::unordered_map<std::__cxx11::string, ASTResource>::iterator astr_iter){
  auto fds = astr_iter->second.GetFunctionDecls();
  auto vds = astr_iter->second.GetVarDecls();
  for(auto vd :vds){//存储数组定义名称 -> 数组大小
    FindArrayDefine(vd);
  }
  for(auto fd :fds){//检测所有函数
    scMg=&fd->getASTContext().getSourceManager();
    Stmt* begin=fd->getBody();
    Order(begin);
  }
}

void Out_Index::OI_Entry_New(SourceManager&SrcMgr,Stmt*S,int index){
  if(strcmp(S->getStmtClassName(),"DeclStmt")==0){
    DeclStmt *dlstmt = (DeclStmt *)S; 
    for (auto j = dlstmt->decl_begin(); j != dlstmt->decl_end(); ++j) {
        Decl *anydecl = *j;
				unsigned kind = anydecl->getKind();
        if(kind==57){//VarDecl
          VarDecl* vd = (VarDecl *)anydecl;
          FindArrayDefine(vd);
        }
    }
  }
  ifIndex  = index;
  scMg=&SrcMgr;
  Order(S);
  
}

void Out_Index::OI_Dectect()
{
  for(auto i = ArrayUse.begin();i != ArrayUse.end();i++){
    for (int j=0;j<i->second.size();j++)
    {
        if(ArrayDefn[i->first][j]<=i->second[j])
        {
            // error_info*e=new_error_info(NULL,scMg->getFilename(ArrayLocation[i->first]).str(),scMg->getSpellingLineNumber(ArrayLocation[i->first]),scMg->getSpellingColumnNumber(ArrayLocation[i->first]),TYPE_ERROR,OI_ERROR_TYPE_ARRAY_A+i->first+OI_ERROR_TYPE_ARRAY_B);
            error_info*e=new_error_info(NULL,scMg->getFilename(ArrayLocation[i->first]).str(),scMg->getSpellingLineNumber(ArrayLocation[i->first]),scMg->getSpellingColumnNumber(ArrayLocation[i->first]),TYPE_ERROR,OI_ERROR_TYPE_ARRAY_A+i->first+OI_ERROR_TYPE_ARRAY_B,ifIndex);
            result.push(e);
            break;
        }
    }
  }
}

#endif

