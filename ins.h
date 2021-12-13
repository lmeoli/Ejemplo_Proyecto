#ifndef INS_H
#define INS_H
#include <iostream>
#include <string>

using namespace std;

class ins{
private:
    string addr;
    string tx_id;
    unsigned int idx;
public:
    ins();
    ins(string, string, unsigned int);
    ins(const ins &);
    ~ins();
    void setAddr(string);
    void setTxId(string);
    void setIdx(unsigned int);
    string getAddr(){return addr;};
    string getTxId(){return tx_id;};
    unsigned int getIdx(){return idx;};
    ins &operator=(const ins&);
    
    friend std::ostream & operator<<(std::ostream &, const ins &);
};
#endif //INS_H