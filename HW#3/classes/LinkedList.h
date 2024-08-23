#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <string>
#include "DynamicStack.h"
using namespace std;

struct Function{
    string name;
    Function *next;
    operation *operations;

    Function(string &name, Function *next, operation *operations) : name(name), next(next), operations(operations){}

    Function(){
        name = "";
        next = nullptr;
        operations = nullptr;
    }
};

class LinkedList{
    private:
        Function *head;
    
    public:
        LinkedList();
        ~LinkedList();
        Function *add_function_name(string);
        void add_function(Function*, string);
        Function *get_head();
        bool isEmpty();
        void print_list();
        Function *find_function_name(string);
        bool function_name_exists(string) const;

};

#endif