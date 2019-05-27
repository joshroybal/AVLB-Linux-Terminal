#ifndef CSV_HPP
#define CSV_HPP

#include <string>
#include <vector>

// global constants
const int RECSIZ = 256;
const int NOFLDS = 20;
const int FLDLEN = 50;

int getNoFlds(const std::string&);
void getFields(std::vector<std::string>&, const std::string&);
void getField(std::string&, const std::string&, int);
bool containsComma(const std::string&);

#endif
