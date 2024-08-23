//CS204 Fall 2023 - HW3: Stacks & Queues and SU services

// Melih Kaan Şahinbaş 31317 //

#include <iostream>
#include <fstream>       // for ifstream
#include <cctype>        
#include <string>
#include <vector>
#include <sstream>      // std::stringstream

using namespace std;

int instructor_serve = 0;

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

#ifndef DYNAMICSTACK_H
#define DYNAMICSTACK_H


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

#ifndef DYNAMICQUEUE_H
#define DYNAMICQUEUE_H



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

#ifndef PRICEQUEUE_H
#define PRICEQUEUE_H


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

#ifndef LINKEDLIST_H
#define LINKEDLIST_H


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


//constructor of ArrayQueue class
ArrayQueue::ArrayQueue(int size) {
	queueArray = new Request[size];
	queueSize = size;
	front = -1;
	rear = -1;
	numofInstructors = 0;
}

//destructor of ArrayQueue class
ArrayQueue::~ArrayQueue() {
	delete[] queueArray;
}

//checks whether given arrayqueue is empty or not
bool ArrayQueue::isEmpty() const{
	if (numofInstructors > 0)
		return false;
	else
		return true;
}

//enqueue member function of ArrayQueue class, creates and inserts element to the end of the queue
void ArrayQueue::enqueue(string name, int id, string function){
	
	// Calculate the new rear position circularly beacuse in this class circular array is used
	rear = (rear + 1) % queueSize;
	
	// create a request variable and then add it to end of the array
	Request request_to_add(name, id, function, nullptr); 
	queueArray[rear] = request_to_add;

	// Update item count.
	numofInstructors++;
	
}

// dequeue member function of ArrayQueue class, dequeues first element and dequeued element is stored in front_ variable which is a reference parameter
void ArrayQueue::dequeue(Request &front_){
	if (isEmpty()) {
		cout << "Attempting to dequeue on empty queue, exiting program...\n";
	}
	else {
		// change the front value of the circular ArrayQueue
		front = (front + 1) % queueSize;
		// take first element to the reference parameter to use in program
		front_ = queueArray[front];
		// decrease the # of instructors
		numofInstructors--;
	}
}


// constructor of LinkedList class which is the class of our main functions structure
LinkedList::LinkedList(){
	head = nullptr;
}

// deconstructor of LinkedList class which is the class of our main functions structure
LinkedList::~LinkedList() {
	//create temporary variables for deconstructor
	operation *to_be_deleted;
	operation *temp_operation;
	Function *function_tbd;

	while (head) {
		temp_operation = head->operations;
		function_tbd = head;

		// delete every function of the given function's operations
		while (temp_operation) {
			to_be_deleted = temp_operation; // tbd points to the deleted one

			temp_operation = temp_operation->next; //points to next element so head of operations is saved

			delete to_be_deleted; // delete tbd operation
		}

		head = head->next; //points to next element so head of structure is stored safely, prevent memory leakage

		delete function_tbd; //delete tbd function after deleting all its operations

	}
}

//member function of our LinkedList class, adds function's to the structure
Function *LinkedList:: add_function_name(string function_name){
	Function *function_to_add = new Function(function_name, nullptr, nullptr);
	if (!head){
		head = function_to_add; //if empty make it first one and head
	}
	else{
		Function *temp_func = head; 

		//otherwise go to end and add the function
		while (temp_func->next) temp_func = temp_func->next;
		temp_func->next = function_to_add; 
	}

	return function_to_add;
}

//member function of our LinkedList class, adds opearitons to the structure according to its function
void LinkedList::add_function(Function *function, string operation_){ 
	operation *temp_func = function->operations;
	operation *op_to_add = new operation(function->name, operation_); //burada struct mı kullanmalı yoksa string mi on hocaya sor bi

	
	if (!temp_func) function->operations = op_to_add; ////if empty make it first that is pointed by its funtion
	else {
		//otherwise go to end and add the operation
		while (temp_func->next){
			temp_func = temp_func->next;
		}

		temp_func->next = op_to_add;
	}	
}

