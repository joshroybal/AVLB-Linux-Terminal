#ifndef FILE_IO_HPP
#define FILE_IO_HPP

#include <vector>

// global function prototypes
void initialize(int*, int);
void getChoices(int*, int);
void writeTable(std::fstream&, const std::vector<std::string>&);

#endif
