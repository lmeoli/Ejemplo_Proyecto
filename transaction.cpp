#include <iostream>
#include "transaction.h"
#include "array.h"
#include "ins.h"
#include "outs.h"

//Constructores y destructor
transaction::transaction() {
    n_tx_in = 0;
    n_tx_out = 0;
}

transaction::transaction(size_t sin, size_t sout){
    n_tx_in = sin;
    n_tx_out = sout;
    inputs.resize(sin);
    outputs.resize(sout);
}

transaction::transaction(const transaction & trans) {
    n_tx_in = trans.n_tx_in;
    inputs = trans.inputs;
    n_tx_out = trans.n_tx_out;
    outputs = trans.outputs;
}

transaction::~transaction(){}

//SETTERS
void transaction::setTransactionIn(string dir, string tx, unsigned int id, size_t i) {
    if(i >= n_tx_in)
        return;

    inputs[i].setAddr(dir);
    inputs[i].setTxId(tx);
    inputs[i].setIdx(id);
}

void transaction::setTransactionOut(float val, string dir, size_t i){
    if(i >= n_tx_out)
        return;
    outputs[i].setValue(val);
    outputs[i].setAddr(dir);
}

void transaction::setSizeIn(size_t sin){
    n_tx_in = sin;
    inputs.resize(n_tx_in);
}

void transaction::setSizeOut(size_t sout){
    n_tx_out = sout;
    outputs.resize(n_tx_out);
}

//GETTERS

size_t transaction::getSizeIn(){
    return n_tx_in;
}

size_t transaction::getSizeOut(){
    return n_tx_out;
}

float transaction::getValueOut(size_t i) {
    if(i>=n_tx_out)
        return 0;
    return outputs[i].getValue();
}

string transaction::getAddrOut(size_t i) {
    if(i>=n_tx_out)
        return "\0";

    return outputs[i].getAddr();
}

string transaction::getAddrIn(size_t i){
    if(i>=n_tx_in)
        return "\0";
    return inputs[i].getAddr();
}

string transaction::getTxIdIn(size_t i){
    if(i>=n_tx_in)
        return "\0";
    return inputs[i].getTxId();
}

unsigned int transaction::getIdxIn(size_t i){
    if(i>=n_tx_in)
        return 0;
    return inputs[i].getIdx();
}

bool transaction::checkEqualsInputs() { //chequea si existen lineas iguales o no

    for(size_t i = 0; i<n_tx_in ;i++){
        for(size_t j =i+1; j<n_tx_in ; j++){
            if(inputs[i].getAddr() == inputs[j].getAddr()){
                if(inputs[i].getTxId() == inputs[j].getTxId()){
                    if(inputs[i].getIdx()==inputs[j].getIdx())
                        return true;
                }
            }
        }
    }
    return false;
}

bool transaction::checkEqualsOutputs() { //chequea si existen lineas iguales o no

    for(size_t i = 0; i<n_tx_out ;i++){
        for(size_t j =i+1; j<n_tx_out ; j++){
            if(outputs[i].getAddr() == outputs[j].getAddr())
                        return true;
        }
    }
    return false;
}

//OPERADORES

transaction & transaction::operator = (const transaction & right){


    if ( &right != this ){ //verifico que no sean el mismo

        if (n_tx_in != right.n_tx_in) {
            n_tx_in = right.n_tx_in;
            this->setSizeIn(n_tx_in);
            inputs=right.inputs;

        }else{//longitud igual
            inputs=right.inputs;
        }

        if (n_tx_out != right.n_tx_out) {
            this->setSizeOut(right.n_tx_out);
            n_tx_out = right.n_tx_out;
            outputs=right.outputs;

        }else{//longitud igual
            outputs=right.outputs;
        }
        return *this;

    }
    return *this;
}

std::ostream &operator<<(std::ostream & os, transaction &t)
{
    os<<t.n_tx_in<<endl;
    os<<t.inputs<<endl;
    os<<t.n_tx_out<<endl;
    os<<t.outputs;
    return os;
}

std::istream & operator>>(std::istream & is, transaction & a)
{
    size_t n_tx_in_, n_tx_out_;
    string _tx_id_, _addr_i_, addr_o_;
    unsigned int _idx_;
    float value_;

	is >> n_tx_in_;
    a.setSizeIn(n_tx_in_);

	for(size_t i=0; i < n_tx_in_; i++) {
	    is >> _tx_id_
           >> _idx_
           >> _addr_i_;
        a.setTransactionIn(_addr_i_, _tx_id_, _idx_, i);
    }

	is >> n_tx_out_;
    a.setSizeOut(n_tx_out_);

    for(size_t i = 0; i < n_tx_out_; i++) {
        is >> value_
           >> addr_o_;
        a.setTransactionOut(value_, addr_o_, i);
    }
    
    return is;
}