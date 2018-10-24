

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

int safestoi(std::string& s, int defvalue=0, int povinne=0);
long int safestol(std::string& s, long int defvalue=0, int povinne=0);
unsigned long int safestoul(std::string& s, unsigned long int defvalue=0, int povinne=0);
double safestod(std::string& s, double defvalue=0, int povinne=0);
void split_string(const std::string& s, char c, std::vector<std::string>& v) ;


