/** @file UserAlg.cxx
    @brief declartion, implementaion of the class UserAlg

    $Header: /nfs/slac/g/glast/ground/cvs/userAlg/src/UserAlg.cxx,v 1.17 2004/10/21 18:59:06 heather Exp $
*/
// Gaudi system includes
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/Algorithm.h"


// if using the gui
#include "GuiSvc/IGuiSvc.h"
#include "gui/GuiMgr.h"
#include "gui/DisplayControl.h"

// TDS class declarations: input data, and McParticle tree
#include "Event/MonteCarlo/McParticle.h"
#include "Event/MonteCarlo/McIntegratingHit.h"
#include "Event/MonteCarlo/McPositionHit.h"

#include "Event/TopLevel/Event.h"
#include "Event/TopLevel/EventModel.h"
#include "Event/TopLevel/MCEvent.h"

#include "Event/Recon/TkrRecon/TkrVertex.h"
#include "Event/Recon/CalRecon/CalCluster.h"
#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "Event/Recon/AcdRecon/AcdRecon.h"

#include "ntupleWriterSvc/INTupleWriterSvc.h"

// for access to geometry perhaps
#include "GlastSvc/GlastDetSvc/IGlastDetSvc.h"

// for the propagator
#include "GlastSvc/Reco/IPropagator.h"

#include <cassert>
#include <sstream>

/** @class UserAlg
@brief A simple algorithm.
*/
class UserAlg : public Algorithm {
public:
    UserAlg(const std::string& name, ISvcLocator* pSvcLocator);
    /// set parameters and attach to various perhaps useful services.
    StatusCode initialize();
    /// process one event
    StatusCode execute();
    /// clean up
    StatusCode finalize();
    
private: 
    /// number of times called
    double m_count; 
    /// the GlastDetSvc used for access to detector info
    IGlastDetSvc*    m_detSvc; 
    /// access to the Gui Service for display of 3-d objects
    IGuiSvc*    m_guiSvc;

    /// access the ntupleWriter service to write out to ROOT ntuples
    INTupleWriterSvc *m_rootTupleSvc;

    /// parameter to store the logical name of the ROOT file to write to
    std::string m_treeName;

    /// geometry propagator
    IPropagator* m_prop;

    /// distance of conversion point and nearest ACD along the vertex
    /// aka completely useless variable
    double m_cuv;
};


static const AlgFactory<UserAlg>  Factory;
const IAlgFactory& UserAlgFactory = Factory;

UserAlg::UserAlg(const std::string& name, ISvcLocator* pSvcLocator)
:Algorithm(name, pSvcLocator)
,m_count(0), m_detSvc(0), m_guiSvc(0)
{
    // declare properties with setProperties calls
    declareProperty("treeName",  m_treeName="");
    
}

StatusCode UserAlg::initialize(){
    StatusCode  sc = StatusCode::SUCCESS;
    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "initialize" << endreq;
    
    // Use the Job options service to set the Algorithm's parameters
    setProperties();
    
    if( m_treeName.empty()) {
        log << MSG::INFO << "tupleName property not set!  No ntuple output"<<endreq;
    }
    
    // now try to find the GlastDevSvc service
    if (service("GlastDetSvc", m_detSvc).isFailure()){
        log << MSG::ERROR << "Couldn't find the GlastDetSvc!" << endreq;
        return StatusCode::FAILURE;
    }
    
    // get the Gui service (not required)
    if (service("GuiSvc", m_guiSvc).isFailure ()){
        log << MSG::WARNING << "No GuiSvc, so no display" << endreq;
    }else{
        //m_guiSvc->guiMgr()->display().add(new Rep, "User rep");
    }
    
    // example code to create a ROOT tuple and schedule it for filling 

    // get a pointer to RootTupleSvc
    sc = service("RootTupleSvc", m_rootTupleSvc);

    if( sc.isFailure() ) {
        log << MSG::ERROR << "userAlg failed to get the RootTupleSvc" << endreq;
        return sc;
    }
    m_rootTupleSvc->addItem(m_treeName, "count", &m_count);
    m_rootTupleSvc->addItem(m_treeName, "completely_useless", &m_cuv);

    IToolSvc* toolSvc = 0;
    if ( service("ToolSvc", toolSvc, true).isFailure() ) {
        log << MSG::ERROR << "Couldn't find the ToolSvc!" << endreq;
        return StatusCode::FAILURE;
    }
    if ( !toolSvc->retrieveTool("G4PropagationTool", m_prop) ) {
        log << MSG::ERROR << "Couldn't find the G4PropagationTool!" << endreq;
        return StatusCode::FAILURE;
    }

    return sc;
}

