#!/usr/bin/env python

import json
import argparse
import StringIO 
import os

import ROOT



limitsJSON = {}


def getRate(ratesJSON, analysisBranch, quarkoniaState, selCategory, sample):
	 altAnalysisBranch = analysisBranch.replace("Photon", "").replace("To", "to")
	 if (sample == "bckg"):
	 	return str(ratesJSON[altAnalysisBranch+"_"+selCategory][0]["Final Yield/Count"])
	 if (sample == "pbckg"):
	 	return str(ratesJSON[altAnalysisBranch+"_"+selCategory][-1]["Final Yield/Count"])
	 if (sample == "signal"):
	 	for s in ratesJSON[altAnalysisBranch+"_"+selCategory][0:-1]:
	 		if (s["Analysis Sample"] == altAnalysisBranch.replace("to", "To")+quarkoniaState+"Gamma"):
	 			return str(s["Final Yield/Count"])


def getSyst(systsJSON, analysisBranch, quarkoniaState, selCategory, sample, syst):
	 altAnalysisBranch = analysisBranch.replace("Photon", "").replace("To", "to")
	 if (sample == "pbckg"):
	 	return str(systsJSON[altAnalysisBranch+"_"+selCategory][-1][syst]/100.0+1.0)
	 if (sample == "signal"):
	 	for s in systsJSON[altAnalysisBranch+"_"+selCategory][:-1]:
	 		if (s["Analysis Sample"] == altAnalysisBranch.replace("to", "To")+quarkoniaState+"Gamma"):
	 			return str(s[syst]/100.0+1.0)

def getLimits(rootFileName, analysisBranch, quarkoniaState, selCategory):
	file = ROOT.TFile(rootFileName)
	tree = file.Get('limit')
	tree.Scan()
	observedLimit = -99.0
	expectedLimit = -99.0
	expectedLimitMinus1Sigma = -99.0
	expectedLimitPlus1Sigma = -99.0
	limitsJSON[analysisBranch[0:-6]+quarkoniaState+"Photon_"+selCategory] = {}
	index = 0
	if (tree.GetEntries() > 2):
		for evt in tree:
			if (index == 5):
				observedLimit = evt.limit
			if (index == 2):
				expectedLimit = evt.limit
			if (index == 1):
				expectedLimitMinus1Sigma = evt.limit
			if (index == 3):
				expectedLimitPlus1Sigma = evt.limit
			index += 1
			limitsJSON[analysisBranch[0:-6]+quarkoniaState+"Photon_"+selCategory]["observedLimit"] = observedLimit
			limitsJSON[analysisBranch[0:-6]+quarkoniaState+"Photon_"+selCategory]["expectedLimit"] = expectedLimit
			limitsJSON[analysisBranch[0:-6]+quarkoniaState+"Photon_"+selCategory]["expectedLimitMinus1Sigma"] = expectedLimitMinus1Sigma
			limitsJSON[analysisBranch[0:-6]+quarkoniaState+"Photon_"+selCategory]["expectedLimitPlus1Sigma"] = expectedLimitPlus1Sigma
	else : # to be removed as soon as we are able to get the correct expected value from COMBINE
		for evt in tree:
			observedLimit = evt.limit
			expectedLimit = evt.limit
			expectedLimitMinus1Sigma = evt.limit
			expectedLimitPlus1Sigma = evt.limit
			index += 1
			limitsJSON[analysisBranch[0:-6]+quarkoniaState+"Photon_"+selCategory]["observedLimit"] = observedLimit
			limitsJSON[analysisBranch[0:-6]+quarkoniaState+"Photon_"+selCategory]["expectedLimit"] = expectedLimit
			limitsJSON[analysisBranch[0:-6]+quarkoniaState+"Photon_"+selCategory]["expectedLimitMinus1Sigma"] = expectedLimitMinus1Sigma
			limitsJSON[analysisBranch[0:-6]+quarkoniaState+"Photon_"+selCategory]["expectedLimitPlus1Sigma"] = expectedLimitPlus1Sigma


def saveDatacard(analysisBranch, quarkoniaState, selCategory, datacard):
	os.system("rm *.root")
	with open("outputDatacards2D/datacard_"+analysisBranch[0:-6]+quarkoniaState+"Photon"+"_"+selCategory+".txt" , "w") as datacardFile:
		datacardFile.write(datacard)
	print datacard

