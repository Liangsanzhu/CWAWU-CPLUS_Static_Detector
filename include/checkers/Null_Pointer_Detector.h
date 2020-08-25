#ifndef N_P_D_H
#define N_P_D_H
#include"Detector.h"
#include "assert.h"
class Null_Pointer_Detector{
private:
SourceManager *SrcMgr;

struct savePointer{
    int id;     //the global id of the pointer
    //int block;  //the block id the pointer in
    bool declared;  // be declared here
    bool defined;   // be defined here
    bool setnull;   // be set null or delete
    bool derefered;     //be used here
    int line;
    int col;
    string file;
};

int route;
defuse_node *all_node;

std::map<int, vector<savePointer>>record;
//std::vector<int>ifBlock;
std::vector<savePointer> recordPointer;

void showSave(){
    cout<<setw(10)<<"pointerID";
    //cout<<setw(10)<<"BlockID";
    cout<<setw(10)<<"Line";
    cout<<setw(10)<<"Col";
    cout<<setw(10)<<"Declared";
    cout<<setw(10)<<"SetNull";
    cout<<setw(10)<<"Defined";
    cout<<setw(10)<<"Derefered";
    cout<<'\t'<<setw(10)<<"FileName"<<endl;
    for (auto re: recordPointer){
        cout<<setw(10)<<re.id;
        //cout<<setw(10)<<re.block;
        cout<<setw(10)<<re.line;
        cout<<setw(10)<<re.col;
        cout<<setw(10)<<re.declared;
        cout<<setw(10)<<re.setnull;
        cout<<setw(10)<<re.defined;
        cout<<setw(10)<<re.derefered;
        cout<<'\t'<<setw(20)<<re.file<<endl;
    }
    cout<<endl;
}

void showSingleSave(std::vector<savePointer> r){
    cout<<setw(10)<<"pointerID";
    //cout<<setw(10)<<"BlockID";
    cout<<setw(10)<<"Line";
    cout<<setw(10)<<"Col";
    cout<<setw(10)<<"Declared";
    cout<<setw(10)<<"SetNull";
    cout<<setw(10)<<"Defined";
    cout<<setw(10)<<"Derefered";
    cout<<'\t'<<setw(10)<<"FileName"<<endl;
    for (auto re: r){
        cout<<setw(10)<<re.id;
        //cout<<setw(10)<<re.block;
        cout<<setw(10)<<re.line;
        cout<<setw(10)<<re.col;
        cout<<setw(10)<<re.declared;
        cout<<setw(10)<<re.setnull;
        cout<<setw(10)<<re.defined;
        cout<<setw(10)<<re.derefered;
        cout<<'\t'<<setw(20)<<re.file<<endl;
    }
    cout<<endl;
}

bool saveEquals(savePointer a, savePointer b){
    return ((a.id == b.id) && (a.line == b.line) && (a.col == b.col) && (a.file == b.file) && 
    (a.declared == b.declared) && (a.setnull == b.setnull) && (a.derefered == b.derefered) && (a.defined == b.defined));
}

void showIf(std::vector<int>ifBlock){
    cout<<"If Block: ";
    for (auto i : ifBlock)
        cout<<i<<" ";
    cout<<endl<<endl;
}

void getVar(const Stmt*stmt,int&id,savePointer* &save){
    cout<<"IN getVar!\n";
    if(stmt!=nullptr)
    {

        if(strcmp(stmt->getStmtClassName(),"DeclRefExpr")==0){
            cout<<"IN getVar declRefExpr\n";
            DeclRefExpr * ref = (DeclRefExpr *)stmt;
            cout<<"1\n";
            std::string tmp = "";
            //VarDecl*vardecl=(VarDecl*)ref->getFoundDecl();
            //std::cout<<vardecl->getNameAsString()<<" "<<vardecl->getType().getAsString()<<" "<<vardecl->getGlobalID()<<std::endl;
            //tmp =vardecl->getType().getAsString();
            // for (auto j = dlstmt->decl_begin(); j != dlstmt->decl_end(); ++j) {
            // Decl *anydecl = *j;
            // 		unsigned kind = anydecl->getKind();
            // if(kind==57){//VarDecl
            //   VarDecl* vardecl = (VarDecl *)anydecl;

            //   auto arrayType=vardecl->getType().getTypePtrOrNull();
            try
            {
                // auto temp = ref->getType().getTypePtrOrNull();
                // cout<<"1.5\n";
                // //tmp = temp.getAsString();
                // if (temp == nullptr) {
                //     std::cout<<"temp nullptr.\n";
                // }
                // cout<<temp->isPointerType()<<endl;;             //why ??       
                // if (temp->isPointerType()){
                //     save->id=ref->getDecl()->getGlobalID();
                //   //  cout<<"4\n";
                //     id = save->id;
                //     cout<<"1.75\n";
                //     //cout<<tmp<<endl;;
                // }
            }
            catch(...)
            {
                return ;
            }
            cout<<"2\n";
            // if (tmp[tmp.length()-1]=='*'){
            //     cout<<"3\n";
            //     save->id=ref->getDecl()->getGlobalID();
            //     cout<<"4\n";
            //     id = save->id;
            //     //save->defined = true;
            // }
        cout<<"come to valuestmt\n";
            ValueStmt*F=(ValueStmt*)stmt;
            Expr*G=F->getExprStmt();
            DeclRefExpr*H=(DeclRefExpr*)G;
        }   
        
        if(strcmp(stmt->getStmtClassName(),"VarDecl")==0){
            VarDecl* vardecl = (VarDecl *)stmt;
            auto temp=vardecl->getType().getTypePtrOrNull();
            if(temp->isPointerType()){
                save->id=vardecl->getGlobalID();
                id = save->id;
                cout<<"1.75\n";
            }
        }

        if (strcmp(stmt->getStmtClassName(),"UnaryOperator")==0){
            cout<<"in Getvar UnaryOpr\n";
            UnaryOperator*unary = (UnaryOperator*)stmt;
            if(unary->getOpcodeStr(unary->getOpcode()).str()=="*")
                save->derefered = true;
            else if(unary->getOpcodeStr(unary->getOpcode()).str()=="&")
                save->defined = true;
        }

        if(strcmp(stmt->getStmtClassName(),"GNUNullExpr")==0){
            cout<<"IN getvar GNUnullexpr\n";
            save->setnull = true;
            save->defined = false;
            GNUNullExpr *gnu = (GNUNullExpr *)stmt;
        }

        if(strcmp(stmt->getStmtClassName(),"CXXNewExpr")==0){
            cout<<"IN getVar cxxnewexpr\n";
            CXXNewExpr *cxxnew = (CXXNewExpr*)stmt;
            save->defined = true;
            save->setnull = false;
        }
        //if(stmt->child_begin()->getStmtClass()!=nullptr) {
        for (auto it = stmt->child_begin();stmt->child_begin()!=nullptr&&it!=stmt->child_end();it++){
            getVar(*it,id,save);
        }
       // }
    }
}

void getMalloc(const Stmt*stmt, int &id, bool flag, savePointer* &save){ 
  cout<<"IN getMalloc\n";
  for(auto bt=stmt->child_begin();bt!=stmt->child_end();bt++)
        getMalloc(*bt,id, flag, save);
    
  if(strcmp(stmt->getStmtClassName(),"DeclRefExpr")==0)
    {
        cout<<"in malloc declrefexpr\n";
      ValueStmt*F=(ValueStmt*)stmt;
      Expr*G=F->getExprStmt();
      DeclRefExpr*H=(DeclRefExpr*)G;
      id = H->getDecl()->getGlobalID();
//      cout<<
      save->id = id;
      if(flag==false&&strcmp(H->getDecl()->getName().data(),"malloc")==0){   
        //id=S->getBeginLoc().getRawEncoding();
        save->defined = true;
        //id = save->id;
        flag = true;
      }
      else if (flag==false&&strcmp(H->getDecl()->getName().data(),"free")==0){
        //id=S->getBeginLoc().getRawEncoding();
        save->setnull = true;
       // id = save->id;
        flag = true;
      }
    }   
}

void NPD_Traverse(SourceManager *SrcMgr, Stmt *stmt){
    cout<<"In NPD Traverse!\n";
    //std::cout<<stmt->getStmtClassName()<<std::endl;
    //DeclStmt
    if(stmt!=nullptr)
    {
        if (strcmp(stmt->getStmtClassName(),"DeclStmt")==0){
            cout<<"in declSTmt\n";
            DeclStmt* declstmt=(DeclStmt*)stmt;
            VarDecl*vardecl=(VarDecl*)declstmt->getSingleDecl();
            //std::cout<<vardecl->getNameAsString()<<" "<<vardecl->getType().getAsString()<<" "<<vardecl->getGlobalID()<<std::endl;
            string tmp =vardecl->getType().getAsString();
            if (tmp[vardecl->getType().getAsString().length()-1]=='*'){
                savePointer *save= new savePointer;
                save->id=vardecl->getGlobalID();
                //save->block = block_id;
                save->declared = true;
                save->setnull = false;
                save->defined = false;
                save->derefered = false;
                save->line=SrcMgr->getSpellingLineNumber(vardecl->getBeginLoc());
                save->col=SrcMgr->getSpellingColumnNumber(vardecl->getBeginLoc());
                save->file = SrcMgr->getFilename(vardecl->getBeginLoc());
                delete save;
            }
            return ;
        }
        //BinaryOperator
        if (strcmp(stmt->getStmtClassName(),"BinaryOperator")==0){
            cout<<"in BinaryOpr\n";
            BinaryOperator *bin = (BinaryOperator *)stmt;
            //cout<<SrcMgr->getSpellingLineNumber(bin->getExprLoc())<<endl;
            if (bin->getOpcodeStr().str()!="="){
                return ;
            }
            int id = -1;
            savePointer *save = new savePointer;
            save->defined = false;
            save->declared = false;
            save->setnull = false;
            save->derefered = false;
            //save->block = block_id;
            save->line=SrcMgr->getSpellingLineNumber(bin->getExprLoc());
            save->col=SrcMgr->getSpellingColumnNumber(bin->getExprLoc());
            save->file = SrcMgr->getFilename(bin->getBeginLoc());
        
            getVar(stmt, id, save);
            if (id != -1)
                recordPointer.push_back(*save);
            delete save;
        }

        if (strcmp(stmt->getStmtClassName(),"IfStmt")==0){
            cout<<"in Ifstmt";
            IfStmt * ifstmt = (IfStmt *)stmt;
            //cout<<"IfStmt here!\n";
            //cout<<SrcMgr->getSpellingLineNumber(ifstmt->getBeginLoc())<<endl;
        }

        if (strcmp(stmt->getStmtClassName(),"CXXDeleteExpr")==0){
            cout<<"IN cxxdelete\n";
            CXXDeleteExpr *cxxdelt = (CXXDeleteExpr *)stmt;
            savePointer *save = new savePointer;
            int id = -1;
            save->defined = false;
            save->declared = false;
            save->setnull = true;
            save->derefered = false;
            //save->block = block_id;
            save->line=SrcMgr->getSpellingLineNumber(cxxdelt->getExprLoc());
            save->col=SrcMgr->getSpellingColumnNumber(cxxdelt->getExprLoc());
            save->file = SrcMgr->getFilename(cxxdelt->getBeginLoc());
            getVar(stmt, id, save);
            if (id != -1)
                recordPointer.push_back(*save);
            delete save;
        }

        if (strcmp(stmt->getStmtClassName(),"CXXOperatorCallExpr")==0){
            cout<<"in cxxoprcall\n";
            CXXOperatorCallExpr * cxxcall = (CXXOperatorCallExpr *)stmt;
            savePointer *save = new savePointer;
            int id = -1;
            save->defined = false;
            save->declared = false;
            save->setnull = false;
            save->derefered = false;
            //save->block = block_id;
            save->line=SrcMgr->getSpellingLineNumber(cxxcall->getExprLoc());
            save->col=SrcMgr->getSpellingColumnNumber(cxxcall->getExprLoc());
            save->file = SrcMgr->getFilename(cxxcall->getBeginLoc());
            getVar(stmt, id, save);
            if (id != -1)
                recordPointer.push_back(*save);
            delete save;
        }

        if (strcmp(stmt->getStmtClassName(),"CallExpr")==0){
            cout<<"in callexpr\n";
            CallExpr * callexpr = (CallExpr *)stmt;
            savePointer *save = new savePointer;
            int id = -1;
            save->defined = false;
            save->declared = false;
            save->setnull = false;
            save->derefered = false;
            //save->block = block_id;
            save->line=SrcMgr->getSpellingLineNumber(callexpr->getExprLoc());
            save->col=SrcMgr->getSpellingColumnNumber(callexpr->getExprLoc());
            save->file = SrcMgr->getFilename(callexpr->getBeginLoc());
            bool flag = false;
            getMalloc(stmt, id, flag, save);
            if (id != -1)
                recordPointer.push_back(*save);
            delete save;
        }

        for (auto it = stmt->child_begin();it != stmt->child_end();it++){
            NPD_Traverse(SrcMgr,*it);
        }
    }
}

void VisitBlock(clang::CFGBlock *block, SourceManager *SrcMgr, clang::CFGBlock *exit){
    cout<<"Visit Block!\n";
    if(block!=nullptr)
    {
        if (block->getBlockID() == exit->getBlockID()) 
            return ;
        //cout<<"     Get the block id\n";
    /*   if(block->getTerminatorStmt()!=nullptr)
            if(strcmp(block->getTerminatorStmt()->getStmtClassName(),"IfStmt")==0)
                ifBlock.push_back(block->getBlockID()-1); */
        //if(block->getLabel()!=nullptr)
            //std::cout<<block->getLabel()->getStmtClassName()<<block->getBlockID()<<std::endl;    
        int block_id = block->getBlockID();
        for (auto I = block->begin(); I != block->end(); ++I) {
            if (Optional<CFGStmt> CS = (*I).getAs<CFGStmt>()) {
                Stmt *S = const_cast<Stmt *>(CS->getStmt());
                assert(S != nullptr && "Expecting non-null Stmt");
                ////cout <<"1.\n"<<std::endl;
                NPD_Traverse(SrcMgr,S);
            }
        }
        if(*block->succ_begin()!=nullptr)
            for (auto i = block->succ_begin();i!=block->succ_end()&& *i!=nullptr;i++)
                VisitBlock(*i, SrcMgr,exit);
    }
}

void FindMalloc(Stmt *stmt,int id)
{
    if (stmt == nullptr)
        return ;
    if (strcmp(stmt->getStmtClassName(),"DeclRefExpr")==0)
    {
        DeclRefExpr *declrefexpr = (DeclRefExpr *)(stmt);
        if(strcmp(declrefexpr->getDecl()->getName().data(),"malloc")==0)
        {
            //printf("\033[33m%s\n\033[0m","******************malloc");
            savePointer save;
            //save.id = declrefexpr->getDecl()->getGlobalID();
            save.id = id;
            save.declared = false;
            save.defined = true;
            save.setnull = false;
            save.derefered = false;
            save.line = SrcMgr->getSpellingLineNumber(stmt->getBeginLoc());
            save.col = SrcMgr->getSpellingColumnNumber(stmt->getBeginLoc());
            save.file = SrcMgr->getFilename(stmt->getBeginLoc());
            if (recordPointer.size()!=0)
                if (!saveEquals(recordPointer.back(),save))
                    recordPointer.push_back(save);
        }
        if(strcmp(declrefexpr->getDecl()->getName().data(),"free")==0)
        {
            //printf("\033[33m%s\n\033[0m","******************free");
            savePointer save;
            //save.id = declrefexpr->getDecl()->getGlobalID();
            save.id = id;
            save.declared = false;
            save.defined = false;
            save.setnull = true;
            save.derefered = false;
            save.line = SrcMgr->getSpellingLineNumber(stmt->getBeginLoc());
            save.col = SrcMgr->getSpellingColumnNumber(stmt->getBeginLoc());
            save.file = SrcMgr->getFilename(stmt->getBeginLoc());
            if (recordPointer.size()!=0)
                if (!saveEquals(recordPointer.back(),save)&&save.file!="")
                    recordPointer.push_back(save);
        }
    }
    for (auto it = stmt->child_begin();it != stmt->child_end();it++){
        FindMalloc((*it),id);
    }
}

void FindSetNull(Stmt *stmt, int &id, clang::SourceLocation loc, bool &flag)
{
    if (flag)
        return ;
    if (stmt == nullptr)
        return ;
    if (strcmp(stmt->getStmtClassName(),"CallExpr")==0)
        return ;
    if (strcmp(stmt->getStmtClassName(),"ConditionalOperator")==0)
        return ;
    if (strcmp(stmt->getStmtClassName(),"DeclRefExpr")==0)
    {
        DeclRefExpr *declrefexpr = (DeclRefExpr *)(stmt);
        id = declrefexpr->getDecl()->getGlobalID();
    }
    if (strcmp(stmt->getStmtClassName(),"MemberExpr")==0)
    {
        flag = true;
        return ;
    }
    if (strcmp(stmt->getStmtClassName(),"CStyleCastExpr")==0)
    {
        CStyleCastExpr *cs = (CStyleCastExpr *)stmt;
        //printf("\033[33m%s\n\033[0m","************CStyleCast");
        //cout<<cs->getCastKindName()<<endl;;
        if (strcmp(cs->getCastKindName(),"NullToPointer")==0)
        {
            savePointer save;
            //save.id = declrefexpr->getDecl()->getGlobalID();
            save.id = id;
            save.declared = false;
            save.defined = false;
            save.setnull = true;
            save.derefered = false;
            save.line = SrcMgr->getSpellingLineNumber(loc);
            save.col = SrcMgr->getSpellingColumnNumber(loc);
            save.file = SrcMgr->getFilename(loc);
            if (recordPointer.size()!=0)
                if (!saveEquals(recordPointer.back(),save)&&save.file!="")
                    recordPointer.push_back(save);
            flag = true;
            return ;
        }
    }
    for (auto it = stmt->child_begin(); it!= stmt->child_end(); it++)
    {
        FindSetNull((*it),id,loc,flag);
    }
}

void FindMemberID(int &id, Stmt *stmt, savePointer &save){
    if(stmt == nullptr)
        return ;
    if (strcmp(stmt->getStmtClassName(),"DeclRefExpr")==0)
    {
        DeclRefExpr *declrefexpr = (DeclRefExpr *)(stmt);
        id = declrefexpr->getDecl()->getGlobalID();
        save.id = id;
        if (recordPointer.size()!=0)
            if (!saveEquals(recordPointer.back(),save)&&save.file!="")
                recordPointer.push_back(save);
    }
   for (auto it = stmt->child_begin(); it != stmt->child_end(); it++){
       FindMemberID(id,(*it),save);
   }
}

void FindUse(Stmt *stmt, int &id)
{
    //printf("\033[33m%s\n\033[0m","***FindUse");
    if (stmt == nullptr)
        return ;
    if (strcmp(stmt->getStmtClassName(),"DeclRefExpr")==0)
    {
        DeclRefExpr *declrefexpr = (DeclRefExpr *)(stmt);
        id = declrefexpr->getDecl()->getGlobalID();
        savePointer save;
        //save.id = declrefexpr->getDecl()->getGlobalID();
        save.id = id;
        save.declared = false;
        save.defined = true;
        save.setnull = false;
        save.derefered = false;
        save.line = SrcMgr->getSpellingLineNumber(stmt->getBeginLoc());
        save.col = SrcMgr->getSpellingColumnNumber(stmt->getBeginLoc());
        save.file = SrcMgr->getFilename(stmt->getBeginLoc());
        if (recordPointer.size()!=0)
            if (!saveEquals(recordPointer.back(),save)&&save.file!="")
                recordPointer.push_back(save);
    }
    if (strcmp(stmt->getStmtClassName(),"MemberExpr")==0)
    {
        //printf("\033[33m%s\n\033[0m","***MemberExpr");
        savePointer save;
        //save.id = declrefexpr->getDecl()->getGlobalID();
        save.id = id;
        save.declared = false;
        save.defined = false;
        save.setnull = false;
        save.derefered = true;
        save.line = SrcMgr->getSpellingLineNumber(stmt->getBeginLoc());
        save.col = SrcMgr->getSpellingColumnNumber(stmt->getBeginLoc());
        save.file = SrcMgr->getFilename(stmt->getBeginLoc());
        //printf("\033[33m%d\n\033[0m",save.line);
        FindMemberID(id,stmt,save);
    }
    if (strcmp(stmt->getStmtClassName(),"ArraySubscriptExpr")==0)
    {
        savePointer save;
        //save.id = declrefexpr->getDecl()->getGlobalID();
        save.id = id;
        save.declared = false;
        save.defined = false;
        save.setnull = false;
        save.derefered = true;
        save.line = SrcMgr->getSpellingLineNumber(stmt->getBeginLoc());
        save.col = SrcMgr->getSpellingColumnNumber(stmt->getBeginLoc());
        save.file = SrcMgr->getFilename(stmt->getBeginLoc());
        //printf("\033[33m%d\n\033[0m",save.line);
        FindMemberID(id,stmt,save);
    }
    for (auto it = stmt->child_begin(); it != stmt->child_end(); it++){
        FindUse((*it),id);
    }
}

void FindUnaryUse(Stmt *stmt,int &id)
{
    if (stmt == nullptr)
        return ;
    if (strcmp(stmt->getStmtClassName(),"UnaryOperator")==0){
        UnaryOperator *unopr = (UnaryOperator *)stmt;
        //printf("\033[33m%s\n\033[0m","***UnaryOP");
        //printf("\033[33m%d\n\033[0m",SrcMgr->getSpellingLineNumber(stmt->getBeginLoc()));
        if (unopr->getOpcodeStr(unopr->getOpcode()).str()=="*"){
            savePointer save;
            save.id = id;
            save.declared = false;
            save.defined = false;
            save.setnull = false;
            save.derefered = true;
            save.line = SrcMgr->getSpellingLineNumber(stmt->getBeginLoc());
            save.col = SrcMgr->getSpellingColumnNumber(stmt->getBeginLoc());
            save.file = SrcMgr->getFilename(stmt->getBeginLoc());
            FindMemberID(id,stmt,save);
        }
    }
    for (auto it = stmt->child_begin(); it != stmt->child_end(); it++){
        FindUnaryUse((*it),id);
    }
}

//鍙橀噺绫诲瀷
void FigureDeclStmt(Stmt *stmt)
{
    DeclStmt *dlstmt = (DeclStmt *)stmt;
    for (auto j = dlstmt->decl_begin(); j != dlstmt->decl_end(); ++j) {
        Decl *anydecl = *j;
		unsigned kind = anydecl->getKind();
        int tmp = -1;
        FindUnaryUse(stmt,tmp);
        if(kind==57)    //VarDecl
        {
          VarDecl* vardecl = (VarDecl *)anydecl;
          auto arrayType=vardecl->getType().getTypePtrOrNull();
          if(arrayType->isPointerType())        //鍙戠幇鎸囬拡鍙橀噺绫诲瀷
          {
            savePointer save;
            save.id = vardecl->getGlobalID();
            save.declared = true;
            save.defined = false;
            save.setnull = false;
            save.derefered = false;
            save.line = SrcMgr->getSpellingLineNumber(stmt->getBeginLoc());
            save.col = SrcMgr->getSpellingColumnNumber(stmt->getBeginLoc());
            save.file = SrcMgr->getFilename(stmt->getBeginLoc());
            recordPointer.push_back(save);
            FindMalloc(stmt,vardecl->getGlobalID());   
            bool flag = false;
            int id = vardecl->getGlobalID();
            FindSetNull(stmt,id,stmt->getBeginLoc(),flag);
          }   
        }
       
    } 

}
//浜屽厓鎿嶄綔绗︼紝涓昏鏄祴鍊兼搷浣?
void FigureBinOp(Stmt *stmt, bool IF)
{
    BinaryOperator *binop = (BinaryOperator *)stmt;
    for (auto it = stmt->child_begin();it != stmt->child_end(); it++)
    {
        int id = -1;
        bool flag = false;
        FindUse((*it), id);
        if(binop->getOpcode()==BO_Assign && IF)
            FindSetNull(stmt, id, binop->getBeginLoc(),flag);
    }
}
//鍑芥暟璋冪敤锛屾寚閽堝叧蹇僲alloc锛宖ree
void FigureCallExpr(Stmt *stmt)
{
    //printf("\033[33m%s\n\033[0m","***Callexpr");
    CallExpr *callexpr = (CallExpr *)stmt;
    for (auto j = callexpr->child_begin(); j != callexpr->child_end(); ++j) {
        // if(strcmp((*j)->getStmtClassName(),"DeclRefExpr")==0)
        // {
        //    // printf("\033[33m%s\n\033[0m","DeclRefExpr");
        //     DeclRefExpr *declrefexpr = (DeclRefExpr *)(*j);
        //     if(strcmp(declrefexpr->getDecl()->getName().data(),"malloc")==0)
        //     {
        //         printf("\033[33m%s\n\033[0m","******************malloc");
        //         savePointer save;
        //         save.id = declrefexpr->getDecl()->getGlobalID();
        //         save.declared = false;
        //         save.defined = true;
        //         save.setnull = false;
        //         save.derefered = false;
        //         save.line = SrcMgr->getSpellingLineNumber(stmt->getBeginLoc());
        //         save.col = SrcMgr->getSpellingColumnNumber(stmt->getBeginLoc());
        //         save.file = SrcMgr->getFilename(stmt->getBeginLoc());
        //         recordPointer.push_back(save);
        //     }
        //     if(strcmp(declrefexpr->getDecl()->getName().data(),"free")==0)
        //     {
        //         printf("\033[33m%s\n\033[0m","******************free");
        //         savePointer save;
        //         save.id = declrefexpr->getDecl()->getGlobalID();
        //         save.declared = false;
        //         save.defined = false;
        //         save.setnull = true;
        //         save.derefered = false;
        //         save.line = SrcMgr->getSpellingLineNumber(stmt->getBeginLoc());
        //         save.col = SrcMgr->getSpellingColumnNumber(stmt->getBeginLoc());
        //         save.file = SrcMgr->getFilename(stmt->getBeginLoc());
        //         recordPointer.push_back(save);
        //     }
        //}
    }
}


void Traverse_Ast(Stmt *stmt, bool flag, int &count){
    //cout<<"閬嶅巻AST涓?<<endl;;
   if (stmt == nullptr)
   {
       return ;
   }
   // 鍙橀噺澹版槑锛屾敹闆嗘寚閽堢被鍨?
    if(strcmp(stmt->getStmtClassName(),"DeclStmt")==0)
    {
        FigureDeclStmt(stmt);
    }
    // 浜屽厓鎿嶄綔锛?
    if (strcmp(stmt->getStmtClassName(),"BinaryOperator")==0)
    {
        FigureBinOp(stmt, true);
    }
    if (strcmp(stmt->getStmtClassName(),"IfStmt")==0)
    {
        if (strcmp(stmt->getStmtClassName(),"BinaryOperator")==0){
            FigureBinOp(stmt, false);
            return ;
        }
    }
     if (strcmp(stmt->getStmtClassName(),"CompoundStmt")==0 && count >0)
        return ;
    // malloc,free绛夋搷浣?
    // if (strcmp(stmt->getStmtClassName(),"CallExpr")==0)
    // {
    //     FigureCallExpr(stmt);
    // }

    // if (strcmp(stmt->getStmtClassName(),"DeclRefExpr")==0)
    // {
    //     FigureMF(stmt);
    // }
    count+=1;
   for (auto i = stmt->child_begin(); i != stmt->child_end(); i++)
   {
       Traverse_Ast(*i,flag,count);
   }
}

public:
void NPD_Entry(clang::FunctionDecl* fd)
{
    bool flag = false;
    int count = 0;
    SrcMgr = &(fd->getASTContext().getSourceManager());
    //TraverseStruct(fd->getBody(),SrcMgr);
    //auto fd_cfg=common::buildCFG(fd);
    string fd_name = fd->getNameAsString();
    Traverse_Ast(fd->getBody(),flag, count);
    listRecord();
    NPD_Single_Detect();
    //if (fd_name == "get_event_loop" || fd_name == "create_new_element" ||fd_name == "pack_into"|| fd_name =="instancemethod_getattro" || fd_name == "_PyObject_GenericGetAttrWithDict" )
    //if (fd_name == "_bytes_shared")
    // {
    //     //cout<<"***"<<fd_name<<"***"<<endl;
    //     fd->dump();
    //     Traverse_Ast(fd->getBody(),flag, count);
    //     //showSave();
    //     listRecord();
    //     NPD_Single_Detect();
    // }

    //CFGBlock* exit = &(fd_cfg->getExit());
    //VisitBlock(block,SrcMgr,exit);
    //showSave();
    vector <savePointer>().swap(recordPointer);
    record.clear();
    //int fid = fd->getGlobalID();
    //NPD_Detect();
}

void listRecord(){
    for (auto it = recordPointer.begin(); it != recordPointer.end(); it++){
        if (record.count(it->id)==0){
            std::vector<savePointer> tmp;
            tmp.push_back((*it));
            record.insert(make_pair(it->id,tmp));
        }
        else{
            record[it->id].push_back((*it));
        }
    }
}

void Detect_Ast()
{
    cout<<"detecting..."<<endl;
}

void NPD_Single_Detect(){
    for (auto it = record.begin();it != record.end();it++){
        //showSingleSave(it->second);
        if (it->second.size()>1){
            bool setnull = false;
            bool deref = false;
            bool define = false;
            int line0 = -1, line1 = -1;
            int col0 = -1, col1 = -1;
            string file0 = "", file1 = "";
            for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
            {
               if(it2->setnull){
                   setnull = true;
                   deref = false;
                   define = false;
                   line0 = it2->line;
                   col0 = it2->col;
                   file0 = it2->file;
               }
               else if (it2->defined){
                   setnull = false;
                   deref = false;
                   define = true;
                   line0 = -1;
                   col0 = -1;
                   file0 = "";
               }
               else if (it2->derefered){
                   if (setnull){
                        setnull = false;
                        deref = false;
                        define = true;
                        line1 = it2->line;
                        col1 = it2->col;
                        file1 = it2->file;
                        error_info* loc_set_null=new_error_info(NULL,file0,line0,col0,TYPE_NOTE,NPD_ERROR_TYPE_SET_NULL,npd);
                        error_info* loc_dereference=new_error_info(loc_set_null,file1,line1,col1,TYPE_ERROR,NPD_ERROR_TYPE_DEREFERENCE,npd);
                        result.push(loc_dereference);
                   }
               }
            }
        }
    }
}

void NPD_Detect(){
    //cout<<endl;
    //printf("\033[33m%s\n\033[0m","Detect NPD in the Function");
    int col0 = -1, line0=-1, col=-1,line=-1;
    string file0, file;
   // RED
    showSave();
    
    //CLOSE
    bool npd = false;
    //cout<<"I'm in detect of each route.\n";
    for (int i = 0; recordPointer.size()>0 && i<recordPointer.size()-1;i++){
      //  cout<<"**hello here!\n";
        int var_id = recordPointer[i].id;
        //cout<<var_id<<endl;
        bool flag = false;
        bool may = false;
        if (recordPointer[i].setnull){
            flag = true;
            file0 = recordPointer[i].file;
            line0 = recordPointer[i].line;
            col0 = recordPointer[i].col;
            //cout<<"setnull"<<" "<<line0<<" "<<may<<endl;
           // may = may || (find(ifBlock.begin(),ifBlock.end(),recordPointer[i].block)!=ifBlock.end());
        }
        if (recordPointer[i].defined){
            npd = false;
            flag = false;
            col0 = -1, line0=-1;
            file0 = " ";
            //cout<<"define"<<" "<<line<<" "<<may<<endl;
            //may = may || (find(ifBlock.begin(),ifBlock.end(),recordPointer[i].block)!=ifBlock.end());
        }
        int j = 0;
        //cout<<recordPointer.size()<<endl;
        for (j = i+1; j<recordPointer.size();j++){
            //cout<<j<<endl;
            if (j >= recordPointer.size())
                break;
            if (var_id == recordPointer[j].id){
                if (recordPointer[j].setnull){
                    flag = true;
                    file0 = recordPointer[j].file;
                    line0 = recordPointer[j].line;
                    col0 = recordPointer[j].col;
                   // may = may || (find(ifBlock.begin(),ifBlock.end(),recordPointer[j].block)!=ifBlock.end());
                   // cout<<"setnull"<<" "<<line0<<" "<<may<<endl;
                }
                if (recordPointer[j].defined){
                    npd = false;
                    flag = false;
                    col0 = -1, line0=-1;
                    file0 = " ";
                   // may = may || (find(ifBlock.begin(),ifBlock.end(),recordPointer[j].block)!=ifBlock.end());
                    //cout<<"define"<<" "<<line<<" "<<may<<endl;
                }
                if (recordPointer[j].derefered && flag){
                    //cout<<"NPD!\n";
                    file = recordPointer[j].file;
                    line = recordPointer[j].line;
                    col = recordPointer[j].col;
                   // may =  may ||(find(ifBlock.begin(),ifBlock.end(),recordPointer[j].block)!=ifBlock.end());
                    //cout<<"derefer"<<" "<<line<<" "<<may<<endl;
                    if (may){
                        error_info* loc_set_null=new_error_info(NULL,file0,line0,col0,TYPE_NOTE,NPD_ERROR_TYPE_SET_NULL,npd);
                        error_info* loc_dereference=new_error_info(loc_set_null,file,line,col,TYPE_ERROR,NPD_ERROR_TYPE_DEREFERENCE,npd);
                        result.push(loc_dereference);
                    }
                    else{
                        error_info* loc_set_null=new_error_info(NULL,file0,line0,col0,TYPE_NOTE,NPD_ERROR_TYPE_SET_NULL,npd);
                        error_info* loc_dereference=new_error_info(loc_set_null,file,line,col,TYPE_ERROR,NPD_ERROR_TYPE_DEREFERENCE,npd);
                        result.push(loc_dereference);
                    }
                    npd = true;
                    may = false;
                    break;
                }
            }
        }
        if(npd){
            i = j;
            npd = false;
        }
    }
    vector <savePointer>().swap(recordPointer);
 }
};



#endif
