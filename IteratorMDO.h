
#pragma once

#include "MDO.h"
#include <exception>

class IteratorMDO{
    friend class MDO;
private:
public:
    IteratorMDO(const MDO& dictionar);
    const MDO& dict;
    int i;
    int j;
public:
    void prim();
    void urmator();
    bool valid() const;
    pair <TCheie, TValoare> element() const;
};