//member function of our LinkedList class to get head of the linkedlist
Function * LinkedList::get_head(){
	return head;
}

//member function of our LinkedList class, checks whether is list is empty or not and return a boolean according to answer
bool LinkedList::isEmpty(){
	if (!head) return true;
	return false;
}

//fmember function of our LinkedList class to print our linked list
void LinkedList::print_list(){

	if (!isEmpty()){
		cout << "------------------------------------------------------------------" << endl;
		cout << "PRINTING AVAILABLE SERVICES (FUNCTIONS) TO BE CHOSEN FROM THE USERS" << endl;
		cout << "------------------------------------------------------------------" << endl << endl << endl;

		Function *temp_func = head;

		while(temp_func){
			cout << temp_func->name << ':' << endl;
			operation *temp_operation = temp_func->operations;
			while(temp_operation){
				if (temp_operation->next) { 
					cout << temp_operation->operation_name << ";, ";
				}
				else {
					cout << temp_operation->operation_name << '.' << endl << endl;
				}
				temp_operation = temp_operation->next;
			}
			temp_func = temp_func->next;
		}
	}
}

//member function of our LinkedList class, checks whether entered function name is in list or not
bool LinkedList::function_name_exists(string function_name) const{
	Function *temp_function = head;

	while (temp_function) {
		if (temp_function->name == function_name) return true; //if encounter wanted function returns true immediately
		temp_function = temp_function->next;
	}

	//if cannot find returns false at the end
	return false;
}

//member function of our LinkedList class, returns the location of function that is being wanted to find
Function *LinkedList::find_function_name(string function_name){
	Function *temp = nullptr;
	
	
	Function *temp_function = head;

	//iterates linkedlist's function part and returns if it finds
	while (temp_function) {
		if (temp_function->name == function_name) {
			temp = temp_function;
			return temp;
		}	
		
		temp_function = temp_function->next;
	}

	//otherwise returns as nullptr
	return temp;
}


//constructor of DynamicQueue class
DynamicQueue::DynamicQueue(){
	front = nullptr;
	rear = nullptr; 
}

//deconstructor of DynamicQueue class
DynamicQueue::~DynamicQueue() {
	Request *temp_request;

	while (front) {
		temp_request = front;
		front = front->next;
		delete temp_request;
	}
}

//member function of DynamicQueue class, returns true if queue is not empty, returns false if viceversa
bool DynamicQueue::isEmpty(){
	if (front == nullptr)
		return true;
	else 
		return false;
}

//member function of DynamicQueue class, creates a request object according to parameters and adds it to the queue
void DynamicQueue::enqueue(string name, int ID, string function){
	//if queue is empty add to beginning
	if (isEmpty()) { 
		front = new Request(name, ID, function, nullptr);
		rear = front;
	}
	//otherwise makes new one as next of rear
	else { 
		rear->next = new Request(name, ID, function, nullptr);
		rear = rear->next;
	} 
}

//member function of DynamicQueue class, dequeues first element and dequeued element is stored in front_ variable which is a reference parameter
void DynamicQueue::dequeue(Request *&front_){
	Request *temp;

	//if empty throw an message
	if (isEmpty()) {
		cout << "Attempting to dequeue on empty queue, exiting program...\n";
		exit(1);
	}
	//otherwise create a new object which includes copy of properties of front and it is saved because ist reference parameter and delete temp from queue
	else { 
		front_ = new Request(front->name, front->ID, front->request_func, nullptr);
		temp = front;
		front = front->next;
		delete temp; 
	}
}


//constructor of PriceQueue class
PriceQueue::PriceQueue() {
	front = nullptr;
	rear = nullptr; 
}

//deconstructor of PriceQueue class
PriceQueue::~PriceQueue() {
	People *temp_people;

	//delete elements one by one from top
	while (front) {
		temp_people = front;
		front = front->next;
		delete temp_people;
	}
}

