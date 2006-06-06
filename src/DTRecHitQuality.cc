/*
 *  See header file for a description of this class.
 *
 *  $Date: 2006/03/22 16:15:36 $
 *  $Revision: 1.5 $
 *  \author G. Cerminara - INFN Torino
 */

#include "DTRecHitQuality.h"


#include "Geometry/DTGeometry/interface/DTLayer.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "Histograms.h"

#include "TFile.h"

#include <iostream>
#include <map>



using namespace std;
using namespace edm;

HRes1DHit hRes_S1RPhi("S1RPhi");     // RecHits, 1. step, RPhi
HRes1DHit hRes_S2RPhi("S2RPhi");     // RecHits, 2. step, RPhi
HRes1DHit hRes_S3RPhi("S3RPhi");     // RecHits, 3. step, RPhi

HRes1DHit hRes_S1RZ("S1RZ");         // RecHits, 1. step, RZ
HRes1DHit hRes_S2RZ("S2RZ");	    // RecHits, 2. step, RZ
HRes1DHit hRes_S3RZ("S3RZ");	    // RecHits, 3. step, RZ

HRes1DHit hRes_S1RZ_W0("S1RZ_W0");   // RecHits, 1. step, RZ, wheel 0
HRes1DHit hRes_S2RZ_W0("S2RZ_W0");   // RecHits, 2. step, RZ, wheel 0
HRes1DHit hRes_S3RZ_W0("S3RZ_W0");   // RecHits, 3. step, RZ, wheel 0

HRes1DHit hRes_S1RZ_W1("S1RZ_W1");   // RecHits, 1. step, RZ, wheel +-1
HRes1DHit hRes_S2RZ_W1("S2RZ_W1");   // RecHits, 2. step, RZ, wheel +-1
HRes1DHit hRes_S3RZ_W1("S3RZ_W1");   // RecHits, 3. step, RZ, wheel +-1

HRes1DHit hRes_S1RZ_W2("S1RZ_W2");   // RecHits, 1. step, RZ, wheel +-2
HRes1DHit hRes_S2RZ_W2("S2RZ_W2");   // RecHits, 2. step, RZ, wheel +-2
HRes1DHit hRes_S3RZ_W2("S3RZ_W2");   // RecHits, 3. step, RZ, wheel +-2


HEff1DHit hEff_S1RPhi("S1RPhi");     // RecHits, 1. step, RPhi
HEff1DHit hEff_S2RPhi("S2RPhi");     // RecHits, 2. step, RPhi
HEff1DHit hEff_S3RPhi("S3RPhi");     // RecHits, 3. step, RPhi

HEff1DHit hEff_S1RZ("S1RZ");         // RecHits, 1. step, RZ
HEff1DHit hEff_S2RZ("S2RZ");	    // RecHits, 2. step, RZ
HEff1DHit hEff_S3RZ("S3RZ");	    // RecHits, 3. step, RZ

HEff1DHit hEff_S1RZ_W0("S1RZ_W0");   // RecHits, 1. step, RZ, wheel 0
HEff1DHit hEff_S2RZ_W0("S2RZ_W0");   // RecHits, 2. step, RZ, wheel 0
HEff1DHit hEff_S3RZ_W0("S3RZ_W0");   // RecHits, 3. step, RZ, wheel 0

HEff1DHit hEff_S1RZ_W1("S1RZ_W1");   // RecHits, 1. step, RZ, wheel +-1
HEff1DHit hEff_S2RZ_W1("S2RZ_W1");   // RecHits, 2. step, RZ, wheel +-1
HEff1DHit hEff_S3RZ_W1("S3RZ_W1");   // RecHits, 3. step, RZ, wheel +-1

HEff1DHit hEff_S1RZ_W2("S1RZ_W2");   // RecHits, 1. step, RZ, wheel +-2
HEff1DHit hEff_S2RZ_W2("S2RZ_W2");   // RecHits, 2. step, RZ, wheel +-2
HEff1DHit hEff_S3RZ_W2("S3RZ_W2");   // RecHits, 3. step, RZ, wheel +-2




