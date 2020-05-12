#ifndef TEMPLATECHECKER_H
#define TEMPLATECHECKER_H

#include "framework/ASTManager.h"

class TemplateChecker : public ASTManager {
public:
  TemplateChecker(std::vector<std::string> &ASTFiles, Config &configure)
      : ASTManager(ASTFiles, configure) {}

  void check();

private:
};

#endif // TEMPLATECHECKER_H