//member function of PriceQueue class, returns true if queue is not empty, returns false if viceversa
bool PriceQueue::isEmpty(){
	if (front == nullptr)
		return true;
	else 
		return false;
}

//member function of PriceQueue class, creates a request object according to parameters and adds it to the queue
void PriceQueue::enqueue(string name, int ID) {

	People *new_people = new People(name, ID);

	//if empty adds to beginnig
	if (isEmpty()) { 
		front = new_people;
		rear = front;
	}
	//otherwise makes it next of the rear and updates rear to last one(new_people in this case)
	else { 
		rear->next = new_people;
		rear = rear->next;
	} 
}

//member function of PriceQueue class, dequeues first element and dequeued element is stored in front_ variable which is a pointer reference parameter
void PriceQueue::dequeue(People *&front_){
	People *temp_people;

	//if empty displays an message to user
	if (isEmpty()) {
		cout << "Attempting to dequeue on empty queue, exiting program...\n";
		exit(1);
	}
	//otherwise create a new object which includes copy of properties of front and it is saved because ist reference parameter and delete temp from queue
	else { 
		front_ = new People(front->name, front->ID, front->price);
		temp_people = front;
		front = front->next;
		delete temp_people; 
	}
}

//member function of PriceQueue class, checks whether referenced name and ID is exist in queue or not, returns true if exist, returns false if does not exist
bool PriceQueue::is_in_queue(string name, int ID) {
	if (isEmpty()) {
		return false;
	}
	else {
		People *temp_people = front;

		while (temp_people) {
			if (temp_people->name == name && temp_people->ID == ID) 
				return true;
			temp_people = temp_people->next;
		}

		return false;
	} 
}

//member function of PriceQueue class, increases the total price of user's whose name and ID are parameters by price value in the parameters
void PriceQueue::increase_price(string name, int ID, int price) {
	
	
	People *temp_people = front;
	//find the user and increase the price
	while (temp_people) {
		if (temp_people->name == name && temp_people->ID == ID) {
			temp_people->price += price;
			break;
		}
		temp_people = temp_people->next;
	}
}

//member function of PriceQueue class, prints the users' names, IDs, and total prices
void PriceQueue::print_queue() {
	People *temp_people = front;

	if (front == nullptr) {
		cout << "No payments defined." << endl;
	}
	else {
		while (temp_people) {
			cout << "Name: " << temp_people->name << " ID: " << temp_people->ID << " " << temp_people->price << " TRY" << endl;
			temp_people = temp_people->next;
		}
	}
}

//constructor of DynamicStack class
DynamicStack::DynamicStack() {
	top = nullptr;
}

//deconstructor of DynamicStack class, clears the dynamic memory in order to prevent memory leakage
DynamicStack::~DynamicStack() {
	operation *temp_operation; //to_be_deleted operation

	while (top) {
		temp_operation = top;
		top = top->next;
		delete temp_operation;
	}
}

//member function of DynamicStack class, returns true if queue is not empty, returns false if viceversa
bool DynamicStack::isEmpty(void) {
	if (top == nullptr)
		return true;
	else
		return false;
}

//member function of DynamicStack class, creates and pushs operation to stack according to parameters
void DynamicStack::push(string function_name, string function) {

	operation *new_operation;
	
	new_operation = new operation(function_name, function);

	if (isEmpty()) {
		top = new_operation;
		new_operation->next = nullptr;
	}
	else {
		new_operation->next = top;
		top = new_operation;
	}
} 