def runDatacard(analysisBranch, quarkoniaState, selCategory, toRun = True):
	if (selCategory == "Cat123"):
		os.system("combineCards.py Cat1=outputDatacards2D/datacard_"+analysisBranch[0:-6]+quarkoniaState+"Photon"+"_Cat1.txt Cat2=outputDatacards2D/datacard_"+analysisBranch[0:-6]+quarkoniaState+"Photon"+"_Cat2.txt Cat3=outputDatacards2D/datacard_"+analysisBranch[0:-6]+quarkoniaState+"Photon"+"_Cat3.txt > outputDatacards2D/datacard_"+analysisBranch[0:-6]+quarkoniaState+"Photon"+"_"+selCategory+".txt")
		with open("outputDatacards2D/datacard_"+analysisBranch[0:-6]+quarkoniaState+"Photon"+"_"+selCategory+".txt", 'r') as fin:
			data = fin.read().replace("outputDatacards2D/inputData", "inputData").splitlines(True)
		with open("outputDatacards2D/datacard_"+analysisBranch[0:-6]+quarkoniaState+"Photon"+"_"+selCategory+".txt", 'w') as fout:
			fout.writelines(data[1:])
	if toRun:
		print "\n\n------------> Running Combine: combine -M Asymptotic outputDatacards2D/datacard_"+analysisBranch[0:-6]+quarkoniaState+"Photon"+"_"+selCategory+".txt ..."
		os.system("combine -v 1 -M Asymptotic outputDatacards2D/datacard_"+analysisBranch[0:-6]+quarkoniaState+"Photon"+"_"+selCategory+".txt  --rAbsAcc=0.001 --rRelAcc=0.0005 --rMax=10000000 --rMin=0 >> outputLimits2D/combineOutput_Asymptotic_"+analysisBranch[0:-6]+quarkoniaState+"Photon"+"_"+selCategory+".txt 2>&1")
		print "\n\n------------> Combine output:"
		os.system("cat outputLimits2D/combineOutput_Asymptotic_"+analysisBranch[0:-6]+quarkoniaState+"Photon"+"_"+selCategory+".txt")
		print ""
		# print "\n\n------------> Running Combine: combine -M HybridNew"
		# os.system("                               combine -v 1 -M HybridNew --frequentist --testStat=LHC outputDatacards2D/datacard_"+analysisBranch+"_"+selCategory+".txt -H ProfileLikelihood --fork 4 >> outputLimits2D/combineOutput_HybridNew_"+analysisBranch+"_"+selCategory+".txt 2>&1")
		# print "\n\n------------> Combine output:"
		# os.system("cat outputLimits2D/combineOutput_HybridNew_"+analysisBranch[0:-6]+quarkoniaState+"Photon"+"_"+selCategory+".txt")
		print ""
		os.system("mkdir -p outputLimits2D/rootFiles ; mv higgsCombineTest.Asymptotic.mH120.root outputLimits2D/rootFiles/higgsCombineOutput_"+analysisBranch[0:-6]+quarkoniaState+"Photon"+"_"+selCategory+".root")
		getLimits("outputLimits2D/rootFiles/higgsCombineOutput_"+analysisBranch[0:-6]+quarkoniaState+"Photon"+"_"+selCategory+".root", analysisBranch, quarkoniaState, selCategory)




singleCategoryTemplate_top = """imax 1
jmax *
ikmax *
-------------------------------------------------------------------------------------------------------
shapes signal     @@CAT@@      @@WS_DIR@@/@@ANA_BRANCH@@PhotonSignalAndBackgroundFit/@@ANA_BRANCH@@@@QUARKONIA_STATE@@PhotonSignalAndBackgroundFit_workspace_@@CAT@@.root w:@@SIGNAL_MODEL@@
shapes bckg       @@CAT@@      @@WS_DIR@@/ftestOutput2D/CMS-HGG_multipdf_@@ANA_BRANCH@@Photon_@@CAT@@_afterFtest.root                              multipdf:@@BCKG_MODEL@@
@@IS_Z@@shapes pbckg      @@CAT@@      @@WS_DIR@@/@@ANA_BRANCH@@PhotonSignalAndBackgroundFit/@@ANA_BRANCH@@@@QUARKONIA_STATE@@PhotonSignalAndBackgroundFit_workspace_@@CAT@@.root w:@@PBCKG_MODEL@@
shapes data_obs   @@CAT@@      @@WS_DIR@@/@@ANA_BRANCH@@PhotonSignalAndBackgroundFit/@@ANA_BRANCH@@@@QUARKONIA_STATE@@PhotonSignalAndBackgroundFit_workspace_@@CAT@@.root w:data_obs
--------------------------------------------------------------------------------------------------------
bin @@CAT@@
observation -1
--------------------------------------------------------------------------------------------------------
"""

