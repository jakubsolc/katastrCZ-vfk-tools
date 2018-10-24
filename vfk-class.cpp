

// // g++ my_xpath_select.cpp pugixml.cpp -o z
// // ./z -quiet -addpc 284 -addpc "13/1" -addpc "st. 6/1" -expoo -status > exkuk.m
// // ./z -quiet -ku 618225.xml -addlist "st. 31/1" "st. 32" 526 525 456/8 "st. 28/2" 456/11 528 "st. 65" 456/5 456/9 "st. 33" "st. 34" 11/3 11/4 -expoo > plancach.m


//  VFK importer pro SOBR, ?SPOl, ...

//  potrebuji nacist SOBR, SPOL, SBP, HP, OP

//  hlavni objekty:  tabulka obrazu bodu s pevnym poradim sloupcu


/*  NEUTOPIT SE VE SNAZE O ROBUSTNOST. PROMENNE PORADI V TABULKACH A PROMENNE FORMATY SLOUPCU JSOU AKADEMICKY PROBLEM,
 *  KTERY MOZNA ANI NEEXISTUJE. STACI OTESTOVAT SHODNOST FORMATOVEHO RADKU.
 */

//#include "pugixml.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>  //setw
#include <string>
#include <vector>
#include <map>

#include "vfk-headers.h"

#include "safestring.cpp"

//#define     DEBPR(A,B)              std::cout << A << B << '\n';

/*
 * 
 * 
 * 
 * 
 * 
 * 
 * 
*/

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

const char TSBP::BSTRING[] = {"&BSBP;ID N30;STAV_DAT N2;DATUM_VZNIKU D;DATUM_ZANIKU D;PRIZNAK_KONTEXTU N1;RIZENI_ID_VZNIKU N30;RIZENI_ID_ZANIKU N30;BP_ID N30;\
                                      PORADOVE_CISLO_BODU N38;OB_ID N30;HP_ID N30;DPM_ID N30;PARAMETRY_SPOJENI T100;ZVB_ID N30"}; 

const char TSOBR::BSTRING[] = {"&BSOBR;ID N30;STAV_DAT N2;KATUZE_KOD N6;CISLO_ZPMZ N5;CISLO_TL N4;CISLO_BODU N12;UPLNE_CISLO N12;SOURADNICE_Y N10.2;SOURADNICE_X N10.2;KODCHB_KOD N2"};
const char TSPOL::BSTRING[] = {"&BSPOL;ID N30;STAV_DAT N2;KATUZE_KOD N6;CISLO_ZPMZ N5;CISLO_TL N4;CISLO_BODU N12;UPLNE_CISLO N12;SOURADNICE_Y N10.2;SOURADNICE_X N10.2;KODCHB_KOD N2;KATUZE_KOD_MER N6;CISLO_ZPMZ_MER N5"};
    
const char TOP::BSTRING[] = {"&BOP;ID N30;STAV_DAT N2;DATUM_VZNIKU D;DATUM_ZANIKU D;PRIZNAK_KONTEXTU N1;RIZENI_ID_VZNIKU N30;RIZENI_ID_ZANIKU N30;TYPPPD_KOD N10;\
                                SOURADNICE_Y N10.2;SOURADNICE_X N10.2;TEXT T255;VELIKOST N10.2;UHEL N10.4;PAR_ID N30;OPAR_TYPE T10;VZTAZNY_BOD N2"};
const char THP::BSTRING[] = {"&BHP;ID N30;STAV_DAT N2;DATUM_VZNIKU D;DATUM_ZANIKU D;PRIZNAK_KONTEXTU N1;RIZENI_ID_VZNIKU N30;RIZENI_ID_ZANIKU N30;TYPPPD_KOD N10;PAR_ID_1 N30;PAR_ID_2 N30"};


void LSOBR::pr()
{
    const int wdl  = 16;
    const int wds  = 8;
    
        std::cout << std::fixed << std::setprecision(2) << std::right;
        std::cout << std::setw(wdl) <<  ID;
        std::cout << std::setw(wds) <<  STAV_DAT;
        std::cout << std::setw(wds) <<  KATUZE_KOD;
        std::cout << std::setw(wds) <<  CISLO_ZPMZ;
        std::cout << std::setw(wds) <<  CISLO_TL;
        std::cout << std::setw(wdl) <<  CISLO_BODU;
        std::cout << std::setw(wdl) <<  UPLNE_CISLO;
        std::cout << std::setw(wdl) <<  SOURADNICE_Y;
        std::cout << std::setw(wdl) <<  SOURADNICE_X;
        std::cout << std::setw(wds) <<  KOD_CHB;
        std::cout << "\n";
}

void LSPOL::pr()
{
    const int wdl  = 16;
    const int wds  = 8;
    
        std::cout << std::fixed << std::setprecision(2) << std::right;
        std::cout << std::setw(wdl) <<  ID;
        std::cout << std::setw(wds) <<  STAV_DAT;
        std::cout << std::setw(wds) <<  KATUZE_KOD;
        std::cout << std::setw(wds) <<  CISLO_ZPMZ;
        std::cout << std::setw(wds) <<  CISLO_TL;
        std::cout << std::setw(wdl) <<  CISLO_BODU;
        std::cout << std::setw(wdl) <<  UPLNE_CISLO;
        std::cout << std::setw(wdl) <<  SOURADNICE_Y;
        std::cout << std::setw(wdl) <<  SOURADNICE_X;
        std::cout << std::setw(wds) <<  KOD_CHB;
        std::cout << std::setw(wds) <<  KATUZE_KOD_MER;
        std::cout << std::setw(wds) <<  CISLO_ZPMZ_MER;
        std::cout << "\n";
}