StatusCode UserAlg::execute()
{
    StatusCode  sc = StatusCode::SUCCESS;
    MsgStream   log( msgSvc(), name() );
    log << MSG::INFO << "executing " << ++m_count << " time" << endreq;

    if( m_rootTupleSvc!=0) {
        // necessary to store the tuple entry
        m_rootTupleSvc->storeRowFlag(m_treeName, true);
    }
    

    // Retrieving pointers from the TDS
    
    SmartDataPtr<Event::EventHeader>   header(eventSvc(),    EventModel::EventHeader);
    SmartDataPtr<Event::MCEvent>     mcheader(eventSvc(),    EventModel::MC::Event);
    SmartDataPtr<Event::McParticleCol> particles(eventSvc(), EventModel::MC::McParticleCol);
    SmartDataPtr<Event::TkrVertexCol>  tracks(eventSvc(),    EventModel::TkrRecon::TkrVertexCol);
    SmartDataPtr<Event::CalClusterCol> clusters(eventSvc(),  EventModel::CalRecon::CalClusterCol);
    SmartDataPtr<Event::CalXtalRecCol> xtalrecs(eventSvc(),  EventModel::CalRecon::CalXtalRecCol);
    SmartDataPtr<Event::AcdRecon> acdrec(eventSvc(),         EventModel::AcdRecon::Event);

    double t = header->time();
    log << MSG::DEBUG << "Event time: " << t << endreq;;

    
    // loop over the monte carlo particle collection
    if (particles) {
        
        Event::McParticleCol::const_iterator piter;
        
        for (piter = particles->begin(); piter != particles->end(); piter++) {
            const Event::McParticle& mcp = **piter;
            
        }
    }

     
    // *************************************************************************
    // exercise 1:
    // Apply cuts using the TDS data to determine we want this event to be
    // written to output or not
    // For example we could use the number of vertices (yes the vertices 
    // collection is called tracks in this example) to determine if we 
    // want this event written to output.  
    // If not, we setFilterPassed false and return - 
    // and in addition we must set up the
    // jobOptions such that this algorithm is used to filter - which means
    // it determines what algorithms are run after it    
    // One should redefine the Triggered sequence such that UserAlg appears
    // before either meritAlg or the Output sequence.
    //  Triggered.Members={
    //    "Sequencer/Trigger",
    //    "Sequencer/Reconstruction",
    //    "Sequencer/RecoDisplay",
    //    "UserAlg",
    //    "Sequencer/Output",
    //    "FilterTracks","FilterAlg",
    //    "meritAlg" };

    //if ((!tracks) || (tracks.size() <= 0)) {
    //    setFilterPassed( false );
    //    return sc;
    //}


    // *************************************************************************
    // exercise 2:
    // 1) get the first vertex of the TkrVertexCol.
    //    We need a pointer to Event::TkrVertexCol in the TDS.  No new code, was
    //    here anyway.
    // 2) retrieve position and direction.
    // 3) use a propagator to propagate back to the ACD.
    //    We need the IPropagator* m_prop. The pointer to it has to be retrieved
    //    in initialize().
    // 4) determine the distance of the vertex (conversion point) and the ACD.
    // 5) store this completely useless variable in MeritTuple.
    //    Not exactly true, actually.  It is stored in the tree specified by
    //    UserAlg.treeName in jobOptions.txt.  We need m_ntupleSvc and the
    //    pointer to it.  No new code, was here anyway.

    m_cuv = 0.;

    if ( tracks->size() > 0 ) {// no clue why the vertex col is called "tracks"
        const Event::TkrVertex* vertex1 = tracks->front();
        const Point p = vertex1->getPosition();
        const Vector v = vertex1->getDirection();
        log << MSG::DEBUG;
        if ( log.isActive() ) {
            std::ostringstream ost;
            ost << "initial " << p;
            log << ost.str();
        }
        log << endreq;
        log << MSG::DEBUG;
        if ( log.isActive() ) {
            std::ostringstream ost;
            ost << "initial Direction" << v;
            log << ost.str();
        }
        log << endreq;

        // start the propagation at p and go into the opposite direction of v
        m_prop->setStepStart(p, -v);
        // first, I misunderstood what step means.  It is the distance to be
        // stepped by the propagator, not the length of a single step.
        m_prop->step(3000.); // 3000 mm should do it

        for ( int i=0; i<m_prop->getNumberSteps(); ++i ) {
            idents::VolumeIdentifier volId = m_prop->getStepVolumeId(i);
            Point pp = m_prop->getStepPosition(i);            
            // Use new method to check that this is an ACD detector, 
            // rather than support structure
            if (!idents::AcdId::checkVolId(volId)) continue;
            idents::AcdId acd(volId);
            if ( !acd.tile() )  // not yet ACD
                continue;
            // fine, it's a tile
            log << MSG::DEBUG;
            if ( log.isActive() ) {
                std::ostringstream ost;
                ost << "ACD " << pp;
                log << ost.str() << ' ' << volId.name() << ' ' << acd.tile();
            }
            log << endreq;
            // fill the completely useless variable
            m_cuv = (pp-p).magnitude();
            // let's exit, we found an ACD
            break;
        }
    }
    log << MSG::DEBUG << "completely useless: " << m_cuv << endreq;
    // *************************************************************************
    
#if 0 // enable to use pause with the display
    // An example of pausing the display 
    bool pause = false;
    // calculate some condition here...
    // pause = .....;
    if (pause) {
        log << MSG::INFO << "Pausing at event " << m_count <<  endreq;
        m_guiSvc->guiMgr()->pause();
    } 
#endif
#if 0
    log << MSG::FATAL << "test of a fatal error " << endreq;
    sc = StatusCode::FAILURE;
#endif
    return sc;
}

StatusCode UserAlg::finalize(){
    StatusCode  sc = StatusCode::SUCCESS;
    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "finalize after " << m_count << " calls." << endreq;
    
    return sc;
}

