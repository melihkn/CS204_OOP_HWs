#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <climits>
#include <algorithm>
#include <numeric>
#include <cstdint>

//Melih Kaan Şahinbaş 31317 HW6 CS 204
using namespace std;

constexpr uint64_t c1 = 0xcc9e2d51ull;
constexpr uint64_t c2 = 0x1b873593ull;
constexpr uint64_t r1 = 15;
constexpr uint64_t r2 = 13;
constexpr uint64_t m = 5;
constexpr uint64_t n = 0xe6546b64ull;

uint64_t a1 = 97102761753631939 , b1 = 42506983374872291;
uint64_t a2 = 56842397421741207 , b2 = 18456721873196387;
uint64_t a3 = 61811894731001917 , b3 = 37217717671130671;
uint64_t a4 = 31415926535897931 , b4 = 27182818284590453;
uint64_t a5 = 98765432109876543 , b5 = 57548533468232177;

uint64_t murmur_hash(const std::string& key, uint64_t seed = 0) {
  uint64_t hash = seed;
  const uint8_t* data = reinterpret_cast<const uint8_t*>(key.c_str());
  size_t len = key.length();
  for (size_t i = 0; i < len / 8; ++i) {
    uint64_t k = *reinterpret_cast<const uint64_t*>(data + i * 8);
    k *= c1;
    k = (k << r1) | (k >> (64 - r1));
    k *= c2;
    
    hash ^= k;
    hash = ((hash << r2) | (hash >> (64 - r2))) * m + n;
  }
  const uint8_t* tail = data + (len / 8) * 8;
  uint64_t k1 = 0;
  switch (len & 7) {
  case 7:
    k1 ^= static_cast<uint64_t>(tail[6]) << 48;
  case 6:
    k1 ^= static_cast<uint64_t>(tail[5]) << 40;
  case 5:
    k1 ^= static_cast<uint64_t>(tail[4]) << 32;
  case 4:
    k1 ^= static_cast<uint64_t>(tail[3]) << 24;
  case 3:
    k1 ^= static_cast<uint64_t>(tail[2]) << 16;
  case 2:
    k1 ^= static_cast<uint64_t>(tail[1]) << 8;
  case 1:
    k1 ^= static_cast<uint64_t>(tail[0]);
    k1 *= c1;
    k1 = (k1 << r1) | (k1 >> (64 - r1));
    k1 *= c2;
    hash ^= k1;
  }
  hash ^= len;
  hash ^= (hash >> 37);
  hash *= 0x85ebca6bull;
  hash ^= (hash >> 32);
  return hash;
}

uint64_t ax_b_string_func(uint64_t a, uint64_t b, string to_hash){
  
  uint64_t hash = b;
  for(char c : to_hash){
    hash = hash * a + static_cast<uint64_t>(c);
  }
  
  return (uint64_t) hash;
}

class Sketch {
  public:
    virtual void add(string s) = 0;
};

class MembershipSketch : public Sketch {
  public:
    virtual bool isMember(string s) = 0;
};

class BloomFilter : public MembershipSketch {
  public:

    //constructor of bloomfilter class
    BloomFilter(int k, int size, int _switch) : size(size), k(k), _switch(_switch) {
      int arrs = size/64;
      filter = new uint64_t[arrs];
      fill_n(filter, arrs, 0);
    }

    //function to add s to filter, change the bits value
    void add(string s) {
      uint64_t idx, mod_, start = pow(2,63);
      if (_switch == 1) {
        for (int i = 0; i < k; i++) {
          hash = ax_b_string_func(a[i], b[i], s) % size;
          idx = hash / 64;
          mod_ = hash % 64;
          filter[idx] = filter[idx] | (start >> mod_);
          //start value 2^63 is like 10000....00 right shifting this value as times as mod_ gives us the correct bit
        }
      }
      
      if (_switch == 2) {
        for (int i = 0; i < k; i++) {
          hash = murmur_hash(s, a[i]) % size;
          idx = hash / 64;
          mod_ = hash % 64;
          filter[idx] = filter[idx] | (start >> mod_);
          //start value 2^63 is like 10000....00 right shifting this value as times as mod_ gives us the correct bit
        }
      }
    }

