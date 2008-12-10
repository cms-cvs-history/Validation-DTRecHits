import FWCore.ParameterSet.Config as cms

process = cms.Process("CazziMiei")

## General CMS
process.load("Configuration.StandardSequences.FakeConditions_cff")
# process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
# process.GlobalTag.connect = cms.string('frontier://FrontierProd/CMS_COND_21X_DT')
# process.GlobalTag.globaltag = "CRUZET4_V3P::All"
process.load("Configuration.StandardSequences.MagneticField_cff")

# Geometry related
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cff")
process.load("Geometry.DTGeometry.dtGeometry_cfi")
process.load("Geometry.CSCGeometry.cscGeometry_cfi")
process.load("Geometry.DTGeometryBuilder.idealForDigiDtGeometry_cff")

## DT unpacker
process.load("EventFilter.DTRawToDigi.DTFrontierCabling_cfi")
process.load("EventFilter.DTRawToDigi.dtunpacker_cfi")

process.load("CalibMuon.Configuration.DT_FakeConditions_cff")

## DT local Reco
process.load("RecoLocalMuon.Configuration.RecoLocalMuon_cff")
process.dt1DRecHits.dtDigiLabel  = cms.InputTag("simMuonDTDigis")

# ===================================================
#  7) configuration of your analyzer
# ===================================================
# Validation RecHits
process.load("Validation.DTRecHits.DTRecHitQuality_cfi")
# process.rechivalidation.segment2DLabel = cms.untracked.string('dt2DExtendedSegments')
# process.seg2dvalidation.segment2DLabel = cms.untracked.string('dt2DExtendedSegments')
#process.seg2dvalidation.debug = cms.untracked.bool(True)

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

process.options = cms.untracked.PSet(
    #FailPath = cms.untracked.vstring('ProductNotFound'),
    makeTriggerResults = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(True)
)

process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    ),
    destinations = cms.untracked.vstring('cout')
)

process.source = cms.Source("PoolSource",
fileNames = cms.untracked.vstring( 
### SingleMuon Pt1000
# '/store/relval/CMSSW_2_1_9/RelValSingleMuPt1000/GEN-SIM-DIGI-RAW-HLTDEBUG/IDEAL_V9_v2/0000/162D72AC-B185-DD11-9D6E-000423D6AF24.root',
# '/store/relval/CMSSW_2_1_9/RelValSingleMuPt1000/GEN-SIM-DIGI-RAW-HLTDEBUG/IDEAL_V9_v2/0000/46E3E527-AF85-DD11-97BC-000423D98EC4.root',
# '/store/relval/CMSSW_2_1_9/RelValSingleMuPt1000/GEN-SIM-DIGI-RAW-HLTDEBUG/IDEAL_V9_v2/0000/C6D7FAEB-AC85-DD11-8F06-001617E30D40.root',
# '/store/relval/CMSSW_2_1_9/RelValSingleMuPt1000/GEN-SIM-DIGI-RAW-HLTDEBUG/IDEAL_V9_v2/0001/4AC212E4-0487-DD11-9A6D-000423D94A04.root'

### SingleMuon Pt100
        '/store/relval/CMSSW_3_0_0_pre2/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/IDEAL_V9_v2/0001/2862A759-6FB2-DD11-B1A1-001617C3B6DE.root',
        '/store/relval/CMSSW_3_0_0_pre2/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/IDEAL_V9_v2/0001/C23551E3-1CB4-DD11-AC37-001617C3B6FE.root',
        '/store/relval/CMSSW_3_0_0_pre2/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/IDEAL_V9_v2/0001/F45F1117-6FB2-DD11-AA4A-000423D6A6F4.root'



### SingleMuon Pt10
        # '/store/relval/CMSSW_3_0_0_pre2/RelValSingleMuPt10/GEN-SIM-DIGI-RAW-HLTDEBUG/IDEAL_V9_test_SL5_v7/0000/B01D4C37-DBC1-DD11-96ED-003048945652.root'
        )

)

process.muonLocalReco = cms.Sequence(
        process.dtlocalreco_with_2DSegments
        # process.dtlocalreco_with_2DExtendedSegments
        )

process.ana = cms.Sequence(
        process.dtLocalRecoValidation
        #+ process.STAnalyzer
        # process.DTSegAnalyzer +
        # process.DTEffAnalyzer +
        )

process.p = cms.Path(
#            process.muonDTDigis *
            process.muonLocalReco *
            process.ana
            # )
        )

