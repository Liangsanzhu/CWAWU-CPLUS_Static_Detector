#ifndef O_I_H
#define O_I_H

#include "Detector.h"
using namespace std;
struct ArrayVar{
  long id;//数组id
  std::string name;//数组名
  std::vector<int> useline;//访问下标
};
struct varVar{
  bool hasValue;
  int value;
};
struct varVarTwo{
  bool hasValue;
  std::vector<int> value;
};

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

  /*----------------------------2.0-------------------------------------*/
  map<SourceLocation,ArrayVar> ArrayUseTwo;//数组使用位置，数组信息
  map<long,ArrayVar> ArrayDefnTwo;//<id,{数组定义id,name,定义长度}>
  long useID;
  /*----------------------------2.0-------------------------------------*/

  /*----------------------------4.0-------------------------------------*/
  map<long,varVar>VarUseFour;//VarID,是否初始化/有赋值，变量值
  /*----------------------------4.0-------------------------------------*/

  /*----------------------------5.0,【test！】-------------------------------------*/
  map<long,varVarTwo> VarUseFor;//VarID，hasValue,value队列；存储for循环变量
  std::string VarNameFor;
  std::vector<int> VarValueFor;
  long VarIDFor;
  std::vector<int> UseTempFor;//for循环里使用的数组长度
  map<SourceLocation,ArrayVar> ArrayUseTwoFor;//for循环里数组使用位置，数组信息
  /*----------------------------5.0,【test！】-------------------------------------*/

private:
  Stmt* Order(Stmt* node);//原始版本
  Stmt* OrderOne(Stmt* node,int n);//1.0版本

  void OrderDeclStmt(Stmt* node);

  void FindArrayDefine(VarDecl *  vd);
  Stmt* FindArrayUse(Stmt* node);
  int  FindVarDefine(Stmt* node);

  void RenewVarValue(Stmt* node);//变量赋值
  int VarCalculator(char opcode,Stmt* node);
  void RenewVarValueSpecial(Stmt* node);//特殊的变量赋值，++,--

  int ifIndex;//分支序号

  /*----------------------------2.0-------------------------------------*/
  Stmt* OrderTwo(Stmt* node,int n);//2.0版本
  Stmt* FindArrayUseTwo(Stmt* node);
  void FindArrayDefineTwo(VarDecl *  vd);
  /*----------------------------2.0-------------------------------------*/

  /*----------------------------3.0-------------------------------------*/
  void RenewVarValueThree(Stmt* node);//变量赋值,改进id不存在的情况
  void RenewVarValueSpecialThree(Stmt* node);//特殊的变量赋值，+=2,-=3
  void RenewVarValueSpecialThree_One(Stmt* node);//特殊的变量赋值，新增+=n,-=n
  Stmt* OrderThree(Stmt* node,int n);//3.0版本
  /*----------------------------3.0-------------------------------------*/

  /*----------------------------4.0,【最新！】-------------------------------------*/
  void RenewVarValueFour(Stmt* node);//改进变量有无赋值
  void RenewVarValueSpecialFour(Stmt* node);
  void OrderDeclStmtFour(Stmt* node);
  Stmt* FindArrayUseFour(Stmt* node);
  int VarCalculatorFour(char opcode,Stmt* node);
  varVar* OrderImplicitCastExpr(ImplicitCastExpr* node);
  Stmt* OrderFour(Stmt* node,int n);//4.0版本
  /*----------------------------4.0,【最新！】-------------------------------------*/

  /*----------------------------5.0,【test！】-------------------------------------*/
  //1.0 动态数组
  void OrderDeclStmtFive(Stmt* node);
  int OrderCXXNewExpr(Stmt* node,int& arraySizeRes);
  void FindDynamicArrayDefine(CXXNewExpr* cxxex,VarDecl* vardecl,Stmt* node2cxxex);
  //2.0 新增fortmt的处理，只处理了i访问，>,>=,<,<=的情况
  Stmt* OrderFive(Stmt* node,int n);//5.0版本
  void SaveForstmtVar(Stmt* node);
  void RenewVarValueFive(Stmt* node);//新增for循环内的赋值
  void OrderBinaryOperator(char opcode,bool equal,Stmt* node,long &varID);//遍历BinaryOperator，获得变量值
  void OrderDeclStmtFive_One(Stmt* node);
  Stmt* FindArrayUseFive(Stmt* node);
  void SaveForstmtVarFive(Stmt* node);
  Stmt* Ordertest(Stmt* node,int n);
  Stmt* FindArrayUseFive_One(Stmt* node);
  Stmt* OrderFive_One(Stmt* node,int n);//5.0版本
  /*----------------------------5.0,【test！】-------------------------------------*/


public:
  void OI_Entry(SourceManager&SrcMgr,Stmt*S,int index);//原始版本
  void OI_Dectect();
  void OI_EntryZero(std::unordered_map<std::__cxx11::string, ASTResource>::iterator astr_iter);//原始版本
  void OI_DectectZero();
  void OI_Entry_New(SourceManager&SrcMgr,Stmt*S,int index);//1.0版本

  /*----------------------------2.0-------------------------------------*/
  void OI_DectectTwo();
  void OI_DectectTwo_One();
  void OI_EntryTwo(SourceManager&SrcMgr,Stmt*S,int index);//2.0版本
  void OI_EntryTwo_One(SourceManager&SrcMgr,Stmt*S,int index);//2.0版本
  /*----------------------------2.0-------------------------------------*/

  /*----------------------------3.0,【最新！】-------------------------------------*/
  void OI_DectectThree();
  void OI_EntryThree(SourceManager&SrcMgr,Stmt*S,int index);//2.0版本
  /*----------------------------3.0,【最新！】-------------------------------------*/

  /*----------------------------5.0,【test！】-------------------------------------*/
  void OI_DectectFive();
  /*----------------------------5.0,【test！】-------------------------------------*/
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

  if (strcmp(node->getStmtClassName(),"BinaryOperator")==0)//变量赋值
  {
    /* DeclRefExpr*/
    if (strcmp(node->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
    {     
      RenewVarValue(node);
    }  
  }

  /*特殊的变量赋值，++,--*/
  // if(strcmp(node->getStmtClassName(),"UnaryOperator")==0)
  // {
  //   RenewVarValueSpecial(node); 
  // }

  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    /*特殊的变量赋值，++,--*/
    if((strcmp(node ->getStmtClassName(),"CompoundStmt")==0) )//&& (strcmp(i->getStmtClassName(),"BinaryOperator")==0))
      {
        //node是CompoundStmt，i是BinaryOperator,i->child是DeclRefExpr
        // if (strcmp(i->child_begin()->getStmtClassName(),"DeclRefExpr")==0)    
        //   RenewVarValue(node);
      }
    else
    {
      Order(*i);
    }
  }
}

