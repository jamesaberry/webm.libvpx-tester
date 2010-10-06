#if !defined(DATARATE_ARGUMENTS_HPP)
#define DATARATE_ARGUMENTS_HPP
//______________________________________________________________________________
//
//  Arguments.hpp
//  datarate command line argument handling

#include <string>

namespace datarate
{

//--------------------------------------
class Arguments
{
public:
    static std::string strFile;
    static int nStream;
    static long nFrameBegin;
    static long nFrameEnd;
    static double dTimeBegin;
    static double dTimeEnd;
    static bool bDetails;

    static void usage(const char *szProgram);
    static bool parse(int nArgs, char *pszArg[]);
    static void display();

private:
    Arguments();
    Arguments(const Arguments &a);
    Arguments &operator=(const Arguments &a);

    static double seconds(const std::string &strTime);
};

}  //  namespace datarate

#endif  //  DATARATE_ARGUMENTS_HPP
