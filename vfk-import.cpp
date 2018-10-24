
#include "vfk-headers.h"


#include "runtime.cpp"
#include "safestring.h"


// // g++ my_xpath_select.cpp pugixml.cpp -o z
// // ./z -quiet -addpc 284 -addpc "13/1" -addpc "st. 6/1" -expoo -status > exkuk.m
// // ./z -quiet -ku 618225.xml -addlist "st. 31/1" "st. 32" 526 525 456/8 "st. 28/2" 456/11 528 "st. 65" 456/5 456/9 "st. 33" "st. 34" 11/3 11/4 -expoo > plancach.m


//  VFK importer pro SOBR, ?SPOl, ...

//  potrebuji nacist SOBR, SPOL, SBP, HP, OP

//  hlavni objekty:  tabulka obrazu bodu s pevnym poradim sloupcu


//  HELP:

//          -buset_pr   N           N je styl tisku: 0 sirsi, 1 uzky
//          -bdset_pr   N           
//          -bdlist                 tiskne vzdy stylem 0
//          -bulist

/*  NEUTOPIT SE VE SNAZE O ROBUSTNOST. PROMENNE PORADI V TABULKACH A PROMENNE FORMATY SLOUPCU JSOU AKADEMICKY PROBLEM,
 *  KTERY MOZNA ANI NEEXISTUJE. STACI OTESTOVAT SHODNOST FORMATOVEHO RADKU.
 */

//#include "pugixml.hpp"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#define         dequote(A)      A






//int TDatabase::find_parcela_neighbors(std::string s, int st, VEC_idx &indi_hp, VEC_idx &indi_op, VEC_id &par_ids, VEC_s &par_cisla, VEC_idx &typppd)



VEC_idx  TSOBR::select_by_rectangle(const VEC_idx arr, Rect re)
{
    VEC_idx selected;
    for(auto it = arr.begin(); it != arr.end(); ++it)
    {
        if ( re.is_inside(data[*it].SOURADNICE_X, data[*it].SOURADNICE_Y) )
            selected.push_back(*it);
    }
    return selected;
}

VEC_idx  TSOBR::select_by_rectangle(Rect re)
{
    VEC_idx selected;
    for(auto it = data.begin(); it != data.end(); ++it)
    {
        if ( re.is_inside( it->SOURADNICE_X, it->SOURADNICE_Y) )
            selected.push_back( it - data.begin() );
    }
    return selected;
}

VEC_idx  TOB::select_by_rectangle(const VEC_idx arr, Rect re)
{
    VEC_idx selected;
    for(auto it = arr.begin(); it != arr.end(); ++it)
    {
        if ( re.is_inside(data[*it].SOURADNICE_X, data[*it].SOURADNICE_Y) )
            selected.push_back(*it);
    }
    return selected;
}

VEC_idx  TOB::select_by_rectangle(Rect re)
{
    VEC_idx selected;
    for(auto it = data.begin(); it != data.end(); ++it)
    {
        if ( re.is_inside( it->SOURADNICE_X, it->SOURADNICE_Y) )
            selected.push_back( it - data.begin() );
    }
    return selected;
}



// RETURN VALUE:  pocet nalez budov
//                -1 nenalezena

// TODO  prislusnot k parcele urcuje zatim jen pomoci boundaryBox

