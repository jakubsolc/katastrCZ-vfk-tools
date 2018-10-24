

#include "vfk-headers.h"

#include <limits>   //quiet_NaN, infinity

void CBoundarySegment::swap_order()
{
                std::swap(X1, X2);
                std::swap(Y1, Y2);
                std::swap(KODCH1, KODCH2);
                std::swap(CISLO1, CISLO2);
                std::swap(UPLNE_CISLO1, UPLNE_CISLO2);
}



void CBoundarySegment::pr() const
{
    std::cout << std::fixed 
            << std::setw(8) << std::right << CISLO1 << "  " << UPLNE_CISLO1 << "  " << X1 << "   " << Y1 << "   " << KODCH1 << "   " 
            << std::setw(8) << std::right << CISLO2 << "  " << UPLNE_CISLO2 << "  " << X2 << "   " << Y2 << "   " << KODCH2 << "   " 
            << std::setw(11) << PAR_ID_1  << "  " << std::setw(11) << PAR_ID_2 << "  " << std::setw(12) << HP_ID << "  " << PARAM << "\n";
}

void CBoundarySegment::prnxy() const
{
    std::cout << std::setw(8) << std::right << CISLO1 << "  " << Y1 << "   " << X1 << '\n';
}

void BoundaryList::mprint(std::string A) const
{

    bare_export(A);
    
    std::cout << "plot(-"<<A<<"(:,2), -"<<A<<"(:,3), 'linewidth', 2); \n";
    std::cout << "daspect([1 1 1]);\nhold on;\nfsz=10;";
    std::cout << "for (k=1:(size("<<A<<",1)-1)), text(-"<<A<<"(k,2), -"<<A<<"(k,3), sprintf('%3d',"<<A<<"(k,1)), 'fontsize',fsz); end\n";
    std::cout << "%%yticks = get (gca, 'ytick');\n"
    "%%ylabels = arrayfun (@(x) sprintf ('%.0f', x), yticks, 'uniformoutput', false);\n"
    "%%set (gca, 'yticklabel', ylabels)\n";

    std::cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
    
}

Rect BoundaryList::boundaryBox()
{
    double Xa, Xb, Ya, Yb;
    Xa = Ya = std::numeric_limits<double>::infinity();
    Xb = Yb = -Xa;
    for (auto it = this->begin(); it != this->end(); it++)
    {
        if (Xa > it->X1) Xa = it->X1;
        if (Ya > it->Y1) Ya = it->Y1;
        if (Xb < it->X1) Xb = it->X1;
        if (Yb < it->Y1) Yb = it->Y1;
    }

    if (Xa > Xb) return Rect(std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::quiet_NaN());
    return Rect(Xa, Xb, Ya, Yb);
}


void BoundaryList::bare_export(std::string A) const
{
    if (this->size()==0) {
        std::cout << FRED("% bare_export: List is empty!\n"); return;}
    std::cout << A << " = [\n";
    for (int k = 0; k < (*this).size(); k++)
        (*this)[k].prnxy();
    (*this)[0].prnxy();         //...cycle
    
    std::cout << "];\n";    
}

void BoundaryList::pr(int styl) const
{
    if (this->size()==0) 
    {
        std::cout << FRED("List is empty!\n"); 
        return;
    }
    if (styl == 0)
    {
        std::cout << "-------------------------------------------------\n";
        if (METADATA != "") 
        {
            std::cout << METADATA << "\n";
            std::cout << "-------------------------------------------------\n";
        }
        std::cout << std::fixed << std::setprecision(2) << std::right;
        for (int k = 0; k < (*this).size(); k++)
            std::cout << std::setw(10) << (*this)[k].UPLNE_CISLO1 << std::setw(8) << (*this)[k].CISLO1 << std::setw(14) << (*this)[k].X1 << std::setw(14) << (*this)[k].Y1 << '\n';
    
        std::cout << "\n";    
    }
    if (styl == 1)
    {
        std::cout << "--------------------------------\n";
        if (METADATA2 != "") 
        {
            std::cout << METADATA2 << "\n";
            std::cout << "--------------------------------\n";
        }
        std::cout << std::fixed << std::setprecision(2) << std::right;
        for (int k = 0; k < (*this).size(); k++)
            std::cout << std::setw(6) << (*this)[k].CISLO1 << std::setw(13) << (*this)[k].X1 << std::setw(12) << (*this)[k].Y1 << '\n';
    
        std::cout << "\n";    
    }
        
}

void BoundarySet::pr(int styl) const
{
    if (this->size()==0) {
        std::cout << FRED("Set is empty!\n"); 
        return;
    }
 
    if (styl == 0)
    {
        std::cout << "#################################################\n";
        if (METADATA != "") 
        {
            std::cout << METADATA << "\n";
            std::cout << "#################################################\n";
        }
        for (int k = 0; k < (*this).size(); k++)
            (*this)[k].pr();
        std::cout << "#################################################\n";
        std::cout << "\n";    
    }
    if (styl == 1)
    {
        std::cout << "################################\n";
        if (METADATA2 != "") 
        {
            std::cout << METADATA2 << "\n";
            std::cout << "################################\n";
        }
        for (int k = 0; k < (*this).size(); k++)
            (*this)[k].pr(styl);
        std::cout << "################################\n";
        std::cout << "\n";    
    }
}

Rect BoundarySet::boundaryBox()
{
    Rect re = (*this)[0].boundaryBox();
    for (int i=1; i<(*this).size(); i++)
    {
        re = (*this)[i].boundaryBox(re);
    }
    return re;
}
Rect BoundaryList::boundaryBox(Rect r)
{

    for (auto it = this->begin(); it != this->end(); it++)
    {
        if (r.X1 > it->X1) r.X1 = it->X1;
        if (r.Y1 > it->Y1) r.Y1 = it->Y1;
        if (r.X2 < it->X2) r.X2 = it->X2;
        if (r.Y2 < it->Y2) r.Y2 = it->Y2;
    }

    return r;
}
