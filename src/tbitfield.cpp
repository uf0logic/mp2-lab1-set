// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <cmath>

TBitField::TBitField(int len)
{
    if (len < 1)
        throw "Invalid bitfield length";

    BitLen = len;
    MemLen = static_cast<int>(ceil(static_cast<double>(BitLen) / (sizeof(TELEM) * 8)));
    pMem = new TELEM[MemLen];
    if (!pMem)
        throw "Memory allocation error";

    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf)
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    if (!pMem)
        throw "Memory allocation error";

    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const
{
    if (n < 0 || n >= BitLen)
        throw "Index out of range";

    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const
{
    if (n < 0 || n >= BitLen)
        throw "Index out of range";

    int offset = n % (sizeof(TELEM) * 8);
    return static_cast<TELEM>(1) << offset;
}

int TBitField::GetLength(void) const
{
    return BitLen;
}

void TBitField::SetBit(const int n)
{
    if (n < 0 || n >= BitLen)
        throw "Index out of range";

    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] |= mask;
}

void TBitField::ClrBit(const int n)
{
    if (n < 0 || n >= BitLen)
        throw "Index out of range";

    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const
{
    if (n < 0 || n >= BitLen)
        throw "Index out of range";

    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[index] & mask) ? 1 : 0;
}

int TBitField::operator==(const TBitField &bf) const
{
    if (BitLen != bf.BitLen)
        return 0;

    for (int i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i])
            return 0;
    }

    return 1;
}

int TBitField::operator!=(const TBitField &bf) const
{
    return !(*this == bf);
}

TBitField& TBitField::operator=(const TBitField &bf)
{
    if (this == &bf)
        return *this;

    if (BitLen != bf.BitLen)
    {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        if (!pMem)
            throw "Memory allocation error";
    }

    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];

    return *this;
}

//TBitField TBitField::operator|(const TBitField &bf)
//{
//    if (BitLen != bf.BitLen)
//        throw "Bitfields of different lengths cannot be OR'ed";
//
//    TBitField result(BitLen);
//    for (int i = 0; i < MemLen; i++)
//        result.pMem[i] = pMem[i] | bf.pMem[i];
//
//    return result;
//}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    const int mn = min(BitLen, bf.BitLen);
    TBitField tmp(1);
    if (BitLen < bf.BitLen)
        tmp = bf;
    else
        tmp = *this;
    for (int i = 0; i < mn; ++i) {
        if (GetBit(i) | bf.GetBit(i))
            tmp.SetBit(i);
    }
    return tmp;
}

//TBitField TBitField::operator&(const TBitField &bf)
//{
//    if (BitLen != bf.BitLen)
//        throw "Bitfields of different lengths cannot be AND'ed";
//
//    TBitField result(BitLen);
//    for (int i = 0; i < MemLen; i++)
//        result.pMem[i] = pMem[i] & bf.pMem[i];
//
//    return result;
//}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    const int mn = min(BitLen, bf.BitLen);
    TBitField tmp(max(BitLen, bf.BitLen));
    for (int i = 0; i < mn; ++i) {
        if (GetBit(i) && bf.GetBit(i))
            tmp.SetBit(i);
    }
    return tmp;
}

TBitField TBitField::operator~(void)
{
    TBitField result(BitLen);
    for (int i = 0; i < BitLen; i++) {
        //result.pMem[i] = ~pMem[i];
        if (!GetBit(i))             //Украл у Эмиля, который украл у Кости :)
            result.SetBit(i);       //Капец, теперь всё переписывать. Ошибка обнаружена, так сказать
    }

    return result;
}

istream &operator>>(istream &istr, TBitField &bf)
{
    int len;
    istr >> len;
    TBitField tmp(len);
    for (int i = 0; i < len; i++)
    {
        int bit;
        istr >> bit;
        if (bit)
            tmp.SetBit(i);
    }
    bf = tmp;
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf)
{
    int len = bf.GetLength();
    ostr << len << " ";
    for (int i = 0; i < len; i++)
        ostr << bf.GetBit(i) << " ";
    return ostr;
}