binAndProcess_Z = """bin             @@CAT@@      @@CAT@@    
process         signal    bckg   
process         0         1       
rate            @@RATE_SIGNAL@@  @@RATE_BCKG@@ 
---------------------------------------------------------------------------------------------------------
lumi       lnN   1.025                         -       
HZ_xs_sc   lnN   @@SYST_HZ_XSEC_SC_SIGNAL@@    -   
HZ_xs_pdf  lnN   @@SYST_HZ_XSEC_PDF_SIGNAL@@   -   
pu_r       lnN   @@SYST_PU_R_SIGNAL@@          -   
trg        lnN   @@SYST_TRG_SIGNAL@@           -   
muon_id    lnN   @@SYST_MUON_ID_SIGNAL@@       -   
ph_id      lnN   @@SYST_PH_ID_SIGNAL@@         -   
ele_veto   lnN   @@SYST_ELE_VETO_SIGNAL@@      -   
pol        lnN   @@SYST_POL_SIGNAL@@           -   
pdfindex_UntaggedTag_0_13TeV_@@CAT@@   discrete
@@MEAN_VAR@@   param @@MEAN_VAL@@  @@MEAN_ERR@@
@@SIGMA_VAR@@  param @@SIGMA_VAL@@  @@SIGMA_ERR@@"""


binAndProcess_H = """bin             @@CAT@@      @@CAT@@    @@CAT@@
process         signal    bckg    pbckg
process         0         1       2
rate            @@RATE_SIGNAL@@  @@RATE_BCKG@@  @@RATE_PBCKG@@
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
pol        lnN   @@SYST_POL_SIGNAL@@           -   -
pdfindex_UntaggedTag_0_13TeV_@@CAT@@   discrete
@@MEAN_VAR@@   param @@MEAN_VAL@@  @@MEAN_ERR@@
@@SIGMA_VAR@@  param @@SIGMA_VAL@@  @@SIGMA_ERR@@"""