Stmt* Out_Index::OrderOne(Stmt* node,int n){
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

  if (strcmp(node->getStmtClassName(),"BinaryOperator")==0)//变量赋值
  {
    /* DeclRefExpr*/
    if (strcmp(node->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
    {     
      RenewVarValue(node);
    }  
  }

  /*特殊的变量赋值，++,--*/
  if(strcmp(node->getStmtClassName(),"UnaryOperator")==0 && n == 0)//根下的子节点，不是递归节点
  {
    RenewVarValueSpecial(node); 
  }

  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    OrderOne(*i,n++);//每递归一次，+1
  }
}

Stmt* Out_Index::OrderTwo(Stmt* node,int n){
  if (node==nullptr)
    return nullptr;
  if(strcmp(node->getStmtClassName(),"DeclStmt")==0){//变量、数组定义检测
    //下含VarDecl
    OrderDeclStmt(node);
  }
  
  if (strcmp(node->getStmtClassName(),"ArraySubscriptExpr")==0)//数组使用检测
  {
    UseTemp.clear();
    //FindArrayUse(node);
    FindArrayUseTwo(node);
    //ArrayUse.insert(make_pair(useName,UseTemp));
    ArrayVar * temp = new ArrayVar{useID,useName,UseTemp};
    ArrayUseTwo.insert(make_pair(node->getBeginLoc(),*temp));
  }

  if (strcmp(node->getStmtClassName(),"BinaryOperator")==0)//变量赋值
  {
    /* DeclRefExpr*/
    if (strcmp(node->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
    {     
      RenewVarValue(node);
    }  
  }

  /*特殊的变量赋值，++,--*/
  if(strcmp(node->getStmtClassName(),"UnaryOperator")==0 && n == 0)//根下的子节点，不是递归节点
  {
    RenewVarValueSpecial(node); 
  }


  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    OrderTwo(*i,n++);//每递归一次，+1
  }

  
}

Stmt* Out_Index::OrderThree(Stmt* node,int n){
  if (node==nullptr)
    return nullptr;
  if(strcmp(node->getStmtClassName(),"DeclStmt")==0){//变量、数组定义检测
    //下含VarDecl
    OrderDeclStmt(node);
  }
  
  if (strcmp(node->getStmtClassName(),"ArraySubscriptExpr")==0)//数组使用检测
  {
    UseTemp.clear();
    //FindArrayUse(node);
    FindArrayUseTwo(node);
    //ArrayUse.insert(make_pair(useName,UseTemp));
    ArrayVar * temp = new ArrayVar{useID,useName,UseTemp};
    ArrayUseTwo.insert(make_pair(node->getBeginLoc(),*temp));
  }

  if (strcmp(node->getStmtClassName(),"BinaryOperator")==0)//变量赋值
  {
    /* DeclRefExpr*/
    if (strcmp(node->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
    {     
      RenewVarValueThree(node);
    }  
  }

  /*特殊的变量赋值，++,--*/
  if(strcmp(node->getStmtClassName(),"UnaryOperator")==0 && n == 0)//根下的子节点，不是递归节点
  {
    RenewVarValueSpecial(node); 
  }

  /*特殊的变量赋值，+=2,-=2*/
  if(strcmp(node->getStmtClassName(),"CompoundAssignOperator")==0)//CompoundAssignOperator
  {
    RenewVarValueSpecialThree_One(node); 
  }


  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    OrderThree(*i,n++);//每递归一次，+1
  }

  
}

Stmt* Out_Index::OrderFour(Stmt* node,int n){
  if (node==nullptr)
    return nullptr;
  if(strcmp(node->getStmtClassName(),"DeclStmt")==0){//变量、数组定义检测
    //下含VarDecl
    OrderDeclStmtFour(node);
    //OrderDeclStmtFive(node);
  }
  
  if (strcmp(node->getStmtClassName(),"ArraySubscriptExpr")==0)//数组使用检测
  {
    UseTemp.clear();
    //FindArrayUse(node);
    FindArrayUseFour(node);
    //ArrayUse.insert(make_pair(useName,UseTemp));
    ArrayVar * temp = new ArrayVar{useID,useName,UseTemp};
    if (ArrayUseTwo.count(node->getBeginLoc())>0)
    {
      ArrayUseTwo[node->getBeginLoc()] = *temp;
    }
    else
    {
      ArrayUseTwo.insert(make_pair(node->getBeginLoc(),*temp));
    }  
  }

  if (strcmp(node->getStmtClassName(),"BinaryOperator")==0)//变量赋值
  {
    /* DeclRefExpr*/
    if (strcmp(node->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
    {     
      RenewVarValueFour(node);
    }  
  }

  /*特殊的变量赋值，++,--*/
  if(strcmp(node->getStmtClassName(),"UnaryOperator")==0 && n == 0)//根下的子节点，不是递归节点
  {
    RenewVarValueSpecial(node); 
  }

  /*特殊的变量赋值，+=2,-=2*/
  if(strcmp(node->getStmtClassName(),"CompoundAssignOperator")==0)//CompoundAssignOperator
  {
    RenewVarValueSpecialFour(node); 
  }


  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    OrderFour(*i,n++);//每递归一次，+1
  }

  
}

Stmt* Out_Index::OrderFive(Stmt* node,int n){
  if (node==nullptr)
    return nullptr;
  if(strcmp(node->getStmtClassName(),"DeclStmt")==0){//变量、数组定义检测
    //下含VarDecl
    //OrderDeclStmtFive(node);
    OrderDeclStmtFive_One(node);
  }
  
  if (strcmp(node->getStmtClassName(),"ArraySubscriptExpr")==0)//数组使用检测
  {
    UseTemp.clear();
    //FindArrayUse(node);
    //FindArrayUseFour(node);
    FindArrayUseFive(node);
    //ArrayUse.insert(make_pair(useName,UseTemp));
    ArrayVar * temp = new ArrayVar{useID,useName,UseTemp};
    if (ArrayUseTwo.count(node->getBeginLoc())>0)
    {
      ArrayUseTwo[node->getBeginLoc()] = *temp;
    }
    else
    {
      ArrayUseTwo.insert(make_pair(node->getBeginLoc(),*temp));
    }  
  }

  if (strcmp(node->getStmtClassName(),"BinaryOperator")==0)//变量赋值
  {
    /* DeclRefExpr*/
    if (strcmp(node->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
    {     
      RenewVarValueFive(node);
    }
    SaveForstmtVarFive(node);  
  }

  /*特殊的变量赋值，++,--*/
  if(strcmp(node->getStmtClassName(),"UnaryOperator")==0 && n == 0)//根下的子节点，不是递归节点
  {
    RenewVarValueSpecial(node); 
  }

  /*特殊的变量赋值，+=2,-=2*/
  if(strcmp(node->getStmtClassName(),"CompoundAssignOperator")==0)//CompoundAssignOperator
  {
    RenewVarValueSpecialFour(node); 
  }

  /*for循环里的变量*/
  // if(strcmp(node->getStmtClassName(),"ForStmt")==0)//for循环里的变量存储
  // {
  //   VarValueFor.clear();
  //   SaveForstmtVar(node);
  //   /*实现<的情况:for(int i=0;i<n;i++)*/
  //   sort(VarValueFor.begin(),VarValueFor.end());
  //   VarValueFor.end() = VarValueFor.end()-1;
  //   /*实现<的情况:for(int i=0;i<n;i++)*/
  //   
  //   varVarTwo * varVarTwoTemp = new varVarTwo{true,VarValueFor};
  //   VarUseFor.insert(make_pair(VarIDFor,*varVarTwoTemp));
  // }
  

  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    OrderFive(*i,n++);//每递归一次，+1
  }

  
}

Stmt* Out_Index::OrderFive_One(Stmt* node,int n){
  if (node==nullptr)
    return nullptr;
  if(strcmp(node->getStmtClassName(),"DeclStmt")==0){//变量、数组定义检测
    //下含VarDecl
    //OrderDeclStmtFive(node);
    OrderDeclStmtFive_One(node);
  }
  
  if (strcmp(node->getStmtClassName(),"ArraySubscriptExpr")==0)//数组使用检测
  {
    UseTemp.clear();
    UseTempFor.clear();
    //FindArrayUse(node);
    //FindArrayUseFour(node);
    FindArrayUseFive_One(node);
    //ArrayUse.insert(make_pair(useName,UseTemp));
    ArrayVar * temp = new ArrayVar{useID,useName,UseTemp};
    /*forstmt*/
    ArrayVar * tempFor = new ArrayVar{useID,useName,UseTempFor};
    if(UseTempFor.size()>0){
      if (ArrayUseTwoFor.count(node->getBeginLoc())>0)
      {
        ArrayUseTwoFor[node->getBeginLoc()] = *tempFor;
      }
    else
      {
        ArrayUseTwoFor.insert(make_pair(node->getBeginLoc(),*tempFor));
      }  
    }
    /*forstmt*/
    else if(UseTemp.size()>0)
    {
      if (ArrayUseTwo.count(node->getBeginLoc())>0)
      {
        ArrayUseTwo[node->getBeginLoc()] = *temp;
      }
      else
      {
        ArrayUseTwo.insert(make_pair(node->getBeginLoc(),*temp));
      } 
    }   
  }

  if (strcmp(node->getStmtClassName(),"BinaryOperator")==0)//变量赋值
  {
    /* DeclRefExpr*/
    if (strcmp(node->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
    {     
      RenewVarValueFive(node);
    }
    SaveForstmtVarFive(node);  
  }

  /*特殊的变量赋值，++,--*/
  if(strcmp(node->getStmtClassName(),"UnaryOperator")==0 && n == 0)//根下的子节点，不是递归节点
  {
    RenewVarValueSpecial(node); 
  }

  /*特殊的变量赋值，+=2,-=2*/
  if(strcmp(node->getStmtClassName(),"CompoundAssignOperator")==0)//CompoundAssignOperator
  {
    RenewVarValueSpecialFour(node); 
  }

  /*for循环里的变量*/
  // if(strcmp(node->getStmtClassName(),"ForStmt")==0)//for循环里的变量存储
  // {
  //   VarValueFor.clear();
  //   SaveForstmtVar(node);
  //   /*实现<的情况:for(int i=0;i<n;i++)*/
  //   sort(VarValueFor.begin(),VarValueFor.end());
  //   VarValueFor.end() = VarValueFor.end()-1;
  //   /*实现<的情况:for(int i=0;i<n;i++)*/
  //   
  //   varVarTwo * varVarTwoTemp = new varVarTwo{true,VarValueFor};
  //   VarUseFor.insert(make_pair(VarIDFor,*varVarTwoTemp));
  // }
  

  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    OrderFive_One(*i,n++);//每递归一次，+1
  }

  
}


Stmt* Out_Index::Ordertest(Stmt* node,int n){
  //cout<<node->getStmtClassName()<<endl;
  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    Ordertest(*i,n++);//每递归一次，+1
  }

}


void Out_Index::RenewVarValue(Stmt* node){//node为BinaryOperator
  DeclRefExpr* dre=(DeclRefExpr*)*(node->child_begin());//DeclRefExpr
  
  VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl());
  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    if (strcmp(i->getStmtClassName(),"IntegerLiteral")==0)//有赋值
    {
      IntegerLiteral* itl=(IntegerLiteral*)*i;
      //std::cout<<itl->getValue().getSExtValue()<<std::endl;//变量新的赋值
      if(VarUse.count(dre2vd->getID())>0){
        //变量访问，VarUse存储有值的<变量ID,变量值>
        VarUse[dre2vd->getID()] = itl->getValue().getSExtValue();
      }
    }
    if (strcmp(i->getStmtClassName(),"BinaryOperator")==0)//BinaryOperator下有BinaryOperator
    {
      BinaryOperator* bop = (BinaryOperator*)*i;
      char opcode ='?';
      if (bop->getOpcode()==BO_Add)//"+"
      {
        opcode = '+';
      }
      else if(bop->getOpcode()==BO_Sub)//"-"
      {
        opcode = '-';
      }
      else if (bop->getOpcode()==BO_Mul)//"*"
      {
        opcode = '*';
      }
      else if (bop->getOpcode()==BO_Div)//"/"
      {
        opcode = '/';
      }
      else if (bop->getOpcode()==BO_Rem)//"%"
      {
        opcode = '%';
      }
      int res = VarCalculator(opcode,(Stmt*)bop);
      VarUse[dre2vd->getID()] = res;
    }
  }
}

void Out_Index::RenewVarValueThree(Stmt* node){//node为BinaryOperator
  DeclRefExpr* dre=(DeclRefExpr*)*(node->child_begin());//DeclRefExpr
  
  VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl());
  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    if (strcmp(i->getStmtClassName(),"IntegerLiteral")==0)//有赋值
    {
      IntegerLiteral* itl=(IntegerLiteral*)*i;
      //std::cout<<itl->getValue().getSExtValue()<<std::endl;//变量新的赋值
      if(VarUse.count(dre2vd->getID())>0){
        //变量访问，VarUse存储有值的<变量ID,变量值>
        VarUse[dre2vd->getID()] = itl->getValue().getSExtValue();
      }
    }
    if (strcmp(i->getStmtClassName(),"BinaryOperator")==0)//BinaryOperator下有BinaryOperator
    {
      BinaryOperator* bop = (BinaryOperator*)*i;
      char opcode ='?';
      if (bop->getOpcode()==BO_Add)//"+"
      {
        opcode = '+';
      }
      else if(bop->getOpcode()==BO_Sub)//"-"
      {
        opcode = '-';
      }
      else if (bop->getOpcode()==BO_Mul)//"*"
      {
        opcode = '*';
      }
      else if (bop->getOpcode()==BO_Div)//"/"
      {
        opcode = '/';
      }
      else if (bop->getOpcode()==BO_Rem)//"%"
      {
        opcode = '%';
      }
      int res = VarCalculator(opcode,(Stmt*)bop);
      if(VarUse.count(dre2vd->getID())>0){
        VarUse[dre2vd->getID()] = res;
      }
    }
  }
}

void Out_Index::RenewVarValueFour(Stmt* node){//改进变量无赋值，node为BinaryOperator
  if(strcmp(node->child_begin()->getStmtClassName(),"DeclRefExpr")==0){
    DeclRefExpr* dre=(DeclRefExpr*)*(node->child_begin());//DeclRefExpr
    
    VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl());
    for (auto i = node->child_begin(); i != node->child_end(); i++)
    {
      if (strcmp(i->getStmtClassName(),"IntegerLiteral")==0)//有赋值
      {
        IntegerLiteral* itl=(IntegerLiteral*)*i;
        //std::cout<<itl->getValue().getSExtValue()<<std::endl;//变量新的赋值
        if(VarUseFour.count(dre2vd->getID())>0){
          //变量访问，VarUse存储有值的<变量ID,变量值>
          VarUseFour[dre2vd->getID()].value = itl->getValue().getSExtValue();
        }
      }
      if (strcmp(i->getStmtClassName(),"BinaryOperator")==0)//BinaryOperator下有BinaryOperator
      {
        BinaryOperator* bop = (BinaryOperator*)*i;
        char opcode ='?';
        if (bop->getOpcode()==BO_Add)//"+"
        {
          opcode = '+';
        }
        else if(bop->getOpcode()==BO_Sub)//"-"
        {
          opcode = '-';
        }
        else if (bop->getOpcode()==BO_Mul)//"*"
        {
          opcode = '*';
        }
        else if (bop->getOpcode()==BO_Div)//"/"
        {
          opcode = '/';
        }
        else if (bop->getOpcode()==BO_Rem)//"%"
        {
          opcode = '%';
        }
        int res = VarCalculatorFour(opcode,(Stmt*)bop);
        if(VarUseFour.count(dre2vd->getID())>0){
          VarUseFour[dre2vd->getID()].value = res;
        }
      }
    }
  }
}

