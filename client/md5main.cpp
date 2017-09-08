#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include "md5.h"
 
using std::cout; using std::endl;
 
int main(int argc, char *argv[])
{
    std::ifstream t("original.pdf");
    std::string str;
    
    t.seekg(0,std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0,std::ios::beg);
    str.assign( (std::istreambuf_iterator<char>(t) ),
                        (std::istreambuf_iterator<char>()    )  );
    cout << "md5 of 'original.pdf': " << md5(str) << endl;
    return 0;
}