void LSBP::pr()
{
    const int wdl  = 16;
    const int wds  = 8;
    
        std::cout << std::fixed << std::setprecision(2) << std::right;
        std::cout << std::setw(wdl) <<  ID;
        std::cout << std::setw(wds) <<  STAV_DAT;
        std::cout << std::setw(wds) <<  DATUM_VZNIKU;
        std::cout << std::setw(wds) <<  DATUM_ZANIKU;
        std::cout << std::setw(wds) <<  PRIZNAK_KONTEXTU;
        std::cout << std::setw(wds) <<  RIZENI_ID_VZNIKU;
        std::cout << std::setw(wds) <<  RIZENI_ID_ZANIKU;
        std::cout << std::setw(wds) <<  BP_ID;
        std::cout << std::setw(wds) <<  PORADOVE_CISLO_BODU;
        std::cout << std::setw(wds) <<  OB_ID;
        std::cout << std::setw(wds) <<  HP_ID;
        std::cout << std::setw(wds) <<  DPM_ID;
        std::cout << std::setw(wds) <<  PARAMETRY_SPOJENI;
        std::cout << std::setw(wds) <<  ZVB_ID;
        std::cout << "\n";
}

void LHP::pr()
{
    const int wdl  = 16;
    const int wds  = 8;
    
        std::cout << std::fixed << std::setprecision(2) << std::right;
        std::cout << std::setw(wdl) <<  ID;
        std::cout << std::setw(wds) <<  STAV_DAT;
        std::cout << std::setw(wds) <<  DATUM_VZNIKU;
        std::cout << std::setw(wds) <<  DATUM_ZANIKU;
        std::cout << std::setw(wds) <<  PRIZNAK_KONTEXTU;
        std::cout << std::setw(wds) <<  RIZENI_ID_VZNIKU;
        std::cout << std::setw(wds) <<  RIZENI_ID_ZANIKU;
        std::cout << std::setw(wds) <<  TYPPPD_KOD;
        std::cout << std::setw(wds) <<  PAR_ID_1;
        std::cout << std::setw(wds) <<  PAR_ID_2;
        std::cout << "\n";
}


void LOP::pr()
{
    const int wdl  = 16;
    const int wds  = 8;
    
        std::cout << std::fixed << std::setprecision(2) << std::right;
        std::cout << std::setw(wdl) <<  ID;
        std::cout << std::setw(wds) <<  STAV_DAT;
        std::cout << std::setw(wds) <<  DATUM_VZNIKU;
        std::cout << std::setw(wds) <<  DATUM_ZANIKU;
        std::cout << std::setw(wds) <<  PRIZNAK_KONTEXTU;
        std::cout << std::setw(wds) <<  RIZENI_ID_VZNIKU;
        std::cout << std::setw(wds) <<  RIZENI_ID_ZANIKU;
        std::cout << std::setw(wds) <<  TYPPPD_KOD;
        std::cout << std::setw(wdl) <<  SOURADNICE_Y;
        std::cout << std::setw(wdl) <<  SOURADNICE_X;
        std::cout << std::setw(wds) <<  TEXT;
        std::cout << std::setw(wds) <<  VELIKOST;
        std::cout << std::setw(wds) <<  UHEL;
        std::cout << std::setw(wds) <<  PAR_ID;
        std::cout << std::setw(wds) <<  OPAR_TYPE;
        std::cout << std::setw(wds) <<  VZTAZNY_BOD;
        std::cout << "\n";
}

// export only one parcel with envelope
void LOB::pr()
{
    const int wdl  = 16;
    const int wds  = 8;
    
        std::cout << std::fixed << std::setprecision(2) << std::right;
        std::cout << std::setw(wdl) <<  ID;
        std::cout << std::setw(wds) <<  STAV_DAT;
        std::cout << std::setw(wds) <<  DATUM_VZNIKU;
        std::cout << std::setw(wds) <<  DATUM_ZANIKU;
        std::cout << std::setw(wds) <<  PRIZNAK_KONTEXTU;
        std::cout << std::setw(wds) <<  RIZENI_ID_VZNIKU;
        std::cout << std::setw(wds) <<  RIZENI_ID_ZANIKU;
        std::cout << std::setw(wds) <<  TYPPPD_KOD;
        std::cout << std::setw(wdl) <<  SOURADNICE_Y;
        std::cout << std::setw(wdl) <<  SOURADNICE_X;
        std::cout << std::setw(wds) <<  VELIKOST;
        std::cout << std::setw(wds) <<  UHEL;
        std::cout << std::setw(wdl) <<  BUD_ID;
        std::cout << std::setw(wds) <<  OBRBUD_TYPE;
        std::cout << "\n";
}



void help(std::map<std::string, VFK_CMD> cmdcode)
{
    for (std::map<std::string, VFK_CMD>::iterator it = cmdcode.begin(); it != cmdcode.end(); it++)
        std::cout << it->first << "\t   " << it->second << "\n";

}


