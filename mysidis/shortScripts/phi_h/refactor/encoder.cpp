#ifndef encoder_cpp 
#define encoder_cpp 

#include <cmath>
#include <iostream>
#include <map>
#include <vector>

class BitCalculator {
public:

  static int GetNumberBitsQuick(int x){
    return std::ceil(std::log2(x));
  }

  static int GetNumberBits(int x){
    int n      = 0;
    int total  = 1;
    bool found = false;

    while( !found ){
      n++;
      total += std::pow(2,n);

      if(total > x){
	found = true;
      }
    }

    return n+1;
  }

  static long int WriteInt(int number, int stop){
    long int x = number;
    x = x << stop;
    return x;
  }

  static long int ReadInt(long int number, int start, int stop){
    long int mask = 0;

    int shift = start-stop;
    if(shift < 1){
      std::cout << "You didn't setup start and stop properly." << std::endl;
      return -1;
    }

    for(int i=0; i<shift; i++){
      mask += std::pow(2,i);
    }

    //  this is the one which took a long time
    return (number & mask) >> stop;
  }
};

class Indexer {
public:

  Indexer(std::vector<int> dims) : fDimensions(dims) {
    fDimension = dims.size();

    for(int d : fDimensions){
      fNumBits.push_back(BitCalculator::GetNumberBits(d));
    }

    fTotalBits = 0;
    for(int b : fNumBits){
      fTotalBits += b;
    }
  }


  long int getIndex(std::vector<int> indices){

    // safety check
    if (indices.size() != fDimension){
      std::cerr << "[Indexer::GetIndex] Vector not the same size as space " << std::endl;
      return -1;
    }

    long int index = 0;
    for(int i=0; i<fDimensions.size(); i++){
      index += indices[i];
      index = index << fNumBits[i];
    }

    return index;
  }

  std::vector<int> getIndices(long int index){
    std::vector<int> indices;

    for(int i=0; i<fDimensions.size(); i++){

    }

    return indices;
  }

protected:
  int              fDimension;
  int              fTotalBits;
  std::vector<int> fNumBits;
  std::vector<int> fDimensions;
};

// Class that can hold big tables that 
// are sparse and occupy less memory. 
template <class T>
class SparseContainer {

public:
  SparseContainer(std::vector<int> dims) : indexer(dims) {
  }

  void insert(T & content, std::vector<int> index){
    container[indexer.getIndex(index)] = content; 
  }

  T query(std::vector<int> index){
    long int key = indexer.getIndex(index); 

    if (container.count(key) == 1){
      return container[key]; 
    } else { 
      return T(); 
    }
  }

protected:
  Indexer indexer; 
  std::map<long int, T> container; 
};

#endif 
