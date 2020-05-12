#ifndef N_P_D_H
#define N_P_D_H
#include"Detector.h"

struct NPDSave{
    const char * className;       //className
    int line;
    int col;
    string fileName;
    string varType;
    string varName;        //name or value
    string varHand;
    string opr;
};
vector <NPDSave> myRecord;

void NPD_Detect(){
    //cout<<endl;
    //printf("\033[33m%s\n\033[0m","Detect NPD in the Function");
    int col0 = -1, line0=-1, col=-1,line=-1;
    string file0, file;
    for (int re=0; re<myRecord.size();re++){
       
        if(strcmp(myRecord[re].className,"DeclRefExpr")==0 && '*'==(myRecord[re].varType[myRecord[re].varType.length()-1])){     //a pointer
           // //cout<<"There is a pointer!\n";
            bool flag = false;
            if(strcmp(myRecord[re+1].className,"GNUNullExpr")==0){//has been set to null
               // //cout<<"The pointer was null!\n";
                flag = true;
                col0 = myRecord[re].col;
                line0 = myRecord[re].line;
                file0 = myRecord[re].fileName;
            }  
                
            for (int i = re+1;i<myRecord.size();i++){
                if (strcmp(myRecord[i].className,"UnaryOperator")==0){
                    if (myRecord[i].opr=="*"){
                        if (flag ==true)
                            {
                            col=myRecord[i].col;
                            line = myRecord[i].line;
                            file = myRecord[i].fileName;
                             //(line0,col0,line,col,file0,file);
                              error_info*loc_set_null=new_error_info(NULL,file0,line0,col0,TYPE_NOTE,NPD_ERROR_TYPE_SET_NULL);
                            error_info* loc_dereference=new_error_info(loc_set_null,file,line,col,TYPE_ERROR,NPD_ERROR_TYPE_DEREFERENCE);
                           
                            result.push(loc_dereference);
                            }
                    }
                    if (myRecord[i].opr=="&")
                        flag = false;
                }
            }
        }

           
    }
}


void ReportNPD(int LineNull,int ColNull,int LineDe,int ColDe,string FileNull,string FileDe){
    if(ColNull!=-1 && ColDe!=-1){
        //cout<<"-----------------Find Null Pointer Dereference Fault!-----------------\n";
        //cout<<"Set Null or Delete at <Line "<<LineNull<<", Col "<<ColNull<<"> at File: "<<FileNull<<endl;
        //cout<<"Dereference at <Line "<<LineDe<<", Col "<<ColDe<<"> at File: "<<FileDe<<endl;
        //cout<<"--------------------------------------------------------------------------------\n";
    }
}

void TraverseRecord(){
    //cout<<setw(20)<<"ClassName";
    //cout<<setw(10)<<"Line";
    //cout<<setw(10)<<"Col";
    //cout<<setw(15)<<"VarType";
    //cout<<setw(10)<<"varName";
    //cout<<setw(15)<<"varHand";
    //cout<<setw(10)<<"Operator";
    //cout<<'\t'<<setw(20)<<"FileName"<<endl;
    for (auto re: myRecord){
        //cout<<setw(20)<<re.className;
        //cout<<setw(10)<<re.line;
        //cout<<setw(10)<<re.col;
        //cout<<setw(15)<<re.varType;
        //cout<<setw(10)<<re.varName;
        //cout<<setw(15)<<re.varHand;
        //cout<<setw(10)<<re.opr;
        //cout<<'\t'<<setw(20)<<re.fileName<<endl;
    }
}


