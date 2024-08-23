#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <ctime>

using namespace std;

// assigned global variable for id and airlines id because we should access and modify it at everywhere of the program because some of the given function does not have parameters such as id or airline id
int id = 0, airlines_id = 0; 

struct flight{
  string from;
  string to;
  int hour;
  int min;
  int price;
  int ID;
  
  flight *next;
  flight *prev;

  //this constructor for adding flights to vector while reading the flights from txt file
  flight(const string &from, const string &to, const int &hour, const int &min, const int &price)
    : from(from), to(to), hour(hour), min(min), price(price)
  {}

  //this constructor for our linkedlist structure, this constructor is used while creating linkedlist structure to add flights
  flight(const string &from, const string &to, const int &hour, const int &min, const int &price, const int &ID, flight *next, flight *prev)
    : from(from), to(to), hour(hour), min(min), price(price), ID(ID), next(next), prev(prev)
  {}
};

struct airline{
  string name;
  int ID;
  airline *next;
  flight *flights;

  //this constructor for airlines which will be added to linkedlist
  airline(const string &name, const int &ID, airline *next, flight *flights)
    : name(name), ID(ID), next(next), flights(flights)
  {}
};


pair<vector<string>, vector<vector<flight>>> read_files(bool input_done){
  
  ///READ FLIGHTS///
  vector<string> airlines;
  vector<vector<flight>> flights(100); //start with a previously sized vector or you get seg fault (for sure)
  
  while(! input_done){
    string airline_name;
    string filename;
    cout << "Enter the name of airline: ";
    cin >> airline_name;
    cout << "Enter the name of airline file: ";
    cin >> filename;
    ifstream input(filename);

    if(filename == "done"){
      input_done = true;
      return make_pair(airlines, flights);
    }
    
    while( input.fail() ){ //force user enter valid filename until user gives
      cout << "Could not open file: " << filename << endl;
      cout << "Enter the name of airline: ";
      cin >> airline_name;
      cout << "Enter the name of airline file: ";
      cin >> filename;
      input.open(filename);

      if(filename == "done"){
      input_done = true;
      return make_pair(airlines, flights);
      }
    }
    
    airlines.push_back(airline_name);
    
   
    cout << "Processing input file: " << filename <<endl;
    
    //creates a row of matrix, corresponds to given airline
    int line_no = 0;
    vector<string> lines;
    string line;
    int vector_location = airlines.size() - 1;
    while(getline(input, line)){
      lines.push_back(line);
      line_no += 1;
      if(line_no % 5 == 0){
	      flight a_flight(lines[line_no-5], lines[line_no-4], stoi(lines[line_no-3]), stoi(lines[line_no-2]), stoi(lines[line_no-1]));
	      flights[vector_location].push_back(a_flight);
      }
    }
  }
  if(input_done){
    cout << "Input files are already read, however you can do manual changes.." << endl;
  }

  return make_pair(airlines, flights);
}
//function to check which flight takes more time and returns true if first one is longer, false vice-versa
bool flight_time_check(flight *f1, flight *f2){
  if (f1->hour != f2->hour)
    return (f1->hour > f2->hour);
  return (f1->min > f2->min);
}

//this funciton can be used both for adding single flight manually and for adding flights from vector
flight *add_flight_ordered(flight *flight_to_add, airline *&head, const string &airline_name){
  airline *temp_airline = head;
  while (temp_airline && temp_airline->name != airline_name){// to find the airline of the flight
    temp_airline = temp_airline->next;
  }

  flight *flight_head = temp_airline->flights; //

  //this is to check whether list is empty or new flight should be at the beginning of the list
  if (!flight_head || flight_time_check(flight_head, flight_to_add)){ 
    flight_to_add->next = flight_head;
    if (flight_head) 
      flight_head->prev = flight_to_add;
    return flight_to_add;
  }

  flight *temp_flight = flight_head;
  //itearte over until end of the list or until finding the one later than the flight that want to added
  while (temp_flight->next && !flight_time_check(temp_flight->next, flight_to_add)){
    temp_flight = temp_flight->next;
  }

  //adding flight sorted way
  flight_to_add->next = temp_flight->next;
  if (temp_flight->next) //to check flight to prevent error that caused last element
    temp_flight->next->prev = flight_to_add;
  temp_flight->next = flight_to_add;
  flight_to_add->prev = temp_flight;
  
  return flight_head; //return head of the flights to be pointed by its airline's flight
}
// to check whether airline is exist or not, because while adding flights manually we create new airline if it does not exist
bool is_airline_exists(airline *head, const string &airline_name){ 
  airline *temp_airline = head;
  while(temp_airline){
    if (temp_airline->name == airline_name) //return true if there is this airline
      return true;
    temp_airline = temp_airline->next;
  }
  return false; //returns false if there are no such airline in the list
}

