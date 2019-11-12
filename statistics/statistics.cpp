#include <iostream>
#include "../src/phylotree/tree.h"
#include "../src/newick/parser.h"
#include "../src/phylotree/sparse_table.h"
#include "../src/phylotree/euler_tree.h"
#include <glpk.h>
#include <dirent.h>
#include <sstream>
#include <cstring>
#include <fstream>

int num_incompatible_paths(string tree1, string tree2, int spr, bool verbose);
int num_incompatible_triples(string tree1, string tree2, int spr, bool verbose);

#include <regex>

std::string exec(const char *cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
  if (!pipe) throw std::runtime_error("popen() failed!");
  while (!feof(pipe.get())) {
    if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
      result += buffer.data();
  }
  return result;
}

int main() {
  std::vector<int> spr_num;
  std::vector<int> leaves_num;
  std::vector<int> ips_num;
  std::vector<int> its_num;
  std::string dirname = "../../tests/trees/";

  char diry[1024];
  std::strcpy(diry, dirname.c_str());
  std::string rsprcmd = "rspr -fpt < ../../tests/trees/";
  std::string filename = "tree1_20_1.txt";
  std::string sedpipecmd = "| tail -n 1 |sed 's/[^0-9]*//g'";


  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(diry)) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      std:
      string tempname = dirname;
      tempname += "/";
      tempname += ent->d_name;
      if (tempname.find(".txt") != string::npos) {
        std::ifstream infile(tempname);

        std::string tree1;
        getline(infile, tree1);
        std::string tree2;
        getline(infile, tree2);

        lexer l1(tree1);
        parser p1(l1);
        auto t1 = p1.parse();
        lexer l2(tree2);
        parser p2(l2);
        auto t2 = p2.parse();

        std::string rsprcmd = "rspr -fpt < ../../tests/trees/";
        std::string sedpipecmd = "| tail -n 1 |sed 's/[^0-9]*//g'";
        std::string cmd = rsprcmd + ent->d_name + sedpipecmd;
        auto s = exec(cmd.c_str());
        auto dist = std::stoi(s);

        spr_num.push_back(dist);
        leaves_num.push_back(t1->leaves().size());
        ips_num.push_back(num_incompatible_paths(tree1, tree2, 0, false));
        its_num.push_back(num_incompatible_triples(tree1, tree2, 0, false));

      }
    }
    closedir(dir);
  } else {
    perror("");
  }

  std::cout << "lea, ips, its, spr" << std::endl;//, spr" << std::endl;
  for (int i = 0; i < leaves_num.size(); i++) {

    std::cout << leaves_num.at(i) << ", ";
    std::cout << ips_num.at(i) << ", ";
    std::cout << its_num.at(i) << ", ";
    std::cout << spr_num.at(i);
    std::cout << std::endl;
  }

  std::cout << std::endl;

}

int num_incompatible_paths(string s, string t, int spr, bool verbose) {
  lexer lex1(s);
  lexer lex2(t);
  parser parser1(lex1);
  parser parser2(lex2);
  auto t1 = parser1.parse();
  auto t2 = parser2.parse();
  auto tour1 = euler_tree(*t1);
  auto tour2 = euler_tree(*t2);

  std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> ips;

  tour1.incompatible_paths(*t2, ips);
  return ips.size();
}

int num_incompatible_triples(string s, string t, int spr, bool verbose) {
  lexer lex1(s);
  lexer lex2(t);
  parser parser1(lex1);
  parser parser2(lex2);
  auto t1 = parser1.parse();
  auto t2 = parser2.parse();
  auto tour1 = euler_tree(*t1);
  auto tour2 = euler_tree(*t2);

  std::vector<std::pair<int, std::pair<int, int>>> its;
  tour1.incompatible_triples(*t2, its);
  return its.size();
}