void TraverseStruct(Stmt * my_stmt, SourceManager *SrcMgr) {
  if (my_stmt != nullptr) {

      // myRecord
      NPDSave *npdSave = new NPDSave;
      //unsigned my_loc
    CompoundStmt *cmpstmt;      //8
    DeclStmt *dlstmt;           //12
    NullStmt *nullstmt;         //19
    ReturnStmt *returnstmt;     //74
    BinaryOperator *bitopr;     //92
    CXXDeleteExpr *cdex;        //101
    CXXNewExpr * cnewexpr;      //105
    CallExpr *callexpr;         //116
    CXXOperatorCallExpr *coprcall;      //119
    ImplicitCastExpr *impcastexpr;      //129
    DeclRefExpr *dre;           //136
    GNUNullExpr *gnuxexpr;      //148
    IntegerLiteral *intlit;     //153
    UnaryOperator *unopr;       //195

   // Decl *locstmt;

    unsigned stmtclass = my_stmt->getStmtClass();
    switch (stmtclass) {
        case 8: //compoundstmt
            ////cout<<"In cmpstmt!\n";
            cmpstmt = (CompoundStmt *)my_stmt;
            ////cout<<setw(20)<<SrcMgr->getFilename(cmpstmt->getBeginLoc());
            npdSave->className = cmpstmt->getStmtClassName();
            npdSave->line = SrcMgr->getSpellingLineNumber(cmpstmt->getBeginLoc());
            npdSave->col = SrcMgr->getSpellingColumnNumber(cmpstmt->getBeginLoc());
            npdSave->fileName = SrcMgr->getFilename(cmpstmt->getBeginLoc()).str();
            npdSave->varName = "NULL";
            npdSave->varType = "NULL";
            npdSave->varHand = "NULL";
            npdSave->opr = "NULL";
            myRecord.push_back(*npdSave);
            ////printf("\033[33m%20s\033[0m",cmpstmt->getStmtClassName());

            for (auto i = cmpstmt->body_begin(); i != cmpstmt->body_end()&&(*i)!=nullptr; i++) {
                TraverseStruct(*i,SrcMgr);
            }
            ////cout<<"Out cmpstmt!\n";
            break;
        case 12: //declstmt
            dlstmt = (DeclStmt *)my_stmt;
            ////cout<<"In declstmt!\n";
    
            ////printf("\033[33m%20s\033[0m",dlstmt->getStmtClassName());
            for (auto j = dlstmt->decl_begin(); j != dlstmt->decl_end()&&j!=nullptr; ++j){
                if (j != nullptr) {        //vardecl
                    VarDecl *vardecl;
                    unsigned kind = (*j)->getKind();
                    
                if(kind ==57){
                    npdSave->className = dlstmt->getStmtClassName();
                    npdSave->line = SrcMgr->getSpellingLineNumber(dlstmt->getBeginLoc());
                    npdSave->col = SrcMgr->getSpellingColumnNumber(dlstmt->getBeginLoc());
                    npdSave->fileName = SrcMgr->getFilename(dlstmt->getBeginLoc()).str();
                    npdSave->varName = "NULL";
                    npdSave->varType = "NULL";
                    npdSave->varHand = "NULL";
                    npdSave->opr = "NULL";
                    vardecl = (VarDecl *)(*j);
                    //npdSave->varName = vardecl->getQualifiedNameAsString();
                    npdSave->varType = vardecl->getType().getAsString();
                    npdSave->varName = vardecl->getQualifiedNameAsString();
                   /*  //cout<<"***Strange here!";
                    //cout << vardecl->getQualifiedNameAsString() << vardecl->getType().getAsString() ;
                    //cout << endl;  */
                    myRecord.push_back(*npdSave);
                    if (vardecl->ensureEvaluatedStmt() != nullptr)
                        TraverseStruct(vardecl->ensureEvaluatedStmt()->Value, SrcMgr);
                }
                } 
            }

            break;
        
        case 19:   //nullstmt
            ////cout<<"In nullstmt!\n";
            nullstmt = (NullStmt*)my_stmt;
            ////cout<<setw(20)<<nullstmt->getStmtClassName()<<endl;
            npdSave->className = nullstmt->getStmtClassName();
            npdSave->line = SrcMgr->getSpellingLineNumber(nullstmt->getBeginLoc());
            npdSave->col = SrcMgr->getSpellingColumnNumber(nullstmt->getBeginLoc());
            npdSave->fileName = SrcMgr->getFilename(nullstmt->getBeginLoc()).str();
            npdSave->varName = "NULL";
            npdSave->varType = "NULL";
            npdSave->varHand = "NULL";
            npdSave->opr = "NULL";
            ////printf("\033[33m%20s\033[0m",nullstmt->getStmtClassName());
            for(auto i = nullstmt->child_begin(); i != nullstmt->child_end()&&(*i)!=nullptr; i++)
                TraverseStruct(*i, SrcMgr);

            myRecord.push_back(*npdSave);
            ////cout<<"Out cmpstmt!\n";
            break;

        case 74:    //returnstmt
            ////cout<<"In returnstmt!\n";
            returnstmt = (ReturnStmt *)my_stmt;
            ////cout<<setw(20)<<returnstmt->getStmtClassName()<<endl;
            npdSave->className = returnstmt->getStmtClassName();
            npdSave->line = SrcMgr->getSpellingLineNumber(returnstmt->getBeginLoc());
            npdSave->col = SrcMgr->getSpellingColumnNumber(returnstmt->getBeginLoc());
            npdSave->fileName = SrcMgr->getFilename(returnstmt->getBeginLoc()).str();
            npdSave->varName = "NULL";
            npdSave->varType = "NULL";
            npdSave->varHand = "NULL";
            npdSave->opr = "NULL";

            myRecord.push_back(*npdSave);
            for(auto i = returnstmt->child_begin();i!=returnstmt->child_end()&&(*i)!=nullptr;i++)
                TraverseStruct(*i, SrcMgr);
            ////cout<<"Out returnstmt!\n";
            break;

        case 92:    //BinaryOperator
            ////cout<<"In binaryopr!\n";
            bitopr = (BinaryOperator*)my_stmt;
            ////cout<<setw(20)<<bitopr->getStmtClassName()<<": ";
            npdSave->className = bitopr->getStmtClassName();
            npdSave->line = SrcMgr->getSpellingLineNumber(bitopr->getBeginLoc());
            npdSave->col = SrcMgr->getSpellingColumnNumber(bitopr->getBeginLoc());
            npdSave->fileName = SrcMgr->getFilename(bitopr->getBeginLoc()).str();
            npdSave->varName = "NULL";
            npdSave->varType = bitopr->getType().getAsString();
            
            npdSave->opr = bitopr->getOpcodeStr().str();
           // //printf("\033[33m%20s\033[0m",bitopr->getStmtClassName());
            switch(bitopr->getValueKind()){
                case 0:
                    ////cout<<setw(20)<<" VK_RValue";
                    npdSave->varHand = "VK_RValue";
                    break;
                case 1:
                    ////cout<<setw(20)<<" VK_LValue";
                    npdSave->varHand = "VK_LValue";
                    break;
                case 2:
                    ////cout<<setw(20)<<" VK_XValue";
                    npdSave->varHand = "VK_XValue";
                    break;
            }

            myRecord.push_back(*npdSave);
            for (auto i = bitopr->child_begin(); i != bitopr->child_end()&&(*i)!=nullptr; ++i)
                TraverseStruct(*i, SrcMgr);
            ////cout<<"Out binaryopr!\n";
            break;

        case 101:       //CXXDeleteExpr
            ////cout<<"In cxxdeleteexpr!\n";
            cdex = (CXXDeleteExpr *)my_stmt;
             npdSave->className = cdex->getStmtClassName();
            npdSave->line = SrcMgr->getSpellingLineNumber(cdex->getBeginLoc());
            npdSave->col = SrcMgr->getSpellingColumnNumber(cdex->getBeginLoc());
            npdSave->fileName = SrcMgr->getFilename(cdex->getBeginLoc()).str();
            npdSave->varName = "NULL";
            npdSave->varType = "NULL";
            npdSave->varHand = "NULL";
            npdSave->opr = "NULL";
           // //printf("\033[33m%20s\033[0m",cdex->getStmtClassName());
            ////cout<<setw(20)<<cdex->getType().getAsString();
            switch(cdex->getValueKind()){
                case 0:
                    ////cout<<setw(20)<<" VK_RValue";
                    npdSave->varHand = "VK_RValue";
                    break;
                case 1:
                    ////cout<<setw(20)<<" VK_LValue";
                    npdSave->varHand = "VK_LValue";
                    break;
                case 2:
                    ////cout<<setw(20)<<" VK_XValue";
                    npdSave->varHand = "VK_XValue";
                    break;
            }

            myRecord.push_back(*npdSave);
            ////cout<<"Out cxxdeleteexpr!\n";
            break;

        case 105:       //CXXNewExpr
        ////cout<<"In cxxnewexpr!\n";
            cnewexpr = (CXXNewExpr *)my_stmt;
            npdSave->className = cnewexpr->getStmtClassName();
            npdSave->line = SrcMgr->getSpellingLineNumber(cnewexpr->getBeginLoc());
            npdSave->col = SrcMgr->getSpellingColumnNumber(cnewexpr->getBeginLoc());
            npdSave->fileName = SrcMgr->getFilename(cnewexpr->getBeginLoc()).str();
            npdSave->varName = "NULL";
            npdSave->varType = "NULL";
            npdSave->varHand = "NULL";
            npdSave->opr = "NULL";
            ////printf("\033[33m%20s\033[0m",cnewexpr->getStmtClassName());
            ////cout<<setw(20)<<cnewexpr->getType().getAsString();
            switch(cnewexpr->getValueKind()){
                case 0:
                    ////cout<<setw(20)<<" VK_RValue";
                    npdSave->varHand = "VK_RValue";
                    break;
                case 1:
                    ////cout<<setw(20)<<" VK_LValue";
                    npdSave->varHand = "VK_LValue";
                    break;
                case 2:
                   // //cout<<setw(20)<<" VK_XValue";
                    npdSave->varHand = "VK_XValue";
                    break;
            }

            myRecord.push_back(*npdSave);
            ////cout<<"Out cxxnewexpr!\n";
            break;

        case 116:       //CallExpr
         ////cout<<"In callexpr!\n";
            callexpr = (CallExpr *)my_stmt;
            ////cout<<setw(20)<<callexpr->getStmtClassName()<<":"
            npdSave->className = callexpr->getStmtClassName();
            npdSave->line = SrcMgr->getSpellingLineNumber(callexpr->getBeginLoc());
            npdSave->col = SrcMgr->getSpellingColumnNumber(callexpr->getBeginLoc());
            npdSave->fileName = SrcMgr->getFilename(callexpr->getBeginLoc()).str();
            npdSave->varName = "NULL";
            npdSave->varType = callexpr->getType().getAsString();
            npdSave->varHand = "NULL";
            npdSave->opr = "NULL";
           
            myRecord.push_back(*npdSave);
            for (auto i = callexpr->child_begin(); i != callexpr->child_end()&&(*i)!=nullptr; ++i)
                TraverseStruct(*i, SrcMgr);
            ////cout<<"Out callexpr!\n";
            break;
        
        case 119: //CXXOperatorCallExpr
        ////cout<<"In cxxoprcallexpr!\n";
            coprcall = (CXXOperatorCallExpr *)my_stmt;
            npdSave->className = coprcall->getStmtClassName();
            npdSave->line = SrcMgr->getSpellingLineNumber(coprcall->getBeginLoc());
            npdSave->col = SrcMgr->getSpellingColumnNumber(coprcall->getBeginLoc());
            npdSave->fileName = SrcMgr->getFilename(coprcall->getBeginLoc()).str();
            npdSave->varName = "NULL";
            npdSave->varType = "NULL";//coprcall->getType().getAsString();
            npdSave->varHand = "NULL";
            npdSave->opr = "NULL";
            ////printf("\033[33m%20s\033[0m",coprcall->getStmtClassName());
            ////cout<<setw(20)<<coprcall->getType().getAsString();
            switch(coprcall->getValueKind()){
                case 0:
                    ////cout<<setw(20)<<" VK_RValue";
                    npdSave->varHand = "VK_RValue";
                    break;
                case 1:
                    ////cout<<setw(20)<<" VK_LValue";
                    npdSave->varHand = "VK_LValue";
                    break;
                case 2:
                    ////cout<<setw(20)<<" VK_XValue";
                    npdSave->varHand = "VK_XValue";
                    break;
            }

            myRecord.push_back(*npdSave);
           // //cout<<"Out cxxoprcallexpr!\n";
            break;
        
        case 129:      //ImplicitCastExpr
         ////cout<<"In implicitcastexpr!\n";
            impcastexpr = (ImplicitCastExpr *)my_stmt;
            npdSave->className = impcastexpr->getStmtClassName();
            npdSave->line = SrcMgr->getSpellingLineNumber(impcastexpr->getBeginLoc());
            npdSave->col = SrcMgr->getSpellingColumnNumber(impcastexpr->getBeginLoc());
            npdSave->fileName = "NULL";
            npdSave->varName = "NULL";
            npdSave->varType = "NULL";
            npdSave->varHand = "NULL";
            npdSave->opr = "NULL";
        
            for (auto i = impcastexpr->child_begin(); i != impcastexpr->child_end()&&(*i)!=nullptr; ++i)
                TraverseStruct(*i, SrcMgr);
            ////cout<<"Out implicitcastexpr!\n";
            myRecord.push_back(*npdSave);
            break;

        case 136:       //DeclRefExpr
        ////cout<<"In declrefexpr!\n";
            dre = (DeclRefExpr *)my_stmt;
            ////cout<<setw(20)<<dre->getStmtClassName()<<": ";
            npdSave->className = dre->getStmtClassName();
            npdSave->line = SrcMgr->getSpellingLineNumber(dre->getBeginLoc());
            npdSave->col = SrcMgr->getSpellingColumnNumber(dre->getBeginLoc());
            npdSave->fileName = SrcMgr->getFilename(dre->getBeginLoc()).str();
            npdSave->varName = dre->getDecl()->getNameAsString();
            npdSave->varType = dre->getType().getAsString();
            npdSave->varHand = "NULL";
            npdSave->opr = "NULL";
            ////printf("\033[33m%20s\033[0m",dre->getStmtClassName());
            switch(dre->getValueKind()){
               case 0:
                    ////cout<<setw(20)<<" VK_RValue";
                    npdSave->varHand = "VK_RValue";
                    break;
                case 1:
                    ////cout<<setw(20)<<" VK_LValue";
                    npdSave->varHand = "VK_LValue";
                    break;
                case 2:
                    ////cout<<setw(20)<<" VK_XValue";
                    npdSave->varHand = "VK_XValue";
                    break;
            }

            myRecord.push_back(*npdSave);
            for (auto i = dre->child_begin(); i != dre->child_end()&&(*i)!=nullptr; ++i)
                TraverseStruct(*i, SrcMgr);
          // //cout<<"Out declrefexpr!\n";
            break;

        case 148:       //GNUNullExpr
      // //cout<<"In GNUnull\n";
            gnuxexpr = (GNUNullExpr *)my_stmt;
            npdSave->className = gnuxexpr->getStmtClassName();
            npdSave->line = SrcMgr->getSpellingLineNumber(gnuxexpr->getBeginLoc());
            npdSave->col = SrcMgr->getSpellingColumnNumber(gnuxexpr->getBeginLoc());
            npdSave->fileName = SrcMgr->getFilename(gnuxexpr->getBeginLoc()).str();
            npdSave->varName = "NULL";
            npdSave->varType = gnuxexpr->getType().getAsString();
            npdSave->varHand = "NULL";
            npdSave->opr = "NULL";
            //DO NOT get VALUEKIND here!
            myRecord.push_back(*npdSave);
            for (auto i = gnuxexpr->child_begin(); i != gnuxexpr->child_end()&&(*i)!=nullptr; ++i)
                TraverseStruct(*i, SrcMgr);
           // //cout<<"Out GNUnull!\n";
            break;

        case 153:       //IntegerLiteral
       // //cout<<"In integer!\n";
            intlit = (IntegerLiteral*)my_stmt;
            npdSave->className = intlit->getStmtClassName();
            npdSave->line = SrcMgr->getSpellingLineNumber(intlit->getBeginLoc());
            npdSave->col = SrcMgr->getSpellingColumnNumber(intlit->getBeginLoc());
            npdSave->fileName = SrcMgr->getFilename(intlit->getBeginLoc()).str();
            npdSave->varName = to_string(intlit->getValue().getSExtValue());
            npdSave->varType = intlit->getType().getAsString();
            npdSave->varHand = "NULL";
            npdSave->opr = "NULL";
            ////cout<<setw(20)<<intlit->getStmtClassName()<<": ";
           // //printf("\033[33m%20s\033[0m",intlit->getStmtClassName());
            switch(intlit->getValueKind()){
                case 0:
                    ////cout<<setw(20)<<" VK_RValue";
                    npdSave->varHand = "VK_RValue";
                    break;
                case 1:
                    ////cout<<setw(20)<<" VK_LValue";
                    npdSave->varHand = "VK_LValue";
                    break;
                case 2:
                    ////cout<<setw(20)<<" VK_XValue";
                    npdSave->varHand = "VK_XValue";
                    break;
            }

            myRecord.push_back(*npdSave);
            for (auto i = intlit->child_begin(); i != intlit->child_end()&&(*i)!=nullptr; ++i)
                TraverseStruct(*i, SrcMgr);
           // //cout<<"Out integer!\n";
            break;

        case 195:       //UnaryOperator
       // //cout<<"In unaryopr!\n";
            unopr = (UnaryOperator *)my_stmt;
            npdSave->className = unopr->getStmtClassName();
            npdSave->line = SrcMgr->getSpellingLineNumber(unopr->getBeginLoc());
            npdSave->col = SrcMgr->getSpellingColumnNumber(unopr->getBeginLoc());
            npdSave->fileName = SrcMgr->getFilename(unopr->getBeginLoc()).str();
            npdSave->varName = "NULL";
            npdSave->varType = unopr->getType().getAsString();
            npdSave->varHand = "NULL";
            npdSave->opr = unopr->getOpcodeStr(unopr->getOpcode()).str();
            ////cout<<setw(20)<<unopr->getStmtClassName()<<": ";
            ////printf("\033[33m%20s\033[0m",unopr->getStmtClassName());
            switch(unopr->getValueKind()){
               case 0:
                   // //cout<<setw(20)<<" VK_RValue";
                    npdSave->varHand = "VK_RValue";
                    break;
                case 1:
                   // //cout<<setw(20)<<" VK_LValue";
                    npdSave->varHand = "VK_LValue";
                    break;
                case 2:
                   // //cout<<setw(20)<<" VK_XValue";
                    npdSave->varHand = "VK_XValue";
                    break;
            }

          //  //cout<<"Out unaryopr!\n";
            myRecord.push_back(*npdSave);
            break;
        
        default: 
            //cout <<setw(20)<< "***Unclassified now***  "<< my_stmt->getStmtClass()<<" "<< my_stmt->getStmtClassName()<< endl;
        break;
    }
    delete npdSave;
  }
}
void NPD_Entry(clang::FunctionDecl* fd)
{
    SourceManager *SrcMgr = &(fd->getASTContext().getSourceManager());
    TraverseStruct(fd->getBody(),SrcMgr);
    TraverseRecord();
    //NPD_Detect();
}
#endif