// to add a ariline to end of the list, this function can be used both manual entries and reading from vector/txt file
airline * add_airline(airline *&head, airline *airline_to_add){ 
  airline * temp_airline = head;
  if (!temp_airline){ //if linked list is empty head becomes added airline, this is for reading from vector
    return airline_to_add;
  }
  while (temp_airline->next){ // to find the end of the linked list it is used for both manual entries and reading from vector
    temp_airline = temp_airline->next;
  }
  temp_airline->next = airline_to_add; //the last one becomes added airline

  return head; //returns the head of airlines also, head of our structure
}


//this function is for adding manual flights from user, it contains required functions to add a flight to our structure
void add_flight_with_input(airline *&head){
  string airline_name, from, to, hour, min, price;
  int hour_, min_, price_;
  cout << "Adding manual entry:" << endl;
  cout << "AIRLINE: ";
  cin >> airline_name;
  cout << "FROM: ";
  cin >> from;
  cout << "TO: ";
  cin >> to;
  cout << "HOUR: ";
  cin >> hour;
  cout << "MIN: ";
  cin >> min;
  cout << "PRICE: ";
  cin >> price;

  flight *flight_to_add = new flight(from, to, stoi(hour), stoi(min), stoi(price), id, nullptr, nullptr); //create flight to be added



  if (! is_airline_exists(head, airline_name)){ //check airline if not exist create new airline

    airline *airline_to_add = new airline(airline_name, airlines_id, nullptr, nullptr);
    head = add_airline(head, airline_to_add);
    airlines_id++;
    airline_to_add->flights = add_flight_ordered(flight_to_add, head, airline_name);
  }
  else { //if airline exist we find the airline and add new flight
  
    airline *temp_airline = head;
    while(temp_airline){
      if(temp_airline->name == airline_name) {
        temp_airline->flights = add_flight_ordered(flight_to_add, head, airline_name);
        break;
      }
      temp_airline = temp_airline->next;
    }

  }

  cout << "Flight ID " << id << " is added to the list.." << endl;
  id++;
}

//this is for checking airline to find deleted one, returns false is airline becomes empty
bool check_airline(airline *&airline_to_check, const int &delete_id, bool &is_found){ //we control whether we found the deleted one or not with is_found boolean
  flight *temp_flight = airline_to_check->flights;
  flight *to_be_deleted = nullptr;
  bool is_airline_empty = false;

  if (temp_flight->ID == delete_id){//we first chech first flight
    is_found = true;
    to_be_deleted = temp_flight;
    if (temp_flight->next == nullptr){//check whether list contains one flight or not
      is_airline_empty = true;
      airline_to_check->flights = nullptr; //if so list becomes empty
    }
    else{
      temp_flight->next->prev = nullptr;
      airline_to_check->flights = to_be_deleted->next;
    }
    delete to_be_deleted; //delete flight
  }
  else{ //check the remaining ones
    while (temp_flight->next && ! is_found){
      if (temp_flight->next->ID == delete_id){
        is_found = true;
        to_be_deleted = temp_flight->next;
        if (to_be_deleted->next){ //check whether last one or not because things that will be done change
          to_be_deleted->next->prev = temp_flight;
          temp_flight->next = to_be_deleted->next;
        }
        else{
          temp_flight->next = nullptr;
        }
        delete to_be_deleted;
      }
      else{
        temp_flight = temp_flight->next;
      }
    }
  }

  return is_airline_empty;
}

