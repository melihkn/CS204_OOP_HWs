#ifndef DYNAMICQUEUE_H
#define DYNAMICQUEUE_H

#include <string>
#include "ArrayQueue.h"
using namespace std;



class DynamicQueue
{
    private:
        // These track the front and rear of the queue.
        Request *front;
        Request *rear;
    public:
        DynamicQueue(); // Constructor.
        ~DynamicQueue(); // Destructor
        // Member functions.
        void enqueue(string, int, string);
        void dequeue(Request *&);
        bool isEmpty();
};

#endif