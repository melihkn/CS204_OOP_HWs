#ifndef DYNAMICSTACK_H
#define DYNAMICSTACK_H

#include <string>
using namespace std;

struct operation
{
    string function_name; //function_1
    string operation_name; //define x
	operation *next;

	operation(const string &function_name, const string &operation_name) : function_name(function_name), operation_name(operation_name){
		next = nullptr;
	}
};



class DynamicStack
{
	private:
		operation *top;

	public:
		DynamicStack();
		~DynamicStack();
		void push(string, string);
		void pop(operation *&);
		void pop();
		bool isEmpty(void);
        void print_stack(operation *);
		operation *get_top();
		bool is_operation_exist();
		void clear_stack_from_func(string);
};


#endif