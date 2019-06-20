// AVL class test driver program
// imitates some of the functionality of a relational database system
// Revision 7-15-2018
// Revision 6-17-2019 
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <ctime>
#include "avl.hpp"
#include "csv.hpp"
#include "fileio.hpp"
#include "screenio.hpp"

enum { dummy, first, last, company, address, city, county, state, zip, phone,
   cell, email, url };

int input(int[], int[], int[], std::string&, const std::string&, int);
void process(char*, std::vector<std::string>&, int[], int[], int[], const std::string&, int, int);
void output(const std::vector<std::string>&);

int main(int argc, char *argv[])
{
   const int RECSIZ = 512;
   const int NOFLDS = 12;
   const std::string header("first name,last name,company,address,city,county,state,zip code,phone no.,cell no.,e-mail,web page");
   int select_fields[NOFLDS], find_fields[NOFLDS], order_fields[NOFLDS];
   char buf[RECSIZ];
   std::string field, target;
   std::vector<std::string> relation;
   std::ifstream direct;

   if ( argc < 2 ) {
      std::cerr << "Usage: " << argv[0] << " infile" << std::endl;
      return 1;
   }

   ClearScreen();
   std::string infile(argv[1]);
   direct.open(argv[1], std::ios::in | std::ios::binary);
   unsigned int n = 0;
   unsigned int factor = 2;
   while ( direct.read(buf, RECSIZ) ) {
      std::string record(buf);
      if (n % factor == 0) {
         std::cout << " record no. " << n+1;
         screenRecord(record);
         factor *= 2;
         // this is a screen delay loop
         float t1 = float(clock()) / CLOCKS_PER_SEC;
         while (true) {
            float t = float(clock()) / CLOCKS_PER_SEC;
            if (t - t1 > 0.25) break;
         }
         ClearScreen();
      }
      n++;
   }
   direct.close();
   std::cout << " no. of records = " << n << std::endl;
   std::string reply("yes");
   while (reply[0] == 'y' || reply[0] == 'Y') {
      if (input(select_fields, find_fields, order_fields, target, header, NOFLDS)) {
         std::cout << " Copyright (c) 2016-2019 Josh Roybal" << std::endl;
         return 0;
      }
      // the header line for the select table may as well be constructed now by the computer
      // std::ostringstream table_header("rec. no.");
      std::ostringstream table_header;
      table_header << "rec. no.";
      int idx = 0;
      while (select_fields[idx]) {
         table_header << ',';
         if ( select_fields[idx] == first ) table_header << "first name";
         if ( select_fields[idx] == last ) table_header << "last name";
         if ( select_fields[idx] == company ) table_header << "company";
         if ( select_fields[idx] == address ) table_header << "address";
         if ( select_fields[idx] == city ) table_header << "city";
         if ( select_fields[idx] == county ) table_header << "county";
         if ( select_fields[idx] == state ) table_header << "state";
         if ( select_fields[idx] == zip ) table_header << "zip code";
         if ( select_fields[idx] == phone ) table_header << "phone no.";
         if ( select_fields[idx] == cell ) table_header << "cell no.";
         if ( select_fields[idx] == email ) table_header << "e-mail";
         if ( select_fields[idx] == url ) table_header << "web page";
         ++idx;
      }
      relation.push_back(table_header.str());
      // process data based on user input
      process(argv[1], relation, select_fields, find_fields, order_fields, target, RECSIZ, NOFLDS);
      // ClearScreen();
      screenReport(relation);
      std::cout << ' ' << relation.size()-1 << " records in relation" << std::endl;
      std::cout << " output reports to files? (y/n) ";
      getline(std::cin, reply);
      if (reply[0] == 'y' || reply[0] == 'Y') {
         // ClearScreen();
         std::cout << " writing reports" << std::endl;
         output(relation);
      }
      relation.clear();
      std::cout << " continue processing? (y/n) ";
      getline(std::cin, reply);
   }
   std::cout << " Copyright \u00a9 Josh Roybal 2016-2018" << std::endl;
   return 0;
}

int input(int select_fields[], int find_fields[], int order_fields[], 
      std::string& target, const std::string& header, int NOFLDS)
{
   ClearScreen();
   displayMenu(header);
   std::cout << " select fields : ";
   initialize(select_fields, NOFLDS);   // zeroes out choice array
   getChoices(select_fields, NOFLDS);   // gets 'select' field choices from user 
   if (select_fields[0] == 0) {
      std::cout << " thank-you and good-bye" << std::endl;
      return 1;
   }
   std::cout << " search fields : ";
   initialize(find_fields, NOFLDS);    // zeroes out choice array
   getChoices(find_fields, NOFLDS);    // gets 'where domain=?' field choices from user
   std::cout << " enter search string : ";
   getline(std::cin, target);
   std::cout << " order fields : ";
   initialize(order_fields, NOFLDS);   // zeroes out choice array
   getChoices(order_fields, NOFLDS);   // gets 'order by' field choices from user
   return 0;
}

