#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <cmdline.h>

#include "sha256.h"
#include "outs.h"
#include "transaction.h"
#include "ins.h"
#include "block.h"

#define LENGTH_SHA 64

using namespace std;

bool checkLineInput(string const & str, string & _tx_id, unsigned int & _idx, string & _addr_i);
bool checkLineOutput(string const & str , float & value, string & addr_o);

static void opt_input(string const &);
static void opt_output(string const &);
static void opt_difficulty(string const &);
static void opt_help(string const &);

static option_t options[] = {
        {1, "i", "input", "-", opt_input, OPT_DEFAULT},
        {1, "o", "output", "-", opt_output, OPT_DEFAULT},
        {1, "d", "difficulty", NULL, opt_difficulty, OPT_MANDATORY},
        {0, "h", "help", NULL, opt_help, OPT_DEFAULT},
        {0, },
};
static int difficulty;
static istream *iss = 0;
static ostream *oss = 0;
static fstream ifs;
static fstream ofs;

static void
opt_input(string const &arg)
{
    // Si el nombre del archivos es "-", usaremos la entrada
    // est�ndar. De lo contrario, abrimos un archivo en modo
    // de lectura.
    //
    if (arg == "-") {
        iss = &cin;
    } else {
        ifs.open(arg.c_str(), ios::in);
        iss = &ifs;
    }

    // Verificamos que el stream este OK.
    //
    if (!iss->good()) {
        cerr << "cannot open "
             << arg
             << "."
             << endl;
        exit(1);
    }
}

static void
opt_output(string const &arg)
{
    // Si el nombre del archivos es "-", usaremos la salida
    // est�ndar. De lo contrario, abrimos un archivo en modo
    // de escritura.
    //
    if (arg == "-") {
        oss = &cout;
    } else {
        ofs.open(arg.c_str(), ios::out);
        oss = &ofs;
    }

    // Verificamos que el stream este OK.
    //
    if (!oss->good()) {
        cerr << "cannot open "
             << arg
             << "."
             << endl;
        exit(1);
    }
}

static void
opt_difficulty(string const &arg)
{
    istringstream iss(arg);

    if (!(iss >> difficulty)
        || !iss.eof()) {
        cerr << "non-integer difficulty: "
             << arg
             << "."
             << endl;
        exit(1);
    }

    if (iss.bad()) {
        cerr << "cannot read integer difficulty."
             << endl;
        exit(1);
    }
}

static void
opt_help(string const &arg)
{
    cout << "cmdline -d difficulty [-i file] [-o file]"
         << endl;
    exit(0);
}

/*Lee las transacciones del flujo de entrada apuntado por iss
y las escribe en el flujo de salida apuntado por os*/
void
entry(istream *is, ostream *os)
{
    long int n_tx_in, n_tx_out;
    string _tx_id, _addr_i, addr_o;
    unsigned int _idx;
    float value;
    string concatenation;
    string line;
    string prev = "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
    size_t txn_count = 0;
    size_t aux = 0;
    transaction trans;
    block b(txn_count, prev, "", difficulty);

    while (!is->eof()) {
        //Lectura de inputs
        getline(*is, line);
        line.push_back('\n');

        if(is->eof() && txn_count == 0){ //Si el archivo esta vacio
            b.proof();
            *os<<b;
            return;
        }

        try { n_tx_in = stoi(line,&aux);
        } catch (invalid_argument &e) {
            cerr << "Error input number field." << endl;
            return;
        }

        if(n_tx_in <= 0 || (line.length()-1)!= aux){
            cerr << "Error input number field." << endl;
            return;
        }

        trans.setSizeIn(n_tx_in);
        concatenation = concatenation + line;

        for (long int i = 0; i < n_tx_in; i++) {
            getline(*is, line);
            line.push_back('\n');

            if (checkLineInput(line, _tx_id, _idx, _addr_i) == false) {
                cout << "Error input line." << endl;
                return ;
            }
            trans.setTransactionIn(_addr_i, _tx_id, _idx, i);
            concatenation = concatenation + line;
        }

        if(trans.checkEqualsInputs() == true){//Verifica que no existan lineas de inputs iguales
            cout << "Error input line." << endl;
            return ;
        }

        //Lectura de Outputs
        getline(*is, line);
        line.push_back('\n');

        try { n_tx_out = stoi(line,&aux);
        } catch (invalid_argument &e) {
            cerr << "Error output number field." << endl;
            return ;
        }
        if(n_tx_out <= 0 || (line.length()-1)!= aux){
            cerr << "Error output number field." << endl;
            return;
        }

        trans.setSizeOut(n_tx_out);
        concatenation = concatenation + line;

        for(long int i=0; i<n_tx_out; i++) {

            if(is->eof()){
                cout << "Error output line." << endl;
                return;
            }

            getline(*is, line);
            line.push_back('\n');

            if (checkLineOutput(line, value, addr_o) == false) {
                cout << "Error output line." << endl;
                return;
            }

            concatenation = concatenation + line;
            trans.setTransactionOut(value, addr_o, i);
        }

        if(trans.checkEqualsOutputs() == true){//Verifica que no existan lineas de outputs iguales
            cout << "Error output line." << endl;
            return ;
        }

        b.setSize(++txn_count);
        b.setTxns(trans,txn_count-1);
    }

    b.concatenate(concatenation);

    if(!b.proof()) {
        cerr << "Error HASH difficulty" << endl;
        return;
    }

    *os<<b;

    if (os->bad()) {
        cerr << "cannot write to output stream."
             << endl;
        exit(1);
    }
    if (is->bad()) {
        cerr << "cannot read from input stream."
             << endl;
        exit(1);
    }
    if (!is->eof()) {
        cerr << "cannot find EOF on input stream."
             << endl;
        exit(1);
    }
}

int main(int argc, char * const argv[])
{
    cmdline cmdl(options);
    cmdl.parse(argc, argv);
    entry(iss, oss);
}

bool checkLineInput(string const & str, string & _tx_id, unsigned int &_idx, string & _addr_i) {

    string aux;
    string aux2;
    string aux3;
    size_t final_char;
    int i =0;

    for(; str[i] != ' ' && str[i] != '\n'; i++)
        aux.push_back(str[i]);

    if(aux.length() != LENGTH_SHA)
        return false;

    _tx_id = aux;

    for(i = i+1; str[i] != ' ' && str[i] != '\n'; i++)
        aux2.push_back(str[i]);

    try{ _idx = stoi(aux2,&final_char);
    } catch (invalid_argument &e) {
        return false;}

    if(aux2.length() != final_char)
        return false;

    for(i = i+1; str[i] != ' ' && str[i] != '\n'&& str[i] != '\r' ; i++)
        aux3.push_back(str[i]);

    if(aux3.length() != LENGTH_SHA)
        return false;

    if(aux==aux3)
        return false;

    _addr_i=aux3;

    return true;

}

bool checkLineOutput(string const & str , float & value, string & addr_o) {

    string aux;
    string aux2;
    size_t final_char;
    int i =0;

    for(; str[i] != ' ' && str[i] != '\n'; i++)
        aux.push_back(str[i]);

    try   {value = stof(aux,&final_char);
    } catch (invalid_argument &e){
        return false;}

    if(aux.length() != final_char)
        return false;

    for(i=i+1; str[i] != ' ' && str[i] != '\n'&& str[i] != '\r' ; i++)
        aux2.push_back(str[i]);

    if(aux2.length() != LENGTH_SHA)
        return false;

    addr_o = aux2;

    return true;
}