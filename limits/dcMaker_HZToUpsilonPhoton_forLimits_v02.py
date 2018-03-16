#!/usr/bin/env python

import json
import argparse
import StringIO 
import os

import ROOT

runCombine = True
# runCombine = False

limitsJSON = {}

def getRate(ratesJSON, analysisBranch, selCategory, sample):
	 altAnalysisBranch = analysisBranch.replace("Photon", "").replace("To", "to")
	 if (sample == "bckg"):
	 	return str(ratesJSON[altAnalysisBranch+"_"+selCategory][0]["Final Yield/Count"])
	 if (sample == "pbckg"):
	 	return str(ratesJSON[altAnalysisBranch+"_"+selCategory][-1]["Final Yield/Count"])
	 if (sample == "signal"):
	 	for s in ratesJSON[altAnalysisBranch+"_"+selCategory][0:-1]:
	 		if (analysisBranch == "ZToJPsiPhoton" and s["Analysis Sample"] == "ZToJPsiGamma"):
	 			return str(s["Final Yield/Count"])
	 		if (analysisBranch == "HToJPsiPhoton" and s["Analysis Sample"] == "HToJPsiGamma"):
	 			return str(s["Final Yield/Count"])
	 		if (analysisBranch == "ZToUpsilonPhoton" and s["Analysis Sample"] == "ZToUpsilon1SGamma"):
	 			return str(s["Final Yield/Count"])
	 		if (analysisBranch == "HToUpsilonPhoton" and s["Analysis Sample"] == "HToUpsilonGamma"):
	 			return str(s["Final Yield/Count"])

def getSyst(systsJSON, analysisBranch, selCategory, sample, syst):
	 altAnalysisBranch = analysisBranch.replace("Photon", "").replace("To", "to")
	 if (sample == "pbckg"):
	 	return str(systsJSON[altAnalysisBranch+"_"+selCategory][-1][syst]/100.0+1.0)
	 if (sample == "signal"):
	 	for s in systsJSON[altAnalysisBranch+"_"+selCategory][:-1]:
	 		if (analysisBranch == "ZToJPsiPhoton" and s["Analysis Sample"] == "ZToJPsiGamma"):
	 			return str(s[syst]/100.0+1.0)
	 		if (analysisBranch == "HToJPsiPhoton" and s["Analysis Sample"] == "HToJPsiGamma"):
	 			return str(s[syst]/100.0+1.0)
	 		if (analysisBranch == "ZToUpsilonPhoton" and s["Analysis Sample"] == "ZToUpsilon1SGamma"):
	 			return str(s[syst]/100.0+1.0)
	 		if (analysisBranch == "HToUpsilonPhoton" and s["Analysis Sample"] == "HToUpsilonGamma"):
	 			return str(s[syst]/100.0+1.0)

def getLimits(rootFileName, analysisBranch, selCategory):
	file = ROOT.TFile(rootFileName)
	tree = file.Get('limit')
	lastLimit = -99.0
	for evt in tree:
		lastLimit = evt.limit
	limitsJSON[analysisBranch+"_"+selCategory] = lastLimit


