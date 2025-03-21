// -*- C++ -*-
//
// Package:    JetAnalyzer
// Class:      JetAnalyzer
//

// system include files
#include <memory>
#include <TMath.h>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Common/interface/Ref.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "math.h"

//classes to extract PFJet information
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/SimpleJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/SimpleJetCorrectionUncertainty.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfo.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfoMatching.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"

//pf
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/JetReco/interface/PFJet.h"

//classes to save data
#include "TTree.h"
#include "TFile.h"
#include<vector>

#include "TRandom3.h"


//
// class declaration
//

class JetAnalyzer : public edm::EDAnalyzer {
public:
  explicit JetAnalyzer(const edm::ParameterSet&);
  ~JetAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  std::vector<float> factorLookup(float eta);
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual double getBtagEfficiency(double pt);
  virtual double getCtagEfficiency(double pt);
  virtual double getLFtagEfficiency(double pt);
  virtual double getBorCtagSF(double pt, double eta);
  virtual double getLFtagSF(double pt, double eta);
  virtual double uncertaintyForBTagSF( double pt, double eta);
  virtual double uncertaintyForLFTagSF( double pt, double eta);
  virtual void endJob() ;
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  //declare the input tag for PFJetCollection
  edm::InputTag jetInput;

  // ----------member data ---------------------------
  // jec variables
  std::vector<std::string> jecPayloadNames_;
  std::string              jecL1_;
  std::string              jecL2_;
  std::string              jecL3_;
  std::string              jecRes_;
  std::string              jecUncName_;
  std::string              jerResName_;
  boost::shared_ptr<JetCorrectionUncertainty> jecUnc_;
  boost::shared_ptr<FactorizedJetCorrector> jec_;
  boost::shared_ptr<SimpleJetCorrector> jer_;
  bool isData;

  int numjet; //number of jets in the event
  int numZjet; //number of First Z daug in the event
  TTree *mtree;
  std::vector<float> jet_e;
  std::vector<float> jet_pt;
  std::vector<float> jet_px;
  std::vector<float> jet_py;
  std::vector<float> jet_pz;
  std::vector<float> jet_eta;
  std::vector<float> jet_phi;
  std::vector<float> jet_ch;
  std::vector<float> jet_mass;
  std::vector<math::XYZPointF> const_pos;
  std::vector<float> const_pt;
  std::vector<float> genjet_e;
  std::vector<float> genjet_pt;
  std::vector<float> genjet_px;
  std::vector<float> genjet_py;
  std::vector<float> genjet_pz;
  std::vector<float> genjet_eta;
  std::vector<float> genjet_phi;
  std::vector<float> genjet_ch;
  std::vector<float> genjet_mass;
  std::vector<float> genjet_DRscore;
  std::vector<double> jet_btag;
  std::vector<float> corr_jet_pt;
  std::vector<float> corr_jet_ptUp;
  std::vector<float> corr_jet_ptDown;
  std::vector<float> corr_jet_ptSmearUp;
  std::vector<float> corr_jet_ptSmearDown;
  std::vector<int> NConstituents;
  std::vector<float> NeutralHF;
  std::vector<float> NeutralEMF;
  std::vector<float> MuonF;
  std::vector<float> ChargedEMF;
  std::vector<float> ChargedHF;
  std::vector<float> ChargedMult;

