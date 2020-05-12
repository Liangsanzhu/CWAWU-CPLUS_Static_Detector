#ifndef DEF_USE_H
#define DEF_USE_H

#include <string>
#include <string.h>
#include <list>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include<iostream>
using namespace std;

class def_use {
public:

  def_use() {
      ;
  }

  void add(int def, int use) {
      std::pair<int, int> pr(def, use);
      du.push_back(pr);
  }

  void output_all() {
    for (int i = 0; i < du.size(); ++i) {
      std::cout << du[i].first << "  ";
      std::cout << du[i].second << std::endl;
    }
  }

  std::vector<std::pair<int, int>> get_du() {
    return du;
  }

private:
  std::vector<std::pair<int, int>> du;
  
};

class defuse_node {
public:
  
  defuse_node() {
      ;
  }

  void add_var(std::string var) {
    du_node[var] = def_use();
  }

  void add_du(std::string var, int def, int use) {
    du_node[var].add(def, use);
  }

  void output_node() {
    for (auto b = du_node.begin(); b != du_node.end(); ++b) {
      std::cout << (*b).first << "  ";
      (*b).second.output_all();
    }
  }

  std::map<std::string, def_use> get_node() {
    return du_node;
  }

private:
  std::map<std::string, def_use> du_node;
};

#endif //DEF_USE_H