void TDatabase::info()
{
    std::cout << "=========== Data info ==============\n";
    
    std::cout << "SOBR:  ";
    if (p_sobr) {
        std::cout << "ALLOC   " << p_sobr->data.size() << '\n';
    }
    else std::cout << "NULL" << '\n';
    
    std::cout << "SPOL:  ";
    if (p_spol) {
        std::cout << "ALLOC   " << p_spol->data.size() << '\n';
    }
    else std::cout << "NULL" << '\n';

    std::cout << "SBP:   ";
    if (p_sbp) {
        std::cout << "ALLOC   " << p_sbp->data.size() << '\n';
    }
    else std::cout << "NULL" << '\n';
    std::cout << "HP:    ";
    if (p_hp) {
        std::cout << "ALLOC   " << p_hp->data.size() << '\n';
    }
    else std::cout << "NULL" << '\n';
    std::cout << "OP:    ";
    if (p_op) {
        std::cout << "ALLOC   " << p_op->data.size() << '\n';
    }
    else std::cout << "NULL" << '\n';
    std::cout << "OB:    ";
    if (p_ob) {
        std::cout << "ALLOC   " << p_ob->data.size() << '\n';
    }
    else std::cout << "NULL" << '\n';

    std::cout << "===========--------==============\n";
           
}


TYP_idx TSOBR::find_point_by_id(TYP_id pt)
{        
    TYP_idx res = 0;
    while(res < data.size())
    {
        if(data[res].ID == pt) break;
        else res++;
    }
    if (res==data.size()) res = -1;
    return res;   
}

void TSOBR::status()
{
    int nn = data.size();
    std::cout << "=========== status sobr =========\n";
    std::cout << "Size: " << "\t   " << nn << "\n";
    if (nn>20)
    {
        std::cout << "Showing the first 10 records.\n";
        nn = 10;
    }
    for (int k=0; k<nn; k++)
        std::cout << std::setw(12) << data[k].ID << ":\t   " << data[k].CISLO_BODU << "\n";
    for (int k=0; k<nn; k++)
        data[k].pr();
    std::cout << "===========--------==============\n";
           
}

void TSPOL::status()
{
    int nn = data.size();
    std::cout << "=========== status SPOL =========\n";
    std::cout << "Size: " << "\t   " << nn << "\n";
    if (nn>20)
    {
        std::cout << "Showing the first 10 records.\n";
        nn = 10;
    }
    for (int k=0; k<nn; k++)
        std::cout << std::setw(12) << data[k].ID << ":\t   " << data[k].CISLO_BODU << "\n";
    for (int k=0; k<nn; k++)
        data[k].pr();
    std::cout << "===========--------==============\n";
           
}

void TSBP::status()
{
    int nn = data.size();
    std::cout << "=========== status SBP ==========\n";
    std::cout << "Size: " << "\t   " << nn << "\n";
    if (nn>20)
    {
        std::cout << "Showing the first 10 records.\n";
        nn = 10;
    }
    for (int k=0; k<nn; k++)
        std::cout << std::setw(12) << data[k].ID << ":\t   " << data[k].PORADOVE_CISLO_BODU << "   " << data[k].BP_ID << '\n';
    for (int k=0; k<nn; k++)
        data[k].pr();
    std::cout << "===========--------==============\n";
}

void THP::status()
{
    int nn = data.size();
    std::cout << "=========== status HP ===========\n";
    std::cout << "Size: " << "\t   " << nn << "\n";
    if (nn>20)
    {
        std::cout << "Showing the first 10 records.\n";
        nn = 10;
    }
    for (int k=0; k<nn; k++)
        std::cout << std::setw(12) << data[k].ID << ":\t   " << data[k].PAR_ID_1 << "  " << data[k].PAR_ID_2 << "\n";
    for (int k=0; k<nn; k++)
        data[k].pr();
    std::cout << "===========--------==============\n";
}

void TOP::status()
{
    int nn = data.size();
    std::cout << "=========== status OP ===========\n";
    std::cout << "Size: " << "\t   " << nn << "\n";
    if (nn>20)
    {
        std::cout << "Showing the first 10 records.\n";
        nn = 10;
    }
    for (int k=0; k<nn; k++)
        std::cout << std::setw(12) << data[k].ID << ":\t   " << data[k].PAR_ID << "\n";
    for (int k=0; k<nn; k++)
        data[k].pr();
    std::cout << "===========--------==============\n";
}

// najde vsechny hp, ktere ukazuji na danou parcelu
// tj. kazdy segment hranice
// find all, return vec
// returns count of found
int THP::find_par_id(TYP_id source_id, std::vector<int> &indi, VEC_id &par_ids)
{
    indi.clear();
    par_ids.clear();

    for (int k=0; k<data.size(); k++)
    {
        if (data[k].PAR_ID_1 == source_id) 
        {
            indi.push_back(k);
            if (data[k].PAR_ID_2 > 0)
                par_ids.push_back(data[k].PAR_ID_2);        // na statni hranici default value 0    
        }
        if (data[k].PAR_ID_2 == source_id) 
        {
            indi.push_back(k);
            par_ids.push_back(data[k].PAR_ID_1);            // zde povinna polozka, exists
        }
    }
    return indi.size();
}


int TOP::uniquize_list(VEC_s &par_cis, VEC_idx &par_idx, VEC_idx &typppd)
{

    auto it_cis = par_cis.begin();
    auto it_idx = par_idx.begin();
    auto it_ppd = typppd.begin();
    int k = 0, smaz=0;                      // is not used
    
    while(it_idx != par_idx.end())
    {
        auto it_idx_walk = par_idx.begin();
        while(it_idx_walk != it_idx)
        {
            if( *it_idx_walk == *it_idx ) break;
            it_idx_walk++;
        }

        if (it_idx_walk != it_idx)    // found,  
        {
//            std::cout << "DEB Uniquize: Erase " << *it_cis << "  idx=" << *it_idx << "\n";
            par_cis.erase(it_cis);
            par_idx.erase(it_idx);
            typppd.erase(it_ppd);   // don't increment iterators!
            smaz++;
        }
        else
        {
            it_cis++;
            it_idx++;
            it_ppd++;
            k++;
        }

    }
//    std::cout << "DEB Uniquize: Erase end: uniq=" << k << "  smaz=" << smaz << "  idx_size=" << par_idx.size() << "\n";
    return par_idx.size();

}