  float btagWeight;
  float btagWeightUp;
  float btagWeightDn;

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

JetAnalyzer::JetAnalyzer(const edm::ParameterSet& iConfig)
{
//now do what ever initialization is needed
  jetInput = iConfig.getParameter<edm::InputTag>("InputCollection");
  edm::Service<TFileService> fs;
  mtree = fs->make<TTree>("Events", "Events");

  isData = iConfig.getParameter<bool>("isData");
  jecL1_ = iConfig.getParameter<edm::FileInPath>("jecL1Name").fullPath(); // JEC level payloads
  jecL2_ = iConfig.getParameter<edm::FileInPath>("jecL2Name").fullPath(); // JEC level payloads
  jecL3_ = iConfig.getParameter<edm::FileInPath>("jecL3Name").fullPath(); // JEC level payloads
  jecRes_= iConfig.getParameter<edm::FileInPath>("jecResName").fullPath();
  jecUncName_ = iConfig.getParameter<edm::FileInPath>("jecUncName").fullPath(); // JEC uncertainties
  jerResName_ = iConfig.getParameter<edm::FileInPath>("jerResName").fullPath(); // JER Resolutions

  //Get the factorized jet corrector parameters.
  jecPayloadNames_.push_back(jecL1_);
  jecPayloadNames_.push_back(jecL2_);
  jecPayloadNames_.push_back(jecL3_);
  if( isData == true ) jecPayloadNames_.push_back(jecRes_);
  std::vector<JetCorrectorParameters> vPar;
  for ( std::vector<std::string>::const_iterator payloadBegin = jecPayloadNames_.begin(),
	  payloadEnd = jecPayloadNames_.end(), ipayload = payloadBegin; ipayload != payloadEnd; ++ipayload ) {
    JetCorrectorParameters pars(*ipayload);
    vPar.push_back(pars);
  }

  // Make the FactorizedJetCorrector and Uncertainty
  jec_ = boost::shared_ptr<FactorizedJetCorrector> ( new FactorizedJetCorrector(vPar) );
  jecUnc_ = boost::shared_ptr<JetCorrectionUncertainty>( new JetCorrectionUncertainty(jecUncName_) );
  JetCorrectorParameters *jerPar = new JetCorrectorParameters(jerResName_);
  jer_ = boost::shared_ptr<SimpleJetCorrector>( new SimpleJetCorrector(*jerPar) );


  mtree->Branch("numberjet",&numjet);
  mtree->GetBranch("numberjet")->SetTitle("Number of Jets");
  mtree->Branch("jet_e",&jet_e);
  mtree->GetBranch("jet_e")->SetTitle("Uncorrected Jet Energy");
  mtree->Branch("jet_pt",&jet_pt);
  mtree->GetBranch("jet_pt")->SetTitle("Uncorrected Transverse Jet Momentum");
  mtree->Branch("jet_px",&jet_px);
  mtree->GetBranch("jet_px")->SetTitle("X-Component of Jet Momentum");
  mtree->Branch("jet_py",&jet_py);
  mtree->GetBranch("jet_py")->SetTitle("Y-Component of Jet Momentum");
  mtree->Branch("jet_pz",&jet_pz);
  mtree->GetBranch("jet_pz")->SetTitle("Z-Component of Jet Momentum");
  mtree->Branch("jet_eta",&jet_eta);
  mtree->GetBranch("jet_eta")->SetTitle("Jet Eta");
  mtree->Branch("jet_phi",&jet_phi);
  mtree->GetBranch("jet_phi")->SetTitle("Jet Phi");
  mtree->Branch("jet_ch",&jet_ch);
  mtree->GetBranch("jet_ch")->SetTitle("Jet Charge");
  mtree->Branch("jet_mass",&jet_mass);
  mtree->GetBranch("jet_mass")->SetTitle("Jet Mass");
  mtree->Branch("const_pos",&const_pos);
  mtree->GetBranch("const_pos")->SetTitle("jet's most energetic constituent entrance at ecal");
  mtree->Branch("const_pt",&const_pt);
  mtree->GetBranch("const_pt")->SetTitle("jet's most energetic constituent pt");
  mtree->Branch("numberZjet",&numZjet);
  mtree->GetBranch("numberZjet")->SetTitle("Number of first Z daugthers");
  mtree->Branch("genjet_e",&genjet_e);
  mtree->GetBranch("genjet_e")->SetTitle("Uncorrected gen Jet Energy");
  mtree->Branch("genjet_pt",&genjet_pt);
  mtree->GetBranch("genjet_pt")->SetTitle("Uncorrected gen Transverse Jet Momentum");
  mtree->Branch("genjet_px",&genjet_px);
  mtree->GetBranch("genjet_px")->SetTitle("X-Component of gen Jet Momentum");
  mtree->Branch("genjet_py",&genjet_py);
  mtree->GetBranch("genjet_py")->SetTitle("Y-Component of gen Jet Momentum");
  mtree->Branch("genjet_pz",&genjet_pz);
  mtree->GetBranch("genjet_pz")->SetTitle("Z-Component of gen Jet Momentum");
  mtree->Branch("genjet_eta",&genjet_eta);
  mtree->GetBranch("genjet_eta")->SetTitle("gen Jet Eta");
  mtree->Branch("genjet_phi",&genjet_phi);
  mtree->GetBranch("genjet_phi")->SetTitle("gen Jet Phi");
  mtree->Branch("genjet_ch",&genjet_ch);
  mtree->GetBranch("genjet_ch")->SetTitle("gen Jet Charge");
  mtree->Branch("genjet_mass",&genjet_mass);
  mtree->GetBranch("genjet_mass")->SetTitle("gen Jet Mass");
  mtree->Branch("genjet_DRscore",&genjet_DRscore);
  mtree->GetBranch("genjet_DRscore")->SetTitle("gen Jet Delta R for matching with gen particles");
  mtree->Branch("jet_btag",&jet_btag);
  mtree->GetBranch("jet_btag")->SetTitle("Jet Btagging Discriminant (CSV)");
  mtree->Branch("corr_jet_pt",&corr_jet_pt);
  mtree->GetBranch("corr_jet_pt")->SetTitle("Corrected Transverse Jet Momentum");
  mtree->Branch("corr_jet_ptUp",&corr_jet_ptUp);
  mtree->GetBranch("corr_jet_ptUp")->SetTitle("Corrected Transverse Jet Momentum (JEC Shifted Up)");
  mtree->Branch("corr_jet_ptDown",&corr_jet_ptDown);
  mtree->GetBranch("corr_jet_ptDown")->SetTitle("Corrected Transverse Jet Momentum (JEC Shifted Down)");
  mtree->Branch("corr_jet_ptSmearUp",&corr_jet_ptSmearUp);
  mtree->GetBranch("corr_jet_ptSmearUp")->SetTitle("Corrected Transverse Jet Momentum (JER Shifted Up)");
  mtree->Branch("corr_jet_ptSmearDown",&corr_jet_ptSmearDown);
  mtree->GetBranch("corr_jet_ptSmearDown")->SetTitle("Corrected Transverse Jet Momentum (JER Shifted Down)");
  mtree->Branch("btag_Weight", &btagWeight);
  mtree->GetBranch("btag_Weight")->SetTitle("B-Tag event weight");
  mtree->Branch("btag_WeightUp", &btagWeightUp);
  mtree->GetBranch("btag_WeightUp")->SetTitle("B-Tag Up event weight");
  mtree->Branch("btag_WeightDn", &btagWeightDn);
  mtree->GetBranch("btag_WeightDn")->SetTitle("B-Tag Down event weight");
  mtree->Branch("NConstituents", &NConstituents);
  mtree->GetBranch("NConstituents")->SetTitle("Jet # of constituents");
  mtree->Branch("NeutralHF", &NeutralHF);
  mtree->GetBranch("NeutralHF")->SetTitle("Jet neutral hadron energy fraction");
  mtree->Branch("ChargedEMF", &ChargedEMF);
  mtree->GetBranch("ChargedEMF")->SetTitle("Jet charged EM energy fraction");
  mtree->Branch("ChargedHF", &ChargedHF);
  mtree->GetBranch("ChargedHF")->SetTitle("Jet charged hadron energy fraction");
  mtree->Branch("ChargedMult", &ChargedMult);
  mtree->GetBranch("ChargedMult")->SetTitle("Jet charged multiplicity");
  mtree->Branch("NeutralEMF", &NeutralEMF);
  mtree->GetBranch("NeutralEMF")->SetTitle("Jet neutral EM energy fraction");
  mtree->Branch("MuonF", &MuonF);
  mtree->GetBranch("MuonF")->SetTitle("Jet muon energy fraction");

}

JetAnalyzer::~JetAnalyzer()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}

