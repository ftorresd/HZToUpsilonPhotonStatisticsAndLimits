#!/usr/bin/env python

import json
import math 
import os 

def getSysterror(nominal, up, down):
	diff_up = up - nominal
	diff_down = down - nominal
	max_diff = max(abs(diff_down), abs(diff_up))
	# return float(format(max_diff/nominal*100, '.2f'))
	return max_diff/nominal

def getMaxDiff(nominal, up, down):
	diff_up = up - nominal
	diff_down = down - nominal
	return max(abs(diff_down), abs(diff_up))

# shapesMasks = [
# 		"default",
# 		"statRocCorError_UP",
# 		"statRocCorError_DOWN",
# 		"refRocCorError_UP",
# 		"refRocCorError_DOWN",
# 		"profMassRocCorError_UP",
# 		"profMassRocCorError_DOWN",
# 		"fitMassRocCorError_UP",
# 		"fitMassRocCorError_DOWN",
# 		"phoScale_stat_UP",
# 		"phoScale_stat_DOWN",
# 		"phoScale_syst_UP",
# 		"phoScale_syst_DOWN",
# 		"phoScale_gain_UP",
# 		"phoScale_gain_DOWN",
# 		"phoResol_rho_UP",
# 		"phoResol_rho_DOWN",
# 		"phoResol_phi_UP",
# 		"phoResol_phi_DOWN",
# 	]

# ["ZTo"+analysisBranch+quarkoniaState+"Photon"][selCategory][shapeSystDirectory]["fittedMean"]



categoriesS = [
		"Cat0", 
		"Cat1", 
		"Cat2", 
		"Cat3",
		]


analysisBranches = [
		"ZToJPsiPhoton",
		"ZToUpsilon1SPhoton",
		"ZToUpsilon2SPhoton",
		"ZToUpsilon3SPhoton",
		"HToJPsiPhoton",
		"HToUpsilon1SPhoton",
		"HToUpsilon2SPhoton",
		"HToUpsilon3SPhoton",
		]


outputJSON = {}
for branch in analysisBranches:
	outputJSON[branch] = {}
	for cat in categoriesS:
		if ((branch[0] == "H" and cat[-1] == "0") or (branch[0] == "Z")):
			outputJSON[branch][cat] = {"muon": {}, "photon": {}, "total": {},}
			


