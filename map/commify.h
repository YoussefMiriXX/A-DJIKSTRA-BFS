#ifndef COMMIFY_H
#define COMMIFY_H

#include <ostream>
#include <iostream>

using std::string;

class Commify {

private:

    long int value_;

public:

    explicit Commify(long int value);
    friend std::ostream& operator<<(std::ostream &os, const Commify &c);
};
#endif