void Out_Index::RenewVarValueFive(Stmt* node){//改进变量无赋值，node为BinaryOperator
  DeclRefExpr* dre=(DeclRefExpr*)*(node->child_begin());//DeclRefExpr
  VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl());

  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    if (strcmp(i->getStmtClassName(),"IntegerLiteral")==0)//有赋值
    {
      IntegerLiteral* itl=(IntegerLiteral*)*i;
      //std::cout<<itl->getValue().getSExtValue()<<std::endl;//变量新的赋值
      if(VarUseFour.count(dre2vd->getID())>0){
        //变量访问，VarUse存储有值的<变量ID,变量值>
        VarUseFour[dre2vd->getID()].value = itl->getValue().getSExtValue();
      }
    }
    if (strcmp(i->getStmtClassName(),"BinaryOperator")==0)//BinaryOperator下有BinaryOperator
    {
      BinaryOperator* bop = (BinaryOperator*)*i;
      char opcode ='?';
      if (bop->getOpcode()==BO_Add)//"+"
      {
        opcode = '+';
      }
      else if(bop->getOpcode()==BO_Sub)//"-"
      {
        opcode = '-';
      }
      else if (bop->getOpcode()==BO_Mul)//"*"
      {
        opcode = '*';
      }
      else if (bop->getOpcode()==BO_Div)//"/"
      {
        opcode = '/';
      }
      else if (bop->getOpcode()==BO_Rem)//"%"
      {
        opcode = '%';
      }
      int res = VarCalculatorFour(opcode,(Stmt*)bop);
      if(VarUseFour.count(dre2vd->getID())>0){
        VarUseFour[dre2vd->getID()].value = res;
      }
    }
  }
}

void Out_Index::SaveForstmtVarFive(Stmt* node){//node = BinaryOperator
   /*forstmt更新值*/
  BinaryOperator * node2bop = (BinaryOperator*)node;
  long tempId = -1;
  if(node2bop->getOpcode()==BO_LT)//<
  {
    OrderBinaryOperator('<',false,(Stmt*)node2bop,tempId);
    return;
  }
  else if(node2bop->getOpcode()==BO_LE)//<=
  {
    OrderBinaryOperator('<',true,(Stmt*)node2bop,tempId);
    return;
  }
  else if(node2bop->getOpcode()==BO_GT)//>
  {
    OrderBinaryOperator('>',false,(Stmt*)node2bop,tempId);
    return;
  }
  else if(node2bop->getOpcode()==BO_GE)//>=
  {
    OrderBinaryOperator('>',true,(Stmt*)node2bop,tempId);
    return;
  }
  /*forstmt更新值*/

}

void Out_Index::OrderBinaryOperator(char opcode,bool equal,Stmt* node,long& varID){
  //>|<,是否取等,BinaryOperator，变量id
  for(auto i = node->child_begin();i!=node->child_end();i++){
    if(strcmp(i->getStmtClassName(),"DeclRefExpr")==0){
      DeclRefExpr * dcex = (DeclRefExpr*)*i;
      VarDecl *vd=dyn_cast<VarDecl>(dcex->getDecl());
      varID = vd->getID();
    }
    if(strcmp(i->getStmtClassName(),"IntegerLiteral")==0){
      IntegerLiteral *itl = (IntegerLiteral*)*i;
      // cout<<itl->getValue().getSExtValue()<<endl;
      // VarUseFor[varID].value.push_back(itl->getValue().getSExtValue());
      if(equal == true)//>=,<=
      {
        VarUseFor[varID].value.push_back(itl->getValue().getSExtValue());
      }
      else if(equal ==false && opcode =='>')//>
      {
        VarUseFor[varID].value.push_back(itl->getValue().getSExtValue()+1);
      }
      else if(equal ==false && opcode =='<')//<
      {
        VarUseFor[varID].value.push_back(itl->getValue().getSExtValue()-1);
      }
    }
    OrderBinaryOperator(opcode,equal,*i,varID);
  }
}

int Out_Index::VarCalculator(char opcode,Stmt* node){//opcode是运算符，node是BinaryOperator
  int sum;
  bool flag=false;
  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    if (strcmp(i->getStmtClassName(),"IntegerLiteral")==0)
    {
      IntegerLiteral* itl=(IntegerLiteral*)*i;
      //std::cout<<itl->getValue().getSExtValue()<<std::endl;//itl的值
      if(!flag)//第一个运算值
      {
        sum = itl->getValue().getSExtValue();
        flag = true;
      }
      else
      {
        switch (opcode)
        {
        case '+':
          sum +=itl->getValue().getSExtValue();
          break;
        case '-':
          sum -=itl->getValue().getSExtValue();
          break;
        case '*':
          sum *=itl->getValue().getSExtValue();
          break;
        case '/':
          sum /=itl->getValue().getSExtValue();
          break;
        case '%':
          sum %=itl->getValue().getSExtValue();
          break;
        default:
          break;
        }      
      }     
    }   
  }
  return sum;
}

int Out_Index::VarCalculatorFour(char opcode,Stmt* node){//opcode是运算符，node是BinaryOperator
  long sum;
  bool flag=false;
  varVar * firstVar,*secodeVar;
  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    if (strcmp(i->getStmtClassName(),"IntegerLiteral")==0)
    {
      IntegerLiteral* itl=(IntegerLiteral*)*i;
      //std::cout<<itl->getValue().getSExtValue()<<std::endl;//itl的值
      if(!flag)//第一个运算值
      {
        sum = itl->getValue().getSExtValue();
        firstVar = new varVar{true,sum};
        flag = true;
      }
      else
      {
        switch (opcode)
        {
        case '+':
          sum +=itl->getValue().getSExtValue();
          break;
        case '-':
          sum -=itl->getValue().getSExtValue();
          break;
        case '*':
          sum *=itl->getValue().getSExtValue();
          break;
        case '/':
          sum /=itl->getValue().getSExtValue();
          break;
        case '%':
          sum %=itl->getValue().getSExtValue();
          break;
        default:
          break;
        }      
        secodeVar = new varVar{true,itl->getValue().getSExtValue()};
      }     
    }   
    
    if (strcmp(i->getStmtClassName(),"ImplicitCastExpr")==0)
    {
      ImplicitCastExpr* icex=(ImplicitCastExpr*)*i;
      varVar* tempVar = OrderImplicitCastExpr(icex);
      //std::cout<<itl->getValue().getSExtValue()<<std::endl;//itl的值
      if(!flag)//第一个运算值
      {
        if (tempVar != nullptr)
        {
          sum = tempVar->value;
          firstVar = tempVar;
          flag = true;
        }
        
      }
      else
      {
        if (tempVar != nullptr)
        {
          secodeVar = tempVar;
          switch (opcode)
          {
          case '+':
            sum +=tempVar->value;
            break;
          case '-':
            sum -=tempVar->value;
            break;
          case '*':
            sum *=tempVar->value;
            break;
          case '/':
            sum /=tempVar->value;
            break;
          case '%':
            sum %=tempVar->value;
            break;
          default:
            break;
          }    
        }  
      }     
    }   
 
  }

  if (firstVar->hasValue==false || secodeVar->hasValue==false)
  {
    sum = -1;
  }
  return sum;
}

