//Fall 2023 CS204 HW#5 -- main.cpp
/*   Melih Kaan Sahinbas HW5 31317   */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "melih.sahinbas_Sahinbas_MelihKaan_hw5_myVector.h"

//this function adds a new node to a sorted list in a sorted way in order to use the vector to find median
template <typename T1, typename T2>
void Insert(node<T1, T2> & node_to_add, vector<node<T1, T2>> & mylist) {
    mylist.push_back(node_to_add);
    int idx = mylist.size() - 1;
    node<T1, T2> temp;

    while (idx > 0 && mylist.at(idx-1).value > node_to_add.value) {
        temp = mylist[idx];
        mylist[idx] = mylist[idx-1];
        mylist[idx-1] = temp;
        idx--;
    }
}

//default constructor
template <typename T1, typename T2>
myVector<T1, T2>::myVector() {}

//copy constructor
template <typename T1, typename T2>
myVector<T1, T2>::myVector(myVector<T1, T2> & copy_list) {
    list = copy_list.list;
    vector_name = copy_list.vector_name;
    size = copy_list.size;
}

//this function changes the value of vector_name
template <typename T1, typename T2>
void myVector<T1, T2>::setName(const string & name_to_set) {vector_name = name_to_set;}

//returns vector name
template <typename T1, typename T2>
string myVector<T1, T2>::getName() const {return vector_name;}

//this operator overload returns value of the given list inside [] operator, again T2 is the class of key so parameter's class is
//T2 and value's class is T1 so return type of function is T1 pointer because function supposed to return address of wanted value
template <typename T1, typename T2>
T1 * myVector<T1, T2>::operator [] (const T2 & key) {
    node<T1, T2> current_node;
    T1 * found_val = nullptr;
    int current_idx = 0;

    //iterates until encounter wanted key, return it's value's address, otherwise returns nullptr as found_val
    while (current_idx < list.size()) {
        current_node = list[current_idx];
        if (current_node.key == key) {
            return &(list[current_idx].value);
        }
        else {
            current_idx++;
        }
    }
    return found_val;
}

//this function returns the object itself
template <typename T1, typename T2>
myVector<T1, T2> & myVector<T1, T2>::begin() {return *this;}

//this function removes the element at the given key, if the key exists
template <typename T1, typename T2>
void myVector<T1, T2>::remove (T2 & key_to_delete) {
    node<T1, T2> current;
    Iterator lhs_iterator(*this);
    lhs_iterator.init();
    int current_idx = 0;
    while (!lhs_iterator.is_end()) {
        //current_idx = lhs_iterator.currentIdx();
        current = list[current_idx];
        if (current.key == key_to_delete) {
            list.erase(list.begin() + current_idx);
            size--;
            break;
        }
        else {
            lhs_iterator.next();
            current_idx++;
        }
    }
}

//adds given values to list
template <typename T1, typename T2>
void myVector<T1, T2>::push_back(T1 & value, T2 & key) {
    node<T1, T2> node_to_add(value, key);
    list.push_back(node_to_add);
    size++;
}


template <typename T1, typename T2>
bool myVector<T1, T2>::operator!=( const myVector<T1, T2>& rhs) {
    return this->getName() != rhs.getName();
}

//this function checks whether lhs and rhs objects are same or not
template <typename T1, typename T2>
bool myVector<T1, T2>::operator == (myVector<T1, T2> & rhs)  {
    int current_idx = 0;
    node<T1, T2> lhs_current, rhs_current;
    //first checks their sizes
    if (this->list.size() != rhs.list.size()) return false;
    //else if (this->getName() != rhs.getName()) return false;
    
    //iterate until finding different key or value, if encounter returns false, else returns false at the end
    while (current_idx < list.size()) {
        lhs_current = list[current_idx];
        rhs_current = rhs.getList()[current_idx];
        if (lhs_current.key != rhs_current.key || lhs_current.value != rhs_current.value) {
            return false;
        }
        else {
            current_idx++;
        }
    }
    return true;
}

