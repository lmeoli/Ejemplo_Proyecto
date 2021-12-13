#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iostream>
#include "array.h"
#include "outs.h"
#include "ins.h"

class transaction{
    size_t n_tx_in;
    Array<ins> inputs;
    size_t n_tx_out;
    Array<outs> outputs;

public:
    transaction();
    transaction(size_t , size_t);
    transaction(const transaction &);
    ~transaction();

    void setSizeIn(size_t);
    void setSizeOut(size_t);
    void setTransactionIn(string, string, unsigned int, size_t);
    void setTransactionOut(float, string, size_t);

    size_t getSizeIn();
    size_t getSizeOut();
    float getValueOut(size_t);
    string getAddrOut(size_t);
    string getAddrIn(size_t);
    string getTxIdIn(size_t);
    unsigned int getIdxIn(size_t);
    bool checkEqualsInputs();
    bool checkEqualsOutputs();

    transaction & operator = (const transaction &);

    friend std::ostream & operator<<(std::ostream &, transaction &);
};


#endif //TRANSACTION_H
