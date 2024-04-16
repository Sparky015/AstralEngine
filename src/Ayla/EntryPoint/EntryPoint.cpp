
#include "Ayla/EntryPoint/EngineLoop.h"

// SDL REQUIRES that it has the 'int argc, char* args[]' main signature, or it will cry.
int main( int argc, char* args[] ) {
    Ayla::EntryPoint::EngineLoop();
    // TODO: Make a function that abstracts this implementation of main() so it can be exposed to the Ayla API
    //       Then the client will use this function to enter into the engine.
}