//
// member functions
//

// ------------ method called for each event  ------------

//Function that gets the efficiencies for B tag jets.
//returns hard coded values from data analyzed from out root -l plotBEff.C

double
JetAnalyzer::getBtagEfficiency(double pt){
  if(pt < 25) return 0.263407;
  else if(pt < 50) return 0.548796;
  else if(pt < 75) return 0.656801;
  else if(pt < 100) return 0.689167;
  else if(pt < 125) return 0.697911;
  else if(pt < 150) return 0.700187;
  else if(pt < 200) return 0.679236;
  else if(pt < 400) return 0.625296;
  else return 0.394916;
}

//Function that gets the efficiencies for C tag jets.
//returns hard coded values from data analyzed from out root -l plotBEff.C

double
JetAnalyzer::getCtagEfficiency(double pt){
  if(pt < 25) return 0.065630;
  else if(pt < 50) return 0.161601;
  else if(pt < 75) return 0.209222;
  else if(pt < 100) return 0.242979;
  else if(pt < 125) return 0.223005;
  else if(pt < 150) return 0.210210;
  else if(pt < 200) return 0.225191;
  else if(pt < 400) return 0.227437;
  else return 0.153846;
}

//Function that gets the efficiencies for LF tag jets.
//returns hard coded values from data analyzed from out root -l plotBEff.C

