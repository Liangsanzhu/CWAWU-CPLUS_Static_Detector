#ifndef V_U_H
#define V_U_H
#include"Detector.h"
#include "TemplateCheckerSinglePath.h"
#include<stack>
#include<algorithm>
/*
struct Info {
  std::string varname;
  int def_line;
  Stmt *def_stmt;
  Decl *def_decl;
  int defined;
  //tmp
  int block_id;
  int var_id;
  SourceLocation location;
};
*/


class VariableUndefined{
public:
struct ResVar{
  std::string name;
  long id;
  SourceLocation location;
  Decl* decl;
};

//std::map<FunctionDecl *, std::map<int, std::vector<CFGBlock *>>> path_tree;
//extern std::map<FunctionDecl *, std::unique_ptr<clang::CFG>> cfg_path;

//void TraverseDecl(Decl *anydecl, int count, SourceManager *scm);
//void TraverseStmt(Stmt * anystmt, int count, SourceManager *scm);
defuse_node all_node;
bool definition = false;
bool unary_opcode = false;
bool integ = false;
bool callee = false;
bool call_def = false;
bool member_flag = false;
bool unary_ptr = false;
bool member_ptr = false;
//block_id, varID, Info
std::map<int, std::map<int, Info>> def_tmp;
std::map<int, std::map<std::pair<int, int>, Info>> struct_def_tmp;
std::vector<Info> assign_tmp;
std::vector<struct_Info> struct_assign_tmp;
bool saved = true;
std::map<int, Info> global_def;
//std::map<std::string, bool> decl_or_def;
int block_id = 1;
std::vector<int> reverse_path;
std::stack<CFGBlock *> stack_path;
std::string g_varname;
std::string cur_funcname;
FunctionDecl *cur_fd;
//Detector detec;
int g_id;
Stmt *cur_stmt;
Decl *cur_decl;
//test
//SourceLocation sl;
int idx = 0;
//memberid to dlrefexpr
int member_in_id;
std::string member_in_name;
std::string member_in_type;

Info info_tmp;
struct_Info struct_info_tmp;
SourceManager *srcmng;

void output_deftmp() {
  //def_tmp
  for (auto i = def_tmp.begin(); i != def_tmp.end(); ++i) {
    std::cout << "blockid:"<<i->first << "\n";
    for (auto ii = i->second.begin(); ii != i->second.end(); ++ii) {
      std::cout << "VarID:"<<ii->first << "  ";
      std::cout << "name: "<<ii->second.varname << "  defline:" << ii->second.def_line << "\n";
    }
  }
}

void TraverseDecl(Decl *anydecl, int count, SourceManager *scm) {
  if (anydecl != nullptr) {
    //for (int i = 0; i < count; i++)
      //printf(" ");
    VarDecl *vardecl;
    unsigned line = 0;
    unsigned cl = 0;
    unsigned kind = anydecl->getKind();

    CXXRecordDecl* cxxrd;
    ResVar* res;

    switch (kind)
    {
    case 57: //vardecl
      vardecl = (VarDecl *)anydecl;
      line = vardecl->getASTContext().getSourceManager().getSpellingLineNumber(vardecl->getLocation());
      cl = vardecl->getASTContext().getSourceManager().getSpellingColumnNumber(vardecl->getLocation());
      //std::cout << vardecl->getQualifiedNameAsString() << " " << vardecl->getType().getAsString() << std::endl;
      //std::cout << vardecl->getLocation().printToString(*scm) << "\n";
      all_node.add_var(vardecl->getID(), vardecl->getQualifiedNameAsString(), vardecl->getType().getAsString());
      //all_node.add_du(vardecl->getID(), line, 0, cur_funcname, cur_stmt, nullptr);
      //add
      if (block_id == -1) {
        global_def[vardecl->getID()].def_line = line;
        global_def[vardecl->getID()].def_cl = cl;
        global_def[vardecl->getID()].varname = vardecl->getQualifiedNameAsString();
        global_def[vardecl->getID()].def_decl = cur_decl;
        global_def[vardecl->getID()].defined = 0;
      }
      else {
        def_tmp[block_id][vardecl->getID()].varname = vardecl->getQualifiedNameAsString();
        def_tmp[block_id][vardecl->getID()].def_line = line;
        def_tmp[block_id][vardecl->getID()].def_cl = cl;
        def_tmp[block_id][vardecl->getID()].def_stmt = cur_stmt;
        def_tmp[block_id][vardecl->getID()].defined = 0;
      }
      
      //output_deftmp();
      //std::cout << count << std::endl;
    if ((vardecl->ensureEvaluatedStmt() != nullptr)) {
        ++count;
        if (vardecl->ensureEvaluatedStmt()->Value != nullptr) {
          //if (strcmp(vardecl->ensureEvaluatedStmt()->Value->getStmtClassName(), "IntegerLiteral") == 0
            //|| strcmp(vardecl->ensureEvaluatedStmt()->Value->getStmtClassName(), "InitListExpr") == 0) {
          if((strcmp(vardecl->ensureEvaluatedStmt()->Value->getStmtClassName(), "CXXConstructExpr") != 0)) {
            integ = true;
            g_varname = vardecl->getQualifiedNameAsString();
            g_id = vardecl->getID();
            if (block_id == -1)
              global_def[g_id].defined = 1;
            else
              def_tmp[block_id][g_id].defined = 1;

            all_node.add_defuse_ln(g_id, idx, vardecl->getLocation(), line, 0, cl, 0);
            all_node.add_funcname(g_id, idx, vardecl->getLocation(), cur_funcname);
            all_node.add_defuse_stmt(g_id, idx, vardecl->getLocation(), cur_stmt, nullptr);
            all_node.add_blockid(g_id, idx, vardecl->getLocation(), block_id);
            all_node.add_defined(g_id, idx, vardecl->getLocation(), 1);
          }
          else {
            all_node.add_defuse_ln(vardecl->getID(), idx, vardecl->getLocation(), line, 0, cl, 0);
            all_node.add_funcname(vardecl->getID(), idx, vardecl->getLocation(), cur_funcname);
            all_node.add_defuse_stmt(vardecl->getID(), idx, vardecl->getLocation(), cur_stmt, nullptr);
            all_node.add_blockid(vardecl->getID(), idx, vardecl->getLocation(), block_id);
            all_node.add_defined(vardecl->getID(), idx, vardecl->getLocation(), 0);
          }
          //std::cout << vardecl->ensureEvaluatedStmt()->Value->getStmtClassName()<<"\n";
          TraverseStmt(vardecl->ensureEvaluatedStmt()->Value, count, scm);
        }
        else
        {
          integ = false;
          all_node.add_defuse_ln(vardecl->getID(), idx, vardecl->getLocation(), line, 0, cl, 0);
          all_node.add_funcname(vardecl->getID(), idx, vardecl->getLocation(), cur_funcname);
          all_node.add_defuse_stmt(vardecl->getID(), idx, vardecl->getLocation(), cur_stmt, nullptr);
          all_node.add_blockid(vardecl->getID(), idx, vardecl->getLocation(), block_id);
          all_node.add_defined(vardecl->getID(), idx, vardecl->getLocation(), 0);
        }
        //TraverseStmt(vardecl->ensureEvaluatedStmt()->Value, count, scm);
        
    }
    else {
        all_node.add_defuse_ln(vardecl->getID(), idx, vardecl->getLocation(), line, 0, cl, 0);
        all_node.add_funcname(vardecl->getID(), idx, vardecl->getLocation(), cur_funcname);
        all_node.add_defuse_stmt(vardecl->getID(), idx, vardecl->getLocation(), cur_stmt, nullptr);
        all_node.add_blockid(vardecl->getID(), idx, vardecl->getLocation(), block_id);
        all_node.add_defined(vardecl->getID(), idx, vardecl->getLocation(), 0);
    }
	//all_node.output_node(scm);
	//all_node.output_struct_node(scm);
    break;
    case 32://CXXRecordDecl,struct
      cxxrd = (CXXRecordDecl*) anydecl;
      /* here return global struct define message
     noting!!!
     */
      res = new ResVar{cxxrd->getNameAsString(),cxxrd->getGlobalID(),cxxrd->getLocation(),cxxrd};
      /*
      show all member name and type in struct
      */
     /*
      for (auto i = cxxrd->field_begin(); i !=cxxrd->field_end(); i++)
      {
        //std::cout<<i->getID()<<"\t"<<i->getQualifiedNameAsString()<<"\t"<<i->getType().getAsString()<<std::endl;
      }
      */
      break;
    
    default:
      break;
    }
  }
}



void TraverseStmt(Stmt * anystmt, int count, SourceManager *scm) {
  if (anystmt != nullptr) {
    CompoundStmt *cmpstmt;
    DeclStmt *dlstmt;
    //ForStmt *forstmt;
    //ReturnStmt *retstmt;
    CallExpr *callexpr;
    //IntegerLiteral *intltr;
    //CStyleCastExpr *cscastexpr;
    //ImplicitCastExpr *imcastexpr;
    DeclRefExpr *dlrefexpr;
    //StringLiteral *strltr;
    //GNUNullExpr *gnu;
    BinaryOperator *bin;
    UnaryOperator *unary;
    //ParenExpr *paren;
    //SwitchStmt *swstmt;
    //CaseStmt * casestmt;
    //ConstantExpr *constmt;
    //BreakStmt *brstmt;
    //DefaultStmt *defau;
    //IfStmt *ifstmt;
    //CXXNullPtrLiteralExpr *nullptrltr;
    //CXXConstructExpr *construct;
    MemberExpr *member;
    //FloatingLiteral *floatltr;
    InitListExpr *initlst;
    //ArraySubscriptExpr *arrsubexpr;
    //CharacterLiteral *charltr;
    CompoundAssignOperator *cap;
    unsigned line;
    unsigned cl;
    int pos;
    cur_stmt = anystmt;
    
    //for (int k = 0; k < count; ++k)
      //printf(" ");
     //std::cout << anystmt->getStmtClassName()<<" ";
     // std::cout << anystmt->getStmtClass()<<"\n";
    unsigned stmtclass = anystmt->getStmtClass();
    switch (stmtclass) {
        /*
      case 3: //breakstmt
        brstmt = (BreakStmt *)anystmt;
        //std::cout << brstmt->getStmtClassName() << std::endl;
        break;
        */
      case 8: //compoundstmt
        cmpstmt = (CompoundStmt *)anystmt;
        //std::cout << cmpstmt->getStmtClassName() << std::endl;
        ++count;
        for (auto tmp = cmpstmt->body_begin(); tmp != cmpstmt->body_end(); ++tmp) {
          //std::cout << (*tmp)->getBeginLoc().getPtrEncoding();
          TraverseStmt(*tmp, count, scm);
        }
        break;
      case 12: //declstmt
        ++count;
        dlstmt = (DeclStmt *)anystmt;
        //std::cout << dlstmt->getStmtClassName() << ": " << std::endl;
        for (auto j = dlstmt->decl_begin(); j != dlstmt->decl_end(); ++j) {
          
          TraverseDecl(*j, count, scm);
        }
        break;
        /*
      case 14: //forstmt
        ++count;
        forstmt = (ForStmt *)anystmt;
        //std::cout << forstmt->getStmtClassName() << ": " << std::endl;
        for (auto j = forstmt->child_begin(); j != forstmt->child_end(); ++j) {
          TraverseStmt(*j, count, scm);
        }
        break;
        */
       /*
      case 16: //ifstmt
        ++count;
        ifstmt = (IfStmt *)anystmt;
        //std::cout << ifstmt->getStmtClassName() << ": " << std::endl;
        for (auto i = ifstmt->child_begin(); i != ifstmt->child_end(); ++i)
          TraverseStmt(*i, count, scm);
        break;
        */
       /*
      case 74: //returnstmt
        ++count;
        retstmt = (ReturnStmt *)anystmt;
        //std::cout << retstmt->getStmtClassName() << ": " << std::endl;
        for (auto j = retstmt->child_begin(); j != retstmt->child_end(); ++j) {
          TraverseStmt(*j, count, scm);
        }
        break;
        */
       /*
      case 79: //casestmt
        ++count;
        casestmt = (CaseStmt *)anystmt;
        //std::cout << casestmt->getStmtClassName() << ": " << std::endl;
        for (auto i = casestmt->child_begin(); i != casestmt->child_end(); ++i) {
          TraverseStmt(*i, count, scm);
        }
        break;
        */
       /*
      case 80: //defaultstmt
        ++count;
        defau = (DefaultStmt *)anystmt;
        //std::cout << defau->getStmtClassName() << ": " << std::endl;
        for (auto i = defau->child_begin(); i != defau->child_end(); ++i)
          TraverseStmt(*i, count, scm);
        break;
        */
       /*
      case 81: //switchstmt
        ++count;
        swstmt = (SwitchStmt *)anystmt;
        //std::cout << swstmt->getStmtClassName() << ": " << std::endl;
        for (auto i = swstmt->child_begin(); i != swstmt->child_end(); ++i) {
          TraverseStmt(*i, count, scm);
        }
        break;
        */
       /*
      case 88:  //ArraySubscriptExpr
        ++count;
        arrsubexpr = (ArraySubscriptExpr *)anystmt;
        for (auto i = arrsubexpr->child_begin(); i != arrsubexpr->child_end(); ++i) {
          TraverseStmt(*i, count, scm);
        }
        */
       /*
       case 91: //AtomicExpr
         break;
         */
      case 92: //BinaryOperator
        ++count;
        bin = (BinaryOperator *)anystmt;
        //std::cout << bin->getStmtClassName() << ": ";
        //std::cout << bin->getType().getAsString() << "  " << bin->getOpcodeStr().str() << std::endl;
        if (bin->getOpcodeStr().str() == "=")
            definition = true;
        else 
            definition = false;
        for (auto i = bin->child_begin(); i != bin->child_end(); ++i) {
          
          TraverseStmt(*i, count, scm);
        }
        
        if (!saved) {
          //add node
          if (assign_tmp.size() != 0) {
            info_tmp = assign_tmp[assign_tmp.size()-1];
            assign_tmp.pop_back();
            //std::cout << "defined:" << info_tmp.defined << "\n";
            def_tmp[info_tmp.block_id][info_tmp.var_id].def_line = info_tmp.def_line;
            def_tmp[info_tmp.block_id][info_tmp.var_id].def_cl = info_tmp.def_cl;
            def_tmp[info_tmp.block_id][info_tmp.var_id].varname = info_tmp.varname;
            def_tmp[info_tmp.block_id][info_tmp.var_id].def_stmt = info_tmp.def_stmt;
            def_tmp[info_tmp.block_id][info_tmp.var_id].defined = info_tmp.defined;

            //all_node.add_du(dlrefexpr->getDecl()->getID(), line, 0, cur_funcname, cur_stmt, nullptr);
            all_node.add_defuse_ln(info_tmp.var_id, idx, info_tmp.location, info_tmp.def_line, 0, info_tmp.def_cl, 0);
            all_node.add_funcname(info_tmp.var_id, idx, info_tmp.location, cur_funcname);
            all_node.add_defuse_stmt(info_tmp.var_id, idx, info_tmp.location, info_tmp.def_stmt, nullptr);
            all_node.add_blockid(info_tmp.var_id, idx, info_tmp.location, info_tmp.block_id);
            all_node.add_defined(info_tmp.var_id, idx, info_tmp.location, info_tmp.defined);
          }

          if (struct_assign_tmp.size() != 0) {
            struct_info_tmp = struct_assign_tmp[struct_assign_tmp.size()-1];
            struct_assign_tmp.pop_back();
            struct_def_tmp[struct_info_tmp.block_id][struct_info_tmp.var_id].def_line = struct_info_tmp.def_line;
            struct_def_tmp[struct_info_tmp.block_id][struct_info_tmp.var_id].def_cl = struct_info_tmp.def_cl;
            struct_def_tmp[struct_info_tmp.block_id][struct_info_tmp.var_id].varname = struct_info_tmp.varname;
            struct_def_tmp[struct_info_tmp.block_id][struct_info_tmp.var_id].def_stmt = struct_info_tmp.def_stmt;
            struct_def_tmp[struct_info_tmp.block_id][struct_info_tmp.var_id].defined = struct_info_tmp.defined;
            
            if ((all_node.get_struct_node().count(struct_info_tmp.var_id) != 0)) {
              all_node.add_struct_defuse_ln(struct_info_tmp.var_id, idx, struct_info_tmp.location, struct_info_tmp.def_line, 0, struct_info_tmp.def_cl, 0);
              all_node.add_struct_funcname(struct_info_tmp.var_id, idx, struct_info_tmp.location, cur_funcname);
              all_node.add_struct_defuse_stmt(struct_info_tmp.var_id, idx, struct_info_tmp.location, struct_info_tmp.def_stmt, nullptr);
              all_node.add_struct_blockid(struct_info_tmp.var_id, idx, struct_info_tmp.location, struct_info_tmp.block_id);
              all_node.add_struct_defined(struct_info_tmp.var_id, idx, struct_info_tmp.location, struct_info_tmp.defined);
            }
            else {
              all_node.add_struct_var(struct_info_tmp.var_id, struct_info_tmp.varname, struct_info_tmp.type);
              all_node.add_struct_defuse_ln(struct_info_tmp.var_id, idx, struct_info_tmp.location, struct_info_tmp.def_line, 0, struct_info_tmp.def_cl, 0);
              all_node.add_struct_funcname(struct_info_tmp.var_id, idx, struct_info_tmp.location, cur_funcname);
              all_node.add_struct_defuse_stmt(struct_info_tmp.var_id, idx, struct_info_tmp.location, struct_info_tmp.def_stmt, nullptr);
              all_node.add_struct_blockid(struct_info_tmp.var_id, idx, struct_info_tmp.location, struct_info_tmp.block_id);
              all_node.add_struct_defined(struct_info_tmp.var_id, idx, struct_info_tmp.location, struct_info_tmp.defined);
            }
          }

          saved = true;
        }
        break;
      case 93:  //CompoundAssignOperator
        ++count;
        //std::cout << "compoundassign.\n";
        cap = (CompoundAssignOperator *)anystmt;
        definition = true;
        //std::cout <<"child_begin stmt.\n";
        /*
        if ((cap->child_begin() != cap->child_end()) && (*(cap->child_begin()) != NULL)) {
          std::cout << *(cap->child_begin())->getStmtClassName()<<"\n";
        }
        else {
          std::cout << "cap konglekongle!.\n";
        }
        */
        if((cap->child_begin() != cap->child_end()) && (*(cap->child_begin()) != nullptr)) {
          //std::cout << "compound traverse start.\n";
          for (auto i = cap->child_begin(); i != cap->child_end(); ++i) {
            //std::cout << *i->getStmtClassName()<<"\n";
            TraverseStmt(*i, count, scm);
          }
        }
        if (!saved) {
          //std::cout << "compound not saved, saving.\n";
          //add node
          if (assign_tmp.size() != 0) {
            info_tmp = assign_tmp[assign_tmp.size()-1];
            assign_tmp.pop_back();
            //std::cout << "defined:" << info_tmp.defined << "\n";
            def_tmp[info_tmp.block_id][info_tmp.var_id].def_line = info_tmp.def_line;
            def_tmp[info_tmp.block_id][info_tmp.var_id].def_cl = info_tmp.def_cl;
            def_tmp[info_tmp.block_id][info_tmp.var_id].varname = info_tmp.varname;
            def_tmp[info_tmp.block_id][info_tmp.var_id].def_stmt = info_tmp.def_stmt;
            def_tmp[info_tmp.block_id][info_tmp.var_id].defined = info_tmp.defined;

            //all_node.add_du(dlrefexpr->getDecl()->getID(), line, 0, cur_funcname, cur_stmt, nullptr);
            all_node.add_defuse_ln(info_tmp.var_id, idx, info_tmp.location, info_tmp.def_line, 0, info_tmp.def_cl, 0);
            all_node.add_funcname(info_tmp.var_id, idx, info_tmp.location, cur_funcname);
            all_node.add_defuse_stmt(info_tmp.var_id, idx, info_tmp.location, info_tmp.def_stmt, nullptr);
            all_node.add_blockid(info_tmp.var_id, idx, info_tmp.location, info_tmp.block_id);
            all_node.add_defined(info_tmp.var_id, idx, info_tmp.location, info_tmp.defined);
          }

          if (struct_assign_tmp.size() != 0) {
            struct_info_tmp = struct_assign_tmp[struct_assign_tmp.size()-1];
            struct_assign_tmp.pop_back();
            struct_def_tmp[struct_info_tmp.block_id][struct_info_tmp.var_id].def_line = struct_info_tmp.def_line;
            struct_def_tmp[struct_info_tmp.block_id][struct_info_tmp.var_id].def_cl = struct_info_tmp.def_cl;
            struct_def_tmp[struct_info_tmp.block_id][struct_info_tmp.var_id].varname = struct_info_tmp.varname;
            struct_def_tmp[struct_info_tmp.block_id][struct_info_tmp.var_id].def_stmt = struct_info_tmp.def_stmt;
            struct_def_tmp[struct_info_tmp.block_id][struct_info_tmp.var_id].defined = struct_info_tmp.defined;
            
            if ((all_node.get_struct_node().count(struct_info_tmp.var_id) != 0)) {
              all_node.add_struct_defuse_ln(struct_info_tmp.var_id, idx, struct_info_tmp.location, struct_info_tmp.def_line, 0, struct_info_tmp.def_cl, 0);
              all_node.add_struct_funcname(struct_info_tmp.var_id, idx, struct_info_tmp.location, cur_funcname);
              all_node.add_struct_defuse_stmt(struct_info_tmp.var_id, idx, struct_info_tmp.location, struct_info_tmp.def_stmt, nullptr);
              all_node.add_struct_blockid(struct_info_tmp.var_id, idx, struct_info_tmp.location, struct_info_tmp.block_id);
              all_node.add_struct_defined(struct_info_tmp.var_id, idx, struct_info_tmp.location, struct_info_tmp.defined);
            }
            else {
              all_node.add_struct_var(struct_info_tmp.var_id, struct_info_tmp.varname, struct_info_tmp.type);
              all_node.add_struct_defuse_ln(struct_info_tmp.var_id, idx, struct_info_tmp.location, struct_info_tmp.def_line, 0, struct_info_tmp.def_cl, 0);
              all_node.add_struct_funcname(struct_info_tmp.var_id, idx, struct_info_tmp.location, cur_funcname);
              all_node.add_struct_defuse_stmt(struct_info_tmp.var_id, idx, struct_info_tmp.location, struct_info_tmp.def_stmt, nullptr);
              all_node.add_struct_blockid(struct_info_tmp.var_id, idx, struct_info_tmp.location, struct_info_tmp.block_id);
              all_node.add_struct_defined(struct_info_tmp.var_id, idx, struct_info_tmp.location, struct_info_tmp.defined);
            }
          }

          saved = true;
        }
        //std::cout << "compound finished save.\n";
        break;
        /*
      case 97: //CXXConstructExpr
        construct = (CXXConstructExpr *)anystmt;
        //std::cout << construct->getStmtClassName() << ": ";
        //std::cout << construct->getType().getAsString() << "  ";
        //std::cout << construct->getConstructor()->getNumParams() << " ";*/
        /*
        for (unsigned i = 0; i < construct->getConstructor()->getNumParams(); ++i) {
          //std::cout << construct->getConstructor()->getParamDecl(i)->getType().getAsString() << "," 
            //<< construct->getConstructor()->getParamDecl(i)->getNameAsString() << "  ";
        }
        */
        //std::cout << std::endl;
        //break;
        /*
      case 107: //CXXNullptrLiteralExpr
        nullptrltr = (CXXNullPtrLiteralExpr *)anystmt;
        //std::cout << nullptrltr->getStmtClassName() << ": ";
        //std::cout << nullptrltr->getType().getAsString() << std::endl;
        break;
        */
      case 116: //callExpr
        callee = true;
        ++count;
        callexpr = (CallExpr *)anystmt;
        call_def = true;
        //std::cout << callexpr->getStmtClassName() << ": " << std::endl;
        for (auto i = callexpr->child_begin(); i != callexpr->child_end(); ++i) {
            call_def = true;
          TraverseStmt(*i, count, scm);
        }
        call_def = false;
        callee = false;
        break;
        /*
      case 122: //CStyleCastExpr
        ++count;
        cscastexpr = (CStyleCastExpr *)anystmt;
        //std::cout << cscastexpr->getStmtClassName() << ": ";
        //std::cout << cscastexpr->getType().getAsString() << std:: endl;
        if (*(cscastexpr->child_begin()) != nullptr) {
          for (auto i = cscastexpr->child_begin(); i != cscastexpr->child_end(); ++i) {
            TraverseStmt(*i, count, scm);
          }
        }
        break;
        */
       /*
      case 129: //ImplicitCastExpr
        imcastexpr = (ImplicitCastExpr *)anystmt;
        //std::cout << imcastexpr->getStmtClassName() << ": ";
        //std::cout << imcastexpr->getType().getAsString() << std::endl;
        if (*(imcastexpr->child_begin()) != nullptr) {
          ++count;
          for (auto i = imcastexpr->child_begin(); i != imcastexpr->child_end(); ++i) {
            TraverseStmt(*i, count, scm);
          }
        }
        break;
        */
       /*
      case 130: //CharacterLiteral
        charltr = (CharacterLiteral *)anystmt;
        break;
        */
      case 136: //DeclRefExpr
        dlrefexpr = (DeclRefExpr *)anystmt;
        //std::cout << dlrefexpr->getStmtClassName() << ": ";
        //std::cout << dlrefexpr->getType().getAsString() << "  ";
        //std::cout << dlrefexpr->getDecl()->getDeclKindName() << "  ";
        //std::cout << dlrefexpr->getDecl()->getID() << "\t";
        //std::cout << dlrefexpr->getDecl()->getNameAsString() << std::endl;
        //std::cout << dlrefexpr->getLocation().printToString(*scm) << "\n";
        line = scm->getSpellingLineNumber(dlrefexpr->getLocation());
        cl = scm->getSpellingColumnNumber(dlrefexpr->getLocation());
        //std::cout << line << std::endl;
        //not function
        if (strcmp(dlrefexpr->getDecl()->getDeclKindName(), "Function") != 0) {
            //std::cout << "not function.\n";
            //std::cout << "calldef:"<<call_def<<" definition:"<<definition<<"\n";
          if (call_def || definition) {
            if (call_def == true) {
                //std::cout << "call_def: ";
              if (!member_flag) {
                  //std::cout << "normal var.\n";
                //if pointer, def
                if ((dlrefexpr->getType().getTypePtrOrNull()->isPointerType()) || unary_ptr) {
                  def_tmp[block_id][dlrefexpr->getDecl()->getID()].def_line = line;
                  def_tmp[block_id][dlrefexpr->getDecl()->getID()].def_cl = cl;
                  def_tmp[block_id][dlrefexpr->getDecl()->getID()].varname = dlrefexpr->getDecl()->getNameAsString();
                  def_tmp[block_id][dlrefexpr->getDecl()->getID()].def_stmt = cur_stmt;
                  def_tmp[block_id][dlrefexpr->getDecl()->getID()].defined = 1;

                  all_node.add_defuse_ln(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), line, 0, cl, 0);
                  all_node.add_funcname(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), cur_funcname);
                  all_node.add_defuse_stmt(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), cur_stmt, nullptr);
                  all_node.add_blockid(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), block_id);
                  all_node.add_defined(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), 1);
                  if (unary_ptr) {
                    unary_ptr = false;
                  }
                  //std::cout << "pointer end.\n";
                }
              }
              else {
                  //std::cout << "member var.\n";
                //member structure
                if (member_ptr || unary_ptr) {
                  std::pair<int, int> declrefmem(dlrefexpr->getDecl()->getID(), member_in_id);
                  if (struct_def_tmp[block_id].count(declrefmem) != 0) {
                    struct_def_tmp[block_id][declrefmem].def_line = line;
                    struct_def_tmp[block_id][declrefmem].def_cl = cl;
                    struct_def_tmp[block_id][declrefmem].varname = dlrefexpr->getDecl()->getNameAsString();
                    struct_def_tmp[block_id][declrefmem].def_stmt = cur_stmt;
                    struct_def_tmp[block_id][declrefmem].defined = 1;

                    all_node.add_struct_defuse_ln(declrefmem, idx, dlrefexpr->getLocation(), line, 0, cl, 0);
                    all_node.add_struct_funcname(declrefmem, idx, dlrefexpr->getLocation(), cur_funcname);
                    all_node.add_struct_defuse_stmt(declrefmem, idx, dlrefexpr->getLocation(), cur_stmt, nullptr);
                    all_node.add_struct_blockid(declrefmem, idx, dlrefexpr->getLocation(), block_id);
                    all_node.add_struct_defined(declrefmem, idx, dlrefexpr->getLocation(), 1);
                  }
                  else {
                    struct_def_tmp[block_id][declrefmem].def_line = line;
                    struct_def_tmp[block_id][declrefmem].def_cl = cl;
                    struct_def_tmp[block_id][declrefmem].varname = dlrefexpr->getDecl()->getNameAsString();
                    struct_def_tmp[block_id][declrefmem].def_stmt = cur_stmt;
                    struct_def_tmp[block_id][declrefmem].defined = 1;

                    all_node.add_struct_var(declrefmem, member_in_name, member_in_type);
                    all_node.add_struct_defuse_ln(declrefmem, idx, dlrefexpr->getLocation(), line, 0, cl, 0);
                    all_node.add_struct_funcname(declrefmem, idx, dlrefexpr->getLocation(), cur_funcname);
                    all_node.add_struct_defuse_stmt(declrefmem, idx, dlrefexpr->getLocation(), cur_stmt, nullptr);
                    all_node.add_struct_blockid(declrefmem, idx, dlrefexpr->getLocation(), block_id);
                    all_node.add_struct_defined(declrefmem, idx, dlrefexpr->getLocation(), 1);

                  }
                  if (member_ptr) {
                    member_ptr = false;
                  }
                  if (unary_ptr) {
                    unary_ptr = false;
                  }
                }
              }
              
              //call_def = false;
            }
            //when new-define or update-define a var
            if (definition == true) {
              //update def
              //std::cout << "definition true: ";
              if (member_flag == true) {
                  //std::cout << "member var.\n";
                std::pair<int, int> member_id(dlrefexpr->getDecl()->getID(), member_in_id);
                struct_info_tmp.type = member_in_type;
                struct_info_tmp.block_id = block_id;
                struct_info_tmp.def_line = line;
                struct_info_tmp.def_cl = cl;
                struct_info_tmp.def_stmt = cur_stmt;
                struct_info_tmp.var_id = member_id;
                struct_info_tmp.varname = member_in_name;
                struct_info_tmp.location = dlrefexpr->getLocation();
                struct_info_tmp.defined = 1;
                struct_assign_tmp.push_back(struct_info_tmp);
              }
              else {
                //  std::cout << "normal var.\n";
                info_tmp.block_id = block_id;
                info_tmp.def_line = line;
                info_tmp.def_cl = cl;
                info_tmp.def_stmt = cur_stmt;
                info_tmp.var_id = dlrefexpr->getDecl()->getID();
                info_tmp.varname = dlrefexpr->getDecl()->getNameAsString();
                info_tmp.location = dlrefexpr->getLocation();
                info_tmp.defined = 1;
                assign_tmp.push_back(info_tmp);
              }
            
              saved = false;
              definition = false;
              //std::cout << "definitioin storage end.\n";
            }
          }
          else {
              //std::cout << "use.\n";
            //use
            //def-reverse to last definition
            int pos_blockid;
            for (pos_blockid = reverse_path.size()-1; pos_blockid >= 0; --pos_blockid) {
              if (reverse_path[pos_blockid] == block_id) {
                pos = pos_blockid;
                break;
              }
            }
            if (member_flag == true) {
              //std::cout <<member_in_id <<" "<< member_in_name << "\n";
              //add member Info
              std::pair<int, int> member_id(dlrefexpr->getDecl()->getID(), member_in_id);
              //member in current block
              if (struct_def_tmp[block_id].count(member_id) != 0) {
               // std::cout <<"struct_def_tmp[block_id].count(member_id) != 0\n";
                all_node.add_struct_defuse_ln(member_id, idx, dlrefexpr->getLocation(), struct_def_tmp[block_id][member_id].def_line, line, struct_def_tmp[block_id][member_id].def_cl, cl);
                all_node.add_struct_funcname(member_id, idx, dlrefexpr->getLocation(), cur_funcname);
                all_node.add_struct_defuse_stmt(member_id, idx, dlrefexpr->getLocation(), struct_def_tmp[block_id][member_id].def_stmt, cur_stmt);
                all_node.add_struct_blockid(member_id, idx, dlrefexpr->getLocation(), block_id);
                all_node.add_struct_defined(member_id, idx, dlrefexpr->getLocation(), struct_def_tmp[block_id][member_id].defined);
                //std::cout << "struct_def_tmp[block_id].count(member_id) != 0 end.***********\n";
              }
              else {  //member not in current block
                //check if member in other block 
                //std::cout << "member not in current block;check if member in other block\n";
                --pos;
                while (pos >= 0) {
                  if (struct_def_tmp[reverse_path[pos]].count(member_id) != 0) {
                    all_node.add_struct_defuse_ln(member_id, idx, dlrefexpr->getLocation(), struct_def_tmp[reverse_path[pos]][member_id].def_line, line, struct_def_tmp[reverse_path[pos]][member_id].def_cl, cl);
                    all_node.add_struct_funcname(member_id, idx, dlrefexpr->getLocation(), cur_funcname);
                    all_node.add_struct_defuse_stmt(member_id, idx, dlrefexpr->getLocation(), struct_def_tmp[reverse_path[pos]][member_id].def_stmt, cur_stmt);
                    all_node.add_struct_blockid(member_id, idx, dlrefexpr->getLocation(), block_id);
                    all_node.add_struct_defined(member_id, idx, dlrefexpr->getLocation(), struct_def_tmp[reverse_path[pos]][member_id].defined);
                    break;
                  }
                  --pos;
                }
              }
              //member not in any block, first build, then set defined=0
              if (pos == -1) {
                //std::cout << "member not in any block, first build, then set defined=0\n";
                
                //if member_id not in 
                //add new member in.
                //std::cout << "member id:"<<dlrefexpr->getDecl()->getID()<<"\n";
                //std::cout <<pos_blockid<<" "<<reverse_path[pos_blockid] <<"\n";
                //output_deftmp();
                //std::cout << "pos_blockid:"<<pos_blockid<<"\n";
                if (def_tmp[reverse_path[pos_blockid]].count(dlrefexpr->getDecl()->getID()) != 0) {
                    //std::cout << "in current block.\n";
                  pos = pos_blockid;
                }
                else {
                    //std::cout << "in other block.\n";
                  while (pos_blockid > 0) {
                    --pos_blockid;
                    if (def_tmp[reverse_path[pos_blockid]].count(dlrefexpr->getDecl()->getID()) != 0) {
                      pos = pos_blockid;
                      break;
                    }
                    if (pos_blockid == 0) {
                      pos = -1;
                    }
                  }
                }
                //std::cout << "pos:"<<pos<<"\n";
                
                if (pos != -1) {
                  //std::cout << pos_blockid << "******************\n";
                  //std::cout << pos << "******************\n";
                 // std::cout <<def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].varname<<" ";
                  //std::cout <<pos<<" ";
                  //std::cout << reverse_path[pos] <<"\n";
                  //std::cout << def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_line << " "
                    //<<def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_cl<<"\n";
                  all_node.add_struct_var(member_id, member_in_name, member_in_type);
                  all_node.add_struct_defuse_ln(member_id, idx, dlrefexpr->getLocation(), def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_line, line, 
                                              def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_cl, cl);
                  all_node.add_struct_funcname(member_id, idx, dlrefexpr->getLocation(), cur_funcname);
                  all_node.add_struct_defuse_stmt(member_id, idx, dlrefexpr->getLocation(), def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_stmt, cur_stmt);
                  all_node.add_struct_blockid(member_id, idx, dlrefexpr->getLocation(), block_id);
                  all_node.add_struct_defined(member_id, idx, dlrefexpr->getLocation(), def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].defined);

                  //add to struct_def_tmp
                  struct_def_tmp[block_id][member_id].def_line = def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_line;
                  struct_def_tmp[block_id][member_id].def_cl = def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_cl;
                  struct_def_tmp[block_id][member_id].varname = member_in_name;
                  struct_def_tmp[block_id][member_id].def_stmt = cur_stmt;
                  struct_def_tmp[block_id][member_id].defined = def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].defined;
                  //std::cout << "pos != -1 end.\n";
                }
                else {
                  //GLOBAL DEF!!!!!!!!!!!!!!!!*********************
                  //std::cout << "Global def start.\n";
                  all_node.add_struct_var(member_id, member_in_name, member_in_type);
                  all_node.add_struct_defuse_ln(member_id, idx, dlrefexpr->getLocation(), global_def[dlrefexpr->getDecl()->getID()].def_line, line, 
                                              global_def[dlrefexpr->getDecl()->getID()].def_cl, cl);
                  all_node.add_struct_funcname(member_id, idx, dlrefexpr->getLocation(), cur_funcname);
                  all_node.add_struct_defuse_stmt(member_id, idx, dlrefexpr->getLocation(), global_def[dlrefexpr->getDecl()->getID()].def_stmt, cur_stmt);
                  all_node.add_struct_blockid(member_id, idx, dlrefexpr->getLocation(), block_id);
                  all_node.add_struct_defined(member_id, idx, dlrefexpr->getLocation(), global_def[dlrefexpr->getDecl()->getID()].defined);

                  //add to struct_def_tmp
                  struct_def_tmp[block_id][member_id].def_line = global_def[dlrefexpr->getDecl()->getID()].def_line;
                  struct_def_tmp[block_id][member_id].def_cl = global_def[dlrefexpr->getDecl()->getID()].def_cl;
                  struct_def_tmp[block_id][member_id].varname = member_in_name;
                  struct_def_tmp[block_id][member_id].def_stmt = cur_stmt;
                  struct_def_tmp[block_id][member_id].defined = global_def[dlrefexpr->getDecl()->getID()].defined;
                }
              }
            }
            else {
                //std::cout << "normal var use.\n";
              if (def_tmp[block_id].count(dlrefexpr->getDecl()->getID()) != 0) {
                all_node.add_defuse_ln(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), def_tmp[block_id][dlrefexpr->getDecl()->getID()].def_line, line, def_tmp[block_id][dlrefexpr->getDecl()->getID()].def_cl, cl);
                all_node.add_funcname(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), cur_funcname);
                all_node.add_defuse_stmt(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), def_tmp[block_id][dlrefexpr->getDecl()->getID()].def_stmt, cur_stmt);
                all_node.add_blockid(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), block_id);
                all_node.add_defined(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), def_tmp[block_id][dlrefexpr->getDecl()->getID()].defined);
              }
              else {
                --pos;
                //std::cout << pos << std::endl;
                while (pos >= 0) {
                  if (def_tmp[reverse_path[pos]].count(dlrefexpr->getDecl()->getID()) != 0) {
                    all_node.add_defuse_ln(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_line, line, def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_cl, cl);
                    all_node.add_funcname(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), cur_funcname);
                    all_node.add_defuse_stmt(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_stmt, cur_stmt);
                    all_node.add_blockid(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), block_id);
                    all_node.add_defined(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].defined);
                    break;
                  }
                  --pos;
                }
              }
              if (pos == -1) {
                  //std::cout << "pos == -1.\n";
                all_node.add_defuse_ln(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), global_def[dlrefexpr->getDecl()->getID()].def_line, line, global_def[dlrefexpr->getDecl()->getID()].def_cl, cl);
                all_node.add_funcname(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), cur_funcname);
                all_node.add_defuse_stmt(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), global_def[dlrefexpr->getDecl()->getID()].def_stmt, cur_stmt);
                all_node.add_blockid(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), block_id);
                all_node.add_defined(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), global_def[dlrefexpr->getDecl()->getID()].defined);
              }
            }
          }
        }
        else {
          if(strcmp(dlrefexpr->getDecl()->getNameAsString().c_str(), "scanf") == 0) {
            call_def = true;
          }
          else if (dlrefexpr->getDecl()->getNameAsString() != "strcpy") {
            call_def = true;
          }
        }
        break;
        /*
      case 144: //floatingLiteral
        floatltr = (FloatingLiteral *)anystmt;
        //std::cout << floatltr->getStmtClassName() << ": ";
        //std::cout << floatltr->getType().getAsString() << "  " << floatltr->getValue().convertToDouble() << std::endl;
        *//*
        if (*(floatltr->child_begin()) != nullptr) {
          for (auto i = floatltr->child_begin(); i != floatltr->child_end(); ++i) {
            TraverseStmt(*i, count, scm);
          }
        }
        */
        //break;
        /*
      case 145: //ConstantExpr
        constmt = (ConstantExpr *)anystmt;
        //std::cout << constmt->getStmtClassName() << ": ";
        //std::cout << constmt->getType().getAsString() << "  ";
        if (constmt->getAPValueResult().isInt() == true)
          //std::cout << constmt->getAPValueResult().getInt().getSExtValue() << std::endl;
          ++count;
        for (auto i = constmt->child_begin(); i != constmt->child_end(); ++i) {
          TraverseStmt(*i, count, scm);
        }
        break;
        */
       /*
      case 148: //GNUNullExpr
        gnu = (GNUNullExpr *)anystmt;
        //std::cout << gnu->getStmtClassName() << ": ";
        //std::cout << gnu->getType().getAsString() << std::endl;
        break;
        */
       
      case 152: //InitLitExpr
        initlst = (InitListExpr *)anystmt;
        //std::cout << initlst->getStmtClassName() << ": ";
        //std::cout << initlst->getType().getAsString() << "\n";
        /*
        for (auto i = initlst->begin(); i != initlst->end(); ++i) {
          TraverseStmt(*i, count, scm);
        }
        */

        break;
       /* 
      case 153: //IntegerLiteral
        intltr = (IntegerLiteral *)anystmt;
        //std::cout << intltr->getStmtClassName() << ": ";
        //std::cout << intltr->getType().getAsString() << "  ";
        //std::cout << intltr->getValue().getSExtValue() << std::endl;
        *//*
        if (*(intltr->child_begin()) != nullptr) {
          for (auto i = intltr->child_begin(); i != intltr->child_end(); ++i) {
            TraverseStmt(*i, count, scm);
          }
        }
        */
        //break;
      case 158: //MemberExpr
        ++count;
        member_flag = true;
        member = (MemberExpr *)anystmt;
        //std::cout << member->getMemberDecl()->getID() << "\t";
        member_in_id = member->getMemberDecl()->getID();
        //std::cout << member->getID(member->getMemberDecl()->getASTContext()) << "\t";
        //std::cout << member->getStmtClassName() << ": ";
        //std::cout << member->getType().getAsString() << "  " << member->getMemberNameInfo().getAsString() << std::endl;
        member_in_name = member->getMemberNameInfo().getAsString();
        member_in_type = member->getType().getAsString();
        //std::cout << "member in type:"<<member_in_type<<"\n";
        member_ptr = member->getType().getTypePtrOrNull()->isPointerType();
        for (auto i = member->child_begin(); i != member->child_end(); ++i)
          TraverseStmt(*i, count, scm);
        
        member_flag = false;
        break;
        /*
      case 181: //ParenExpr
        paren = (ParenExpr *)anystmt;
        //std::cout << paren->getStmtClassName() << ": ";
        //std::cout << paren->getType().getAsString() << "  ";
        
        switch (paren->getValueKind()) {
          //case 0: std::cout << "rvalue" << std::endl; break;
          //case 1: std::cout << "lvalue" << std::endl; break;
          //case 2: std::cout << "xvalue" << std:: endl; break;
          default: break;
        }
        
        if (*(paren->child_begin()) != nullptr) {
          for (auto i = paren->child_begin(); i != paren->child_end(); ++i) {
            TraverseStmt(*i, count, scm);
          }
        }
        break;
        */
       /*
      case 189: //StringLiteral
        strltr = (StringLiteral *)anystmt;
        //std::cout << strltr->getStmtClassName() << ": ";
        //std::cout << strltr->getType().getAsString() << "  ";
        //std::cout << strltr->getString().str() << std::endl;
        break;
        */
      case 195: //UnaryOperator
        ++count;
        unary = (UnaryOperator *)anystmt;
        //std::cout << unary->getStmtClassName() << ": ";
        //std::cout << unary->getType().getAsString() << "  " << unary->getOpcodeStr(unary->getOpcode()).str() << std::endl;
        if (unary->getType().getTypePtrOrNull()->isPointerType()) {
            //std::cout << "true.\n";
          unary_ptr = true;
        }
        if ((strcmp(unary->getOpcodeStr(unary->getOpcode()).str().c_str(), "++") == 0) || (strcmp(unary->getOpcodeStr(unary->getOpcode()).str().c_str(), "--") == 0)) {
          definition = true;
          //std::cout << "definition true.\n";
        }
        for (auto i = unary->child_begin(); i != unary->child_end(); ++i) {
          //std::cout << *i->getStmtClassName() <<"\n";
          TraverseStmt(*i, count, scm);
          //std::cout << "back to unary.\n";        
        }
        //unary_ptr = false;
        break;
      default: 
	  	 // ++count;
        //std::cout << anystmt->getStmtClassName() << anystmt->getStmtClass() << std::endl;
        //std::cout << "switch default.\n";
        if ((anystmt->child_begin() != anystmt->child_end()) && (*(anystmt->child_begin()) != nullptr)) {
          //std::cout << "not nullptr.\n";
            for (auto i = anystmt->child_begin(); i != anystmt->child_end(); ++i) {
              //std::cout << (*i)->getStmtClassName() << "\n";
		          TraverseStmt(*i, count, scm);
            }
        }
        //std::cout << "safe traverse.\n";
        break;
    }
  }
}

