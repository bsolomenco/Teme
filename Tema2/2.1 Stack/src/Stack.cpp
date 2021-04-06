#include "Stack.hpp"

//--------------------------------------------------------------------------------
void Stack::push(const Info& info){
    _head = new Nod{info, _head};
    _size++;
}

//--------------------------------------------------------------------------------
void Stack::pop(){
    if(_head){
        auto nod = _head;
        _head = _head->next;
        delete nod;
        _size--;
    }
}

//--------------------------------------------------------------------------------
void Stack::clear(){
    while(_head){
        auto nod = _head;
        _head = _head->next;
        delete nod;
    }
    _size = 0;
}