//member function of DynamicStack class, deletes last element of stack (LIFO) the deleted one's copy is stored at the op_to_delete object which is pointer reference variable
void DynamicStack::pop(operation *&op_to_delete) {
	operation *temp;
	if (isEmpty()) {
		cout << "The stack is empty.\n";
	}
	else {
		op_to_delete = new operation(top->function_name, top->operation_name);
		temp = top->next;
		delete top;
		top = temp;
	}
}
//member function of DynamicStack class, deletes last element of stack (LIFO), this overload does not have any parameter to store deleted one, used when deleted one's informations are not necessary
void DynamicStack::pop(){
	operation *temp_operation;
	if (isEmpty()) {
		cout << "The stack is empty.\n";
	}
	else {
		temp_operation = top->next;
		delete top;
		top = temp_operation;
	}
}

//member function of DynamicStack class, returns the top of the stack
operation *DynamicStack::get_top(){
	return top;
}
//member function of DynamicStack class, prints the stack reversed and recursively
void DynamicStack::print_stack(operation *copy_top) {

	if (copy_top) {
		print_stack(copy_top->next);
		cout << copy_top->function_name << ": " << copy_top->operation_name << ";" << endl;

	}
}

//member function of DynamicStack class, delete's the member of stack whose function name is equal to the string given as parameter
void DynamicStack::clear_stack_from_func(string function_name) {

	while (top && top->function_name == function_name) {
		pop();
	}
}

//recursive function to do the operations of the function that user wanted to process
void processWorkload(DynamicStack &process_stack, LinkedList &Structure, int &price, const string &function_name){
	//finds location of current function's location in our structure(main linkedlist)
	Function *current_function = Structure.find_function_name(function_name);

	if (!(current_function == nullptr)){ //if function exist in structure (find_function_name function return a nullptr if cannot find any function)

		operation *temp_operation = current_function->operations; // assign location of operations of function to temp_operation
		string function;

	
		while(temp_operation) {

			stringstream sstream(temp_operation->operation_name); //divide parts of operations operation name part in order to determine which type of operations will happen

			sstream >> function;

			//if first word is print, print the stacj
			if (function == "print"){
				cout << "Executing print stack command from " << function_name << endl;
				operation *copy_top = process_stack.get_top();
				cout << "PRINTING THE STACK TRACE:" << endl;

				if (process_stack.isEmpty()) cout << "The stack is empty" << endl;
				process_stack.print_stack(copy_top);
				price += 2;
			}
			//if first word is define add operation to the stack
			else if(function == "define"){

				//if operation not in our stack, then add operation (asked to TA after his answer you may modify this part)
				
				process_stack.push(function_name, temp_operation->operation_name);
				price += 1;
			}
			//if first word is call, call function recursively and give function name as the second word of operation name
			else if (function == "call") {
				price += 5;
				string recursive_func_name;
				sstream >> recursive_func_name;

				cout << "Calling " << recursive_func_name << " from " << function_name << endl;
				
				processWorkload(process_stack, Structure, price, recursive_func_name);
				
			}

			//go to next operation and clear the stringstream object
			temp_operation = temp_operation->next;
			sstream.clear();
			sstream.str("");
		}

		//clear stack from this function's operations and prompt and information message to user that points out operations of this function is ended (function means function_1, function_2,...)
		process_stack.clear_stack_from_func(function_name);
		cout << function_name << " is finished. Clearing the stack from it's data..." << endl;
	}

}