// Constructor
DTRecHitQuality::DTRecHitQuality(const ParameterSet& pset){
  // Get the debug parameter for verbose output
  debug = pset.getUntrackedParameter<bool>("debug");
  rootFileName = pset.getUntrackedParameter<string>("rootFileName");
  // the name of the simhit collection
  simHitLabel = pset.getUntrackedParameter<string>("simHitLabel", "SimG4Object");
  // the name of the 1D rec hit collection
  recHitLabel = pset.getUntrackedParameter<string>("recHitLabel", "DTRecHit1DProducer");
  // the name of the 2D rec hit collection
  segment2DLabel = pset.getUntrackedParameter<string>("segment2DLabel");
  // the name of the 4D rec hit collection
  segment4DLabel = pset.getUntrackedParameter<string>("segment4DLabel");
    
  // Switches for analysis at various steps
  doStep1 = pset.getUntrackedParameter<bool>("doStep1", false);
  doStep2 = pset.getUntrackedParameter<bool>("doStep2", false);
  doStep3 = pset.getUntrackedParameter<bool>("doStep3", false);


  if(debug)
    cout << "[DTRecHitQuality] Constructor called" << endl;

  // Create the root file
  theFile = new TFile(rootFileName.c_str(), "RECREATE");
  theFile->cd();

}



// Destructor
DTRecHitQuality::~DTRecHitQuality(){
  if(debug) 
    cout << "[DTRecHitQuality] Destructor called" << endl;
}



void DTRecHitQuality::endJob() {
  // Write the histos to file
  theFile->cd();

  hEff_S1RPhi.ComputeEfficiency();
  hEff_S2RPhi.ComputeEfficiency();
  hEff_S3RPhi.ComputeEfficiency();

  hEff_S1RZ.ComputeEfficiency();
  hEff_S2RZ.ComputeEfficiency();
  hEff_S3RZ.ComputeEfficiency();

  hEff_S1RZ_W0.ComputeEfficiency();
  hEff_S2RZ_W0.ComputeEfficiency();
  hEff_S3RZ_W0.ComputeEfficiency();

  hEff_S1RZ_W1.ComputeEfficiency();
  hEff_S2RZ_W1.ComputeEfficiency();
  hEff_S3RZ_W1.ComputeEfficiency();

  hEff_S1RZ_W2.ComputeEfficiency();
  hEff_S2RZ_W2.ComputeEfficiency();
  hEff_S3RZ_W2.ComputeEfficiency();

  // Write histos to file
  hRes_S1RPhi.Write();
  hRes_S2RPhi.Write();
  hRes_S3RPhi.Write();

  hRes_S1RZ.Write();
  hRes_S2RZ.Write();
  hRes_S3RZ.Write();

  hRes_S1RZ_W0.Write();
  hRes_S2RZ_W0.Write();
  hRes_S3RZ_W0.Write();

  hRes_S1RZ_W1.Write();
  hRes_S2RZ_W1.Write();
  hRes_S3RZ_W1.Write();

  hRes_S1RZ_W2.Write();
  hRes_S2RZ_W2.Write();
  hRes_S3RZ_W2.Write();


  hEff_S1RPhi.Write();
  hEff_S2RPhi.Write();
  hEff_S3RPhi.Write();

  hEff_S1RZ.Write();
  hEff_S2RZ.Write();
  hEff_S3RZ.Write();

  hEff_S1RZ_W0.Write();
  hEff_S2RZ_W0.Write();
  hEff_S3RZ_W0.Write();

  hEff_S1RZ_W1.Write();
  hEff_S2RZ_W1.Write();
  hEff_S3RZ_W1.Write();

  hEff_S1RZ_W2.Write();
  hEff_S2RZ_W2.Write();
  hEff_S3RZ_W2.Write();

  theFile->Close();
}

// The real analysis
void DTRecHitQuality::analyze(const Event & event, const EventSetup& eventSetup){
  cout << "--- [DTRecHitQuality] Analysing Event: #Run: " << event.id().run()
       << " #Event: " << event.id().event() << endl;
  theFile->cd();
  // Get the DT Geometry
  ESHandle<DTGeometry> dtGeom;
  eventSetup.get<MuonGeometryRecord>().get(dtGeom);


  // Get the SimHit collection from the event
  Handle<PSimHitContainer> simHits;
  event.getByLabel(simHitLabel, "MuonDTHits", simHits);

  // Map simhits per wire
  map<DTWireId, vector<PSimHit> > simHitsPerWire =
    mapSimHitsPerWire(simHits.product());

 


  //=======================================================================================
  // RecHit analysis at Step 1
  if(doStep1) {
    if(debug)
      cout << "  -- DTRecHit S1: begin analysis:" << endl;
    // Get the rechit collection from the event
    Handle<DTRecHitCollection> dtRecHits;
    event.getByLabel(recHitLabel, dtRecHits);
    
    // Map rechits per wire
    map<DTWireId,vector<DTRecHit1DPair> > recHitsPerWire = 
      map1DRecHitsPerWire(dtRecHits.product());

    compute(dtGeom.product(), simHitsPerWire, recHitsPerWire, 1);
  }

  

  //=======================================================================================
  // RecHit analysis at Step 2
  if(doStep2) {
    if(debug)
      cout << "  -- DTRecHit S2: begin analysis:" << endl;
 
   // Get the 2D rechits from the event
    Handle<DTRecSegment2DCollection> segment2Ds;
    event.getByLabel(segment2DLabel, segment2Ds);

    // Map rechits per wire
    map<DTWireId,vector<DTRecHit1D> > recHitsPerWire = 
      map1DRecHitsPerWire(segment2Ds.product());

    compute(dtGeom.product(), simHitsPerWire, recHitsPerWire, 2);
  }


  //=======================================================================================
  // RecHit analysis at Step 3
  if(doStep3) {
    if(debug)
      cout << "  -- DTRecHit S3: begin analysis:" << endl;

    // Get the 4D rechits from the event
    Handle<DTRecSegment4DCollection> segment4Ds;
    event.getByLabel(segment4DLabel, segment4Ds);

    // Map rechits per wire
    map<DTWireId,vector<DTRecHit1D> > recHitsPerWire = 
      map1DRecHitsPerWire(segment4Ds.product());

    compute(dtGeom.product(), simHitsPerWire, recHitsPerWire, 3);
  }

}




