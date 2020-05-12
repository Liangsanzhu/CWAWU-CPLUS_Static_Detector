#include "framework/ASTManager.h"

ASTManager::ASTManager(std::vector<std::string> &ASTFiles, Config &configure)
    : config(configure) {
  for (std::string ASTF : ASTFiles) {
    std::unique_ptr<ASTUnit> AU = common::loadFromASTFile(ASTF);
    ASTTraverse ASTT;
    ASTT.HandleTranslationUnit(AU->getASTContext());
    ASTRs[ASTF] = std::move(ASTT.ASTR);
    ASTUs[ASTF] = move(AU);
  }
}