void TDatabase::clear()
{
                p_sobr->data.clear();
                p_sbp->data.clear();
                p_spol->data.clear();
                p_hp->data.clear();
                p_op->data.clear();
                p_ob->data.clear();
}


//int TDatabase::find_parcela_neighbors(std::string s, int st, VEC_idx &indi_hp, VEC_idx &indi_op, VEC_id &par_ids, VEC_s &par_cisla, VEC_idx &typppd)



VFK_OPCODE get_opcode(const std::string s)
{
    size_t idx = s.find_first_of(';');  // if not found, whole string is cmd
    
    std::string cm = s.substr(0,idx);
    
    if      (cm == "&DSOBR") return VFK_OPCODE_DSOBR;        // TODO casem predelat na hash (OPCODE,string) nebo obracene
    else if (cm == "&BSOBR") return VFK_OPCODE_BSOBR;
    else if (cm == "&BSBP") return VFK_OPCODE_BSBP;
    else if (cm == "&DSBP") return VFK_OPCODE_DSBP;
    else if (cm == "&BHP") return VFK_OPCODE_BHP;
    else if (cm == "&DHP") return VFK_OPCODE_DHP;
    else if (cm == "&BOP") return VFK_OPCODE_IGNORE;
    else if (cm == "&DOP") return VFK_OPCODE_DOP;
    else if (cm == "&DOB") return VFK_OPCODE_DOB;
    else if (cm == "&BOB") return VFK_OPCODE_BOB;
    else if (cm == "&BSPOL") return VFK_OPCODE_IGNORE;
    else if (cm == "&DSPOL") return VFK_OPCODE_DSPOL;
    else if (cm == "&HZMENY") return VFK_OPCODE_IGNORE;
    else if (cm == "&HPOLYG") return VFK_OPCODE_IGNORE;
    else if (cm == "&HKATUZE") return VFK_OPCODE_IGNORE;
    else if (cm == "&DKATUZE") return VFK_OPCODE_IGNORE;
    else if (cm == "&HNAVRHY") return VFK_OPCODE_IGNORE;
    else if (cm == "&HSKUPINA") return VFK_OPCODE_IGNORE;
    else if (cm == "&HPLATNOST") return VFK_OPCODE_IGNORE;
    else if (cm == "&HJMENO") return VFK_OPCODE_IGNORE;
    else if (cm == "&DSPOL") return VFK_OPCODE_IGNORE;
    else if (cm == "&DZVB") return VFK_OPCODE_IGNORE;
    else if (cm == "&BZVB") return VFK_OPCODE_IGNORE;
    else if (cm == "&BDPM") return VFK_OPCODE_IGNORE;
    else if (cm == "&DDPM") return VFK_OPCODE_IGNORE;
    else if (cm == "&DOBPEJ") return VFK_OPCODE_IGNORE;
    else if (cm == "&DHBPEJ") return VFK_OPCODE_IGNORE;
    else if (cm == "&BOBPEJ") return VFK_OPCODE_IGNORE;
    else if (cm == "&BHBPEJ") return VFK_OPCODE_IGNORE;
    else if (cm == "&DOBBP") return VFK_OPCODE_IGNORE;
    else if (cm == "&BOBBP") return VFK_OPCODE_IGNORE;
    else if (cm == "&DSBP") return VFK_OPCODE_IGNORE;
    else if (cm == "&BSBM") return VFK_OPCODE_IGNORE;
    else if (cm == "&DSBM") return VFK_OPCODE_IGNORE;
    else if (cm == "&BTYPPPD") return VFK_OPCODE_IGNORE;
    else if (cm == "&DTYPPPD") return VFK_OPCODE_IGNORE;
    else if (cm == "&DTYPSOS") return VFK_OPCODE_IGNORE;
    else if (cm == "&BTYPSOS") return VFK_OPCODE_IGNORE;
    else if (cm == "&DKODCHB") return VFK_OPCODE_IGNORE;
    else if (cm == "&BKODCHB") return VFK_OPCODE_IGNORE;
    else if (cm == "&HVERZE") return VFK_OPCODE_IGNORE;
    else if (cm == "&HPUVOD") return VFK_OPCODE_IGNORE;
    else if (cm == "&HVYTVORENO") return VFK_OPCODE_IGNORE;
    else if (cm == "&HCODEPAGE") return VFK_OPCODE_IGNORE;
    else if (cm == "&BPOM") return VFK_OPCODE_IGNORE;
    else if (cm == "&BSPOM") return VFK_OPCODE_IGNORE;
    else if (cm == "&K") return VFK_OPCODE_IGNORE;

    else return VFK_OPCODE_UNKNOWN;
}





    


std::string get_opcode_str(const std::string s)
{
    size_t idx = s.find_first_of(';');  // if not found, whole string is cmd    
    return s.substr(0,idx);
}

