/** @file UserAlg.cxx
    @brief declartion, implementaion of the class UserAlg

    $Header: /nfs/slac/g/glast/ground/cvs/userAlg/src/UserAlg.cxx,v 1.11 2003/03/15 22:12:28 burnett Exp $
*/
// Gaudi system includes
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/Algorithm.h"

// ntuple interface
#define NTUPLE
#ifdef NTUPLE
#include "ntupleWriterSvc/INTupleWriterSvc.h"
#endif
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

// for access to geometry perhaps
#include "GlastSvc/GlastDetSvc/IGlastDetSvc.h"

#include <cassert>

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
    int m_count; 
    /// the GlastDetSvc used for access to detector info
    IGlastDetSvc*    m_detSvc; 
    /// access to the Gui Service for display of 3-d objects
    IGuiSvc*    m_guiSvc;
#ifdef NTUPLE
    /// access the ntupleWriter service to write out to ROOT ntuples
    INTupleWriterSvc *m_ntupleWriteSvc;
#endif
    /// parameter to store the logical name of the ROOT file to write to
    std::string m_tupleName;
};


static const AlgFactory<UserAlg>  Factory;
const IAlgFactory& UserAlgFactory = Factory;

UserAlg::UserAlg(const std::string& name, ISvcLocator* pSvcLocator)
:Algorithm(name, pSvcLocator)
,m_count(0), m_detSvc(0), m_guiSvc(0)
{
    // declare properties with setProperties calls
    declareProperty("tupleName",  m_tupleName="");
    
}

StatusCode UserAlg::initialize(){
    StatusCode  sc = StatusCode::SUCCESS;
    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "initialize" << endreq;
    
    // Use the Job options service to set the Algorithm's parameters
    setProperties();
    
    if( m_tupleName.empty()) {
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
    
#ifdef NTUPLE
    // get a pointer to our ntupleWriterSvc
    if (!m_tupleName.empty()) {
        if (service("ntupleWriterSvc", m_ntupleWriteSvc, true).isFailure()) {
            log << MSG::ERROR << "UserAlg failed to get the ntupleWriterSvc" << endreq;
            return StatusCode::FAILURE;
        }
    }
#endif

#ifdef NTUPLE
    // Here we are adding to our ROOT ntuple all of the columns for initialization
    if (!m_tupleName.empty())
        sc = m_ntupleWriteSvc->addItem(m_tupleName.c_str(), "NumCalls", 0.0);
#endif


    return sc;
}

StatusCode UserAlg::execute()
{
    StatusCode  sc = StatusCode::SUCCESS;
    MsgStream   log( msgSvc(), name() );
    log << MSG::INFO << "executing " << ++m_count << " time" << endreq;

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

    
    // lop over the monte carlo particle collection
    if (particles) {
        
        Event::McParticleCol::const_iterator piter;
        
        for (piter = particles->begin(); piter != particles->end(); piter++) {
            const Event::McParticle& mcp = **piter;
            
        }
    }
    
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
#ifdef NTUPLE
    // Here we are adding to our ROOT ntuple
    if (!m_tupleName.empty())
        sc = m_ntupleWriteSvc->addItem(m_tupleName.c_str(), "NumCalls", m_count);
    
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

