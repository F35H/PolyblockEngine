#include <PRVTPB.h>

/* CONSTRUCTORS */
inline pb::Control::Motion::Motion() noexcept {};
inline pb::Control::Motion::Motion(const char* motionName) {
  MotionName = motionName; 
}; //Motion
inline pb::Control::Motion::Motion(const char* filename, const char* motionName) {
  auto motion = pb::Utils::Input::MotionFromFile(filename, name);

  CopyFromExtern(motion);
  delete motion;
}; //Camera filename

/* SETTERS */


/* GETTERS */