import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

import os
import sys

relBase = os.environ['CMSSW_BASE']

#---- sys.argv takes the parameters given as input cmsRun PhysObjectExtractor/python/poet_cfg.py <isData (default=False)> <doPat (default=False)>
#----  e.g: cmsRun PhysObjectExtractor/python/poet_cfg.py True True
#---- NB the first two parameters are always "cmsRun" and the config file name
#---- Work with data (if False, assumed MC simulations)
#---- This needs to be in agreement with the input files/datasets below.
if len(sys.argv) > 2:
    isData = eval(sys.argv[2])
else:
    isData = False

#---- Flag for using the Physics Analysis Toolkit for jets and MET
if len(sys.argv) > 3:
    doPat = eval(sys.argv[3])
else:
    doPat = False


process = cms.Process("POET")

#---- Configure the framework messaging system
#---- https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMessageLogger
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = "WARNING"
process.MessageLogger.categories.append("POET")
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    limit=cms.untracked.int32(-1))
process.options = cms.untracked.PSet(wantSummary=cms.untracked.bool(True))

#---- Select the maximum number of events to process (if -1, run over all events)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(200) )

#---- Load needed configuration
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

#---- Define the test source files to be read using the xrootd protocol (root://), or local files (file:)
#---- Several files can be comma-separated
#---- A local file, for testing, can be downloaded using, e.g., the cern open data client (https://cernopendata-client.readthedocs.io/en/latest/):
#---- python cernopendata-client download-files --recid 6004 --filter-range 1-1
#---- For running over larger number of files, comment out this section and use/uncomment the FileUtils infrastructure below
if isData:
	sourceFile='root://eospublic.cern.ch//eos/opendata/cms/Run2012B/DoublePhoton/AOD/22Jan2013-v1/20000/0013EBD3-FA6C-E211-A1DF-00261894384A.root'
else:
	sourceFile='root://eospublic.cern.ch//eos/opendata/cms/MonteCarlo2012/Summer12_DR53X/TTbar_8TeV-Madspin_aMCatNLO-herwig/AODSIM/PU_S10_START53_V19-v2/00000/000A9D3F-CE4C-E311-84F8-001E673969D2.root'
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:recoLW5.root'
	sourceFile
    )
)

#---- Alternatively, to run on larger scale, one could use index files as obtained from the Cern Open Data Portal
#---- and pass them into the PoolSource.  The example is for 2012 data
#files = FileUtils.loadListFromFile("data/CMS_Run2012B_DoubleMuParked_AOD_22Jan2013-v1_10000_file_index.txt")
#files.extend(FileUtils.loadListFromFile("data/CMS_Run2012B_DoubleMuParked_AOD_22Jan2013-v1_20000_file_index.txt"))
#files.extend(FileUtils.loadListFromFile("data/CMS_Run2012B_DoubleMuParked_AOD_22Jan2013-v1_20001_file_index.txt"))
#files.extend(FileUtils.loadListFromFile("data/CMS_Run2012B_DoubleMuParked_AOD_22Jan2013-v1_20002_file_index.txt"))
#files.extend(FileUtils.loadListFromFile("data/CMS_Run2012B_DoubleMuParked_AOD_22Jan2013-v1_210000_file_index.txt"))
#files.extend(FileUtils.loadListFromFile("data/CMS_Run2012B_DoubleMuParked_AOD_22Jan2013-v1_30000_file_index.txt"))
#files.extend(FileUtils.loadListFromFile("data/CMS_Run2012B_DoubleMuParked_AOD_22Jan2013-v1_310000_file_index.txt"))
#process.source = cms.Source(
#    "PoolSource", fileNames=cms.untracked.vstring(*files))



