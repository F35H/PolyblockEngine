#pragma once

#include <pbMacros.h>

namespace pbCntx {
  
  struct pbCntx {
    union Renderer {
      pbGLFW* glfw;
    };

    Renderer* r;

    static FINT8 rend;


    pbCntx();
  }; //pbCntx
} //pbCntx