

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

#include "colors.h"
//#include "safestring.cpp"
#include "rect.h"


#define     DEBPR(A,B)              std::cout << FGRN(A) << B << '\n';

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


/*
 * 
 * 
 * 
 * 
 * 
 * 
 * 
*/

typedef long unsigned int           TYP_id;
typedef std::vector<TYP_id>         VEC_id;
typedef int                         TYP_idx;
typedef std::vector<TYP_idx>        VEC_idx;
typedef std::vector<std::string>    VEC_s;


                
typedef enum {CMD_NOP, CMD_KU, CMD_PC, CMD_PR, CMD_EXP, CMD_EXPOO, CMD_ADDL, CMD_INFO, CMD_ADDPC, CMD_GO, CMD_QUI, CMD_VERB, CMD_DBGA, CMD_HLP, CMD_STAT} VFK_CMD;
typedef enum {VFK_OPCODE_UNKNOWN, VFK_OPCODE_IGNORE, VFK_OPCODE_DSOBR, VFK_OPCODE_BSOBR, VFK_OPCODE_DSBP, VFK_OPCODE_BSBP, VFK_OPCODE_DHP, VFK_OPCODE_BHP, 
                VFK_OPCODE_DOP, VFK_OPCODE_BOP, VFK_OPCODE_DOB, VFK_OPCODE_BOB, VFK_OPCODE_DSPOL, VFK_OPCODE_BSPOL} VFK_OPCODE;

class CBoundarySegment
{
public:
    double          X1;
    double          Y1;
    double          X2;
    double          Y2;
    int             CISLO1;
    int             CISLO2;
    int             UPLNE_CISLO1;
    int             UPLNE_CISLO2;
    int             KODCH1;
    int             KODCH2;
    TYP_id          PAR_ID_1; 
    TYP_id          PAR_ID_2;
    TYP_id          HP_ID;
    std::string     PARAM;
    
    void            pr() const;
    void            swap_order();
    void            prnxy() const;
};

//typedef std::vector<CBoundarySegment> BoundaryList;
class BoundaryList : public std::vector<CBoundarySegment>
{
public:
        std::string     METADATA;
        std::string     METADATA2;
        std::string     TEXT;
        double          REF_X;
        double          REF_Y;
        double          REF_U;
        
        void        cyclize();
        void        bare_export(std::string A) const;
        void        mprint(std::string A="A") const;
        Rect        boundaryBox();
        Rect        boundaryBox(Rect r);           //preinitialized by r
        void        pr(int styl = 0) const;

};
    
class  BoundarySet : public std::vector<BoundaryList>
{
public:
        std::string     METADATA;
        std::string     METADATA2;
        Rect            boundaryBox();
        void            pr(int styl = 0) const;
};
                
class LSOBR          
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

class LSPOL          
//              LINE OF TABLE SOBR                 KEY          POV SIZE       (podle specifikace VFK)
{
public:
        long unsigned int   ID;                 // PRIMARY      A   30.0
        int                 STAV_DAT;           //              N   2
        int                 KATUZE_KOD;         // UNIQ CIZI    N   6.0
        int                 CISLO_ZPMZ;         // UNIQ CIZI    N   5
        int                 CISLO_TL;           // UNIQ         N   4
        long int            CISLO_BODU;         // UNIQ         A   12
        long int            UPLNE_CISLO;        //              A   12
        double              SOURADNICE_Y;       //              A   10.2
        double              SOURADNICE_X;       //              A   10.2
        int                 KOD_CHB;            //      CIZI    N   2.0
        int                 KATUZE_KOD_MER;     // UNIQ CIZI    N   6.0
        int                 CISLO_ZPMZ_MER;     // UNIQ ciZI    N   5

        void                pr();               // printer
};