def makeDatacard(analysisBranch, quarkoniaState, selCategory):
	print "="*50
	print "-- Datacard: " + analysisBranch + " - " + selCategory + "\n"
	#read JSONs
	with open(ratesFile) as jsonRates:
		with open(systematicsErrorsFile) as jsonSysts:
			ratesJSON = json.load(jsonRates)
			systsJSON = json.load(jsonSysts)
			datacard = singleCategoryTemplate_top
			if (analysisBranch[0] == "Z"): #Z channel
				datacard += binAndProcess_Z 
			else:
				datacard += binAndProcess_H 
			# ws
			datacard = datacard.replace("@@QUARKONIA_STATE@@", quarkoniaState)
			datacard = datacard.replace("@@CAT@@", selCategory)
			datacard = datacard.replace("@@WS_DIR@@", wsDir)
			datacard = datacard.replace("@@ANA_BRANCH@@", analysisBranch[0:-6])
			if (analysisBranch[0] == "Z"): #Z channel
				datacard = datacard.replace("@@IS_Z@@", "#")
				datacard = datacard.replace("@@SIGNAL_MODEL@@", "OniaSigZSig")
				datacard = datacard.replace("@@BCKG_MODEL@@", "CMS_HZToUpsilonPhoton_UntaggedTag_0_13TeV_bkgshape")
				datacard = datacard.replace("@@PBCKG_MODEL@@", "OniaBkgZSig")
				with open('inputData/fitPlotFiles2D/signalMeanSigmaJSON2D.json', 'r') as inputJSONFile: 
					signalMeanSigmaJSON = json.load(inputJSONFile)
					datacard = datacard.replace("@@MEAN_VAR@@", "mean_mHZ_"+selCategory)
					datacard = datacard.replace("@@MEAN_VAL@@", str(signalMeanSigmaJSON[analysisBranch.split("Photon")[0]+quarkoniaState+"Photon"][selCategory]["default"]["fittedMean"]))				
					with open('inputData/fitPlotFiles2D/systErrorShapes2D.json', 'r') as signalParamSystJSONFile: 
						signalParamSystJSON = json.load(signalParamSystJSONFile)
						datacard = datacard.replace("@@MEAN_ERR@@", str(signalParamSystJSON[analysisBranch.split("Photon")[0]+quarkoniaState+"Photon"][selCategory]["total"]["fittedMean"]/100.0*signalMeanSigmaJSON[analysisBranch.split("Photon")[0]+quarkoniaState+"Photon"][selCategory]["default"]["fittedMean"]))				
					# datacard = datacard.replace("@@MEAN_ERR@@", str(signalMeanSigmaJSON[analysisBranch.split("Photon")[0]+quarkoniaState+"Photon"][selCategory]["default"]["fittedMean"]*0.001))				
					datacard = datacard.replace("@@SIGMA_VAR@@", "sigma_mHZ_"+selCategory)
					datacard = datacard.replace("@@SIGMA_VAL@@", str(signalMeanSigmaJSON[analysisBranch.split("Photon")[0]+quarkoniaState+"Photon"][selCategory]["default"]["fittedSigma"]))
					with open('inputData/fitPlotFiles2D/systErrorShapes2D.json', 'r') as signalParamSystJSONFile: 
						signalParamSystJSON = json.load(signalParamSystJSONFile)
						datacard = datacard.replace("@@SIGMA_ERR@@", str(signalParamSystJSON[analysisBranch.split("Photon")[0]+quarkoniaState+"Photon"][selCategory]["total"]["fittedSigma"]/100.0*signalMeanSigmaJSON[analysisBranch.split("Photon")[0]+quarkoniaState+"Photon"][selCategory]["default"]["fittedSigma"]))				
					# datacard = datacard.replace("@@SIGMA_ERR@@", str(signalMeanSigmaJSON[analysisBranch.split("Photon")[0]+quarkoniaState+"Photon"][selCategory]["default"]["fittedSigma"]*0.02))
			if (analysisBranch[0] == "H"): # Higgs channel
				datacard = datacard.replace("@@IS_Z@@", "")
				datacard = datacard.replace("@@SIGNAL_MODEL@@", "OniaSigHSig")
				datacard = datacard.replace("@@BCKG_MODEL@@", "CMS_HZToUpsilonPhoton_UntaggedTag_0_13TeV_bkgshape")
				datacard = datacard.replace("@@PBCKG_MODEL@@", "OniaBkgHSig")
				with open('inputData/fitPlotFiles2D/signalMeanSigmaJSON2D.json', 'r') as inputJSONFile: 
					signalMeanSigmaJSON = json.load(inputJSONFile)
					datacard = datacard.replace("@@MEAN_VAR@@", "mean_Higgs_mHZ_"+selCategory)
					datacard = datacard.replace("@@MEAN_VAL@@", str(signalMeanSigmaJSON[analysisBranch.split("Photon")[0]+quarkoniaState+"Photon"][selCategory]["default"]["fittedMean"]))				
					with open('inputData/fitPlotFiles2D/systErrorShapes2D.json', 'r') as signalParamSystJSONFile: 
						signalParamSystJSON = json.load(signalParamSystJSONFile)
						datacard = datacard.replace("@@MEAN_ERR@@", str(signalParamSystJSON[analysisBranch.split("Photon")[0]+quarkoniaState+"Photon"][selCategory]["total"]["fittedMean"]/100.0*signalMeanSigmaJSON[analysisBranch.split("Photon")[0]+quarkoniaState+"Photon"][selCategory]["default"]["fittedMean"]))				
					# datacard = datacard.replace("@@MEAN_ERR@@", str(signalMeanSigmaJSON[analysisBranch.split("Photon")[0]+quarkoniaState+"Photon"][selCategory]["default"]["fittedMean"]*0.001))				
					datacard = datacard.replace("@@SIGMA_VAR@@", "sigma_cb_mHZ_"+selCategory)
					datacard = datacard.replace("@@SIGMA_VAL@@", str(signalMeanSigmaJSON[analysisBranch.split("Photon")[0]+quarkoniaState+"Photon"][selCategory]["default"]["fittedSigma"]))
					with open('inputData/fitPlotFiles2D/systErrorShapes2D.json', 'r') as signalParamSystJSONFile: 
						signalParamSystJSON = json.load(signalParamSystJSONFile)
						datacard = datacard.replace("@@SIGMA_ERR@@", str(signalParamSystJSON[analysisBranch.split("Photon")[0]+quarkoniaState+"Photon"][selCategory]["total"]["fittedSigma"]/100.0*signalMeanSigmaJSON[analysisBranch.split("Photon")[0]+quarkoniaState+"Photon"][selCategory]["default"]["fittedSigma"]))				
					# datacard = datacard.replace("@@SIGMA_ERR@@", str(signalMeanSigmaJSON[analysisBranch.split("Photon")[0]+quarkoniaState+"Photon"][selCategory]["default"]["fittedSigma"]*0.05))
			# rates
			datacard = datacard.replace("@@RATE_SIGNAL@@", getRate(ratesJSON, analysisBranch, quarkoniaState, selCategory, "signal"))
			if (analysisBranch[0] == "Z"): # Z channel
				with open('inputData/fitPlotFiles2D/ftestOutput2D/bckgNormalization.json', 'r') as inputJSONFile: 
					bckgNormalization = json.load(inputJSONFile)
					datacard = datacard.replace("@@RATE_BCKG@@", "1.0")
					# datacard = datacard.replace("@@RATE_BCKG@@", str( bckgNormalization[analysisBranch][selCategory]["nTotalBCKG"] ))
					# datacard = datacard.replace("@@RATE_BCKG@@", str( bckgNormalization[analysisBranch][selCategory]["nCombBCKG"] + bckgNormalization[analysisBranch][selCategory]["nPeakBCKG"] ))
					# datacard = datacard.replace("@@RATE_PBCKG@@", str( bckgNormalization[analysisBranch][selCategory]["nPeakBCKG"] ))
			if (analysisBranch[0] == "H"): # Higgs channel
				with open('inputData/fitPlotFiles2D/ftestOutput2D/bckgNormalization.json', 'r') as inputJSONFile: 
					bckgNormalization = json.load(inputJSONFile)
					datacard = datacard.replace("@@RATE_BCKG@@", "1.0")
					# datacard = datacard.replace("@@RATE_BCKG@@", str( bckgNormalization[analysisBranch][selCategory]["nTotalBCKG"] ))
				datacard = datacard.replace("@@RATE_PBCKG@@", getRate(ratesJSON, analysisBranch, quarkoniaState, selCategory, "pbckg"))
			# datacard = datacard.replace("@@SYST_HZ_XSEC_SIGNAL@@", getRate(ratesJSON, analysisBranch, quarkoniaState, selCategory, "pbckg"))
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
				datacard = datacard.replace("@@SYST_HZ_XSEC_PDF_SIGNAL@@", "1.0173")
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
			datacard = datacard.replace("@@SYST_PU_R_SIGNAL@@", getSyst(systsJSON, analysisBranch, quarkoniaState, selCategory, "signal", "Pileup"))
			datacard = datacard.replace("@@SYST_PU_R_PBCKG@@", getSyst(systsJSON, analysisBranch, quarkoniaState, selCategory, "pbckg", "Pileup"))
			# trg
			datacard = datacard.replace("@@SYST_TRG_SIGNAL@@", getSyst(systsJSON, analysisBranch, quarkoniaState, selCategory, "signal", "Trigger"))
			datacard = datacard.replace("@@SYST_TRG_PBCKG@@", getSyst(systsJSON, analysisBranch, quarkoniaState, selCategory, "pbckg", "Trigger"))
			# datacard = datacard.replace("@@SYST_TRG_SIGNAL@@", str(1.05))
			# datacard = datacard.replace("@@SYST_TRG_PBCKG@@", str(1.05))
			# muon_id
			datacard = datacard.replace("@@SYST_MUON_ID_SIGNAL@@", getSyst(systsJSON, analysisBranch, quarkoniaState, selCategory, "signal", "Muon ID"))
			datacard = datacard.replace("@@SYST_MUON_ID_PBCKG@@", getSyst(systsJSON, analysisBranch, quarkoniaState, selCategory, "pbckg", "Muon ID"))
			# ph_id
			datacard = datacard.replace("@@SYST_PH_ID_SIGNAL@@", getSyst(systsJSON, analysisBranch, quarkoniaState, selCategory, "signal", "Photon ID"))
			datacard = datacard.replace("@@SYST_PH_ID_PBCKG@@", getSyst(systsJSON, analysisBranch, quarkoniaState, selCategory, "pbckg", "Photon ID"))
			# ele_veto
			datacard = datacard.replace("@@SYST_ELE_VETO_SIGNAL@@", getSyst(systsJSON, analysisBranch, quarkoniaState, selCategory, "signal", "Electron Veto"))
			datacard = datacard.replace("@@SYST_ELE_VETO_PBCKG@@", getSyst(systsJSON, analysisBranch, quarkoniaState, selCategory, "pbckg", "Electron Veto"))
			# polarization
			if (analysisBranch[0] == "Z"): # Z channel
				datacard = datacard.replace("@@SYST_POL_SIGNAL@@", getSyst(systsJSON, analysisBranch, quarkoniaState, selCategory, "signal", "Polarization"))
				pass
			if (analysisBranch[0] == "H"): # Higgs channel
				datacard = datacard.replace("pol        lnN   @@SYST_POL_SIGNAL@@           -   -\n", "")
				pass

			saveDatacard(analysisBranch, quarkoniaState, selCategory, datacard)