#---- These two lines are needed if you require access to the conditions database. E.g., to get jet energy corrections, trigger prescales, etc.
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.Services_cff')
#---- Uncomment and arrange a line like this if you are getting access to the conditions database through CVMFS snapshot files (requires installing CVMFS client)
#process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/FT53_V21A_AN6_FULL.db')
#---- The global tag must correspond to the needed epoch (comment out if no conditions needed)
if isData: process.GlobalTag.globaltag = 'FT53_V21A_AN6::All'
else: process.GlobalTag.globaltag = "START53_V27::All"
#---- If the container has local DB files available, uncomment lines like the ones below
#---- instead of the corresponding lines above
#if isData: process.GlobalTag.connect = cms.string('sqlite_file:/opt/cms-opendata-conddb/FT53_V21A_AN6_FULL_data_stripped.db')
#else:  process.GlobalTag.connect = cms.string('sqlite_file:/opt/cms-opendata-conddb/START53_V27_MC_stripped.db')
#if isData: process.GlobalTag.globaltag = 'FT53_V21A_AN6_FULL::All'
#else: process.GlobalTag.globaltag = "START53_V27::All"


#---- Apply the data quality JSON file filter. This example is for 2012 data
#---- It needs to be done after the process.source definition
#---- Make sure the location of the file agrees with your setup
if isData:
	goodJSON = "data/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt"
	myLumis = LumiList.LumiList(filename=goodJSON).getCMSSWString().split(",")
	process.source.lumisToProcess = CfgTypes.untracked(
	    	CfgTypes.VLuminosityBlockRange())
	process.source.lumisToProcess.extend(myLumis)


#---- Configure the PhysObjectExtractor modules!

#---- More information about InputCollections at https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideRecoDataTable
process.myevents = cms.EDAnalyzer('EventAnalyzer')

process.myelectrons = cms.EDAnalyzer('ElectronAnalyzer',
				     InputCollection = cms.InputTag("gsfElectrons"),
                     isData = cms.bool(isData)
				     )

process.mymuons = cms.EDAnalyzer('MuonAnalyzer',
				 InputCollection = cms.InputTag("muons")
				 )

process.myphotons = cms.EDAnalyzer('PhotonAnalyzer',
                                   InputCollection = cms.InputTag("photons")
				   )

#---- Jet correction paths -- these correspond to the Global Tag. **Run jec_cfg.py first to get .txt files!!**
JecString = 'START53_V27_'
if isData: JecString = 'FT53_V21A_AN6_'

#---- Jets are simpler to work with in "Physics Analysis Toolkit" format. See more at https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookPAT
if doPat:
	#---- Load PAT configs and build some light sequences to process jets and MET
	process.load('PhysicsTools.PatAlgos.producersLayer1.jetProducer_cff')
	process.load('PhysicsTools.PatAlgos.producersLayer1.metProducer_cff')
	process.load('PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi')
	process.patCandidates = cms.Sequence(process.makePatJets+process.makePatMETs)
	process.selectedPatCandidates = cms.Sequence(process.selectedPatJets)
	process.patDefaultSequence = cms.Sequence(process.patCandidates * process.selectedPatCandidates)
	process.load('RecoJets.Configuration.RecoPFJets_cff')
	from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection, runBTagging
	from PhysicsTools.PatAlgos.tools.coreTools import runOnData

	#---- Choose which jet correction levels to apply
	jetcorrlabels = ['L1FastJet','L2Relative','L3Absolute']
	if isData:
		#---- For data we need to remove generator-level matching processes
		runOnData(process, ['Jets','METs'], "", None, [])
		jetcorrlabels.append('L2L3Residual')

	#---- Configure the addJetCollection tool
	#---- This process will make corrected jets with b-tagging included, and will make Type1-corrected MET
	process.ak5PFJets.doAreaFastjet = True
	addJetCollection(process,cms.InputTag('ak5PFJets'),
			 'AK5', 'PFCorr',
			 doJTA        = True,
			 doBTagging   = True,
			 jetCorrLabel = ('AK5PF', cms.vstring(jetcorrlabels)),
			 doType1MET   = True,
			 doL1Cleaning = False,
			 doL1Counters = False,
			 doJetID      = True,
			 jetIdLabel   = "ak5",
			 )

	#---- Configure the POET jet analyzer
	#---- Don't forget to run jec_cfg.py to get these .txt files!
	process.myjets= cms.EDAnalyzer('PatJetAnalyzer',
				       InputCollection = cms.InputTag("selectedPatJetsAK5PFCorr"),
				       isData = cms.bool(isData),
				       jecUncName = cms.FileInPath('AcausalPOETAnalyzer/PhysObjectExtractor/JEC/'+JecString+'Uncertainty_AK5PF.txt'),
				       jerResName = cms.FileInPath('AcausalPOETAnalyzer/PhysObjectExtractor/JEC/JetResolutionInputAK5PF.txt')
				       )