void VU_Entry1(std::unordered_map<std::string, ASTResource>::iterator astr_iter)
{
  
  //std::cout << "VU_Entry1\n";
  //auto crds = astr_iter->second.GetCXXRecordDecls();//struct
  //std::cout << "crds.\n";
  auto vds = astr_iter->second.GetVarDecls();
  //std::cout << "vds\n";
  /*
  for(auto crd : crds){
	crd->dump();
    TraverseDecl(crd,0,&(crd->getASTContext().getSourceManager()));
  }
  */
  for (auto vd : vds) {
      //printf("Global variable: ");
	    //vd->dump();
      /*
      std::cout << vd->getQualifiedNameAsString() << "  "
        << vd->getType().getAsString() << "  " << std::endl;
      */
      cur_decl = vd;
      TraverseDecl(vd, 0, &(vd->getASTContext().getSourceManager()));
      //getArrayVar(vd);
  }
  //std::cout << "vd end.\n";
}

void VU_Entry2(clang::FunctionDecl*fd)
{
	auto fd_cfg = common::buildCFG(fd);
    //cfg_path[fd] = common::buildCFG(fd);

    cur_funcname = fd->getQualifiedNameAsString();
	srcmng = &(fd->getASTContext().getSourceManager());
    //output_tree();
    cur_fd = fd;
    
    //parameter
    if (fd->getNumParams() > 0) {
    	//printf("  Parameters: \n");
    	for (unsigned i = 0; i < fd->getNumParams(); i++) {
          //std::cout << "  " << fd->getParamDecl(i)->getQualifiedNameAsString() << " ";
          //std::cout << "  " << fd->getParamDecl(i)->getType().getAsString() << std::endl;
          //************
          global_def[fd->getParamDecl(i)->getID()].def_line = fd->getParamDecl(i)->getASTContext().getSourceManager().getSpellingLineNumber(fd->getParamDecl(i)->getLocation());
          global_def[fd->getParamDecl(i)->getID()].def_cl = fd->getParamDecl(i)->getASTContext().getSourceManager().getSpellingColumnNumber(fd->getParamDecl(i)->getLocation());
          global_def[fd->getParamDecl(i)->getID()].varname = fd->getParamDecl(i)->getNameAsString();
          global_def[fd->getParamDecl(i)->getID()].def_stmt = nullptr;
          global_def[fd->getParamDecl(i)->getID()].defined = 1;

          all_node.add_var(fd->getParamDecl(i)->getID(), fd->getParamDecl(i)->getQualifiedNameAsString(), fd->getParamDecl(i)->getType().getAsString());
          all_node.add_defuse_ln(fd->getParamDecl(i)->getID(), idx, fd->getParamDecl(i)->getLocation(), 
          fd->getParamDecl(i)->getASTContext().getSourceManager().getSpellingLineNumber(fd->getParamDecl(i)->getLocation()), 0, 
          fd->getParamDecl(i)->getASTContext().getSourceManager().getSpellingColumnNumber(fd->getParamDecl(i)->getLocation()), 0);
          all_node.add_funcname(fd->getParamDecl(i)->getID(), idx, fd->getParamDecl(i)->getLocation(), cur_funcname);
          all_node.add_defuse_stmt(fd->getParamDecl(i)->getID(), idx, fd->getParamDecl(i)->getLocation(), nullptr, nullptr);
          all_node.add_blockid(fd->getParamDecl(i)->getID(), idx, fd->getParamDecl(i)->getLocation(), block_id);
          all_node.add_defined(fd->getParamDecl(i)->getID(), idx, fd->getParamDecl(i)->getLocation(), 1);
        }
    }
      
      //Compoundstmt
    //if (fd->getNameAsString() == "_PyImport_FindSharedFuncptr")
       // {fd->dump();
      Stmt *cmpstmt = (fd->getBody());
      int count = 0;
      idx = 1;
      reverse_path.push_back(1);
      TraverseStmt(cmpstmt, count, srcmng);//}

      //printf("\n");

      // Traverse CFG
      //output_tree();
      /*
    LangOptions LangOpts;
    LangOpts.CPlusPlus = true;
    //fd_cfg->dump(LangOpts, true);
      
    auto block = &(fd_cfg->getEntry());
    auto exit = &fd_cfg->getExit();
      //first
    CFGBlock *t;
	CFGBlock *current;
	current = block;
    idx = 1;
    while(block->getBlockID() != exit->getBlockID()) {
        //std::cout << "block_id:"<<block->getBlockID()<<"\n";
        //handle loop
        *//*for (auto r=reverse_path.begin(); r != reverse_path.end(); ++r) {
          std::cout <<*r<<" ";
        }
        std::cout << "\n";*/
        /*if (std::find(reverse_path.begin(), reverse_path.end(), block->getBlockID()) != reverse_path.end()) {
            break;
          //std::cout << "in.\n";
          //if (block->succ_size() > 1) {
            for (auto i = block->succ_begin(); i != block->succ_end(); ++i) {
              t = i->getReachableBlock();
            }
            block = t;
          //}
          //else {
            //stack_path.pop();
            //reverse_path.pop_back();
            //for (auto i = stack_path.top()->succ_begin(); i != stack_path.top()->succ_end(); ++i) {
              //t = i->getReachableBlock();
            //}
            //block = t;
          //}
          //std::cout << block->getBlockID() << "\n";
          continue;
        }
        if (block == nullptr) {
          break;
        }
        if (block->getBlockID() != exit->getBlockID()) {
          stack_path.push(block);
          block_id = block->getBlockID();
          //CFGBlock *cb = convert(*block, fd_cfg->getBumpVectorContext(), block->getParent());
          //path_tree[cur_fd][idx].push_back(block);
          reverse_path.push_back(block_id);
          *//*
          for (auto I = block->begin(); I != block->end(); ++I) {
            if (Optional<CFGStmt> CS = (*I).getAs<CFGStmt>()) {
              Stmt *S = const_cast<Stmt *>(CS->getStmt());
              assert(S != nullptr && "Expecting non-null Stmt");
              //std::cout <<"1.\n"<<std::endl;
              cur_stmt = S;
              //cur_stmt->getBeginLoc().printToString(fd->getASTContext().getSourceManager());
              TraverseStmt(S, 0, &fd->getASTContext().getSourceManager());
            }
          }
          */
		  /*for (auto n = block->succ_begin(); n != block->succ_end(); ++n) {
			  if (n->getReachableBlock() != nullptr) {
				  block = n->getReachableBlock();
				  break;
			  }
		  }
          //block = block->succ_begin()->getReachableBlock();
		  
        }
		current = block;
		
    }*/

/*
    CFGBlock *tmp;
    CFGBlock *next;
    while ((stack_path.empty() == false)) {
      std::cout << "blockid:"<<stack_path.top()->getBlockID()<<"\n";
        if (stack_path.top()->succ_size() > 1) {
          //std::cout << "succ_size > 1\n";
          tmp = stack_path.top();
          //find next successor of block tmp
          for (auto i = tmp->succ_begin(); i != tmp->succ_end(); ++i) {
            	if ((i->getReachableBlock() != nullptr) && (i->getReachableBlock()->getBlockID() == current->getBlockID())) {
              		next = (++i)->getReachableBlock();
              		break;
            	}
          	}
        }
*/
        //the last child
        //all_node.output_node(&fd->getASTContext().getSourceManager());
        //all_node.output_struct_node(&fd->getASTContext().getSourceManager());
        /*
        if (next != nullptr) {
          std::cout << "next not null !*******************\n";
        }
        */
/*        if (stack_path.top()->succ_size() <= 1 || next == nullptr || next == tmp->succ_end()->getReachableBlock() || next->getBlockID() == exit->getBlockID()) {
			//next path is to exit.
            if (stack_path.top()->succ_size() > 1 && (next != nullptr) && (next != tmp->succ_end()->getReachableBlock()) && (next->getBlockID() == exit->getBlockID())) {
              	//std::cout << "in second if.\n";
              	++idx;
              	//add path
            	//join entry
            	path_tree[cur_fd][idx].push_back(path_tree[cur_fd][1][0]);
            	int path_idx = 1;
            	for (auto b = ++(reverse_path.begin()); b != reverse_path.end(); ++b) {
              		//int bk = (*b);
              		path_tree[cur_fd][idx].push_back(path_tree[cur_fd][idx-1][path_idx]);
              		++path_idx;
					
            	}
       		}
        	current = stack_path.top();
       		stack_path.pop();
        	reverse_path.pop_back();
        }
        else {
			//size > 1 && next != suceed_end
          	++idx;
          	//join entry
          	path_tree[cur_fd][idx].push_back(path_tree[cur_fd][1][0]);
          	int path_idx = 1;
          	for (auto b = ++(reverse_path.begin()); b != reverse_path.end(); ++b) {
            	//int bk = (*b);
            	path_tree[cur_fd][idx].push_back(path_tree[cur_fd][idx-1][path_idx]);
            	++path_idx;
            	
			}
        	while(next->getBlockID() != exit->getBlockID()) {
            std::cout << "blockid:"<<next->getBlockID()<<"\n";
            	//std::cout << reverse_path.size() << "\n";
            	if (std::find(reverse_path.begin(), reverse_path.end(), next->getBlockID()) != reverse_path.end()) {
              		for (auto i = next->succ_begin(); i != next->succ_end(); ++i) {
                		t = i->getReachableBlock();
              		}
              		next = t;
            	}
              if (next == nullptr) {
                //current = stack_path.top();
       		      //stack_path.pop();
        	      //reverse_path.pop_back();
                //std::cout << "next null.\n";
                break;
              }
            	if (next->getBlockID() != exit->getBlockID()) {
              		stack_path.push(next);
              		//CFGBlock *cb_1 = convert(*next, fd_cfg->getBumpVectorContext(), next->getParent());
              		path_tree[cur_fd][idx].push_back(next);
              		block_id = next->getBlockID();
              		reverse_path.push_back(block_id);
              		for (auto I = next->begin(); I != next->end(); ++I) {
                		if (Optional<CFGStmt> CS = (*I).getAs<CFGStmt>()) {
                  			Stmt *S = const_cast<Stmt *>(CS->getStmt());
                  			assert(S != nullptr && "Expecting non-null Stmt");
                  			//std::cout <<"1.\n"<<std::endl;
                  			cur_stmt = S;
                  			TraverseStmt(S, 0, &fd->getASTContext().getSourceManager());
                		}  
              		}
					for (auto n = next->succ_begin(); n != next->succ_end(); ++n) {
						if (n->getReachableBlock() != NULL) {
							next = n->getReachableBlock();
							break;
						}
					}
              		//next = next->succ_begin()->getReachableBlock();
            	}
        	}
		}
    }*/
    block_id = 1;
	reverse_path.clear();
  
	//all_node.output_node(&fd->getASTContext().getSourceManager());
	//all_node.output_struct_node(&fd->getASTContext().getSourceManager());
}

