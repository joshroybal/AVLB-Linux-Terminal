#ifndef CSV_HPP
#define CSV_HPP

#include <string>
#include <vector>

// global constants
const int RECSIZ = 512;
const int NOFLDS = 12;
const int FLDLEN = 80;

int getNoFlds(const std::string&);
void getFields(std::vector<std::string>&, const std::string&);
void getField(std::string&, const std::string&, int);
void getField(char*, const char*, int);
std::string getField(const char*, int);
bool containsComma(const std::string&);

#endif
