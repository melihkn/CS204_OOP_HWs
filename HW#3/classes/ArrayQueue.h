#include <string>

#ifndef ARRAYQUEUE_H
#define ARRAYQUEUE_H

using namespace std;

struct Request{
	string name;
	int ID;
    string request_func;
    Request *next;

	Request(const string &name, const int &ID, const string &request_func, Request *link) : name(name), ID(ID), request_func(request_func), next(link){}

    Request() : name(""), ID(0), request_func(""), next(nullptr){}
    

};

class ArrayQueue
{
    private:
        Request *queueArray;
        int queueSize; //capacity of queue
        int front; //head of queue
        int rear; //tail of queue
        int numofInstructors; //# of elements currently in the queue
    public:
        ArrayQueue(int); //constructor, parameter is capacity
        ~ArrayQueue(); //destructor of the ArrayQueue class
        void enqueue(string, int, string); //member function to add variable to queue
        void dequeue(Request &); //member function to remove variable from queue
        bool isEmpty() const; 
        //bool isFull() const;
        //void clear(); //removes all elements
};
#endif