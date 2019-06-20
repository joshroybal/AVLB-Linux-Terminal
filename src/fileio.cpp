#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "fileio.hpp"
#include "csv.hpp"

// file  and console I/O function definitions
void initialize(int* field_choices, int n)
{
   for (int i = 0; i < n; i++) field_choices[i] = 0;
}

// int function fills selection array with user choices
void getChoices(int* field_choices, int n)
{
   int no_choices = 0;
   std::string tmp;
   std::stringstream ss;

   getline(std::cin, tmp);
   ss << tmp;
   while (ss.good() && no_choices < n) {
      ss >> field_choices[no_choices++];
      ss.ignore();   // this works
   }
}

// so's we's know's we's can's do's it's
void writeTable(std::ofstream& sequential, const std::vector<std::string>& relation)
{
   unsigned int maxima[NOFLDS];
   std::vector<std::string> flds;
   std::vector< std::vector<std::string> > table;
   std::string stars(75, '*');

   for (unsigned int i = 0; i < NOFLDS; i++) maxima[i] = 0;
   // get the header
   getFields(flds, relation[0]);
   table.push_back(flds);
   flds.clear();
   // but it doesn't contribute to computing maximum field widths
   for (unsigned i = 1; i < relation.size(); i++) {
      getFields(flds, relation[i]);
      table.push_back(flds);
      // compute field widths
      for (unsigned j = 0; j < flds.size(); j++)
         if (flds[j].length() > maxima[j]) maxima[j] = flds[j].length();
      flds.clear();
   }
   std::stringstream hdr;
   for (unsigned j = 0; j < table[0].size(); j++) 
      hdr << ' ' << std::setw(maxima[j]) << std::left << table[0][j].substr(0, maxima[j]) << ' ';
   sequential << hdr.str() << std::endl;
   for (unsigned i = 1; i < relation.size(); i++) {
      sequential << ' ' << std::setw(maxima[0]) << std::right << std::setfill('0') << table[i][0] << ' ';
      sequential << std::setfill(' ');
      for (unsigned j = 1; j < table[i].size(); j++) 
         sequential << ' ' << std::setw(maxima[j]) << std::left << table[i][j] << ' ';
      sequential << std::endl;
   }
}
