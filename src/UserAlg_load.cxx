/** 
* @file UserAlg_load.cpp
* @brief This is needed for forcing the linker to load all components
* of the library.
*
*  $Header: /nfs/slac/g/glast/ground/cvs/userAlg/src/UserAlg_load.cxx,v 1.4 2002/04/22 19:03:24 heather Exp $
*/

#include "GaudiKernel/DeclareFactoryEntries.h"

DECLARE_FACTORY_ENTRIES(UserAlg) {
    DECLARE_ALGORITHM( UserAlg );
    DECLARE_ALGORITHM( PauseEvent );
    DECLARE_ALGORITHM( Replay );
}