void processWorkload(ArrayQueue &instructorsQueue, DynamicQueue &studentsQueue, LinkedList &Structure, PriceQueue & priceQueue)
{
	//if instructors queue is not empty or 10 request are not served for instructors consequtively make the next process of instructors queue
	if (!instructorsQueue.isEmpty() && instructor_serve < 10) {
		cout<<"Processing instructors queue..."<<endl;

		//dequeue first element and assign it to front
		Request front; 
		instructorsQueue.dequeue(front);
		string function_name = front.request_func;

		//creates stack for this operation and assigns price to 0
		DynamicStack processStack;
		int price = 0; 
		
		//process the operations
		cout << "Processing prof." << front.name << "'s request (with ID "<< front.ID << ") of service (function):\n" << function_name <<endl
		<< "-------------------------------------------------------" << endl;
		
        processWorkload(processStack, Structure, price, function_name);

		priceQueue.increase_price(front.name, front.ID, price); //increment user's price to by price

        cout<<"GOING BACK TO MAIN MENU"<<endl;

		instructor_serve++;
	}
	else if (!studentsQueue.isEmpty()) {
        
		//determine whether instructor queue is empty or this is time for students
		if (instructor_serve == 10) {
			cout << "10 instructors are served. Taking 1 student from the queue..." << endl;
		}
		else {
			cout << "Instructors queue is empty. Proceeding with students queue..." << endl;
		}

		instructor_serve = 0;

		//dequeue first element and assign it to front
		Request *front;
		studentsQueue.dequeue(front);
		string function_name = front->request_func;

		//creates stack for this operation and assigns price to 0
		DynamicStack processStack;
		int price = 0;
		
		//process the operations
		cout << "Processing " << front->name << "'s request (with ID " << front->ID << ") of service (function):\n" << function_name << endl
		<< "-------------------------------------------------------"<<endl;
    
		
		processWorkload(processStack, Structure, price, function_name);

		priceQueue.increase_price(front->name, front->ID, price); //increment user's price to by price
		
        cout << "GOING BACK TO MAIN MENU" << endl;
		delete front;
	}
	else {
		cout << "Both instructor's and student's queue is empty.\nNo request is processed." << endl << "GOING BACK TO MAIN MENU" << endl;
	}
}

// function to add inputfile's ingredient to structure
void add_to_structure(LinkedList &structure, ifstream &input_file){
	string line, name;

	getline(input_file, name);
	name = name.substr(0, name.length()-1);

	//add function
	Function * function_name_to_add = structure.add_function_name(name);

	//one by one add operations connected to their function in structure
	while (!input_file.eof()){

		getline(input_file, line);
		line = line.substr(0, line.length()-1);

		structure.add_function(function_name_to_add, line);
	}

	input_file.close();
}

//fucntion to add instructor workload to their queue
void addInstructorWorkload(const LinkedList &structure, ArrayQueue &instructorsQueue, PriceQueue &priceQueue){

	//input function name from user, if function name is not exists in our structure prompt a message to user and go back to main menu
	string function_name;
	cout << "Add a service (function) that the instructor wants to use:" << endl;
	cin >> function_name;

	if (!structure.function_name_exists(function_name)){
		cout << "The requested service (function) does not exist." << 
		endl << "GOING BACK TO MAIN MENU" << endl << endl;
	}
	else{
		//mesela burada ekleme yaparken sadece ID-name ikilisinin mi uniqueliğine bakıcaz, mesela ben kamer 10 girdim melih 10 girersem hata verdirmeli miyiz?

		//get name and id information from user and add operation to instructor queue and add people to peoplequeue if people is not defined beforehand
		string name, ID_input;
		int ID ;
		cout << "Give instructor's name: ";
		cin >> name;

		cout << "Give instructor's ID (an int): ";
		cin >> ID_input;
		ID = stoi(ID_input);

		instructorsQueue.enqueue(name, ID, function_name);

		if (!priceQueue.is_in_queue(name, ID)) {
			priceQueue.enqueue(name, ID);
		}

		cout << "Prof. "<< name << "'s service request of " << function_name << endl << "has been put in the instructor's queue." 
		<< endl << "Waiting to be served..." << endl;
	}
}

//fucntion to add students workload to their queue
void addStudentWorkload(const LinkedList &structure, DynamicQueue &studentsQueue, PriceQueue &priceQueue){

	//input function name from user, if function name is not exists in our structure prompt a message to user and go back to main menu
	string function_name;
	cout << "Add a service (function) that the student wants to use:" << endl;
	cin >> function_name;

	if (!structure.function_name_exists(function_name)){
		cout << "The requested service (function) does not exist." << endl
		<< "GOING BACK TO MAIN MENU" << endl << endl;
	}
	else{
		//get name and id information from user and add operation to student queue and add people to peoplequeue if people is not defined beforehand
		string name, ID_input;
		int ID ;
		cout << "Give student's name: ";
		cin >> name;

		cout << "Give student's ID (an int): ";
		cin >> ID_input;
		ID = stoi(ID_input);

		studentsQueue.enqueue(name, ID, function_name);
		
		//burada ona göre bir hata mesajı?
		if (!priceQueue.is_in_queue(name, ID)) {
			priceQueue.enqueue(name, ID);
		}

		cout << name << "'s service request of "<< function_name << " has been put in the student's queue." << 
		endl << "Waiting to be served..." << endl;
	}

}

