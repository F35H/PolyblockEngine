#include <PRVTPB.h>

inline constexpr std::vector<UINT16> getCubeIndices() {
  return {
    0, 1, 2, 
    2, 3, 0,
    
    4, 5, 6, 
    6, 7, 4,
    
    8, 9, 10, 
    10, 11, 8,
    
    12, 13, 14, 
    14, 15, 12,
    
    16, 17, 18, 
    18, 19, 16,
    
    20, 21, 22, 
    22, 23, 20
  }; //return
}; //getBlockIndices

std::vector<UINT16> getBlockIndices(unsigned int type) {
  switch (type) {
    //Platonic Solids
  case CUBE:
    return getCubeIndices();
  }; //switch
}; //getBlockVertices