#include "checkers/TemplateChecker.h"
//#include "checkers/All_Detector.h"
#include "stack"
#include "algorithm"
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

struct ResVar{
  std::string name;
  long id;
  SourceLocation location;
  Decl* decl;
};

extern std::map<FunctionDecl *, std::map<int, std::vector<CFGBlock *>>> path_tree;
extern std::map<FunctionDecl *, std::unique_ptr<clang::CFG>> cfg_path;

void TraverseDecl(Decl *anydecl, int count, SourceManager *scm);
void TraverseStmt(Stmt * anystmt, int count, SourceManager *scm);
defuse_node all_node;
bool definition = false;
bool unary_opcode = false;
bool integ = false;
bool callee = false;
bool call_def = false;
bool member_flag = false;
//block_id, varID, Info
std::map<int, std::map<int, Info>> def_tmp;
std::map<int, std::map<std::pair<int, int>, Info>> struct_def_tmp;
std::vector<Info> assign_tmp;
std::vector<struct_Info> struct_assign_tmp;
bool saved = true;
std::map<int, Info> global_def;
//std::map<std::string, bool> decl_or_def;
int block_id = -1;
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



void output_deftmp() {
  //def_tmp
  /*for (auto i = def_tmp.begin(); i != def_tmp.end(); ++i) {
    std::cout << i->first << "\n";
    for (auto ii = i->second.begin(); ii != i->second.end(); ++ii) {
      std::cout << ii->first << "  ";
      std::cout << ii->second.varname << "  " << ii->second.def_line << "\n";
    }
  }*/
}

void output_tree() {
   /* for (auto i = path_tree.begin(); i != path_tree.end(); ++i) {
        std::cout << "FunctionName:  " << (*i).first->getQualifiedNameAsString() << ":  ";
        for (auto i_in = (*i).second.begin(); i_in != (*i).second.end(); ++i_in) {
          std::cout << "index: " << (*i_in).first << "\n";
          //for (auto bk = (*i_in).second.begin(); bk != (*i_in).second.end(); ++bk) {
            //std::cout << (*bk) << " ";
            
          //}
        }
        std::cout << "\n";
    }*/
}

CFGBlock *convert(CFGBlock& cfg_bk, BumpVectorContext& bv, CFG *p) {
  CFGBlock *bk = new CFGBlock(cfg_bk.getBlockID(), bv, p);
  for (auto i = cfg_bk.begin(); i != cfg_bk.end(); ++i) {
    if (Optional<CFGStmt> CS = (*i).getAs<CFGStmt>()) {
      Stmt *S = const_cast<Stmt *>(CS->getStmt());
      assert(S != nullptr && "Expecting non-null Stmt");
      bk->appendStmt(S, bv);
    }
  }
  //std::cout << "size: " << bk->size() << "\n";
  bk->setHasNoReturnElement();
  bk->setLabel(cfg_bk.getLabel());
  bk->setLoopTarget(cfg_bk.getLoopTarget());
  bk->setTerminator(cfg_bk.getTerminator());
  return bk;
}