double
JetAnalyzer::getLFtagEfficiency(double pt){
  if(pt < 25) return 0.002394;
  else if(pt < 50) return 0.012683;
  else if(pt < 75) return 0.011459;
  else if(pt < 100) return 0.012960;
  else if(pt < 125) return 0.011424;
  else if(pt < 150) return 0.011727;
  else if(pt < 200) return 0.011302;
  else if(pt < 400) return 0.014760;
  else return 0.011628;
}

//Function that gets the SF for B or C tag jets since there equations from the CSV.csv files are the same.
//returns the SF of the B or C tag jet depending on the pt of the jet

double
JetAnalyzer::getBorCtagSF(double pt, double eta){
  if (pt > 670.) pt = 670;
  if(fabs(eta) > 2.4 or pt<20.) return 1.0;

  return 0.92955*((1.+(0.0589629*pt))/(1.+(0.0568063*pt)));
}

//Function that gets the SF for lf tag jets since there equations from the CSV.csv files are the same.
//returns the SF of the lf jet depending on the eta of the jet

double
JetAnalyzer::getLFtagSF(double pt, double eta){
  if (pt > 1000.) pt = 1000;
  if(fabs(eta) > 2.4 or pt<20.) return 1.0;
  if(eta < 0.8)
    return (((0.922288+(0.00134434*pt))+(-3.14949e-06*(pt*pt)))+(2.08253e-09*(pt*(pt*pt))));
  else if (eta < 1.6)
    return (((0.908178+(0.00117751*pt))+(-3.16036e-06*(pt*pt)))+(2.41646e-09*(pt*(pt*pt))));
  else
    return (((0.869129+(0.000791629*pt))+(-2.62216e-06*(pt*pt)))+(2.49432e-09*(pt*(pt*pt))));
}


//Function that gets the uncertainty for B tag jets.
//returns the uncertainty from the CSV.csv file depending on the pt of the jet.
//This will also be used to find the uncertainty of C tag jets but will be multiplied by 2 since the uncertainty of C tag jets are 2 times that of B tag jets

double
JetAnalyzer::uncertaintyForBTagSF( double pt, double eta){
  if(fabs(eta) > 2.4 or pt<20.) return 0;
  if(pt < 30) return 0.0466655;
  else if(pt < 40) return 0.0203547;
  else if(pt < 50) return 0.0187707;
  else if(pt < 60) return 0.0250719;
  else if(pt < 70) return 0.023081;
  else if(pt < 80) return 0.0183273;
  else if(pt < 100) return 0.0256502;
  else if(pt < 120) return 0.0189555;
  else if(pt < 160) return 0.0236561;
  else if(pt < 210) return 0.0307624;
  else if(pt < 260) return 0.0387889;
  else if(pt < 320) return 0.0443912;
  else if(pt < 400) return 0.0693573;
  else if(pt < 500) return 0.0650147;
  else return 0.066886;
}


