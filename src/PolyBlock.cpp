#include <PolyBlock.h>

namespace pb {
  //MainLoop
  pbInit::pbInit() {
    try {
      do {
        gfxLoop();
        cntrlLoop();

      } while (!prgrmCloseCheck());

    }
    catch (const std::exception& e) {
      std::string rtrnStr = e.what();

      WritetoTimedLog(e.what());
    }
    catch (...) {
      WritetoTimedLog(
        "Unknown Error | Unknown Action | Terminating");

    } //Catch

  } //pbInit

} //pb