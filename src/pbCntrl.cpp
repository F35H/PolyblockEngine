#include <pbCntrl.h>

namespace pbCntrl {
  void pbCntrl::cntrlLoop() {
    glfwPollEvents();
  }; // cntrlLoop

  bool pbCntrl::winCloseCheck(int i) {
    return glfwWindowShouldClose(winVec[i].win);
  }//winCloseCheck

  bool pbCntrl::winCloseCheck(int i) {
    for (const auto& win : winVec) {
      if (!glfwWindowShouldClose(win.win)) {
        return false;
      }
    };

    return true;
  }//prgrmCloseCheck
} //pbCntrl Namespace