ResVar * getArrayVar(VarDecl* vd){
  // std::cout<<vd->getType().getAsString()<<std::endl;
   auto arrayType=vd->getType().getTypePtrOrNull();
  //std::cout<<arrayType->getTypeClassName()<<std::endl;
   if(strcmp(arrayType->getTypeClassName(),"ConstantArray")==0){
     /* here return global array message
     noting: res的结构为ResVar，最后一项是decl指针；
     若decl指针为空，则返回的不是数组结构，可以不存储。
     
     */
     ResVar* res = new ResVar{vd->getNameAsString(),vd->getGlobalID(),vd->getLocation(),vd};
     //std::cout<<res->name<<"\t"<<res->id<<"\t"<<res->decl->getDeclKindName()<<std::endl;
     return res;
   }
   ResVar* res = new ResVar{"noArray",0,vd->getLocation(),nullptr};
  // std::cout<<res->name<<"\t"<<res->id<<"\t"<<std::endl;
   return res;
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
        TraverseStmt(vardecl->ensureEvaluatedStmt()->Value, count, scm);
      }
      else {
        all_node.add_defuse_ln(vardecl->getID(), idx, vardecl->getLocation(), line, 0, cl, 0);
        all_node.add_funcname(vardecl->getID(), idx, vardecl->getLocation(), cur_funcname);
        all_node.add_defuse_stmt(vardecl->getID(), idx, vardecl->getLocation(), cur_stmt, nullptr);
        all_node.add_blockid(vardecl->getID(), idx, vardecl->getLocation(), block_id);
        all_node.add_defined(vardecl->getID(), idx, vardecl->getLocation(), 0);
      }
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
      for (auto i = cxxrd->field_begin(); i !=cxxrd->field_end(); i++)
      {
        //std::cout<<i->getID()<<"\t"<<i->getQualifiedNameAsString()<<"\t"<<i->getType().getAsString()<<std::endl;
      }
      
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
    ArraySubscriptExpr *arrsubexpr;
    unsigned line;
    unsigned cl;
    int pos;
    
    //for (int k = 0; k < count; ++k)
      //printf(" ");
    unsigned stmtclass = anystmt->getStmtClass();
    switch (stmtclass) {
      case 3: //breakstmt
        brstmt = (BreakStmt *)anystmt;
        //std::cout << brstmt->getStmtClassName() << std::endl;
        break;
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
      case 14: //forstmt
        ++count;
        forstmt = (ForStmt *)anystmt;
        //std::cout << forstmt->getStmtClassName() << ": " << std::endl;
        for (auto j = forstmt->child_begin(); j != forstmt->child_end(); ++j) {
          TraverseStmt(*j, count, scm);
        }
        break;
      case 16: //ifstmt
        ++count;
        ifstmt = (IfStmt *)anystmt;
        //std::cout << ifstmt->getStmtClassName() << ": " << std::endl;
        for (auto i = ifstmt->child_begin(); i != ifstmt->child_end(); ++i)
          TraverseStmt(*i, count, scm);
        break;
      case 74: //returnstmt
        ++count;
        retstmt = (ReturnStmt *)anystmt;
        //std::cout << retstmt->getStmtClassName() << ": " << std::endl;
        for (auto j = retstmt->child_begin(); j != retstmt->child_end(); ++j) {
          TraverseStmt(*j, count, scm);
        }
        break;
      case 79: //casestmt
        ++count;
        casestmt = (CaseStmt *)anystmt;
        //std::cout << casestmt->getStmtClassName() << ": " << std::endl;
        for (auto i = casestmt->child_begin(); i != casestmt->child_end(); ++i) {
          TraverseStmt(*i, count, scm);
        }
        break;
      case 80: //defaultstmt
        ++count;
        defau = (DefaultStmt *)anystmt;
        //std::cout << defau->getStmtClassName() << ": " << std::endl;
        for (auto i = defau->child_begin(); i != defau->child_end(); ++i)
          TraverseStmt(*i, count, scm);
        break;
      case 81: //switchstmt
        ++count;
        swstmt = (SwitchStmt *)anystmt;
        //std::cout << swstmt->getStmtClassName() << ": " << std::endl;
        for (auto i = swstmt->child_begin(); i != swstmt->child_end(); ++i) {
          TraverseStmt(*i, count, scm);
        }
        break;
      case 88:  //ArraySubscriptExpr
        ++count;
        arrsubexpr = (ArraySubscriptExpr *)anystmt;
        for (auto i = arrsubexpr->child_begin(); i != arrsubexpr->child_end(); ++i) {
          TraverseStmt(*i, count, scm);
        }
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
      case 97: //CXXConstructExpr
        construct = (CXXConstructExpr *)anystmt;
        //std::cout << construct->getStmtClassName() << ": ";
        //std::cout << construct->getType().getAsString() << "  ";
        //std::cout << construct->getConstructor()->getNumParams() << " ";
        for (unsigned i = 0; i < construct->getConstructor()->getNumParams(); ++i) {
          //std::cout << construct->getConstructor()->getParamDecl(i)->getType().getAsString() << "," 
            //<< construct->getConstructor()->getParamDecl(i)->getNameAsString() << "  ";
        }
        //std::cout << std::endl;
        break;
      case 107: //CXXNullptrLiteralExpr
        nullptrltr = (CXXNullPtrLiteralExpr *)anystmt;
        //std::cout << nullptrltr->getStmtClassName() << ": ";
        //std::cout << nullptrltr->getType().getAsString() << std::endl;
        break;
      case 116: //callExpr
        callee = true;
        ++count;
        callexpr = (CallExpr *)anystmt;
        //std::cout << callexpr->getStmtClassName() << ": " << std::endl;
        for (auto i = callexpr->child_begin(); i != callexpr->child_end(); ++i) {
          TraverseStmt(*i, count, scm);
        }
        callee = false;
        break;
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
        if (strcmp(dlrefexpr->getDecl()->getDeclKindName(), "Function") != 0) {
          if (call_def == true) {
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

          call_def = false;
          }
          //when new-define or update-define a var
          else if (definition == true) {
            //update def
            if (member_flag == true) {
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
          }
          else {
            //use
            //def-reverse to last definition
            int pos_blockid;
            for (pos_blockid = reverse_path.size(); pos_blockid >= 0; --pos_blockid) {
              if (reverse_path[pos_blockid] == block_id) {
                pos = pos_blockid;
                break;
              }
            }
            if (member_flag == true) {
              //add member Info
              std::pair<int, int> member_id(dlrefexpr->getDecl()->getID(), member_in_id);
              //member in current block
              if (struct_def_tmp[block_id].count(member_id) != 0) {
                all_node.add_struct_defuse_ln(member_id, idx, dlrefexpr->getLocation(), struct_def_tmp[block_id][member_id].def_line, line, struct_def_tmp[block_id][member_id].def_cl, cl);
                all_node.add_struct_funcname(member_id, idx, dlrefexpr->getLocation(), cur_funcname);
                all_node.add_struct_defuse_stmt(member_id, idx, dlrefexpr->getLocation(), struct_def_tmp[block_id][member_id].def_stmt, cur_stmt);
                all_node.add_struct_blockid(member_id, idx, dlrefexpr->getLocation(), block_id);
                all_node.add_struct_defined(member_id, idx, dlrefexpr->getLocation(), struct_def_tmp[block_id][member_id].defined);
              }
              else {  //member not in current block
                //check if member in other block 
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
                //if member_id not in 
                //add new member in.
                if (def_tmp[reverse_path[pos_blockid]].count(dlrefexpr->getDecl()->getID()) != 0) {
                  pos = pos_blockid;
                }
                else {
                  while (pos_blockid > 0) {
                    --pos_blockid;
                    if (def_tmp[reverse_path[pos_blockid]].count(dlrefexpr->getDecl()->getID()) != 0) {
                      pos = pos_blockid;
                      break;
                    }
                  }
                }
                //std::cout << pos_blockid << "******************\n";
                //std::cout << pos << "******************\n";
                all_node.add_struct_var(member_id, member_in_name, member_in_type);
                all_node.add_struct_defuse_ln(member_id, idx, dlrefexpr->getLocation(), def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_line, line, 
                                              def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_cl, cl);
                all_node.add_struct_funcname(member_id, idx, dlrefexpr->getLocation(), cur_funcname);
                all_node.add_struct_defuse_stmt(member_id, idx, dlrefexpr->getLocation(), def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_stmt, cur_stmt);
                all_node.add_struct_blockid(member_id, idx, dlrefexpr->getLocation(), block_id);
                all_node.add_struct_defined(member_id, idx, dlrefexpr->getLocation(), 0);

                //add to struct_def_tmp
                struct_def_tmp[block_id][member_id].def_line = def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_line;
                struct_def_tmp[block_id][member_id].def_cl = def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_cl;
                struct_def_tmp[block_id][member_id].varname = member_in_name;
                struct_def_tmp[block_id][member_id].def_stmt = cur_stmt;
                struct_def_tmp[block_id][member_id].defined = 0;
              }
            }
            else {
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
        }
        break;
      case 144: //floatingLiteral
        floatltr = (FloatingLiteral *)anystmt;
        //std::cout << floatltr->getStmtClassName() << ": ";
        //std::cout << floatltr->getType().getAsString() << "  " << floatltr->getValue().convertToDouble() << std::endl;
        break;
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
      case 148: //GNUNullExpr
        gnu = (GNUNullExpr *)anystmt;
        //std::cout << gnu->getStmtClassName() << ": ";
        //std::cout << gnu->getType().getAsString() << std::endl;
        break;
      case 152: //InitLitExpr
        initlst = (InitListExpr *)anystmt;
        //std::cout << initlst->getStmtClassName() << ": ";
        //std::cout << initlst->getType().getAsString() << "\n";
        for (auto i = initlst->begin(); i != initlst->end(); ++i) {
          TraverseStmt(*i, count, scm);
        }

        break;
      case 153: //IntegerLiteral
        intltr = (IntegerLiteral *)anystmt;
        //std::cout << intltr->getStmtClassName() << ": ";
        //std::cout << intltr->getType().getAsString() << "  ";
        //std::cout << intltr->getValue().getSExtValue() << std::endl;
        break;
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
        for (auto i = member->child_begin(); i != member->child_end(); ++i)
          TraverseStmt(*i, count, scm);
        
        member_flag = false;
        break;
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
        break;
      case 189: //StringLiteral
        strltr = (StringLiteral *)anystmt;
        //std::cout << strltr->getStmtClassName() << ": ";
        //std::cout << strltr->getType().getAsString() << "  ";
        //std::cout << strltr->getString().str() << std::endl;
        break;
      case 195: //UnaryOperator
        ++count;
        unary = (UnaryOperator *)anystmt;
        //std::cout << unary->getStmtClassName() << ": ";
        //std::cout << unary->getType().getAsString() << "  " << unary->getOpcodeStr(unary->getOpcode()).str() << std::endl;
        if ((strcmp(unary->getOpcodeStr(unary->getOpcode()).str().c_str(), "++") == 0) || (strcmp(unary->getOpcodeStr(unary->getOpcode()).str().c_str(), "--") == 0)) {
          definition = true;
        }
        for (auto i = unary->child_begin(); i != unary->child_end(); ++i) {
          TraverseStmt(*i, count, scm);
        }
        break;
      default: 
        //std::cout << anystmt->getStmtClassName() << anystmt->getStmtClass() << std::endl;
        break;
    }
  }
}

void TemplateChecker::check() {
  // for example
  auto astr_iter = getASTRsBegin();

  auto crds = astr_iter->second.GetCXXRecordDecls();//struct
  auto vds = astr_iter->second.GetVarDecls();

  for(auto crd : crds){
    TraverseDecl(crd,0,&(crd->getASTContext().getSourceManager()));
  }

  for (auto vd : vds) {
      //printf("Global variable: ");
      //std::cout << vd->getQualifiedNameAsString() << "  "
        //<< vd->getType().getAsString() << "  " << std::endl;
      cur_decl = vd;
      TraverseDecl(vd, 0, &(vd->getASTContext().getSourceManager()));
      getArrayVar(vd);
  }
  
  while (astr_iter != getASTRsEnd()) {
    auto fds = astr_iter->second.GetFunctionDecls();

    for (auto fd : fds) {
      //output_tree();
      //printf("FunctionDecl: \n");
      //std::cout << common::getFullName(fd) << std::endl;
      //fd->dump();
      //output_tree();
      //auto fd_cfg = common::buildCFG(fd);
      cfg_path[fd] = common::buildCFG(fd);
      //output_tree();
      //std::cout << fd->getQualifiedNameAsString() << " "
        //<< fd->getType().getAsString() << std::endl;
      cur_funcname = fd->getQualifiedNameAsString();
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
      //Stmt *cmpstmt = (fd->getBody());
      
      //TraverseStmt(cmpstmt);

      //printf("\n");

      // Traverse CFG
      //output_tree();
      LangOptions LangOpts;
      LangOpts.CPlusPlus = true;
      //cfg_path[fd]->dump(LangOpts, true);
      
      auto block = &(cfg_path[fd]->getEntry());
      auto exit = &cfg_path[fd]->getExit();
      //first
      CFGBlock *t;
      idx = 1;
      
      while(block->getBlockID() != exit->getBlockID()) {
        //handle loop
        if (std::find(reverse_path.begin(), reverse_path.end(), block->getBlockID()) != reverse_path.end()) {
          //std::cout << "in.\n";
          for (auto i = block->succ_begin(); i != block->succ_end(); ++i) {
            t = i->getReachableBlock();
          }
          block = t;
          //std::cout << block->getBlockID() << "\n";
        }
        if (block->getBlockID() != exit->getBlockID()) {
          stack_path.push(block);
          block_id = block->getBlockID();
          //CFGBlock *cb = convert(*block, fd_cfg->getBumpVectorContext(), block->getParent());
          path_tree[cur_fd][idx].push_back(block);
          reverse_path.push_back(block_id);
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
          block = block->succ_begin()->getReachableBlock();
        }
      }

      //output_tree();
      
      //std::cout << "out the first.\n";
      //next
      CFGBlock *current;
      CFGBlock *tmp;
      CFGBlock *next;
      while ((stack_path.empty() == false)) {
        //std::cout << stack_path.top()->succ_size() << "\n";
        if (stack_path.top()->succ_size() > 1) {
          tmp = stack_path.top();
          //find next successor of block tmp
          for (auto i = tmp->succ_begin(); i != tmp->succ_end(); ++i) {
            if (i->getReachableBlock()->getBlockID() == current->getBlockID()) {
              next = (++i)->getReachableBlock();
              break;
            }
          }
        }
        //the last child
        //all_node.output_node(&fd->getASTContext().getSourceManager());
        //all_node.output_struct_node(&fd->getASTContext().getSourceManager());
        //std::cout << "***************************\n";
        if (stack_path.top()->succ_size() <= 1 || next == tmp->succ_end()->getReachableBlock() || next->getBlockID() == exit->getBlockID()) {
          //std::cout << "in first if.\n";
          if (stack_path.top()->succ_size() > 1 && (next != tmp->succ_end()->getReachableBlock()) && (next->getBlockID() == exit->getBlockID())) {
            //std::cout << "in second if.\n";
            ++idx;
            //add path
            std::vector<int> all_id;
            std::vector<std::pair<int, int>> struct_all_id;
            std::map<int, def_use> e = all_node.get_node();
            std::map<std::pair<int, int>, def_use> struct_e = all_node.get_struct_node();
            for (auto i = e.begin(); i != e.end(); ++i)
              all_id.push_back((*i).first);
            for (auto i = struct_e.begin(); i != struct_e.end(); ++i)
              struct_all_id.push_back((*i).first);
            //join entry
            path_tree[cur_fd][idx].push_back(path_tree[cur_fd][1][0]);
            int path_idx = 1;
            for (auto b = ++(reverse_path.begin()); b != reverse_path.end(); ++b) {
              int bk = (*b);
              path_tree[cur_fd][idx].push_back(path_tree[cur_fd][idx-1][path_idx]);
              ++path_idx;

              for (auto m = all_id.begin(); m != all_id.end(); ++m) {
                int id = (*m);
                //std::cout << id << "\n";
                std::map<std::pair<int, SourceLocation>, element> n = all_node.get_defuse(id).get_du();
                for (auto f = n.begin(); f != n.end(); ++f) {
                  element e = (*f).second;
                  //std::cout << e.get_blockid() << "\n";
                  if (((*f).first.first == 1) && (e.get_blockid() == bk)) {
                    //std::cout << "in if.\n";
                    SourceLocation sl = (*f).first.second;
                    //std::cout << e.get_defuse_ln().first << " " << e.get_defuse_ln().second << "\n";
                    //sl.printToString(fd->getASTContext().getSourceManager());
                    all_node.add_defuse_ln(id, idx, sl, e.get_defuse_ln().first, e.get_defuse_ln().second, e.get_defuse_cl().first, e.get_defuse_cl().second);
                    all_node.add_funcname(id, idx, sl, e.get_funcname());
                    all_node.add_defuse_stmt(id, idx, sl, e.get_defuse_stmt().first, e.get_defuse_stmt().second);
                    all_node.add_blockid(id, idx, sl, bk);
                    all_node.add_defined(id, idx, sl, e.get_defined());
                  }
                }
              }
              //struct
              for (auto m = struct_all_id.begin(); m != struct_all_id.end(); ++m) {
                std::pair<int, int> struct_id = (*m);
                std::map<std::pair<int, SourceLocation>, element> struct_n = all_node.get_struct_defuse(struct_id).get_du();
                for (auto f = struct_n.begin(); f != struct_n.end(); ++f) {
                  element e = (*f).second;
                  
                  if (((*f).first.first == 1) && (e.get_blockid() == bk)) {
                    SourceLocation sl = (*f).first.second;
                    //std::cout << e.get_defuse_ln().first << "\t" << e.get_defuse_cl().first << "*******\n";
                    all_node.add_struct_defuse_ln(struct_id, idx, sl, e.get_defuse_ln().first, e.get_defuse_ln().second, e.get_defuse_cl().first, e.get_defuse_cl().second);
                    all_node.add_struct_funcname(struct_id, idx, sl, e.get_funcname());
                    all_node.add_struct_defuse_stmt(struct_id, idx, sl, e.get_defuse_stmt().first, e.get_defuse_stmt().second);
                    all_node.add_struct_blockid(struct_id, idx, sl, bk);
                    all_node.add_struct_defined(struct_id, idx, sl, e.get_defined());
                  }
                }
              }
            }
          }
          current = stack_path.top();
          stack_path.pop();
          reverse_path.pop_back();
        }
        else {
          ++idx;
          //add path
          std::vector<int> all_id;
          std::vector<std::pair<int, int>> struct_all_id;
          std::map<int, def_use> e = all_node.get_node();
          std::map<std::pair<int, int>, def_use> struct_e = all_node.get_struct_node();
          for (auto i = e.begin(); i != e.end(); ++i)
            all_id.push_back((*i).first);
          for (auto i = struct_e.begin(); i != struct_e.end(); ++i) {
            struct_all_id.push_back((*i).first);
          }
          //join entry
          path_tree[cur_fd][idx].push_back(path_tree[cur_fd][1][0]);
          int path_idx = 1;
          for (auto b = ++(reverse_path.begin()); b != reverse_path.end(); ++b) {
            int bk = (*b);
            path_tree[cur_fd][idx].push_back(path_tree[cur_fd][idx-1][path_idx]);
            ++path_idx;
            //std::cout << bk << "\n";
            //output_deftmp();
            for (auto m = all_id.begin(); m != all_id.end(); ++m) {
              int id = (*m);
              //std::cout << id << "\n";
              std::map<std::pair<int, SourceLocation>, element> n = all_node.get_defuse(id).get_du();
              for (auto f = n.begin(); f != n.end(); ++f) {
                element e = (*f).second;
                //std::cout << e.get_blockid() << "\n";
                if (((*f).first.first == 1) && (e.get_blockid() == bk)) {
                  SourceLocation sl = (*f).first.second;
                  all_node.add_defuse_ln(id, idx, sl, e.get_defuse_ln().first, e.get_defuse_ln().second, e.get_defuse_cl().first, e.get_defuse_cl().second);
                  all_node.add_funcname(id, idx, sl, e.get_funcname());
                  all_node.add_defuse_stmt(id, idx, sl, e.get_defuse_stmt().first, e.get_defuse_stmt().second);
                  all_node.add_blockid(id, idx, sl, bk);
                  all_node.add_defined(id, idx, sl, e.get_defined());
                }
              }
            }
            
            //struct
            for (auto m = struct_all_id.begin(); m != struct_all_id.end(); ++m) {
              std::pair<int, int> struct_id = (*m);
              std::map<std::pair<int, SourceLocation>, element> struct_n = all_node.get_struct_defuse(struct_id).get_du();
              for (auto f = struct_n.begin(); f != struct_n.end(); ++f) {
                element e = (*f).second;
                if (((*f).first.first == 1) && (e.get_blockid() == bk)) {
                  SourceLocation sl = (*f).first.second;
                  //std::cout << e.get_defuse_ln().first << "\t" << e.get_defuse_cl().first << "*******\n";
                  all_node.add_struct_defuse_ln(struct_id, idx, sl, e.get_defuse_ln().first, e.get_defuse_ln().second, e.get_defuse_cl().first, e.get_defuse_cl().second);
                  all_node.add_struct_funcname(struct_id, idx, sl, e.get_funcname());
                  all_node.add_struct_defuse_stmt(struct_id, idx, sl, e.get_defuse_stmt().first, e.get_defuse_stmt().second);
                  all_node.add_struct_blockid(struct_id, idx, sl, bk);
                  all_node.add_struct_defined(struct_id, idx, sl, e.get_defined());
                }
              }
            }
          }
          
          while(next->getBlockID() != exit->getBlockID()) {
            //std::cout << reverse_path.size() << "\n";
            if (std::find(reverse_path.begin(), reverse_path.end(), next->getBlockID()) != reverse_path.end()) {
              for (auto i = next->succ_begin(); i != next->succ_end(); ++i) {
                t = i->getReachableBlock();
              }
              next = t;
              //std::cout << next->getBlockID() << "\n";
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
              next = next->succ_begin()->getReachableBlock();
            }
          }
        }
        //std::cout << "a while end.\n";
      }
      //output_tree();
      //all_node.output_node(&fd->getASTContext().getSourceManager());
      //all_node.output_struct_node(&fd->getASTContext().getSourceManager());
      //std::cout << "out the whole while.\n";
      //output_tree();
      block_id = -1;
    }
    
    
    ++astr_iter;
  }

  
  //output_tree();

  //global
  /*
  for (auto i = global_def.begin(); i != global_def.end(); ++i) {
    std::cout << (*i).first << ": ";
    std::cout << (*i).second.def_line << "  ";
    std::cout << (*i).second.varname << "\n";
  }
  */

  
  //detec.detector(path_tree);

  //analyze
  /*
  for (auto b = decl_or_def.begin(); b != decl_or_def.end(); ++b) {
    if((*b).second == false) {
      std::cout << "error: ";
      std::cout << "variable" << "'" << (*b).first << "'" << "uninitialized!\n";
      std::vector<element> d = all_node.get_node()[(*b).first].get_du();
      for (auto b_in = d.begin(); b_in != d.end(); ++b_in) {
        (*b_in).output_element();
      }
    }
  }
  */
}
