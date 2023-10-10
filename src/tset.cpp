// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : MaxPower(mp), BitField(mp) {
    // Constructor with the given max power
}

TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField) {
    // Copy constructor
}

TSet::TSet(const TBitField &bf) : MaxPower(bf.GetLength()), BitField(bf) {
    // Constructor for conversion from TBitField
}

TSet::operator TBitField() {
    return BitField;
    // Conversion operator to TBitField
}

int TSet::GetMaxPower(void) const {
    return MaxPower;
    // Return the max power of the set
}

int TSet::IsMember(const int Elem) const {
    if (Elem < 0 || Elem >= MaxPower)
        throw "Element out of range";
    return BitField.GetBit(Elem);
    // Check if an element is a member of the set
}

void TSet::InsElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower)
        throw "Element out of range";
    BitField.SetBit(Elem);
    // Insert an element into the set
}

void TSet::DelElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower)
        throw "Element out of range";
    BitField.ClrBit(Elem);
    // Delete an element from the set
}

int TSet::operator==(const TSet &s) const {
    return (MaxPower == s.MaxPower) && (BitField == s.BitField);
    // Check if two sets are equal
}

int TSet::operator!=(const TSet &s) const {
    return !(*this == s);
    // Check if two sets are not equal
}

TSet& TSet::operator=(const TSet &s) {
    if (this == &s)
        return *this;

    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
    // Assignment operator for sets
}

TSet TSet::operator+(const TSet &s) {
    if (MaxPower != s.MaxPower)
        throw "Sets of different MaxPower cannot be unioned";

    TSet result(MaxPower);
    result.BitField = BitField | s.BitField;
    return result;
    // Union of two sets
}

TSet TSet::operator+(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower)
        throw "Element out of range";

    TSet result(*this);
    result.InsElem(Elem);
    return result;
    // Union of a set with an element
}

TSet TSet::operator-(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower)
        throw "Element out of range";

    TSet result(*this);
    result.DelElem(Elem);
    return result;
    // Difference of a set with an element
}

TSet TSet::operator*(const TSet &s) {
    if (MaxPower != s.MaxPower)
        throw "Sets of different MaxPower cannot be intersected";

    TSet result(MaxPower);
    result.BitField = BitField & s.BitField;
    return result;
    // Intersection of two sets
}

TSet TSet::operator~(void) {
    TSet result(MaxPower);
    result.BitField = ~BitField;
    return result;
    // Complement of a set
}

istream &operator>>(istream &istr, TSet &s) {
    int maxPower;
    istr >> maxPower;
    TSet tmp(maxPower);
    for (int i = 0; i < maxPower; i++) {
        int element;
        istr >> element;
        if (element)
            tmp.InsElem(i);
    }
    s = tmp;
    return istr;
    // Input operator for sets
}

ostream &operator<<(ostream &ostr, const TSet &s) {
    int maxPower = s.GetMaxPower();
    ostr << maxPower << " ";
    for (int i = 0; i < maxPower; i++)
        ostr << s.IsMember(i) << " ";
    return ostr;
    // Output operator for sets
}
