#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "screenio.hpp"
#include "csv.hpp"

// display subroutine
void displayMenu(const std::string& header)
{
   int width = 75;
   char c = '*';
   std::string stars(78, '*');
   std::vector<std::string> flds;
   getFields(flds, header);
   int nf = flds.size();

   std::cout << " main menu " << std::endl;
   displayLine(stars);
   displayLine(" 0 - terminate processing");
   for (int i = 0; i < nf; i++) {
      std::ostringstream ss;
      ss << i + 1;
      displayLine(" " + ss.str() + " - " + flds[i]);
   }
   displayLine(stars);
}

void screenRecord(const std::string& csvline)
{
   int width = 78;
   char c = '*';
   std::string stars;
   std::vector<std::string> flds;

   stars.assign(width, c);
   getFields(flds, csvline);
   int nf = flds.size();
   std::cout << std::endl;
   displayLine(stars);
   for (int i = 0; i < nf; i++) 
      displayLine(" " + flds[i]);
   displayLine(stars);
   std::cout << std::endl;
}

// provide the string with a leading blank yourself if you insist
void displayLine(const std::string& str)
{
   std::cout << "\033[37;44;1m" << "*";
   std::cout << std::left << std::setw(78) << str;
   std::cout << "*" << "\033[0m" << std::endl;
}

void screenReport(const std::vector<std::string>& relation)
{
   int maxima[NOFLDS];
   std::vector<std::string> flds;
   std::vector< std::vector<std::string> > table;
   std::string stars(78, '*');

   for (int i = 0; i < NOFLDS; i++) maxima[i] = 0;

   // get the header but
   getFields(flds, relation[0]);
   table.push_back(flds);
   flds.clear();
   // but it doesn't contribute to computing maximum field widths
   for (int i = 1; i < relation.size(); i++) {
      getFields(flds, relation[i]);
      table.push_back(flds);
      // compute field widths
      for (int j = 0; j < flds.size(); j++)
         if (flds[j].length() > maxima[j]) maxima[j] = flds[j].length();
      flds.clear();
   }
   displayLine(stars);  // we're not here unless there're matches
   std::stringstream hdr;
   for (int j = 0; j < table[0].size(); j++) 
      hdr << ' ' << std::setw(maxima[j]) << std::left << table[0][j].substr(0, maxima[j]) << ' ';
   displayLine(hdr.str().substr(0, 77));
   int last = std::min((int)relation.size(), 8);
   int first = std::max(8, (int)relation.size() - 8);
   for (int i = 1; i < last; i++) {
      std::stringstream ss;
      ss << ' ' << std::setw(maxima[0]) << std::setfill('0') << table[i][0] << ' ';
      ss << std::setfill(' ');
      for (int j = 1; j < table[i].size(); j++) 
         ss << ' ' << std::left << std::setw(maxima[j]) << table[i][j] << ' ';
      displayLine(ss.str().substr(0, 78));
   }
   if (first > last && first < relation.size()) std::cout << " . . ." << std::endl;
   for (int i = first; i < relation.size(); i++) {
      std::stringstream ss;
      ss << ' ' << std::setw(maxima[0]) << std::setfill('0') << table[i][0] << ' ';
      ss << std::setfill(' ');
      for (int j = 1; j < table[i].size(); j++) 
         ss << ' ' << std::left << std::setw(maxima[j]) << table[i][j] << ' ';
      displayLine(ss.str().substr(0, 78));
   }
   displayLine(stars);  // we're not here unless there're matches
}

void ClearScreen()
{
    std::cout << "\E[H\E[2J";
}