// Return a map between simhits of a layer and the wireId of their cell
map<DTWireId, vector<PSimHit> >
DTRecHitQuality::mapSimHitsPerWire(const PSimHitContainer* simhits) {
  map<DTWireId, vector<PSimHit> > hitWireMapResult;
   
  for(PSimHitContainer::const_iterator simhit = simhits->begin();
      simhit != simhits->end();
      simhit++) {
    hitWireMapResult[DTWireId((*simhit).detUnitId())].push_back(*simhit);
  }
   
  return hitWireMapResult;
}


// Return a map between DTRecHit1DPair and wireId
map<DTWireId, vector<DTRecHit1DPair> >
DTRecHitQuality::map1DRecHitsPerWire(const DTRecHitCollection* dt1DRecHitPairs) {
  map<DTWireId, vector<DTRecHit1DPair> > ret;
  
  for(DTRecHitCollection::const_iterator rechit = dt1DRecHitPairs->begin();
      rechit != dt1DRecHitPairs->end(); rechit++) {
    ret[(*rechit).wireId()].push_back(*rechit);
  }

  return ret;
}


// Return a map between DTRecHit1D at S2 and wireId
map<DTWireId, vector<DTRecHit1D> >
DTRecHitQuality::map1DRecHitsPerWire(const DTRecSegment2DCollection* segment2Ds) {
  map<DTWireId, vector<DTRecHit1D> > ret;
  
  // Loop over all 2D segments
  for(DTRecSegment2DCollection::const_iterator segment = segment2Ds->begin();
      segment != segment2Ds->end();
      segment++) {
    vector<DTRecHit1D> component1DHits= (*segment).specificRecHits();
    // Loop over all component 1D hits
    for(vector<DTRecHit1D>::const_iterator hit = component1DHits.begin();
	hit != component1DHits.end();
	hit++) {
      ret[(*hit).wireId()].push_back(*hit);
    }
  }
  return ret;
}



// Return a map between DTRecHit1D at S3 and wireId
map<DTWireId, std::vector<DTRecHit1D> >
DTRecHitQuality::map1DRecHitsPerWire(const DTRecSegment4DCollection* segment4Ds) {
   map<DTWireId, vector<DTRecHit1D> > ret;
   // Loop over all 4D segments
   for(DTRecSegment4DCollection::const_iterator segment = segment4Ds->begin();
       segment != segment4Ds->end();
       segment++) {
     // Get component 2D segments
     vector<const TrackingRecHit*> segment2Ds = (*segment).recHits();
     // Loop over 2D segments:
     for(vector<const TrackingRecHit*>::const_iterator segment2D = segment2Ds.begin();
      segment2D != segment2Ds.end();
      segment2D++) {
       // Get 1D component rechits
       vector<const TrackingRecHit*> hits = (*segment2D)->recHits();
       // Loop over them
       for(vector<const TrackingRecHit*>::const_iterator hit = hits.begin();
	   hit != hits.end(); hit++) {
	 const DTRecHit1D* hit1D = dynamic_cast<const DTRecHit1D*>(*hit);
	 ret[hit1D->wireId()].push_back(*hit1D);
       }
     }
   }

   return ret;
}



