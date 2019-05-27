#include "csv.hpp"

// scan header for no. of fields that will need to be processed
int getNoFlds(const std::string& hdr)
{
   std::vector<std::string> flds;
   getFields(flds, hdr);
   return flds.size();
}


void getFields(std::vector<std::string>& fields, const std::string& record)
{
   unsigned int i = 0, j = 0;

   while (i < record.length()) {
      if ( record[i] == '\r' || record[i] == '\n') return;
      fields.push_back("\0");
      while (record[i] != ',' && i < record.length()) {
         if ( record[i] == '\r' || record[i] == '\n') return;
         if (record[i] != '\"') {
            fields[j] += record[i++];
         } else {   // we're escaped right?
            i++;  // step through the quote
            while (record[i] != '\"') 
               fields[j] += record[i++];
            i++;  // step through the quote
         }
      }
      j++;
      i++;
   }
}  

void getField(std::string& field, const std::string& record, int n)
{
   unsigned int i = 0, j = 0;

   field.clear();
   while (i < record.length()) {
      while (record[i] != ',' && i < record.length())
         if (record[i] != '\"')
            if (j + 1 == n)
               field += record[i++];
            else
               i++;
         else {
            i++;
            while (record[i] != '\"') {
               if (j + 1 == n)
                  field += record[i++];
               else
                  i++;
            }
            i++;
         }
      if (j + 1 == n) return;
      i++;
      j++;
   }
   field.clear();
}

bool containsComma(const std::string& str)
{
   unsigned int i = 0, n = str.length();
   while (str[i++] != ',' && i < n);
   if (i == n)
      return false;
   else
      return true;
}