varVar* Out_Index::OrderImplicitCastExpr(ImplicitCastExpr* node){
  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    if (strcmp(i->getStmtClassName(),"DeclRefExpr")==0)
    {
      DeclRefExpr * drex = (DeclRefExpr*)*i;
      VarDecl * drex2vd = dyn_cast<VarDecl>(drex->getDecl());
      if (VarUseFour.count(drex2vd->getID())>0)
      {
        varVar * resTemp = new varVar{VarUseFour[drex2vd->getID()].hasValue,VarUseFour[drex2vd->getID()].value};
        return resTemp;
      }
      
    }
  }
  return nullptr;
}

void Out_Index::RenewVarValueSpecial(Stmt* node)//node为UnaryOperator,返回变量id
{
  if(strcmp(node->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
  {
    DeclRefExpr* dre=(DeclRefExpr*)*(node->child_begin());//DeclRefExpr
    VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl());    
    UnaryOperator* unop = (UnaryOperator*)node;
    if(unop->isPrefix()){
      //++n;
      if(unop->isIncrementOp())
      {
        if(VarUse.count(dre2vd->getID())>0){
          ++VarUse[dre2vd->getID()];
        }
      }
      else if (unop->isDecrementOp())
      {
        if(VarUse.count(dre2vd->getID())>0){
          --VarUse[dre2vd->getID()];
        }
      }
    }
    else if (unop->isPostfix())
    {
      //n++;
      if(unop->isIncrementOp())
      {
        if(VarUse.count(dre2vd->getID())>0){
          VarUse[dre2vd->getID()]++;
        }
      }
      else if (unop->isDecrementOp())
      {
        if(VarUse.count(dre2vd->getID())>0){
          VarUse[dre2vd->getID()]--;
        }
      }
    }
  }
}

void GetSomeStmt(Stmt * nodeStmt) {
  if (nodeStmt != nullptr) {
    CompoundStmt *cmpstmt;
    DeclStmt *dlstmt;
    ForStmt *forstmt;
    ReturnStmt *retstmt;
    CallExpr *callexpr;
    IntegerLiteral *intltr;
    CStyleCastExpr *cscastexpr;
    ImplicitCastExpr *imcastexpr;
    DeclRefExpr *dlrefexpr;
    StringLiteral *strltr;
    GNUNullExpr *gnu;
    BinaryOperator *bin;
    UnaryOperator *unary;
    ParenExpr *paren;
    SwitchStmt *swstmt;
    CaseStmt * casestmt;
    ConstantExpr *constmt;
    BreakStmt *brstmt;
    DefaultStmt *defau;
    IfStmt *ifstmt;
    CXXNullPtrLiteralExpr *nullptrltr;
    CXXConstructExpr *construct;
    MemberExpr *member;
    FloatingLiteral *floatltr;
    InitListExpr *initlst;
    unsigned line;
    int pos;
    Info info_tmp;
    unsigned stmtclass = nodeStmt->getStmtClass();
    switch (stmtclass) {
      case 3: //breakstmt
        brstmt = (BreakStmt *)nodeStmt;
        brstmt->getStmtClassName();
        break;
      case 8: //compoundstmt
        cmpstmt = (CompoundStmt *)nodeStmt;
        cmpstmt->getStmtClassName();
        for (auto tmp = cmpstmt->body_begin(); tmp != cmpstmt->body_end(); ++tmp) {
          (*tmp)->getBeginLoc().getPtrEncoding();
          GetSomeStmt(*tmp);
        }
        break;
      case 12: //declstmt
        dlstmt = (DeclStmt *)nodeStmt;
        dlstmt->getStmtClassName() ;
        for (auto j = dlstmt->decl_begin(); j != dlstmt->decl_end(); ++j) {
          (*j)->getBeginLoc();
        }
        break;
      case 14: //forstmt
        forstmt = (ForStmt *)nodeStmt;
        forstmt->getStmtClassName() ;
        for (auto j = forstmt->child_begin(); j != forstmt->child_end(); ++j) {
          GetSomeStmt(*j);
        }
        break;
      case 16: //ifstmt
        ifstmt = (IfStmt *)nodeStmt;
        ifstmt->getStmtClassName() ;
        for (auto i = ifstmt->child_begin(); i != ifstmt->child_end(); ++i)
          GetSomeStmt(*i);
        break;
      case 74: //returnstmt
        retstmt = (ReturnStmt *)nodeStmt;
        retstmt->getStmtClassName() ;
        for (auto j = retstmt->child_begin(); j != retstmt->child_end(); ++j) {
          GetSomeStmt(*j);
        }
        break;
      case 79: //casestmt
        casestmt = (CaseStmt *)nodeStmt;
        casestmt->getStmtClassName();
        for (auto i = casestmt->child_begin(); i != casestmt->child_end(); ++i) {
          GetSomeStmt(*i);
        }
        break;
      case 80: //defaultstmt
        defau = (DefaultStmt *)nodeStmt;
        defau->getStmtClassName();
        for (auto i = defau->child_begin(); i != defau->child_end(); ++i)
          GetSomeStmt(*i);
        break;
      case 81: //switchstmt
        swstmt = (SwitchStmt *)nodeStmt;
        swstmt->getStmtClassName();
        for (auto i = swstmt->child_begin(); i != swstmt->child_end(); ++i) {
          GetSomeStmt(*i);
        }
        break;
      case 92: //BinaryOperator
        bin = (BinaryOperator *)nodeStmt;
         bin->getStmtClassName() ;
         bin->getType().getAsString() ;
        break;
      case 97: //CXXConstructExpr
        construct = (CXXConstructExpr *)nodeStmt;
        construct->getStmtClassName();
       // cout << construct->getType().getAsString();
        //cout << construct->getConstructor()->getNumParams() ;
        for (unsigned i = 0; i < construct->getConstructor()->getNumParams(); ++i) {
          construct->getConstructor()->getParamDecl(i)->getNameAsString();
        }
        break;
      case 107: //CXXNullptrLiteralExpr
        nullptrltr = (CXXNullPtrLiteralExpr *)nodeStmt;
        nullptrltr->getStmtClassName();
        nullptrltr->getType().getAsString() ;
        break;
      case 116: //callExpr
        callexpr = (CallExpr *)nodeStmt;
        callexpr->getStmtClassName();
        for (auto i = callexpr->child_begin(); i != callexpr->child_end(); ++i) {
          GetSomeStmt(*i);
        }
        break;
      case 122: //CStyleCastExpr
        cscastexpr = (CStyleCastExpr *)nodeStmt;
        cscastexpr->getStmtClassName() ;
        cscastexpr->getType().getAsString();
        if (*(cscastexpr->child_begin()) != nullptr) {
          for (auto i = cscastexpr->child_begin(); i != cscastexpr->child_end(); ++i) {
            GetSomeStmt(*i);
          }
        }
        break;
      case 129: //ImplicitCastExpr
        imcastexpr = (ImplicitCastExpr *)nodeStmt;
        imcastexpr->getStmtClassName() ;
        imcastexpr->getType().getAsString();
        if (*(imcastexpr->child_begin()) != nullptr) {
          for (auto i = imcastexpr->child_begin(); i != imcastexpr->child_end(); ++i) {
            GetSomeStmt(*i);
          }
        }
        break;
      case 136: //DeclRefExpr
        dlrefexpr = (DeclRefExpr *)nodeStmt;
        dlrefexpr->getStmtClassName();
        dlrefexpr->getType().getAsString() ;
        dlrefexpr->getDecl()->getDeclKindName();
        dlrefexpr->getDecl()->getNameAsString();
        break;
      case 144: //floatingLiteral
        floatltr = (FloatingLiteral *)nodeStmt;
        floatltr->getStmtClassName() ;
        floatltr->getType().getAsString();
        break;
      case 145: //ConstantExpr
        constmt = (ConstantExpr *)nodeStmt;
        constmt->getStmtClassName() ;
        constmt->getType().getAsString() ;
        if (constmt->getAPValueResult().isInt() == true)
          constmt->getAPValueResult().getInt().getSExtValue() ;
        for (auto i = constmt->child_begin(); i != constmt->child_end(); ++i) {
          GetSomeStmt(*i);
        }
        break;
      case 148: //GNUNullExpr
        gnu = (GNUNullExpr *)nodeStmt;
        gnu->getStmtClassName();
        gnu->getType().getAsString();
        break;
      case 152: //InitLitExpr
        initlst = (InitListExpr *)nodeStmt;
        initlst->getStmtClassName() ;
        initlst->getType().getAsString();
        for (auto i = initlst->begin(); i != initlst->end(); ++i) {
          GetSomeStmt(*i);
        }
        
        break;
      case 153: //IntegerLiteral
        intltr = (IntegerLiteral *)nodeStmt;
        intltr->getStmtClassName();
        intltr->getType().getAsString();
        intltr->getValue().getSExtValue();
        break;
      case 158: //MemberExpr
        member = (MemberExpr *)nodeStmt;
        member->getStmtClassName();
        member->getType().getAsString();
         for (auto i = member->child_begin(); i != member->child_end(); ++i)
          GetSomeStmt(*i);
        break;
      case 181: //ParenExpr
        paren = (ParenExpr *)nodeStmt;
        paren->getStmtClassName();
        paren->getType().getAsString();
        switch (paren->getValueKind()) {
          case 0:  break;
          case 1:  break;
          case 2:  break;
          default: break;
        }
        break;
      case 189: //StringLiteral
        strltr = (StringLiteral *)nodeStmt;
        ///strltr->getStmtClassName();
        //strltr->getType().getAsString();
        //strltr->getString().str();
        break;
      case 195: //UnaryOperator
        unary = (UnaryOperator *)nodeStmt;
        //unary->getStmtClassName() ;
         //unary->getOpcodeStr(unary->getOpcode()).str();
        for (auto i = unary->child_begin(); i != unary->child_end(); ++i) {
          GetSomeStmt(*i);
        }
        break;
      default: 
        nodeStmt->getStmtClassName();
        break;
    }
  }
}

void Out_Index::RenewVarValueSpecialThree(Stmt* node)//node为CompoundAssignOperator
{
  if(strcmp(node->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
  {
    DeclRefExpr* dre=(DeclRefExpr*)*(node->child_begin());//DeclRefExpr
    VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl());   //获得被加/减数
    CompoundAssignOperator * caop = (CompoundAssignOperator*) node;

    //获得加/减数
    long num =0;
    for (auto i = node->child_begin(); i != node->child_end(); i++)
    {
      if (strcmp(i->getStmtClassName(),"IntegerLiteral")==0)//+=2，-=2的情况
      {
        IntegerLiteral * itl = (IntegerLiteral*)*i;
        num  = itl->getValue().getSExtValue();
      }
      
    }
    
    // +=
    if(strcmp(caop->getOpcodeStr().str().data(),"+=")==0){
      if(VarUse.count(dre2vd->getID())>0){
        VarUse[dre2vd->getID()] += num;
      }
    }

    //-=
    if(strcmp(caop->getOpcodeStr().str().data(),"-=")==0){
      if(VarUse.count(dre2vd->getID())>0){
        VarUse[dre2vd->getID()] -= num;
      }
    }
  }
}

void Out_Index::RenewVarValueSpecialThree_One(Stmt* node)//node为CompoundAssignOperator
{
  if(strcmp(node->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
  {
    DeclRefExpr* dre=(DeclRefExpr*)*(node->child_begin());//DeclRefExpr
    VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl());   //获得被加/减数
    CompoundAssignOperator * caop = (CompoundAssignOperator*) node;

    //获得加/减数
    long num =0;
    for (auto i = node->child_begin(); i != node->child_end(); i++)
    {
      if (strcmp(i->getStmtClassName(),"IntegerLiteral")==0)//+=2，-=2的情况
      {
        IntegerLiteral * itl = (IntegerLiteral*)*i;
        num  = itl->getValue().getSExtValue();
      }

      if (strcmp(i->getStmtClassName(),"ImplicitCastExpr")==0)//+=n，-=n的情况
      {
        ImplicitCastExpr * icex = (ImplicitCastExpr*)*i;
        for (auto ichild = i->child_begin(); ichild != i->child_end(); ichild++)
        {
          if (strcmp(ichild->getStmtClassName(),"DeclRefExpr")==0)
          {
            DeclRefExpr * drexChild = (DeclRefExpr*) *ichild;
            VarDecl *dre2vdChild=dyn_cast<VarDecl>(drexChild->getDecl());   //获得n
            if(VarUse.count(dre2vdChild->getID())>0){
              num = VarUse[dre2vdChild->getID()];
            }
          }
        }
      }     
    }
    
    // +=
    if(strcmp(caop->getOpcodeStr().str().data(),"+=")==0){
      if(VarUse.count(dre2vd->getID())>0){
        VarUse[dre2vd->getID()] += num;
      }
    }

    //-=
    if(strcmp(caop->getOpcodeStr().str().data(),"-=")==0){
      if(VarUse.count(dre2vd->getID())>0){
        VarUse[dre2vd->getID()] -= num;
      }
    }
  }
}

void Out_Index::RenewVarValueSpecialFour(Stmt* node)//node为CompoundAssignOperator
{
  bool hasInitValue = true;//+=n,-=n的情况，若n有赋值为true，否则为false

  if(strcmp(node->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
  {
    DeclRefExpr* dre=(DeclRefExpr*)*(node->child_begin());//DeclRefExpr
    VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl());   //获得被加/减数
    CompoundAssignOperator * caop = (CompoundAssignOperator*) node;

    //获得加/减数
    long num =0;
    for (auto i = node->child_begin(); i != node->child_end(); i++)
    {
      if (strcmp(i->getStmtClassName(),"IntegerLiteral")==0)//+=2，-=2的情况
      {
        IntegerLiteral * itl = (IntegerLiteral*)*i;
        num  = itl->getValue().getSExtValue();
      }

      if (strcmp(i->getStmtClassName(),"ImplicitCastExpr")==0)//+=n，-=n的情况
      {
        ImplicitCastExpr * icex = (ImplicitCastExpr*)*i;
        for (auto ichild = i->child_begin(); ichild != i->child_end(); ichild++)
        {
          if (strcmp(ichild->getStmtClassName(),"DeclRefExpr")==0)
          {
            DeclRefExpr * drexChild = (DeclRefExpr*) *ichild;
            VarDecl *dre2vdChild=dyn_cast<VarDecl>(drexChild->getDecl());   //获得n
            if(VarUseFour.count(dre2vdChild->getID())>0){
              num = VarUseFour[dre2vdChild->getID()].value;
              hasInitValue = VarUseFour[dre2vdChild->getID()].hasValue;
            }
            
          }
        }
      }     
    }
    
    // +=
    if(strcmp(caop->getOpcodeStr().str().data(),"+=")==0){
      if(hasInitValue == true){
        if(VarUseFour.count(dre2vd->getID())>0){
          VarUseFour[dre2vd->getID()].value += num;
        }
      }
      else
      {
        if(VarUseFour.count(dre2vd->getID())>0){
          VarUseFour[dre2vd->getID()].value = -1;
        }
      }
      
    }

    //-=
    if(strcmp(caop->getOpcodeStr().str().data(),"-=")==0){
      if(hasInitValue == true){
        if(VarUseFour.count(dre2vd->getID())>0){
          VarUseFour[dre2vd->getID()].value -= num;
        }
      }
      else
      {
        if(VarUseFour.count(dre2vd->getID())>0){
          VarUseFour[dre2vd->getID()].value = -1;
        }
      }
    }
  }
}

void Out_Index::SaveForstmtVar(Stmt* node)//node = forStmt
{
  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    if(strcmp(i->getStmtClassName(),"VarDecl")==0){//获得变量信息
      VarDecl * vd = (VarDecl*)*i;
      VarNameFor = vd->getNameAsString();
      VarIDFor = vd->getID();
    }
    if(strcmp(i->getStmtClassName(),"IntegerLiteral")==0){//获得变量值
      IntegerLiteral* itl = (IntegerLiteral*)*i;
      VarValueFor.push_back(itl->getValue().getSExtValue());
    }
    SaveForstmtVar(*i);
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
            else//未赋值,认为溢出（自动初始化为-1）
            {
              VarUse.insert(make_pair(vardecl->getID(),-1));
            }
          }
        }
		}

}

void Out_Index::OrderDeclStmtFour(Stmt* node){
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
              varVar *varTemp = new varVar{true,FindVarDefine(varInit)};
              VarUseFour.insert(make_pair(vardecl->getID(),*varTemp));
            }
            else//未赋值,认为溢出（自动初始化为-1）
            {
              varVar *varTemp = new varVar{false,-1};
              VarUseFour.insert(make_pair(vardecl->getID(),*varTemp));
            }
          }
        }
		}

}

