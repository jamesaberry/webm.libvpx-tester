//------------------------------------------------------------------------------
//
//  Copyright (c) 2000-2006 On2 Technologies Inc. All Rights Reserved.
//
//------------------------------------------------------------------------------
//
//  $Workfile$
//  $Date$
//  $Revision$
//
//------------------------------------------------------------------------------

#ifndef FOURCC_HPP
#define FOURCC_HPP

#include <iosfwd>
#include <cstring>

namespace on2common
{

class FourCC
{
public:

    FourCC();
    explicit FourCC(const char *);
    explicit FourCC(unsigned long);

    bool operator==(const FourCC &) const;
    bool operator!=(const FourCC &) const;

    bool operator==(const char *) const;
    bool operator!=(const char *) const;

    operator unsigned long() const;
    unsigned long asLong() const;

    FourCC &operator=(unsigned long);

    char operator[](int) const;

#if 1 //def ON2FLVSDK_FOURCC_STREAMIO
    std::ostream &put(std::ostream &) const;
    std::wostream &put(std::wostream &) const;
    bool printable() const;
#endif

    //  Special values allowed as FourCC's
    static const FourCC Null;
    static const FourCC RGB;
    static const FourCC RLE8;
    static const FourCC RLE4;
    static const FourCC BITFIELDS;
    static const FourCC JPEG;
    static const FourCC PNG;

private:

    union
    {
        char code[4];
        unsigned long codeAsLong;
    };

};

//std::ostream& operator<<(std::ostream&, const FourCC&);
//std::wostream& operator<<(std::wostream&, const FourCC&);

}  //end namespace on2common


inline on2common::FourCC::FourCC()
{
}

inline on2common::FourCC::FourCC(unsigned long x)
    : codeAsLong(x)
{
}

inline on2common::FourCC::FourCC(const char *str)
{
    memcpy(code, str, 4);
}

inline bool on2common::FourCC::operator==(const FourCC &rhs) const
{
    return codeAsLong == rhs.codeAsLong;
}

inline bool on2common::FourCC::operator!=(const FourCC &rhs) const
{
    return !operator==(rhs);
}

inline bool on2common::FourCC::operator==(const char *rhs) const
{
    return (memcmp(code, rhs, 4) == 0);
}

inline bool on2common::FourCC::operator!=(const char *rhs) const
{
    return !operator==(rhs);
}

inline on2common::FourCC::operator unsigned long() const
{
    return codeAsLong;
}

inline unsigned long on2common::FourCC::asLong() const
{
    return codeAsLong;
}

inline char on2common::FourCC::operator[](int i) const
{
    return code[i];
}

inline on2common::FourCC &on2common::FourCC::operator=(unsigned long val)
{
    codeAsLong = val;
    return *this;
}

#if 1 //def ON2FLVSDK_FOURCC_STREAMIO

inline std::ostream &operator<<(std::ostream &os, const on2common::FourCC &rhs)
{
    return rhs.put(os);
}

inline std::wostream &operator<<(std::wostream &os, const on2common::FourCC &rhs)
{
    return rhs.put(os);
}

#endif  //ON2FLVSDK_FOURCC_STREAMIO

#endif  //FOURCC_HPP