int TDatabase::hledej_budovu(std::string s, int st, BoundaryList &L, int poradove_cislo_budovy_od_nuly)  // TYP_id pc_id
{
    BoundaryList bdL;
    int res = parcela_coord(s, st, bdL);
    if ( res < 0 ) return -1;
    
//    bdL.boundaryBox().pr();
    
    VEC_idx pt = p_ob->select_by_rectangle(bdL.boundaryBox());
    
    if (pt.size() == 0) 
    {
        std::cout << FRED("Budova nenalezena.\n"); 
        return -1;
    }
        
    for(auto it = pt.begin(); it != pt.end(); ++it) p_ob-> data[*it].pr();
 
    if (pt.size() > 1)
    {
        std::cout << FRED("Na parcele nalezeno vice budov: "<<pt.size()<<". Pokracuji s idx=" <<poradove_cislo_budovy_od_nuly<<".\n"); 
    }
    
    int pt_idx = poradove_cislo_budovy_od_nuly;
    
    if (pt_idx < pt.size())
    {
        TYP_id bud_Id = p_ob -> data[pt[pt_idx]].BUD_ID; 
        VEC_idx indi_ob;
        VEC_id  ob_ids;
        res = p_ob -> find_linesegments_by_id(bud_Id, indi_ob, ob_ids);

        std::cout << "Budova id = " << bud_Id << "#No." << pt_idx+1 << "\n";

        std::cout << "B: res=" << res << "\n";

        // krok C       lookup SBP(exists in hplist): najdu seznam segmentu Li
    
        VEC_idx  sbp_idx;
        VEC_idx  sbp_12;
        VEC_id   sbp_XY_ids;
        VEC_idx  whathp_idx;
        VEC_idx  hp_counts;
 
        res = p_sbp -> find_sources_of_oblist(ob_ids, sbp_idx, sbp_12, sbp_XY_ids, whathp_idx, hp_counts);
    
        std::cout << "C: res=" << res << "\n";

        // krok D        lookup SOBR(forall Li)
        VEC_idx body_idx;
        for(int k = 0; k < sbp_idx.size(); ++k) 
        {
            int bod_idx;
            bod_idx = p_sobr -> find_point_by_id(sbp_XY_ids[k]);
                
            if (bod_idx >= 0) body_idx.push_back(bod_idx);
            else return 0;
        
//            std::cout << "X " << p_sobr -> data[bod_idx].ID <<  "     " << p_sobr -> data[bod_idx].UPLNE_CISLO << "  " << p_sbp -> data[sbp_idx[k]].PORADOVE_CISLO_BODU << "  " << whathp_idx[k] 
//                  << "    " << p_sobr -> data[bod_idx].SOURADNICE_X << "    " << p_sobr -> data[bod_idx].SOURADNICE_Y << "\n";

        }

    
        // krok E - slozeni
 
        
        ////////////////////////////////////////////////////////////////////////////////////
    
    VEC_idx  idx_of_block(ob_ids.size()+1);
    for (int k=0; k<ob_ids.size();k++) std::cout << hp_counts[k] << "  ";
    for (int k=0; k<ob_ids.size();k++) if (hp_counts[k] <2) {std::cout << "Integrity check: num of hp_counts[" << k << "] = " << hp_counts[k] << "\n"; hp_counts[k] = 1; }  // 1 means no change, safe value
    idx_of_block[0] = 0;
    for (int k=1; k<ob_ids.size()+1; k++) 
    {
        idx_of_block[k] = idx_of_block[k-1] + hp_counts[k-1] - 1;     // cumulative sum:  first index of block , back() is total count       
//       DEBPR("\nib", idx_of_block[k])
     }   

    
    L.resize(idx_of_block.back());

    
    for(int k = 0; k < sbp_idx.size(); ++k) 
    {
        int whp     = whathp_idx[k];
        int sobridx  = body_idx[k];
        LSBP *psbp    = &( p_sbp  -> data[ sbp_idx[k]] );
        LOB  *php     = &( p_ob   -> data[ indi_ob[whp]] );
        LSOBR *psobr  = &( p_sobr -> data[ sobridx ] );
        int por = psbp -> PORADOVE_CISLO_BODU;
        
        int addr = idx_of_block[whp] + por-1;
/* 
        DEBPR("\nk", k)
        DEBPR("whp", whp)
        DEBPR("por", por)
        DEBPR("sobridx", sobridx)
        DEBPR("Lsi", L.size())
        
        // integrity check TODO
//        DEBPR("addr", addr)
*/
        if ( por < hp_counts[whp] ) {                                     /// not last por
            L[addr].HP_ID  = ob_ids[whp];
            L[addr].PAR_ID_1  = bud_Id;
            L[addr].PAR_ID_2  = bud_Id;
            L[addr].X1  = psobr -> SOURADNICE_X;
            L[addr].Y1  = psobr -> SOURADNICE_Y;
            L[addr].CISLO1        = psobr -> CISLO_BODU;
            L[addr].UPLNE_CISLO1  = psobr -> UPLNE_CISLO;
            L[addr].KODCH1        = psobr -> KOD_CHB;

        }
        if ( por > 1 ) {                                     /// not first  - koncovky
            addr--;
            L[addr].X2  = psobr -> SOURADNICE_X;
            L[addr].Y2  = psobr -> SOURADNICE_Y;
            L[addr].CISLO2        = psobr -> CISLO_BODU;
            L[addr].UPLNE_CISLO2  = psobr -> UPLNE_CISLO;
            L[addr].KODCH2        = psobr -> KOD_CHB;
        }

 //       L[addr].prnxy();
    }   

        ////////////////////////////////////////////////////////////////////////////////////

    
        L.cyclize();
        
        L.METADATA = "%% Budova  #"+ std::to_string(pt_idx+1) + " na parcele " + dequote_string(s) + (st ? " (st.)":" (poz.)")  + " id=" + std::to_string(bud_Id);
        L.METADATA2 = "%% Bud. #"+ std::to_string(pt_idx+1) + " - " + dequote_string(s) + (st ? " st.":" poz.");
    
        L.TEXT  = p_ob -> data[pt[0]].OBRBUD_TYPE;
        L.REF_X = p_ob -> data[pt[0]].SOURADNICE_X;
        L.REF_Y = p_ob -> data[pt[0]].SOURADNICE_Y;
        L.REF_U = p_ob -> data[pt[0]].UHEL;
    

    }     //end if

    return pt.size();      // pocet budov
}


class Context
{
public:
        BoundaryList bdList;
        BoundarySet bdSet;
        BoundaryList buList;
        BoundarySet buSet;
        BoundaryList coordList;
        BoundarySet coordSet;
        Rect BBox;
        
        Rect reg_bbox[10];
        BoundaryList reg_bd[10];
        BoundaryList reg_bu[10];
        
        Context();
        ~Context();
        std::string fname_export;
        
        std::streambuf  *cout_sbuf;
        std::ofstream   txxfile;
        
        
        void export_octave();
        void run_octave();
        void file_redirect(std::string fn);        
        void file_cout();
        
};

Context::Context()
{
    fname_export = "out.m";
    cout_sbuf = std::cout.rdbuf();
    //TODO inic pole reg
}

Context::~Context()
{
    file_cout();
}


void Context::file_redirect(std::string fn)
{
    txxfile.open( (fn + ".txx").c_str() );  // radeji ne jako automatickou promennou
    if (! txxfile.good()) {std::cerr << "Error opening/redirect file " << fn <<"\n"; return;}
    //cout_sbuf = cout.rdbuf();           in constructor
    std::cout.rdbuf( txxfile.rdbuf() );
}

void Context::file_cout()
{
    if (txxfile.good()) txxfile.close();      
    std::cout.rdbuf( cout_sbuf );
}

