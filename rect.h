

class Rect
{
public:
    double X1;
    double X2;
    double Y1;
    double Y2;
    Rect() : X1(0), X2(0), Y1(0), Y2(0) {};
    Rect(double aX1, double aX2, double aY1, double aY2);
    void load_rect(std::string aX1, std::string aX2, std::string aY1, std::string aY2);
    bool ok();                                                                              // validity text, void rect [0,0,0,0] is not ok
    bool is_inside(double X, double Y);
    void pr();
    Rect& expand_round(double exp, double unit);
    Rect& expand_round(double expx, double expy, double unit);
};