def saveAndRun(analysisBranch, selCategory, datacard, toRun = True):
	os.system("rm *.root")
	with open("outputDatacards_v02/datacard_"+analysisBranch+"_"+selCategory+".txt" , "w") as datacardFile:
		datacardFile.write(datacard)
	print datacard
	if toRun:
		print "\n\n------------> Running Combine: combine -M Asymptotic outputDatacards_v02/datacard_"+analysisBranch+"_"+selCategory+".txt ..."
		# os.system("combine -v 1 -M Asymptotic outputDatacards_v02/datacard_"+analysisBranch+"_"+selCategory+".txt >> outputLimits_v02/combineOutput_Asymptotic_"+analysisBranch+"_"+selCategory+".txt 2>&1")
		os.system("combine -v 1 -M Asymptotic outputDatacards_v02/datacard_"+analysisBranch+"_"+selCategory+".txt >> outputLimits_v02/combineOutput_Asymptotic_"+analysisBranch+"_"+selCategory+".txt 2>&1")
		print "\n\n------------> Combine output:"
		os.system("cat outputLimits_v02/combineOutput_Asymptotic_"+analysisBranch+"_"+selCategory+".txt")
		print ""
		# print "\n\n------------> Running Combine: combine -M HybridNew"
		# os.system("                               combine -v 2 -M HybridNew --frequentist --testStat=LHC outputDatacards_v02/datacard_"+analysisBranch+"_"+selCategory+".txt -H ProfileLikelihood --fork 4 >> outputLimits_v02/combineOutput_HybridNew_"+analysisBranch+"_"+selCategory+".txt 2>&1")
		# os.system("                               combine -v 2 -M HybridNew --frequentist --testStat=LHC outputDatacards_v02/datacard_"+analysisBranch+"_"+selCategory+".txt -T 500 -H ProfileLikelihood --fork 4 >> outputLimits_v02/combineOutput_HybridNew_"+analysisBranch+"_"+selCategory+".txt 2>&1")
		# os.system("                               combine -v 2 -M HybridNew --frequentist --testStat=LHC outputDatacards_v02/datacard_"+analysisBranch+"_"+selCategory+".txt -T 500 -H ProfileLikelihood --fork 4 ")
		# print "\n\n------------> Combine output:"
		# os.system("cat outputLimits_v02/combineOutput_HybridNew_"+analysisBranch+"_"+selCategory+".txt")
		print ""
		os.system("mkdir -p outputLimits_v02/rootFiles ; mv higgsCombineTest.Asymptotic.mH120.root outputLimits_v02/rootFiles/higgsCombineOutput_"+analysisBranch+"_"+selCategory+".root")
		getLimits("outputLimits_v02/rootFiles/higgsCombineOutput_"+analysisBranch+"_"+selCategory+".root", analysisBranch, selCategory)



 

singleCategoryTemplate = """imax 1
jmax *
ikmax *
-------------------------------------------------------------------------------------------------------
shapes signal     Cat0      @@WS_DIR@@/@@ANA_BRANCH@@SignalAndBackgroundFit/@@ANA_BRANCH@@SignalAndBackgroundFit_workspace_Cat0.root w:@@SIGNAL_MODEL@@
shapes bckg       Cat0      @@WS_DIR@@/ftestOutput/CMS-HGG_multipdf_@@ANA_BRANCH@@_Cat0_afterFtest.root                              multipdf:@@BCKG_MODEL@@
shapes pbckg      Cat0      @@WS_DIR@@/@@ANA_BRANCH@@SignalAndBackgroundFit/@@ANA_BRANCH@@SignalAndBackgroundFit_workspace_Cat0.root w:@@PBCKG_MODEL@@
shapes data_obs   Cat0      @@WS_DIR@@/@@ANA_BRANCH@@SignalAndBackgroundFit/@@ANA_BRANCH@@SignalAndBackgroundFit_workspace_Cat0.root w:data_obs
# shapes data_obs   Cat0      @@WS_DIR@@/ftestOutput/CMS-HGG_multipdf_@@ANA_BRANCH@@_Cat0_afterFtest.root multipdf:roohist_data_mass_UntaggedTag_0
--------------------------------------------------------------------------------------------------------
bin Cat0
observation -1
--------------------------------------------------------------------------------------------------------
bin             Cat0      Cat0    Cat0
process         signal    bckg    pbckg
process         0         1       2
rate            @@RATE_SIGNAL@@  1.0  @@RATE_PBCKG@@
---------------------------------------------------------------------------------------------------------
lumi       lnN   1.025                         -   1.025    
HZ_xs_sc   lnN   @@SYST_HZ_XSEC_SC_SIGNAL@@    -   @@SYST_HZ_XSEC_SC_PBCKG@@
HZ_xs_pdf  lnN   @@SYST_HZ_XSEC_PDF_SIGNAL@@   -   @@SYST_HZ_XSEC_PDF_PBCKG@@
br_peak    lnN   -                             -   @@SYST_BR_PEAK_PBCKG@@
pu_r       lnN   @@SYST_PU_R_SIGNAL@@          -   @@SYST_PU_R_PBCKG@@
trg        lnN   @@SYST_TRG_SIGNAL@@           -   @@SYST_TRG_PBCKG@@
muon_id    lnN   @@SYST_MUON_ID_SIGNAL@@       -   @@SYST_MUON_ID_PBCKG@@
ph_id      lnN   @@SYST_PH_ID_SIGNAL@@         -   @@SYST_PH_ID_PBCKG@@
ele_veto   lnN   @@SYST_ELE_VETO_SIGNAL@@      -   @@SYST_ELE_VETO_PBCKG@@
pdfindex_UntaggedTag_0_13TeV_Cat0   discrete
# CMS_hgg_UntaggedTag_0_13TeV_bkgshape_norm flatparam
#mean_HZ   param 125  1.01
#sigma_cb  param 3    1.0493
"""