void load_file(std::istream& in, TDatabase &D) {

   std::string tmp;
   VFK_OPCODE opc;
   
   while (!in.eof()) {
      getline(in, tmp, '\n');                    
           
      if (tmp.back() == '\r') tmp.pop_back();       // remove trailing CR
      
      opc = get_opcode(tmp);
      if (opc != VFK_OPCODE_IGNORE)
      {
//          std::cout << "DISPATCH: " << tmp << "   " << "opcode: " << opc << '\n';
      }

      switch(opc)
      {
          case VFK_OPCODE_DSOBR:
              D.p_sobr -> import_line(tmp);
              break;
          case VFK_OPCODE_DSBP:
              D.p_sbp -> import_line(tmp);
              break;
          case VFK_OPCODE_DSPOL:
              D.p_spol -> import_line(tmp);
              break;
          case VFK_OPCODE_DHP:
              D.p_hp -> import_line(tmp);
              break;
          case VFK_OPCODE_DOP:
              D.p_op -> import_line(tmp);
              break;
          case VFK_OPCODE_DOB:
              D.p_ob -> import_line(tmp);
              break;
          case VFK_OPCODE_UNKNOWN:
              std::cout << "Opcode: " << opc << "  " << get_opcode_str(tmp) << " not implemented.\n";
              break;
      }

      tmp.clear();
   }
}


    
// nesmi byt mezera ani za cislem ??? TODO detrail
void TSOBR::import_line(std::string li)
{
//    std::cout << "Importing line |" << li << "|.\n";

   LSOBR newline;
        
   std::vector<std::string> vecstr;

      split_string(li, ';', vecstr);
      
      if (0)    // debug: test splitting
      {
          for (std::vector<std::string>::iterator it = vecstr.begin(); it != vecstr.end(); ++it)
          {
                std::cout << *it << " | ";
          }
          std::cout << "\n";
      }
      
 //     std::cout << vecstr.size();
      // PRO PEVNY FORMAT DSOBR
      if (vecstr.size() != 11)
      {
          std::cout << "Error: format of DSOBR - wrong num of words " << vecstr.size() << "\n";
          return;
      }
          
//      newline.ID            = std::stoul("1234");
//        std::cout << vecstr[1].length() << "  " << vecstr[1] << "\n";
        
        int pos = 1;
        try {
            newline.ID            = safestoul(vecstr[1],0,1);  pos++;      // povinne polozky hodi exception vzdycky
            newline.STAV_DAT      = safestoi(vecstr[2],-1);     pos++;
            newline.KATUZE_KOD    = safestoi(vecstr[3],-1);     pos++;
            newline.CISLO_ZPMZ    = safestoi(vecstr[4],-1);     pos++;
            newline.CISLO_TL      = safestoi(vecstr[5],-1);     pos++;
            newline.CISLO_BODU    = safestol(vecstr[6],0,1);    pos++;
            newline.UPLNE_CISLO   = safestol(vecstr[7],-1);     pos++;
            newline.SOURADNICE_Y  = safestod(vecstr[8],0,1);   pos++;
            newline.SOURADNICE_X  = safestod(vecstr[9],0,1);   pos++;
            newline.KOD_CHB       = safestoi(vecstr[10],-1);
        }
        catch(std::invalid_argument &ia) {
                      std::cout << "Error '" << ia.what() << "': converting '  argno=" << pos << ",  str='" << vecstr[pos] << "'\n";
                      return;
            
        }
        data.push_back(newline);
//      std::cout << "$$$\n";
//      vecstr.clear();
   
}

void TSPOL::import_line(std::string li)
{
//    std::cout << "Importing line |" << li << "|.\n";

   LSPOL newline;
        
   std::vector<std::string> vecstr;

      split_string(li, ';', vecstr);
      
      if (0)    // debug: test splitting
      {
          for (std::vector<std::string>::iterator it = vecstr.begin(); it != vecstr.end(); ++it)
          {
                std::cout << *it << " | ";
          }
          std::cout << "\n";
      }
      
 //     std::cout << vecstr.size();
      // PRO PEVNY FORMAT DSOBR
      if (vecstr.size() != 13)
      {
          std::cout << "Error: format of DSPOL - wrong num of words " << vecstr.size() << "\n";
          return;
      }
          
//      newline.ID            = std::stoul("1234");
//        std::cout << vecstr[1].length() << "  " << vecstr[1] << "\n";
        
        int pos = 1;
        try {
            newline.ID            = safestoul(vecstr[1],0,1);   pos++;      // povinne polozky hodi exception vzdycky
            newline.STAV_DAT      = safestoi(vecstr[2],-1);     pos++;
            newline.KATUZE_KOD    = safestoi(vecstr[3],-1);     pos++;
            newline.CISLO_ZPMZ    = safestoi(vecstr[4],-1);     pos++;
            newline.CISLO_TL      = safestoi(vecstr[5],-1);     pos++;
            newline.CISLO_BODU    = safestol(vecstr[6],0,1);    pos++;
            newline.UPLNE_CISLO   = safestol(vecstr[7],0,1);    pos++;      // povinne
            newline.SOURADNICE_Y  = safestod(vecstr[8],0,1);    pos++;
            newline.SOURADNICE_X  = safestod(vecstr[9],0,1);    pos++;
            newline.KOD_CHB       = safestoi(vecstr[10],-1);    pos++;
            newline.KATUZE_KOD    = safestoi(vecstr[11],-1);    pos++;
            newline.CISLO_ZPMZ    = safestoi(vecstr[12],-1);    
        }
        catch(std::invalid_argument &ia) {
                      std::cout << "Error '" << ia.what() << "': converting '  argno=" << pos << ",  str='" << vecstr[pos] << "'\n";
                      return;
            
        }
        data.push_back(newline);
//      std::cout << "$$$\n";
//      vecstr.clear();
   
}
// nesmi byt mezera ani za cislem ??? TODO detrail
void TSBP::import_line(std::string li)
{
//    std::cout << "Importing line |" << li << "|.\n";

    LSBP newline;
        
   std::vector<std::string> vecstr;

      split_string(li, ';', vecstr);
      
      // PRO PEVNY FORMAT DSOBR
      if (vecstr.size() != 15)        //TODO
      {
          std::cout << "Error: format of DSBP - wrong num of words " << vecstr.size() << "\n";
          return;
      }
          
//      newline.ID            = std::stoul("1234");
//        std::cout << vecstr[1].length() << "  " << vecstr[1] << "\n";

        int pos = 1;
        try {
            newline.ID                      = safestoul(vecstr[1],0,1);     pos++;      // povinne polozky hodi exception vzdycky
            newline.STAV_DAT                = safestoi(vecstr[2],-1);       pos++;
            newline.DATUM_VZNIKU            = vecstr[3];                    pos++;
            newline.DATUM_ZANIKU            = vecstr[4];                    pos++;
            newline.PRIZNAK_KONTEXTU        = safestoi(vecstr[5],-1);       pos++;
            newline.RIZENI_ID_VZNIKU        = vecstr[6];                    pos++;
            newline.RIZENI_ID_ZANIKU        = vecstr[7];                    pos++;
            newline.BP_ID                   = safestoul(vecstr[8],0,1);     pos++;
            newline.PORADOVE_CISLO_BODU     = safestoul(vecstr[9],0,1);     pos++;
            newline.OB_ID                   = safestoul(vecstr[10],0,0);    pos++;
            newline.HP_ID                   = safestoul(vecstr[11],0,0);    pos++;
            newline.DPM_ID                  = safestoul(vecstr[12],0,0);    pos++;
            newline.PARAMETRY_SPOJENI       = vecstr[13];                   pos++;
            newline.ZVB_ID                  = safestoul(vecstr[14],0,0);
        }
        catch(std::invalid_argument &ia) {
                      std::cout << "Error '" << ia.what() << "': converting '  argno=" << pos << ",  str='" << vecstr[pos] << "'\n";
                      return;
            
        }
        data.push_back(newline);
   
}