// copy rhs myVector object to lhs myVector object
template <typename T1, typename T2>
myVector<T1, T2> & myVector<T1, T2>::operator = (myVector<T1, T2> & rhs) {
    this->list = rhs.list;
    this->vector_name = rhs.vector_name;
    this->size = rhs.size;
    return *this;
}

//processing data and calculating statistics according to data type
template <typename T1, typename T2>
void myVector<T1, T2>::process_data() {
    Iterator process_iterator(*this);
    //if value type is numeric calculate this way (mean, median, standard deviation, max, mode)
    if constexpr (is_same<T1, int>::value || is_same<T1, double>::value) {

        double mean = 0.0, median = 0.0, standard_deviation = 0.0, total = 0.0, variance = 0;
        T2 max_key;
        vector<node<T1, T2>> sorted_vector;
        int current_idx = 0, count = 0, mode_count = INT_MIN;
        T1 value, mode, max = 0;

        while (current_idx < list.size()) {
            value = list[current_idx].value;
            Insert(list[current_idx], sorted_vector);//insertion sort a new list to calculate median
            total += value;
            count = 0;

            //check max
            if (value > max) {
                max = value;
                max_key = list[current_idx].key;
            }

            //find mode
            for (int i = 0; i < list.size(); i++) {
                if (list[i].value == value) count++;
            }
            if (count > mode_count) {
                mode = value;
                mode_count = count;
            }
            current_idx++;
        }

        //finding mean
        if (sorted_vector.size() % 2 == 0) {
            median = (sorted_vector[sorted_vector.size()/2 - 1].value + sorted_vector[sorted_vector.size()/2].value) / 2.0;
        }
        else {
            median = sorted_vector[sorted_vector.size()/2].value;
        }
        mean = total / sorted_vector.size();


        //calcualte variance and standard deviation
        int i = 0;
        while (i < list.size()) {
            value = list[i].value;
            variance += (value - mean) * (value - mean);
            i++;
        }
        standard_deviation = pow(variance/sorted_vector.size(), 0.5);


        //print statistics to console
        cout << "Mean: " << mean << endl << "Median: " << median << endl << "Standard Deviation: " << standard_deviation 
        << endl << "Max: " << max << endl << "Mode: " << mode << endl;
        
    }
    //if tpye of T1 is string calculate most frequent character and word in the list and print their frequency
    else {
        vector<int> letter_count(128, 0);
        string word, max_word;
        char letter, max_letter;
        int current_idx = 0, idx, max = 0, count, max_l = 0;

        while(current_idx < list.size()) {
            word = list[current_idx].value;
            count = 0;

            //calculate most frequent word
            for (int i = 0; i < list.size(); i++) {
                if (list[i].value == word) count++;
            }

            if (count > max || (count == max && word < max_word)) {
                max = count;
                max_word = word;
            }

            //calculate frequency of every letter
            for (int j = 0; j < word.length(); j++) {
                letter = word[j];
                letter_count[letter]++;
            }
            current_idx++;
        }

        //find letter with maximum frequency
        for (int i = 0; i < letter_count.size(); i++) {
            if (letter_count[i] > max_l || (letter_count[i] == max_l && i < max_letter)) {
                max_l = letter_count[i];
                max_letter = i;
            }
        }

        //print the statistics to the console
        cout << "Most Frequent Character: '" << max_letter << "' (Frequency: " << max_l << ")" << endl;
        cout << "Most Frequent Word: \"" << max_word << "\" (Frequency: " << max << ")" << endl;
    }
    
}

//returns list
template <typename T1, typename T2>
vector<node<T1, T2>> & myVector<T1, T2>::getList() {return list;}

//constructors
template <typename T1, typename T2>
myVector<T1, T2>::Iterator::Iterator() {}

template <typename T1, typename T2>
myVector<T1, T2>::Iterator::Iterator(myVector<T1, T2> & vec_) : vec(vec_), vec_list(vec_.getList()), first_node(vec_.getList().begin()){
    vec_list = vec.getList();
    current_idx = 0;
}

//init function to initialize iterator
template <typename T1, typename T2>
void myVector<T1, T2>::Iterator::init() {
    current_idx = 0;
    current_node = vec_list[current_idx];
    
}

