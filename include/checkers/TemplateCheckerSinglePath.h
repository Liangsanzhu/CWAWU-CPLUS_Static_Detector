#ifndef TEMPLATECHECKER_H
#define TEMPLATECHECKER_H

#include "framework/ASTManager.h"

class TemplateCheckerSinglePath : public ASTManager {
public:
  TemplateCheckerSinglePath(std::vector<std::string> &ASTFiles, Config &configure)
      : ASTManager(ASTFiles, configure) {}

  void check();

private:
};

#endif // TEMPLATECHECKER_H
