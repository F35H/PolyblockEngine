#include <pbGFX.h>

namespace pbGFX {
  pbGFX::pbGFX() {
    pbCntx();

    //Verify Renderer
    switch (rend) {
    default:
    case VULKAN13:
      WritetoTimedLog(
        "Selecting Renderer | Program Initialization | Attempting Vulkan 1.3");

      pbVulkan();
      return;

    case OPENGL46:
    case OPENGL45:
    case OPENGL44:
    case OPENGL43:
    case OPENGL42:
    case OPENGL41:
    case OPENGL40:
    case OPENGL30:
    case OPENGL20:
      return;

    } //Switch
  } //Cnstrct

  void pbGFX::gfxLoop() {
    pbVulkan();

  } //gfxLoop

}; //GFX Namespace