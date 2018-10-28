#!/usr/bin/env python

import json
import ROOT
ROOT.gROOT.SetBatch(ROOT.kTRUE);
import os

# AutoVivification Class 
# ---> Testing:
# a = AutoVivification()
# a[1][2][3] = 4
# a[1][3][3] = 5
# a[1][2]['test'] = 6
# print a

# ---> Output:

# {1: {2: {'test': 6, 3: 4}, 3: {3: 5}}}

class AutoVivification(dict):
    """Implementation of perl's autovivification feature."""
    def __getitem__(self, item):
        try:
            return dict.__getitem__(self, item)
        except KeyError:
            value = self[item] = type(self)()
            return value

upsilonFitParamsJSON = AutoVivification()

def getFits(boson, selCategory):
	#############################################################################
	# LOAD DATA 
	outTreeToFitFileData = ROOT.TFile.Open("inputData/outputHistos/outTreeUpsilonControlSample_HZtoUpsilonPhoton_Data_"+boson+"toUpsilon_"+selCategory+"_ZZZZZZ.root")
	outTreeToFitData = outTreeToFitFileData.Get("default/outTree_HZtoUpsilonPhotonUpsilonControlSample")

	mUpsilonControlSample = ROOT.RooRealVar("mUpsilonControlSample", "mUpsilonControlSample", 8.5, 11., "GeV")
	mUpsilonControlSample.setRange("range", 8.5, 11.)

	data = ROOT.RooDataSet("data", "", ROOT.RooArgSet(mUpsilonControlSample), ROOT.RooFit.Import(outTreeToFitData))



	##############################
	# COMBINATORIAL BACKGROUND - mMuMu
	# mean 
	M1S = 9460.30/1000.   # upsilon 1S pgd mass value
	M2S = 10023.26/1000.  # upsilon 2S pgd mass value
	M3S = 10355.2/1000. # upsilon 3S pgd mass value 

	# mean_m = ROOT.RooRealVar("mean_m","#Upsilon mean", M1S, M1S-3, M1S+3, "GeV")
	mean_m = ROOT.RooRealVar("mean_m","#Upsilon mean", M1S)
	shift21 = ROOT.RooRealVar("shift2", "mass diff #Upsilon(1,2S)" ,M2S-M1S)
	shift31 = ROOT.RooRealVar("shift3", "mass diff #Upsilon(1,3S)" ,M3S-M1S)
	mscale = ROOT.RooRealVar("mscale", "mass scale factor", 1.0, -10, 10)
	# mscale.setConstant(ROOT.kTRUE)
	mean1S = ROOT.RooFormulaVar("mean1S", "@0*@1", ROOT.RooArgList(mean_m, mscale) )
	mean2S = ROOT.RooFormulaVar("mean2S", "@0*@1+@2", ROOT.RooArgList(mean_m, mscale, shift21) )
	mean3S = ROOT.RooFormulaVar("mean3S", "@0*@1+@2", ROOT.RooArgList(mean_m, mscale, shift31) )

	# sigma
	sigmaUpsilon = ROOT.RooRealVar("sigmaUpsilon", "sigmaUpsilon", 0.06, 0., 10.)
	sigma1S = ROOT.RooFormulaVar("sigma1S", "@0", ROOT.RooArgList(sigmaUpsilon) )
	sigma2S = ROOT.RooFormulaVar("sigma2S", "@0*@1", ROOT.RooArgList(sigmaUpsilon, ROOT.RooFit.RooConst(M2S/M1S) ) )
	sigma3S = ROOT.RooFormulaVar("sigma3S", "@0*@1", ROOT.RooArgList( sigmaUpsilon, ROOT.RooFit.RooConst(M3S/M1S) ) )

	#alpha 1
	alpha1_1S = ROOT.RooRealVar("alpha1_1S"," tail shift1", 1.5, 1, 3)
	alpha1_2S = ROOT.RooRealVar("alpha1_2S", "tail shift1", 1.5, 1, 3)
	alpha1_3S = ROOT.RooRealVar("alpha1_3S", "tail shift1", 1.5, 1, 3)
	#alpha 2
	alpha2_1S = ROOT.RooRealVar("alpha2_1S", "tail shift2", 1.5, 0, 10)
	alpha2_2S = ROOT.RooRealVar("alpha2_2S", "tail shift2", 1.5, 0, 10)
	alpha2_3S = ROOT.RooRealVar("alpha2_3S", "tail shift2", 1.5, 0, 10)

	#npow 1
	npow1_1S = ROOT.RooRealVar("npow1_1S", "power order1", 1.5, 0, 3)
	npow1_2S = ROOT.RooRealVar("npow1_2S", "power order1", 1.5, 0, 3)
	npow1_3S = ROOT.RooRealVar("npow1_3S", "power order1", 1.5, 0, 3)
	#npow 2
	npow2_1S = ROOT.RooRealVar("npow2_1S", "power order2", 2, 0, 10)
	npow2_2S = ROOT.RooRealVar("npow2_2S", "power order2", 2, 0, 10)
	npow2_3S = ROOT.RooRealVar("npow2_3S", "power order2", 2, 0, 10)

	# Upsilon 1S
	# bckg1S = ROOT.RooDoubleCB("bckg1S", "bckg1S", mUpsilonControlSample, mean1S, sigma1S, alpha1_1S, npow1_1S, alpha2_1S, npow2_1S)
	# bckg1S = ROOT.RooCBShape("bckg1S", "bckg1S", mUpsilonControlSample, mean1S, sigma1S, alpha1_1S, npow1_1S)
	bckg1S = ROOT.RooGaussian("bckg1S", "bckg1S", mUpsilonControlSample, mean1S, sigma1S)

	# Upsilon 2S
	# bckg2S = ROOT.RooDoubleCB("bckg2S", "bckg2S", mUpsilonControlSample, mean2S, sigma2S, alpha1_2S, npow1_2S, alpha2_2S, npow2_2S)
	# bckg2S = ROOT.RooCBShape("bckg2S", "bckg2S", mUpsilonControlSample, mean2S, sigma2S, alpha1_2S, npow1_2S)
	bckg2S = ROOT.RooGaussian("bckg2S", "bckg2S", mUpsilonControlSample, mean2S, sigma2S)

	# Upsilon 3S
	# bckg3S = ROOT.RooDoubleCB("bckg3S", "bckg3S", mUpsilonControlSample, mean3S, sigma3S, alpha1_3S, npow1_3S, alpha2_3S, npow2_3S)
	# bckg3S = ROOT.RooCBShape("bckg3S", "bckg3S", mUpsilonControlSample, mean3S, sigma3S, alpha1_3S, npow1_3S)
	bckg3S = ROOT.RooGaussian("bckg3S", "bckg3S", mUpsilonControlSample, mean3S, sigma3S)

	#Background
	bkg_a1  = ROOT.RooRealVar("bkg_a1", "background a1", 0, -1, 1)
	mBkg  = ROOT.RooChebychev("mBkg", "linear background", mUpsilonControlSample, ROOT.RooArgList(bkg_a1) )


	fbckg1S = ROOT.RooRealVar("fbckg1S", "fbckg1S",  0.1, 0.0, 1.0)
	fbckg2S = ROOT.RooRealVar("fbckg2S", "fbckg1S",  0.1, 0.0, 1.0)
	fbckg3S = ROOT.RooRealVar("fbckg3S", "fbckg1S",  0.1, 0.0, 1.0)
	combBckg_mMuMu = ROOT.RooAddPdf("combBckg_mMuMu", "combBckg_mMuMu", ROOT.RooArgList( bckg1S, bckg2S, bckg3S, mBkg), ROOT.RooArgList(fbckg1S,fbckg2S,fbckg3S), ROOT.kTRUE)

	print "\n\n---------------------------------------------------------------------------------------------------> Begin Upsilon Bckg Fit ( " + boson + " - " + selCategory + " )\n\n"
	fitResultSignal = combBckg_mMuMu.fitTo( data, ROOT.RooFit.Save(ROOT.kTRUE) ) 
	fitResultSignal.Print()
	print "----> Saving Fit parameters... ( " + boson + " - " + selCategory + " )\n\n"

	#Get list of parameters, given list of observables
	model_params = combBckg_mMuMu.getParameters(data)
	model_params.Print("v")

	iter = model_params.createIterator()
	var = iter.Next()
	while var :
		# print var.getVal()
		# print var.GetName()
		upsilonFitParamsJSON[boson+"ToUpsilonPhoton"][selCategory][var.GetName()] = var.getVal()
		var = iter.Next()

   	print "----> Saved!"


   	print "----> Plotting..."
	# SIGNAL PLOT
	cSignal = ROOT.TCanvas("c2","c2")
	cSignal.SetCanvasSize(1050*2,750*2)
	ROOT.gPad.SetLeftMargin(0.17)
	ROOT.gPad.SetRightMargin(0.05)
	ROOT.gPad.SetTopMargin(0.08)

	# xframeSignal = mUpsilonControlSample.frame(ROOT.RooFit.Title("M_{#mu#mu}")) 
	xframeSignal = mUpsilonControlSample.frame(ROOT.RooFit.Title(" ")) 
	data.plotOn(xframeSignal,ROOT.RooFit.Binning(50),ROOT.RooFit.Name("mUpsilonControlSample"), ROOT.RooFit.MarkerSize(3)) 	
	
	combBckg_mMuMu.plotOn(xframeSignal,ROOT.RooFit.Name("mBkg"), ROOT.RooFit.Components( "mBkg" ), ROOT.RooFit.LineColor(ROOT.kBlack), ROOT.RooFit.LineStyle(7));
	combBckg_mMuMu.plotOn(xframeSignal,ROOT.RooFit.Name("bckg1S"), ROOT.RooFit.Components( "bckg1S" ), ROOT.RooFit.LineColor(ROOT.kAzure-3), ROOT.RooFit.LineStyle(7));
	combBckg_mMuMu.plotOn(xframeSignal,ROOT.RooFit.Name("bckg2S"), ROOT.RooFit.Components( "bckg2S" ), ROOT.RooFit.LineColor(ROOT.kAzure-3), ROOT.RooFit.LineStyle(7));
	combBckg_mMuMu.plotOn(xframeSignal,ROOT.RooFit.Name("bckg3S"), ROOT.RooFit.Components( "bckg3S" ), ROOT.RooFit.LineColor(ROOT.kAzure-3), ROOT.RooFit.LineStyle(7));
	combBckg_mMuMu.plotOn(xframeSignal,ROOT.RooFit.Name("combBckg_mMuMu"), ROOT.RooFit.LineColor(ROOT.kRed-2));


	data.plotOn(xframeSignal,ROOT.RooFit.Binning(35),ROOT.RooFit.Name("data"), ROOT.RooFit.MarkerSize(3)) 

	xframeSignal.SetMinimum(0.00001)
	xframeSignal.GetXaxis().SetTitle("M_{#mu#mu} (GeV)")
	xframeSignal.GetXaxis().SetLabelOffset(0.02)
	xframeSignal.Draw()

	# legend
	legendSignal = ROOT.TLegend(0.56,0.66,0.87,0.87)
	legendSignal.SetTextSize(0.037)
	legendSignal.SetHeader( boson + " #rightarrow #Upsilon + #gamma Analysis")
	legendSignal.SetBorderSize(0)
	legendSignal.SetFillStyle(0)
	legendSignal.AddEntry(xframeSignal.findObject("data"), "Upsilon Control Sample", "lpe")
	legendSignal.AddEntry(xframeSignal.findObject("mBkg"), "Combinatorial Component", "l")
	legendSignal.AddEntry(xframeSignal.findObject("bckg1S"), "#Upsilon(1S) Component", "l")
	legendSignal.AddEntry(xframeSignal.findObject("bckg2S"), "#Upsilon(2S) Component", "l")
	legendSignal.AddEntry(xframeSignal.findObject("bckg3S"), "#Upsilon(3S) Component", "l")
	legendSignal.AddEntry(xframeSignal.findObject("combBckg_mMuMu"), "Upsilon Control Model", "l")
	
	legendSignal.Draw()

	# CMS decoration
	latexSignal = ROOT.TLatex()
	latexSignal.SetNDC()
	latexSignal.SetTextFont(61)
	latexSignal.SetTextSize(0.05)
	latexSignal.DrawLatex(.17, 0.93, "CMS")
	latexSignal.SetTextFont(52)
	latexSignal.SetTextSize(0.04)
	latexSignal.SetTextAlign(11)
	latexSignal.DrawLatex(.25, 0.93, "Preliminary")
	latexSignal.SetTextFont(42)
	latexSignal.SetTextSize(0.04)
	latexSignal.SetTextAlign(31)
	latexSignal.DrawLatex(0.96, 0.93, "35.86 fb^{-1} (13 TeV, 2016) ")

	os.system("mkdir -p  fitPlotFiles2D/UpsilonControlSample")
	cSignal.SaveAs( "fitPlotFiles2D/UpsilonControlSample/upsilonControlSample_"+boson+"ToUpsilonPhoton_"+selCategory+".root" )
	cSignal.SaveAs( "fitPlotFiles2D/UpsilonControlSample/upsilonControlSample_"+boson+"ToUpsilonPhoton_"+selCategory+".pdf" )
	cSignal.SaveAs( "fitPlotFiles2D/UpsilonControlSample/upsilonControlSample_"+boson+"ToUpsilonPhoton_"+selCategory+".png" )
	







if __name__ == "__main__":
	# clean-up
	os.system("rm -rf fitPlotFiles2D/UpsilonControlSample")

	# get fits
	# getFits("H", "Cat0")
	getFits("Z", "Cat0")
	# getFits("Z", "Cat1")
	# getFits("Z", "Cat2")
	# getFits("Z", "Cat3")

	# write output
	with open('fitPlotFiles2D/UpsilonControlSample/upsilonFitParams.json', 'w') as upsilonFitParamsFile:  
		json.dump(upsilonFitParamsJSON, upsilonFitParamsFile, indent=2)
		upsilonFitParamsFile.write("\n")

	# print output
	# os.system("cat fitPlotFiles2D/UpsilonControlSample/upsilonFitParams.json")