if __name__ == "__main__":
	#############################################################################################
	# C O N F I G
	ratesFile = "inputData/evtsCountFiles/FinalYields.json"
	systematicsErrorsFile = "inputData/evtsCountFiles/systErrorYields.json"
	wsDir = "inputData/fitPlotFiles2D"

	runCombine = True
	# runCombine = False
	#############################################################################################

	print "\n-----> Datacard Maker - H/Z --> Upsilon + Photon" 

	os.system("rm -rf outputDatacards2D/*")
	os.system("rm -rf outputLimits2D/*")

	#ZToJPsiPhoton
	# makeDatacard("ZToJPsiPhoton", "", "Cat0")
	# makeDatacard("ZToJPsiPhoton", "", "Cat1")
	# makeDatacard("ZToJPsiPhoton", "", "Cat2")
	# makeDatacard("ZToJPsiPhoton", "", "Cat3")

	#HToJPsiPhoton
	# makeDatacard("HToJPsiPhoton", "", "Cat0")


	# ZToUpsilonPhoton - 1S
	makeDatacard("ZToUpsilonPhoton", "1S", "Cat1")
	makeDatacard("ZToUpsilonPhoton", "1S", "Cat2")
	makeDatacard("ZToUpsilonPhoton", "1S", "Cat3")
	makeDatacard("ZToUpsilonPhoton", "1S", "Cat0")

	# ZToUpsilonPhoton - 2S
	makeDatacard("ZToUpsilonPhoton", "2S", "Cat1")
	makeDatacard("ZToUpsilonPhoton", "2S", "Cat2")
	makeDatacard("ZToUpsilonPhoton", "2S", "Cat3")
	makeDatacard("ZToUpsilonPhoton", "2S", "Cat0")

	# ZToUpsilonPhoton - 2S
	makeDatacard("ZToUpsilonPhoton", "3S", "Cat1")
	makeDatacard("ZToUpsilonPhoton", "3S", "Cat2")
	makeDatacard("ZToUpsilonPhoton", "3S", "Cat3")
	makeDatacard("ZToUpsilonPhoton", "3S", "Cat0")

	# HToUpsilonPhoton - 1S
	makeDatacard("HToUpsilonPhoton", "1S", "Cat0")

	# HToUpsilonPhoton - 1S
	makeDatacard("HToUpsilonPhoton", "2S", "Cat0")

	# HToUpsilonPhoton - 1S
	makeDatacard("HToUpsilonPhoton", "3S", "Cat0")


	#### Run Combine
	print "\n-----> Running Combine - H/Z --> Upsilon + Photon" 

	# ZToJPsiPhoton
	# runDatacard("ZToJPsiPhoton", "", "Cat0", runCombine)
	# runDatacard("ZToJPsiPhoton", "", "Cat123", runCombine)

	# HToJPsiPhoton
	# runDatacard("HToJPsiPhoton", "", "Cat0", runCombine)

	# ZToUpsilonPhoton - 1S
	runDatacard("ZToUpsilonPhoton", "1S", "Cat0", runCombine)
	runDatacard("ZToUpsilonPhoton", "1S", "Cat123", runCombine)

	# ZToUpsilonPhoton - 2S
	runDatacard("ZToUpsilonPhoton", "2S", "Cat0", runCombine)
	runDatacard("ZToUpsilonPhoton", "2S", "Cat123", runCombine)

	# ZToUpsilonPhoton - 2S
	runDatacard("ZToUpsilonPhoton", "3S", "Cat0", runCombine)
	runDatacard("ZToUpsilonPhoton", "3S", "Cat123", runCombine)

	# HToUpsilonPhoton - 1S
	runDatacard("HToUpsilonPhoton", "1S", "Cat0", runCombine)

	# HToUpsilonPhoton - 1S
	runDatacard("HToUpsilonPhoton", "2S", "Cat0", runCombine)

	# HToUpsilonPhoton - 1S
	runDatacard("HToUpsilonPhoton", "3S", "Cat0", runCombine)


	#### Dump limits
	jsondata = json.dumps(limitsJSON, sort_keys=True, indent=2, separators=(',', ': '))
	os.system("rm outputLimits2D/limits.json")
	with open("outputLimits2D/limits.json", 'w') as out_file:
		out_file.write(jsondata)
	print "\n"+jsondata+"\n"

	# SM values
    # REFs: https://docs.google.com/spreadsheets/d/1zP8P9kp-yFrkMu9bGt4fpIirKAKYlH-w2em_kVkYYKw/edit#gid=1026998768
	# Page: 2 
	xSecSM = {}
	xSecSM["ZToJPsiGamma_BR"] = 8.9600E-08
	xSecSM["ZToUpsilon1SPhoton_BR"] = 4.8000E-08
	xSecSM["ZToUpsilon2SPhoton_BR"] = 2.4400E-08
	xSecSM["ZToUpsilon3SPhoton_BR"] = 1.8800E-08

	xSecSM["HToJPsiGamma_BR"] = 2.9900E-06
	xSecSM["HToUpsilon1SPhoton_BR"] = 5.2200E-09
	xSecSM["HToUpsilon2SPhoton_BR"] = 1.4200E-09
	xSecSM["HToUpsilon3SPhoton_BR"] = 9.1000E-10

	xSecSM["ppToZ_xSec"] = 5.7095E+04
	xSecSM["ppToH_xSec"] = 5.5614E+01

	xSecSM["JPsiToMuMu_BR"] = 5.9610E-02
	xSecSM["Upsilon1SToMuMu_BR"] = 2.4800E-02
	xSecSM["Upsilon2SToMuMu_BR"] = 1.9300E-02
	xSecSM["Upsilon3SToMuMu_BR"] = 2.1800E-02

	# BR Limits
	limitsToEvaluate = ["observedLimit", "expectedLimit", "expectedLimitMinus1Sigma", "expectedLimitPlus1Sigma"]
	BRLimits = {}
	BRLimits["ZToJPsiPhoton"] = {}
	BRLimits["ZToUpsilon1SPhoton"] = {}
	BRLimits["ZToUpsilon2SPhoton"] = {}
	BRLimits["ZToUpsilon3SPhoton"] = {}
	BRLimits["ZToUpsilonPhoton"] = {}
	# Higgs
	BRLimits["HToJPsiPhoton"] = {}
	BRLimits["HToUpsilon1SPhoton"] = {}
	BRLimits["HToUpsilon2SPhoton"] = {}
	BRLimits["HToUpsilon3SPhoton"] = {}
	BRLimits["HToUpsilonPhoton"] = {}

	for limit in limitsToEvaluate:
		# Z
		# BRLimits["ZToJPsiPhoton"][limit] = min(limitsJSON["ZToJPsiPhoton_Cat0"][limit], limitsJSON["ZToJPsiPhoton_Cat123"][limit])*xSecSM["ZToJPsiGamma_BR"]
		BRLimits["ZToUpsilon1SPhoton"][limit] = min(limitsJSON["ZToUpsilon1SPhoton_Cat0"][limit], limitsJSON["ZToUpsilon1SPhoton_Cat123"][limit])*xSecSM["ZToUpsilon1SPhoton_BR"]
		BRLimits["ZToUpsilon2SPhoton"][limit] = min(limitsJSON["ZToUpsilon2SPhoton_Cat0"][limit], limitsJSON["ZToUpsilon2SPhoton_Cat123"][limit])*xSecSM["ZToUpsilon2SPhoton_BR"]
		BRLimits["ZToUpsilon3SPhoton"][limit] = min(limitsJSON["ZToUpsilon3SPhoton_Cat0"][limit], limitsJSON["ZToUpsilon3SPhoton_Cat123"][limit])*xSecSM["ZToUpsilon3SPhoton_BR"]
		BRLimits["ZToUpsilonPhoton"][limit] = ((BRLimits["ZToUpsilon1SPhoton"][limit]*xSecSM["Upsilon1SToMuMu_BR"])+(BRLimits["ZToUpsilon2SPhoton"][limit]*xSecSM["Upsilon2SToMuMu_BR"])+(BRLimits["ZToUpsilon3SPhoton"][limit]*xSecSM["Upsilon3SToMuMu_BR"]))
		BRLimits["ZToUpsilonPhoton"][limit] = ((BRLimits["ZToUpsilon1SPhoton"][limit]*xSecSM["Upsilon1SToMuMu_BR"])+(BRLimits["ZToUpsilon2SPhoton"][limit]*xSecSM["Upsilon2SToMuMu_BR"])+(BRLimits["ZToUpsilon3SPhoton"][limit]*xSecSM["Upsilon3SToMuMu_BR"]))/((xSecSM["ZToUpsilon1SPhoton_BR"]*xSecSM["Upsilon1SToMuMu_BR"])+(xSecSM["ZToUpsilon2SPhoton_BR"]*xSecSM["Upsilon2SToMuMu_BR"])+(xSecSM["ZToUpsilon3SPhoton_BR"]*xSecSM["Upsilon3SToMuMu_BR"]))

		# Higgs
		# BRLimits["HToJPsiPhoton"][limit] = limitsJSON["HToJPsiPhoton_Cat0"][limit]*xSecSM["HToJPsiGamma_BR"]
		BRLimits["HToUpsilon1SPhoton"][limit] = limitsJSON["HToUpsilon1SPhoton_Cat0"][limit]*xSecSM["HToUpsilon1SPhoton_BR"]
		BRLimits["HToUpsilon2SPhoton"][limit] = limitsJSON["HToUpsilon2SPhoton_Cat0"][limit]*xSecSM["HToUpsilon2SPhoton_BR"]
		BRLimits["HToUpsilon3SPhoton"][limit] = limitsJSON["HToUpsilon3SPhoton_Cat0"][limit]*xSecSM["HToUpsilon3SPhoton_BR"]
		BRLimits["HToUpsilonPhoton"][limit] = ((BRLimits["HToUpsilon1SPhoton"][limit]*xSecSM["Upsilon1SToMuMu_BR"])+(BRLimits["HToUpsilon2SPhoton"][limit]*xSecSM["Upsilon2SToMuMu_BR"])+(BRLimits["HToUpsilon3SPhoton"][limit]*xSecSM["Upsilon3SToMuMu_BR"]))
		BRLimits["HToUpsilonPhoton"][limit] = ((BRLimits["HToUpsilon1SPhoton"][limit]*xSecSM["Upsilon1SToMuMu_BR"])+(BRLimits["HToUpsilon2SPhoton"][limit]*xSecSM["Upsilon2SToMuMu_BR"])+(BRLimits["HToUpsilon3SPhoton"][limit]*xSecSM["Upsilon3SToMuMu_BR"]))/((xSecSM["HToUpsilon1SPhoton_BR"]*xSecSM["Upsilon1SToMuMu_BR"])+(xSecSM["HToUpsilon2SPhoton_BR"]*xSecSM["Upsilon2SToMuMu_BR"])+(xSecSM["HToUpsilon3SPhoton_BR"]*xSecSM["Upsilon3SToMuMu_BR"]))

	jsondataBRs = json.dumps(BRLimits, sort_keys=True, indent=2, separators=(',', ': '))
	os.system("rm outputLimits2D/BRLimits.json")
	with open("outputLimits2D/BRLimits.json", 'w') as out_file:
		out_file.write(jsondataBRs)
	print "\n"+jsondataBRs+"\n"

	os.system("./dumpLatexTable_v01.py")