class LSBP          
//              LINE OF TABLE SOBR                 KEY          POV SIZE       (podle specifikace VFK)
{
public:
        long unsigned int   ID;                     // PRIMARY      A   30.0        !!!
        int                 STAV_DAT;               //              N   2
        std::string         DATUM_VZNIKU;           //              N   D
        std::string         DATUM_ZANIKU;           //              N   D
        int                 PRIZNAK_KONTEXTU;       //              N   1
        std::string         RIZENI_ID_VZNIKU;       //              N   30
        std::string         RIZENI_ID_ZANIKU;       //              N   30
        long unsigned int   BP_ID;                  //      cizi    A   30
        long unsigned int   PORADOVE_CISLO_BODU;    // uniq         A   38
        long unsigned int   OB_ID;                  // uniq CIZI    N   30
        long unsigned int   HP_ID;                  // uniq CIZI    N   30
        long unsigned int   DPM_ID;                 // uniq CIZI    N   30
        std::string         PARAMETRY_SPOJENI;      //              N   T100
        long unsigned int   ZVB_ID;                 //      CIZI    A   30

        void                pr();                   // printer

};

//&BHP;ID N30;STAV_DAT N2;DATUM_VZNIKU D;DATUM_ZANIKU D;PRIZNAK_KONTEXTU N1;RIZENI_ID_VZNIKU N30;RIZENI_ID_ZANIKU N30;TYPPPD_KOD N10;PAR_ID_1 N30;PAR_ID_2 N30

class LHP         
//              LINE OF TABLE SOBR                 KEY          POV SIZE       (podle specifikace VFK)
{
public:
        long unsigned int   ID;                     // PRIMARY      A   30.0        !!!
        int                 STAV_DAT;               //              N   2
        std::string         DATUM_VZNIKU;           //              N   D
        std::string         DATUM_ZANIKU;           //              N   D
        int                 PRIZNAK_KONTEXTU;       //              N   1
        std::string         RIZENI_ID_VZNIKU;       //              N   30
        std::string         RIZENI_ID_ZANIKU;       //              N   30
        int                 TYPPPD_KOD;             //              A   10
        long unsigned int   PAR_ID_1;               //      CIZI    A   30
        long unsigned int   PAR_ID_2;               //              N   30

        void                pr();                   // printer

};

class LOP        
//              LINE OF TABLE SOBR                 KEY          POV SIZE       (podle specifikace VFK)
{
public:
        long unsigned int   ID;                     // PRIMARY      A   30.0        !!!
        int                 STAV_DAT;               //              N   2
        std::string         DATUM_VZNIKU;           //              N   D
        std::string         DATUM_ZANIKU;           //              N   D
        int                 PRIZNAK_KONTEXTU;       //              N   1
        std::string         RIZENI_ID_VZNIKU;       //              N   30
        std::string         RIZENI_ID_ZANIKU;       //              N   30
        int                 TYPPPD_KOD;             //              A   10
        double              SOURADNICE_Y;           //              N   10.2
        double              SOURADNICE_X;           //              N   10.2
        std::string         TEXT;                   //              N   VARCHAR2 255
        double              VELIKOST;               //              N   10.2
        double              UHEL;                   //              N   10.2
        long unsigned int   PAR_ID;                 //      CIZI    A   30
        std::string         OPAR_TYPE;              //              A   VARCHAR2 10
        int                 VZTAZNY_BOD;            //              N   2

        void                pr();                   // printer

};

class LOB        
//              LINE OF TABLE SOBR                 KEY          POV SIZE       (podle specifikace VFK)
{
public:
        long unsigned int   ID;                     // PRIMARY      A   30.0        !!!
        int                 STAV_DAT;               //              N   2
        std::string         DATUM_VZNIKU;           //              N   D
        std::string         DATUM_ZANIKU;           //              N   D
        int                 PRIZNAK_KONTEXTU;       //              N   1
        std::string         RIZENI_ID_VZNIKU;       //              N   30
        std::string         RIZENI_ID_ZANIKU;       //              N   30
        int                 TYPPPD_KOD;             //              A   10
        double              SOURADNICE_Y;           //              N   10.2
        double              SOURADNICE_X;           //              N   10.2
        double              VELIKOST;               //              N   10.2
        double              UHEL;                   //              N   10.4
        long unsigned int   BUD_ID;                 //      CIZI    A   30
        std::string         OBRBUD_TYPE;            //              A   VARCHAR2 10

        void                pr();                   // printer

};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class TSOBR
{
public:
        std::vector<LSOBR>   data;

        void        pr();
        void        status();

        void        import_line(std::string li);
//        static const int wd = 13;
        static const char BSTRING[];

        TYP_idx     find_point_by_id(TYP_id pt);
        VEC_idx     select_by_rectangle(const VEC_idx arr, Rect re);
        VEC_idx     select_by_rectangle(Rect re);
};