//this is the main function to remove the flight whose id is given by user
void remove_flight_with_input(airline *&head){
  string id_;
  cout << "Flight id to remove: ";
  cin >> id_;
  int delete_id = stoi(id_);
  airline *temp_airline = head;
  bool is_found = false, is_airline_empty = false;

  is_airline_empty = check_airline(temp_airline, delete_id, is_found); //we check first airline
  if (temp_airline->flights == nullptr){ //if airline is empty we change head and delete the old head for preventing any memory leakage
    head = temp_airline->next;
    delete temp_airline;
  }

  while (!is_airline_empty && temp_airline->next && !is_found){ //to iterate other airlines
    is_airline_empty = check_airline(temp_airline->next, delete_id, is_found); //check current airline
    if (is_airline_empty){ //if airline is empty change connect the previous and next ones and delete the empty airline
      airline *to_be_deleted_airline = temp_airline->next;
      temp_airline->next = to_be_deleted_airline->next;
      delete to_be_deleted_airline;
      to_be_deleted_airline = nullptr;

    }
    else{//if not continue to iterate
      temp_airline = temp_airline->next;
    }
    
  }
  if (is_found){ //if the flight found, print a message to user
    cout << "Flight ID " << delete_id << " is removed from the list.." << endl;
  }
  temp_airline = nullptr;
  delete temp_airline;
}


airline* make_linked_list_structure(vector<string> airlines, vector<vector<flight>> flights){
  
  int a_size = airlines.size(), f_size;
  airline *head_airlines = nullptr; //assigning this airline to add real airlines to linkedlist
  airline *temp = head_airlines; // created to iterate over linkedlist without losing head of linkedlist
  vector<flight> airlines_flights; //to store the flights of airline
  flight *temp_flight;

  int a = 0, b = 0; //b is starting from 1 because we have added first flight manually, so loop for next ones
  while(a < a_size){//to add all airlines and their flights

    temp = new airline(airlines[a], airlines_id, nullptr, nullptr);
    head_airlines = add_airline(head_airlines, temp);
    airlines_flights = flights[a];
    f_size = airlines_flights.size();
    b = 0;

    while (b < f_size){ //to add airline's flights to its flights pointer

      temp_flight = new flight(airlines_flights[b].from, airlines_flights[b].to, airlines_flights[b].hour, airlines_flights[b].min, airlines_flights[b].price, id, nullptr, nullptr);
      temp->flights = add_flight_ordered(temp_flight, head_airlines, temp->name);
      b++;
      id++;
    }

    a++;
    airlines_id++;
  }

  return head_airlines;
}

// a function to print all the structure
void print_all(airline *head){
  airline *airlines = head;

  if (!head){ //if linkedlist is empty print an error message
    cout << "List is empty.." << endl;
  }

  while (airlines){//print the airlines by one by
    cout << "###################################" << endl;
    cout << "### AIRLINE ID: " << airlines->ID << " ###" << endl << "NAME:" << airlines->name << endl << "FLIGHTS: ";

    flight *temp_flight = airlines->flights;
    while(temp_flight){ //print the flights of airlines one by one
      cout << "#[" << temp_flight->ID << "|" << temp_flight->from << "->" << temp_flight->to << "|" << temp_flight->hour << ":" << temp_flight->min << "|" << temp_flight->price << "TRY]#";
      temp_flight = temp_flight->next;
    }
    cout << endl;
    airlines = airlines->next;
  }
  airlines = nullptr;
  delete airlines; 
}

//this function is to add flight to linkedlist path at the pathfinder, deletes next one and puts new one instead
void add_to_path(flight *&head, flight *current_flight, flight *flight_to_add){
  flight *to_be_deleted;
  if (!head || !current_flight){//if empty add to the first and make this flight head of the linked list
    to_be_deleted = head;
    head = flight_to_add;
    current_flight = flight_to_add;
  }else{ //else delete the current flights next and make it flight to add
    to_be_deleted = current_flight->next;
    current_flight->next = nullptr;
    current_flight->next = flight_to_add;
  }
  delete to_be_deleted; //delete this one to prevent memory leakage because we no longer need this and except to_be_deleted pointer nothing points this
  to_be_deleted = nullptr;
}

//this function is to clear our minimum path linked list when we encounter cheaper route 
void clear_linked_list(flight*& head) {
  while (head){
    flight *to_be_deleted = head;
    head = head->next;
    delete to_be_deleted;
    to_be_deleted = nullptr;
  }
  head = nullptr;
}

