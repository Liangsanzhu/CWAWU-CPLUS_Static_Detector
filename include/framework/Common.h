#ifndef BASE_COMMON_H
#define BASE_COMMON_H

#include <clang/Analysis/CFG.h>
#include <clang/Frontend/ASTUnit.h>
#include <string>
#include <vector>
using namespace clang;

std::vector<std::string> initialize(std::string astList);

namespace common {
std::unique_ptr<ASTUnit> loadFromASTFile(std::string AST);
std::string getFullName(FunctionDecl *FD);
std::unique_ptr<CFG> buildCFG(Decl *);
} // namespace common

#endif
