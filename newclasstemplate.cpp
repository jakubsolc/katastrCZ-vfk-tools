

// // g++ my_xpath_select.cpp pugixml.cpp -o z
// // ./z -quiet -addpc 284 -addpc "13/1" -addpc "st. 6/1" -expoo -status > exkuk.m
// // ./z -quiet -ku 618225.xml -addlist "st. 31/1" "st. 32" 526 525 456/8 "st. 28/2" 456/11 528 "st. 65" 456/5 456/9 "st. 33" "st. 34" 11/3 11/4 -expoo > plancach.m


//  VFK importer pro SOBR, ?SPOl, ...

//  hlavni objekty:  tabulka obrazu bodu s pevnym poradim sloupcu


//#include "pugixml.hpp"

#include <iostream>
#include <iomanip>  //setw
#include <string>
#include <vector>
#include <map>

/*
 * 
 * 
 * 
 * 
 * 
 * 
 * 
*/


class LOBR          
//              LINE OF TABLE SOBR                 KEY          POV SIZE       (podle specifikace VFK)
{
public:
        long unsigned int   ID;                 // PRIMARY      A   30.0
        int                 STAV_DAT;           //              N   2
        int                 KATUZE_KOD;         // UNIQ CIZI    N   6.0
        int                 CISLO_ZPMZ;         // UNIQ CIZI    N   5
        int                 CISLO_TL;           // UNIQ         N   4
        long int            CISLO_BODU;         // UNIQ         A   12.0
        long int            UPLNE_CISLO;        //              N   12.0
        double              SOURADNICE_Y;       //              A   10.2
        double              SOURADNICE_X;       //              A   10.2
        int                 KOD_CHB;            //      CIZI    N   2.0

        void                pr();               // printer
};

class TOBR
{
public:
        std::vector<LOBR>   data;

        void        pr();
        void        status();

//        static const int wd = 13;
};


void LOBR::pr()
{
/*        std::cout << std::setw(wd) << std::left << "Id: " << id << "\n";
        std::cout << std::setw(wd) << std::left << "Cislo: " << cislo << "\n";
//        std::cout << << std::setw(8) << std::left <<((strlabel.length()<7) ? "\t" : "\t");
        std::cout << std::setw(wd) << std::left << "Area: " << area << "\n";
        std::cout << std::setw(wd) << std::left << "Ref. Point:  " << refPoint << "\n";
        std::cout << "Coord:\n  A=[ " << coords << "];\n";
*/}

// export only one parcel with envelope
void TOBR::pr()
{
/*        std::cout << std::setw(wd) << std::left << "\n%  Cislo: " << cislo << "\n";
//        std::cout << << std::setw(8) << std::left <<((strlabel.length()<7) ? "\t" : "\t");
        std::cout << std::setw(wd) << std::left << "PC_IDS{end+1} = '" << cislo << "'\n";
        std::cout << std::setw(wd) << std::left << "PC_RP{end+1}  = [" << refPoint << "];\n";
        std::cout << "PC_COO{end+1} = [ " << coords << "];\n\n";
        std::cout << "PC_RPr{end+1}=reshape(PC_RP{end},2,size(PC_RP{end},2)/2);\n";
        std::cout << "PC_COOr{end+1}=reshape(PC_COO{end},2,size(PC_COO{end},2)/2);\n";
        std::cout << "lw = 3;\n";
        std::cout << "plot(PC_COOr{end}(1,:), PC_COOr{end}(2,:), 'linewidth', lw);\n";
        std::cout << "hold on\n\n";
        std::cout << "text(mean(PC_COOr{end}(1,:)),mean(PC_COOr{end}(2,:)),PC_IDS{end}, 'fontsize', 12, 'horizontalalignment', 'center');\ndaspect([1 1 1]);\n";
//        std::cout << "text(PC_RPr{end}(1,:), PC_RPr{end}(2,:), PC_IDS{end}, 'fontsize', 12, 'horizontalalignment', 'center');\n\n";
*/
}



typedef enum {CMD_NOP, CMD_KU, CMD_PC, CMD_PR, CMD_EXP, CMD_EXPOO, CMD_ADDL, CMD_INFO, CMD_ADDPC, CMD_GO, CMD_QUI, CMD_VERB, CMD_DBGA, CMD_HLP, CMD_STAT} VFK_CMD;


