#pragma once
#include "Nod.hpp"

class Stack{
public:
    int size() const {
        return _size;
    }

    void push(const Info& info);
    void pop();

    Info& top(){//check empty() before top()
        return _head->info;
    }

    bool empty() const {
        return _size==0;
    }

    void clear();

protected:
    Nod* _head = nullptr;
    int  _size = 0;
};