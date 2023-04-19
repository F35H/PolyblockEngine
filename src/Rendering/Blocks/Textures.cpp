#include <PRVTPB.h>

inline constexpr std::vector<float> getCubeTextures() {
  return {
    //Bottom
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,

    //Back Plane
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,

    //Front Plane
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,

    //Right Plane
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,

    //Left Plane
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,

    //Top
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
  }; //getCubeTextures
}; //getCubeTextures

std::vector<float> getBlockTextures(unsigned int type) {
  switch (type) {
    //Platonic Solids
  case CUBE:
    return getCubeTextures();
  }; //switch
}; //getBlockVertices