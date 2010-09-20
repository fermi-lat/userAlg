// Gaudi system includes
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/Algorithm.h"

// if use the gui
#include "GuiSvc/IGuiSvc.h"
#include "gui/GuiMgr.h"
#include "gui/DisplayControl.h"

#include "Event/MonteCarlo/McParticle.h"
#include "Event/TopLevel/Event.h"

// for access to instrument.ini
#include "GlastSvc/GlastDetSvc/IGlastDetSvc.h"

#include <cassert>
#include <algorithm>
#include <map>
#include <string>
#include <stdlib.h>
#include <iostream>


/** @class PauseEvent
* @brief Algorithm to pause execution to all user to examine an event more closely. 
*
* $Header: /nfs/slac/g/glast/ground/cvs/userAlg/src/PauseEvent.cxx,v 1.7 2005/01/10 21:36:41 burnett Exp $
<br> Example jobOptions fragment:
<pre>
// use this when you want to specify the run numbers (Run_Event):
ApplicationMgr.TopAlg += {"EventList"};
PauseEvent.eventList = {"0_20", "0_22", "3_24", "5_50"};
// and this when run number is not needed: (not sure why this works, but it does!)
PauseEvent.eventList = {20, 22, 24, 50};
</pre>
*/
class PauseEvent : public Algorithm {
public:
    PauseEvent(const std::string& name, ISvcLocator* pSvcLocator);
    StatusCode initialize();
    StatusCode execute();
    StatusCode finalize();

private: 

    /// Read in the event list provided in the job options file
    void processEventList();
    //! number of times called
    int m_count; 
    //! access to the Gui Service for display of 3-d objects
    IGuiSvc*    m_guiSvc;

    /// List of Event Ids to Pause on
    StringArrayProperty m_eventList;
    /// keeps track of run numbers and event ids specified in job options file
    std::map< int, std::vector<int> > m_idMap;
    /// Flag which denotes if the user provided run numbers in the eventList
    bool m_runNumFlag;

};

// necessary to define a Factory for this algorithm
// expect that the xxx_load.cxx file contains a call     
//     DLL_DECL_ALGORITHM( PauseEvent );

//static const AlgFactory<PauseEvent>  Factory;
//const IAlgFactory& PauseEventFactory = Factory;
DECLARE_ALGORITHM_FACTORY(PauseEvent);

PauseEvent::PauseEvent(const std::string& name, ISvcLocator* pSvcLocator)
:Algorithm(name, pSvcLocator)
,m_count(0), m_runNumFlag(true)
{
    // List of strings, of the form:  {"runNum_eventId", "runNum_eventId"...}
    declareProperty("eventList", m_eventList);

}

StatusCode PauseEvent::initialize()
{
    StatusCode  sc = StatusCode::SUCCESS;
    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "initialize" << endreq;

    // Use the Job options service to set the Algorithm's parameters
    setProperties();

    processEventList();

    // get the Gui service
    if (service("GuiSvc", m_guiSvc).isFailure ()){
        log << MSG::WARNING << "GuiSvc not found" << endreq;
    }


    return sc;
}

StatusCode PauseEvent::execute()
{
    StatusCode  sc = StatusCode::SUCCESS;
    MsgStream   log( msgSvc(), name() );
    log << MSG::INFO << "Called " << ++m_count << " time(s)" << endreq;

    // Check the event Id for this event, if this id and run number combination
    // is in our eventList, we will pause the GUI.
    SmartDataPtr<Event::EventHeader> event(eventSvc(), "/Event");

    if (event) {
        unsigned int runNum = 0;
        if (m_runNumFlag) runNum = event->run();
        unsigned int eventId = event->event();
        std::vector<int> &eventList = m_idMap[runNum];

        // Search to see if this event id is among the list of ids we want to pause on
        std::vector<int>::iterator loc = std::find(eventList.begin(), eventList.end(), eventId);		
        // If we found it, we set the GUI to PAUSE
        if (loc != eventList.end()) {
            log << MSG::INFO << "Event found in list" << endreq;
            m_guiSvc->guiMgr()->pause();
        } 
    } else {
        log << MSG::DEBUG << "Error accessing event" << endreq;
        sc = StatusCode::FAILURE;
    }
    return sc;
}

StatusCode PauseEvent::finalize(){
    StatusCode  sc = StatusCode::SUCCESS;
    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "finalize after " << m_count << " calls." << endreq;

    return sc;
}

void PauseEvent::processEventList() {

    MsgStream log(msgSvc(), name());

    const std::vector<std::string>& theIds = m_eventList.value( );
    std::vector<std::string>::const_iterator it;
    std::vector<std::string>::const_iterator itend = theIds.end( );
    for (it = theIds.begin(); it != itend; it++) {
        int len = (*it).size();
        int delimPos = (*it).find_first_of('_');
        int runNum;
        if (delimPos >= 0) {
            runNum = atoi((*it).substr(0, delimPos).c_str());
        } else {
            runNum = 0;
            if (m_runNumFlag) {
                m_runNumFlag = false;
                log << MSG::DEBUG << "Run Flag set to " << m_runNumFlag << endreq;
            }
        }
        int eventId = atoi((*it).substr(delimPos+1, len-delimPos-1).c_str());
        log << MSG::DEBUG << "RunNum " << runNum << " EventId " << eventId<< endreq;
        std::vector<int>& curList = m_idMap[runNum];
        curList.push_back(eventId);		
    }
}