void Context::export_octave()
{
     
    std::ostream &coux = std::cout;

    std::ofstream exportfile ((fname_export + ".m").c_str());
    
    std::streambuf  *sbuf = coux.rdbuf();           // redirectiong standard output for a while
    
    coux.rdbuf( exportfile.rdbuf() );
    
    int no_bu = buSet.size();
    int no_bd = bdSet.size();
    int no_co = coordSet.size();
    
    coux << std::fixed <<
            "%%%%%%%%%%%%%%  E X P O R T    A L L  %%%%%%%%%%%%%%%%%%%%%%\n"
            "close all, clear all\n"
            "fsize = 10;\n"
            "parclabelsize = 14;\n"
            "build_color = [1 0.9 0.9];\n"
            "lw_pc = 0.8;\n"
            "lw_bu = 0.2;\n"
            "no_bd = " << no_bd << ";\n"
            "no_bu = " << no_bu << ";\n"
            "no_co = " << no_co << ";\n"
            "\n";
    coux << "%% No of boundary: " << std::left << no_bd << '\n' <<
            "%% No of building: " << std::left << no_bu << '\n' <<
            "%% No of coordset: " << std::left << no_co << '\n' <<
            "\n";

            
    coux << "figure;\n"
            "hold on\n"
            "set(gca,'box','on')\n";
    std::string varname = "";
    for (int k = 0; k < no_bu; k++)
    {
        coux << "%%" << buSet[k].METADATA << "\n";
        if (buSet[k].TEXT != "") 
        {
            coux << "REF_BUD_YX{" << k+1 << "} = [ " << buSet[k].REF_Y << ", " << buSet[k].REF_X << ", " << buSet[k].REF_U << "];     %% REF POINT BUDOVY\n";
            coux << "BUD_TEXT{" << k+1 << "} = " << buSet[k].TEXT << "; %% used only for existence\n";
        }
        varname = "BU{" + std::to_string(k+1) + "}";
        buSet[k].bare_export(varname);
        coux << "%%--------------------------------------------------------\n";
    }
    for (int k = 0; k < no_bd; k++)
    {
        coux << "%%" << bdSet[k].METADATA << "\n";
        if (bdSet[k].TEXT != "") 
        {
            coux << "PARC_CIS_YX{" << k+1 << "} = [ " << bdSet[k].REF_Y << ", " << bdSet[k].REF_X << ", " << bdSet[k].REF_U << "];     %% REF POINT PARCELNIHO CISLA\n";
            coux << "PARC_CIS_TEXT{" << k+1 << "} = " << bdSet[k].TEXT << ";\n";
        }
        varname = "BD{" + std::to_string(k+1) + "}";
        bdSet[k].bare_export(varname);
        coux << "%%--------------------------------------------------------\n";
    }
    
    if(no_bu > 0)
    {
        coux << "for k = 1:no_bu\n";
        coux << "   fill( -BU{k}(:,2), -BU{k}(:,3), build_color, 'linewidth', lw_bu);\n";
//        coux << "   fill( -BU{k}(:,2), -BU{k}(:,3), 'y', 'linewidth', lw_bu);\n";
        coux << "   plot( -BU{k}(:,2), -BU{k}(:,3), 'linewidth', lw_bu);\n";
        coux << "end;\n";
    }
    if(no_bd > 0)
    {
        coux << "for k = 1:no_bd\n";
        coux << "   plot( -BD{k}(:,2), -BD{k}(:,3), 'linewidth', lw_pc);\n";
        coux << "end\n";
    }
    coux << "daspect([1 1 1]);\n";
    if(no_bu > 0)
    {
        coux << "for k = 1:no_bu\n";
        coux << "   for bod = 1 : (size(BU{k},1)-1)  %% last=first\n";
        coux << "      ht=text(-BU{k}(bod,2), -BU{k}(bod,3), sprintf('%3d', BU{k}(bod,1)), 'color', 'b', 'fontsize',fsize);\n";
        coux << "      set(ht, 'clipping', 'on');\n";
        coux << "   end\n";
        coux << "   if (~isempty(BUD_TEXT{k}))\n";
        coux << "      plot(-REF_BUD_YX{k}(1), -REF_BUD_YX{k}(2), '.k');\n";
        coux << "   end \n";
        coux << "end\n";
    }
    if(no_bd > 0)
    {
        coux << "for k = 1:no_bd\n";
        coux << "   for bod = 1 : (size(BD{k},1)-1)  %% last=first\n";
        coux << "      ht=text(-BD{k}(bod,2), -BD{k}(bod,3), sprintf('%3d', BD{k}(bod,1)), 'fontsize',fsize);\n";
        coux << "      set(ht, 'clipping', 'on');\n";
        coux << "   end\n";
        coux << "   if (~isempty(PARC_CIS_TEXT{k}))\n";
        coux << "      ht = text(-PARC_CIS_YX{k}(1), -PARC_CIS_YX{k}(2), PARC_CIS_TEXT{k}, 'fontsize',parclabelsize);\n";
        coux << "      set(ht, 'clipping', 'on');\n";
        coux << "      set(ht, 'rotation',  PARC_CIS_YX{k}(3));     %% does work e.g. with print -deps, but not at screen\n";
        coux << "      set(ht, 'fontweight', 'bold', 'color', 'g'); %%\n";
        coux << "      set(ht, 'horizontalalignment', 'center');\n";
        coux << "      set(ht, 'verticalalignment', 'baseline');\n";
        coux << "   end \n";
        coux << "end\n";
    }
    coux << "\n";
    coux << "set(gcf,'paperorientation','landscape')\n";
    coux << "%%set(gca,'ylim', [-1036305 -1036230])\n";
    coux << "%%set(gca,'ylim', [-1036350 -1036210])\n";
    Rect BBoxx = bdSet.boundaryBox().expand_round(2,5);
    {
        coux << "set(gca,'ylim', [" << -BBoxx.X2 << "  " << -BBoxx.X1 << "]);  %  computed BBox;\n";
        coux << "set(gca,'xlim', [" << -BBoxx.Y2 << "  " << -BBoxx.Y1 << "]);\n";
    }
    if (BBox.X1 < BBox.X2) 
    {
        coux << "set(gca,'ylim', [" << -BBox.X2 << "  " << -BBox.X1 << "]);  %   from context;\n";
        coux << "set(gca,'xlim', [" << -BBox.Y2 << "  " << -BBox.Y1 << "]);\n";
    }
    coux << "%%---------------------------------------------------\n";
    coux << "yticks = get (gca, 'ytick');\n"
                 "ylabels = arrayfun (@(x) sprintf ('%.0f', x), yticks, 'uniformoutput', false);\n"
                 "set (gca, 'yticklabel', ylabels)\n";
    coux << "\n";
    coux << "print -dpdfwrite -r300 -landscape -F:5 " << fname_export << ".pdf\n";

    coux << "\n";
    coux << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
        
    coux.rdbuf(sbuf);

    
}


// vzhledem k tomu, ze neindexuji primarni klice pro vyhledavani, 
// je potreba vzdy projit celou tabulku.
// to znamena, ze vyhledani primeho odkazu je stejne efektivni, jako zpetne hledani, lookup je vzdycky
// RETURN VALUE:
//      0    success
//      -1   parcel not found
//
// A: lookup OP("6/1"): najdu parid
// B: lookup HP(parid): najdu seznam hranice hplist
// C: lookup SBP(exists in hplist): najdu seznam segmentu Li
// D: lookup SOBR(forall Li)

