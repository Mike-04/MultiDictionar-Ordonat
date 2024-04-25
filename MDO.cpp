#include "IteratorMDO.h"
#include "MDO.h"
#include <iostream>
#include <vector>

#include <exception>
using namespace std;


LDI::LDI() {
    this->prim = -1;
    this->primLiber = -1;
    this->cp = 0;
}

LDI::LDI(TCheie c)
{
    this->cheie = c;
    this->prim = -1;
    this->primLiber = 0;
    this->cp = 2;

    this->elems = new TElem[cp];
    urm = new int[cp];
    pre = new int[cp];
    for (int i = 0; i < cp - 1; i++)
    {
        urm[i] = i + 1;
        pre[i] = -1;
    }
    urm[cp - 1] = -1;
    pre[cp - 1] = -1;
}

int LDI::aloca()
{
    int i = primLiber;
    primLiber = urm[primLiber];
    return i;
}

void LDI::dealoca(int i)
{
    urm[i] = primLiber;
    pre[primLiber] = i;
    primLiber = i;
}


int LDI::creeazaNod(TElem el)
{
    if (primLiber == -1)
        redim();
    int i = aloca();
    elems[i] = el;
    if(prim != -1)
        pre[prim] = i;
    urm[i] = prim;
    prim = i;
    pre[i] = -1;
    return i;
}


void LDI::redim()
{
    int cp_nou = cp * 2;
    TElem* elem_nou = new TElem[cp_nou];
    int* urm_nou = new int[cp_nou];
    int* pre_nou = new int[cp_nou];
    for (int i = 0; i < cp; ++i)
    {
        elem_nou[i] = elems[i];
        urm_nou[i] = urm[i];
        pre_nou[i] = pre[i];
    }
    for (int i = cp; i < cp_nou; ++i)
    {
        urm_nou[i] = i + 1;
        pre_nou[i] = -1;
    }
    urm_nou[cp_nou - 1] = -1;
    pre_nou[cp_nou - 1] = -1;

    this->primLiber = cp;
    delete[] elems;
    delete[] urm;
    delete[] pre;
    this->elems = elem_nou;
    this->urm = urm_nou;
    this->cp = cp_nou;
}

int MDO::aloca()
{
    int i = primLiber;
    primLiber = urm[primLiber];
    return i;
}


void MDO::dealoca(int i)
{
    urm[i] = primLiber;
    pre[primLiber] = i;
    primLiber = i;
}


int MDO::creeazaNod(TElem el)
{

    if (primLiber == -1)
        redim();
    int i = aloca();
    LDI list(el.first);
    elems[i] = list;
    elems[i].creeazaNod(el);
    return i;
}


void MDO::redim()
{
    //resize the array
    int cp_nou = cp * 2;
    LDI* elem_nou = new LDI[cp_nou];
    int* urm_nou = new int[cp_nou];
    int* pre_nou = new int[cp_nou];
    for (int i = 0; i < cp; ++i)
    {
        elem_nou[i] = elems[i];
        urm_nou[i] = urm[i];
        pre_nou[i] = pre[i];
    }
    for (int i = cp; i < cp_nou; ++i)
    {
        urm_nou[i] = i + 1;
        pre_nou[i] = -1;
    }

    urm_nou[cp_nou - 1] = -1;
    pre_nou[cp_nou - 1] = -1;

    this->primLiber = cp;
    delete[] elems;
    delete[] urm;
    delete[] pre;
    this->elems = elem_nou;
    this->urm = urm_nou;
    this->pre = pre_nou;
    this->cp = cp_nou;
}

MDO::MDO(Relatie r) {
    this->len = 0;
    this->rel = r;
    this->prim = -1;
    this->primLiber = 0;
    this->cp = 2;

    this->elems = new LDI[cp];
    this->urm = new int[cp];
    this->pre = new int[cp];
    for (int i = 0; i < cp - 1; i++)
    {
        urm[i] = i + 1;
        pre[i] = -1;
    }
    urm[cp - 1] = -1;
    pre[cp - 1] = -1;
    prim = -1;
    primLiber = 0;
}

