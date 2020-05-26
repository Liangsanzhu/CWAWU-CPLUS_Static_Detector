#ifndef ALL_DETECTOR_H
#define ALL_DETECTOR_H

#include <map>
#include <vector>
#include "framework/ASTManager.h"
#include "checkers/def_use.h"


class Detector {
public:
  Detector() {
      ;
  }
  void detector();

private:
};

#endif //ALL_DETECTOR