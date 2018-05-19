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


categoriesS = [
		"Cat0", 
		"Cat1", 
		"Cat2", 
		"Cat3",
		]


analysisBranches = [
		# "ZToJPsiPhoton",
		"ZToUpsilon1SPhoton",
		"ZToUpsilon2SPhoton",
		"ZToUpsilon3SPhoton",
		# "HToJPsiPhoton",
		"HToUpsilon1SPhoton",
		"HToUpsilon2SPhoton",
		"HToUpsilon3SPhoton",
		]


outputJSON = {}
for branch in analysisBranches:
	outputJSON[branch] = {}
	for cat in categoriesS:
		if ((branch[0] == "H" and cat[-1] == "0") or (branch[0] == "Z")):
			outputJSON[branch][cat] = {
					"muon": {}, 
					"photon": {}, 
					"total": {}, 
					# "phoScale_stat_UP": {},
					# "phoScale_stat_DOWN": {},
					# "phoScale_syst_UP": {},
					# "phoScale_syst_DOWN": {},
					# "phoScale_gain_UP": {},
					# "phoScale_gain_DOWN": {},
					# "phoResol_rho_UP": {},
					# "phoResol_rho_DOWN": {},
					# "phoResol_phi_UP": {},
					# "phoResol_phi_DOWN": {},
					}




with open('fitPlotFiles2D/signalMeanSigmaJSON2D.json', 'r') as inputJSONFile: 
	signalMeanSigmaJSON = json.load(inputJSONFile)
	for branch in analysisBranches:
		for cat in categoriesS:
			if ((branch[0] == "H" and cat[-1] == "0") or (branch[0] == "Z")):

				# fittedMean
				nominal = signalMeanSigmaJSON[branch][cat]["default"]["fittedMean"]
				#RocCorError
				up = signalMeanSigmaJSON[branch][cat]["RocCorError_UP"]["fittedMean"]
				down = signalMeanSigmaJSON[branch][cat]["RocCorError_DOWN"]["fittedMean"]
				RocCorError = getMaxDiff(nominal, up, down)
				outputJSON[branch][cat]["muon"]["fittedMean"] = RocCorError/nominal
				# print outputJSON
				# fittedSigma
				nominal = signalMeanSigmaJSON[branch][cat]["default"]["fittedSigma"]
				#RocCorError
				up = signalMeanSigmaJSON[branch][cat]["RocCorError_UP"]["fittedSigma"]
				down = signalMeanSigmaJSON[branch][cat]["RocCorError_DOWN"]["fittedSigma"]
				RocCorError = getMaxDiff(nominal, up, down)
				outputJSON[branch][cat]["muon"]["fittedSigma"] = RocCorError/nominal

	for branch in analysisBranches:
		for cat in categoriesS:
			if ((branch[0] == "H" and cat[-1] == "0") or (branch[0] == "Z")):
				# fittedMean
				nominal = signalMeanSigmaJSON[branch][cat]["default"]["fittedMean"]
				phoScale_stat_UP = abs(signalMeanSigmaJSON[branch][cat]["phoScale_stat_UP"]["fittedMean"] - nominal)
				outputJSON[branch][cat]["photon"]["phoScale_stat_UP"] = phoScale_stat_UP/nominal*100
				phoScale_stat_DOWN = abs(signalMeanSigmaJSON[branch][cat]["phoScale_stat_DOWN"]["fittedMean"] - nominal)
				outputJSON[branch][cat]["photon"]["phoScale_stat_DOWN"] = phoScale_stat_DOWN/nominal*100
				phoScale_syst_UP = abs(signalMeanSigmaJSON[branch][cat]["phoScale_syst_UP"]["fittedMean"] - nominal)
				outputJSON[branch][cat]["photon"]["phoScale_syst_UP"] = phoScale_syst_UP/nominal*100
				phoScale_syst_DOWN = abs(signalMeanSigmaJSON[branch][cat]["phoScale_syst_DOWN"]["fittedMean"] - nominal)
				outputJSON[branch][cat]["photon"]["phoScale_syst_DOWN"] = phoScale_syst_DOWN/nominal*100
				phoScale_gain_UP = abs(signalMeanSigmaJSON[branch][cat]["phoScale_gain_UP"]["fittedMean"] - nominal)
				outputJSON[branch][cat]["photon"]["phoScale_gain_UP"] = phoScale_gain_UP/nominal*100
				phoScale_gain_DOWN = abs(signalMeanSigmaJSON[branch][cat]["phoScale_gain_DOWN"]["fittedMean"] - nominal)
				outputJSON[branch][cat]["photon"]["phoScale_gain_DOWN"] = phoScale_gain_DOWN/nominal*100
				outputJSON[branch][cat]["photon"]["fittedMean"] = math.sqrt(max([phoScale_stat_UP, phoScale_stat_DOWN])**2 + max([phoScale_syst_UP, phoScale_syst_DOWN])**2 + max([phoScale_gain_UP, phoScale_gain_DOWN]))/nominal
				# fittedSigma
				nominal = signalMeanSigmaJSON[branch][cat]["default"]["fittedSigma"]
				phoResol_rho_UP = abs(signalMeanSigmaJSON[branch][cat]["phoResol_rho_UP"]["fittedSigma"] - nominal)
				outputJSON[branch][cat]["photon"]["phoResol_rho_UP"] = phoResol_rho_UP/nominal*100
				phoResol_rho_DOWN = abs(signalMeanSigmaJSON[branch][cat]["phoResol_rho_DOWN"]["fittedSigma"] - nominal)
				outputJSON[branch][cat]["photon"]["phoResol_rho_DOWN"] = phoResol_rho_DOWN/nominal*100
				phoResol_phi_UP = abs(signalMeanSigmaJSON[branch][cat]["phoResol_phi_UP"]["fittedSigma"] - nominal)
				outputJSON[branch][cat]["photon"]["phoResol_phi_UP"] = phoResol_phi_UP/nominal*100
				phoResol_phi_DOWN = abs(signalMeanSigmaJSON[branch][cat]["phoResol_phi_DOWN"]["fittedSigma"] - nominal)
				outputJSON[branch][cat]["photon"]["phoResol_phi_DOWN"] = phoResol_phi_DOWN/nominal*100
				outputJSON[branch][cat]["photon"]["fittedSigma"] = math.sqrt(max([phoResol_rho_UP, phoResol_rho_DOWN])**2 + max([phoResol_phi_UP, phoResol_phi_DOWN])**2)/nominal


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


with open('fitPlotFiles2D/systErrorShapes2D.json', 'w') as outputJSONFile:  
    json.dump(outputJSON, outputJSONFile, indent=2)
    outputJSONFile.write("\n")


os.system("cat fitPlotFiles2D/systErrorShapes2D.json")
# os.system("cat fitPlotFiles/signalMeanSigmaJSON.json")