else:
	if not isData:
		#---- Get non-PAT access to the jet flavour information
		from PhysicsTools.JetMCAlgos.HadronAndPartonSelector_cfi import selectedHadronsAndPartons
		process.selectedHadronsAndPartons = selectedHadronsAndPartons.clone()
		from PhysicsTools.JetMCAlgos.AK5PFJetsMCFlavourInfos_cfi import ak5JetFlavourInfos
		process.jetFlavourInfosAK5PFJets = ak5JetFlavourInfos.clone()

	#---- Configure the POET jet analyzer
	#---- Don't forget to run jec_cfg.py to get these .txt files!
	process.myjets= cms.EDAnalyzer('JetAnalyzer',
				       InputCollection = cms.InputTag("ak5PFJets"),
				       isData = cms.bool(isData),
				       jecL1Name = cms.FileInPath('AcausalPOETAnalyzer/PhysObjectExtractor/JEC/'+JecString+'L1FastJet_AK5PF.txt'),
				       jecL2Name = cms.FileInPath('AcausalPOETAnalyzer/PhysObjectExtractor/JEC/'+JecString+'L2Relative_AK5PF.txt'),
				       jecL3Name = cms.FileInPath('AcausalPOETAnalyzer/PhysObjectExtractor/JEC/'+JecString+'L3Absolute_AK5PF.txt'),
				       jecResName = cms.FileInPath('AcausalPOETAnalyzer/PhysObjectExtractor/JEC/'+JecString+'L2L3Residual_AK5PF.txt'),
				       jecUncName = cms.FileInPath('AcausalPOETAnalyzer/PhysObjectExtractor/JEC/'+JecString+'Uncertainty_AK5PF.txt'),
				       jerResName = cms.FileInPath('AcausalPOETAnalyzer/PhysObjectExtractor/JEC/JetResolutionInputAK5PF.txt')
				       )

process.mymets= cms.EDAnalyzer('MetAnalyzer',
                               InputCollection = cms.InputTag("pfMet"),
			       doPat = cms.bool(doPat),
			       )
if doPat: process.mymets.InputCollectionPat = cms.InputTag("pfType1CorrectedMet")

process.mytaus = cms.EDAnalyzer('TauAnalyzer',
                                InputCollection = cms.InputTag("hpsPFTauProducer")
				)

process.mytrigEvent = cms.EDAnalyzer('TriggObjectAnalyzer',
                                     filterName = cms.string("hltL2DoubleMu23NoVertexL2PreFiltered"),
                             )

process.mypvertex = cms.EDAnalyzer('VertexAnalyzer')

process.mytracks= cms.EDAnalyzer('TrackAnalyzer')

process.mygenparticle= cms.EDAnalyzer('GenParticleAnalyzer',
				      #---- Collect particles with specific "status:pdgid"
                                      #---- Check PDG ID in the PDG.
				      #---- if 0:0, collect them all
				      #---- if 0:pdg, collect particle for all status
				      input_particle = cms.vstring("1:11","0:23","1:-11","2:556","2:-556","0:24","0:-24"),
				      trackingTruth = cms.untracked.InputTag('mergedtruth', 'MergedTrackTruth')
				      )

