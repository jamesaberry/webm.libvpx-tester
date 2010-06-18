//#pragma warning(disable:4710) //not inlined
//#pragma warning(push, 3)
#include "FourCC.hpp"
#if 1 //def ON2FLVSDK_FOURCC_STREAMIO
#include <iostream> //<ostream>
#include <iomanip>
#include <cctype>
#endif
//#pragma warning(pop)
//#pragma warning(disable:4514) //unref'd inline

namespace on2common
{

const FourCC FourCC::Null("    ");
const FourCC FourCC::RGB(0UL);
const FourCC FourCC::RLE8(1UL);
const FourCC FourCC::RLE4(2UL);
const FourCC FourCC::BITFIELDS(3UL);
const FourCC FourCC::JPEG(4UL);
const FourCC FourCC::PNG(5UL);

#if 1 //def ON2FLVSDK_FOURCC_STREAMIO

bool FourCC::printable() const
{
    for (int i = 0; i < 4; ++i)
    {
        const unsigned char c = code[i];

        if (!isprint(c))
            return false;
    }

    return true;
}

std::ostream &FourCC::put(std::ostream &os) const
{
    if (printable())
        return os.write(code, 4);
    else
        return os << std::setfill(' ') << std::setw(4) << codeAsLong;
}

std::wostream &FourCC::put(std::wostream &os) const
{
    if (printable())
    {
        //return os.write(code, 4);

        for (int i = 0; i < 4; ++i)
            os.put(wchar_t(code[i]));
    }
    else
        return os << std::setfill(L' ') << std::setw(4) << codeAsLong;

    return os;
}

#endif  //ON2FLVSDK_FOURCC_STREAMIO

}  //end namespace on2common