    //check whether parameter s in filter or not
    bool isMember(string s) {
      uint64_t idx, mod_, start = pow(2,63);

      if (_switch == 1) {
        for (int i = 0; i < k; i++) {
          hash = ax_b_string_func(a[i], b[i], s) % size; 
          idx = hash / 64;
          mod_ = hash % 64;
          if ((filter[idx] & start >> mod_) != (start >> mod_)) {return false;} //if not found return true
          //start value 2^63 is like 10000....00 right shifting this value as times as mod_ gives us the correct bit
        }
      }
      else if (_switch == 2) {
        for (int i = 0; i < k; i++) {
          hash = murmur_hash(s, a[i]) % size;
          idx = hash / 64; //find index
          mod_ = hash % 64; //find mod value to insert

          if ((filter[idx] & (start >> mod_)) != (start >> mod_)) {return false;} //if not found return true
          //start value 2^63 is like 10000....00 right shifting this value as times as mod_ gives us the correct bit
        }
      }
      return true;
    }

    int get_size() {return size;}
    int get_k() {return k;}



  private:
    int k; // number of hash functions
    uint64_t* filter; // bit array
    int _switch; // select between hash functions
    int size; // size in bits
    uint64_t a[5] = {a1, a2, a3, a4, a5};
    uint64_t b[5] = {b1, b2, b3, b4, b5};
    uint64_t hash;
};   

class CardinalitySketch : public Sketch {
  public:
    virtual double estimateCardinality() = 0;
};

class HyperLogLog : public CardinalitySketch {
  public:

    HyperLogLog(int b, int _switch) : b(b), _switch(_switch){
      buckets = new uint64_t[b];
      std::fill(buckets, buckets + b, 0);
    }

    //function to count consequtive zeros in hash value
    int countConsecutiveZeros(uint64_t hash_) {
      int count = 0;
      while ((hash_ & 1) == 0 ) {
        count++;
        hash_ >>= 1;
      }
      return count + 1; //plus one should be here
    }

    //adding hash value to buckets
    void add(string s) {
      uint64_t hash;
      //find hash value according to switch value
      if (_switch == 1) {
          hash = ax_b_string_func(a1, b1, s);
      } else {
          hash = murmur_hash(s);
      }

      uint64_t bucketIndex = hash >> uint64_t(64- log2(b)); //find index with lhs
      uint64_t consecutiveZeros = countConsecutiveZeros(hash); //find consequtive zeros at the end of the program

      //change the value of array's index if new consequtivezeros is bigger than the value in the array's found index
      if (buckets[bucketIndex] < consecutiveZeros) buckets[bucketIndex] = consecutiveZeros; 

    }

    //function to implement estimate cardinality function
    double estimateCardinality() {
      double alpha_m;
      // alpha_m values are constants used in the HyperLogLog algorithm
      if (b == 16) alpha_m = 0.673;
      else if (b == 32) alpha_m = 0.697;
      else if (b == 64) alpha_m = 0.709;
      else alpha_m = 0.7213 / (1 + 1.079 / b); // For larger bucket sizes

      double sum = 0.0;
      for (int i = 0; i < b; i++) {
          // Summing the inverses of 2 raised to the power of bucket values
          sum += pow(0.5, buckets[i]); 
      }

      return alpha_m * b * b / sum;
    }

  
  private:
    int b; // number of buckets
    uint64_t* buckets; // bit array
    int _switch; // select between hash functions


};

class KMinVal : public CardinalitySketch {
  public:

    //Constructor of KMinVal class
    KMinVal(int K, int _switch) : K(K), _switch(_switch) {
      val = new uint64_t[K];
      std::fill(val, val + K, -1);
    }

    //add function to store min k value
    void add(string s) {
      uint64_t hash;

      //find hash value according to switch value
      if (_switch == 1) hash = ax_b_string_func(a1, b1, s);
      else if (_switch == 2) hash = murmur_hash(s);

      //insert the value to the correct space in the array
      int i = 0;
      for (i = 0; i < K; i++) {

        if (val[i] == hash) {
            // If the hash value is already in the array, do nothing
            return;
        }

        else if (hash < val[i] || val[i] != -1) break; //break when encounter higher value
      }
      //check whether new hash value should be in array or not and insert it to array
      if (i<K) {
        for (int j = K - 1; j > i; j--) {
          val[j] = val[j-1];
        }
        val[i] = hash;
      }


    } 

    //function to estimate cardinality
    double estimateCardinality() {
      uint64_t max_possible = UINT64_MAX;
      uint64_t kthminval = val[K-1];

      double distinct_count = double(K-1) * max_possible / kthminval;

      return distinct_count;
    }



  private:
    int K; // number of values to keep
    uint64_t* val; // array
    int _switch; // select between hash functions
};