//Function that gets the uncertainty for LF tag jets.
//returns the uncertainty from the CSV.csv file depending on the eta of the jet.

double
JetAnalyzer::uncertaintyForLFTagSF( double pt, double eta){
  if (pt > 1000.)  pt = 1000;
  if(fabs(eta) > 2.4 or pt<20.)  return 0;
  if(eta < 0.8)
    return ((((1.01835+(0.0018277*pt))+(-4.37801e-06*(pt*pt)))+(2.90957e-09*(pt*(pt*pt))))-(((0.826257+(0.000858843*pt))+(-1.91563e-06*(pt*pt)))+(1.25331e-09*(pt*(pt*pt)))))/2.0;
  else if (eta < 1.6)
    return ((((1.00329+(0.00152898*pt))+(-4.21068e-06*(pt*pt)))+(3.23445e-09*(pt*(pt*pt))))-(((0.813077+(0.000824155*pt))+(-2.10494e-06*(pt*pt)))+(1.59692e-09*(pt*(pt*pt)))))/2.0;
  else
    return  ((((0.953682+(0.00104827*pt))+(-3.71967e-06*(pt*pt)))+(3.73067e-09*(pt*(pt*pt))))-(((0.784552+(0.000534202*pt))+(-1.52298e-06*(pt*pt)))+(1.26036e-09*(pt*(pt*pt)))))/2.0;
}


void
JetAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;
  using namespace std;
  using namespace reco;

  Handle<reco::PFJetCollection> myjets;
  iEvent.getByLabel(jetInput, myjets);
  Handle<reco::JetTagCollection> btags;
  iEvent.getByLabel(InputTag("combinedSecondaryVertexBJetTags"), btags);
  Handle<double> rhoHandle;
  iEvent.getByLabel(InputTag("fixedGridRhoAll"), rhoHandle);
  Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel(InputTag("offlinePrimaryVertices"), vertices);
  Handle<reco::JetFlavourInfoMatchingCollection> injets;
  if (!isData){
    iEvent.getByLabel(InputTag("jetFlavourInfosAK5PFJets"), injets);
  }

  numjet = 0;
  jet_e.clear();
  jet_pt.clear();
  jet_px.clear();
  jet_py.clear();
  jet_pz.clear();
  jet_eta.clear();
  jet_phi.clear();
  jet_ch.clear();
  jet_mass.clear();
  const_pos.clear();
  const_pt.clear();
  numZjet = 0;
  genjet_e.clear();
  genjet_pt.clear();
  genjet_px.clear();
  genjet_py.clear();
  genjet_pz.clear();
  genjet_eta.clear();
  genjet_phi.clear();
  genjet_ch.clear();
  genjet_mass.clear();
  genjet_DRscore.clear();
  jet_btag.clear();
  corr_jet_pt.clear();
  corr_jet_ptUp.clear();
  corr_jet_ptDown.clear();
  corr_jet_ptSmearUp.clear();
  corr_jet_ptSmearDown.clear();
  NConstituents.clear();
  NeutralHF.clear();
  NeutralEMF.clear();
  MuonF.clear();
  ChargedEMF.clear();
  ChargedHF.clear();
  ChargedMult.clear();