//check whether iterator at the end of the list or viceversa
template <typename T1, typename T2>
bool myVector<T1, T2>::Iterator::is_end() {
    return (current_idx == vec_list.size()); //current_idx passed the last element 
}

//increase index
template <typename T1, typename T2>
void myVector<T1, T2>::Iterator::next() {
    current_idx++;
}

//changes current_node
template <typename T1, typename T2>
void myVector<T1, T2>::Iterator::next_val() {
    current_node = vec_list[current_idx];
}

//returns current node
template <typename T1, typename T2>
node<T1, T2> & myVector<T1, T2>::Iterator::current() {return current_node;}

//this function prints the vector
template <typename T1, typename T2>
void myVector<T1, T2>::Iterator::printVector() {

    init();

    while (current_idx < vec_list.size()) {
        next_val();
        cout << "Value: " <<  current_node.value << ", Key: " << current_node.key << endl;
        next();
    }
}

//finds given key's index
template <typename T1, typename T2>
bool myVector<T1, T2>::Iterator::find (const T2 & old_key, const myVector<T1, T2> & vector_) {

    init();

    while (!is_end()) {
        next_val();
        if (current_node.key == old_key) {
            return true;
        }
        else {
            next();
        }
    }
    return false;
}

//replace the key of current node
template <typename T1, typename T2>
void myVector<T1, T2>::Iterator::replaceKey (const T2 & new_key) {
    vec_list[current_idx].key = new_key;
}