void THP::import_line(std::string li)
{
//    std::cout << "Importing line |" << li << "|.\n";

    LHP newline;
        
   std::vector<std::string> vecstr;

      split_string(li, ';', vecstr);
      
      // PRO PEVNY FORMAT DSOBR
      if (vecstr.size() != 11)        //TODO
      {
          std::cout << "Error: format of DHP - wrong num of words " << vecstr.size() << "\n";
          return;
      }
          
//      newline.ID            = std::stoul("1234");
//        std::cout << vecstr[1].length() << "  " << vecstr[1] << "\n";

        int pos = 1;
        try {
            newline.ID                      = safestoul(vecstr[1],0,1);     pos++;      // povinne polozky hodi exception vzdycky
            newline.STAV_DAT                = safestoi(vecstr[2],-1);       pos++;
            newline.DATUM_VZNIKU            = vecstr[3];                    pos++;
            newline.DATUM_ZANIKU            = vecstr[4];                    pos++;
            newline.PRIZNAK_KONTEXTU        = safestoi(vecstr[5],-1);       pos++;
            newline.RIZENI_ID_VZNIKU        = vecstr[6];                    pos++;
            newline.RIZENI_ID_ZANIKU        = vecstr[7];                    pos++;
            newline.TYPPPD_KOD              = safestoi(vecstr[8],0,1);      pos++;
            newline.PAR_ID_1                = safestoul(vecstr[9],0,1);     pos++;
            newline.PAR_ID_2                = safestoul(vecstr[10],0,0);    pos++;
        }
        catch(std::invalid_argument &ia) {
                      std::cout << "Error '" << ia.what() << "': converting '  argno=" << pos << ",  str='" << vecstr[pos] << "'\n";
                      return;
            
        }
        data.push_back(newline);
}



std::string quote_string(const char *s)
{
    std::string a = "\"";
    a += s;
    a.push_back('"');
    return a;
}

std::string quote_string(const std::string s)
{
    std::string a = "\"";
    a += s;
    a.push_back('"');
    return a;
}

std::string dequote_string(const std::string s)
{
    std::string a = "";
    if (s[0]=='\"') a = s.substr(1,std::string::npos);
    else a = s;
    if (a.back() == '\"') a.pop_back();
    return a;
}

void TOP::import_line(std::string li)
{
//    std::cout << "Importing line |" << li << "|.\n";

    LOP newline;
        
   std::vector<std::string> vecstr;

      split_string(li, ';', vecstr);
      
      // PRO PEVNY FORMAT DSOBR
      if (vecstr.size() != 17)        //TODO
      {
          std::cout << "Error: format of DOP - wrong num of words " << vecstr.size() << "\n";
          return;
      }
          
//      newline.ID            = std::stoul("1234");
//        std::cout << vecstr[1].length() << "  " << vecstr[1] << "\n";

        int pos = 1;
        try {
            newline.ID                      = safestoul(vecstr[1],0,1);     pos++;      // povinne polozky hodi exception vzdycky
            newline.STAV_DAT                = safestoi(vecstr[2],-1);       pos++;
            newline.DATUM_VZNIKU            = vecstr[3];                    pos++;
            newline.DATUM_ZANIKU            = vecstr[4];                    pos++;
            newline.PRIZNAK_KONTEXTU        = safestoi(vecstr[5],-1);       pos++;
            newline.RIZENI_ID_VZNIKU        = vecstr[6];                    pos++;
            newline.RIZENI_ID_ZANIKU        = vecstr[7];                    pos++;
            newline.TYPPPD_KOD              = safestoi(vecstr[8],0,1);      pos++;
            newline.SOURADNICE_Y            = safestod(vecstr[9],0,0);      pos++;
            newline.SOURADNICE_X            = safestod(vecstr[10],0,0);     pos++;
            newline.TEXT                    = vecstr[11];                   pos++;
            newline.VELIKOST                = safestod(vecstr[12],12,0);    pos++;      //default 12
            newline.UHEL                    = safestod(vecstr[13],0,0);     pos++;
            newline.PAR_ID                  = safestoul(vecstr[14],0,1);    pos++;
            newline.OPAR_TYPE               = vecstr[15];                   pos++;
            newline.VZTAZNY_BOD             = safestoi(vecstr[16],0,0);
        }
        catch(std::invalid_argument &ia) {
                      std::cout << "Error '" << ia.what() << "': converting '  argno=" << pos << ",  str='" << vecstr[pos] << "'\n";
                      return;
            
        }
        data.push_back(newline);
}