//this function copies path linked_list to a copy of minimum path linkedlist
void copy_linked_list(flight *&head_to_copy, flight *temp_head){
  flight *current_flight = temp_head;
  flight *current_copy = nullptr;
  flight *flight_to_add = nullptr;

  //if copy list is empty add to the beginning
  if (!head_to_copy) {
    flight_to_add = new flight(current_flight->from, current_flight->to, current_flight->hour, current_flight->min, current_flight->price, current_flight->ID,nullptr, nullptr);
    head_to_copy = flight_to_add;
    current_copy = head_to_copy;
  }
  
  while (current_flight->next){ //then copy the rest of the list to copy linked list
    flight_to_add = new flight(current_flight->next->from, current_flight->next->to, current_flight->next->hour, current_flight->next->min, current_flight->next->price, current_flight->next->ID,nullptr, nullptr);
    current_copy->next = flight_to_add;
    current_flight = current_flight->next;
    current_copy = current_copy->next;
  }

  //we copy the linked list because if we equal the heads, we cannot seperate minimum path or the current path.
  //So, we copy the list and min path's head and current path's head might have same flights but they are stored at different places at heap.
  //This allows us to store min_path until find cheaper route and then we can change it to the current path if we found cheaper route
}

// this function is to determine whether found path chepaer or not and if it is cheaper we modify the minimum path via copying current route
void find_min_path(flight *&min_head, flight *temp_head, int &min_price, int &temp_price, flight *&head_to_copy){
  
  //temp_price = calculate_path_price(temp_head); // eğer parametre olarak verme işinde başarılı olursam temp_price'ı burada hesaplamama gerek kalmaz
  //bu foknsiyonda min_head'i alamıyorum, ama bu hata temp_head ve veya current_flight ile de alakalı olabilir.
  if (temp_price < min_price){
    clear_linked_list(head_to_copy); //if we will add new route to head_to_copy we should clear the list to prevent memory leakage
    copy_linked_list(head_to_copy, temp_head); //copy the current path to copy path
    min_head = head_to_copy; //min_head pointer points to copied minimum path
    min_price = temp_price; //change the minimum price
  }
}

//this function recursively searchs for best path throught our structure
void search(airline *head, string &departure, string &arrival, int &min_price, int temp_price, int number_of_flights, const int &max_flight, flight *&min_head, flight *&temp_head, flight *current_flight, flight *&head_of_copy){
  airline *temp_airline = head;
  flight *temp_flight, *flight_to_add = nullptr;
  string temp_arrival;
  int final_price, flight_price; 
  if (number_of_flights < max_flight && temp_price < min_price){

    while (temp_airline){ //search every airline
      temp_flight = temp_airline->flights;

      while (temp_flight){ //search every flight of temp_airline

        if (temp_flight->from == departure){ //if we encounter same departure
          delete flight_to_add;
          flight *flight_to_add = new flight(temp_flight->from, temp_flight->to, temp_flight->hour, temp_flight->min, temp_flight->price, temp_flight->ID, nullptr, nullptr);
          flight_price = flight_to_add->price;

          if (flight_to_add && flight_to_add->to == arrival){  //if we encounter the arrival we add the flight to our path and check path's price

            final_price = 0;
            final_price = temp_price + flight_price;
            add_to_path(temp_head, current_flight, flight_to_add);
            find_min_path(min_head, temp_head, min_price, final_price, head_of_copy); //we call this function to change path and price if temp price is cheaper
          }
          else if (number_of_flights + 1 < max_flight && temp_price + flight_price < min_price){ //if flight number and minimum price is not exceeded

            if (current_flight){ //this is to prevent consequtive flights such as OTTOWA-KITCHENER and KITCHENER-OTTOWA
              if (current_flight->from != flight_to_add->to)
                add_to_path(temp_head, current_flight, flight_to_add);
                search(head, temp_flight->to, arrival, min_price, temp_price + flight_price, number_of_flights + 1, max_flight, min_head, temp_head, flight_to_add, head_of_copy);
            }
            else{
              add_to_path(temp_head, current_flight, flight_to_add);
              search(head, temp_flight->to, arrival, min_price, temp_price + flight_price, number_of_flights + 1, max_flight, min_head, temp_head, flight_to_add, head_of_copy);
            }
          }
        }
        temp_flight = temp_flight->next;
      }
      temp_airline = temp_airline->next;
    }
  }
}

