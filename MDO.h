
#pragma once

#include <vector>
#include <stdlib.h>

typedef int TCheie;
typedef int TValoare;

#include <utility>
typedef std::pair<TCheie, TValoare> TElem;

using namespace std;

class IteratorMDO;
class IteratorValori;
class MDO;

class LDI {
public:
    int cp;				// capacitate memorare vectori
    int* urm{};			// lista cu pozitiile elementelor urmatoare
    int* pre{};
    int prim;			// primul element din colectie
    int primLiber;		// pozitia primului element liber
    TElem* elems{};
    TCheie cheie{};

    LDI();
    LDI(TCheie c);
    int aloca();
    void dealoca(int i);
    int creeazaNod(TElem el);
    void redim();
    ~LDI() = default;
};

typedef bool(*Relatie)(TCheie, TCheie);

class MDO {
    friend class IteratorMDO;
private:
public:
    Relatie rel;
    int cp;				// capacitate memorare vectori
    int* urm;			// lista cu pozitiile elementelor urmatoare
    int* pre;           // lista cu pozitiile elementelor precedente
    int prim;			// primul element din colectie
    int primLiber;		// pozitia primului element liber
    int len;			// numarul elementelor
    LDI* elems;

    int aloca();
    void dealoca(int i);
    int creeazaNod(TElem el);
    void redim();
public:
    MDO(Relatie r);
    void adauga(TCheie c, TValoare v);
    vector<TValoare> cauta(TCheie c) const;
    bool sterge(TCheie c, TValoare v);
    int dim() const;
    bool vid() const;
    IteratorMDO iterator() const;
    ~MDO();
};
