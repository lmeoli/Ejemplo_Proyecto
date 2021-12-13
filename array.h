#ifndef ARRAYTEMP_H
#define ARRAYTEMP_H
#include<iostream>

using namespace std;
//agrego este comentario para smartgit
template <typename T>
class Array{

private:
    size_t size;
    size_t vsize;
    T *ptr;

public:
    Array();
    Array(size_t);
    Array(const Array<T> &);
    ~Array();
    int getSize(){return size;};
    void setSize(size_t);
    Array<T>& operator=(const Array<T> &);
    bool operator==(const Array<T> &);
    T &operator[](size_t);
    size_t resize(size_t);
    void pull_back();

    template <typename TT>
    friend ostream& operator<<(ostream &, Array<TT> &);

};

template<typename T>
Array<T>::Array(){
    ptr = NULL;
    vsize =0;
    size = 0;
}

template<typename T>
Array<T>::Array(size_t i){
    ptr = i != 0 ? new T[i] : 0;
    size = vsize = i;
}

template <typename T>
Array<T>::Array(const Array<T> &init){
    ptr = init.size != 0 ? new T[init.size] : 0;
    for(size_t i=0; i < init.size; i++)
        ptr[i] = init.ptr[i];
    size = vsize = init.size;
}

template <typename T>
Array<T>::~Array(){
    if(ptr)
        delete[] ptr;
}

template <typename T>
Array<T>& Array<T>::operator=(const Array<T> &right){
    if(&right != this){
        if(size != right.size){
            T *aux;
            aux = new T[right.size];
            delete[] ptr;
            size = right.size;
            ptr = aux;
            for(size_t i=0; i < size; i++)
                ptr[i] = right.ptr[i];
            return *this;
        }
        else{
            for (size_t i = 0; i < size; i++)
                ptr[i] = right.ptr[i];
            return *this;
        }
    }
    vsize = right.size;
    return *this;
}

template <typename T>
bool Array<T>::operator==(const Array<T> &right){
    if(size != right.size)
        return false;
    else
        for(size_t i=0; i<size; i++)
            if(ptr[i] != right.ptr[i])
                return false;
    return true;
}

template <typename T>
T &Array<T>::operator[](size_t subscript){
    if(subscript >= vsize)
        abort();
    return ptr[subscript];
}

template <typename TT>
ostream & operator<<(ostream &os, Array<TT> &right){
    for(size_t i=0; i<right.size; i++){
        os << right.ptr[i];
        if(i != right.size-1)
            os<< '\n';
    }
    return os;
}

template <typename T>
size_t Array<T>::resize(size_t nsize){
    if(nsize > size){
        T *aux = new T[nsize];
        for(size_t i=0; i<vsize; i++)
            aux[i] = ptr[i];
        delete[] ptr;
        ptr = aux;
        size = nsize;
    }
    return vsize = nsize;
}

template<typename T>
void Array<T>::setSize(size_t s){
    resize(s);
}

template<typename T>
void Array<T>::pull_back()
{
    T *unused = 0;
    pull_back(unused);
}

template<typename T>
void Array<T>::pull_back(T *p)
{
    if (vsz <= 0)
        std::abort();
    --vsz;
    if (p)
        *p = v[vsz];
}

template<typename T>
void Array<T>::pull_back(T &t)
{
    pull_back(&t);
}

#endif