//function to print our structure
void displayUsers(PriceQueue &priceQueue){
	priceQueue.print_queue();
}





int main()
{
	string answer;
	string filename;
	bool continue_to_ask = false, entered_correctly = true; //some check variables

	//creating objects of classes that was created by author
	LinkedList structure;
	ArrayQueue instructorsQueue(500);
	DynamicQueue studentsQueue;
	PriceQueue priceQueue;

	//taking input file unless user faces any abnormality

	int a = 0;
	do {
		if (a == 0)
			cout << "If you want to open a service (function) defining file," << endl << "then press (Y/y) for 'yes', otherwise press any single key" << endl;
		else cout << "Do you want to open another service defining file?" << endl << "Press (Y/y) for 'yes', otherwise press anykey" << endl;
		cin >> answer;

		a++;

		if (answer == "y" || answer == "Y") {
			cout << "Enter the input file name: ";
			cin >> filename;
			continue_to_ask = true;
			
			ifstream input_file;
			input_file.open(filename.c_str());

			//if user entered filename is cannot opened, terminate the program (may use exit instead of check boolean variables)
			if (input_file.fail()){
				cout << "Could not open the file " << filename << endl;
				continue_to_ask = false;
				entered_correctly = false;
			}
			else {
				add_to_structure(structure, input_file); //calling function to add ingredients of input file to our structure
			}
			
			//clear input_file variable
			input_file.clear();
			input_file.close();
		}
		else{
			continue_to_ask = false;
		}
	} while(continue_to_ask);

	if (entered_correctly) structure.print_list(); //print the structure if no abnormality happened during input period
	

	while (entered_correctly){		
       cout << endl;
 		cout<<"**********************************************************************"<<endl
	    <<"**************** 0 - EXIT PROGRAM                        *************"<<endl
	    <<"**************** 1 - ADD AN INSTRUCTOR SERVICE REQUEST   *************"<<endl
	    <<"**************** 2 - ADD A STUDENT SERVICE REQUEST       *************"<<endl	
	    <<"**************** 3 - SERVE (PROCESS) A REQUEST           *************"<<endl
		<<"**************** 4 - DISPLAY USER PAYMENTS               *************"<<endl
	    <<"**********************************************************************"<<endl;
		cout << endl;
		int option;
		cout << "Pick an option from above (int number from 0 to 3): ";
		cin>>option;
		switch (option)
		{
			case 0:
				cout<<"PROGRAM EXITING ... "<<endl;
				//burada programdan çıkarken linkedlist içeren classlarla oluşturduğumuz variableların içini temizleyelim mi
				//array için deconstructor, diğerleri içn clear funcyion
				//added deconstructors to all these class that I used in this program and prevented memory leakage through this implementation
				exit(0);
			case 1:
				addInstructorWorkload(structure, instructorsQueue, priceQueue); //function to add instructor workload to their queue
				break;
			case 2:
				addStudentWorkload(structure, studentsQueue, priceQueue); //function to add student workload to their queue
				break;
			case 3:
				processWorkload(instructorsQueue, studentsQueue, structure, priceQueue); //to process workload according to priority criteria of 10 to 1
				break;
			case 4:
				displayUsers(priceQueue); // display users and their price's
				break;
			default:
				cout<<"INVALID OPTION!!! Try again"<<endl;
		}
	}
	return 0;
}