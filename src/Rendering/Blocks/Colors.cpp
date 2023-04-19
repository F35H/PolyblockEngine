#include <PRVTPB.h>

inline constexpr std::vector<float> getCubeColors() {
  return {
    //Bottom
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,

    //Back Plane
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,

    //Front Plane
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,

    //Right Plane
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,

    //Left Plane
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,

    //Top
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f,
    1.0f, 1.0f, 1.0f, 0.5f
  }; //return
}; //getCubeColors

std::vector<float> getBlockColors(unsigned int type) {
  switch (type) {
    //Platonic Solids
  case CUBE:
    return getCubeColors();
  }; //switch
}; //getBlockVertices