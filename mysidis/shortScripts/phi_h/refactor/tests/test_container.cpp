#include <vector>

#include "../encoder.cpp"

class TestType {

public:
  TestType(){
    a = 1; 
    b = 2; 
    c = 3; 
  }

  int getA() const {
    return a; 
  }

  int getB() const {
    return b; 
  }

  int getC() const {
    return c; 
  }

  void set(int x, int y, int z){
    a = x; 
    b = y; 
    c = z; 
  }

protected:
  int a, b, c; 
};

void test_container(){

  std::vector<int> dimensions = {100, 100, 100};
  SparseContainer<int> counts(dimensions);   

  std::vector<int> point1 = {3, 33, 35}; 
  std::vector<int> point2 = {23, 98, 15}; 

  int content = 42; 
  counts.insert(content, point1); 
  
  std::cout << counts.query(point1) << std::endl; 
  std::cout << counts.query(point2) << std::endl; 

  // Test out with non-int 
  SparseContainer<TestType> testContainer(dimensions); 
  
  TestType testContent; 
  testContent.set(3, 2, 1); 
  testContainer.insert(testContent, point1);
  
  TestType retrievedType = testContainer.query(point1); 
  std::cout << retrievedType.getA() << std::endl; 
  std::cout << retrievedType.getB() << std::endl; 
  std::cout << retrievedType.getC() << std::endl; 

  TestType retrievedType2 = testContainer.query(point2); 
  std::cout << retrievedType2.getA() << std::endl; 
  std::cout << retrievedType2.getB() << std::endl; 
  std::cout << retrievedType2.getC() << std::endl; 


}
