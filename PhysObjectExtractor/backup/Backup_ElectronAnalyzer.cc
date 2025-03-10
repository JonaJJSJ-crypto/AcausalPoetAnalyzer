//
// Package:    ElectronAnalyzer
// Class:      ElectronAnalyzer
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//classes to extract electron information
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

//Secondary Vertex Clases
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

//Drawer
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/deltaR.h"

//jetCollectio
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"

//Trigg object match
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

//classes to save data
#include "TTree.h"
#include "TFile.h"
#include<vector>

//
// class declaration
//

class ElectronAnalyzer : public edm::EDAnalyzer {
public:
  explicit ElectronAnalyzer(const edm::ParameterSet&);
  ~ElectronAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  virtual float effectiveArea0p3cone(float eta);

  //declare the input tag for GsfElectronCollection
  edm::InputTag electronInput;
  std::string filterName_;

  // ----------member data ---------------------------
  bool isData;

  TTree *mtree;
  int numelectron; //number of reco electrons in the event
  int numgenelec; //# of gen electrons in the event
  int numsecvec;  //number of secondary vertex disp in the event
  int numdisp;  //tool for finding best match for electrons
  std::vector<float> electron_e;
  std::vector<float> electron_pt;
  std::vector<float> electron_px;
  std::vector<float> electron_py;
  std::vector<float> electron_pz;
  std::vector<float> electron_vx;
  std::vector<float> electron_vy;
  std::vector<float> electron_vz;
  //std::vector<float> electron_vxerr;
  //std::vector<float> electron_vyerr;
  //std::vector<float> electron_vzerr;
  std::vector<float> electron_eta;
  std::vector<float> electron_phi;
  std::vector<float> electron_ch;
  std::vector<float> genelec_e;
  std::vector<float> genelec_pt;
  std::vector<float> genelec_px;
  std::vector<float> genelec_py;
  std::vector<float> genelec_pz;
  std::vector<float> genelec_eta;
  std::vector<float> genelec_phi;
  std::vector<float> genelec_ch;
  std::vector<float> genelec_DRscore;
  std::vector<float> electron_iso;
  std::vector<bool> electron_isLoose;
  std::vector<bool> electron_isMedium;
  std::vector<bool> electron_isTight;
  std::vector<int> trk_identity;
  std::vector<int> trk_isHQ;
  std::vector<float> trk_NChi2;
  std::vector<float> trk_d0;
  std::vector<float> trk_d0E;
  std::vector<float> trk_dZ;
  std::vector<float> trk_dZE;
  std::vector<int> trk_Nlayer;
  std::vector<float> electron_dxy;
  std::vector<float> electron_dz;
  std::vector<float> electron_dxyError;
  std::vector<float> electron_dzError;
  std::vector<float> secvec_posx;
  std::vector<float> secvec_posy;
  std::vector<float> secvec_posz;
  std::vector<float> secvec_poserrorx;
  std::vector<float> secvec_poserrory;
  std::vector<float> secvec_poserrorz;
  std::vector<int> secvec_eleTag;
  //std::vector<float> secvec_disp;
  //std::vector<float> secvec_dispR;
  //std::vector<float> secvec_deltaR;
  std::vector<float> secvec_phi;
  std::vector<float> secvec_eta;
  std::vector<float> secvec_deltaR1;
  std::vector<float> secvec_phi1;
  std::vector<float> secvec_eta1;
  std::vector<float> secvec_deltaR2;
  std::vector<float> secvec_phi2;
  std::vector<float> secvec_eta2;
  //std::vector<int> electron_Bsecvec;//best match for electron sec vec
  std::vector<int> electron_BdR;//best match delta R for electron sec vec
  std::vector<int> electron_secN;//# of printed electrons in Secvert
  std::vector<float> electron_deltaRsim;
  std::vector<float> electron_deltaR1sim;
  std::vector<float> electron_deltaR2sim;
  std::vector<float> electron_deltaRtrue;
  std::vector<float> electron_deltaR1true;
  std::vector<float> electron_deltaR2true;
  std::vector<float> Zjet_pt;
  std::vector<float> Bsp_x;
  std::vector<float> Bsp_y;
  std::vector<float> Bsp_z;
  std::vector<float> Bsp_sigmaz;
  std::vector<float> Bsp_dxdz;
  std::vector<float> Bsp_dydz;
  std::vector<float> Bsp_widthx;
  std::vector<float> Bsp_widthy;
  std::vector<float> electron_superclusterposx;
  std::vector<float> electron_superclusterposy;
  std::vector<float> electron_superclusterposz;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

ElectronAnalyzer::ElectronAnalyzer(const edm::ParameterSet& iConfig)
{
//now do what ever initialization is needed
  isData = iConfig.getParameter<bool>("isData");
  electronInput = iConfig.getParameter<edm::InputTag>("InputCollection");
  //filterName_ = iConfig.getParameter<std::string>("filterName");
  edm::Service<TFileService> fs;
  mtree = fs->make<TTree>("Events", "Events");


  mtree->Branch("numberelectron",&numelectron);
  mtree->GetBranch("numberelectron")->SetTitle("number of reco electrons");
  mtree->Branch("numbergenelec",&numgenelec);
  mtree->GetBranch("numbergenelec")->SetTitle("number of gen electrons");
  mtree->Branch("numbersecvec",&numsecvec);
  mtree->GetBranch("numbersecvec")->SetTitle("number of secondary vertex displascements");
  mtree->Branch("electron_e",&electron_e);
  mtree->GetBranch("electron_e")->SetTitle("electron energy");
  mtree->Branch("electron_pt",&electron_pt);
  mtree->GetBranch("electron_pt")->SetTitle("electron transverse momentum");
  mtree->Branch("electron_px",&electron_px);
  mtree->GetBranch("electron_px")->SetTitle("electron momentum x-component");
  mtree->Branch("electron_py",&electron_py);
  mtree->GetBranch("electron_py")->SetTitle("electron momentum y-component");
  mtree->Branch("electron_pz",&electron_pz);
  mtree->GetBranch("electron_pz")->SetTitle("electron momentum z-component");
  mtree->Branch("electron_vx",&electron_vx);
  mtree->GetBranch("electron_vx")->SetTitle("electron vertex x-component");
  mtree->Branch("electron_vy",&electron_vy);
  mtree->GetBranch("electron_vy")->SetTitle("electron vertex y-component");
  mtree->Branch("electron_vz",&electron_vz);
  mtree->GetBranch("electron_vz")->SetTitle("electron vertex z-component");
  //mtree->Branch("electron_vxerr",&electron_vxerr);
  //mtree->GetBranch("electron_vxerr")->SetTitle("electron vertex x-component error");
  //mtree->Branch("electron_vyerr",&electron_vyerr);
  //mtree->GetBranch("electron_vyerr")->SetTitle("electron vertex y-component error");
  //mtree->Branch("electron_vzerr",&electron_vzerr);
  //mtree->GetBranch("electron_vzerr")->SetTitle("electron vertex z-component error");
  mtree->Branch("electron_eta",&electron_eta);
  mtree->GetBranch("electron_eta")->SetTitle("electron pseudorapidity");
  mtree->Branch("electron_phi",&electron_phi);
  mtree->GetBranch("electron_phi")->SetTitle("electron polar angle");
  mtree->Branch("electron_ch",&electron_ch);
  mtree->GetBranch("electron_ch")->SetTitle("electron charge");
  mtree->Branch("genelec_e",&genelec_e);
  mtree->GetBranch("genelec_e")->SetTitle("electron gen energy");
  mtree->Branch("genelec_pt",&genelec_pt);
  mtree->GetBranch("genelec_pt")->SetTitle("electron gen transverse momentum");
  mtree->Branch("genelec_px",&genelec_px);
  mtree->GetBranch("genelec_px")->SetTitle("electron gen momentum x-component");
  mtree->Branch("genelec_py",&genelec_py);
  mtree->GetBranch("genelec_py")->SetTitle("electron gen momentum y-component");
  mtree->Branch("genelec_pz",&genelec_pz);
  mtree->GetBranch("genelec_pz")->SetTitle("electron gen momentum z-component");
  mtree->Branch("genelec_eta",&genelec_eta);
  mtree->GetBranch("genelec_eta")->SetTitle("electron gen pseudorapidity");
  mtree->Branch("genelec_phi",&genelec_phi);
  mtree->GetBranch("genelec_phi")->SetTitle("electron gen polar angle");
  mtree->Branch("genelec_ch",&genelec_ch);
  mtree->GetBranch("genelec_ch")->SetTitle("electron gen charge");
  mtree->Branch("genelec_DRscore",&genelec_DRscore);
  mtree->GetBranch("genelec_DRscore")->SetTitle("electron delta R score obtained during fitting");
  mtree->Branch("electron_iso",&electron_iso);
  mtree->GetBranch("electron_iso")->SetTitle("electron isolation");
  mtree->Branch("electron_isLoose",&electron_isLoose);
  mtree->GetBranch("electron_isLoose")->SetTitle("electron tagged loose");
  mtree->Branch("electron_isMedium",&electron_isMedium);
  mtree->GetBranch("electron_isMedium")->SetTitle("electron tagged medium");
  mtree->Branch("electron_isTight",&electron_isTight);
  mtree->GetBranch("electron_isTight")->SetTitle("electron tagged tight");
  mtree->Branch("trk_identity",&trk_identity);
  mtree->GetBranch("trk_identity")->SetTitle("electron track identity tag");
  mtree->Branch("trk_isHQ",&trk_isHQ);
  mtree->GetBranch("trk_isHQ")->SetTitle("electron track tagged HighQuality");
  mtree->Branch("trk_NChi2",&trk_NChi2);
  mtree->GetBranch("trk_NChi2")->SetTitle("electron track Normalized Chi squared (mm)");
  mtree->Branch("trk_d0",&trk_d0);
  mtree->GetBranch("trk_d0")->SetTitle("electron track transverse distance to beamspot (mm)");
  mtree->Branch("trk_d0E",&trk_d0E);
  mtree->GetBranch("trk_d0E")->SetTitle("electron track transverse distance to beamspot error (mm)");
  mtree->Branch("trk_dZ",&trk_dZ);
  mtree->GetBranch("trk_dZ")->SetTitle("electron track longitudinal distance to beamspot (mm)");
  mtree->Branch("trk_dZE",&trk_dZE);
  mtree->GetBranch("trk_dZE")->SetTitle("electron track longitudinal distance to beamspot error (mm)");
  mtree->Branch("trk_Nlayer",&trk_Nlayer);
  mtree->GetBranch("trk_Nlayer")->SetTitle("electron track number of hit layers (mm)");
  mtree->Branch("electron_dxy",&electron_dxy);
  mtree->GetBranch("electron_dxy")->SetTitle("electron transverse plane impact parameter (mm)");
  mtree->Branch("electron_dz",&electron_dz);
  mtree->GetBranch("electron_dz")->SetTitle("electron longitudinal impact parameter (mm)");
  mtree->Branch("electron_dxyError",&electron_dxyError);
  mtree->GetBranch("electron_dxyError")->SetTitle("electron transverse impact parameter uncertainty (mm)");
  mtree->Branch("electron_dzError",&electron_dzError);
  mtree->GetBranch("electron_dzError")->SetTitle("electron longitudinal impact parameter uncertainty (mm)");
  mtree->Branch("secvec_posx",&secvec_posx);
  mtree->GetBranch("secvec_posx")->SetTitle("secvec position x (mm)");
  mtree->Branch("secvec_posy",&secvec_posy);
  mtree->GetBranch("secvec_posy")->SetTitle("secvec position y (mm)");
  mtree->Branch("secvec_posz",&secvec_posz);
  mtree->GetBranch("secvec_posz")->SetTitle("secvec position z (mm)");
  mtree->Branch("secvec_poserrorx",&secvec_poserrorx);
  mtree->GetBranch("secvec_poserrorx")->SetTitle("secvec position x error (mm)");
  mtree->Branch("secvec_poserrory",&secvec_poserrory);
  mtree->GetBranch("secvec_poserrory")->SetTitle("secvec position y error (mm)");
  mtree->Branch("secvec_poserrorz",&secvec_poserrorz);
  mtree->GetBranch("secvec_poserrorz")->SetTitle("secvec position z error (mm)");
  //mtree->Branch("secvec_disp",&secvec_disp);
  //mtree->GetBranch("secvec_disp")->SetTitle("secvec displacement from primary vertex (mm)");
  //mtree->Branch("secvec_dispR",&secvec_dispR);
  //mtree->GetBranch("secvec_dispR")->SetTitle("secvec weigthed displacement from primary vertex");
  //mtree->Branch("secvec_deltaR",&secvec_deltaR);
  //mtree->GetBranch("secvec_deltaR")->SetTitle("1-2tracks deltaR");
  mtree->Branch("secvec_phi",&secvec_phi);
  mtree->GetBranch("secvec_phi")->SetTitle("Track seed phi");
  mtree->Branch("secvec_eta",&secvec_eta);
  mtree->GetBranch("secvec_eta")->SetTitle("Track seed eta");
  mtree->Branch("secvec_deltaR1",&secvec_deltaR1);
  mtree->GetBranch("secvec_deltaR1")->SetTitle("1st track deltaR");
  mtree->Branch("secvec_phi1",&secvec_phi1);
  mtree->GetBranch("secvec_phi1")->SetTitle("Track 1 phi");
  mtree->Branch("secvec_eta1",&secvec_eta1);
  mtree->GetBranch("secvec_eta1")->SetTitle("Track 1 eta");
  mtree->Branch("secvec_deltaR2",&secvec_deltaR2);
  mtree->GetBranch("secvec_deltaR2")->SetTitle("2nd track deltaR");
  mtree->Branch("secvec_phi2",&secvec_phi2);
  mtree->GetBranch("secvec_phi2")->SetTitle("Track 2 phi");
  mtree->Branch("secvec_eta2",&secvec_eta2);
  mtree->GetBranch("secvec_eta2")->SetTitle("Track 2 eta");
  /*mtree->Branch("electron_Bsecvec",&electron_Bsecvec);
  mtree->GetBranch("electron_Bsecvec")->SetTitle("best match electrons for secondary vertex");*/
  mtree->Branch("electron_BdR",&electron_BdR);
  mtree->GetBranch("electron_BdR")->SetTitle("best match delta R electrons for secondary vertex");
  mtree->Branch("electron_secN",&electron_secN);
  mtree->GetBranch("electron_secN")->SetTitle("# electrons for secondary vertex");
  mtree->Branch("electron_deltaRsim",&electron_deltaRsim);
  mtree->GetBranch("electron_deltaRsim")->SetTitle("1-2Electron deltaRsim");
  mtree->Branch("electron_deltaR1sim",&electron_deltaR1sim);
  mtree->GetBranch("electron_deltaR1sim")->SetTitle("1 Electron deltaRsim");
  mtree->Branch("electron_deltaR2sim",&electron_deltaR2sim);
  mtree->GetBranch("electron_deltaR2sim")->SetTitle("2 Electron deltaRsim");
  mtree->Branch("electron_deltaRtrue",&electron_deltaRtrue);
  mtree->GetBranch("electron_deltaRtrue")->SetTitle("1-2Electron deltaRtrue");
  mtree->Branch("electron_deltaR1true",&electron_deltaR1true);
  mtree->GetBranch("electron_deltaR1true")->SetTitle("1 Electron deltaRtrue");
  mtree->Branch("electron_deltaR2true",&electron_deltaR2true);
  mtree->GetBranch("electron_deltaR2true")->SetTitle("2 Electron deltaRtrue");
  mtree->Branch("Zjet_pt",&Zjet_pt);
  mtree->GetBranch("Zjet_pt")->SetTitle("Z daugthers pt");
  mtree->Branch("Bsp_x",&Bsp_x);
  mtree->GetBranch("Bsp_x")->SetTitle("vertex Beamspot position x (mm)");
  mtree->Branch("Bsp_y",&Bsp_y);
  mtree->GetBranch("Bsp_y")->SetTitle("vertex Beamspot position y (mm)");
  mtree->Branch("Bsp_z",&Bsp_z);
  mtree->GetBranch("Bsp_z")->SetTitle("vertex Beamspot position z (mm)");
  mtree->Branch("Bsp_sigmaz",&Bsp_sigmaz);
  mtree->GetBranch("Bsp_sigmaz")->SetTitle("vertex Beamspot sigma z (mm)");
  mtree->Branch("Bsp_dxdz",&Bsp_dxdz);
  mtree->GetBranch("Bsp_dxdz")->SetTitle("vertex Beamspot dxdz (mm)");
  mtree->Branch("Bsp_dydz",&Bsp_dydz);
  mtree->GetBranch("Bsp_dydz")->SetTitle("vertex Beamspot position dydz (mm)");
  mtree->Branch("Bsp_widthx",&Bsp_widthx);
  mtree->GetBranch("Bsp_widthx")->SetTitle("vertex Beamspot width x (mm)");
  mtree->Branch("electron_superclusterposx",&electron_superclusterposx);
  mtree->GetBranch("electron_superclusterposx")->SetTitle("Electron supercluster x position (mm)");
  mtree->Branch("electron_superclusterposy",&electron_superclusterposy);
  mtree->GetBranch("electron_superclusterposy")->SetTitle("Electron supercluster y position (mm)");
  mtree->Branch("electron_superclusterposz",&electron_superclusterposz);
  mtree->GetBranch("electron_superclusterposz")->SetTitle("Electron supercluster z position (mm)");
}

ElectronAnalyzer::~ElectronAnalyzer()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}