void MDO::adauga(TCheie c, TValoare v) {

    if (this->prim == -1)
    {
        TElem element_auxiliar;
        element_auxiliar.first = c;
        element_auxiliar.second = v;
        int i = this->creeazaNod(element_auxiliar);

        if (prim != -1)
            pre[prim] = i;
        urm[i] = prim;
        prim = i;
        pre[i] = -1;
        this->len = 1;
        return;
    }

    int anterior = -1;
    int current = prim;
    while (current != -1 && rel(this->elems[current].cheie, c))
    {
        if (this->elems[current].cheie == c)
        {
            TElem element_auxiliar;
            element_auxiliar.first = c;
            element_auxiliar.second = v;
            this->elems[current].creeazaNod(element_auxiliar);
            this->len++;
            return;
        }

        anterior = current;
        current = urm[current];
    }

    if (current <= -1)
    {
        TElem element_auxiliar;
        element_auxiliar.first = c;
        element_auxiliar.second = v;
        int i = this->creeazaNod(element_auxiliar);
        this->len++;
        urm[anterior] = i;
        pre[i] = anterior;
        urm[i] = -1;
        return;
    }
    else if (this->elems[current].cheie == c)
    {
        TElem element_auxiliar;
        element_auxiliar.first = c;
        element_auxiliar.second = v;
        this->elems[current].creeazaNod(element_auxiliar);
        this->len++;
        return;
    }
    else if (!rel(this->elems[current].cheie, c))
    {
        TElem element_auxiliar;
        element_auxiliar.first = c;
        element_auxiliar.second = v;
        int i = this->creeazaNod(element_auxiliar);

        if(anterior > -1)
            urm[anterior] = i;
        pre[i] = anterior;
        urm[i] = current;
        pre[current] = i;
        if (anterior == -1)
            this->prim = i;
        this->len++;
        return;
    }
}

vector<TValoare> MDO::cauta(TCheie c) const {

    vector<TValoare> v;

    int current = prim;
    while (current != -1 && rel(this->elems[current].cheie, c))
    {
        if (this->elems[current].cheie == c)
        {
            int first_elem = this->elems[current].prim;
            while (first_elem != -1)
            {
                v.push_back(this->elems[current].elems[first_elem].second);
                first_elem = this->elems[current].urm[first_elem];
            }
            return v;
        }
        current = urm[current];
    }

    return v;
}

bool MDO::sterge(TCheie c, TValoare v) {
    if(prim == -1)
        return false;

    int current = prim;

    if (c == elems[current].cheie)
    {
        int first_elem = this->elems[current].prim;
        while (first_elem != -1)
        {
            if (this->elems[current].elems[first_elem].second == v)
            {
                if (this->elems[current].pre[first_elem] > -1)
                    this->elems[current].urm[this->elems[current].pre[first_elem]] = this->elems[current].urm[first_elem];

                if (this->elems[current].urm[first_elem] > -1)
                    this->elems[current].pre[this->elems[current].urm[first_elem]] = this->elems[current].pre[first_elem];

                if (this->elems[current].prim == first_elem)
                    this->elems[current].prim = this->elems[current].urm[first_elem];

                this->elems[current].dealoca(first_elem);

                if (this->elems[current].prim == -1)
                {
                    if (this->pre[current] > -1)
                        this->urm[this->pre[current]] = this->urm[current];
                    if (this->urm[current] > -1)
                        this->pre[this->urm[current]] = this->pre[current];

                    this->prim = this->urm[current];
                    this->dealoca(current);
                }
                this->len--;
                return true;
            }
            first_elem = this->elems[current].urm[first_elem];
        }
        return false;
    }

    current = urm[current];
    while (current != -1 && rel(this->elems[current].cheie, c))
    {
        if (this->elems[current].cheie == c)
        {
            int first_elem = this->elems[current].prim;
            while (first_elem != -1)
            {
                if (this->elems[current].elems[first_elem].second == v)
                {
                    if (this->elems[current].pre[first_elem] > -1)
                        this->elems[current].urm[this->elems[current].pre[first_elem]] = this->elems[current].urm[first_elem];

                    if (this->elems[current].urm[first_elem] > -1)
                        this->elems[current].pre[this->elems[current].urm[first_elem]] = this->elems[current].pre[first_elem];

                    if (this->elems[current].prim == first_elem)
                        this->elems[current].prim = this->elems[current].urm[first_elem];

                    this->elems[current].dealoca(first_elem);
                    if (this->elems[current].prim == -1)
                    {
                        if (this->pre[current] > -1)
                            this->urm[this->pre[current]] = this->urm[current];
                        if (this->urm[current] > -1)
                            this->pre[this->urm[current]] = this->pre[current];

                        this->dealoca(current);
                    }
                    this->len--;
                    return true;
                }
                first_elem = this->elems[current].urm[first_elem];
            }
            return false;
        }
        current = urm[current];
    }

    return false;
}
int MDO::dim() const {
    return this->len;
}

bool MDO::vid() const {
    return this->len == 0;
}

IteratorMDO MDO::iterator() const {
    return IteratorMDO(*this);
}

MDO::~MDO() {
//    delete[] elems;
//    delete[] urm;
//    delete[] pre;
}
