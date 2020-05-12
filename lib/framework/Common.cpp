#include "framework/Common.h"
#include <clang/Frontend/CompilerInstance.h>
#include <fstream>
#include <iostream>
namespace common {
/**
 * load an ASTUnit from ast file.
 * ASTF : the name of the ast file.
 */
std::unique_ptr<ASTUnit> loadFromASTFile(std::string ASTF) {

  FileSystemOptions FileSystemOpts;
  IntrusiveRefCntPtr<DiagnosticsEngine> Diags =
      CompilerInstance::createDiagnostics(new DiagnosticOptions());
  std::shared_ptr<PCHContainerOperations> PCHContainerOps;
  PCHContainerOps = std::make_shared<PCHContainerOperations>();
  return std::unique_ptr<ASTUnit>(
      ASTUnit::LoadFromASTFile(ASTF, PCHContainerOps->getRawReader(),
                               ASTUnit::LoadEverything, Diags, FileSystemOpts));
}

std::string getParams(FunctionDecl *FD) {
  std::string params = "";
  for (auto param = FD->param_begin(); param != FD->param_end(); param++) {
    params = params + (*param)->getOriginalType().getAsString() + "  ";
  }
  return params;
}

std::string getFullName(FunctionDecl *FD) {
  std::string name = FD->getQualifiedNameAsString();

  name = name + "  " + getParams(FD);
  return name;
}

/// Builds a CFG from an AST.
std::unique_ptr<CFG> buildCFG(Decl *D) {
  return std::unique_ptr<CFG>(
      CFG::buildCFG(D, D->getBody(), &D->getASTContext(), CFG::BuildOptions()));
}
} // namespace common

std::string trim(std::string s) {
  std::string result = s;
  result.erase(0, result.find_first_not_of(" \t\r\n"));
  result.erase(result.find_last_not_of(" \t\r\n") + 1);
  return result;
}

std::vector<std::string> initialize(std::string astList) {
  std::vector<std::string> astFiles;

  std::ifstream fin(astList);
  std::string line;
  while (getline(fin, line)) {
    line = trim(line);
    if (line == "")
      continue;
    std::string fileName = line;
    astFiles.push_back(fileName);
  }
  fin.close();

  return astFiles;
}
