// $Header: /nfs/slac/g/glast/ground/cvs/aoRecon/src/Dll/aoRecon_load.cxx,v 1.1.1.1 2001/03/01 15:42:46 burnett Exp $
//====================================================================
//
//  Description: Implementation of <Package>_load routine.
//               This routine is needed for forcing the linker
//               to load all the components of the library. 
//
//====================================================================

#include "Gaudi/Interfaces/ICnvFactory.h"
#include "Gaudi/Interfaces/ISvcFactory.h"
#include "Gaudi/Interfaces/IAlgFactory.h"


#define DLL_DECL_SERVICE(x)    extern const ISvcFactory& x##Factory; x##Factory.addRef();
#define DLL_DECL_CONVERTER(x)  extern const ICnvFactory& x##Factory; x##Factory.addRef();
#define DLL_DECL_ALGORITHM(x)  extern const IAlgFactory& x##Factory; x##Factory.addRef();
#define DLL_DECL_OBJECT(x)     extern const IFactory& x##Factory; x##Factory.addRef();

//! Load all  services: 
void pdrApp_load() {
    DLL_DECL_ALGORITHM( UserAlg );
} 

extern "C" void pdrApp_loadRef()    {
    pdrApp_load();
}