//
// member functions
//

float
ElectronAnalyzer::effectiveArea0p3cone(float eta)
{
  if(fabs(eta) < 1.0) return 0.13;
  else if(fabs(eta) < 1.479) return 0.14;
  else if(fabs(eta) < 2.0) return 0.07;
  else if(fabs(eta) < 2.2) return 0.09;
  else if(fabs(eta) < 2.3) return 0.11;
  else if(fabs(eta) < 2.4) return 0.11;
  else return 0.14;
}

// ------------ method called for each event  ------------
void
ElectronAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  using namespace reco;

  //cout<<"\n\n\n==========================================ESTA=============================================\n\n\n"<<endl;

  Handle<reco::GsfElectronCollection> myelectrons;
  iEvent.getByLabel(electronInput, myelectrons);
  Handle<reco::ConversionCollection> hConversions;
  iEvent.getByLabel("allConversions", hConversions);
  Handle<reco::BeamSpot> bsHandle;
  iEvent.getByLabel("offlineBeamSpot", bsHandle);
  const reco::BeamSpot &beamspot = *bsHandle.product();
  Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel(InputTag("offlinePrimaryVertices"), vertices);
  math::XYZPoint pv(vertices->begin()->position());
  Handle<double> rhoHandle;
  iEvent.getByLabel(InputTag("fixedGridRhoAll"), rhoHandle);

  numelectron = 0;
  numgenelec = 0;
  numsecvec = 0;
  numdisp = 0;
  electron_e.clear();
  electron_pt.clear();
  electron_px.clear();
  electron_py.clear();
  electron_pz.clear();
  electron_vx.clear();
  electron_vy.clear();
  electron_vz.clear();
  //electron_vxerr.clear();
  //electron_vyerr.clear();
  //electron_vzerr.clear();
  electron_eta.clear();
  electron_phi.clear();
  electron_ch.clear();
  genelec_e.clear();
  genelec_pt.clear();
  genelec_px.clear();
  genelec_py.clear();
  genelec_pz.clear();
  genelec_eta.clear();
  genelec_phi.clear();
  genelec_ch.clear();
  genelec_DRscore.clear();
  electron_iso.clear();
  electron_isLoose.clear();
  electron_isMedium.clear();
  electron_isTight.clear();
  trk_identity.clear();
  trk_isHQ.clear();
  trk_NChi2.clear();
  trk_d0.clear();
  trk_d0E.clear();
  trk_dZ.clear();
  trk_dZE.clear();
  trk_Nlayer.clear();
  electron_dxy.clear();
  electron_dz.clear();
  electron_dxyError.clear();
  electron_dzError.clear();
  secvec_posx.clear();
  secvec_posy.clear();
  secvec_posz.clear();
  secvec_poserrorx.clear();
  secvec_poserrory.clear();
  secvec_poserrorz.clear();

  //secvec_disp.clear();
  //secvec_dispR.clear();
  //secvec_deltaR.clear();
  secvec_phi.clear();
  secvec_eta.clear();
  secvec_deltaR1.clear();
  secvec_phi1.clear();
  secvec_eta1.clear();
  secvec_deltaR2.clear();
  secvec_phi2.clear();
  secvec_eta2.clear();
  //electron_Bsecvec.clear();
  electron_BdR.clear();
  electron_secN.clear();
  electron_deltaRsim.clear();
  electron_deltaR1sim.clear();
  electron_deltaR2sim.clear();
  electron_deltaRtrue.clear();
  electron_deltaR1true.clear();
  electron_deltaR2true.clear();
  Zjet_pt.clear();
  Bsp_x.clear();
  Bsp_y.clear();
  Bsp_z.clear();
  Bsp_sigmaz.clear();
  Bsp_dxdz.clear();
  Bsp_dydz.clear();
  Bsp_widthx.clear();
  Bsp_widthy.clear();
  electron_superclusterposx.clear();
  electron_superclusterposy.clear();
  electron_superclusterposz.clear();

  if(myelectrons.isValid()){
    // get the number of electrons in the event
    //numelectron=myelectrons->size();
    //cout<<"No.Ele: "<<numelectron<<endl;
    for (reco::GsfElectronCollection::const_iterator itElec=myelectrons->begin(); itElec!=myelectrons->end(); ++itElec){
     if(itElec->pt()>-1){
      numelectron++;//Here get actual number of electrons

      int missing_hits = itElec->gsfTrack()->trackerExpectedHitsInner().numberOfHits()-itElec->gsfTrack()->hitPattern().numberOfHits();
      bool passelectronveto = !ConversionTools::hasMatchedConversion(*itElec, hConversions, beamspot.position());

      float el_pfIso = 999;
      if (itElec->passingPflowPreselection()) {
	double rho = 0;
	if(rhoHandle.isValid()) rho = *(rhoHandle.product());
	double Aeff = effectiveArea0p3cone(itElec->eta());
	auto iso03 = itElec->pfIsolationVariables();
	el_pfIso = (iso03.chargedHadronIso + std::max(0.0,iso03.neutralHadronIso + iso03.photonIso - rho*Aeff))/itElec->pt();
      }
      auto trk = itElec->gsfTrack();
      bool isLoose = false, isMedium = false, isTight = false;
      if ( abs(itElec->eta()) <= 1.479 ) {
	if ( abs(itElec->deltaEtaSuperClusterTrackAtVtx())<.007 && abs(itElec->deltaPhiSuperClusterTrackAtVtx())<.15 &&
	     itElec->sigmaIetaIeta()<.01 && itElec->hadronicOverEm()<.12 &&
	     abs(trk->dxy(pv))<.02 && abs(trk->dz(pv))<.2 &&
	     missing_hits<=1 && passelectronveto==true &&
	     abs(1/itElec->ecalEnergy()-1/(itElec->ecalEnergy()/itElec->eSuperClusterOverP()))<.05 &&
	     el_pfIso<.15){

	  isLoose = true;

	  if ( abs(itElec->deltaEtaSuperClusterTrackAtVtx())<.004 && abs(itElec->deltaPhiSuperClusterTrackAtVtx())<.06 && abs(trk->dz(pv))<.1 ){
	    isMedium = true;

	    if (abs(itElec->deltaPhiSuperClusterTrackAtVtx())<.03 && missing_hits<=0 && el_pfIso<.10 ){
	      isTight = true;
	    }
	  }
	}
      }
      else if ( abs(itElec->eta()) > 1.479 && abs(itElec->eta()) < 2.5 ) {
	if ( abs(itElec->deltaEtaSuperClusterTrackAtVtx())<.009 && abs(itElec->deltaPhiSuperClusterTrackAtVtx())<.1 &&
	     itElec->sigmaIetaIeta()<.03 && itElec->hadronicOverEm()<.1 &&
	     abs(trk->dxy(pv))<.02 && abs(trk->dz(pv))<.2 &&
	     missing_hits<=1 && el_pfIso<.15 && passelectronveto==true &&
	     abs(1/itElec->ecalEnergy()-1/(itElec->ecalEnergy()/itElec->eSuperClusterOverP()))<.05) {

	  isLoose = true;

	  if ( abs(itElec->deltaEtaSuperClusterTrackAtVtx())<.007 && abs(itElec->deltaPhiSuperClusterTrackAtVtx())<.03 && abs(trk->dz(pv))<.1 ){
	    isMedium = true;

	    if ( abs(itElec->deltaEtaSuperClusterTrackAtVtx())<.005 && abs(itElec->deltaPhiSuperClusterTrackAtVtx())<.02 && missing_hits<=0 && el_pfIso<.10 ){
	      isTight = true;
	    }
	  }
	}
      }
      //cout<<trk->quality(reco::Track::highPurity)<<endl;
      //cout<<"Correspondance:"<<itElec->p4()<<endl;
      electron_e.push_back(itElec->energy());
      electron_pt.push_back(itElec->pt());
      electron_px.push_back(itElec->px());
      electron_py.push_back(itElec->py());
      electron_pz.push_back(itElec->pz());
      electron_px.push_back(itElec->vx());
      electron_py.push_back(itElec->vy());
      electron_pz.push_back(itElec->vz());
      //electron_px.push_back(itElec->vxerror());
      //electron_py.push_back(itElec->vyerror());
      //electron_pz.push_back(itElec->pzvzerror());
      electron_eta.push_back(itElec->eta());
      electron_phi.push_back(itElec->phi());
      electron_ch.push_back(itElec->charge());
      electron_iso.push_back(el_pfIso);
      electron_isLoose.push_back(isLoose);
      electron_isMedium.push_back(isMedium);
      electron_isTight.push_back(isTight);
      electron_dxy.push_back(trk->dxy(pv));
      electron_dz.push_back(trk->dz(pv));
      electron_dxyError.push_back(trk->d0Error());
      electron_dzError.push_back(trk->dzError());
	Bsp_x.push_back(beamspot.x0());
  	Bsp_y.push_back(beamspot.y0());
  	Bsp_z.push_back(beamspot.z0());
  	Bsp_sigmaz.push_back(beamspot.sigmaZ());
  	Bsp_dxdz.push_back(beamspot.dxdz());
  	Bsp_dydz.push_back(beamspot.dydz());
  	Bsp_widthx.push_back(beamspot.BeamWidthX());
  	Bsp_widthy.push_back(beamspot.BeamWidthY());
	electron_superclusterposx.push_back(itElec->superClusterPosition().x());
        electron_superclusterposy.push_back(itElec->superClusterPosition().y());
        electron_superclusterposz.push_back(itElec->superClusterPosition().z());


     }//fin if pt elec
    }
/////////////////////////////////////Best Gen particle match//////////////////////////////////
if(!isData){
   Handle<GenParticleCollection> genParticles;
   iEvent.getByLabel("genParticles", genParticles);
   vector<GenParticle> genElec;

   //storing selected electrons
   for(size_t i = 0; i < genParticles->size(); ++ i) {

     const GenParticle & p = (*genParticles)[i];
     int id = abs(p.pdgId());
     int st = p.status();
     if(id==11 && st==1){genElec.emplace_back(p);}
   }
   numgenelec=genElec.size();
   //Comparing deltaR GenvsReco
     //cout<<"\n\n No.Reco: "<<myelectrons->size()<<" No.Gen: "<<genElec.size()<<"\n\n"<<endl;

   for (GsfElectronCollection::const_iterator itElec1=myelectrons->begin(); itElec1!=myelectrons->end(); ++itElec1)
   {
    if(itElec1->pt()>-1){
     float saveDR=100;
     int idg=-1; //identity gen particle
     for(auto g=genElec.begin(); g!=genElec.end(); g++)
     {
        if(deltaR(g->p4(),itElec1->p4())<saveDR){
       	   saveDR=deltaR(g->p4(),itElec1->p4());
           idg=g-genElec.begin();
        }
     }
     //cout<<"Idex: "<<idg<<" Recop4: "<< itElec1->p4()<<endl;
     if(idg!=-1){
	auto g=genElec.begin()+idg;
	//cout<<"Genp4: "<<g->p4()<<" deltaR: "<<deltaR(g->p4(),itElec1->p4())<<"\n"<<endl;
      	genelec_e.push_back(g->energy());
      	genelec_pt.push_back(g->pt());
      	genelec_px.push_back(g->px());
      	genelec_py.push_back(g->py());
      	genelec_pz.push_back(g->pz());
      	genelec_eta.push_back(g->eta());
      	genelec_phi.push_back(g->phi());
      	genelec_ch.push_back(g->charge());
	genelec_DRscore.push_back(deltaR(g->p4(),itElec1->p4()));
     }
    }//fin If pt elec
   }

}

/////////////////////////////////////////End best Gen match//////////////////////////////////

///////////////////////////////////////Secondary Vertices//////////////////////////////////////////////

   Handle<TrackCollection> tracks;
   Handle<reco::TrackCollection> tks;
   ESHandle<TransientTrackBuilder> theB;

   iEvent.getByLabel("generalTracks", tks);
   iEvent.getByLabel("generalTracks", tracks);
/////////////////////TransientTrackBuilding////////////////

      iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
      vector<TransientTrack> t_tks = (*theB).build(tks);
//////////////////////secondary reco////////////////
   //Handle<PFJetCollection> myjets;//jets collection if neede
   //iEvent.getByLabel("ak5PFJets", myjets);
//displacement variables
  /*float dispx;
  float dispy;
  float dispz;
  float disp;

  float dispR;
  float xerr;
  float yerr;
  float zerr;*/

//iterative identification between track and electron using Delta R, only using tracks pt>20 and electrons from gsf
float saveDR;
int identyTrack[myelectrons->size()];
for(size_t x=0; x!=myelectrons->size();x++){identyTrack[x]=-1;}

int k=0;
//cout<<"size "<<myelectrons->size()<<endl;
for (GsfElectronCollection::const_iterator itElec1=myelectrons->begin(); itElec1!=myelectrons->end(); ++itElec1)
{
 saveDR=100;
 //if(itElec1->pt()>20) {
  int j=0;
  for(TrackCollection::const_iterator itTrack1 = tracks->begin(); itTrack1 != tracks->end(); ++itTrack1)
  {
     if(itTrack1->pt()>1){
        //cout<<"check\n";
	float DR_ET = deltaR(itElec1->eta(),itElec1->phi(),itTrack1->eta(),itTrack1->phi());
	if(DR_ET<saveDR){
	  saveDR=DR_ET;
	  identyTrack[k]=j;//track idex thta best fit Electrons
        }
        j++;
     //}
  }
 //}


//////////////////////////////////////////////////////////////////////

 if(identyTrack[k]!=-1){
  //cout<<"   "<<identyTrack[k]<<' '<<itElec1->pt()<<' ';
  int j=0;
  for(TrackCollection::const_iterator itTrack1 = tracks->begin(); itTrack1 != tracks->end(); ++itTrack1)
  {
     if(itTrack1->pt()>1){
      	if(j==identyTrack[k]){
	   /*auto trk1=itElec1->gsfTrack();
	   auto DRtest=deltaR(itElec1->eta(),itElec1->phi(),itTrack1->eta(),itTrack1->phi());
	   cout<<trk1->pt()<<' '<<j<<' '<< itTrack1->pt()<<' '<<DRtest<<endl;
	   cout<<"   "<<trk1->quality(reco::Track::highPurity)<<' '<<itTrack1->quality(reco::Track::highPurity);
	   cout<<' '<<itTrack1->normalizedChi2()<<' ';
	   cout<<itTrack1->dxy(beamspot.position())<<' '<<itTrack1->dz(beamspot.position())<<' '<<itTrack1->d0Error()<<endl;*/
	   trk_isHQ.push_back(itTrack1->quality(reco::Track::highPurity));
           trk_NChi2.push_back(itTrack1->normalizedChi2());
	   trk_d0.push_back(itTrack1->dxy(beamspot.position()));
	   trk_d0E.push_back(itTrack1->d0Error());
           trk_dZ.push_back(itTrack1->dz(beamspot.position()));
           trk_dZE.push_back(itTrack1->dzError());
	   trk_Nlayer.push_back(itTrack1->hitPattern().trackerLayersWithMeasurement());

	}
	j++;
     //}
  }
 }
 else {
   trk_isHQ.push_back(-1);
         trk_NChi2.push_back(-1);
   trk_d0.push_back(-1);
   trk_d0E.push_back(-1);
         trk_dZ.push_back(-1);
         trk_dZE.push_back(-1);
   trk_Nlayer.push_back(-1);
 }
 k++;
 electron_BdR.push_back(saveDR);

 //electron_Bsecvec.push_back(-1);//initialyzing best secondary vertex vector
}

for(size_t x =0; x<myelectrons->size(); x++)trk_identity.push_back(identyTrack[x]);

////////////////////////////////////////////////////////////////////



//fin identifier

vector<float> savedisp (myelectrons->size());
//float savedisp[myelectrons.size()];
int  i=0;
for(TrackCollection::const_iterator itTrack1 = tracks->begin();
       itTrack1 != tracks->end();
       ++itTrack1)
{
  if(itTrack1->pt()>1){   if(itTrack1->quality(reco::Track::highPurity) ){
      //cout<<itTrack1->quality(reco::Track::highPurity)<<endl;
      int	j2=0;
      for(TrackCollection::const_iterator itTrack2 = tracks->begin();
         itTrack2 != tracks->end();
         ++itTrack2)
         {

           if( itTrack2->pt()>1 && itTrack2->quality(reco::Track::highPurity) ){
            	//int j3=0;
            	//for(TrackCollection::const_iterator itTrack3 = tracks->begin(); itTrack3 != tracks->end(); ++itTrack3){
              	//if( itTrack3->pt()>1 && itTrack3->quality(reco::Track::highPurity) ){
                           KalmanVertexFitter fitter;
                           vector<TransientTrack> trackVec;

                           if(t_tks.size()>2 && itTrack1->pt()!=itTrack2->pt()){
                        	  //if(itTrack1->pt()!=itTrack3->pt() && itTrack2->pt()!=itTrack3->pt()){

                        	   /*cout<<"\npt1: "<<itTrack1->pt()<<" pt2: "<<itTrack2->pt()<<" pt3: "<<itTrack3->pt();
                        	   cout<<" deltaR1-2: "<<deltaR(itTrack1->phi(),itTrack1->eta(),itTrack2->phi(),itTrack2->eta());
                        	   cout<<" deltaR1-3: "<<deltaR(itTrack3->phi(),itTrack3->eta(),itTrack1->phi(),itTrack1->eta())<<endl;*/

                                   //auto trk1 = itElec1->gsfTrack();
                                   TransientTrack t_trk1 = (* theB).build(* itTrack1);

                                   //auto trk2 = itElec2->gsfTrack();
                                   TransientTrack t_trk2 = (* theB).build(* itTrack2);
                        	         //TransientTrack t_trk3 = (* theB).build(* itTrack3);

                                   trackVec.push_back(t_trk1);
                                   trackVec.push_back(t_trk2);
                        	         //trackVec.push_back(t_trk3);
                                   TransientVertex myVertex = fitter.vertex(trackVec);//reconstruction of secondary vertex Sometimes
                        	         trackVec.clear();

                                //int k=0;//este K es para el numero de desplazamientos
                               if(myVertex.isValid()){
                              	  numsecvec++;
                              	  secvec_posx.push_back(myVertex.position().x());
                                	secvec_posy.push_back(myVertex.position().y());
                                	secvec_posz.push_back(myVertex.position().z());
                                	secvec_poserrorx.push_back(myVertex.positionError().cxx());
                                	secvec_poserrory.push_back(myVertex.positionError().cyy());
                                	secvec_poserrorz.push_back(myVertex.positionError().czz());
                                  //Delta R entre Track1 Y electron en electron_BdR
                                  secvec_phi.push_back(itTrack1->phi());
                                  secvec_eta.push_back(itTrack1->eta());
                                  secvec_deltaR1.push_back(deltaR(itTrack1->eta(),itTrack1->phi(),itTrack2->eta(),itTrack2->phi()));
                                  secvec_phi1.push_back(itTrack2->phi());
                                  secvec_eta1.push_back(itTrack2->eta());
                                  secvec_deltaR2.push_back(deltaR(itTrack1->eta(),itTrack1->phi(),itTrack3->eta(),itTrack3->phi()));
                                  secvec_phi2.push_back(itTrack3->phi());
                                  secvec_eta2.push_back(itTrack3->eta());
                                  for (size_t k=0; k<myelectrons->size(); k++){
                                    if(identyTrack[k]==i) secvec_eleTag.push_back(k);
                                    else secvec_eleTag.push_back(-1);
                                  }
                              	/*for (GsfElectronCollection::const_iterator itElec1=myelectrons->begin(); itElec1!=myelectrons->end(); ++itElec1)
                              	{
                                           numdisp++;//total of secondary vertex disp per event
                              	     k++;//# of saved disp per SecondaryVertex

                              	     //primaryvertex
                              	     //cout<<"ptE: "<<itElec1->pt()<<endl;
                                           //cout<<"pvx: "<< itElec1->vx()<<" pvy: "<<itElec1->vy()<<" pvz: "<< itElec1->vz()<<endl;
                                           //cout<<"phiE: "<<itElec1->phi()<<" etaE: "<<itElec1->eta()<<" phiT: "<<itTrack1->phi()<<" etaT: "<<itTrack1->eta()<<endl;
                              	     //float deltaR_ET=deltaR(itElec1->phi(),itElec1->eta(),itTrack1->phi(),itTrack1->eta());
                              	     //cout<<"deltaR-ET: "<<deltaR_ET<<endl;

                                           //secondaryvertex
                                           //cout<<"PosX: "<< myVertex.position().x()<<" PosY: "<<myVertex.position().y()<<" PosZ: "<<myVertex.position().z()<<endl;
                                           dispx= itElec1->vx()-myVertex.position().x();
                                           dispy= itElec1->vy()-myVertex.position().y();
                                           dispz= itElec1->vz()-myVertex.position().z();

                                           //error calculation + displacement
                                           xerr=myVertex.positionError().cxx();
                                           yerr=myVertex.positionError().cyy();
                                           zerr=myVertex.positionError().czz();

                                           float difx = (myVertex.position().x())/(sqrt((xerr*xerr)+(yerr*yerr)+(zerr*zerr)));
                                           float dify = (myVertex.position().y())/(sqrt((xerr*xerr)+(yerr*yerr)+(zerr*zerr)));
                                           float difz = (myVertex.position().z())/(sqrt((xerr*xerr)+(yerr*yerr)+(zerr*zerr)));
                                           float err  = sqrt(difx*difx*xerr+dify*dify*yerr+difz*difz*zerr);

                                          disp= sqrt(dispx*dispx + dispy*dispy + dispz*dispz);
                                           //cout<<"Displacement: "<<disp<<endl;
                                           dispR= sqrt(dispx*dispx + dispy*dispy + dispz*dispz)/err;
                              	     //cout<<"Weigthed Displacement: "<<dispR<<'\n'<<endl;


                                           //displacements store
                              	     secvec_disp.push_back(disp);
                                           float totDR=1.5;
                                           if(dispR<totDR){secvec_dispR.push_back(dispR);}
                                           else{secvec_dispR.push_back(totDR);}

                                           //DeltaR identification
                                           float DeltaRPrima = deltaR(itElec1->eta(),itElec1->phi(),itTrack1->eta(),itTrack1->phi());
                              	     //cout<<"DeltaR: "<<DeltaRPrima<<'\n'<<endl;

                                           secvec_deltaR.push_back(DeltaRPrima);
                                           secvec_deltaR1.push_back(deltaR(itElec1->eta(),itElec1->phi(),itTrack2->eta(),itTrack2->phi()));
                                           secvec_deltaR2.push_back(deltaR(itElec1->eta(),itElec1->phi(),itTrack3->eta(),itTrack3->phi()));

                              	     //find best secvec
                                           if(identyTrack[k-1]==i)
                              	     {
                              		//cout<<itTrack1->quality(reco::Track::highPurity)<<"\n ";
                                              //cout<<itTrack1->pt()<<' '<<itElec1->pt()<<' '<<itElec1->eta()<<endl;
                                  		if(dispR > savedisp.at(k-1))
                                  		{
                                  		  savedisp.at(k-1)=dispR;
                                  		  electron_Bsecvec.at(k-1)=numdisp;
                                  		}
                              	     }

                              	 }*/// for itElec

                                }
                        	}//nonequal Tracks condition 3-2 3-1
                         }//Nonequal Tracks condition 1-2
                    //}
                    //j3++;
                  //}//for Track3
         }
         j2++;
      }
      i++;
  }//final HQ track
 }//finalde if pt track
}//fin for Track1
electron_secN.push_back(i);
}// Final de Is ValidElectron

  mtree->Fill();
  return;

}

// ------------ method called once each job just before starting event loop  ------------
void
ElectronAnalyzer::beginJob()
{}

// ------------ method called once each job just after ending the event loop  ------------
void
ElectronAnalyzer::endJob()
{}

// ------------ method called when starting to processes a run  ------------
void
ElectronAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{}

// ------------ method called when ending the processing of a run  ------------
void
ElectronAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{}
// ------------ method called when starting to processes a luminosity block  ------------
void
ElectronAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{}

// ------------ method called when ending the processing of a luminosity block  ------------
void
ElectronAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ElectronAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
 //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ElectronAnalyzer);