with open('fitPlotFiles/signalMeanSigmaJSON.json', 'r') as inputJSONFile: 
	signalMeanSigmaJSON = json.load(inputJSONFile)
	for branch in analysisBranches:
		for cat in categoriesS:
			if ((branch[0] == "H" and cat[-1] == "0") or (branch[0] == "Z")):
				# fittedMean
				nominal = signalMeanSigmaJSON[branch][cat]["default"]["fittedMean"]
				#statRocCorError
				up = signalMeanSigmaJSON[branch][cat]["statRocCorError_UP"]["fittedMean"]
				down = signalMeanSigmaJSON[branch][cat]["statRocCorError_DOWN"]["fittedMean"]
				statRocCorError = getMaxDiff(nominal, up, down)
				#refRocCorError
				up = signalMeanSigmaJSON[branch][cat]["refRocCorError_UP"]["fittedMean"]
				down = signalMeanSigmaJSON[branch][cat]["refRocCorError_DOWN"]["fittedMean"]
				refRocCorError = getMaxDiff(nominal, up, down)
				#profMassRocCorError
				up = signalMeanSigmaJSON[branch][cat]["profMassRocCorError_UP"]["fittedMean"]
				down = signalMeanSigmaJSON[branch][cat]["profMassRocCorError_DOWN"]["fittedMean"]
				profMassRocCorError = getMaxDiff(nominal, up, down)
				#fitMassRocCorError
				up = signalMeanSigmaJSON[branch][cat]["fitMassRocCorError_UP"]["fittedMean"]
				down = signalMeanSigmaJSON[branch][cat]["fitMassRocCorError_DOWN"]["fittedMean"]
				fitMassRocCorError = getMaxDiff(nominal, up, down)
				# muonError - fittedMean
				outputJSON[branch][cat]["muon"]["fittedMean"] = math.sqrt(statRocCorError**2+refRocCorError**2+profMassRocCorError**2+fitMassRocCorError**2)/nominal
				# fittedSigma
				nominal = signalMeanSigmaJSON[branch][cat]["default"]["fittedSigma"]
				#statRocCorError
				up = signalMeanSigmaJSON[branch][cat]["statRocCorError_UP"]["fittedSigma"]
				down = signalMeanSigmaJSON[branch][cat]["statRocCorError_DOWN"]["fittedSigma"]
				statRocCorError = getMaxDiff(nominal, up, down)
				#refRocCorError
				up = signalMeanSigmaJSON[branch][cat]["refRocCorError_UP"]["fittedSigma"]
				down = signalMeanSigmaJSON[branch][cat]["refRocCorError_DOWN"]["fittedSigma"]
				refRocCorError = getMaxDiff(nominal, up, down)
				#profMassRocCorError
				up = signalMeanSigmaJSON[branch][cat]["profMassRocCorError_UP"]["fittedSigma"]
				down = signalMeanSigmaJSON[branch][cat]["profMassRocCorError_DOWN"]["fittedSigma"]
				profMassRocCorError = getMaxDiff(nominal, up, down)
				#fitMassRocCorError
				up = signalMeanSigmaJSON[branch][cat]["fitMassRocCorError_UP"]["fittedSigma"]
				down = signalMeanSigmaJSON[branch][cat]["fitMassRocCorError_DOWN"]["fittedSigma"]
				fitMassRocCorError = getMaxDiff(nominal, up, down)
				# muonError - fittedSigma
				outputJSON[branch][cat]["muon"]["fittedSigma"] = math.sqrt(statRocCorError**2+refRocCorError**2+profMassRocCorError**2+fitMassRocCorError**2)/nominal

	for branch in analysisBranches:
		for cat in categoriesS:
			if ((branch[0] == "H" and cat[-1] == "0") or (branch[0] == "Z")):
				# fittedMean
				nominal = signalMeanSigmaJSON[branch][cat]["default"]["fittedMean"]
				phoScale_stat_UP = abs(signalMeanSigmaJSON[branch][cat]["phoScale_stat_UP"]["fittedMean"] - nominal)
				phoScale_stat_DOWN = abs(signalMeanSigmaJSON[branch][cat]["phoScale_stat_DOWN"]["fittedMean"] - nominal)
				phoScale_syst_UP = abs(signalMeanSigmaJSON[branch][cat]["phoScale_syst_UP"]["fittedMean"] - nominal)
				phoScale_syst_DOWN = abs(signalMeanSigmaJSON[branch][cat]["phoScale_syst_DOWN"]["fittedMean"] - nominal)
				phoScale_gain_UP = abs(signalMeanSigmaJSON[branch][cat]["phoScale_gain_UP"]["fittedMean"] - nominal)
				phoScale_gain_DOWN = abs(signalMeanSigmaJSON[branch][cat]["phoScale_gain_DOWN"]["fittedMean"] - nominal)
				outputJSON[branch][cat]["photon"]["fittedMean"] = max([phoScale_stat_UP, phoScale_stat_DOWN, phoScale_syst_UP, phoScale_syst_DOWN, phoScale_gain_UP, phoScale_gain_DOWN])/nominal
				# fittedSigma
				nominal = signalMeanSigmaJSON[branch][cat]["default"]["fittedSigma"]
				phoResol_rho_UP = abs(signalMeanSigmaJSON[branch][cat]["phoResol_rho_UP"]["fittedSigma"] - nominal)
				phoResol_rho_DOWN = abs(signalMeanSigmaJSON[branch][cat]["phoResol_rho_DOWN"]["fittedSigma"] - nominal)
				phoResol_phi_UP = abs(signalMeanSigmaJSON[branch][cat]["phoResol_phi_UP"]["fittedSigma"] - nominal)
				phoResol_phi_DOWN = abs(signalMeanSigmaJSON[branch][cat]["phoResol_phi_DOWN"]["fittedSigma"] - nominal)
				outputJSON[branch][cat]["photon"]["fittedSigma"] = max([phoResol_rho_UP, phoResol_rho_DOWN, phoResol_phi_UP, phoResol_phi_DOWN])/nominal


				





	for branch in analysisBranches:
		for cat in categoriesS:
			if ((branch[0] == "H" and cat[-1] == "0") or (branch[0] == "Z")):
				outputJSON[branch][cat]["total"]["fittedMean"] = math.sqrt(outputJSON[branch][cat]["muon"]["fittedMean"]**2+outputJSON[branch][cat]["photon"]["fittedMean"]**2)*100.0
				# print outputJSON[branch][cat]["total"]["fittedMean"]
				outputJSON[branch][cat]["total"]["fittedSigma"] = math.sqrt(outputJSON[branch][cat]["muon"]["fittedSigma"]**2+outputJSON[branch][cat]["photon"]["fittedSigma"]**2)*100.0
				# print outputJSON[branch][cat]["total"]["fittedSigma"]
				outputJSON[branch][cat]["muon"]["fittedMean"] = outputJSON[branch][cat]["muon"]["fittedMean"]*100.0
				outputJSON[branch][cat]["muon"]["fittedSigma"] = outputJSON[branch][cat]["muon"]["fittedSigma"]*100.0
				outputJSON[branch][cat]["photon"]["fittedMean"] = outputJSON[branch][cat]["photon"]["fittedMean"]*100.0
				outputJSON[branch][cat]["photon"]["fittedSigma"] = outputJSON[branch][cat]["photon"]["fittedSigma"]*100.0





with open('fitPlotFiles/systErrorShapes.json', 'w') as outputJSONFile:  
    json.dump(outputJSON, outputJSONFile, indent=2)
    outputJSONFile.write("\n")


os.system("cat fitPlotFiles/systErrorShapes.json")
# os.system("cat fitPlotFiles/signalMeanSigmaJSON.json")