void TOB::import_line(std::string li)
{
    LOB newline;
        
   std::vector<std::string> vecstr;

      split_string(li, ';', vecstr);
      
      // PRO PEVNY FORMAT
      if (vecstr.size() != 15)        //TODO
      {
          std::cout << "Error: format of DOB - wrong num of words " << vecstr.size() << "\n";
          return;
      }
          
//      newline.ID            = std::stoul("1234");
//        std::cout << vecstr[1].length() << "  " << vecstr[1] << "\n";

        int pos = 1;
        try {
            newline.ID                      = safestoul(vecstr[1],0,1);     pos++;      // povinne polozky hodi exception vzdycky
            newline.STAV_DAT                = safestoi(vecstr[2],-1);       pos++;
            newline.DATUM_VZNIKU            = vecstr[3];                    pos++;
            newline.DATUM_ZANIKU            = vecstr[4];                    pos++;
            newline.PRIZNAK_KONTEXTU        = safestoi(vecstr[5],-1);       pos++;
            newline.RIZENI_ID_VZNIKU        = vecstr[6];                    pos++;
            newline.RIZENI_ID_ZANIKU        = vecstr[7];                    pos++;
            newline.TYPPPD_KOD              = safestoi(vecstr[8],0,1);      pos++;
            newline.SOURADNICE_Y            = safestod(vecstr[9],0,0);      pos++;
            newline.SOURADNICE_X            = safestod(vecstr[10],0,0);     pos++;
            newline.VELIKOST                = safestod(vecstr[11],12,0);    pos++;      //default 12
            newline.UHEL                    = safestod(vecstr[12],0,0);     pos++;
            newline.BUD_ID                  = safestoul(vecstr[13],0,1);    pos++;
            newline.OBRBUD_TYPE             = vecstr[14];                   pos++;
        }
        catch(std::invalid_argument &ia) {
                      std::cout << "Error '" << ia.what() << "': converting '  argno=" << pos << ",  str='" << vecstr[pos] << "'\n";
                      return;
            
        }
        data.push_back(newline);
}



void BoundaryList::cyclize()
{
    for(int k = 1; k < (*this).size(); ++k)           // start from 1, compare with previous
                                                      // the last one k is also important due to swap_order
    {
        int m = k;
        while(m < (*this).size())
        {
                                                    // [k-1].2 connects to m[2]          
            if ((*this)[k-1].UPLNE_CISLO2 == (*this)[m].UPLNE_CISLO2)   
                                                                // muzem porovnat pres cislo bodu,  [ nebo pres SOBR.ID, ale to je zavislost navic, a taky muze byt bod ze SPOL ]          
                                                                // predpokladame tedy existujici a unikatni cisla bodu - neplati !!!
                                                                // zmena: pridavam UPLNE_CISLO - i kdyz to je nepovinna polozka
                                                                // neunikatnost nezpusobi chybu, ale nedosahne se vysledku - leda porovnat primo XY
            {
                (*this)[m].swap_order();
            }

                                                    // [k-1].2 connects to m[1] 
            if ((*this)[k-1].UPLNE_CISLO2 == (*this)[m].UPLNE_CISLO1)   
            {
                //swap
                if (k != m) std::swap((*this)[k], (*this)[m]);
                break;
            }
            m++;
        }
    }
}




int TSBP::find_sources_of_hplist(VEC_id hp_ids, VEC_idx &sbp_idx, VEC_idx &sbp_12, VEC_id &sbp_XY_id, VEC_idx &whathp_idx, VEC_idx &counts)
{
    int res=0;
    counts.clear();
    counts.resize(hp_ids.size());
    for(int k=0; k < data.size(); k++)
    {
        for(int hh=0; hh < hp_ids.size(); hh++)
        {
            if (data[k].HP_ID == hp_ids[hh])
            {
                sbp_idx.push_back(k);           // nekolik(dva) sbp ukazuje na stejne hp, 
                whathp_idx.push_back(hh);
                sbp_12.push_back(data[k].PORADOVE_CISLO_BODU);
                sbp_XY_id.push_back(data[k].BP_ID);      
                res++;
                counts[hh]++;
            }
        }
    }
    
    return res;
}

