#ifndef SCREEN_IO_HPP
#define SCREEN_IO_HPP

#include <vector>

// global function prototypes
void displayMenu(const std::string&);
void screenRecord(const std::string&);
void displayLine(const std::string&);
void ClearScreen();
void screenReport(const std::vector<std::string>& relation);

#endif