int TDatabase::parcela_coord(std::string s, int st, BoundaryList &L)
{
    L.clear();
    int res;
    TYP_idx op_idx = p_op->find_parcela(s, st);    
    if (op_idx<0) return -1;
    TYP_id  op_id  = p_op->data[op_idx].PAR_ID;

    VEC_idx indi_hp;
    VEC_id  par_ids;
    res = p_hp -> find_par_id(op_id, indi_hp, par_ids);
    std::cout << "B: res=" << res << "\n";

    VEC_id hp_ids;
    for(auto it = indi_hp.begin(); it != indi_hp.end(); ++it) hp_ids.push_back(p_hp -> data[*it].ID);

    // krok C       lookup SBP(exists in hplist): najdu seznam segmentu Li
    
    VEC_idx  sbp_idx;
    VEC_idx  sbp_12;
    VEC_id   sbp_XY_ids;
    VEC_idx  whathp_idx;
    VEC_idx  hp_counts;
    
    res = p_sbp -> find_sources_of_hplist(hp_ids, sbp_idx, sbp_12, sbp_XY_ids, whathp_idx, hp_counts);
    
    std::cout << "C: res=" << res << "\n";

    // std::cout << hp_counts[k] << "  ";
    // krok D        lookup SOBR(forall Li)
    VEC_idx body_idx;
    for(int k = 0; k < sbp_idx.size(); ++k) 
    {
        int bod_idx;
        bod_idx = p_sobr -> find_point_by_id(sbp_XY_ids[k]);
            
        if (bod_idx >= 0) body_idx.push_back(bod_idx);
        else return 0;
        
  //      std::cout << "X " << p_sobr -> data[bod_idx].ID <<  "     " << p_sobr -> data[bod_idx].UPLNE_CISLO << "  " << p_sbp -> data[sbp_idx[k]].PORADOVE_CISLO_BODU << "  " << whathp_idx[k] 
  //                << "    " << p_sobr -> data[bod_idx].SOURADNICE_X << "    " << p_sobr -> data[bod_idx].SOURADNICE_Y << "\n";

    }
    
    // TODO     vyhledat chybejici kody kvality ve SPOL
    

    
    // krok E - slozeni
    
    VEC_idx  idx_of_block(hp_ids.size()+1);
    std::cout << "Pocty hranicnich usecek v retezcich:   "; for (int k=0; k<hp_ids.size();k++) std::cout << hp_counts[k] << "  "; std::cout << '\n';
    for (int k=0; k<hp_ids.size();k++) if (hp_counts[k] <2) {std::cout << "Integrity check: num of hp_counts[" << k << "] = " << hp_counts[k] << "\n"; hp_counts[k] = 1; }  // 1 means no change, safe value
    idx_of_block[0] = 0;
    for (int k=1; k<hp_ids.size()+1; k++) 
    {
        idx_of_block[k] = idx_of_block[k-1] + hp_counts[k-1] - 1;     // cumulative sum:  first index of block , back() is total count       
     }   

    
    L.resize(idx_of_block.back());

    
// TODO jedna hranice HP muze mit vic poradovych cisel, L.resize ne podle hp.size, ale podle res.sbp , .... cyklus pres poradi?

                                                                                                    // ted to je tak, ze:
                                                                                                    //      1 je vzdy pocatecni
                                                                                                    //      2 je vzdy koncovy bod usecky ... v podstate jen trideni,  kompletuji usecky / jde to jinak?
                                                                                                    //
                                                                                                    //      body ale nemuseji jit za sebou ve spravnem poradi
                                                                                                    //
                                                                                                    //      mezibody se vyskytnou vlevo i vpravo
                                                                                                    //

                                                                                                    //      cyklus pres hp // cyklus-while pres poradi, vyhledat por=i, por=i+1
                                                                                                    //      nakonec jinak ... spocitam cilovou adresu a ulozim na misto
                                                                                                    

    for(int k = 0; k < sbp_idx.size(); ++k) 
    {
        int whp     = whathp_idx[k];
        int sobridx  = body_idx[k];
        LSBP *psbp    = &( p_sbp  -> data[ sbp_idx[k]] );
        LHP  *php     = &( p_hp   -> data[ indi_hp[whp]] );
        LSOBR *psobr  = &( p_sobr -> data[ sobridx ] );
        int por = psbp -> PORADOVE_CISLO_BODU;
        
        int addr = idx_of_block[whp] + por-1;
/* 
        DEBPR("\nk", k)
        DEBPR("whp", whp)
        DEBPR("por", por)
        DEBPR("sobridx", sobridx)
        DEBPR("Lsi", L.size())
        
        // integrity check TODO
//        DEBPR("addr", addr)
*/
        if ( por < hp_counts[whp] ) {                                     /// not last por
            L[addr].HP_ID  = hp_ids[whp];
            L[addr].PAR_ID_1  = php -> PAR_ID_1;
            L[addr].PAR_ID_2  = php -> PAR_ID_2;
            L[addr].X1  = psobr -> SOURADNICE_X;
            L[addr].Y1  = psobr -> SOURADNICE_Y;
            L[addr].CISLO1        = psobr -> CISLO_BODU;
            L[addr].UPLNE_CISLO1  = psobr -> UPLNE_CISLO;
            L[addr].KODCH1        = psobr -> KOD_CHB;
        }
        if ( por > 1 ) {                                     /// not first  - koncovky
            addr--;
            L[addr].X2  = psobr -> SOURADNICE_X;
            L[addr].Y2  = psobr -> SOURADNICE_Y;
            L[addr].CISLO2        = psobr -> CISLO_BODU;
            L[addr].UPLNE_CISLO2  = psobr -> UPLNE_CISLO;
            L[addr].KODCH2        = psobr -> KOD_CHB;
        }

 //       L[addr].prnxy();
    }   
     
 
 
    L.cyclize();
    L.METADATA = "%% Hranice parcely  " + dequote_string(s) + ((st) ? " (st.)  ": " (poz.) ") + " id=" + std::to_string(op_id);
    L.METADATA2 = "%% Parcela  " + dequote_string(s) + ((st) ? " (st.)  ": " (poz.) ");
    L.TEXT = p_op -> data[op_idx].TEXT;
    L.REF_X = p_op -> data[op_idx].SOURADNICE_X;
    L.REF_Y = p_op -> data[op_idx].SOURADNICE_Y;
    L.REF_U = p_op -> data[op_idx].UHEL;
    
    return 0;
}
    

void Context::run_octave()
{
    std::cout << ("Running octave...\n");
    std::string cmd = "octave ";
    cmd += fname_export + ".m";
    std::system(cmd.c_str());
    return;
}
    
// A: lookup OP("6/1"): najdu parid
// B: lookup HP(parid): najdu seznam hranice hplist
// C: lookup SBP(exists in hplist): najdu seznam segmentu Li
// D: lookup SOBR(forall Li)