int TSBP::find_sources_of_oblist(VEC_id ob_ids, VEC_idx &sbp_idx, VEC_idx &sbp_12, VEC_id &sbp_XY_id, VEC_idx &whatob_idx, VEC_idx &counts)
{
    int res=0;
    counts.clear();
    counts.resize(ob_ids.size());
    for(int k=0; k < data.size(); k++)
    {
        for(int hh=0; hh < ob_ids.size(); hh++)
        {
            if (data[k].OB_ID == ob_ids[hh])
            {
                sbp_idx.push_back(k);           // nekolik(dva) sbp ukazuje na stejne hp, 
                whatob_idx.push_back(hh);
                sbp_12.push_back(data[k].PORADOVE_CISLO_BODU);
                sbp_XY_id.push_back(data[k].BP_ID);      
                res++;
               counts[hh]++;
             }
        }
    }
    return res;
}

int TOB::find_linesegments_by_id(TYP_id bud_id, VEC_idx &ob_idx, VEC_id &ob_ids, std::string obtyp)
{
    int res=0;
    for(int k=0; k < data.size(); k++)
    {
           if (data[k].BUD_ID == bud_id && data[k].OBRBUD_TYPE == obtyp)
           {
               ob_idx.push_back(k);           // nekolik(dva) sbp ukazuje na stejne hp, 
               ob_ids.push_back(data[k].ID);
               res++;
           }
    }
    return res;
}

void TDatabase::print_parcela_coord(const BoundaryList L) const
{
        std::cout << "--------------- P A R C    C O O R D   (BOUNDARY)  -------------------------\n";
        for(auto it = L.begin(); it != L.end(); ++it)
        {
            it->pr();
        }
}

void TDatabase::print_budova_coord(const BoundaryList L) const
{
        std::cout << "--------------- B U D O V A   C O O R D   (BOUNDARY)  -------------------------\n";
        for(auto it = L.begin(); it != L.end(); ++it)
        {
            it->pr();
        }
}

    

void TOP::print_neighbors(const VEC_idx indi_op, const VEC_id par_ids, const VEC_s par_cisla, const VEC_idx par_typppd)
{
             std::cout << "-----------------  Neighbors results:  ----------------\n";

              

              for (int k=0; k<indi_op.size(); k++) data[indi_op[k]].pr();

              int kk = indi_op.size();
              for (int k=0; k<indi_op.size(); k++) 
                  std::cout << k << ":\t " << "  | " << std::setw(6) << indi_op[k] << "\t| " << par_ids[k] << "\t| " << std::setw(8) << std::right << par_cisla[k] << " " << par_typppd[k] << "\n";

              for (int k=0; k < indi_op.size(); k++)
                std::cout << par_cisla[k] << ":" << par_typppd[k] <<"  ";
              std::cout << "\n";

}



// stavebni: st=1, pozemkova: st=0,
// vraci idx parcely v OP
// not found == -1
// pozor, v katastru jsou vsechna cisla v uvozovkach, tj. hledat "\"284\""
TYP_idx TOP::find_parcela(std::string s, int st)
{
    int k=0; 
    int kod = st ? 28 : 18;
    while(k<data.size())
    {
        if (data[k].TEXT == s && data[k].TYPPPD_KOD==kod) break; //
//        std::cout << data[k].TEXT << '\n';
        k++;
    }
    
    if (k==data.size()) return -1;
    else return k;
}

//find first
std::string TOP::get_parcela_name(TYP_id pc, int &idx, int &typ)
{
    int k=0; 
    while(k<data.size())
    {
        if (data[k].PAR_ID == pc) break;
        k++;
    }
    
    idx = k;
    if (k==data.size()) return "NULL";
    else 
    {
        typ = data[k].TYPPPD_KOD;
        return data[k].TEXT;
    }
}


//stavebni: st=1
//returns count of found
// pozor, v katastru jsou vsechna cisla v uvozovkach, tj. hledat "\"284\""
int TDatabase::find_parcela_neighbors(std::string s, int st, VEC_idx &indi_hp, VEC_idx &indi_op, VEC_id &par_ids, VEC_s &par_cisla, VEC_idx &typppd)
{
    indi_hp.clear();
    indi_op.clear();
    par_ids.clear();
    par_cisla.clear();
    int pc_idx = p_op->find_parcela(s, st);    
//    std::cout << "DEB Neighbor pc_idx=: " << pc_idx << "\n";
    if (pc_idx == -1) return 0;
    TYP_id pc_id = p_op->data[pc_idx].PAR_ID;
    
//    std::cout << "DEB Neighbor pc_id=: " << pc_id << "\n";

    int res;
    
    res = p_hp -> find_par_id(pc_id, indi_hp, par_ids);

//    std::cout << "DEB Neighbor hp_res=: " << res << "\n";
    
    int neigh_pc_idx;
    int neigh_pc_typ;
    for(int k=0; k<res; k++)
    {
        par_cisla.push_back(p_op -> get_parcela_name(par_ids[k], neigh_pc_idx, neigh_pc_typ));        
        indi_op.push_back(neigh_pc_idx);                
        typppd.push_back(neigh_pc_typ);                
 //       std::cout << "DEB Neighbor parids=: " <<  par_ids[k] << "  op_idx=" << neigh_pc_idx << "  " << par_cisla.back() << "\n";
    }

    
    int kk = indi_op.size();
    while(kk-- > 0)
    {
       if (typppd[kk] != 18 && typppd[kk] != 28)         // nechame jen pozemk a stav parcely, necgcwmr zahrady
       {
           typppd.erase(typppd.begin()+kk);
           indi_op.erase(indi_op.begin()+kk);
           par_cisla.erase(par_cisla.begin()+kk);
       }
    }

//    std::cout << "DEB Uniquize: orig=" << res << "\n";
    res = p_op -> uniquize_list(par_cisla, indi_op, typppd);
//    std::cout << "DEB Uniquize: orig=" << res << "\n";
    
    return res;
}

