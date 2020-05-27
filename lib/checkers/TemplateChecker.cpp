#include "checkers/TemplateChecker.h"
//#include "checkers/All_Detector.h"
#include "stack"
#include "algorithm"

struct Info {
  std::string varname;
  int def_line;
  Stmt *def_stmt;
  //tmp
  int block_id;
  int var_id;
  SourceLocation location;
};

extern std::map<FunctionDecl *, std::map<int, std::vector<CFGBlock *>>> path_tree;
extern std::map<FunctionDecl *, std::unique_ptr<clang::CFG>> cfg_path;

void TraverseDecl(Decl *anydecl, int count, SourceManager *scm);
void TraverseStmt(Stmt * anystmt, int count, SourceManager *scm);
defuse_node all_node;
bool definition = false;
bool integ = false;
bool callee = false;
//block_id, varID, Info
std::map<int, std::map<int, Info>> def_tmp;
std::vector<Info> assign_tmp;
bool saved = true;
std::map<int, Info> global_def;
std::map<std::string, bool> decl_or_def;
int block_id = -1;
std::vector<int> reverse_path;
std::stack<CFGBlock *> stack_path;
std::string g_varname;
std::string cur_funcname;
FunctionDecl *cur_fd;
//Detector detec;
int g_id;
Stmt *cur_stmt;
//test
//SourceLocation sl;
int idx = 0;



void output_deftmp() {
  //def_tmp
  for (auto i = def_tmp.begin(); i != def_tmp.end(); ++i) {
    std::cout << i->first << "\n";
    for (auto ii = i->second.begin(); ii != i->second.end(); ++ii) {
      std::cout << ii->first << "  ";
      std::cout << ii->second.varname << "  " << ii->second.def_line << "\n";
    }
  }
}

void output_tree() {
    for (auto i = path_tree.begin(); i != path_tree.end(); ++i) {
        std::cout << "FunctionName:  " << (*i).first->getQualifiedNameAsString() << ":  ";
        for (auto i_in = (*i).second.begin(); i_in != (*i).second.end(); ++i_in) {
          std::cout << "index: " << (*i_in).first << "\n";
          //for (auto bk = (*i_in).second.begin(); bk != (*i_in).second.end(); ++bk) {
            //std::cout << (*bk) << " ";
            
          //}
        }
        std::cout << "\n";
    }
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
  std::cout << "size: " << bk->size() << "\n";
  bk->setHasNoReturnElement();
  bk->setLabel(cfg_bk.getLabel());
  bk->setLoopTarget(cfg_bk.getLoopTarget());
  bk->setTerminator(cfg_bk.getTerminator());
  return bk;
}

