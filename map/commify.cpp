#include "commify.h"
#include <iostream>
#include <ostream>

using std::string;

Commify::Commify(long int value)
{
    value_ = value;
}

std::ostream& operator<<(std::ostream &os, const Commify &c)
{
    string numberToString = std::to_string(c.value_);
    if (numberToString.length() > 3)
    {
        int start = numberToString.length() - 3;
        while(start > 0)
        {
            numberToString.insert(start, ",");
            start-=3;
        }
        return os << numberToString;
    }
    return os << c.value_;
}