multiCategoryTemplate = """imax 3
jmax *
ikmax *
-------------------------------------------------------------------------------------------------------
shapes signal     Cat1      @@WS_DIR@@/@@ANA_BRANCH@@SignalAndBackgroundFit/@@ANA_BRANCH@@SignalAndBackgroundFit_workspace_Cat1.root w:@@SIGNAL_MODEL@@
shapes bckg       Cat1      @@WS_DIR@@/ftestOutput/CMS-HGG_multipdf_@@ANA_BRANCH@@_Cat1_afterFtest.root                              multipdf:@@BCKG_MODEL@@
shapes pbckg      Cat1      @@WS_DIR@@/@@ANA_BRANCH@@SignalAndBackgroundFit/@@ANA_BRANCH@@SignalAndBackgroundFit_workspace_Cat1.root w:@@PBCKG_MODEL@@
shapes data_obs   Cat1      @@WS_DIR@@/@@ANA_BRANCH@@SignalAndBackgroundFit/@@ANA_BRANCH@@SignalAndBackgroundFit_workspace_Cat1.root w:data_obs

shapes signal     Cat2      @@WS_DIR@@/@@ANA_BRANCH@@SignalAndBackgroundFit/@@ANA_BRANCH@@SignalAndBackgroundFit_workspace_Cat2.root w:@@SIGNAL_MODEL@@
shapes bckg       Cat2      @@WS_DIR@@/ftestOutput/CMS-HGG_multipdf_@@ANA_BRANCH@@_Cat2_afterFtest.root                              multipdf:@@BCKG_MODEL@@
shapes pbckg      Cat2      @@WS_DIR@@/@@ANA_BRANCH@@SignalAndBackgroundFit/@@ANA_BRANCH@@SignalAndBackgroundFit_workspace_Cat2.root w:@@PBCKG_MODEL@@
shapes data_obs   Cat2      @@WS_DIR@@/@@ANA_BRANCH@@SignalAndBackgroundFit/@@ANA_BRANCH@@SignalAndBackgroundFit_workspace_Cat2.root w:data_obs

shapes signal     Cat3      @@WS_DIR@@/@@ANA_BRANCH@@SignalAndBackgroundFit/@@ANA_BRANCH@@SignalAndBackgroundFit_workspace_Cat3.root w:@@SIGNAL_MODEL@@
shapes bckg       Cat3      @@WS_DIR@@/ftestOutput/CMS-HGG_multipdf_@@ANA_BRANCH@@_Cat3_afterFtest.root                              multipdf:@@BCKG_MODEL@@
shapes pbckg      Cat3      @@WS_DIR@@/@@ANA_BRANCH@@SignalAndBackgroundFit/@@ANA_BRANCH@@SignalAndBackgroundFit_workspace_Cat3.root w:@@PBCKG_MODEL@@
shapes data_obs   Cat3      @@WS_DIR@@/@@ANA_BRANCH@@SignalAndBackgroundFit/@@ANA_BRANCH@@SignalAndBackgroundFit_workspace_Cat3.root w:data_obs

------------------------------------------------------------------------------------------------------------------
bin               Cat1   Cat2   Cat3
observation       -1     -1     -1
-------------------------------------------------------------------------------------------------------------------
bin               Cat1      Cat1    Cat1   Cat2      Cat2    Cat2   Cat3      Cat3    Cat3
process           signal    bckg    pbckg  signal    bckg    pbckg  signal    bckg    pbckg
process           0         1       2      0         1       2       0        1       2
rate              @@RATE_CAT1_SIGNAL@@  1.0  @@RATE_CAT1_PBCKG@@  @@RATE_CAT2_SIGNAL@@  1.0  @@RATE_CAT2_PBCKG@@   @@RATE_CAT3_SIGNAL@@  1.0  @@RATE_CAT3_PBCKG@@
--------------------------------------------------------------------------------------------------------------------
lumi       lnN    1.025                          -   1.025                         1.025                         -       1.025                          1.025                          -   1.025    
HZ_xs_sc   lnN    @@SYST_HZ_XSEC_SC_SIGNAL@@     -   @@SYST_HZ_XSEC_SC_PBCKG@@     @@SYST_HZ_XSEC_SC_SIGNAL@@    -       @@SYST_HZ_XSEC_SC_PBCKG@@      @@SYST_HZ_XSEC_SC_SIGNAL@@     -   @@SYST_HZ_XSEC_SC_PBCKG@@   
HZ_xs_pdf  lnN    @@SYST_HZ_XSEC_PDF_SIGNAL@@    -   @@SYST_HZ_XSEC_PDF_PBCKG@@    @@SYST_HZ_XSEC_PDF_SIGNAL@@   -       @@SYST_HZ_XSEC_PDF_PBCKG@@     @@SYST_HZ_XSEC_PDF_SIGNAL@@    -   @@SYST_HZ_XSEC_PDF_PBCKG@@   
br_peak    lnN    -                              -   @@SYST_BR_PEAK_PBCKG@@        -                             -       @@SYST_BR_PEAK_PBCKG@@         -                              -   @@SYST_BR_PEAK_PBCKG@@   
pu_r       lnN    @@SYST_PU_R_CAT1_SIGNAL@@      -   @@SYST_PU_R_CAT1_PBCKG@@      @@SYST_PU_R_CAT2_SIGNAL@@     -       @@SYST_PU_R_CAT2_PBCKG@@       @@SYST_PU_R_CAT3_SIGNAL@@      -   @@SYST_PU_R_CAT3_PBCKG@@   
trg        lnN    @@SYST_TRG_CAT1_SIGNAL@@       -   @@SYST_TRG_CAT1_PBCKG@@       @@SYST_TRG_CAT2_SIGNAL@@      -       @@SYST_TRG_CAT2_PBCKG@@        @@SYST_TRG_CAT3_SIGNAL@@       -   @@SYST_TRG_CAT3_PBCKG@@   
muon_id    lnN    @@SYST_MUON_ID_CAT1_SIGNAL@@   -   @@SYST_MUON_ID_CAT1_PBCKG@@   @@SYST_MUON_ID_CAT2_SIGNAL@@  -       @@SYST_MUON_ID_CAT2_PBCKG@@    @@SYST_MUON_ID_CAT3_SIGNAL@@   -   @@SYST_MUON_ID_CAT3_PBCKG@@   
ph_id      lnN    @@SYST_PH_ID_CAT1_SIGNAL@@     -   @@SYST_PH_ID_CAT1_PBCKG@@     @@SYST_PH_ID_CAT2_SIGNAL@@    -       @@SYST_PH_ID_CAT2_PBCKG@@      @@SYST_PH_ID_CAT3_SIGNAL@@     -   @@SYST_PH_ID_CAT3_PBCKG@@   
ele_veto   lnN    @@SYST_ELE_VETO_CAT1_SIGNAL@@  -   @@SYST_ELE_VETO_CAT1_PBCKG@@  @@SYST_ELE_VETO_CAT2_SIGNAL@@ -       @@SYST_ELE_VETO_CAT2_PBCKG@@   @@SYST_ELE_VETO_CAT3_SIGNAL@@  -   @@SYST_ELE_VETO_CAT3_PBCKG@@   
pdfindex_UntaggedTag_0_13TeV_Cat1   discrete
pdfindex_UntaggedTag_0_13TeV_Cat2   discrete
pdfindex_UntaggedTag_0_13TeV_Cat3   discrete
# CMS_hgg_UntaggedTag_0_13TeV_bkgshape_norm flatparam
#mean_HZ   param  125   1.01
#sigma_cb  param  3     1.0493
"""