///////////////////Z dau to jet identifier/////////////////////////////
        vector<GenParticle> genZdau;

        if(!isData){
          Handle<GenParticleCollection> genParticles;
          iEvent.getByLabel("genParticles", genParticles);

          for(size_t i = 0; i < genParticles->size(); ++ i) {
            const GenParticle & p = (*genParticles)[i];
            int id = abs(p.pdgId());
            //cout<<id<<endl;

            if(id==23){

		int n = p.numberOfDaughters();
		//cout<<"Nd: "<<n<<endl;

	     for(int j = 0; j < n; ++ j) {

	   	const Candidate * d = p.daughter( j );
                //cout<<"pt: "<<d->pt()<<" pdg: "<<d->pdgId()<<endl;

		for(size_t k = 0; k < genParticles->size(); ++ k) {

		  const GenParticle & dit = (*genParticles)[k];
		  int ditid=dit.pdgId();
		  //cout<<ditid<<endl;

		  if( dit.pt()==d->pt() && ditid==d->pdgId() ){
			genZdau.emplace_back(dit);
			//if(ditid>10 && ditid<19) cout<<"pdgl: "<<ditid<<endl;
		  }
		}
	      }
	    }
          }
	  numZjet=genZdau.size();

	}
///////////////////Z dau to jet identifier///////////////////



  if(myjets.isValid()){

    int hadronFlavour;
    double eff, SF, SFu, SFd, corrpt;
    double corr, corrUp, corrDown;
    float ptscale, ptscale_down, ptscale_up, res;
    double MC = 1;
    btagWeight = 1;
    btagWeightUp = 1;
    btagWeightDn = 1;
    int min_pt = 20;

    for (reco::PFJetCollection::const_iterator itjet=myjets->begin(); itjet!=myjets->end(); ++itjet){
      reco::Candidate::LorentzVector uncorrJet = itjet->p4();

      corr = 1;
      jec_->setJetEta( uncorrJet.eta() );
      jec_->setJetPt ( uncorrJet.pt() );
      jec_->setJetE  ( uncorrJet.energy() );
      jec_->setJetA  ( itjet->jetArea() );
      jec_->setRho   ( *(rhoHandle.product()) );
      jec_->setNPV   ( vertices->size() );
      corr = jec_->getCorrection();

      corrUp = 1.0;
      corrDown = 1.0;

      if( fabs(itjet->eta()) < 5) jecUnc_->setJetEta( itjet->eta() );
      else jecUnc_->setJetEta( 4.99 );
      jecUnc_->setJetPt( itjet->pt() );
      corrUp = (1 + fabs(jecUnc_->getUncertainty(1)));

      if( fabs(itjet->eta()) < 5) jecUnc_->setJetEta( itjet->eta() );
      else jecUnc_->setJetEta( 4.99 );
      jecUnc_->setJetPt( itjet->pt() );
      corrDown = (1 - fabs(jecUnc_->getUncertainty(-1)));

      ptscale = 1;
      ptscale_down = 1;
      ptscale_up = 1;
      res = 1;
      if(!isData) {
	std::vector<float> factors = factorLookup(fabs(itjet->eta())); // returns in order {factor, factor_down, factor_up}
	std::vector<float> feta;
	std::vector<float> PTNPU;
	float pt = corr * uncorrJet.pt();
	feta.push_back( fabs(uncorrJet.eta()) );
	PTNPU.push_back( pt );
	PTNPU.push_back( vertices->size() );

	res = jer_->correction(feta, PTNPU);

	TRandom3 JERrand;

	JERrand.SetSeed(abs(static_cast<int>(itjet->phi()*1e4)));
	ptscale = max(0.0, 1.0 + JERrand.Gaus(0, res)*sqrt(max(0.0, factors[0]*factors[0] - 1.0)));

	JERrand.SetSeed(abs(static_cast<int>(itjet->phi()*1e4)));
	ptscale_down = max(0.0, 1.0 + JERrand.Gaus(0, res)*sqrt(max(0.0, factors[1]*factors[1] - 1.0)));

	JERrand.SetSeed(abs(static_cast<int>(itjet->phi()*1e4)));
	ptscale_up = max(0.0, 1.0 + JERrand.Gaus(0, res)*sqrt(max(0.0, factors[2]*factors[2] - 1.0)));
      }

      if (ptscale*corr*uncorrJet.pt() >= min_pt){
	/*cout<<itjet->getPFConstituents().size()<<' '<<itjet->chargedMultiplicity()<<' '<<itjet->chargedHadronEnergyFraction();
	cout<<' '<<itjet->neutralHadronEnergyFraction()<<' '<<itjet->neutralEmEnergyFraction()<<' '<<itjet->muonEnergyFraction()
	<<' '<<itjet->chargedEmEnergyFraction()<<endl;*/
	  const_pos.push_back(itjet->getPFConstituent(0)->positionAtECALEntrance());
	  const_pt.push_back(itjet->getPFConstituent(0)->pt());
  	NConstituents.push_back(itjet->getPFConstituents().size());
	NeutralEMF.push_back(itjet->neutralEmEnergyFraction());
	MuonF.push_back(itjet->muonEnergyFraction());
  	NeutralHF.push_back(itjet->neutralHadronEnergyFraction());
  	ChargedEMF.push_back(itjet->chargedEmEnergyFraction());
  	ChargedHF.push_back(itjet->chargedHadronEnergyFraction());
  	ChargedMult.push_back(itjet->chargedMultiplicity());

	jet_e.push_back(itjet->energy());
	jet_pt.push_back(itjet->pt());
	jet_px.push_back(itjet->px());
	jet_py.push_back(itjet->py());
	jet_pz.push_back(itjet->pz());
	jet_eta.push_back(itjet->eta());
	jet_phi.push_back(itjet->phi());
	jet_ch.push_back(itjet->charge());
	jet_mass.push_back(itjet->mass());

	if(btags.isValid() && (itjet - myjets->begin()) < btags->size()) {
	  jet_btag.push_back(btags->operator[](itjet - myjets->begin()).second);
	}
	else jet_btag.push_back(-999);
	corr_jet_pt.push_back(ptscale*corr*uncorrJet.pt());
	corr_jet_ptUp.push_back(ptscale*corrUp*uncorrJet.pt());
	corr_jet_ptDown.push_back(ptscale*corrDown*uncorrJet.pt());
	corr_jet_ptSmearUp.push_back(ptscale_up*corr*uncorrJet.pt());
	corr_jet_ptSmearDown.push_back(ptscale_down*corr*uncorrJet.pt());

///////////////////Z dau to jet identifier/////////////////////////////
	if(!isData){
     	  float saveDR=100;
	  int idg=-1; //identity gen particle
	  for(auto g=genZdau.begin(); g!=genZdau.end(); g++)
     	  {
      	    if(deltaR(g->eta(),g->phi(),itjet->eta(),itjet->phi())<saveDR){
       	      saveDR=deltaR(g->eta(),g->phi(),itjet->eta(),itjet->phi());
              idg=g-genZdau.begin();

            }
     	  }

	  if(idg!=-1){
	    auto g=genZdau.begin()+idg;
	        genjet_e.push_back(g->energy());
        	genjet_pt.push_back(g->pt());
        	genjet_px.push_back(g->px());
        	genjet_py.push_back(g->py());
        	genjet_pz.push_back(g->pz());
        	genjet_eta.push_back(g->eta());
        	genjet_phi.push_back(g->phi());
        	genjet_ch.push_back(g->charge());
        	genjet_mass.push_back(g->mass());
                genjet_DRscore.push_back(deltaR(g->eta(),g->phi(),itjet->eta(),itjet->phi()));

	  }
	}
///////////////////Z dau to jet identifier///////////////////

	if (!isData){
	  SF = 1;
	  SFu = 1;
	  SFd = 1;
	  eff = 1;
	  reco::JetFlavourInfo aInfo = injets->operator[](itjet - myjets->begin()).second;
	  hadronFlavour = aInfo.getPartonFlavour();
	  corrpt = corr_jet_pt.at(numjet);

	  if (jet_btag.at(numjet)> 0.679){
	    if(abs(hadronFlavour) == 5){
	      eff = getBtagEfficiency(corrpt);
	      SF = getBorCtagSF(corrpt, jet_eta.at(numjet));
	      SFu = SF + uncertaintyForBTagSF(corrpt, jet_eta.at(numjet));
	      SFd = SF - uncertaintyForBTagSF(corrpt, jet_eta.at(numjet));
	    } else if(abs(hadronFlavour) == 4){
	      eff = getCtagEfficiency(corrpt);
	      SF = getBorCtagSF(corrpt, jet_eta.at(numjet));
	      SFu = SF + (2 * uncertaintyForBTagSF(corrpt, jet_eta.at(numjet)));
	      SFd = SF - (2 * uncertaintyForBTagSF(corrpt, jet_eta.at(numjet)));
	    } else {
	      eff = getLFtagEfficiency(corrpt);
	      SF = getLFtagSF(corrpt, jet_eta.at(numjet));
	      SFu = SF + ( uncertaintyForLFTagSF(corrpt, jet_eta.at(numjet)));
	      SFd = SF - ( uncertaintyForLFTagSF(corrpt, jet_eta.at(numjet)));
	    }
	    MC *= eff;
	    btagWeight *= SF * eff;
	    btagWeightUp *= SFu * eff;
	    btagWeightDn *= SFd * eff;
	  }
	  else {
	    if(abs(hadronFlavour) == 5){
	      eff = getBtagEfficiency(corrpt);
	      SF = getBorCtagSF(corrpt, jet_eta.at(numjet));
	      SFu = SF + uncertaintyForBTagSF(corrpt, jet_eta.at(numjet));
	      SFd = SF - uncertaintyForBTagSF(corrpt, jet_eta.at(numjet));
	    } else if(abs(hadronFlavour) == 4){
	      eff = getCtagEfficiency(corrpt);
	      SF = getBorCtagSF(corrpt, jet_eta.at(numjet));
	      SFu = SF + (2 * uncertaintyForBTagSF(corrpt, jet_eta.at(numjet)));
	      SFd = SF - (2 * uncertaintyForBTagSF(corrpt, jet_eta.at(numjet)));
	    } else {
	      eff = getLFtagEfficiency(corrpt);
	      SF = getLFtagSF(corrpt, jet_eta.at(numjet));
	      SFu = SF + ( uncertaintyForLFTagSF(corrpt, jet_eta.at(numjet)));
	      SFd = SF - ( uncertaintyForLFTagSF(corrpt, jet_eta.at(numjet)));
	    }
	    MC *= (1 - eff);
	    btagWeight *= (1 - ( SF * eff));
	    btagWeightUp *= (1 - (SFu * eff));
	    btagWeightDn *= (1 -  (SFd * eff));
	  }
	}
	++numjet;
      }
    }
    btagWeight = (btagWeight/MC);
    btagWeightUp = (btagWeightUp/MC);
    btagWeightDn = (btagWeightDn/MC);
   }
  mtree->Fill();
  return;

}

// ------------ method called once each job just before starting event loop  ------------
void
JetAnalyzer::beginJob()
{}

// ------------ method called once each job just after ending the event loop  ------------
void
JetAnalyzer::endJob()
{}

// ------------ method called when starting to processes a run  ------------
void
JetAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{}

// ------------ method called when ending the processing of a run  ------------
void
JetAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{}
// ------------ method called when starting to processes a luminosity block  ------------
void
JetAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{}

// ------------ method called when ending the processing of a luminosity block  ------------
void
JetAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
JetAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
 //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
std::vector<float>
JetAnalyzer::factorLookup(float eta) { //used in jet loop for JER factor value
  //eta input is > 0
  if(eta > 3.2) return {1.056, 0.865, 1.247}; // {factor, factor_down, factor_up}
  else if(eta > 2.8) return {1.395, 1.332, 1.468};
  else if(eta > 2.3) return {1.254, 1.192, 1.316};
  else if(eta > 1.7) return {1.208, 1.162, 1.254};
  else if(eta > 1.1) return {1.121, 1.092, 1.15};
  else if(eta > .5) return {1.099, 1.071, 1.127};
  else return {1.079, 1.053, 1.105};
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetAnalyzer);
