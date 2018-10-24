

/*  UTILITY FOR CONVERTING STRINGS TO NUMBERS
 * 
 *  CHECKS SIZE, IF THE STRING IS BEEN READ COMPLETE.
 * 
 *  EMPTY STRING LEADS TO DEFAULT VALUE
 * 
 *  IN CASE THAT FORMAT OF THE NUMBER DOESNOT FIT, THE EXCEPTION STD::invalid_argument IS RAISED
 * 
 * TRAILING SPACES AFTER THE NUM WILL CAUSE EXCEPTION.
 * 
 * IF povinne==1, NO DEFAULT VALUE IS ISED, EXCEPTION WILL OCCUR
 * 
 *  15. 10. 2018  Jakub Solc
 * */

#include<string>

int safestoi(std::string& s, int defvalue=0, int povinne=0)
{
    int a;
    size_t idx=0;
    try {
        a = std::stoi(s, &idx);
        if (idx != s.size()) 
        {std::cout << "Error in number " << s << "\n"; throw std::invalid_argument("stoi NumError");}
    }
    catch(std::invalid_argument &ia) {
        if (s=="" && !povinne) return defvalue;
        else throw ia;
    }
    return a;
}
long int safestol(std::string& s, long int defvalue=0, int povinne=0)
{
    long int a;
    size_t idx=0;
    try {
        a = std::stol(s, &idx);
        if (idx != s.size()) 
        {std::cout << "Error in number " << s << "\n"; throw std::invalid_argument("stol NumError");}
    }
    catch(std::invalid_argument &ia) {
        if (s=="" && !povinne) return defvalue;
        else throw ia;
    }
    return a;
}
unsigned long int safestoul(std::string& s, unsigned long int defvalue=0, int povinne=0)
{
    unsigned long int a;
    size_t idx=0;
    try {
        a = std::stoul(s, &idx);
        if (idx != s.size()) 
        {std::cout << "Error in number " << s << "\n"; throw std::invalid_argument("stoul NumError");}
    }
    catch(std::invalid_argument &ia) {
        if (s=="" && !povinne) return defvalue;
        else throw ia;
    }
    return a;
}
double safestod(std::string& s, double defvalue=0, int povinne=0)
{
    double a;
    size_t idx=0;
    try {
        a = std::stod(s, &idx);
        if (idx != s.size()) 
        {std::cout << "Error in number " << s << "\n"; throw std::invalid_argument("stod NumError");}
    }
    catch(std::invalid_argument &ia) {
        if (s=="" && !povinne) return defvalue;
        else throw ia;
    }
    return a;
}


void split_string(const std::string& s, char c, std::vector<std::string>& v) 
{
   int i = 0;
   int j = 0;
   int len = s.length();
   while (1) 
   {
      j = s.find_first_of(c,i);
      if (j == std::string::npos)       // posledni, vyzkouset <0??
      {
         v.push_back(s.substr(i, len));  // substr testuje konec, efektivni pocet znaku je len-i
         break;
      }
      v.push_back(s.substr(i, j-i));
      i = ++j;
   }
}

