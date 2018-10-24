class Runtime
{
    std::vector< std::vector<std::string> > v_argv;
    std::vector<int> v_argc;
    std::vector<int> v_IP;
    int depth;

//    int IP;
public:

//    int                         argc;
//    std::vector<std::string>    &argv;

    Runtime() : depth(0)          {};
    
    void init(int &argc, char ** &argv);                                // initialization
    void pop_runtime();
    void push_runtime();

    bool check_next(const int i);                                       // does exist arg in future?
    std::string& next(const int i);                                     // peek for argument in future
    std::string& current();                                             // current command
    bool go(const int i = 1);                                           // go to the next command
    int get_IP();                                                       // current instruction counter
    
    static std::string NULLARG;
    static std::string NULLOP;
private:
                                                                        // reads argc, argv from file 'fn'
                                                                        // on success returns 0
    int prepare_argv(const std::string fn, int &count, std::vector<std::string> &vss);
};

std::string Runtime::NULLARG = "(null arg)";
std::string Runtime::NULLOP  = "(null op)";

void Runtime::init(int &orig_argc, char ** &orig_argv)
{
    std::vector<std::string>  vecstring;
    v_argv.clear();
    v_argc.clear();
    v_IP.clear();
    vecstring.resize(orig_argc);
    for (int k = 0; k < orig_argc; k++)
    {
        vecstring[k] = orig_argv[k];         // operator= deep copy from c-string             
    }
    depth = 0;
    v_argv.push_back(vecstring);
    v_argc.push_back(orig_argc);
    v_IP.push_back(0);                       // with "while(go())" is the first IP=1, which suits to argv[1]
}

// does exist arg in future?
bool Runtime::check_next(const int i)                                       
{
    if (v_IP.back() + i < v_argv.back().size() ) return true;
    else return false;
}
// peek for argument in future
std::string& Runtime::next(const int i)                                      
{
    if (v_IP.back() + i < v_argv.back().size() ) return v_argv.back().at( v_IP.back() + i);
    else return NULLARG;
}

// current command
std::string& Runtime::current()
{
    if (v_IP.back() < v_argv.back().size() ) return v_argv.back().at( v_IP.back() );
    else return NULLOP;
}


// go to the next command
bool Runtime::go(const int i)                                     
{
    v_IP.back() += i;
    if ( v_IP.back() < v_argv.back().size() ) return true;
    else return false;
}

// go to the next command
int Runtime::get_IP()                            
{
   return  v_IP.back();
}

// reads argc, argv from file 'fn'
// on success returns 0
int Runtime::prepare_argv(const std::string fn, int &count, std::vector<std::string> &vss)
{
    std::ifstream in (fn);
    if (!in.good())
    {
        std::cerr << "Error opening batch file '" << fn <<"'\n";
        return 1;
    }
    
    std::string ss;
    vss.clear();
    count = 0;
    while(1)
    {
        ss.clear();
        in >> ss;
        if (!in.good() || in.eof()) break;
        vss.push_back(ss);
        count++;
    }    
    return 0;
}

void Runtime::push_runtime()
{
    int argc;
    std::vector<std::string> vss;
    
    int res = prepare_argv( next(1), argc, vss);
    v_IP.back() += 1;                      // argument next1 processed
    if (res == 0)
    {
        v_argc.push_back(argc);
        v_argv.push_back(vss);
        v_IP.push_back(-1);             // increment in main loop via while(go), next command has index 0
        depth++;
    }
}

void Runtime::pop_runtime()
{
    if (depth == 0 || v_argv.size() == 0)
    {
        std::cerr << "Runtime stack is empty!\n"; 
        return;
    }
    v_argc.pop_back();
    v_argv.pop_back();
    v_IP.pop_back();
    depth--;
}