class TSPOL
{
public:
        std::vector<LSPOL>   data;

        void        pr();
        void        status();

        void        import_line(std::string li);
//        static const int wd = 13;
        static const char BSTRING[];
};

class TSBP
{
public:
        std::vector<LSBP>   data;

        void        pr();
        void        status();

        void        import_line(std::string li);
//        static const int wd = 13;
        static const char BSTRING[];
        int         find_sources_of_hplist(VEC_id hp_ids, VEC_idx &sbp_idx, VEC_idx &sbp_12, VEC_id &sbp_XY_id, VEC_idx &whathp_idx, VEC_idx &counts);
        int         find_sources_of_oblist(VEC_id ob_ids, VEC_idx &sbp_idx, VEC_idx &sbp_12, VEC_id &sbp_XY_id, VEC_idx &whatob_idx, VEC_idx &counts);
};

class THP
{
public:
        std::vector<LHP>   data;

        void        pr();
        void        status();

        void        import_line(std::string li);
//        static const int wd = 13;
        static const char BSTRING[];
        int         find_par_id(TYP_id source_id, std::vector<int> &indi, VEC_id &par_ids);
};
class TOP
{
public:
        std::vector<LOP>   data;

        void        pr();
        void        status();

        void        import_line(std::string li);
//        static const int wd = 13;
        static const char BSTRING[];
        
        TYP_idx     find_parcela(std::string s, int st);    //stavebni: st=1, vraci idx
        std::string get_parcela_name(TYP_id pc, int &idx, int &typ);
        int         uniquize_list(VEC_s &par_cis, VEC_idx &par_idx, VEC_idx &typppd);
        void        print_neighbors(const VEC_idx indi_op, const VEC_id par_ids, const VEC_s par_cisla, const VEC_idx par_typppd);
};

class TOB
{
public:
        std::vector<LOB>   data;

        void        pr();
    //    void        status();

        void        import_line(std::string li);
//        static const int wd = 13;
      //  static const char BSTRING[];
        VEC_idx     select_by_rectangle(const VEC_idx arr, Rect re);
        VEC_idx     select_by_rectangle(Rect re);
        int         find_linesegments_by_id(TYP_id bud_id, VEC_idx &ob_idx, VEC_id &ob_ids, std::string obtyp = "\"OB\"");
};

                                      
class TDatabase
{
public:
        TSOBR *p_sobr;
        TSPOL *p_spol;
        TSBP *p_sbp;
        THP *p_hp;
        TOP *p_op;
        TOB *p_ob;
        
        TDatabase() : p_sobr(NULL),p_spol(NULL),p_sbp(NULL),p_hp(NULL),p_op(NULL),p_ob(NULL) {};
        void info();
        void clear();
        int find_parcela_neighbors(std::string s, int st,  VEC_idx &indi_hp, VEC_idx &indi_op, VEC_id &par_ids, VEC_s &par_cisla, VEC_idx &typppd);
        int parcela_coord(std::string s, int st,  BoundaryList &L);
        int budova_coord (std::string s, int st,  BoundaryList &L);
        void print_parcela_coord(const BoundaryList L) const;
        void print_budova_coord (const BoundaryList L) const;
        int hledej_budovu (std::string s, int st, BoundaryList &L, int poradove_cislo_budovy_od_nuly = 0);
};




//================== PROTOTYPY =================================================================================================================

void load_file(std::istream& in, TDatabase &D);
std::string get_opcode_str(const std::string s);
std::string quote_string(const char *s);
std::string quote_string(const std::string s);
std::string dequote_string(const std::string s);




class CoordList
{
public:
    double          X;
    double          Y;
    int             CISLO;
    int             UPLNE_CISLO;
    int             KODCH;
    std::string     PARAM;
    std::string     METADATA;
    
//    void            pr() const;
//    void            prnxy() const;
};


class CoordSet : public std::vector<CoordList>
{
public:
};











