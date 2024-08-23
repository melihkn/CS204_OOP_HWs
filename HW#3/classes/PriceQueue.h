#ifndef PRICEQUEUE_H
#define PRICEQUEUE_H

#include <string>
using namespace std;

struct People{
	string name;
    int ID;
	People *next;
	int price;

    People(string name_, int num) {
        name = name_;
        ID = num;
        next = nullptr;
        price = 0;
    }

    People (string name_, int num, int price_) {
        name = name_;
        ID = num;
        next = nullptr;
        price = price_;
    }
};



class PriceQueue
{
    private:
        // These track the front and rear of the queue.
        People *front;
        People *rear;
    public:
        PriceQueue(); // Constructor.
        ~PriceQueue();
        // Member functions.
        void enqueue(string, int);
        void dequeue(People *&);
        bool isEmpty(); 
        bool is_in_queue(string, int);
        void increase_price(string, int, int);
        void print_queue();
        void clear();
};

#endif