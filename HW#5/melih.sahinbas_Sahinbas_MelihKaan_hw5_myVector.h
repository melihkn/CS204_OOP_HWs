//Fall 2023 CS204 HW#5 -- myVector.h

/*******************************************************************************************************
    IMPLEMENT FOLLOWING CLASSES
    YOU CAN ADD ANY FUNCTION/STRUCT/CLASS THAT YOU WILL USE HOWEVER YOU CANNOT TOUCH MAIN FUNCTION
******************************************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <type_traits>

using namespace std;

//this our structure that stores value and key datas(T1 is the type of value and T2 is the type of key)
template <typename T1, typename T2>
struct node {

    T1 value;
    T2 key;

    //userdefined and predefined constructors
    node (T1 val, T2 key_) : value(val), key(key_) {}
    node(){}
};

//this is our templated myVector class, which uses std::vector class features
template <typename T1, typename T2>
class myVector {
public:
    //default constructor
    myVector<T1, T2>();

    //copy constructor
    myVector<T1, T2>(myVector<T1, T2> & copy_list);

    //this function changes the value of vector_name
    void setName(const string & name_to_set);

    //returns vector name
    string getName() const;

    //if size = 0 returns false, otherwise returns true
    bool isEmpty() {return (size == 0);}

    //this operator overload returns value of the given list inside [] operator, again T2 is the class of key so parameter's class is
    //T2 and value's class is T1 so return type of function is T1 pointer because function supposed to return address of wanted value
    T1 * operator [] (const T2 & key);
        

    //this function returns the object itself
    myVector<T1, T2> & begin();

    //this function removes the element at the given key, if the key exists
    void remove (T2 & key_to_delete);

    //adds given values to list
    void push_back(T1 & value, T2 & key);

    bool operator!=( const myVector<T1, T2>& rhs);

    //this function checks whether lhs and rhs objects are same or not
    bool operator == (myVector<T1, T2> & rhs);

    // copy rhs myVector object to lhs myVector object
    myVector<T1, T2> & operator = (myVector<T1, T2> & rhs);

    //processing data and calculating statistics according to data type
    void process_data();

    //returns list
    vector<node<T1, T2>> & getList();


    //this is iterator class of myVector class
    class Iterator {
    public:
        
        //consturtors
        Iterator();
        Iterator(myVector<T1, T2> & vec_);

        //init function to initialize iterator
        void init();

        //check whether iterator at the end of the list or viceversa
        bool is_end();

        //increase index
        void next();

        //changes current_node
        void next_val();

        //returns current node
        node<T1, T2> & current();

        //this function prints the vector
        void printVector();

        //finds given key's index
        bool find (const T2 & old_key, const myVector<T1, T2> & vector_);

        //replace the key of current node
        void replaceKey (const T2 & new_key);

    private:
        myVector<T1, T2> & vec;
        vector<node<T1, T2>> & vec_list;
        typename vector<node<T1, T2>>::iterator first_node;
        //typename vector<node<T1, T2>>::iterator current_node;
        node<T1, T2> current_node;
        int current_idx;
    };

private:
    string vector_name;
    vector<node<T1, T2>> list;
    int size;
};


/*******************************************************************************************************/