void VU_Detect()
{
    //std::cout << "into detector.\n";
	//map<int, def_use> node = all_node.get_node();
    map<std::pair<int, int>, def_use> struct_node = all_node.get_struct_node();
	std::string var_name;
	//int detec_idx;
	int detec_defline;
	int detec_defcl;
	int detec_useline;
	int detec_usecl;
    bool detec_flag = false;
	//std::string detec_funcname = fd->getNameAsString();
	SourceManager *sl = srcmng;
    /*
    for (auto i = node.begin(); i != node.end(); ++i) {
      var_name = (*i).second.get_vname();
      map<std::pair<int, SourceLocation>, element> du = (*i).second.get_du();
      for (auto ii = du.begin(); ii != du.end(); ++ii) {
        //detec_idx = (*ii).first.first;
        detec_defline = (*ii).second.get_defuse_ln().first;
        detec_defcl = (*ii).second.get_defuse_cl().first;
        detec_useline = (*ii).second.get_defuse_ln().second;*//* && (fd->getNameAsString() == (*ii).second.get_funcname())*//*) {
          //std::cout << "idx: " << idx << " def: " << def_line << " use: " << use_line << "\n";
          string s2 = "/";
          error_info *e;
          if ((*ii).second.get_defuse_stmt().first != nullptr) {
            string a1 = sl->getFilename((*ii).second.get_defuse_stmt().first->getBeginLoc()).str();
			
            //split(a1, s2, &t1);
            e = new_error_info(NULL, a1, detec_useline, detec_usecl, TYPE_NOTE, VU_ERROR_TYPE_USE);
          }
          else {
            string a1 = sl->getFilename(global_def[(*i).first].def_decl->getBeginLoc()).str();
            //split(a1, s2, &t1);
            e = new_error_info(NULL, a1, detec_useline, detec_usecl, TYPE_NOTE, VU_ERROR_TYPE_USE);
          }
          if ((*ii).second.get_defuse_stmt().second) {
            string a2 = sl->getFilename((*ii).second.get_defuse_stmt().second->getBeginLoc()).str();
            //split(a2, s2, &t2);
            error_info *te = new_error_info(e, a2, detec_defline, detec_defcl, TYPE_ERROR, VU_ERROR_TYPE_DEL_A+var_name+VU_ERROR_TYPE_DEL_B);
            result.push(te);
          }
        }
        t1.clear();
        t2.clear();
      }
    }
    */
    //struct
    //std::cout << "start traverse struct_node.\n";
    for (auto i = struct_node.begin(); i != struct_node.end(); ++i) {
      var_name = (*i).second.get_vname();
      map<std::pair<int, SourceLocation>, element> du = (*i).second.get_du();
      for (auto ii = du.begin(); ii != du.end(); ++ii) {
        //idx = (*ii).first.first;
        detec_defline = (*ii).second.get_defuse_ln().first;
        detec_defcl = (*ii).second.get_defuse_cl().first;
        detec_useline = (*ii).second.get_defuse_ln().second;
        detec_usecl = (*ii).second.get_defuse_cl().second;
        //vector<string> t1;
        //vector<string> t2;
        if ((detec_defline != 0) && (detec_useline != 0) && ((*ii).second.get_defined() == 0)/* && (detec_funcname == (*ii).second.get_funcname())*/) {
          //std::cout << "idx: " << idx << " def: " << def_line << " use: " << use_line << "\n";
          string s2 = "/";
          error_info *e;
          if ((*ii).second.get_defuse_stmt().first != nullptr) {
              //std::cout << "first != mullptr.\n";
            string a1 = sl->getFilename((*ii).second.get_defuse_stmt().first->getBeginLoc()).str();
            //split(a1, s2, &t1);
            if (a1 != "") {
                e = new_error_info(NULL, a1, detec_useline, detec_usecl, TYPE_NOTE, VU_ERROR_TYPE_USE,vud);
                detec_flag = true;
            }
          }
          else {
             if (global_def.count((*i).first.first) != 0) {
                 //std::cout << "decl not null.\n";
                string a1 = sl->getFilename(global_def[(*i).first.first].def_decl->getBeginLoc()).str();
                if (a1 != "") {
                    e = new_error_info(NULL, a1, detec_useline, detec_usecl, TYPE_NOTE, VU_ERROR_TYPE_USE,vud);
                    detec_flag = true;
                }
             }
                //std::cout << "a1 ok.\n";
                //split(a1, s2, &t1);
            
            }
          if (detec_flag && (*ii).second.get_defuse_stmt().second) {
            string a2 = sl->getFilename((*ii).second.get_defuse_stmt().second->getBeginLoc()).str();
            //split(a2, s2, &t2);
            if (a2 != "")
            {
                error_info *te = new_error_info(e, a2, detec_defline, detec_defcl, TYPE_ERROR, VU_ERROR_TYPE_DEL_A+var_name+VU_ERROR_TYPE_DEL_B,vud);
                result.push(te);
                detec_flag = false;
            }
          }
        }
        //t1.clear();
        //t2.clear();
      }
    }
    //std::cout << "traverse end.\n";
    //all_node.output_node(sl);
	  //std::cout << "******************************************\n";
	//all_node.output_struct_node(sl);
    all_node.clear_du_node();
    //std::cout << "clear.********************************************88888\n";
    
    all_node.clear_struct_node();
    //all_node.output_struct_node(sl);
    //std::cout << "between has something?***********************\n";
    def_tmp.clear();
    struct_def_tmp.clear();
    assign_tmp.clear();
    struct_assign_tmp.clear();
    global_def.clear();
    //std::cout << "detector end.\n";
    //path_tree.clear();

}
};
#endif
