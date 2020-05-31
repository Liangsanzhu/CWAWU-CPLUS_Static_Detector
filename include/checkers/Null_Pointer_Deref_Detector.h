#ifndef N_P_D_H
#define N_P_D_H
#include"Detector.h"
#include "assert.h"
class Null_Pointer_Detector{
private:

struct savePointer{
    int id;     //the global id of the pointer
    int block;  //the block id the pointer in
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
    cout<<setw(10)<<"BlockID";
    cout<<setw(10)<<"Line";
    cout<<setw(10)<<"Col";
    cout<<setw(10)<<"Declared";
    cout<<setw(10)<<"SetNull";
    cout<<setw(10)<<"Defined";
    cout<<setw(10)<<"Derefered";
    cout<<'\t'<<setw(10)<<"FileName"<<endl;
    for (auto re: recordPointer){
        cout<<setw(10)<<re.id;
        cout<<setw(10)<<re.block;
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

void showIf(std::vector<int>ifBlock){
    cout<<"If Block: ";
    for (auto i : ifBlock)
        cout<<i<<" ";
    cout<<endl<<endl;
}

void getVar(const Stmt*stmt,int&id,savePointer* &save){
    //cout<<"IN getVar!\n";
    if(strcmp(stmt->getStmtClassName(),"DeclRefExpr")==0){
        DeclRefExpr * ref = (DeclRefExpr *)stmt;
        string tmp = ref->getType().getAsString();
        if (tmp[tmp.length()-1]=='*'){
            save->id=ref->getDecl()->getGlobalID();
            id = save->id;
            //save->defined = true;
        }
        ValueStmt*F=(ValueStmt*)stmt;
        Expr*G=F->getExprStmt();
        DeclRefExpr*H=(DeclRefExpr*)G;
    }   

   if (strcmp(stmt->getStmtClassName(),"UnaryOperator")==0){
        UnaryOperator*unary = (UnaryOperator*)stmt;
        if(unary->getOpcodeStr(unary->getOpcode()).str()=="*")
            save->derefered = true;
        else if(unary->getOpcodeStr(unary->getOpcode()).str()=="&")
            save->defined = true;
    }

    if(strcmp(stmt->getStmtClassName(),"GNUNullExpr")==0){
        save->setnull = true;
        save->defined = false;
        GNUNullExpr *gnu = (GNUNullExpr *)stmt;
    }

    if(strcmp(stmt->getStmtClassName(),"CXXNewExpr")==0){
        CXXNewExpr *cxxnew = (CXXNewExpr*)stmt;
        save->defined = true;
        save->setnull = false;
    }

    for (auto it = stmt->child_begin();it!=stmt->child_end();it++){
        getVar(*it,id,save);
    }
}

void getMalloc(const Stmt*stmt, int &id, bool flag, savePointer* &save){ 
  for(auto bt=stmt->child_begin();bt!=stmt->child_end();bt++)
        getMalloc(*bt,id, flag, save);
    
  if(strcmp(stmt->getStmtClassName(),"DeclRefExpr")==0)
    {
      ValueStmt*F=(ValueStmt*)stmt;
      Expr*G=F->getExprStmt();
      DeclRefExpr*H=(DeclRefExpr*)G;
      id = H->getDecl()->getGlobalID();
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

void NPD_Traverse(SourceManager *SrcMgr, Stmt *stmt,int block_id){
    //cout<<"In NPD Traverse!\n";
    //std::cout<<stmt->getStmtClassName()<<std::endl;
    //DeclStmt
    if (strcmp(stmt->getStmtClassName(),"DeclStmt")==0){
        DeclStmt* declstmt=(DeclStmt*)stmt;
        VarDecl*vardecl=(VarDecl*)declstmt->getSingleDecl();
        //std::cout<<vardecl->getNameAsString()<<" "<<vardecl->getType().getAsString()<<" "<<vardecl->getGlobalID()<<std::endl;
        string tmp =vardecl->getType().getAsString();
        if (tmp[vardecl->getType().getAsString().length()-1]=='*'){
            savePointer *save= new savePointer;
            save->id=vardecl->getGlobalID();
            save->block = block_id;
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
        save->block = block_id;
        save->line=SrcMgr->getSpellingLineNumber(bin->getExprLoc());
        save->col=SrcMgr->getSpellingColumnNumber(bin->getExprLoc());
        save->file = SrcMgr->getFilename(bin->getBeginLoc());
       
        getVar(stmt, id, save);
        if (id != -1)
            recordPointer.push_back(*save);
        delete save;
    }

    if (strcmp(stmt->getStmtClassName(),"IfStmt")==0){
        IfStmt * ifstmt = (IfStmt *)stmt;
        //cout<<"IfStmt here!\n";
        //cout<<SrcMgr->getSpellingLineNumber(ifstmt->getBeginLoc())<<endl;
    }

    if (strcmp(stmt->getStmtClassName(),"CXXDeleteExpr")==0){
        CXXDeleteExpr *cxxdelt = (CXXDeleteExpr *)stmt;
        savePointer *save = new savePointer;
        int id = -1;
        save->defined = false;
        save->declared = false;
        save->setnull = true;
        save->derefered = false;
        save->block = block_id;
        save->line=SrcMgr->getSpellingLineNumber(cxxdelt->getExprLoc());
        save->col=SrcMgr->getSpellingColumnNumber(cxxdelt->getExprLoc());
        save->file = SrcMgr->getFilename(cxxdelt->getBeginLoc());
        getVar(stmt, id, save);
        if (id != -1)
            recordPointer.push_back(*save);
        delete save;
    }

    if (strcmp(stmt->getStmtClassName(),"CXXOperatorCallExpr")==0){
        CXXOperatorCallExpr * cxxcall = (CXXOperatorCallExpr *)stmt;
        savePointer *save = new savePointer;
        int id = -1;
        save->defined = false;
        save->declared = false;
        save->setnull = false;
        save->derefered = false;
        save->block = block_id;
        save->line=SrcMgr->getSpellingLineNumber(cxxcall->getExprLoc());
        save->col=SrcMgr->getSpellingColumnNumber(cxxcall->getExprLoc());
        save->file = SrcMgr->getFilename(cxxcall->getBeginLoc());
        getVar(stmt, id, save);
        if (id != -1)
            recordPointer.push_back(*save);
        delete save;
    }

    if (strcmp(stmt->getStmtClassName(),"CallExpr")==0){
        CallExpr * callexpr = (CallExpr *)stmt;
        savePointer *save = new savePointer;
        int id = -1;
        save->defined = false;
        save->declared = false;
        save->setnull = false;
        save->derefered = false;
        save->block = block_id;
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
        NPD_Traverse(SrcMgr,*it, block_id);
    }
}

void VisitBlock(clang::CFGBlock *block, SourceManager *SrcMgr, clang::CFGBlock *exit){
    //cout<<"Visit Block!\n";
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
			NPD_Traverse(SrcMgr,S,block_id);
		}
	}
    if(*block->succ_begin()!=nullptr)
        for (auto i = block->succ_begin();i!=block->succ_end();i++)
            VisitBlock(*i, SrcMgr, exit);
}

public:

void NPD_Entry(SourceManager *SrcMgr,Stmt* stmt,int idx, defuse_node*ano, int block_id)
{
    RED
    //cout<<"In NPD entry!\n";
    //cout<<fd->getNameAsString()<<endl;
    CLOSE
    route = idx;
    all_node = ano;
    NPD_Traverse(SrcMgr, stmt, block_id);
    //int fid = fd->getGlobalID();

}

void NPD_Detect(std::vector<int>ifBlock){
    //cout<<endl;
    //printf("\033[33m%s\n\033[0m","Detect NPD in the Function");
    int col0 = -1, line0=-1, col=-1,line=-1;
    string file0, file;
    RED
   /*  showSave();
    showIf(ifBlock); */
    CLOSE
    bool npd = false;
    cout<<"I'm in detect of each route.\n";
    for (int i = 0; recordPointer.size()>0 && i<recordPointer.size()-1;i++){
        cout<<"**hello here!\n";
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
            may = may || (find(ifBlock.begin(),ifBlock.end(),recordPointer[i].block)!=ifBlock.end());
        }
        if (recordPointer[i].defined){
            npd = false;
            flag = false;
            col0 = -1, line0=-1;
            file0 = " ";
            //cout<<"define"<<" "<<line<<" "<<may<<endl;
            may = may || (find(ifBlock.begin(),ifBlock.end(),recordPointer[i].block)!=ifBlock.end());
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
                    may = may || (find(ifBlock.begin(),ifBlock.end(),recordPointer[j].block)!=ifBlock.end());
                    cout<<"setnull"<<" "<<line0<<" "<<may<<endl;
                }
                if (recordPointer[j].defined){
                    npd = false;
                    flag = false;
                    col0 = -1, line0=-1;
                    file0 = " ";
                    may = may || (find(ifBlock.begin(),ifBlock.end(),recordPointer[j].block)!=ifBlock.end());
                    //cout<<"define"<<" "<<line<<" "<<may<<endl;
                }
                if (recordPointer[j].derefered && flag){
                    //cout<<"NPD!\n";
                    file = recordPointer[j].file;
                    line = recordPointer[j].line;
                    col = recordPointer[j].col;
                    may =  may ||(find(ifBlock.begin(),ifBlock.end(),recordPointer[j].block)!=ifBlock.end());
                    cout<<"derefer"<<" "<<line<<" "<<may<<endl;
                    if (may){
                        error_info* loc_set_null=new_error_info(NULL,file0,line0,col0,TYPE_NOTE,NPD_ERROR_TYPE_SET_NULL,route);
                        error_info* loc_dereference=new_error_info(loc_set_null,file,line,col,TYPE_ERROR,NPD_ERROR_TYPE_DEREFERENCE, route);
                        result.push(loc_dereference);
                    }
                    else{
                        error_info* loc_set_null=new_error_info(NULL,file0,line0,col0,TYPE_NOTE,NPD_ERROR_TYPE_SET_NULL, route);
                        error_info* loc_dereference=new_error_info(loc_set_null,file,line,col,TYPE_ERROR,NPD_ERROR_TYPE_DEREFERENCE, route);
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