void TraverseDecl(Decl *anydecl, int count, SourceManager *scm) {
  if (anydecl != nullptr) {
    for (int i = 0; i < count; i++)
      printf(" ");
    VarDecl *vardecl;
    unsigned line = 0;
    unsigned kind = anydecl->getKind();
    switch (kind)
    {
    case 57: //vardecl
      vardecl = (VarDecl *)anydecl;
      line = vardecl->getASTContext().getSourceManager().getSpellingLineNumber(vardecl->getLocation());
      //std::cout << vardecl->getID() << "\n";
      //std::cout << line << std::endl;
      /*
      if (sl == vardecl->getLocation()) {
        std::cout << "True.\n";
      }
      else
      {
        std::cout << "False.\n";
      }
      sl = vardecl->getLocation();
      */
      std::cout << vardecl->getQualifiedNameAsString() << " "
        << vardecl->getType().getAsString() << std::endl;
      std::cout << vardecl->getLocation().printToString(*scm) << "\n";
      all_node.add_var(vardecl->getID(), vardecl->getQualifiedNameAsString(), vardecl->getType().getAsString());
      //all_node.add_du(vardecl->getID(), line, 0, cur_funcname, cur_stmt, nullptr);
      //add
      if (block_id == -1) {
        global_def[vardecl->getID()].def_line = line;
        global_def[vardecl->getID()].varname = vardecl->getQualifiedNameAsString();
        global_def[vardecl->getID()].def_stmt = cur_stmt;
      }
      else {
        def_tmp[block_id][vardecl->getID()].varname = vardecl->getQualifiedNameAsString();
        def_tmp[block_id][vardecl->getID()].def_line = line;
        def_tmp[block_id][vardecl->getID()].def_stmt = cur_stmt;
      }
      decl_or_def[vardecl->getQualifiedNameAsString()] = false;
      //output_deftmp();
      //std::cout << count << std::endl;
      if (vardecl->ensureEvaluatedStmt() != nullptr) {
        ++count;
        if (vardecl->ensureEvaluatedStmt()->Value != nullptr) {
          //if (strcmp(vardecl->ensureEvaluatedStmt()->Value->getStmtClassName(), "IntegerLiteral") == 0
            //|| strcmp(vardecl->ensureEvaluatedStmt()->Value->getStmtClassName(), "InitListExpr") == 0) {
          integ = true;
          g_varname = vardecl->getQualifiedNameAsString();
          g_id = vardecl->getID();
          all_node.add_defuse_ln(g_id, idx, vardecl->getLocation(), line, 0);
          all_node.add_funcname(g_id, idx, vardecl->getLocation(), cur_funcname);
          all_node.add_defuse_stmt(g_id, idx, vardecl->getLocation(), cur_stmt, nullptr);
          all_node.add_blockid(g_id, idx, vardecl->getLocation(), block_id);
          //}
        }
        else
        {
          integ = false;
          all_node.add_defuse_ln(vardecl->getID(), idx, vardecl->getLocation(), line, 0);
          all_node.add_funcname(vardecl->getID(), idx, vardecl->getLocation(), cur_funcname);
          all_node.add_defuse_stmt(vardecl->getID(), idx, vardecl->getLocation(), cur_stmt, nullptr);
          all_node.add_blockid(vardecl->getID(), idx, vardecl->getLocation(), block_id);
        }
        TraverseStmt(vardecl->ensureEvaluatedStmt()->Value, count, scm);
      }
      else {
        all_node.add_defuse_ln(vardecl->getID(), idx, vardecl->getLocation(), line, 0);
        all_node.add_funcname(vardecl->getID(), idx, vardecl->getLocation(), cur_funcname);
        all_node.add_defuse_stmt(vardecl->getID(), idx, vardecl->getLocation(), cur_stmt, nullptr);
        all_node.add_blockid(vardecl->getID(), idx, vardecl->getLocation(), block_id);
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
    unsigned line;
    int pos;
    Info info_tmp;
    for (int k = 0; k < count; ++k)
      printf(" ");
    unsigned stmtclass = anystmt->getStmtClass();
    switch (stmtclass) {
      case 3: //breakstmt
        brstmt = (BreakStmt *)anystmt;
        std::cout << brstmt->getStmtClassName() << std::endl;
        break;
      case 8: //compoundstmt
        cmpstmt = (CompoundStmt *)anystmt;
        std::cout << cmpstmt->getStmtClassName() << std::endl;
        ++count;
        for (auto tmp = cmpstmt->body_begin(); tmp != cmpstmt->body_end(); ++tmp) {
          std::cout << (*tmp)->getBeginLoc().getPtrEncoding();
          TraverseStmt(*tmp, count, scm);
        }
        break;
      case 12: //declstmt
        ++count;
        dlstmt = (DeclStmt *)anystmt;
        std::cout << dlstmt->getStmtClassName() << ": " << std::endl;
        for (auto j = dlstmt->decl_begin(); j != dlstmt->decl_end(); ++j) {
          
          TraverseDecl(*j, count, scm);
        }
        break;
      case 14: //forstmt
        ++count;
        forstmt = (ForStmt *)anystmt;
        std::cout << forstmt->getStmtClassName() << ": " << std::endl;
        for (auto j = forstmt->child_begin(); j != forstmt->child_end(); ++j) {
          TraverseStmt(*j, count, scm);
        }
        break;
      case 16: //ifstmt
        ++count;
        ifstmt = (IfStmt *)anystmt;
        std::cout << ifstmt->getStmtClassName() << ": " << std::endl;
        for (auto i = ifstmt->child_begin(); i != ifstmt->child_end(); ++i)
          TraverseStmt(*i, count, scm);
        break;
      case 74: //returnstmt
        ++count;
        retstmt = (ReturnStmt *)anystmt;
        std::cout << retstmt->getStmtClassName() << ": " << std::endl;
        for (auto j = retstmt->child_begin(); j != retstmt->child_end(); ++j) {
          TraverseStmt(*j, count, scm);
        }
        break;
      case 79: //casestmt
        ++count;
        casestmt = (CaseStmt *)anystmt;
        std::cout << casestmt->getStmtClassName() << ": " << std::endl;
        for (auto i = casestmt->child_begin(); i != casestmt->child_end(); ++i) {
          TraverseStmt(*i, count, scm);
        }
        break;
      case 80: //defaultstmt
        ++count;
        defau = (DefaultStmt *)anystmt;
        std::cout << defau->getStmtClassName() << ": " << std::endl;
        for (auto i = defau->child_begin(); i != defau->child_end(); ++i)
          TraverseStmt(*i, count, scm);
        break;
      case 81: //switchstmt
        ++count;
        swstmt = (SwitchStmt *)anystmt;
        std::cout << swstmt->getStmtClassName() << ": " << std::endl;
        for (auto i = swstmt->child_begin(); i != swstmt->child_end(); ++i) {
          TraverseStmt(*i, count, scm);
        }
        break;
      case 92: //BinaryOperator
        ++count;
        bin = (BinaryOperator *)anystmt;
        std::cout << bin->getStmtClassName() << ": ";
        std::cout << bin->getType().getAsString() << "  " << bin->getOpcodeStr().str() << std::endl;
        if (bin->getOpcodeStr().str() == "=")
            definition = true;
          else 
            definition = false;
        for (auto i = bin->child_begin(); i != bin->child_end(); ++i) {
          
          TraverseStmt(*i, count, scm);
        }
        if (!saved) {
          //add node
          info_tmp = assign_tmp[assign_tmp.size()-1];
          assign_tmp.pop_back();
          def_tmp[info_tmp.block_id][info_tmp.var_id].def_line = info_tmp.def_line;
          def_tmp[info_tmp.block_id][info_tmp.var_id].varname = info_tmp.varname;
          def_tmp[info_tmp.block_id][info_tmp.var_id].def_stmt = info_tmp.def_stmt;

          //all_node.add_du(dlrefexpr->getDecl()->getID(), line, 0, cur_funcname, cur_stmt, nullptr);
          all_node.add_defuse_ln(info_tmp.var_id, idx, info_tmp.location, info_tmp.def_line, 0);
          all_node.add_funcname(info_tmp.var_id, idx, info_tmp.location, cur_funcname);
          all_node.add_defuse_stmt(info_tmp.var_id, idx, info_tmp.location, info_tmp.def_stmt, nullptr);
          all_node.add_blockid(info_tmp.var_id, idx, info_tmp.location, info_tmp.block_id);

          saved = true;
        }
        break;
      case 97: //CXXConstructExpr
        construct = (CXXConstructExpr *)anystmt;
        std::cout << construct->getStmtClassName() << ": ";
        std::cout << construct->getType().getAsString() << "  ";
        std::cout << construct->getConstructor()->getNumParams() << " ";
        for (unsigned i = 0; i < construct->getConstructor()->getNumParams(); ++i) {
          std::cout << construct->getConstructor()->getParamDecl(i)->getType().getAsString() << "," 
            << construct->getConstructor()->getParamDecl(i)->getNameAsString() << "  ";
        }
        std::cout << std::endl;
        break;
      case 107: //CXXNullptrLiteralExpr
        nullptrltr = (CXXNullPtrLiteralExpr *)anystmt;
        std::cout << nullptrltr->getStmtClassName() << ": ";
        std::cout << nullptrltr->getType().getAsString() << std::endl;
        break;
      case 116: //callExpr
        callee = true;
        ++count;
        callexpr = (CallExpr *)anystmt;
        std::cout << callexpr->getStmtClassName() << ": " << std::endl;
        for (auto i = callexpr->child_begin(); i != callexpr->child_end(); ++i) {
          TraverseStmt(*i, count, scm);
        }
        callee = false;
        break;
      case 122: //CStyleCastExpr
        ++count;
        cscastexpr = (CStyleCastExpr *)anystmt;
        std::cout << cscastexpr->getStmtClassName() << ": ";
        std::cout << cscastexpr->getType().getAsString() << std:: endl;
        if (*(cscastexpr->child_begin()) != nullptr) {
          for (auto i = cscastexpr->child_begin(); i != cscastexpr->child_end(); ++i) {
            TraverseStmt(*i, count, scm);
          }
        }
        break;
      case 129: //ImplicitCastExpr
        imcastexpr = (ImplicitCastExpr *)anystmt;
        std::cout << imcastexpr->getStmtClassName() << ": ";
        std::cout << imcastexpr->getType().getAsString() << std::endl;
        if (*(imcastexpr->child_begin()) != nullptr) {
          ++count;
          for (auto i = imcastexpr->child_begin(); i != imcastexpr->child_end(); ++i) {
            TraverseStmt(*i, count, scm);
          }
        }
        break;
      case 136: //DeclRefExpr
        dlrefexpr = (DeclRefExpr *)anystmt;
        std::cout << dlrefexpr->getStmtClassName() << ": ";
        std::cout << dlrefexpr->getType().getAsString() << "  ";
        std::cout << dlrefexpr->getDecl()->getDeclKindName() << "  ";
        std::cout << dlrefexpr->getDecl()->getNameAsString() << std::endl;
        std::cout << dlrefexpr->getLocation().printToString(*scm) << "\n";
        line = scm->getSpellingLineNumber(dlrefexpr->getLocation());
        //std::cout << line << std::endl;
        if (strcmp(dlrefexpr->getDecl()->getDeclKindName(), "Function") != 0) {
          if (definition == true) {
            //std::cout << "definition == true" << "\n";
            //output_deftmp();
            //update or add def
            //def_tmp[block_id] = "var+info"
            //def_tmp[block_id][dlrefexpr->getDecl()->getID()].def_line = line;
            //def_tmp[block_id][dlrefexpr->getDecl()->getID()].varname = dlrefexpr->getDecl()->getNameAsString();
            //def_tmp[block_id][dlrefexpr->getDecl()->getID()].def_stmt = cur_stmt;
            decl_or_def[dlrefexpr->getDecl()->getNameAsString()] = true;
            info_tmp.block_id = block_id;
            info_tmp.def_line = line;
            info_tmp.def_stmt = cur_stmt;
            info_tmp.var_id = dlrefexpr->getDecl()->getID();
            info_tmp.varname = dlrefexpr->getDecl()->getNameAsString();
            info_tmp.location = dlrefexpr->getLocation();
            assign_tmp.push_back(info_tmp);
            saved = false;
            //std::cout << line << std::endl;
            //all_node.add_du(dlrefexpr->getDecl()->getID(), line, 0, cur_funcname, cur_stmt, nullptr);
            //all_node.add_defuse_ln(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), line, 0);
            //all_node.add_funcname(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), cur_funcname);
            //all_node.add_defuse_stmt(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), cur_stmt, nullptr);
            //all_node.add_blockid(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), block_id);
            definition = false;
            //output_deftmp();
          }
          else {
            //use
            //def-reverse to last definition
            //all_node.add_du(var, def_tmp)
            //std::cout << "definition == false" << "\n";
            //output_deftmp();
            for (int i = reverse_path.size(); i >= 0; --i) {
              if (reverse_path[i] == block_id) {
                pos = i;
                break;
              }
            }
            //std::cout << "afetr for: " << "\n";
            //output_deftmp();
            if (def_tmp[block_id].find(dlrefexpr->getDecl()->getID()) != def_tmp[block_id].end()) {
              //std::cout << block_id << std::endl;
              //std::cout << def_tmp[block_id][dlrefexpr->getDecl()->getID()].def_line << std::endl;
              //all_node.add_du(dlrefexpr->getDecl()->getID(), def_tmp[block_id][dlrefexpr->getDecl()->getID()].def_line, line, 
                //cur_funcname, def_tmp[block_id][dlrefexpr->getDecl()->getID()].def_stmt, cur_stmt);
              all_node.add_defuse_ln(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), def_tmp[block_id][dlrefexpr->getDecl()->getID()].def_line, line);
              all_node.add_funcname(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), cur_funcname);
              all_node.add_defuse_stmt(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), def_tmp[block_id][dlrefexpr->getDecl()->getID()].def_stmt, cur_stmt);
              all_node.add_blockid(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), block_id);
            }
            else {
              --pos;
              //std::cout << pos << std::endl;
              while (pos >= 0) {
                if (def_tmp[reverse_path[pos]].find(dlrefexpr->getDecl()->getID()) != def_tmp[reverse_path[pos]].end()) {
                  //std::cout << pos << std::endl;
                  //std::cout << reverse_path[pos] << std::endl;
                  //std::cout << "varname: " << def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].varname << std::endl;
                  //std::cout << def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_line << std::endl;
                  //all_node.add_du(dlrefexpr->getDecl()->getID(), def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_line, line, 
                    //cur_funcname, def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_stmt, cur_stmt);
                  all_node.add_defuse_ln(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_line, line);
                  all_node.add_funcname(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), cur_funcname);
                  all_node.add_defuse_stmt(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getID()].def_stmt, cur_stmt);
                  all_node.add_blockid(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), block_id);
                  break;
                }
                --pos;
              }
              //std::cout << pos << std::endl;
            }
            //std::cout << "after if-else: \n";
            //output_deftmp();
            if (pos == -1) {
              //std::cout << global_def[dlrefexpr->getDecl()->getID()].def_line << std::endl;
              //all_node.add_du(dlrefexpr->getDecl()->getID(), global_def[dlrefexpr->getDecl()->getID()].def_line, line, 
                //cur_funcname, global_def[dlrefexpr->getDecl()->getID()].def_stmt, cur_stmt);
              all_node.add_defuse_ln(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), global_def[dlrefexpr->getDecl()->getID()].def_line, line);
              all_node.add_funcname(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), cur_funcname);
              all_node.add_defuse_stmt(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), global_def[dlrefexpr->getDecl()->getID()].def_stmt, cur_stmt);
              all_node.add_blockid(dlrefexpr->getDecl()->getID(), idx, dlrefexpr->getLocation(), block_id);
            }
            //std::cout << "after pos == -1\n";
            //output_deftmp();
          }
        }
        break;
      case 144: //floatingLiteral
        floatltr = (FloatingLiteral *)anystmt;
        std::cout << floatltr->getStmtClassName() << ": ";
        std::cout << floatltr->getType().getAsString() << "  " << floatltr->getValue().convertToDouble() << std::endl;
        break;
      case 145: //ConstantExpr
        constmt = (ConstantExpr *)anystmt;
        std::cout << constmt->getStmtClassName() << ": ";
        std::cout << constmt->getType().getAsString() << "  ";
        if (constmt->getAPValueResult().isInt() == true)
          std::cout << constmt->getAPValueResult().getInt().getSExtValue() << std::endl;
        ++count;
        for (auto i = constmt->child_begin(); i != constmt->child_end(); ++i) {
          TraverseStmt(*i, count, scm);
        }
        break;
      case 148: //GNUNullExpr
        gnu = (GNUNullExpr *)anystmt;
        std::cout << gnu->getStmtClassName() << ": ";
        std::cout << gnu->getType().getAsString() << std::endl;
        break;
      case 152: //InitLitExpr
        initlst = (InitListExpr *)anystmt;
        std::cout << initlst->getStmtClassName() << ": ";
        std::cout << initlst->getType().getAsString() << "\n";
        for (auto i = initlst->begin(); i != initlst->end(); ++i) {
          TraverseStmt(*i, count, scm);
        }
        if (integ == true) {
          //update or add def
          //std::cout << block_id << varname << std::endl;
          def_tmp[block_id][g_id].def_line = scm->getSpellingLineNumber(intltr->getLocation());
          def_tmp[block_id][g_id].varname = g_varname;
          def_tmp[block_id][g_id].def_stmt = cur_stmt;
          decl_or_def[g_varname] = true;
          //all_node.add_du(g_id, def_tmp[block_id][g_id].def_line, 0, cur_funcname, cur_stmt, nullptr);
          std::cout << initlst->getExprLoc().printToString(*scm) << "\n";
          all_node.add_defuse_ln(g_id, idx, initlst->getExprLoc(), def_tmp[block_id][g_id].def_line, 0);
          all_node.add_funcname(g_id, idx, initlst->getExprLoc(), cur_funcname);
          all_node.add_defuse_stmt(g_id, idx, initlst->getExprLoc(), cur_stmt, nullptr);
          all_node.add_blockid(g_id, idx, initlst->getExprLoc(), block_id);
          integ = false;
        }
        break;
      case 153: //IntegerLiteral
        intltr = (IntegerLiteral *)anystmt;
        std::cout << intltr->getStmtClassName() << ": ";
        std::cout << intltr->getType().getAsString() << "  ";
        std::cout << intltr->getValue().getSExtValue() << std::endl;
        if (integ == true) {
          //update or add def
          //std::cout << block_id << varname << std::endl;
          def_tmp[block_id][g_id].def_line = scm->getSpellingLineNumber(intltr->getLocation());
          def_tmp[block_id][g_id].varname = g_varname;
          def_tmp[block_id][g_id].def_stmt = cur_stmt;
          decl_or_def[g_varname] = true;
          //all_node.add_du(g_id, def_tmp[block_id][g_id].def_line, 0, cur_funcname, cur_stmt, nullptr);
          //all_node.add_defuse_ln(g_id, idx, intltr->getLocation(), def_tmp[block_id][g_id].def_line, 0);
          //all_node.add_funcname(g_id, idx, intltr->getLocation(), cur_funcname);
          //all_node.add_defuse_stmt(g_id, idx, intltr->getLocation(), cur_stmt, nullptr);
          //all_node.add_blockid(g_id, idx, intltr->getLocation(), block_id);
          integ = false;
          //output_deftmp();
        }
        break;
      case 158: //MemberExpr
        ++count;
        member = (MemberExpr *)anystmt;
        std::cout << member->getStmtClassName() << ": ";
        std::cout << member->getType().getAsString() << "  " << member->getMemberNameInfo().getAsString() << std::endl;
        for (auto i = member->child_begin(); i != member->child_end(); ++i)
          TraverseStmt(*i, count, scm);
        break;
      case 181: //ParenExpr
        paren = (ParenExpr *)anystmt;
        std::cout << paren->getStmtClassName() << ": ";
        std::cout << paren->getType().getAsString() << "  ";
        switch (paren->getValueKind()) {
          case 0: std::cout << "rvalue" << std::endl; break;
          case 1: std::cout << "lvalue" << std::endl; break;
          case 2: std::cout << "xvalue" << std:: endl; break;
          default: break;
        }
        break;
      case 189: //StringLiteral
        strltr = (StringLiteral *)anystmt;
        std::cout << strltr->getStmtClassName() << ": ";
        std::cout << strltr->getType().getAsString() << "  ";
        std::cout << strltr->getString().str() << std::endl;
        break;
      case 195: //UnaryOperator
        ++count;
        unary = (UnaryOperator *)anystmt;
        std::cout << unary->getStmtClassName() << ": ";
        std::cout << unary->getType().getAsString() << "  " << unary->getOpcodeStr(unary->getOpcode()).str() << std::endl;
        for (auto i = unary->child_begin(); i != unary->child_end(); ++i) {
          TraverseStmt(*i, count, scm);
        }
        break;
      default: 
        std::cout << anystmt->getStmtClassName() << anystmt->getStmtClass() << std::endl;
        break;
    }
  }
}

