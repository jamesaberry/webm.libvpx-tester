//______________________________________________________________________________
//
//  Arguments.cpp
//  datarate command line argument handling

//  Use parser ...

#include <iostream>
#include <limits>
#include <string>
#include <cassert>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

#include "version.hpp"
#include "Arguments.hpp"

using namespace datarate;

//  Defaults
string Arguments::strFile;
int Arguments::nStream = -1;
long Arguments::nFrameBegin = 0;
long Arguments::nFrameEnd = numeric_limits<long>::max();
double Arguments::dTimeBegin = 0;
double Arguments::dTimeEnd = numeric_limits<double>::max();
bool Arguments::bDetails = false;

//------------------------------------------------------------------------------
void Arguments::usage(const char *szProgram)
{
    cout << VERSION_STRING << endl;
    cout << "Calculates the data rate for the given stream and the file as a whole." << endl;
    cout << "If no stream is specified, data rates for all streams in the file will be displayed." << endl;
    cout << "Data rates are calculated with respect to the length, rate, and scale of the selected or current stream." << endl;
    cout << "Calculations can be performed on a section of the stream by specifying a frame or time range." << endl;
    cout << "Returns the (first) stream data rate as its return code." << endl;
    cout << endl;
    cout << "Usage:  " << szProgram << " [options] file" << endl;
    cout << "    -s <stream>             stream" << endl;
    cout << "    -f <frame1> <frame2>    frame range" << endl;
    cout << "    -t <time1> <time2>      time range" << endl;
    cout << "    -l                      list details (sample size)" << endl;

    return;
}

//------------------------------------------------------------------------------
bool Arguments::parse(int nArgs, char *pszArg[])
{
    for (int nArg = 1; nArg < nArgs; ++nArg)
    {
        if (pszArg[nArg][0] == '-')
        {
            if (strcmp(pszArg[nArg], "-s") == 0)
            {
                if (nArg >= nArgs - 1)
                {
                    return false;
                }

                nStream = strtol(pszArg[nArg + 1], 0, 10);
                nArg += 1;
            }
            else if (strcmp(pszArg[nArg], "-f") == 0)
            {
                if (nArg >= nArgs - 2)
                {
                    return false;
                }

                nFrameBegin = strtol(pszArg[nArg + 1], 0, 10);
                nFrameEnd = strtol(pszArg[nArg + 2], 0, 10);
                nArg += 2;
            }
            else if (strcmp(pszArg[nArg], "-t") == 0)
            {
                if (nArg >= nArgs - 2)
                {
                    return false;
                }

                dTimeBegin = seconds(pszArg[nArg + 1]);
                dTimeEnd = seconds(pszArg[nArg + 2]);
                nArg += 2;
            }
            else if (strcmp(pszArg[nArg], "-l") == 0)
            {
                bDetails = true;
            }
            else
            {
                return false;
            }
        }
        else if (strFile.empty())
        {
            strFile = pszArg[nArg];
        }
        else
        {
            return false;
        }
    }

    if (strFile.empty())
    {
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
void Arguments::display()
{
    cout << boolalpha;
    cout << strFile << endl;
    cout << "Stream          " << nStream << endl;
    cout << "Frame range     " << nFrameBegin << ".." << nFrameEnd << endl;
    cout << "Time range      " << dTimeBegin << ".." << dTimeEnd << endl;
    cout << "Details         " << bDetails << endl;

    return;
}

//------------------------------------------------------------------------------
double Arguments::seconds(const string &strTime)
{
    const char *pcEnd = strTime.c_str();
    double dSeconds = strtod(pcEnd, const_cast<char **>(&pcEnd));

    while (*pcEnd == ':')
    {
        ++pcEnd;
        dSeconds *= 60.;
        dSeconds += strtod(pcEnd, const_cast<char **>(&pcEnd));
    }

    return dSeconds;
}