// Find the mu simhit among a collection of simhits
const PSimHit*
DTRecHitQuality::findMuSimHit(const vector<PSimHit>& hits) {
  vector<const PSimHit*> muHits; //FIXME: Do we really need a vector?
  // Loop over simhits
  for (vector<PSimHit>::const_iterator hit = hits.begin();
       hit != hits.end(); hit++) {
    if (abs((*hit).particleType()) == 13) muHits.push_back(&(*hit));
  }

  if (muHits.size()==0)
    return 0;
  else if (muHits.size()>1)
    cout << "[DTRecHitQuality]***WARNING: #muHits = " << muHits.size() << endl;
  return muHits.front();
}



// Compute SimHit distance from wire (cm)
float DTRecHitQuality::simHitDistFromWire(const DTLayer* layer,
					  DTWireId wireId,
					  const PSimHit& hit) {
  float xwire = layer->specificTopology().wirePosition(wireId.wire());
  LocalPoint entryP = hit.entryPoint();
  LocalPoint exitP = hit.exitPoint();
  float xEntry = entryP.x()-xwire;
  float xExit  = exitP.x()-xwire;

  return fabs(xEntry - (entryP.z()*(xExit-xEntry))/(exitP.z()-entryP.z()));//FIXME: check...
}


// Find the RecHit closest to the muon SimHit
template  <typename type>
const type* 
DTRecHitQuality::findBestRecHit(const DTLayer* layer,
				DTWireId wireId,
				const vector<type>& recHits,
				const float simHitDist) {
  float res = 99999;
  const type* theBestRecHit = 0;
  // Loop over RecHits within the cell
  for(typename vector<type>::const_iterator recHit = recHits.begin();
      recHit != recHits.end();
      recHit++) {
    float distTmp = recHitDistFromWire(*recHit, layer);
    if(fabs(distTmp-simHitDist) < res) {
      res = fabs(distTmp-simHitDist);
      theBestRecHit = &(*recHit);
    }
  } // End of loop over RecHits within the cell

  return theBestRecHit;
}


// Compute the distance from wire (cm) of a hits in a DTRecHit1DPair
float 
DTRecHitQuality::recHitDistFromWire(const DTRecHit1DPair& hitPair, const DTLayer* layer) {
  // Compute the rechit distance from wire
  return fabs(hitPair.localPosition(DTEnums::Left).x() -
	      hitPair.localPosition(DTEnums::Right).x())/2.;
}



// Compute the distance from wire (cm) of a hits in a DTRecHit1D
float 
DTRecHitQuality::recHitDistFromWire(const DTRecHit1D& recHit, const DTLayer* layer) {
  return fabs(recHit.localPosition().x() - layer->specificTopology().wirePosition(recHit.wireId().wire()));
}