void Out_Index::OrderDeclStmtFive(Stmt* node){
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
              varVar *varTemp = new varVar{true,FindVarDefine(varInit)};
              VarUseFour.insert(make_pair(vardecl->getID(),*varTemp));
            }
            else//未赋值,认为溢出（自动初始化为-1）
            {
              varVar *varTemp = new varVar{false,-1};
              VarUseFour.insert(make_pair(vardecl->getID(),*varTemp));
            }
          }
          if (arrayType->isPointerType())//动态数组
          {
            //if(node->child_begin()!=node->child_end())
            //node , node->child =  DeclStmt , CXXNewExpr
            for(auto node2cxxex = node->child_begin();node2cxxex != node->child_end();node2cxxex++ )
            {
              //auto node2cxxex = node->child_begin();
              if (strcmp(node2cxxex->getStmtClassName(),"CXXNewExpr")==0)
              {
                CXXNewExpr * cxxex = (CXXNewExpr*)*node2cxxex;
                FindDynamicArrayDefine(cxxex,vardecl,*node2cxxex);
              }
            }
          }
        }
		}

}

void Out_Index::OrderDeclStmtFive_One(Stmt* node){
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
              varVar *varTemp = new varVar{true,FindVarDefine(varInit)};
              VarUseFour.insert(make_pair(vardecl->getID(),*varTemp));

              /*forStmt*/
              std::vector<int> intvtemp;
              intvtemp.push_back(FindVarDefine(varInit));
              varVarTwo *varTeoTemp = new varVarTwo{true,intvtemp};
              VarUseFor.insert(make_pair(vardecl->getID(),*varTeoTemp));
              /*forStmt*/
            }
            else//未赋值,认为溢出（自动初始化为-1）
            {
              varVar *varTemp = new varVar{false,-1};
              VarUseFour.insert(make_pair(vardecl->getID(),*varTemp));
            }
          }
          if (arrayType->isPointerType())//动态数组
          {
            //if(node->child_begin()!=node->child_end())
            //node , node->child =  DeclStmt , CXXNewExpr
            for(auto node2cxxex = node->child_begin();node2cxxex != node->child_end();node2cxxex++ )
            {
              //auto node2cxxex = node->child_begin();
              if (strcmp(node2cxxex->getStmtClassName(),"CXXNewExpr")==0)
              {
                CXXNewExpr * cxxex = (CXXNewExpr*)*node2cxxex;
                FindDynamicArrayDefine(cxxex,vardecl,*node2cxxex);
              }
            }
          }
        }
		}

}


