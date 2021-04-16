#include <cstdio>
#pragma warning(disable : 4996)

typedef int Info;

//Nod structure used in list based stack or queue
struct Nod {
    Info info = {};
    Nod* next = 0;
};
struct Stack {
    int size() const {
        return _size;
    }

    void push(const Info& info);
    void pop();

    Info& top() {//check empty() before top()
        return _head->info;
    }

    bool empty() const {
        return _size == 0;
    }

    void clear();

//data members
    Nod* _head = 0;
    int  _size = 0;
};
//--------------------------------------------------------------------------------
void Stack::push(const Info& info) {
    _head = new Nod{ info, _head };
    _size++;
}

//--------------------------------------------------------------------------------
void Stack::pop() {
    if (_head) {
        Nod* nod = _head;
        _head = _head->next;
        delete nod;
        _size--;
    }
}

//--------------------------------------------------------------------------------
void Stack::clear() {
    while (_head) {
        Nod* nod = _head;
        _head = _head->next;
        delete nod;
    }
    _size = 0;
}
//================================================================================
int main(int argc, char** argv){
    Stack stack;
    int n = 0;
    printf("number of elements: ");
    scanf("%d", &n);
    printf("elements (SPACE or EOL separated):\n");

    for(int element=0; n--; ){
        scanf("%d", &element);
        stack.push(element);
    }

    printf("popping elements from stack:\n");
    while(!stack.empty()){
        Info element = stack.top();
        printf("    %d\n", element);
        stack.pop();
    }

    return 0;
}