void evaluate_bloom(vector<string> words, BloomFilter *bloom, int i){

  int true_positives = 0;
  int false_positives = 0;
  
  for(int i = 0; i < 100; i++){
    true_positives += (int)bloom->isMember(words[i]);
  }

  for(int i = 0; i < 100; i++){
    false_positives += (int)bloom->isMember(words[words.size() - i - 1]);
  }  
  
  cout << "size: " << bloom->get_size() << " k: " << bloom->get_k() << " Inserted: " << i << " TP: " << true_positives << " FP: " << false_positives << endl;
}

int main(int argc, char** argv){

  string fname;
  fname = argv[1];
  
  ifstream file(fname);
  if(!file.is_open()){
    cout << "Error opening file: " << fname << endl;
    return 1;
  }

  vector<string> words;
  string word;

  //Reading each line into vector of strings
  while(std::getline(file, word)){
    words.push_back(word);
  }

  cout << "#######################################################################################" << endl;
  cout << "HyperLogLog Using ax+b hash" << endl;
  for(int an = 2; an <= 8192; an *= 2){
    HyperLogLog loglog(an, 1);
    for(int i = 0; i < words.size(); i++){
      loglog.add(words[i]);
    }
    
    cout << "buckets: " << an << " HyperLogLog estimate: " << loglog.estimateCardinality() << endl;
  }
  cout << "#######################################################################################" << endl;

  cout << "#######################################################################################" << endl;
  cout << "HyperLogLog Using MurMurHash" << endl;
  for(int an = 2; an <= 8192; an *= 2){
    HyperLogLog loglog(an, 2);
    for(int i = 0; i < words.size(); i++){
      loglog.add(words[i]);
    }
    
    cout << "buckets: " << an << " HyperLogLog estimate: " << loglog.estimateCardinality() << endl;
  }
  cout << "#######################################################################################" << endl;

  cout << "#######################################################################################" << endl;
  cout << "KMinVal Using ax + b hash" << endl;
  for(int an = 2; an <= 8192; an *= 2){
    KMinVal kminval(an, 1);
    for(int i = 0; i < words.size(); i++){
      kminval.add(words[i]);
    }
    cout << "buckets: " << an << " KMinVal estimate: " << kminval.estimateCardinality() << endl;
  }
  cout << "#######################################################################################" << endl;

  cout << "#######################################################################################" << endl;
  cout << "KMinVal Using MurMurHash" << endl;
  for(int an = 2; an <= 8192; an *= 2){
    KMinVal kminval(an, 2);
    for(int i = 0; i < words.size(); i++){
      kminval.add(words[i]);
    }
    cout << "buckets: " << an << " KMinVal estimate: " << kminval.estimateCardinality() << endl;
  }
  cout << "#######################################################################################" << endl;

  cout << "#######################################################################################" << endl;
  cout << "BloomFilter using ax + b hash" << endl;
  for(int i = 64; i <= 131072; i *= 2){
    BloomFilter *bloom = new BloomFilter(5, i, 1); //Note that we are using 5 hashes with ax + b
    for(int i = 0; i < words.size() - 100; i++){ //We do not add last 100 words to the bloom filter to be able to check if they return as false positives
      bloom->add(words[i]);
      if(i == 100)
	evaluate_bloom(words, bloom, i);
      if(i == 500)
	evaluate_bloom(words, bloom, i);
      if(i == 1000)
	evaluate_bloom(words, bloom, i);
      if(i == 100000)
	evaluate_bloom(words, bloom, i);
      if(i == 1000000)
	evaluate_bloom(words, bloom, i);
    }
    delete bloom;
  }
  cout << "#######################################################################################" << endl;

  cout << "#######################################################################################" << endl;
  cout << "BloomFilter using MurMurHash" << endl;
  for(int i = 64; i <= 131072; i *= 2){
    BloomFilter *bloom = new BloomFilter(5, i, 2); //Note that we are using 5 hashes with ax + b
    for(int i = 0; i < words.size() - 100; i++){ //We do not add last 100 words to the bloom filter to be able to check if they return as false positives
      bloom->add(words[i]);
      if(i == 100)
	evaluate_bloom(words, bloom, i);
      if(i == 500)
	evaluate_bloom(words, bloom, i);
      if(i == 1000)
	evaluate_bloom(words, bloom, i);
      if(i == 100000)
	evaluate_bloom(words, bloom, i);
      if(i == 1000000)
	evaluate_bloom(words, bloom, i);
    }
    delete bloom;
  }
  cout << "#######################################################################################" << endl;

    
}