/*******************************************************************************************************
    DO NOT CHANGE THE MAIN FUNCTION BELOW
    YOU WILL NOT GET ANY POINTS FROM THIS HOMEWORK IF YOU CHANGE IT
*******************************************************************************************************/
int main()
{
    //Vector and iterator declarations
    myVector<std::string, int> vecStringInt;
    vecStringInt.setName("vecStringInt");
    myVector<std::string, int>::Iterator itrStringInt(vecStringInt.begin());

    myVector<double, double> vecDoubleDouble;
    vecDoubleDouble.setName("vecDoubleDouble");
    myVector<double, double>::Iterator itrDoubleDouble(vecDoubleDouble.begin());

    myVector<int, std::string> vecIntString;
    vecIntString.setName("vecIntString");
    myVector<int, std::string>::Iterator itrIntString(vecIntString.begin());

    myVector<double, std::string> vecDoubleString;
    vecDoubleString.setName("vecDoubleString");
    myVector<double, std::string>::Iterator itrDoubleString(vecDoubleString.begin());

    myVector<std::string, int> vecStringInt2;
    vecStringInt2.setName("vecStringInt2");
    myVector<std::string, int>::Iterator itrStringInt2(vecStringInt2.begin());

    myVector<double, double> vecDoubleDouble2;
    vecDoubleDouble2.setName("vecDoubleDouble2");
    myVector<double, double>::Iterator itrDoubleDouble2(vecDoubleDouble2.begin());

    myVector<int, std::string> vecIntString2;
    vecIntString2.setName("vecIntString2");
    myVector<int, std::string>::Iterator itrIntString2(vecIntString2.begin());

    myVector<double, std::string> vecDoubleString2;
    vecDoubleString2.setName("vecDoubleString2");
    myVector<double, std::string>::Iterator itrDoubleString2(vecDoubleString2.begin());

    //File reading
    std::string filename;
    std::cout << "Enter the filename: ";
    std::getline(std::cin, filename);
    std::ifstream inFile(filename);

    if (!inFile.is_open() || inFile.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "Error: Input file is empty or cannot be opened." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string command, vecName, key;
        iss >> command;

        if (command.empty()) {
            continue; // Skip empty lines or lines with only spaces
        }

        if (command == "push") {
            std::string stringKey;
            int intKey;
            double doubleKey;
            iss >> vecName;
            if (vecName == "vecStringInt") {
                std::string stringValue;
                iss >> intKey >> stringValue;
                vecStringInt.push_back(stringValue, intKey);
            } else if (vecName == "vecDoubleDouble") {
                double doubleValue;
                iss >> doubleKey >> doubleValue;
                vecDoubleDouble.push_back(doubleValue, doubleKey);
            } else if (vecName == "vecIntString") {
                int intValue;
                std::string stringValue;
                iss >> stringKey >> intValue;
                vecIntString.push_back(intValue, stringKey);
            } else if (vecName == "vecDoubleString") {
                double doubleValue;
                iss >> stringKey >> doubleValue;
                vecDoubleString.push_back(doubleValue, stringKey);
            }
        }
        else if (command == "remove") {
            std::string stringKey;
            int intKey;
            double doubleKey;
            iss >> vecName;

            if (vecName == "vecStringInt") {
                iss >> intKey;
                vecStringInt.remove(intKey);
            } else if (vecName == "vecDoubleDouble") {
                iss >> doubleKey;
                vecDoubleDouble.remove(doubleKey);
            } else if (vecName == "vecIntString") {
                iss >> stringKey;;
                vecIntString.remove(stringKey);
            } else if (vecName == "vecDoubleString") {
                iss >> stringKey;
                vecDoubleString.remove(stringKey);
            }
        }
        else if (command == "find") {
            std::string stringKey;
            int intKey;
            double doubleKey;
            iss >> vecName;
            if (vecName == "vecStringInt") {
                iss >> intKey;
                std::string* foundValue = vecStringInt[intKey];
                if (foundValue) std::cout << "Found: Key = " << intKey << ", Value = " << *foundValue << std::endl;
                else std::cout << "Key not found: " << intKey << std::endl;
            } 
            else if (vecName == "vecDoubleDouble") {
                iss >> doubleKey;
                double* foundValue = vecDoubleDouble[doubleKey];
                if (foundValue) std::cout << "Found: Key = " << doubleKey << ", Value = " << *foundValue << std::endl;
                else std::cout << "Key not found: " << doubleKey << std::endl;
            }
            else if (vecName == "vecIntString") {
                iss >> stringKey;
                int* foundValue = vecIntString[stringKey];
                if (foundValue) std::cout << "Found: Key = " << stringKey << ", Value = " << *foundValue << std::endl;
                else std::cout << "Key not found: " << stringKey << std::endl;
            }
            else if (vecName == "vecDoubleString") {
                iss >> stringKey;
                double* foundValue = vecDoubleString[stringKey];
                if (foundValue) std::cout << "Found: Key = " << stringKey << ", Value = " << *foundValue << std::endl;
                else std::cout << "Key not found: " << stringKey << std::endl;
            }
        }

        else if (command == "replace_value") {
            std::string stringKey;
            int intKey;
            double doubleKey;
            iss >> vecName;
            if (vecName == "vecStringInt") {
                std::string stringValue;
                iss >> intKey >> stringValue;
                std::string* foundValue = vecStringInt[intKey];
                if (foundValue) *foundValue = stringValue;
                else std::cout << "Key not found for replacement: " << intKey << std::endl;
            } 
            else if (vecName == "vecDoubleDouble") {
                double doubleValue;
                iss >> doubleKey >> doubleValue;
                double* foundValue = vecDoubleDouble[doubleKey];
                if (foundValue) *foundValue = doubleValue;
                else std::cout << "Key not found for replacement: " << doubleKey << std::endl;
            }
            else if (vecName == "vecIntString") {
                int intValue;
                iss >> stringKey >> intValue;
                int* foundValue = vecIntString[stringKey];
                if (foundValue) *foundValue = intValue;
                else std::cout << "Key not found for replacement: " << stringKey << std::endl;
            }
            else if (vecName == "vecDoubleString") {
                double doubleValue;
                iss >> stringKey >> doubleValue;
                double* foundValue = vecDoubleString[stringKey];
                if (foundValue) *foundValue = doubleValue;
                else std::cout << "Key not found for replacement: " << stringKey << std::endl;
            }
        }

        if (command == "replace_key") {
            std::string oldKeyStr, newKeyStr;
            int oldKeyInt, newKeyInt;
            double oldKeyDouble, newKeyDouble;
            
            iss >> vecName;
            if (vecName == "vecStringInt") {
                iss >> oldKeyInt >> newKeyInt;
                if (itrStringInt.find(oldKeyInt, vecStringInt)) {
                    itrStringInt.replaceKey(newKeyInt);
                } else {
                    std::cout << "Key not found: " << oldKeyInt << std::endl;
                }
            } else if (vecName == "vecDoubleDouble") {
                iss >> oldKeyDouble >> newKeyDouble;
                if (itrDoubleDouble.find(oldKeyDouble, vecDoubleDouble)) {
                    itrDoubleDouble.replaceKey(newKeyDouble);
                } else {
                    std::cout << "Key not found: " << oldKeyDouble << std::endl;
                }
            } else if (vecName == "vecIntString") {
                iss >> oldKeyStr >> newKeyStr;
                if (itrIntString.find(oldKeyStr, vecIntString)) {
                    itrIntString.replaceKey(newKeyStr);
                } else {
                    std::cout << "Key not found: " << oldKeyStr << std::endl;
                }
            } else if (vecName == "vecDoubleString") {
                iss >> oldKeyStr >> newKeyStr;
                if (itrDoubleString.find(oldKeyStr, vecDoubleString)) {
                    itrDoubleString.replaceKey(newKeyStr);
                } else {
                    std::cout << "Key not found: " << oldKeyStr << std::endl;
                }
            } else {
                std::cout << "Unknown vector name: " << vecName << std::endl;
            }
        }

        else if (command == "print") {
            std::string stringKey;
            int intKey;
            double doubleKey;
            iss >> vecName;
            if (vecName == "vecStringInt") {
                iss >> intKey;
                std::string* value = vecStringInt[intKey];
                if (value) {
                    std::cout << "Key: " << intKey << ", Value: " << *value << std::endl;
                } else {
                    std::cout << "Error: Key not found: " << intKey << " in vector '" << vecName << "'" << std::endl;
                }
            }
            else if (vecName == "vecDoubleDouble")
            {
                iss >> doubleKey;
                double* value = vecDoubleDouble[doubleKey];
                if (value) {
                    std::cout << "Key: " << doubleKey << ", Value: " << *value << std::endl;
                } else {
                    std::cout << "Error: Key not found: " << doubleKey << " in vector '" << vecName << "'" << std::endl;
                }
            }
            else if (vecName == "vecIntString")
            {
                iss >> stringKey;
                int* value = vecIntString[stringKey];
                if (value) {
                    std::cout << "Key: " << stringKey << ", Value: " << *value << std::endl;
                } else {
                    std::cout << "Error: Key not found: " << stringKey << " in vector '" << vecName << "'" << std::endl;
                }
            }
            else if (vecName == "vecDoubleString")
            {
                iss >> stringKey;
                double* value = vecDoubleString[stringKey];
                if (value) {
                    std::cout << "Key: " << stringKey << ", Value: " << *value << std::endl;
                } else {
                    std::cout << "Error: Key not found: " << stringKey << " in vector '" << vecName << "'" << std::endl;
                }
            }
        }

        else if (command == "print_vector") {
            iss >> vecName;
            if (vecName == "vecStringInt") {
                if (vecStringInt.isEmpty()) {
                    std::cout << "Error: vecStringInt is empty." << std::endl;
                } else {
                    std::cout << "\nPrinting vecStringInt:" << std::endl;
                    itrStringInt.printVector();
                }
            } else if (vecName == "vecDoubleDouble") {
                if (vecDoubleDouble.isEmpty()) {
                    std::cout << "Error: vecDoubleDouble is empty." << std::endl;
                } else {
                    std::cout << "\nPrinting vecDoubleDouble:" << std::endl;
                    itrDoubleDouble.printVector();
                }
            } else if (vecName == "vecIntString") {
                if (vecIntString.isEmpty()) {
                    std::cout << "Error: vecIntString is empty." << std::endl;
                } else {
                    std::cout << "\nPrinting vecIntString:" << std::endl;
                    itrIntString.printVector();
                }
            } else if (vecName == "vecDoubleString") {
                if (vecDoubleString.isEmpty()) {
                    std::cout << "Error: vecDoubleString is empty." << std::endl;
                } else {
                    std::cout << "\nPrinting vecDoubleString:" << std::endl;
                    itrDoubleString.printVector();
                }
            } else {
                std::cout << "Error: Unknown vector name '" << vecName << "'" << std::endl;
            }
        }

        else if (command == "copy") {
            std::string targetVecName;
            iss >> vecName >> targetVecName;

            if (vecName == "vecStringInt" && targetVecName == "vecStringInt2") {
                vecStringInt2 = vecStringInt;
                std::cout << "\nPrinting vecStringInt2:" << std::endl;
                itrStringInt2.printVector();
            } 
            else if (vecName == "vecDoubleDouble" && targetVecName == "vecDoubleDouble2") {
                vecDoubleDouble2 = vecDoubleDouble;
                std::cout << "\nPrinting vecDoubleDouble2:" << std::endl;
                itrDoubleDouble2.printVector();
            } 
            else if (vecName == "vecIntString" && targetVecName == "vecIntString2") {
                vecIntString2 = vecIntString;
                std::cout << "\nPrinting vecIntString2:" << std::endl;
                itrIntString2.printVector();
            } 
            else if (vecName == "vecDoubleString" && targetVecName == "vecDoubleString2") {
                vecDoubleString2 = vecDoubleString;
                std::cout << "\nPrinting vecDoubleString2:" << std::endl;
                itrDoubleString2.printVector();
            } 
            else {
                std::cout << "Error: Incompatible vector types or unknown vector names for copy." << std::endl;
            }
        }

        else if (command == "check_equal") {
            std::string targetVecName;
            iss >> vecName >> targetVecName;

            if (vecName == "vecStringInt" && targetVecName == "vecStringInt2") {
                if (vecStringInt == vecStringInt2) {
                    std::cout << vecName << " and " << targetVecName << " are equal." << std::endl;
                } else {
                    std::cout << vecName << " and " << targetVecName << " are not equal." << std::endl;
                }
            } else if (vecName == "vecDoubleDouble" && targetVecName == "vecDoubleDouble2") {
                if (vecDoubleDouble == vecDoubleDouble2) {
                    std::cout << vecName << " and " << targetVecName << " are equal." << std::endl;
                } else {
                    std::cout << vecName << " and " << targetVecName << " are not equal." << std::endl;
                }
            } else if (vecName == "vecIntString" && targetVecName == "vecIntString2") {
                if (vecIntString == vecIntString2) {
                    std::cout << vecName << " and " << targetVecName << " are equal." << std::endl;
                } else {
                    std::cout << vecName << " and " << targetVecName << " are not equal." << std::endl;
                }
            } else if (vecName == "vecDoubleString" && targetVecName == "vecDoubleString2") {
                if (vecDoubleString == vecDoubleString2) {
                    std::cout << vecName << " and " << targetVecName << " are equal." << std::endl;
                } else {
                    std::cout << vecName << " and " << targetVecName << " are not equal." << std::endl;
                }
            } else {
                std::cout << "Error: Incompatible vector types or unknown vector names for comparison." << std::endl;
            }
        }

        if (command == "process_data") {
            iss >> vecName;
            if (vecName == "vecStringInt") {
                std::cout << "Processing data of vector " << vecStringInt.getName() << ":" << std::endl;
                vecStringInt.process_data();
            } else if (vecName == "vecDoubleDouble") {
                std::cout << "Processing data of vector " << vecDoubleDouble.getName() << ":" << std::endl;
                vecDoubleDouble.process_data();
            } else if (vecName == "vecIntString") {
                std::cout << "Processing data of vector " << vecIntString.getName() << ":" << std::endl;
                vecIntString.process_data();
            } else if (vecName == "vecDoubleString") {
                std::cout << "Processing data of vector " << vecDoubleString.getName() << ":" << std::endl;
                vecDoubleString.process_data();
            } else {
                std::cout << "Error: Unknown vector name '" << vecName << "'" << std::endl;
            }
        }
    }


    inFile.close();
    return 0;
}
/*******************************************************************************************************/