def makeDatacard(analysisBranch, selCategory):
	print "="*50
	print "-- Datacard: " + analysisBranch + " - " + selCategory + "\n"
		
	###########################################################################
	# Cat0
	if (selCategory == "Cat0"):
		#read JSONs
		with open(ratesFile) as jsonRates:
			with open(systematicsErrorsFile) as jsonSysts:
				ratesJSON = json.load(jsonRates)
				systsJSON = json.load(jsonSysts)
				datacard = singleCategoryTemplate
				# ws
				datacard = datacard.replace("@@WS_DIR@@", wsDir)
				datacard = datacard.replace("@@ANA_BRANCH@@", analysisBranch)
				datacard = datacard.replace("@@ANA_BRANCH@@", analysisBranch)

				if (analysisBranch == "ZToJPsiPhoton" or analysisBranch == "ZToUpsilonPhoton"):
					datacard = datacard.replace("@@SIGNAL_MODEL@@", "dcball")
					datacard = datacard.replace("@@BCKG_MODEL@@", "CMS_hgg_UntaggedTag_0_13TeV_bkgshape")
					datacard = datacard.replace("@@PBCKG_MODEL@@", "dcballPeakingBackground")
				if (analysisBranch == "HToJPsiPhoton" or analysisBranch == "HToUpsilonPhoton"):
					datacard = datacard.replace("@@SIGNAL_MODEL@@", "signal_model")
					datacard = datacard.replace("@@BCKG_MODEL@@", "CMS_hgg_UntaggedTag_0_13TeV_bkgshape")
					datacard = datacard.replace("@@PBCKG_MODEL@@", "PeakingBackground_cb")

				# rates
				datacard = datacard.replace("@@RATE_SIGNAL@@", getRate(ratesJSON, analysisBranch, selCategory, "signal"))
				datacard = datacard.replace("@@RATE_BCKG@@", getRate(ratesJSON, analysisBranch, selCategory, "bckg"))
				datacard = datacard.replace("@@RATE_PBCKG@@", getRate(ratesJSON, analysisBranch, selCategory, "pbckg"))
				datacard = datacard.replace("@@SYST_HZ_XSEC_SIGNAL@@", getRate(ratesJSON, analysisBranch, selCategory, "pbckg"))
				# syst
				#HZ_xs_sc
				if (analysisBranch == "ZToJPsiPhoton" or analysisBranch == "ZToUpsilonPhoton"):
					datacard = datacard.replace("@@SYST_HZ_XSEC_SC_SIGNAL@@", "1.035")
					datacard = datacard.replace("@@SYST_HZ_XSEC_SC_PBCKG@@", "1.050")
				if (analysisBranch == "HToJPsiPhoton" or analysisBranch == "HToUpsilonPhoton"):
					datacard = datacard.replace("@@SYST_HZ_XSEC_SC_SIGNAL@@", "0.933/1.046")
					datacard = datacard.replace("@@SYST_HZ_XSEC_SC_PBCKG@@", "0.933/1.046")
				# HZ_xs_pdf
				if (analysisBranch == "ZToJPsiPhoton" or analysisBranch == "ZToUpsilonPhoton"):
					datacard = datacard.replace("@@SYST_HZ_XSEC_PDF_SIGNAL@@", "1.073")
					datacard = datacard.replace("@@SYST_HZ_XSEC_PDF_PBCKG@@", "1.050")
				if (analysisBranch == "HToJPsiPhoton" or analysisBranch == "HToUpsilonPhoton"):
					datacard = datacard.replace("@@SYST_HZ_XSEC_PDF_SIGNAL@@", "1.032")
					datacard = datacard.replace("@@SYST_HZ_XSEC_PDF_PBCKG@@", "1.032")
				# br_peak
				if (analysisBranch == "ZToJPsiPhoton" or analysisBranch == "ZToUpsilonPhoton"):
					newDatacard = ""
					for line in StringIO.StringIO(datacard).readlines():
						if not(line.startswith("br_peak")):
							newDatacard += line
					datacard = newDatacard
				if (analysisBranch == "HToJPsiPhoton" or analysisBranch == "HToUpsilonPhoton"):
					datacard = datacard.replace("@@SYST_BR_PEAK_PBCKG@@", "1.060")
				# pu_r
				datacard = datacard.replace("@@SYST_PU_R_SIGNAL@@", getSyst(systsJSON, analysisBranch, selCategory, "signal", "Pileup"))
				datacard = datacard.replace("@@SYST_PU_R_PBCKG@@", getSyst(systsJSON, analysisBranch, selCategory, "pbckg", "Pileup"))
				# trg
				datacard = datacard.replace("@@SYST_TRG_SIGNAL@@", getSyst(systsJSON, analysisBranch, selCategory, "signal", "Trigger"))
				datacard = datacard.replace("@@SYST_TRG_PBCKG@@", getSyst(systsJSON, analysisBranch, selCategory, "pbckg", "Trigger"))
				# muon_id
				datacard = datacard.replace("@@SYST_MUON_ID_SIGNAL@@", getSyst(systsJSON, analysisBranch, selCategory, "signal", "Muon ID"))
				datacard = datacard.replace("@@SYST_MUON_ID_PBCKG@@", getSyst(systsJSON, analysisBranch, selCategory, "pbckg", "Muon ID"))
				# ph_id
				datacard = datacard.replace("@@SYST_PH_ID_SIGNAL@@", getSyst(systsJSON, analysisBranch, selCategory, "signal", "Photon ID"))
				datacard = datacard.replace("@@SYST_PH_ID_PBCKG@@", getSyst(systsJSON, analysisBranch, selCategory, "pbckg", "Photon ID"))
				# ele_veto
				datacard = datacard.replace("@@SYST_ELE_VETO_SIGNAL@@", getSyst(systsJSON, analysisBranch, selCategory, "signal", "Electron Veto"))
				datacard = datacard.replace("@@SYST_ELE_VETO_PBCKG@@", getSyst(systsJSON, analysisBranch, selCategory, "pbckg", "Electron Veto"))

				# saveAndRun(analysisBranch, selCategory, datacard)
				saveAndRun(analysisBranch, selCategory, datacard, runCombine)
 
	###########################################################################
	# Cat123
	if (selCategory == "Cat123"):
		#read JSONs
		with open(ratesFile) as jsonRates:
			with open(systematicsErrorsFile) as jsonSysts:
				ratesJSON = json.load(jsonRates)
				systsJSON = json.load(jsonSysts)
				datacard = multiCategoryTemplate
				# ws
				datacard = datacard.replace("@@WS_DIR@@", wsDir)
				datacard = datacard.replace("@@ANA_BRANCH@@", analysisBranch)
				datacard = datacard.replace("@@ANA_BRANCH@@", analysisBranch)

				if (analysisBranch == "ZToJPsiPhoton" or analysisBranch == "ZToUpsilonPhoton"):
					datacard = datacard.replace("@@SIGNAL_MODEL@@", "dcball")
					datacard = datacard.replace("@@BCKG_MODEL@@", "CMS_hgg_UntaggedTag_0_13TeV_bkgshape")
					datacard = datacard.replace("@@PBCKG_MODEL@@", "dcballPeakingBackground")
				if (analysisBranch == "HToJPsiPhoton" or analysisBranch == "HToUpsilonPhoton"):
					datacard = datacard.replace("@@SIGNAL_MODEL@@", "signal_model")
					datacard = datacard.replace("@@BCKG_MODEL@@", "CMS_hgg_UntaggedTag_0_13TeV_bkgshape")
					datacard = datacard.replace("@@PBCKG_MODEL@@", "PeakingBackground_cb")
				# syst - HZ_xs_sc
				if (analysisBranch == "ZToJPsiPhoton" or analysisBranch == "ZToUpsilonPhoton"):
					datacard = datacard.replace("@@SYST_HZ_XSEC_SC_SIGNAL@@", "1.035")
					datacard = datacard.replace("@@SYST_HZ_XSEC_SC_PBCKG@@", "1.050")
				if (analysisBranch == "HToJPsiPhoton" or analysisBranch == "HToUpsilonPhoton"):
					datacard = datacard.replace("@@SYST_HZ_XSEC_SC_SIGNAL@@", "0.933/1.046")
					datacard = datacard.replace("@@SYST_HZ_XSEC_SC_PBCKG@@", "0.933/1.046")
				# syst - HZ_xs_pdf
				if (analysisBranch == "ZToJPsiPhoton" or analysisBranch == "ZToUpsilonPhoton"):
					datacard = datacard.replace("@@SYST_HZ_XSEC_PDF_SIGNAL@@", "1.073")
					datacard = datacard.replace("@@SYST_HZ_XSEC_PDF_PBCKG@@", "1.050")
				if (analysisBranch == "HToJPsiPhoton" or analysisBranch == "HToUpsilonPhoton"):
					datacard = datacard.replace("@@SYST_HZ_XSEC_PDF_SIGNAL@@", "1.032")
					datacard = datacard.replace("@@SYST_HZ_XSEC_PDF_PBCKG@@", "1.032")
				# syst - br_peak
				if (analysisBranch == "ZToJPsiPhoton" or analysisBranch == "ZToUpsilonPhoton"):
					newDatacard = ""
					for line in StringIO.StringIO(datacard).readlines():
						if not(line.startswith("br_peak")):
							newDatacard += line
					datacard = newDatacard
				if (analysisBranch == "HToJPsiPhoton" or analysisBranch == "HToUpsilonPhoton"):
					datacard = datacard.replace("@@SYST_BR_PEAK_PBCKG@@", "1.060")

				# rates
				for cat in ["Cat1", "Cat2", "Cat3"]:
					# rates
					datacard = datacard.replace("@@RATE_"+cat.upper()+"_SIGNAL@@", getRate(ratesJSON, analysisBranch, cat, "signal"))
					datacard = datacard.replace("@@RATE_"+cat.upper()+"_BCKG@@", getRate(ratesJSON, analysisBranch, cat, "bckg"))
					datacard = datacard.replace("@@RATE_"+cat.upper()+"_PBCKG@@", getRate(ratesJSON, analysisBranch, cat, "pbckg"))
					datacard = datacard.replace("@@SYST_HZ_XSEC_"+cat.upper()+"_SIGNAL@@", getRate(ratesJSON, analysisBranch, cat, "pbckg"))
					# syst - pu_r
					datacard = datacard.replace("@@SYST_PU_R_"+cat.upper()+"_SIGNAL@@", getSyst(systsJSON, analysisBranch, cat, "signal", "Pileup"))
					datacard = datacard.replace("@@SYST_PU_R_"+cat.upper()+"_PBCKG@@", getSyst(systsJSON, analysisBranch, cat, "pbckg", "Pileup"))
					# syst - trg
					datacard = datacard.replace("@@SYST_TRG_"+cat.upper()+"_SIGNAL@@", getSyst(systsJSON, analysisBranch, cat, "signal", "Trigger"))
					datacard = datacard.replace("@@SYST_TRG_"+cat.upper()+"_PBCKG@@", getSyst(systsJSON, analysisBranch, cat, "pbckg", "Trigger"))
					# syst - muon_id
					datacard = datacard.replace("@@SYST_MUON_ID_"+cat.upper()+"_SIGNAL@@", getSyst(systsJSON, analysisBranch, cat, "signal", "Muon ID"))
					datacard = datacard.replace("@@SYST_MUON_ID_"+cat.upper()+"_PBCKG@@", getSyst(systsJSON, analysisBranch, cat, "pbckg", "Muon ID"))
					# syst - ph_id
					datacard = datacard.replace("@@SYST_PH_ID_"+cat.upper()+"_SIGNAL@@", getSyst(systsJSON, analysisBranch, cat, "signal", "Photon ID"))
					datacard = datacard.replace("@@SYST_PH_ID_"+cat.upper()+"_PBCKG@@", getSyst(systsJSON, analysisBranch, cat, "pbckg", "Photon ID"))
					# syst - ele_veto
					datacard = datacard.replace("@@SYST_ELE_VETO_"+cat.upper()+"_SIGNAL@@", getSyst(systsJSON, analysisBranch, cat, "signal", "Electron Veto"))
					datacard = datacard.replace("@@SYST_ELE_VETO_"+cat.upper()+"_PBCKG@@", getSyst(systsJSON, analysisBranch, cat, "pbckg", "Electron Veto"))
					
				# saveAndRun(analysisBranch, selCategory, datacard)
				saveAndRun(analysisBranch, selCategory, datacard, runCombine)