int TDatabase::budova_coord(std::string s, int st, BoundaryList &L)
{
    L.clear();
    int res;
    TYP_idx op_idx = p_op->find_parcela(s, st);    
    if (op_idx<0) return 0;
    TYP_id  op_id  = p_op->data[op_idx].PAR_ID;

    VEC_idx indi_hp;
    VEC_id  par_ids;
    res = p_hp -> find_par_id(op_id, indi_hp, par_ids);
    std::cout << "B: res=" << res << "\n";

    VEC_id hp_ids;
    for(auto it = indi_hp.begin(); it != indi_hp.end(); ++it) hp_ids.push_back(p_hp -> data[*it].ID);

    // krok C       lookup SBP(exists in hplist): najdu seznam segmentu Li
    
    VEC_idx  sbp_idx;
    VEC_idx  sbp_12;
    VEC_id   sbp_XY_ids;
    VEC_idx  whathp_idx;
    VEC_idx  hp_counts;
  
    res = p_sbp -> find_sources_of_hplist(hp_ids, sbp_idx, sbp_12, sbp_XY_ids, whathp_idx, hp_counts);
    
    std::cout << "C: res=" << res << "\n";
    
    for (int k=0; k<hp_ids.size();k++) std::cout << hp_counts[k] << "  ";

    // krok D        lookup SOBR(forall Li)
    VEC_idx body_idx;
    for(int k = 0; k < sbp_idx.size(); ++k) 
    {
        int bod_idx;
        bod_idx = p_sobr -> find_point_by_id(sbp_XY_ids[k]);
            
        if (bod_idx >= 0) body_idx.push_back(bod_idx);
        else return 0;
        
//        std::cout << "X " << p_sobr -> data[bod_idx].ID <<  "     " << p_sobr -> data[bod_idx].UPLNE_CISLO << "  " << p_sbp -> data[sbp_idx[k]].PORADOVE_CISLO_BODU << "  " << whathp_idx[k] 
//                  << "    " << p_sobr -> data[bod_idx].SOURADNICE_X << "    " << p_sobr -> data[bod_idx].SOURADNICE_Y << "\n";

    }
    
    // krok E - slozeni
    L.resize(hp_ids.size());
    for(int k = 0; k < indi_hp.size(); ++k) 
    {
        L[k].HP_ID  = hp_ids[k];
        L[k].PAR_ID_1  = p_hp -> data[indi_hp[k]].PAR_ID_1;
        L[k].PAR_ID_2  = p_hp -> data[indi_hp[k]].PAR_ID_2;
    }


    for(int k = 0; k < sbp_idx.size(); ++k) 
    {
        if (p_sbp -> data[sbp_idx[k]].PORADOVE_CISLO_BODU == 1)
        {
            L[whathp_idx[k]].X1  = p_sobr -> data[body_idx[k]].SOURADNICE_X;
            L[whathp_idx[k]].Y1  = p_sobr -> data[body_idx[k]].SOURADNICE_Y;
            L[whathp_idx[k]].CISLO1  = p_sobr -> data[body_idx[k]].CISLO_BODU;
            L[whathp_idx[k]].UPLNE_CISLO1  = p_sobr -> data[body_idx[k]].UPLNE_CISLO;
            L[whathp_idx[k]].KODCH1  = p_sobr -> data[body_idx[k]].KOD_CHB;
            //TODO kodchar
        }
        else if (p_sbp -> data[sbp_idx[k]].PORADOVE_CISLO_BODU == 2)
        {
            L[whathp_idx[k]].X2  = p_sobr -> data[body_idx[k]].SOURADNICE_X;
            L[whathp_idx[k]].Y2  = p_sobr -> data[body_idx[k]].SOURADNICE_Y;
            L[whathp_idx[k]].CISLO2  = p_sobr -> data[body_idx[k]].CISLO_BODU;
            L[whathp_idx[k]].UPLNE_CISLO2  = p_sobr -> data[body_idx[k]].UPLNE_CISLO;
            L[whathp_idx[k]].KODCH2  = p_sobr -> data[body_idx[k]].KOD_CHB;
        } else std::cout << FRED("COORD: Chyba dichotomie.\n");

        
    }
    
    
    L.cyclize();
    
    return 0;
}
    

typedef enum {
        // implemented
        CM_NOP, CM_PARC_NEIGHBORS, CM_KU, CM_KUDEFAULT, CM_INFO, CM_CLEAR, CM_PARC_COORD, CM_COORD_MPRINT, CM_BUD_COORD, CM_BUD_MPRINT, CM_BATCH, CM_ENDBATCH, CM_FIND_BUILDING,
        CM_BDLIST_PUSH, CM_BULIST_PUSH, CM_COORDLIST_PUSH,  CM_BDLIST_CLEAR, CM_BDSET_CLEAR, CM_BDLIST_PR, CM_BDSET_PR,CM_BULIST_PR, CM_BUSET_PR,
        CM_BULIST_CLEAR, CM_BUSET_CLEAR, CM_MATLAB_EXPORT, CM_FNAME_EXPORT, CM_RUN_OCTAVE,
        CM_BDLIST_POKE, CM_BDLIST_PEEK, CM_BULIST_POKE, CM_BULIST_PEEK, CM_BBOX_POKE, CM_BBOX_PEEK, 
        CM_BBOX_PR, CM_BBOX_EXPAND, CM_BBOX_LOAD, 
        CM_FILE_REDIRECT, CM_FILE_COUT, 
        // not implemented
         CM_EXP, CM_EXPOO, CM_ADDL,  CM_ADDPC, CM_GO, CM_QUI, CM_VERB, CM_DBGA, CM_HLP, CM_STAT} TCMD;



