#ifndef V_U_H
#define V_U_H
#include"Detector.h"

class Var_Uninitialized{
//void TraverseDecl(Decl *anydecl, int count, SourceManager *scm);
//void TraverseStmt(Stmt * anystmt, int count, SourceManager *scm);
defuse_node all_node;
bool definition = true;
bool integ = true;
std::map<int, std::map<std::string, int>> def_tmp;
std::map<std::string, bool> decl_or_def;
unsigned block_id;
std::vector<int> reverse_path;
std::stack<CFGBlock *> stack_path;
std::string varname;



void TraverseDecl(Decl *anydecl, int count, SourceManager *scm) {
	if (anydecl != nullptr) {
		//for (int i = 0; i < count; i++)
		  //printf(" ");
		VarDecl *vardecl;
		unsigned line = 0;
		unsigned kind = anydecl->getKind();
		switch (kind)
		{
		case 57: //vardecl
			vardecl = (VarDecl *)anydecl;
			line = vardecl->getASTContext().getSourceManager().getSpellingLineNumber(vardecl->getLocation());
			////cout << line << std::endl;
			//cout << vardecl->getQualifiedNameAsString() << " "
			 // << vardecl->getType().getAsString() << std::endl;
			all_node.add_var(vardecl->getQualifiedNameAsString());
			//add
			def_tmp[block_id][vardecl->getQualifiedNameAsString()] = line;
			decl_or_def[vardecl->getQualifiedNameAsString()] = false;
			////cout << count << std::endl;
			if (vardecl->ensureEvaluatedStmt() != nullptr) {
				++count;
				if (vardecl->ensureEvaluatedStmt()->Value != nullptr) {
					if (strcmp(vardecl->ensureEvaluatedStmt()->Value->getStmtClassName(), "IntegerLiteral") == 0) {
						integ = true;
						varname = vardecl->getQualifiedNameAsString();
					}
				}
				else
				{
					integ = false;
				}
				TraverseStmt(vardecl->ensureEvaluatedStmt()->Value, count, scm);
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
		unsigned line;
		int pos;
		//for (int k = 0; k < count; ++k)
		  //printf(" ");
		unsigned stmtclass = anystmt->getStmtClass();
		switch (stmtclass) {
		case 3: //breakstmt
			brstmt = (BreakStmt *)anystmt;
			//cout << brstmt->getStmtClassName() << std::endl;
			break;
		case 8: //compoundstmt
			cmpstmt = (CompoundStmt *)anystmt;
			//cout << cmpstmt->getStmtClassName() << std::endl;
			++count;
			for (auto tmp = cmpstmt->body_begin(); tmp != cmpstmt->body_end(); ++tmp) {
				//cout << (*tmp)->getBeginLoc().getPtrEncoding();
				TraverseStmt(*tmp, count, scm);
			}
			break;
		case 12: //declstmt
			++count;
			dlstmt = (DeclStmt *)anystmt;
			//cout << dlstmt->getStmtClassName() << ": " << std::endl;
			for (auto j = dlstmt->decl_begin(); j != dlstmt->decl_end(); ++j) {

				TraverseDecl(*j, count, scm);
			}
			break;
		case 14: //forstmt
			++count;
			forstmt = (ForStmt *)anystmt;
			//cout << forstmt->getStmtClassName() << ": " << std::endl;
			for (auto j = forstmt->child_begin(); j != forstmt->child_end(); ++j) {
				TraverseStmt(*j, count, scm);
			}
			break;
		case 16: //ifstmt
			++count;
			ifstmt = (IfStmt *)anystmt;
			//cout << ifstmt->getStmtClassName() << ": " << std::endl;
			for (auto i = ifstmt->child_begin(); i != ifstmt->child_end(); ++i)
				TraverseStmt(*i, count, scm);
			break;
		case 74: //returnstmt
			++count;
			retstmt = (ReturnStmt *)anystmt;
			//cout << retstmt->getStmtClassName() << ": " << std::endl;
			for (auto j = retstmt->child_begin(); j != retstmt->child_end(); ++j) {
				TraverseStmt(*j, count, scm);
			}
			break;
		case 79: //casestmt
			++count;
			casestmt = (CaseStmt *)anystmt;
			//cout << casestmt->getStmtClassName() << ": " << std::endl;
			for (auto i = casestmt->child_begin(); i != casestmt->child_end(); ++i) {
				TraverseStmt(*i, count, scm);
			}
			break;
		case 80: //defaultstmt
			++count;
			defau = (DefaultStmt *)anystmt;
			//cout << defau->getStmtClassName() << ": " << std::endl;
			for (auto i = defau->child_begin(); i != defau->child_end(); ++i)
				TraverseStmt(*i, count, scm);
			break;
		case 81: //switchstmt
			++count;
			swstmt = (SwitchStmt *)anystmt;
			//cout << swstmt->getStmtClassName() << ": " << std::endl;
			for (auto i = swstmt->child_begin(); i != swstmt->child_end(); ++i) {
				TraverseStmt(*i, count, scm);
			}
			break;
		case 92: //BinaryOperator
			++count;
			bin = (BinaryOperator *)anystmt;
			//cout << bin->getStmtClassName() << ": ";
			//cout << bin->getType().getAsString() << "  " << bin->getOpcodeStr().str() << std::endl;
			for (auto i = bin->child_begin(); i != bin->child_end(); ++i) {
				if (strcmp((*i)->getStmtClassName(), "DeclRefExpr") == 0)
					definition = true;
				else
					definition = false;
				TraverseStmt(*i, count, scm);
			}
			break;
		case 97: //CXXConstructExpr
			construct = (CXXConstructExpr *)anystmt;
			//cout << construct->getStmtClassName() << ": ";
			//cout << construct->getType().getAsString() << "  ";
			//cout << construct->getConstructor()->getNumParams() << " ";
			for (unsigned i = 0; i < construct->getConstructor()->getNumParams(); ++i) {
				//cout << construct->getConstructor()->getParamDecl(i)->getType().getAsString() << "," 
				 // << construct->getConstructor()->getParamDecl(i)->getNameAsString() << "  ";
			}
			//cout << std::endl;
			break;
		case 107: //CXXNullptrLiteralExpr
			nullptrltr = (CXXNullPtrLiteralExpr *)anystmt;
			//cout << nullptrltr->getStmtClassName() << ": ";
			//cout << nullptrltr->getType().getAsString() << std::endl;
			break;
		case 116: //callExpr
			++count;
			callexpr = (CallExpr *)anystmt;
			//cout << callexpr->getStmtClassName() << ": " << std::endl;
			for (auto i = callexpr->child_begin(); i != callexpr->child_end(); ++i) {
				TraverseStmt(*i, count, scm);
			}
			break;
		case 122: //CStyleCastExpr
			++count;
			cscastexpr = (CStyleCastExpr *)anystmt;
			//cout << cscastexpr->getStmtClassName() << ": ";
			//cout << cscastexpr->getType().getAsString() << std:: endl;
			if (*(cscastexpr->child_begin()) != nullptr) {
				for (auto i = cscastexpr->child_begin(); i != cscastexpr->child_end(); ++i) {
					TraverseStmt(*i, count, scm);
				}
			}
			break;
		case 129: //ImplicitCastExpr
			imcastexpr = (ImplicitCastExpr *)anystmt;
			//cout << imcastexpr->getStmtClassName() << ": ";
			//cout << imcastexpr->getType().getAsString() << std::endl;
			if (*(imcastexpr->child_begin()) != nullptr) {
				++count;
				for (auto i = imcastexpr->child_begin(); i != imcastexpr->child_end(); ++i) {
					TraverseStmt(*i, count, scm);
				}
			}
			break;
		case 136: //DeclRefExpr
			dlrefexpr = (DeclRefExpr *)anystmt;
			//cout << dlrefexpr->getStmtClassName() << ": ";
			//cout << dlrefexpr->getType().getAsString() << "  ";
			//cout << dlrefexpr->getDecl()->getDeclKindName() << "  ";
			//cout << dlrefexpr->getDecl()->getNameAsString() << std::endl;
			line = scm->getSpellingLineNumber(dlrefexpr->getLocation());
			////cout << line << std::endl;
			if (definition == true) {
				//update or add def
				//def_tmp[block_id] = "var+info"
				def_tmp[block_id][dlrefexpr->getDecl()->getNameAsString()] = line;
				decl_or_def[dlrefexpr->getDecl()->getNameAsString()] = true;
				definition = false;
			}
			else {
				//use
				//def-reverse to last definition
				//all_node.add_du(var, def_tmp)
				for (int i = 0; i < reverse_path.size(); ++i) {
					if (reverse_path[i] == block_id) {
						pos = i;
						break;
					}
				}
				if (def_tmp[block_id].find(dlrefexpr->getDecl()->getNameAsString()) != def_tmp[block_id].end()) {
					all_node.add_du(dlrefexpr->getDecl()->getNameAsString(), def_tmp[block_id][dlrefexpr->getDecl()->getNameAsString()], line);
				}
				else {
					--pos;
					////cout << pos << std::endl;
					while (pos >= 0) {
						if (def_tmp[reverse_path[pos]].find(dlrefexpr->getDecl()->getNameAsString()) != def_tmp[reverse_path[pos]].end()) {
							all_node.add_du(dlrefexpr->getDecl()->getNameAsString(), def_tmp[reverse_path[pos]][dlrefexpr->getDecl()->getNameAsString()], line);
							break;
						}
						--pos;
					}
					////cout << pos << std::endl;
				}
			}
			break;
		case 144: //floatingLiteral
			floatltr = (FloatingLiteral *)anystmt;
			//cout << floatltr->getStmtClassName() << ": ";
			//cout << floatltr->getType().getAsString() << "  " << floatltr->getValue().convertToDouble() << std::endl;
			break;
		case 145: //ConstantExpr
			constmt = (ConstantExpr *)anystmt;
			//cout << constmt->getStmtClassName() << ": ";
			//cout << constmt->getType().getAsString() << "  ";
			if (constmt->getAPValueResult().isInt() == true)
				//cout << constmt->getAPValueResult().getInt().getSExtValue() << std::endl;
				++count;
			for (auto i = constmt->child_begin(); i != constmt->child_end(); ++i) {
				TraverseStmt(*i, count, scm);
			}
			break;
		case 148: //GNUNullExpr
			gnu = (GNUNullExpr *)anystmt;
			//cout << gnu->getStmtClassName() << ": ";
			//cout << gnu->getType().getAsString() << std::endl;
			break;
		case 153: //IntegerLiteral
			intltr = (IntegerLiteral *)anystmt;
			//cout << intltr->getStmtClassName() << ": ";
			//cout << intltr->getType().getAsString() << "  ";
			//cout << intltr->getValue().getSExtValue() << std::endl;
			if (integ == true) {
				//update or add def
				////cout << block_id << varname << std::endl;
				def_tmp[block_id][varname] = scm->getSpellingLineNumber(intltr->getLocation());
				decl_or_def[varname] = true;
				integ = false;
			}
			break;
		case 158: //MemberExpr
			++count;
			member = (MemberExpr *)anystmt;
			//cout << member->getStmtClassName() << ": ";
			//cout << member->getType().getAsString() << "  " << member->getMemberNameInfo().getAsString() << std::endl;
			for (auto i = member->child_begin(); i != member->child_end(); ++i)
				TraverseStmt(*i, count, scm);
			break;
		case 181: //ParenExpr
			paren = (ParenExpr *)anystmt;
			//cout << paren->getStmtClassName() << ": ";
			//cout << paren->getType().getAsString() << "  ";
			switch (paren->getValueKind()) {
			case 0: //cout << "rvalue" << std::endl; break;
			case 1: //cout << "lvalue" << std::endl; break;
			case 2: //cout << "xvalue" << std:: endl; break;
			default: break;
			}
			break;
		case 189: //StringLiteral
			strltr = (StringLiteral *)anystmt;
			//cout << strltr->getStmtClassName() << ": ";
			//cout << strltr->getType().getAsString() << "  ";
			//cout << strltr->getString().str() << std::endl;
			break;
		case 195: //UnaryOperator
			++count;
			unary = (UnaryOperator *)anystmt;
			//cout << unary->getStmtClassName() << ": ";
			//cout << unary->getType().getAsString() << "  " << unary->getOpcodeStr(unary->getOpcode()).str() << std::endl;
			for (auto i = unary->child_begin(); i != unary->child_end(); ++i) {
				TraverseStmt(*i, count, scm);
			}
			break;
		default:
			//cout << anystmt->getStmtClassName() << anystmt->getStmtClass() << std::endl;
			break;
		}
	}
}
public:
void VU_Entry1(std::unordered_map<std::string, ASTResource>::iterator astr_iter)
{

	auto vds = astr_iter->second.GetVarDecls();
	for (auto vd : vds) {
		//printf("Global variable: ");
		//cout << vd->getQualifiedNameAsString() << "  "
		 // << vd->getType().getAsString() << "  " << std::endl;
		TraverseDecl(vd, 0, &(vd->getASTContext().getSourceManager()));
	}
}
void VU_Entry2(clang::FunctionDecl*fd)
{

	//printf("FunctionDecl: ");
	////cout << common::getFullName(fd) << std::endl;
	auto fd_cfg = common::buildCFG(fd);

	//cout << fd->getQualifiedNameAsString() << " "
	 // << fd->getType().getAsString() << std::endl;

	//parameter
	if (fd->getNumParams() > 0) {
		//printf("  Parameters: \n");
		for (unsigned i = 0; i < fd->getNumParams(); i++) {
			//cout << "  " << fd->getParamDecl(i)->getQualifiedNameAsString() << " ";
			//cout << "  " << fd->getParamDecl(i)->getType().getAsString() << std::endl;
		}
	}

	//Compoundstmt
	//Stmt *cmpstmt = (fd->getBody());

	//TraverseStmt(cmpstmt);

	//printf("\n");

	// Traverse CFG
	LangOptions LangOpts;
	LangOpts.CPlusPlus = true;
	//fd_cfg->dump(LangOpts, true);
	auto block = &(fd_cfg->getEntry());
	auto exit = &fd_cfg->getExit();
	while (block->getBlockID() != exit->getBlockID()) {
		block_id = block->getBlockID();
		reverse_path.push_back(block_id);
		for (auto I = block->begin(); I != block->end(); ++I) {
			if (Optional<CFGStmt> CS = (*I).getAs<CFGStmt>()) {
				Stmt *S = const_cast<Stmt *>(CS->getStmt());
				assert(S != nullptr && "Expecting non-null Stmt");
				////cout <<"1.\n"<<std::endl;
				TraverseStmt(S, 0, &fd->getASTContext().getSourceManager());
			}
		}
		block = block->succ_begin()->getReachableBlock();
	}

}
void VU_Detect()
{
	//all_node.output_node();
  //analyze
	for (auto b = decl_or_def.begin(); b != decl_or_def.end(); ++b) {
		if ((*b).second == false) {

			//cout << "error: ";
			//cout << "variable" << "'" << (*b).first << "'" << "uninitialized!\n";
			error_info*head = NULL;
			error_info*tail = NULL;
			int decline = -1;
			std::vector<std::pair<int, int>> d = all_node.get_node()[(*b).first].get_du();
			for (auto b_in = d.begin(); b_in != d.end(); ++b_in) {
				error_info*e = new_error_info(NULL, FILENAME, (*b_in).second, 0, TYPE_NOTE, VU_ERROR_TYPE_USE);
				if (head == NULL)
				{
					head = e;
					decline = (*b_in).first;
					tail = e;
				}
				else
				{
					tail->next = e;
				}
				//cout << "declaration in line: " << (*b_in).first << " used in line: " << (*b_in).second << "\n";
			}
			if (decline != -1)
			{
				error_info* decl = new_error_info(head, FILENAME, decline, 0, TYPE_ERROR, VU_ERROR_TYPE_DEL_A+(*b).first+VU_ERROR_TYPE_DEL_B);
				result.push(decl);
			}
		}
	}
}

};
#endif