if __name__ == "__main__":
	#############################################################################################
	# C O N F I G
	ratesFile = "inputData/evtsCountFiles/FinalYields.json"
	systematicsErrorsFile = "inputData/evtsCountFiles/systErrorYields.json"
	wsDir = "inputData/fitPlotFiles"
	#############################################################################################

	print "\n-----> Datacard Maker - H/Z --> Upsilon + Photon" 
	print """  ___.      _                       _   __  __      _.            
 |   \ __ _| |_ __ _ __ __ _ _ _ __| | |  \/  |__ _| |_____ _ _ 
 | |) / _` |  _/ _` / _/ _` | '_/ _` | | |\/| / _` | / / -_) '_|
 |___/\__,_|\__\__,_\__\__,_|_| \__,_| |_|  |_\__,_|_\_\___|_|  
																"""
	os.system("rm -rf outputDatacards_v02/*")
	os.system("rm -rf outputLimits_v02/*")
	makeDatacard("ZToJPsiPhoton", "Cat0")
	makeDatacard("ZToJPsiPhoton", "Cat123")
	makeDatacard("HToJPsiPhoton", "Cat0")
	makeDatacard("ZToUpsilonPhoton", "Cat0")
	makeDatacard("ZToUpsilonPhoton", "Cat123")
	makeDatacard("HToUpsilonPhoton", "Cat0")

	# dump limits
	jsondata = json.dumps(limitsJSON, sort_keys=True, indent=2, separators=(',', ': '))
	os.system("rm outputLimits_v02/limits.json")
	with open("outputLimits_v02/limits.json", 'w') as out_file:
		out_file.write(jsondata)
	print "\n"+jsondata+"\n"