void help(std::map<std::string, TCMD> cmdcode, std::map<TCMD,int> cmd_argc)
{
    std::cout << std::setw(15) << std:: left << "COMMAND" << "  " << "POCET_ARG" << "    " << "OPCODE" << "\n";
    for (std::map<std::string, TCMD>::iterator it = cmdcode.begin(); it != cmdcode.end(); it++)
        std::cout << std::setw(15) << std:: left << it->first << "  " << cmd_argc[it->second] << "            " << it->second << "\n";

}



        
int main(int argc, char **argv)
{
        TDatabase DD;
        TSOBR    T;
        TSBP    T_SBP;
        
        TSPOL   T_SPOL;
        THP     T_HP;
        TOP     T_OP;
        TOB     T_OB;

        DD.p_sobr = &T;
        DD.p_spol = &T_SPOL;
        DD.p_sbp = &T_SBP;
        DD.p_hp = &T_HP;
        DD.p_op = &T_OP;
        DD.p_ob = &T_OB;
        
        Context ct;
        
        std::cout << std::fixed << std::setprecision(2) << std::right;

        if (0)      // first test
        {
            TDatabase DD;
            TSOBR    T;
            DD.p_sobr = &T;
               
            LSOBR    testLSOBR = {12345678901234, 56, 123456, 54321, 4321, 1234567890123, 987654321012, 1234567890.12, 9876543210.12, 34};
        
            T.data.push_back(testLSOBR);
        
            T.status();
        
            testLSOBR.pr();
        
            std::string sampleline = "&DSOBR;5840312209;0;658499;286;;6;2860006;728920.22;1037089.6;8";
        
            T.import_line(sampleline);
        }    
        

        DD.clear();
        std::cout << "#########################  M A I N   L O O P  ##################################################" << "\n";

        
        Runtime rt;
        rt.init(argc,argv);

        //  DEFAULT FILE
        std::ifstream vfk_instream ("724149-mapa.vfk");

        if (!vfk_instream.good()) 
        { 
            std::cout << FRED("Error opening file. \n");
            
        }
        else
          load_file(vfk_instream, DD);
        DD.info();


    std::map<std::string, TCMD> cmdcode = { 
        // implemented
        {"-neighbors",  CM_PARC_NEIGHBORS},  {"-k", CM_KUDEFAULT},  {"-ku", CM_KU},  {"-clear", CM_CLEAR}, {"-info", CM_INFO}, {"-debug_arg", CM_DBGA}, 
        {"-parcel_coord", CM_PARC_COORD}, {"-coord_mprint", CM_COORD_MPRINT},
        {"-batch", CM_BATCH}, {"-end_batch", CM_ENDBATCH},
        {"-budova_coord", CM_BUD_COORD},  {"-bu", CM_FIND_BUILDING}, {"-budova_mprint", CM_BUD_MPRINT},
        {"-matlab_export", CM_MATLAB_EXPORT}, 
        {"-bdlist_push", CM_BDLIST_PUSH}, {"-bulist_push", CM_BULIST_PUSH}, {"-coordlist_push", CM_COORDLIST_PUSH},  
        {"-bdlist_clear", CM_BDLIST_CLEAR}, {"-bulist_clear", CM_BULIST_CLEAR}, {"-bdset_clear", CM_BDSET_CLEAR}, {"-buset_clear", CM_BUSET_CLEAR},
        {"-bd_poke", CM_BDLIST_POKE}, {"-bd_peek", CM_BDLIST_PEEK}, {"-bu_poke", CM_BULIST_POKE}, {"-bu_peek", CM_BULIST_PEEK}, {"-bbox_poke", CM_BBOX_POKE}, {"-bbox_peek", CM_BBOX_PEEK},
        {"-bbox_pr", CM_BBOX_PR}, {"-bbox_expand", CM_BBOX_EXPAND},
        {"-bdlist_pr", CM_BDLIST_PR}, {"-bdset_pr", CM_BDSET_PR}, {"-bulist_pr", CM_BULIST_PR}, {"-buset_pr", CM_BUSET_PR},
        {"-run_octave", CM_RUN_OCTAVE}, {"-bbox_load", CM_BBOX_LOAD}, 
         {"-f", CM_FNAME_EXPORT}, {"-file_redirect", CM_FILE_REDIRECT}, {"-file_cout", CM_FILE_COUT},
         {"-help", CM_HLP}, {"--help", CM_HLP}, 
        // not implemented
         {"-addlist",CM_ADDL},
        {"-addpc", CM_ADDPC}, {"-go", CM_GO}, {"-quiet", CM_QUI}, {"-verb", CM_VERB}, 
        {"-status", CM_STAT} };

    std::map<TCMD, int> cmd_argc = { 
        // implemented
        {CM_PARC_NEIGHBORS, 1}, {CM_KU,1}, {CM_KUDEFAULT,0}, {CM_INFO,0}, {CM_CLEAR, 0}, {CM_INFO, 0}, {CM_PARC_COORD, 2}, {CM_COORD_MPRINT, 1}, 
        {CM_BUD_COORD, 2}, {CM_BUD_MPRINT, 1}, {CM_FIND_BUILDING, 2}, {CM_BATCH, 1}, { CM_ENDBATCH, 0}, {CM_BDLIST_PUSH,0}, {CM_BULIST_PUSH,0}, {CM_COORDLIST_PUSH,0}, {CM_MATLAB_EXPORT,0},
        {CM_BDLIST_CLEAR,0}, {CM_BULIST_CLEAR,0}, {CM_BDSET_CLEAR,0}, {CM_BUSET_CLEAR,0},
        {CM_BDLIST_POKE,1}, {CM_BDLIST_PEEK,1}, {CM_BULIST_POKE,1}, {CM_BULIST_PEEK,1}, {CM_BBOX_POKE,1}, {CM_BBOX_PEEK,1}, 
        {CM_BBOX_PR,0}, {CM_BBOX_EXPAND,3},
        {CM_BDLIST_PR, 0}, {CM_BDSET_PR, 1},{CM_BDLIST_PR, 0}, {CM_BDSET_PR, 1},
        {CM_FNAME_EXPORT,1}, {CM_RUN_OCTAVE,0}, {CM_BBOX_LOAD,4}, {CM_FILE_REDIRECT, 1}, {CM_FILE_COUT, 0}
        // not implemented
    };
    
    int tmpint;
    double tmp_double[10];
    int tmp_int[10];
    int quiet = 0,
        debug_arg = 0;
    
    int arg_waiting = 0;
    for(int i = 0; i < argc; i++)
    {
        if (cmdcode[argv[i]] == CM_DBGA) debug_arg = 1;
        debug_arg && std::cout << std::left << std::setw(4) << i << ":  " << std::left << std::setw(12) << argv[i] << "  ";
        
        if (arg_waiting == 0) 
        {
            arg_waiting = cmd_argc[cmdcode[argv[i]]];
            debug_arg && std::cout << "OPCODE: " << cmdcode[argv[i]] << (cmdcode[argv[i]] ? "  OK" : "  N/A") << "\n";
        }
        else
        {
            debug_arg && std::cout << "ARG: " << arg_waiting << "\n";
            arg_waiting--;
        }
        if (cmdcode[argv[i]] == CM_QUI) quiet = 1;
    }
    debug_arg && std::cout << "\n";
    
//    int i = 1;
    //----------------------  context ----------------------------
    BoundaryList &glBdList = ct.bdList;
    BoundaryList &glBuList = ct.buList;
    //------------------ end of context --------------------------
    
    while( rt.go() )
    {

        TCMD current_cmd = cmdcode[rt.current()]; 
//        auto chkarg  = [&i,&rt.argc](int m) -> bool { if (i+m >= rt.argc) {std::cout << "Arg is missing.\n"; return 0;}; return 1;} ;
//        auto chkargc = [&i,&rt.argc,&cmd_argc, &current_cmd]() -> bool { if (i+cmd_argc[current_cmd] >= rt.argc) {std::cout << "CMD " << current_cmd << ": Arg is missing.\n"; return 0;}; return 1;} ;

        auto chkargc = [&rt,&cmd_argc, &current_cmd]() -> bool { if (!rt.check_next(cmd_argc[current_cmd])) {std::cout << "CMD " << current_cmd << ": Arg is missing.\n"; return 0;}; return 1;} ;

        VEC_idx tmp_indi_hp;
        VEC_idx tmp_indi_op;
        VEC_idx tmp_par_typppd;
        std::vector<long unsigned> tmp_par_ids;
        VEC_s tmp_par_cisla;
        int res;

        quiet || std::cout << rt.get_IP() << ":  " << rt.current() << "  \t";
        quiet || std::cout << "===== PROCESSING OPCODE: " << cmdcode[rt.current()] << (cmdcode[rt.current()] ? "  OK" : "  N/A") << "\n";

        switch(current_cmd)
        {
            case CM_PARC_NEIGHBORS:
                if (!chkargc()) break;
                tmpint = atoi(rt.next(2).c_str());
                res = DD.find_parcela_neighbors(quote_string(rt.next(1)), tmpint, tmp_indi_hp, tmp_indi_op, tmp_par_ids, tmp_par_cisla, tmp_par_typppd);
              std::cout << "-----------------  Neighbors:  ----------------\n";
              std::cout << "Neighbor count: " << res <<"\n";
              std::cout << "Neighbor HPindisize: " << tmp_indi_hp.size() <<"  OPindisize: " << tmp_indi_op.size() << "\n";

                DD.p_op -> print_neighbors(tmp_indi_op, tmp_par_ids, tmp_par_cisla, tmp_par_typppd);
                rt.go(cmd_argc[current_cmd]);
                break;
 
            case CM_KUDEFAULT:
                vfk_instream.close();
                vfk_instream.open("724149-mapa.vfk");
                if (!vfk_instream.good()) 
                { 
                    std::cout << FRED("Error opening default file. \n");
                }
                else
                    load_file(vfk_instream, DD);        
                DD.info();
                break;
                
            case CM_KU:
                if (!chkargc()) break;        
                vfk_instream.close();
                vfk_instream.open(rt.next(1));
                if (!vfk_instream.good()) 
                { 
                    std::cout << FRED("Error opening file '" << rt.next(1) << "'.\n");
                }
                else
                    load_file(vfk_instream, DD);
                DD.info();
                rt.go(cmd_argc[current_cmd]);
                break;

            case CM_CLEAR:
                DD.clear();
                DD.info();
                break;

            case CM_BDLIST_CLEAR:
                ct.bdList.clear();
                break;

            case CM_BULIST_CLEAR:
                ct.buList.clear();
                break;

            case CM_BDSET_CLEAR:
                ct.bdSet.clear();
                break;

            case CM_BUSET_CLEAR:
                ct.buSet.clear();
                break;
                
            case CM_INFO:
                DD.info();
                break;

           case CM_PARC_COORD:
                if (!chkargc()) break;
                tmpint = atoi(rt.next(2).c_str());
                res = DD.parcela_coord(quote_string(rt.next(1)), tmpint, glBdList);
                DD.print_parcela_coord(glBdList);
                glBdList.boundaryBox().pr();
                rt.go(cmd_argc[current_cmd]);
                break;
 
           case CM_COORD_MPRINT:
                if (!chkargc()) break;
                glBdList.mprint(rt.next(1));
                rt.go(cmd_argc[current_cmd]);
                break;

           case CM_FIND_BUILDING:
                if (!chkargc()) break;
                tmpint = atoi(rt.next(2).c_str());
                res = DD.hledej_budovu(quote_string(rt.next(1)), tmpint, glBuList);
                if (res>1)
                {
                    for (int tmpint2=1; tmpint2<res; tmpint2++ )
                    {
                        ct.buSet.push_back(glBuList);                                            // nesystemova nouzovka, kdyz je budov vic, TODO vracet bdSet a mergovat ??
                        std::cout << FRED("Automaticky push: glBuList-->glBuSet!  (Vice budov na parcele.)\n");
                        DD.hledej_budovu(quote_string(rt.next(1)), tmpint, glBuList, tmpint2);
                    }
                }
                rt.go(cmd_argc[current_cmd]);
                break;

//           case CM_PR:
///                if (!chkarg(1)) break;
      //          tmpint = atoi(rt.next(1).c_str());
//                if (tmpint < global_set.parc.size())
//                  global_set.parc[tmpint].pr();
//                else
  //                  std::cout << "Index " << tmpint << " of global_set not found [size=" <<  global_set.parc.size() << "].\n";
  //              rt.go(cmd_argc[current_cmd]);
    //            break;
            case CM_MATLAB_EXPORT:
                ct.export_octave();
                break;
                
            case CM_BDLIST_PUSH:
                if (ct.bdList.size() > 0)
                  ct.bdSet.push_back(ct.bdList);
                else std::cout << FRED("bdList push: list is empty!\n");
                break;

            case CM_BULIST_PUSH:
                if (ct.buList.size() > 0)
                ct.buSet.push_back(ct.buList);
                else std::cout << FRED("buList push: list is empty!\n");
                break;

            case CM_COORDLIST_PUSH:
                if (ct.coordList.size() > 0)
                ct.coordSet.push_back(ct.coordList);
                else std::cout << FRED("coordList push: list is empty!\n");
                break;

            case CM_BDLIST_POKE:
                if (!chkargc()) break;
                if (ct.bdList.size() > 0) { 
                  tmpint = safestoi(rt.next(1),0,1);
                  if (tmpint<0 || tmpint >= 10) {std::cout << FRED("bdList poke: address " << tmpint << " out of range!\n"); break; }
                  ct.reg_bd[tmpint] = ct.bdList;
                }
                else std::cout << FRED("bdList poke: list is empty!\n");
                rt.go(cmd_argc[current_cmd]);
                break;
            case CM_BDLIST_PEEK:
                if (!chkargc()) break;
                tmpint = safestoi(rt.next(1),0,1);  
                if (tmpint<0 || tmpint >= 10) {std::cout << FRED("bdList peek: address " << tmpint << " out of range!\n"); break; }
                if (ct.reg_bd[tmpint].size() > 0) { 
                    ct.bdList = ct.reg_bd[tmpint] ;
                }
                else std::cout << FRED("bdList peek: list is empty!\n");
                rt.go(cmd_argc[current_cmd]);
                break;
            case CM_BULIST_POKE:
                if (!chkargc()) break;
                if (ct.buList.size() > 0) { 
                  tmpint = safestoi(rt.next(1),0,1);  
                  if (tmpint<0 || tmpint >= 10) {std::cout << FRED("buList poke: address " << tmpint << " out of range!\n"); break; }
                  ct.reg_bu[tmpint] = ct.buList;
                }
                else std::cout << FRED("buList push: list is empty!\n");
                rt.go(cmd_argc[current_cmd]);
                break;
            case CM_BDLIST_PR:
                ct.bdList.pr( (ct.reg_bbox[9].X1 > 0) );
                break;
            case CM_BDSET_PR:
                if (!chkargc()) break;
                tmpint = safestoi(rt.next(1),0,1);  
                ct.bdSet.pr( tmpint );
                rt.go(cmd_argc[current_cmd]);
                break;
            case CM_BULIST_PR:
                ct.buList.pr( (ct.reg_bbox[9].X1 > 0) );
                break;
            case CM_BUSET_PR:
                if (!chkargc()) break;
                tmpint = safestoi(rt.next(1),0,1);  
                ct.buSet.pr( tmpint );
                rt.go(cmd_argc[current_cmd]);
                break;
            case CM_BULIST_PEEK:
                if (!chkargc()) break;
                tmpint = safestoi(rt.next(1),0,1);  
                if (tmpint<0 || tmpint >= 10) {std::cout << FRED("buList peek: address " << tmpint << " out of range!\n"); break; }
                if (ct.reg_bu[tmpint].size() > 0) { 
                    ct.buList = ct.reg_bu[tmpint] ;
                }
                else std::cout << FRED("buList peek: list is empty!\n");
                rt.go(cmd_argc[current_cmd]);
                break;
            case CM_BBOX_POKE:
                if (!chkargc()) break;
                if ( ct.BBox.ok() ) { 
                  tmpint = safestoi(rt.next(1),0,1);  
                  if (tmpint<0 || tmpint >= 10) {std::cout << FRED("bbox poke: address " << tmpint << " out of range!\n"); break; }
                  ct.reg_bbox[tmpint] = ct.BBox;
                }
                else std::cout << FRED("bbox poke: Rect is invalid!\n");
                rt.go(cmd_argc[current_cmd]);
                break;
            case CM_BBOX_PEEK:
                if (!chkargc()) break;
                tmpint = safestoi(rt.next(1),0,1);  
                if (tmpint<0 || tmpint >= 10) {std::cout << FRED("bbox peek: address " << tmpint << " out of range!\n"); break; }
                if (ct.reg_bbox[tmpint].ok() ) { 
                    ct.BBox = ct.reg_bbox[tmpint] ;
                }
                else std::cout << FRED("bbox peek: Rect is invalid!\n");
                rt.go(cmd_argc[current_cmd]);
                break;

            case CM_BBOX_PR:
                if (!chkargc()) break;
                if ( ! ct.BBox.ok() ) std::cout << FRED("Warning: bbox pr: Rect is empty/invalid!\n");
                ct.BBox.pr();
                rt.go(cmd_argc[current_cmd]);
                break;
            case CM_BBOX_EXPAND:
                if (!chkargc()) break;
                tmp_double[1] = safestod(rt.next(1),0,1);  
                tmp_double[2] = safestod(rt.next(2),0,1);  
                tmp_double[3] = safestod(rt.next(3),0,1);  
                ct.BBox.expand_round(tmp_double[1],tmp_double[2],tmp_double[3]);
                rt.go(cmd_argc[current_cmd]);
                break;
            case CM_BBOX_LOAD:
                if (!chkargc()) break;
                ct.BBox.load_rect(rt.next(1), rt.next(2), rt.next(3), rt.next(4));
                rt.go(cmd_argc[current_cmd]);
                break;
            case CM_GO:
                if (!rt.check_next(1)) break;
       //         go(argv[i+1]);
                rt.go(cmd_argc[current_cmd]);
                break;
            case CM_FNAME_EXPORT:
                if (!rt.check_next(1)) break;
                ct.fname_export = rt.next(1);
                rt.go(cmd_argc[current_cmd]);
                break;
            case CM_FILE_REDIRECT:
                if (!rt.check_next(1)) break;
                ct.file_redirect(rt.next(1));
                rt.go(cmd_argc[current_cmd]);
                break;
            case CM_FILE_COUT:
                ct.file_cout();
                break;
            case CM_QUI:
                quiet = 1;
                break;
            case CM_BATCH:
                if (!rt.check_next(1)) break;
                rt.push_runtime();
                break;
            case CM_ENDBATCH:
                rt.pop_runtime();
                break;
            case CM_RUN_OCTAVE:
                ct.run_octave();
                break;
            case CM_VERB:
                quiet = 0;
                break;
                
            case CM_HLP:
                help(cmdcode, cmd_argc);
                break;
            case CM_DBGA:
         //       help(cmdcode);
                break;

            default:
                std::cout << FRED("Unknown command: " << rt.current() << "\n");
                break;
        }
    }
    
 //   std:: cout << (int)43189454010 ;
    return 0;
}
