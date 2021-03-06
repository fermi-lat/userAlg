//$Header: /nfs/slac/g/glast/ground/cvs/userAlg/src/mainpage.h,v 1.2 2002/07/02 17:26:50 burnett Exp $
// (Special "header" just for doxygen)

/*! @mainpage  package userAlg

This package is a "user hook" allowing development of algrorithms in the Glast Gaudi environment
Since it "uses" the global package Gleam, it depends on all packages needed to build the 
simulation/ reconstruction environment. This includes 

  <ul>
<li> access to all quantities in the Transient Data Store (TDS)</li>
<li> writing text output to the log file (which can be turned on/off with the gui "printer").
<li> generating histograms and tuples, which would then be written to a ROOT file. (Need to document this.)
<li> ability to add components to the display, via GuiSvc.
</ul>
  The primary component (which can be easily added to) is UserAlg.

\section utilites Interactive Utilities
<ul>
 <li> PauseEvent </li>
 <li> Replay </li>
 </ul>

 @section joboptioins Job Options
 @param UserAlg.tupleName [""] The name for a ROOT output tuple. 
@param Replay.Algorithms [{""}] List of algorithm names to optionally replay

  <hr>
  \section jobOptions src/jobOptions.txt
  \include jobOptions.txt
  \section requirements requirements
  \include requirements


*/