void help(std::map<std::string, VFK_CMD> cmdcode)
{
    for (std::map<std::string, VFK_CMD>::iterator it = cmdcode.begin(); it != cmdcode.end(); it++)
        std::cout << it->first << "\t   " << it->second << "\n";

}

void TOBR::status()
{
    int nn = data.size();
    std::cout << "=========== status ==============\n";
       std::cout << "Size: " << "\t   " << nn << "\n";
       for (int k=0; k<nn; k++)
         std::cout << std::setw(12) << data[k].ID << ":\t   " << data[k].CISLO_BODU << "\n";
    std::cout << "===========--------==============\n";
           
}

int main(int argc, char **argv)
{

    if (0)      // first test
    {
    }
    
    
/*    if (!global_doc.load_file("724149.xml")) {std::cout << "Error loading default xml file.\n"; return -1;}
    
    std::map<std::string, TCMD> cmdcode = { {"-ku", CM_KU}, {"-pc", CM_PC}, {"-pr", CM_PR}, {"-exp", CM_EXP}, {"-expoo", CM_EXPOO}, {"-addlist",CM_ADDL},
        {"-info", CM_INFO}, {"-addpc", CM_ADDPC}, {"-go", CM_GO}, {"-quiet", CM_QUI}, {"-verb", CM_VERB}, 
    {"-debug_arg", CM_DBGA}, {"-help", CM_HLP}, {"--help", CM_HLP}, {"-status", CM_STAT} };
    int tmpint;
    int quiet = 0,
        debug_arg = 0;
    
    for(int i = 0; i < argc; i++)
    {
        if (cmdcode[argv[i]] == CM_DBGA) debug_arg = 1;
        debug_arg && std::cout << i << ":  " << argv[i] << "  \t";
        
        debug_arg && std::cout << "OPCODE: " << cmdcode[argv[i]] << (cmdcode[argv[i]] ? "  OK" : "  N/A") << "\n";
        if (cmdcode[argv[i]] == CM_QUI) quiet = 1;
    }
    
    int i = 1;
    auto chkarg = [&i,&argc](int m) -> bool { if (i+m >= argc) {std::cout << "Arg is missing.\n"; return 0;}; return 1;} ;
    while(i < argc)
    {
        quiet || std::cout << i << ":  " << argv[i] << "  \t";
        quiet || std::cout << "===== PROCESSING OPCODE: " << cmdcode[argv[i]] << (cmdcode[argv[i]] ? "  OK" : "  N/A") << "\n";
//todo check argc
        switch(cmdcode[argv[i]])
        {
            case CM_KU:
                if (!chkarg(1)) break;
                if (!global_doc.load_file(argv[i+1])) {std::cout << "Error loading xml file '" << argv[i+1] << "'.\n"; return -1;}
                i++;
                break;
            case CM_PC:
                if (!chkarg(1)) break;
                export_parcela(argv[i+1]);
                i++;
                break;
            case CM_PR:
                if (!chkarg(1)) break;
                tmpint = atoi(argv[i+1]);
                if (tmpint < global_set.parc.size())
                  global_set.parc[tmpint].pr();
                else
                    std::cout << "Index " << tmpint << " of global_set not found [size=" <<  global_set.parc.size() << "].\n";
                i++;
                break;
            case CM_EXPOO:
                global_set.export_set_octave();
                break;
                
            case CM_ADDPC:
                if (!chkarg(1)) break;
                global_set.addpc(argv[i+1]);
                i++;
                break;

            case CM_ADDL:
                while ( (i+1 < argc) && argv[i+1][0] != '-')
                {
                    global_set.addpc(argv[i+1]);
                    i++;
                }
                break;
            case CM_GO:
                if (!chkarg(1)) break;
                go(argv[i+1]);
                i++;
                break;
            case CM_QUI:
                quiet = 1;
                break;
            case CM_VERB:
                quiet = 0;
                break;
                
            case CM_HLP:
                help(cmdcode);
                break;

            case CM_STAT:
                global_set.status();
                break;
            default:
                std::cout << "Unknown command: " << argv[i] << "\n";
                break;
        }
        i++;        
    }
    
*/    
    
    return 0;
}