process.mytriggers = cms.EDAnalyzer('TriggerAnalyzer',
                              processName = cms.string("HLT"),
                              #---- These are example triggers for 2012
                              #---- Wildcards * and ? are accepted (with usual meanings)
                               #---- If left empty, all triggers will run
                              triggerPatterns = cms.vstring("HLT_Photon36_Photon22_v*", "HLT_Photon26_Photon18_v*", "HLT_Photon36_CaloId10_Iso50_Photon22_CaloId10_Iso50_v*", "HLT_Photon36_CaloId10_Iso50_Photon22_R9Id85_v*",  "HLT_Photon36_R9Id85_Photon22_CaloId10_Iso50_v*", "HLT_Photon36_R9Id85_Photon22_R9Id85_v*" ),
                              triggerResults = cms.InputTag("TriggerResults","","HLT"),
                              triggerEvent   = cms.InputTag("hltTriggerSummaryAOD","","HLT")
                              )


#---- Example of a CMSSW filter that can be used to cut on a given set of triggers
#---- This filter, however, does know about prescales
#---- A previous trigger study would be needed to cut hard on a given trigger or set of triggers
#---- The filter can be added to the path below if needed but is not applied by default
process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
process.hltHighLevel.HLTPaths = cms.vstring("HLT_Photon36_CaloId10_Iso50_Photon22_CaloId10_Iso50_v*", "HLT_Photon36_CaloId10_Iso50_Photon22_R9Id85_v*",  "HLT_Photon36_R9Id85_Photon22_CaloId10_Iso50_v*", "HLT_Photon36_R9Id85_Photon22_R9Id85_v*")




#---- Example of a very basic home-made filter to select only events of interest
#---- The filter can be added to the running path below if needed but is not applied by default
#process.mutaufilter = cms.EDFilter('SimpleMuTauFilter',
#                                  InputCollectionMuons = cms.InputTag("muons"),
#                                   InputCollectionTaus = cms.InputTag("hpsPFTauProducer"),
#                                   mu_minpt = cms.double(17),
#                                   mu_etacut = cms.double(2.1),
#                                   tau_minpt = cms.double(20),
#                                   tau_etacut = cms.double(2.3)
#                                   )
process.elefilter = cms.EDFilter('SimpleEleFilter',
                                  InputCollectionElectrons = cms.InputTag("gsfElectrons"),
				  #InputCollectionTracks = cms.InputTag("generalTracks"),
                                   ele_minpt = cms.double(15),
				   ele_num = cms.int32(2),
                             	   jet_minpt = cms.double(10),
                                   jet_num = cms.int32(4)
				   #trk_minpt = cms.double(-1),
				   #trk_num = cms.double(-1)
                                   #ele_etacut = cms.double(2.1)
                                   )




#---- Configure the output ROOT filename
process.TFileService = cms.Service(
	"TFileService", fileName=cms.string("myoutput.root"))

#---- Finally run everything!
#---- Separation by * implies that processing order is important.
#---- separation by + implies that any order will work
#---- One can put in or take out the needed processes
if doPat:
	process.p = cms.Path(process.patDefaultSequence+process.myevents+process.myelectrons+process.mymuons+process.myphotons+process.myjets+process.mymets+process.mytaus+process.mytrigEvent+process.mypvertex+process.mytracks+process.mygenparticle+process.mytriggers)
else:
	if isData: process.p = cms.Path(process.hltHighLevel + process.elefilter + process.myevents+process.myelectrons + process.mymuons+process.myphotons+process.myjets + process.mymets+process.mytaus+process.mytrigEvent+process.mypvertex+process.mytracks+process.mygenparticle+process.mytriggers)
        else: process.p = cms.Path(process.hltHighLevel + process.elefilter + process.selectedHadronsAndPartons * process.jetFlavourInfosAK5PFJets * process.myevents + process.myelectrons+process.mymuons+process.myphotons+process.myjets+process.mymets + process.mytaus+process.mytrigEvent + process.mypvertex+process.mytracks+process.mygenparticle+process.mytriggers)