int Out_Index::OrderCXXNewExpr(Stmt* node,int& arraySizeRes){//node = CXXNewExpr
  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    if (strcmp(i->getStmtClassName(),"IntegerLiteral")==0)
    {
      IntegerLiteral * itl = (IntegerLiteral*)*i;
      arraySizeRes = itl->getValue().getSExtValue();
      return itl->getValue().getSExtValue();
    }
    OrderCXXNewExpr(*i,arraySizeRes);
  }
  return arraySizeRes;
}

void Out_Index::FindDynamicArrayDefine(CXXNewExpr* cxxex,VarDecl* vardecl,Stmt* node2cxxex){
  if(cxxex->isArray())
  {
    defnName = vardecl->getQualifiedNameAsString();
    DefnTemp.clear();
    int arraySizeRes =-1;
    DefnTemp.push_back(OrderCXXNewExpr(node2cxxex,arraySizeRes));
    ArrayVar * tempDef = new ArrayVar{vardecl->getID(),defnName,DefnTemp};
    ArrayDefnTwo.insert(make_pair(vardecl->getID(),*tempDef));
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


void Out_Index::FindArrayDefineTwo(VarDecl *  vd){
  //存储数组定义名称 -> 数组大小
    defnName = vd->getQualifiedNameAsString();
    DefnTemp.clear();
    auto arrayType=vd->getType().getTypePtrOrNull();
    if(strcmp(arrayType->getTypeClassName(),"ConstantArray")==0){
      while(strcmp(arrayType->getTypeClassName(),"ConstantArray")==0){
          const ArrayType* arraytp=arrayType->getAsArrayTypeUnsafe();
          ConstantArrayType *cstArraytp=(ConstantArrayType*)arraytp;
          //std::cout<<cstArraytp->getSize().getSExtValue()<<"*";//多维数组长度
          DefnTemp.push_back(cstArraytp->getSize().getSExtValue());
          arrayType=arrayType->getArrayElementTypeNoTypeQual();
      }
      //ArrayDefn.insert(make_pair(defnName,DefnTemp));
      ArrayVar * tempDef = new ArrayVar{vd->getID(),defnName,DefnTemp};
      ArrayDefnTwo.insert(make_pair(vd->getID(),*tempDef));
    }
  
  
}

Stmt* Out_Index::FindArrayUse(Stmt* node){
  for (auto i = node->child_begin(); i != node->child_end(); i++)
    {      
      if(strcmp(i->getStmtClassName(),"IntegerLiteral")==0){ //second
        IntegerLiteral* itl=(IntegerLiteral*)*i;
        //std::cout<<itl->getValue().getSExtValue()<<std::endl;//访问下标
        UseTemp.push_back(itl->getValue().getSExtValue());
      }
      
      if(strcmp(i->getStmtClassName(),"ImplicitCastExpr")==0)
      { //first
        if(strcmp(i->child_begin()->getStmtClassName(),"ArraySubscriptExpr")==0){
          FindArrayUse(*(i->child_begin()));
        }

        else if(strcmp(i->child_begin()->getStmtClassName(),"DeclRefExpr")==0){
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
      
        else if(strcmp(i->child_begin()->getStmtClassName(),"UnaryOperator")==0)//变量Var
        {
          //i是ImplicitCastExpr，i->child_begin是UnaryOperator，i->child->child是DeclRefExpr
          //b[++n];
          if(strcmp(i->child_begin()->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
          {
            DeclRefExpr* dre=(DeclRefExpr*)*(i->child_begin()->child_begin());//DeclRefExpr
            VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl()); 

            RenewVarValueSpecial(*i->child_begin());
            if(VarUse.count(dre2vd->getID())>0)//变量访问，VarUse存储有值的<变量ID,变脸值>
            {
              UseTemp.push_back(VarUse[dre2vd->getID()]);
            }
          }
        }
      }

      if(strcmp(i->getStmtClassName(),"UnaryOperator")==0)
      {
        //b[n++];
        if(strcmp(i->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
        {
          DeclRefExpr* dre=(DeclRefExpr*)*(i->child_begin());//DeclRefExpr
          VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl()); 

          if(VarUse.count(dre2vd->getID())>0)//变量访问，VarUse存储有值的<变量ID,变脸值>
          {
            UseTemp.push_back(VarUse[dre2vd->getID()]);
          }
          RenewVarValueSpecial(*i);
        }
      }
    }
}

Stmt* Out_Index::FindArrayUseTwo(Stmt* node){
  for (auto i = node->child_begin(); i != node->child_end(); i++)
    {      
      if(strcmp(i->getStmtClassName(),"IntegerLiteral")==0){ //second
        IntegerLiteral* itl=(IntegerLiteral*)*i;
        //std::cout<<itl->getValue().getSExtValue()<<std::endl;//访问下标
        UseTemp.push_back(itl->getValue().getSExtValue());
      }
      
      if(strcmp(i->getStmtClassName(),"ImplicitCastExpr")==0)
      { //first
        if(strcmp(i->child_begin()->getStmtClassName(),"ArraySubscriptExpr")==0){
          FindArrayUseTwo(*(i->child_begin()));
        }

        else if(strcmp(i->child_begin()->getStmtClassName(),"DeclRefExpr")==0){
          DeclRefExpr* itl=(DeclRefExpr*)*i->child_begin();//DeclRefExpr
          VarDecl *it2vd=dyn_cast<VarDecl>(itl->getDecl());
          if(VarUse.count(it2vd->getID())>0){//变量访问，VarUse存储有值的<变量ID,变脸值>
            UseTemp.push_back(VarUse[it2vd->getID()]);
          }
          else{
          //std::cout<<it2vd->getQualifiedNameAsString()<<std::endl;//数组名
          useName=it2vd->getQualifiedNameAsString();
          useID = it2vd->getID();
          ArrayLocation.insert(pair<string,SourceLocation>(useName,itl->getBeginLoc()));
          }
        }
      
        else if(strcmp(i->child_begin()->getStmtClassName(),"UnaryOperator")==0)//变量Var
        {
          //i是ImplicitCastExpr，i->child_begin是UnaryOperator，i->child->child是DeclRefExpr
          //b[++n];
          if(strcmp(i->child_begin()->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
          {
            DeclRefExpr* dre=(DeclRefExpr*)*(i->child_begin()->child_begin());//DeclRefExpr
            VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl()); 

            RenewVarValueSpecial(*i->child_begin());
            if(VarUse.count(dre2vd->getID())>0)//变量访问，VarUse存储有值的<变量ID,变脸值>
            {
              UseTemp.push_back(VarUse[dre2vd->getID()]);
            }
          }
        }
      }

      if(strcmp(i->getStmtClassName(),"UnaryOperator")==0)
      {
        //b[n++];
        if(strcmp(i->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
        {
          DeclRefExpr* dre=(DeclRefExpr*)*(i->child_begin());//DeclRefExpr
          VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl()); 

          if(VarUse.count(dre2vd->getID())>0)//变量访问，VarUse存储有值的<变量ID,变脸值>
          {
            UseTemp.push_back(VarUse[dre2vd->getID()]);
          }
          RenewVarValueSpecial(*i);
        }
      }
    }
}

Stmt* Out_Index::FindArrayUseFour(Stmt* node){
  for (auto i = node->child_begin(); i != node->child_end(); i++)
    {      
      if(strcmp(i->getStmtClassName(),"IntegerLiteral")==0){ //second
        IntegerLiteral* itl=(IntegerLiteral*)*i;
        //std::cout<<itl->getValue().getSExtValue()<<std::endl;//访问下标
        UseTemp.push_back(itl->getValue().getSExtValue());
      }
      
      if(strcmp(i->getStmtClassName(),"ImplicitCastExpr")==0)
      { //first
        if(strcmp(i->child_begin()->getStmtClassName(),"ArraySubscriptExpr")==0){
          FindArrayUseFour(*(i->child_begin()));
        }

        else if(strcmp(i->child_begin()->getStmtClassName(),"DeclRefExpr")==0){
          DeclRefExpr* itl=(DeclRefExpr*)*i->child_begin();//DeclRefExpr
          VarDecl *it2vd=dyn_cast<VarDecl>(itl->getDecl());
          if(VarUseFour.count(it2vd->getID())>0){//变量访问，VarUse存储有值的<变量ID,变脸值>
            UseTemp.push_back(VarUseFour[it2vd->getID()].value);
          }
          else{
          //std::cout<<it2vd->getQualifiedNameAsString()<<std::endl;//数组名
          useName=it2vd->getQualifiedNameAsString();
          useID = it2vd->getID();
          ArrayLocation.insert(pair<string,SourceLocation>(useName,itl->getBeginLoc()));
          }
        }
      
        else if(strcmp(i->child_begin()->getStmtClassName(),"UnaryOperator")==0)//变量Var
        {
          //i是ImplicitCastExpr，i->child_begin是UnaryOperator，i->child->child是DeclRefExpr
          //b[++n];
          if(strcmp(i->child_begin()->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
          {
            DeclRefExpr* dre=(DeclRefExpr*)*(i->child_begin()->child_begin());//DeclRefExpr
            VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl()); 

            RenewVarValueSpecial(*i->child_begin());
            if(VarUseFour.count(dre2vd->getID())>0)//变量访问，VarUse存储有值的<变量ID,变脸值>
            {
              UseTemp.push_back(VarUseFour[dre2vd->getID()].value);
            }
          }
        }
      }

      if(strcmp(i->getStmtClassName(),"UnaryOperator")==0)
      {
        //b[n++];
        if(strcmp(i->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
        {
          DeclRefExpr* dre=(DeclRefExpr*)*(i->child_begin());//DeclRefExpr
          VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl()); 

          if(VarUseFour.count(dre2vd->getID())>0)//变量访问，VarUse存储有值的<变量ID,变脸值>
          {
            UseTemp.push_back(VarUseFour[dre2vd->getID()].value);
          }
          RenewVarValueSpecial(*i);
        }
      }
    }
}

Stmt* Out_Index::FindArrayUseFive(Stmt* node){
  for (auto i = node->child_begin(); i != node->child_end(); i++)
    {      
      if(strcmp(i->getStmtClassName(),"IntegerLiteral")==0){ //second
        IntegerLiteral* itl=(IntegerLiteral*)*i;
        //std::cout<<itl->getValue().getSExtValue()<<std::endl;//访问下标
        UseTemp.push_back(itl->getValue().getSExtValue());
      }
      
      if(strcmp(i->getStmtClassName(),"ImplicitCastExpr")==0)
      { //first
        if(strcmp(i->child_begin()->getStmtClassName(),"ArraySubscriptExpr")==0){
          FindArrayUseFive(*(i->child_begin()));
        }

        else if(strcmp(i->child_begin()->getStmtClassName(),"DeclRefExpr")==0){
          DeclRefExpr* itl=(DeclRefExpr*)*i->child_begin();//DeclRefExpr
          VarDecl *it2vd=dyn_cast<VarDecl>(itl->getDecl());
          if(VarUseFour.count(it2vd->getID())>0){//变量访问，VarUse存储有值的<变量ID,变脸值>
            /*forStmt*/
            if(VarUseFor.count(it2vd->getID())>0){
              UseTemp.insert(UseTemp.end(),VarUseFor[it2vd->getID()].value.begin(),VarUseFor[it2vd->getID()].value.end());
              return nullptr;
            }
            /*forStmt*/
            UseTemp.push_back(VarUseFour[it2vd->getID()].value);
          }
          else{
          //std::cout<<it2vd->getQualifiedNameAsString()<<std::endl;//数组名
          useName=it2vd->getQualifiedNameAsString();
          useID = it2vd->getID();
          ArrayLocation.insert(pair<string,SourceLocation>(useName,itl->getBeginLoc()));
          }
        }
      
        else if(strcmp(i->child_begin()->getStmtClassName(),"UnaryOperator")==0)//变量Var
        {
          //i是ImplicitCastExpr，i->child_begin是UnaryOperator，i->child->child是DeclRefExpr
          //b[++n];
          if(strcmp(i->child_begin()->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
          {
            DeclRefExpr* dre=(DeclRefExpr*)*(i->child_begin()->child_begin());//DeclRefExpr
            VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl()); 

            RenewVarValueSpecial(*i->child_begin());
            if(VarUseFour.count(dre2vd->getID())>0)//变量访问，VarUse存储有值的<变量ID,变脸值>
            {
              UseTemp.push_back(VarUseFour[dre2vd->getID()].value);
            }
          }
        }
      }

      if(strcmp(i->getStmtClassName(),"UnaryOperator")==0)
      {
        //b[n++];
        if(strcmp(i->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
        {
          DeclRefExpr* dre=(DeclRefExpr*)*(i->child_begin());//DeclRefExpr
          VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl()); 

          if(VarUseFour.count(dre2vd->getID())>0)//变量访问，VarUse存储有值的<变量ID,变脸值>
          {
            UseTemp.push_back(VarUseFour[dre2vd->getID()].value);
          }
          RenewVarValueSpecial(*i);
        }
      }
    }
}

Stmt* Out_Index::FindArrayUseFive_One(Stmt* node){
  for (auto i = node->child_begin(); i != node->child_end(); i++)
    {      
      if(strcmp(i->getStmtClassName(),"IntegerLiteral")==0){ //second
        IntegerLiteral* itl=(IntegerLiteral*)*i;
        //std::cout<<itl->getValue().getSExtValue()<<std::endl;//访问下标
        UseTemp.push_back(itl->getValue().getSExtValue());
      }
      
      if(strcmp(i->getStmtClassName(),"ImplicitCastExpr")==0)
      { //first
        if(strcmp(i->child_begin()->getStmtClassName(),"ArraySubscriptExpr")==0){
          FindArrayUseFive_One(*(i->child_begin()));
        }

        else if(strcmp(i->child_begin()->getStmtClassName(),"DeclRefExpr")==0){
          DeclRefExpr* itl=(DeclRefExpr*)*i->child_begin();//DeclRefExpr
          VarDecl *it2vd=dyn_cast<VarDecl>(itl->getDecl());
          if(VarUseFour.count(it2vd->getID())>0 || VarUseFor.count(it2vd->getID())>0){//变量访问，VarUse存储有值的<变量ID,变脸值>
            /*forStmt*/
            if(VarUseFor.count(it2vd->getID())>0){
              UseTempFor.insert(UseTemp.end(),VarUseFor[it2vd->getID()].value.begin(),VarUseFor[it2vd->getID()].value.end());
              return nullptr;
            }
            /*forStmt*/
            else{
              UseTemp.push_back(VarUseFour[it2vd->getID()].value);
            }
          }
          else{
          //std::cout<<it2vd->getQualifiedNameAsString()<<std::endl;//数组名
          useName=it2vd->getQualifiedNameAsString();
          useID = it2vd->getID();
          ArrayLocation.insert(pair<string,SourceLocation>(useName,itl->getBeginLoc()));
          }
        }
      
        else if(strcmp(i->child_begin()->getStmtClassName(),"UnaryOperator")==0)//变量Var
        {
          //i是ImplicitCastExpr，i->child_begin是UnaryOperator，i->child->child是DeclRefExpr
          //b[++n];
          if(strcmp(i->child_begin()->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
          {
            DeclRefExpr* dre=(DeclRefExpr*)*(i->child_begin()->child_begin());//DeclRefExpr
            VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl()); 

            RenewVarValueSpecial(*i->child_begin());
            if(VarUseFour.count(dre2vd->getID())>0)//变量访问，VarUse存储有值的<变量ID,变脸值>
            {
              UseTemp.push_back(VarUseFour[dre2vd->getID()].value);
            }
          }
        }
      }

      if(strcmp(i->getStmtClassName(),"UnaryOperator")==0)
      {
        //b[n++];
        if(strcmp(i->child_begin()->getStmtClassName(),"DeclRefExpr")==0)
        {
          DeclRefExpr* dre=(DeclRefExpr*)*(i->child_begin());//DeclRefExpr
          VarDecl *dre2vd=dyn_cast<VarDecl>(dre->getDecl()); 

          if(VarUseFour.count(dre2vd->getID())>0)//变量访问，VarUse存储有值的<变量ID,变脸值>
          {
            UseTemp.push_back(VarUseFour[dre2vd->getID()].value);
          }
          RenewVarValueSpecial(*i);
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

void Out_Index::OI_EntryZero(std::unordered_map<std::__cxx11::string, ASTResource>::iterator astr_iter){
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

void Out_Index::OI_Entry(SourceManager&SrcMgr,Stmt*S,int index){
  OI_EntryThree(SrcMgr,S,index);
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
  //Order(S);
  OrderOne(S,0);
  
}

void Out_Index::OI_EntryTwo(SourceManager&SrcMgr,Stmt*S,int index){
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
  //Order(S);
  OrderTwo(S,0);
  
}

void Out_Index::OI_EntryTwo_One(SourceManager&SrcMgr,Stmt*S,int index){
  if(strcmp(S->getStmtClassName(),"DeclStmt")==0){
    DeclStmt *dlstmt = (DeclStmt *)S; 
    for (auto j = dlstmt->decl_begin(); j != dlstmt->decl_end(); ++j) {
        Decl *anydecl = *j;
				unsigned kind = anydecl->getKind();
        if(kind==57){//VarDecl
          VarDecl* vd = (VarDecl *)anydecl;
          FindArrayDefineTwo(vd);
        }
    }
  }
  ifIndex  = index;
  scMg=&SrcMgr;
  //Order(S);
  OrderTwo(S,0);
  
}

void Out_Index::OI_EntryThree(SourceManager&SrcMgr,Stmt*S,int index){
  if(strcmp(S->getStmtClassName(),"DeclStmt")==0){
    DeclStmt *dlstmt = (DeclStmt *)S; 
    for (auto j = dlstmt->decl_begin(); j != dlstmt->decl_end(); ++j) {
        Decl *anydecl = *j;
				unsigned kind = anydecl->getKind();
        if(kind==57){//VarDecl
          VarDecl* vd = (VarDecl *)anydecl;
          FindArrayDefineTwo(vd);
        }
    }
  }
  ifIndex  = index;
  scMg=&SrcMgr;
  //Order(S);
  OrderFour(S,0);
  
}

void Out_Index::OI_DectectZero()
{

  // for(auto i = ArrayUse.begin();i != ArrayUse.end();i++){
  //   cout<<"use\t"<<i->first<<endl;
  //   for (int j=0;j<i->second.size();j++){
  //     cout<<i->second[j]<<endl;
  //   }
  //   cout<<"dfn\t"<<i->first<<endl;
  //   for (int j=0;j<i->second.size();j++){
  //     cout<<ArrayDefn[i->first][j]<<endl;
  //   }
  // }

  for(auto i = ArrayUse.begin();i != ArrayUse.end();i++){
    for (int j=0;j<i->second.size();j++)
    {
        if(ArrayDefn[i->first][j]<=i->second[j] || i->second[j]<0)//上溢 || 下溢
        {
            // error_info*e=new_error_info(NULL,scMg->getFilename(ArrayLocation[i->first]).str(),scMg->getSpellingLineNumber(ArrayLocation[i->first]),scMg->getSpellingColumnNumber(ArrayLocation[i->first]),TYPE_ERROR,OI_ERROR_TYPE_ARRAY_A+i->first+OI_ERROR_TYPE_ARRAY_B);
            error_info*e=new_error_info(NULL,scMg->getFilename(ArrayLocation[i->first]).str(),scMg->getSpellingLineNumber(ArrayLocation[i->first]),scMg->getSpellingColumnNumber(ArrayLocation[i->first]),TYPE_ERROR,OI_ERROR_TYPE_ARRAY_A+i->first+OI_ERROR_TYPE_ARRAY_B,ifIndex);
            result.push(e);
            break;
        }
    }
  }
}

void Out_Index::OI_Dectect()
{
  OI_DectectTwo_One();
  
}

void Out_Index::OI_DectectTwo()
{

  // for(auto i = ArrayUseTwo.begin();i != ArrayUseTwo.end();i++){
  //   cout<<"use\t"<<i->second.name<<endl;
  //   for (int j=0;j<i->second.useline.size();j++){
  //     cout<<i->second.useline[j]<<endl;
  //   }
  //   cout<<"dfn\t"<<i->second.name<<endl;
  //   for (int j=0;j<i->second.useline.size();j++){
  //     cout<<ArrayDefn[i->second.name][j]<<endl;
  //   }
  // }
 /* for (auto j = ArrayDefn.begin(); j!= ArrayDefn.end(); j++)
  {
    cout<<j->first<<endl;
  }*/
  

  for(auto i = ArrayUseTwo.begin();i != ArrayUseTwo.end();i++){//[location,{is,name,useline}]
    for (int j=0;j<i->second.useline.size();j++)
    {
        if(ArrayDefn[i->second.name][j]<=i->second.useline[j] || i->second.useline[j]<0)//上溢 || 下溢
        {
            // error_info*e=new_error_info(NULL,scMg->getFilename(ArrayLocation[i->first]).str(),scMg->getSpellingLineNumber(ArrayLocation[i->first]),scMg->getSpellingColumnNumber(ArrayLocation[i->first]),TYPE_ERROR,OI_ERROR_TYPE_ARRAY_A+i->first+OI_ERROR_TYPE_ARRAY_B);
            std::string ArrayNameTemp = i->second.name;
            SourceLocation locationTemp = i->first;
            error_info*e=new_error_info(NULL,scMg->getFilename(locationTemp).str(),scMg->getSpellingLineNumber(locationTemp),scMg->getSpellingColumnNumber(locationTemp),TYPE_ERROR,OI_ERROR_TYPE_ARRAY_A+ArrayNameTemp+OI_ERROR_TYPE_ARRAY_B,ifIndex);
            result.push(e);
            break;
        }
    }
  }
}

void Out_Index::OI_DectectTwo_One()
{
  // cout<<ArrayDefnTwo.size()<<"\t"<<ArrayUseTwo.size()<<endl;
  // for(auto i = ArrayUseTwo.begin();i != ArrayUseTwo.end();i++){
  //   cout<<"use\t"<<i->second.name<<"\t"<<i->second.id<<endl;
  //   for (int j=0;j<i->second.useline.size();j++){
  //     cout<<i->second.useline[j]<<endl;
  //   }
  //   cout<<"dfn\t"<<i->second.name<<"\t"<<i->second.id<<endl;
  //   for (int j=0;j<i->second.useline.size();j++){
  //     cout<<"hh"<<ArrayDefnTwo[i->second.id].useline[j]<<endl;
  //   }
  // }
  // for (auto j = ArrayDefnTwo.begin(); j != ArrayDefnTwo.end(); j++)
  // {
  //   cout<<j->first<<endl;
  // }
  // for (auto j = VarUse.begin(); j != VarUse.end(); j++)
  // {
  //   cout<<j->first<<"\t"<<j->second<<endl;
  // }
  // for (auto i = VarUseFor.begin(); i != VarUseFor.end(); i++)
  // {
  //   cout<<"for"<<i->first<<endl;
  //   for (auto j = i->second.value.begin(); j != i->second.value.end(); j++)
  //   {
  //     cout<<*j<<"\t";
  //   }
  //   cout<<endl;
  // }
  

  for(auto i = ArrayUseTwo.begin();i != ArrayUseTwo.end();i++){//[location,{is,name,useline}]
    for (int j=0;j<i->second.useline.size();j++)
    {
      if (ArrayDefnTwo.count(i->second.id)>0)
      {
        if(ArrayDefnTwo[i->second.id].useline[j]<=i->second.useline[j] || i->second.useline[j]<0)//上溢 || 下溢
        {
            // error_info*e=new_error_info(NULL,scMg->getFilename(ArrayLocation[i->first]).str(),scMg->getSpellingLineNumber(ArrayLocation[i->first]),scMg->getSpellingColumnNumber(ArrayLocation[i->first]),TYPE_ERROR,OI_ERROR_TYPE_ARRAY_A+i->first+OI_ERROR_TYPE_ARRAY_B);
            std::string ArrayNameTemp = i->second.name;
            SourceLocation locationTemp = i->first;
            error_info*e=new_error_info(NULL,scMg->getFilename(locationTemp).str(),scMg->getSpellingLineNumber(locationTemp),scMg->getSpellingColumnNumber(locationTemp),TYPE_ERROR,OI_ERROR_TYPE_ARRAY_A+ArrayNameTemp+OI_ERROR_TYPE_ARRAY_B,ifIndex);
            result.push(e);
            break;
        }
      }
    }
  }
}

void Out_Index::OI_DectectFive()
{
  // cout<<ArrayDefnTwo.size()<<"\t"<<ArrayUseTwo.size()<<endl;
  // for(auto i = ArrayUseTwoFor.begin();i != ArrayUseTwoFor.end();i++){
  //   cout<<"use\t"<<i->second.name<<"\t"<<i->second.id<<endl;
  //   for (int j=0;j<i->second.useline.size();j++){
  //     cout<<i->second.useline[j]<<endl;
  //   }
  //   cout<<"dfn\t"<<i->second.name<<"\t"<<i->second.id<<endl;
  //   for (int j=0;j<i->second.useline.size();j++){
  //     cout<<"hh"<<ArrayDefnTwo[i->second.id].useline[j]<<endl;
  //   }
  // }
  // for (auto j = ArrayDefnTwo.begin(); j != ArrayDefnTwo.end(); j++)
  // {
  //   cout<<j->first<<endl;
  // }
  // for (auto j = VarUse.begin(); j != VarUse.end(); j++)
  // {
  //   cout<<j->first<<"\t"<<j->second<<endl;
  // }
  // for (auto i = VarUseFor.begin(); i != VarUseFor.end(); i++)
  // {
  //   cout<<"for"<<i->first<<endl;
  //   for (auto j = i->second.value.begin(); j != i->second.value.end(); j++)
  //   {
  //     cout<<*j<<"\t";
  //   }
  //   cout<<endl;
  // }
  

  for(auto i = ArrayUseTwo.begin();i != ArrayUseTwo.end();i++){//[location,{is,name,useline}]
    for (int j=0;j<i->second.useline.size();j++)
    {
      if (ArrayDefnTwo.count(i->second.id)>0)
      {
        if(ArrayDefnTwo[i->second.id].useline[j]<=i->second.useline[j] || i->second.useline[j]<0)//上溢 || 下溢
        {
            // error_info*e=new_error_info(NULL,scMg->getFilename(ArrayLocation[i->first]).str(),scMg->getSpellingLineNumber(ArrayLocation[i->first]),scMg->getSpellingColumnNumber(ArrayLocation[i->first]),TYPE_ERROR,OI_ERROR_TYPE_ARRAY_A+i->first+OI_ERROR_TYPE_ARRAY_B);
            std::string ArrayNameTemp = i->second.name;
            SourceLocation locationTemp = i->first;
            error_info*e=new_error_info(NULL,scMg->getFilename(locationTemp).str(),scMg->getSpellingLineNumber(locationTemp),scMg->getSpellingColumnNumber(locationTemp),TYPE_ERROR,OI_ERROR_TYPE_ARRAY_A+ArrayNameTemp+OI_ERROR_TYPE_ARRAY_B,ifIndex);
            result.push(e);
            break;
        }
      }
    }
  }

  /*forstmt*/
  for(auto i = ArrayUseTwoFor.begin();i != ArrayUseTwoFor.end();i++){//[location,{is,name,useline}]
    for (int j=0;j<i->second.useline.size();j++)
    {
      if (ArrayDefnTwo.count(i->second.id)>0)
      {
        if(ArrayDefnTwo[i->second.id].useline[0]<=i->second.useline[j] || i->second.useline[j]<0)//上溢 || 下溢
        {
            // error_info*e=new_error_info(NULL,scMg->getFilename(ArrayLocation[i->first]).str(),scMg->getSpellingLineNumber(ArrayLocation[i->first]),scMg->getSpellingColumnNumber(ArrayLocation[i->first]),TYPE_ERROR,OI_ERROR_TYPE_ARRAY_A+i->first+OI_ERROR_TYPE_ARRAY_B);
            std::string ArrayNameTemp = i->second.name;
            SourceLocation locationTemp = i->first;
            error_info*e=new_error_info(NULL,scMg->getFilename(locationTemp).str(),scMg->getSpellingLineNumber(locationTemp),scMg->getSpellingColumnNumber(locationTemp),TYPE_ERROR,OI_ERROR_TYPE_ARRAY_A+ArrayNameTemp+OI_ERROR_TYPE_ARRAY_B,ifIndex);
            result.push(e);
            break;
        }
      }
    }
  }
  /*forstmt*/
}

#endif