template  <typename type>
void DTRecHitQuality::compute(const DTGeometry *dtGeom,
			      std::map<DTWireId, std::vector<PSimHit> > simHitsPerWire,
			      std::map<DTWireId, std::vector<type> > recHitsPerWire,
			      int step) {
  // Loop over cells with a muon SimHit
  for(map<DTWireId, vector<PSimHit> >::const_iterator wireAndSHits = simHitsPerWire.begin();
      wireAndSHits != simHitsPerWire.end();
      wireAndSHits++) {
    DTWireId wireId = (*wireAndSHits).first;
    vector<PSimHit> simHitsInCell = (*wireAndSHits).second;

    // Get the layer
    const DTLayer* layer = dtGeom->layer(wireId);

    // Look for a mu hit in the cell
    const PSimHit* muSimHit = findMuSimHit(simHitsInCell);
    if (muSimHit==0) {
      if (debug) 
	cout << "   No mu SimHit in channel: " << wireId << ", skipping! " << endl;
      continue; // Skip this cell
    }

    // Find the distance of the simhit from the wire
    float simHitWireDist = simHitDistFromWire(layer, wireId, *muSimHit);
    GlobalPoint simHitGlobalPos = layer->toGlobal(muSimHit->localPosition());
      
    bool recHitReconstructed = false;

    // Look for RecHits in the same cell
    if(recHitsPerWire.find(wireId) == recHitsPerWire.end()) {
      // No RecHit found in this cell
      cout << "   No RecHit found at Step: " << step << " in cell: " << wireId << endl;
    } else {
      recHitReconstructed = true;
      // vector<type> recHits = (*wireAndRecHits).second;
      vector<type> recHits = recHitsPerWire[wireId];
      cout << "   " << recHits.size() << " RecHits, Step " << step << " in channel: " << wireId << endl;
	 
      // Find the best RecHit
      const type* theBestRecHit = findBestRecHit(layer, wireId, recHits, simHitWireDist);

	 
      float recHitWireDist =  recHitDistFromWire(*theBestRecHit, layer);
      cout << "    SimHit distance from wire: " << simHitWireDist << endl;
      cout << "    RecHit distance from wire: " << recHitWireDist << endl;
      float recHitErr = recHitPositionError(*theBestRecHit);

      HRes1DHit *hRes = 0;
      HRes1DHit *hResTot = 0;

      // Fill residuals and pulls
      // Select the histo to be filled
      if(step == 1) {
	// Step 1
	if(wireId.superLayer() != 2) {
	  hRes = &hRes_S1RPhi;
	} else {
	  hResTot = &hRes_S1RZ;
	  if(wireId.wheel() == 0)
	    hRes = &hRes_S1RZ_W0;
	  if(abs(wireId.wheel()) == 1)
	    hRes = &hRes_S1RZ_W1;
	  if(abs(wireId.wheel()) == 2)
	    hRes = &hRes_S1RZ_W2;
	}

      } else if(step == 2) {
	// Step 2
	if(wireId.superlayer() != 2) {
	  hRes = &hRes_S2RPhi;
	} else {
	  hResTot = &hRes_S2RZ;
	  if(wireId.wheel() == 0)
	    hRes = &hRes_S2RZ_W0;
	  if(abs(wireId.wheel()) == 1)
	    hRes = &hRes_S2RZ_W1;
	  if(abs(wireId.wheel()) == 2)
	    hRes = &hRes_S2RZ_W2;
	}

      } else if(step == 3) {
	// Step 3
	if(wireId.superlayer() != 2) {
	  hRes = &hRes_S3RPhi;
	} else {
	  hResTot = &hRes_S3RZ;
	  if(wireId.wheel() == 0)
	    hRes = &hRes_S3RZ_W0;
	  if(abs(wireId.wheel()) == 1)
	    hRes = &hRes_S3RZ_W1;
	  if(abs(wireId.wheel()) == 2)
	    hRes = &hRes_S3RZ_W2;
	}

      }

      // Fill
      hRes->Fill(simHitWireDist, recHitWireDist, simHitGlobalPos.eta(),
		 simHitGlobalPos.phi(),recHitErr);
      if(hResTot != 0)
	hResTot->Fill(simHitWireDist, recHitWireDist, simHitGlobalPos.eta(),
		      simHitGlobalPos.phi(),recHitErr);
      
    }

    // Fill Efficiencies
    HEff1DHit *hEff = 0;
    HEff1DHit *hEffTot = 0;
    
    if(step == 1) {
      // Step 1
      if(wireId.superlayer() != 2) {
	hEff = &hEff_S1RPhi;
      } else {
	hEffTot = &hEff_S1RZ;
	if(wireId.wheel() == 0)
	  hEff = &hEff_S1RZ_W0;
	if(abs(wireId.wheel()) == 1)
	  hEff = &hEff_S1RZ_W1;
	if(abs(wireId.wheel()) == 2)
	  hEff = &hEff_S1RZ_W2;
      }

    } else if(step == 2) {
      // Step 2
      if(wireId.superlayer() != 2) {
	hEff = &hEff_S2RPhi;
      } else {
	hEffTot = &hEff_S2RZ;
	if(wireId.wheel() == 0)
	  hEff = &hEff_S2RZ_W0;
	if(abs(wireId.wheel()) == 1)
	  hEff = &hEff_S2RZ_W1;
	if(abs(wireId.wheel()) == 2)
	  hEff = &hEff_S2RZ_W2;
      }

    } else if(step == 3) {
      // Step 3
      if(wireId.superlayer() != 2) {
	hEff = &hEff_S3RPhi;
      } else {
	hEffTot = &hEff_S3RZ;
	if(wireId.wheel() == 0)
	  hEff = &hEff_S3RZ_W0;
	if(abs(wireId.wheel()) == 1)
	  hEff = &hEff_S3RZ_W1;
	if(abs(wireId.wheel()) == 2)
	  hEff = &hEff_S3RZ_W2;
      }

    }
    // Fill
    hEff->Fill(simHitWireDist, simHitGlobalPos.eta(), simHitGlobalPos.phi(), recHitReconstructed);
    if(hEffTot != 0)
      hEffTot->Fill(simHitWireDist, simHitGlobalPos.eta(), simHitGlobalPos.phi(), recHitReconstructed);
  }
}

// Return the error on the measured (cm) coordinate
float DTRecHitQuality::recHitPositionError(const DTRecHit1DPair& recHit) {
  return sqrt(recHit.localPositionError(DTEnums::Left).xx());
}

// Return the error on the measured (cm) coordinate
float DTRecHitQuality::recHitPositionError(const DTRecHit1D& recHit) {
  return sqrt(recHit.localPositionError().xx());
}
