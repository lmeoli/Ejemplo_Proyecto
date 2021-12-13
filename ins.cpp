#include <iostream>
#include "ins.h"

//Constructores y destructor
ins::ins(){
    addr = '\0';
    tx_id = '\0';
    idx = 0;
}

ins::ins(string dir, string tx, unsigned int id){
    addr = dir;
    tx_id = tx;
    idx = id;
}

ins::ins(const ins & ad){
    addr = ad.addr;
    tx_id = ad.tx_id;
    idx = ad.idx;
}

ins::~ins(){}

//Setters y getters
void ins::setAddr(string dir){
    addr = dir;
}

void ins::setTxId(string tx){
    tx_id = tx;
}
void ins::setIdx(unsigned int id){
    idx = id;
}

//Operadores
ins & ins:: operator=(const ins& right){

    if ( &right != this ){ //verifico que no sean el mismo

            addr = right.addr;
            tx_id = right.tx_id;
            idx = right.idx;
            return *this;
    }
    return *this;
}

std::ostream & operator<<(std::ostream & os, const ins & input)
{
    return os<<input.tx_id<<' '<<input.idx<<' '<<input.addr;
}