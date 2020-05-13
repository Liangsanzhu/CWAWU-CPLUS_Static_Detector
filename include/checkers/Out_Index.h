#ifndef O_I_H
#define O_I_H
#include "Detector.h"

class Out_Index{
map<std::string, int> ArrayUse;//数组名->访问下标
  map<std::string,SourceLocation> ArrayLocation;//数组名->位置
  map<std::string, int> ArrayDefn;//数组名->数组长度
SourceManager* scMg;
  string useName;
Stmt* Order(Stmt* node){
  if (node==nullptr)
    return nullptr;
  /*for (int i = 0; i < tab; i++)
  {
    std::cout<<" ";
  }*/
  //
  if (strcmp(node->getStmtClassName(),"ArraySubscriptExpr")==0)
  {
    /*ArraySubscriptExpr* array=(ArraySubscriptExpr*)node;
      std::cout<<array->getBase()->getType().getAsString()<<std::endl;//A
      std::cout<<array->getIdx()->getValueKind()<<std::endl;//[4],访问下标
      auto wannatry=array->getIdx();
      IntegerLiteral *tt=dyn_cast<IntegerLiteral>(wannatry);
      std::cout<<"I want\t"<<tt->getValue().getSExtValue()<<std::endl;
      std::cout<<"true"<<std::endl;*/
  
  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    //std::cout<<i->getStmtClassName()<<std::endl;
    if(strcmp(i->getStmtClassName(),"IntegerLiteral")==0){ //second
      IntegerLiteral* itl=(IntegerLiteral*)*i;
      //std::cout<<itl->getValue().getSExtValue()<<std::endl;//访问下标
      int useNum=itl->getValue().getSExtValue();
      ArrayUse.insert(pair<string,int>(useName,useNum));
    }
    if(strcmp(i->getStmtClassName(),"ImplicitCastExpr")==0){ //first
      DeclRefExpr* itl=(DeclRefExpr*)*i->child_begin();//DeclRefExpr
      VarDecl *it2vd=dyn_cast<VarDecl>(itl->getDecl());
      //std::cout<<it2vd->getQualifiedNameAsString()<<std::endl;//数组名
      useName=it2vd->getQualifiedNameAsString();
      ArrayLocation.insert(pair<string,SourceLocation>(useName,itl->getBeginLoc()));
    }
  }
  }
  //
  //std::cout<<node->getStmtClassName()<<std::endl;
  for (auto i = node->child_begin(); i != node->child_end(); i++)
  {
    Order(*i);
  }
}



public:
void OI_Entry(std::unordered_map<std::__cxx11::string, ASTResource>::iterator astr_iter)
{
     auto fds = astr_iter->second.GetFunctionDecls();
    auto vds = astr_iter->second.GetVarDecls();
    for(auto vd :vds){
      string nameDefn=vd->getQualifiedNameAsString();
      auto tp=vd->getType().getTypePtr();
      const ArrayType* arraytp=tp->getAsArrayTypeUnsafe();
      ConstantArrayType *cstArraytp=(ConstantArrayType*)arraytp;
      //std::cout<<cstArraytp->getSize().getSExtValue()<<std::endl;//数组长度
      int arrayLen=cstArraytp->getSize().getSExtValue();
      ArrayDefn.insert(pair<string,int>(nameDefn,arrayLen));
    }
    for(auto fd :fds){
      scMg=&fd->getASTContext().getSourceManager();
      Stmt* begin=fd->getBody();
      //std::cout<<"===============detect begin==============="<<std::endl;
      Order(begin);
      int errorCount=1;
        }
      //std::cout<<"================detect end================"<<std::endl;
      

    }

void OI_Dectect()
{
    for (auto i = ArrayUse.begin(); i != ArrayUse.end(); i++)
      {
        if(ArrayDefn[i->first]<=i->second)
        {
          //std::cout<<"----------Error"<<errorCount<<"----------"<<std::endl;
          /* std::cout<<"FilePath:\t"<<scMg->getFilename(ArrayLocation[i->first]).str()<<std::endl;
           std::cout<<"Function: "<<fd->getQualifiedNameAsString();
           std::cout<<"\t<line:"<<scMg->getSpellingLineNumber(ArrayLocation[i->first])\
           <<",col:"<<scMg->getSpellingColumnNumber(ArrayLocation[i->first])<<">"<<std::endl;
           std::cout<<scMg->getCharacterData(ArrayLocation[i->first])<<std::endl;
           
           std::cout<<"ArrayName:\t"<<i->first<<"\tOutIndex"<<std::endl;
                */
            error_info*e=new_error_info(NULL,scMg->getFilename(ArrayLocation[i->first]).str(),scMg->getSpellingLineNumber(ArrayLocation[i->first]),scMg->getSpellingColumnNumber(ArrayLocation[i->first]),TYPE_ERROR,OI_ERROR_TYPE_ARRAY_A+i->first+OI_ERROR_TYPE_ARRAY_B);
            result.push(e);
        }
     }
  }
};
#endif

