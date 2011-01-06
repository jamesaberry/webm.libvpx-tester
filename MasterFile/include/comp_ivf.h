#ifndef _compivf_h_
#define _compivf_h_

#include <iostream>

namespace COMPIVF_NAMESPACE
{

class CompIVF
{
public :
    bool compareFileHeader();
    void IVF(char *filename1, char *filename2);
};
}

#endif