void TemplateChecker::check() {
  // for example
  auto astr_iter = getASTRsBegin();

  
  auto vds = astr_iter->second.GetVarDecls();
  for (auto vd : vds) {
      printf("Global variable: ");
      std::cout << vd->getQualifiedNameAsString() << "  "
        << vd->getType().getAsString() << "  " << std::endl;
      TraverseDecl(vd, 0, &(vd->getASTContext().getSourceManager()));
  }
  
  while (astr_iter != getASTRsEnd()) {
    auto fds = astr_iter->second.GetFunctionDecls();

    for (auto fd : fds) {
      //output_tree();
      printf("FunctionDecl: \n");
      //std::cout << common::getFullName(fd) << std::endl;
      //fd->dump();
      //output_tree();
      //auto fd_cfg = common::buildCFG(fd);
      cfg_path[fd] = common::buildCFG(fd);
      //output_tree();
      std::cout << fd->getQualifiedNameAsString() << " "
        << fd->getType().getAsString() << std::endl;
      cur_funcname = fd->getQualifiedNameAsString();
      //output_tree();
      cur_fd = fd;

    

      
      //parameter
      if (fd->getNumParams() > 0) {
        printf("  Parameters: \n");
        for (unsigned i = 0; i < fd->getNumParams(); i++) {
          std::cout << "  " << fd->getParamDecl(i)->getQualifiedNameAsString() << " ";
          std::cout << "  " << fd->getParamDecl(i)->getType().getAsString() << std::endl;
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
      cfg_path[fd]->dump(LangOpts, true);
      
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
        std::cout << stack_path.top()->succ_size() << "\n";
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
        if (stack_path.top()->succ_size() <= 1 || next == tmp->succ_end()->getReachableBlock() || next->getBlockID() == exit->getBlockID()) {
          std::cout << "in first if.\n";
          if (stack_path.top()->succ_size() > 1 && (next != tmp->succ_end()->getReachableBlock()) && (next->getBlockID() == exit->getBlockID())) {
            std::cout << "in second if.\n";
            ++idx;
            //add path
            std::vector<int> all_id;
            std::map<int, def_use> e = all_node.get_node();
            for (auto i = e.begin(); i != e.end(); ++i)
              all_id.push_back((*i).first);
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
                    //std::cout << "in if.\n";
                    SourceLocation sl = (*f).first.second;
                    //std::cout << e.get_defuse_ln().first << " " << e.get_defuse_ln().second << "\n";
                    //sl.printToString(fd->getASTContext().getSourceManager());
                    all_node.add_defuse_ln(id, idx, sl, e.get_defuse_ln().first, e.get_defuse_ln().second);
                    all_node.add_funcname(id, idx, sl, e.get_funcname());
                    all_node.add_defuse_stmt(id, idx, sl, e.get_defuse_stmt().first, e.get_defuse_stmt().second);
                    all_node.add_blockid(id, idx, sl, bk);
                  }
                }
              }
            }
            std::cout << "out second if.\n";
          }
          std::cout << "out first if.\n";
          current = stack_path.top();
          stack_path.pop();
          reverse_path.pop_back();
          std::cout << "rest size: " << stack_path.size() << "\n";
        }
        else {
          ++idx;
          //add path
          std::vector<int> all_id;
          std::map<int, def_use> e = all_node.get_node();
          for (auto i = e.begin(); i != e.end(); ++i)
            all_id.push_back((*i).first);
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
                  //std::cout << "in if.\n";
                  SourceLocation sl = (*f).first.second;
                  //std::cout << e.get_defuse_ln().first << " " << e.get_defuse_ln().second << "\n";
                  //sl.printToString(fd->getASTContext().getSourceManager());
                  all_node.add_defuse_ln(id, idx, sl, e.get_defuse_ln().first, e.get_defuse_ln().second);
                  all_node.add_funcname(id, idx, sl, e.get_funcname());
                  all_node.add_defuse_stmt(id, idx, sl, e.get_defuse_stmt().first, e.get_defuse_stmt().second);
                  all_node.add_blockid(id, idx, sl, bk);
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
        std::cout << "a while end.\n";
      }
      //output_tree();
      all_node.output_node(&fd->getASTContext().getSourceManager());
      //std::cout << "out the whole while.\n";
      //output_tree();
    }
    
    
    ++astr_iter;
  }

  
  //output_tree();

  //global
  for (auto i = global_def.begin(); i != global_def.end(); ++i) {
    std::cout << (*i).first << ": ";
    std::cout << (*i).second.def_line << "  ";
    std::cout << (*i).second.varname << "\n";
  }

  
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