//prints the best path or error message if there is no such path
void print_best_path(flight *min_head, const int &min_price, const string &departure, const string &arrival){

  if (min_head){
    cout << "##Best price path##" << endl;
    flight *temp_flight = min_head;
    
    //printing cheapest route
    while (temp_flight) {
      cout << "[" << temp_flight->ID << "|" << temp_flight->from << "->" << temp_flight->to << "|" << temp_flight->hour << ":" << temp_flight->min << "|" << temp_flight->price << "TRY]";
      if (!temp_flight->next) {
        cout << " $TOTAL PRICE: " << min_price << endl;
      }
      else {
        cout << "->";
      }
      temp_flight = temp_flight->next;  
    }
  }
  else{  //error message displayed when program cannot found any path between departure city and arrival city
    cout << "No path found between " << departure << " and " << arrival <<endl;
  }
  
}

//this is our main pathfinder function, calls required functions inside of it and takes inputs
int pathfinder(airline* head){
  //creating variables
  string from, to, step;
  int max_step, min_price = INT_MAX, temp_price = 0, max_flight, number_of_flights = 0;
  flight *min_head = nullptr, *temp_head = nullptr, *head_of_copy = nullptr;

  //taking inputs
  cout << "Where are you now?" << endl;
  cin >> from;

  cout << "Where do you want to go" << endl;
  cin >> to;

  cout << "Maximum number of transfers: " << endl;
  cin >> step;

  max_step = stoi(step);
  max_flight = max_step + 1;

  flight *current_flight_= nullptr; 
  
  //finding cheapest route
  search(head, from, to, min_price, temp_price, number_of_flights, max_flight, min_head, temp_head, current_flight_, head_of_copy);
  //printing cheapest route
  print_best_path(min_head, min_price, from, to);

  return 1;
}

//this function is to delete a double linkedlist
void delete_double_linked_list(flight *&current_flight){
  
  while (current_flight){
    flight *to_be_deleted = current_flight;
    current_flight = current_flight->next;
    delete to_be_deleted;
    to_be_deleted = nullptr;
  }
}

//this function is to delete airlines and its flights (using function above(delete_double_linked_list) )
void delete_linked_list_recursive(airline *&head){
  airline *to_be_deleted = head;
  delete_double_linked_list(head->flights);
  head = head->next;
  delete to_be_deleted;
  to_be_deleted = nullptr;
  if (head){
    delete_linked_list_recursive(head);
  }
}

//main function to delete our structure 
void delete_linked_list(airline* &head){ 
  //if list not empty initiate the recursive function to delete all the flights
  if (!head){
    delete_linked_list_recursive(head);
  }
  else { //if list is empty print an error message
    cout << "List is empty..";
  }
}



//
void printMainMenu() {
  cout << endl;
  cout <<"I***********************************************I"<<endl
       <<"I               0 - DELETE DATA                 I"<<endl
       <<"I               1 - READ FILES                  I"<<endl
       <<"I               2 - PRINT ALL FLIGHTS           I"<<endl
       <<"I               3 - ADD FLIGHT                  I"<<endl
       <<"I               4 - REMOVE FLIGHT               I"<<endl
       <<"I               5 - PATH FINDER                 I"<<endl
       <<"I               6 - EXIT                        I"<<endl
       <<"I***********************************************I"<<endl
       <<">>";
  cout << endl;
}  

void processMainMenu() {

  pair<vector<string>, vector<vector<flight>>> lines_flights;
  airline* head;
  
  char input;
  do{
    printMainMenu();
    cout << "Please enter your option " << endl;
    cin >> input;
    bool input_done;
    switch (input) {
    case '0':
      delete_linked_list(head);
      cout << "Data is destroyed.." << endl;
      input_done = false;
      break;
    case '1':
      if(! input_done){
	      lines_flights = read_files(input_done);
	      head = make_linked_list_structure(lines_flights.first, lines_flights.second);
	      cout << "Files are read.." << endl;
      }
      else{
	      cout << "Files are already read.." << endl;
      }
      input_done = true;
      break;
    case '2':
      print_all(head);
      break;
    case '3':
      add_flight_with_input(head);
      break;
    case '4':
      remove_flight_with_input(head);
      break;
    case '5':
      pathfinder(head);
      break;
    case '6':
      cout << "Exiting.." << endl;
      exit(0);
    default:
      cout << "Invalid option: please enter again" << endl;
    }
  } while(true);
}
//


int main(){
  
  processMainMenu();
  
  return 0;
  
}



  