void process(char* filename,std::vector<std::string>& relation, int select_fields[], 
      int find_fields[], int order_fields[], const std::string& target, 
      int RECSIZ, int NOFLDS)
{
   char buf[RECSIZ];
   char fldbuf[FLDLEN];
   char keybuf[RECSIZ];
   std::string field;
   AVL* BaseTree = new AVL();
   std::ifstream direct;

   direct.open(filename, std::ios::in | std::ios::binary);
   // read in the search keys
   int n = 0;
   float t1 = float(clock()) / CLOCKS_PER_SEC;
   while ( direct.read(buf, RECSIZ) ) {
      memset(keybuf, '\0', RECSIZ);
      int idx = 0;
      while (find_fields[idx] != 0) {
         if (idx) strcat(keybuf, ",");
         getField(fldbuf, buf, find_fields[idx++]);
         strcat(keybuf, fldbuf);
      }
      if ( std::string(keybuf) == target ) {
         int idx = 0;
         while (order_fields[idx] != 0) {
            getField(fldbuf, buf, order_fields[idx++]);
            strcat(keybuf, fldbuf);
         }
         BaseTree->insert( std::string(keybuf), n);
      }
      memset(buf, '\0', RECSIZ);
      ++n;
   }
   direct.close();
   float t2 = float(clock()) / CLOCKS_PER_SEC;
   std::cout << std::setprecision(3);
   std::cout << std::fixed;
   std::cout << " no. of records = " << n << std::endl; 
   std::cout << ' ' << BaseTree->size() << " keys read into binary search tree in " << t2 - t1 << " seconds." << std::endl;
   std::cout << " height of tree = " << BaseTree->height() << std::endl;
   t1 = float(clock()) / CLOCKS_PER_SEC;
   // std::vector<int> matchIdx = BaseTree->search(target);
   std::vector<int> matchIdx;
   BaseTree->loadIdxVector(matchIdx);
   t2 = float(clock()) / CLOCKS_PER_SEC;
   std::cout << " AVL tree loaded into index in " << t2 - t1 << " seconds." << std::endl;
   t1 = float(clock()) / CLOCKS_PER_SEC;
   delete BaseTree;  // we don't need it any longer - so we delete it
   t2 = float(clock()) / CLOCKS_PER_SEC;
   std::cout << " AVL tree deleted in " << t2 - t1 << " seconds." << std::endl;
   std::cout << ' ' << matchIdx.size() << " matches found." << std::endl;
   // grab the selected fields to write to the relation
   // our final relation is written to a CSV (comma delimited) format file
   t1 = float(clock()) / CLOCKS_PER_SEC;

   direct.open(filename, std::ios::in | std::ios::binary);
   for (unsigned i = 0; i < matchIdx.size(); i++) {
      direct.seekg(matchIdx[i] * RECSIZ, std::ios::beg);
      direct.read(buf, RECSIZ);
      // std::string record(buf);
      memset(keybuf, '\0', RECSIZ);
      unsigned idx = 0;
      // construct the key from the fields the user wants to extract from the record
      std::stringstream ss;
      ss << matchIdx[i];
      strcat(keybuf, ss.str().c_str());
      while (select_fields[idx] != 0) {
         strcat(keybuf, ",");
         getField(fldbuf, buf, select_fields[idx++]);
         if ( containsComma( std::string(fldbuf) ) ) {
            strcat(keybuf, "\"");
            strcat(keybuf, fldbuf);
            strcat(keybuf, "\"");
         }
         else
            strcat(keybuf, fldbuf);
      }
      relation.push_back( std::string(keybuf) );
   }
   direct.close();   // all good things must come to an end
   t2 = float(clock()) / CLOCKS_PER_SEC;
   std::cout << " relation fields retrieved in " << t2 - t1 << " seconds." << std::endl;
}

void output(const std::vector<std::string>& relation)
{
   float t1 = float(clock()) / CLOCKS_PER_SEC;
   std::ofstream sequential("report.csv", std::ios::out);
   for (unsigned int i = 0; i < relation.size(); i++) sequential << relation[i] << std::endl;
   sequential.close();
   float t2 = float(clock()) / CLOCKS_PER_SEC;
   std::cout << " output written to file report.csv in " << t2 - t1 << " seconds." << std::endl;
   t1 = float(clock()) / CLOCKS_PER_SEC;
   sequential.open("report.txt", std::ios::out);
   writeTable(sequential, relation);
   sequential.close();
   t2 = float(clock()) / CLOCKS_PER_SEC;
   std::cout << " output written to file report.txt in " << t2 - t1 << " seconds." << std::endl;
}
