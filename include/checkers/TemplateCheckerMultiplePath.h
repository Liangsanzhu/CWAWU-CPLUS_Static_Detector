#ifndef TEMPLATECHECKER_P_H
#define TEMPLATECHECKER_P_H

#include "framework/ASTManager.h"
#include "checkers/def_use_pro.h"

class TemplateCheckerMultiplePath : public ASTManager {
public:
  TemplateCheckerMultiplePath(std::vector<std::string> &ASTFiles, Config &configure)
      : ASTManager(ASTFiles, configure) {}

  void check();

private:
};

#endif // TEMPLATECHECKER_H
