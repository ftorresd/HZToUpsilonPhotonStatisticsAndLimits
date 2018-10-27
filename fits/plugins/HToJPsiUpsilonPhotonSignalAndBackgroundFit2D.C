#include <iostream>
#include <algorithm>
#include <math.h>  
// #include <stdlib.h>

#include "TFile.h"
// #include "TMath.h"
// #include "TLorentzVector.h"
#include "TTree.h"
// #include "TTreeReader.h"
// #include "TTreeReaderValue.h"
// #include "TTreeReaderArray.h"
// #include "TRandom3.h"
// #include "TROOT.h"
#include "TH2.h"
// #include "TDirectory.h"
// #include "THStack.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TLegend.h"
#include "TLatex.h"


#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooFormulaVar.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooChebychev.h"
#include "RooCBShape.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooProdPdf.h"
#include "RooBernstein.h"
#include "RooArgList.h"
#include "RooWorkspace.h"
#include "RooConstVar.h"
#include "RooVoigtian.h"



#include "RooDataSet.h"
// #include "RooPolynomial.h"
// #include "TAxis.h"
// #include "RooTrace.h"


// #include "plugins/tdrstyle.C"
// #include "plugins/ZToJPsiUpsilonPhotonSignalAndBackgroundFit.C"
// #include "plugins/HToJPsiUpsilonPhotonSignalAndBackgroundFit.C"
#include "../../../HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"


// #include "plugins/getEffSigma.h"

#include "json.hpp"
using json = nlohmann::json;

// // #ifdef __CINT__
// // #pragma link C++ class RooDCBShape+;
// // #endif

using namespace RooFit;
using namespace std;

struct hFitParams
{
	double mean_Higgs_mHZ;
	double sigma_cb_mHZ;
	double n_mHZ;
	double alpha_cb_mHZ;
	double sigma_gauss_mHZ;
	double cb_fraction_mHZ;

	// double mean_mHZ;
	// double sigma_mHZ;
	// double alpha1_mHZ;
	// double n1_mHZ;
	// double alpha2_mHZ;
	// double n2_mHZ;

	double mean_mMuMNU;
	double sigma_mMuMNU;
	double alpha1_mMuMNU;
	double n1_mMuMNU;
	double alpha2_mMuMNU;
	double n2_mMuMNU;
};



////////////////////////////////////////////////////////////////////////////////////
// Higgs Fit
hFitParams HToJPsiUpsilonPhotonSignalAndBackgroundFit2D(json * effSigmaJSON, json * signalMeanSigmaJSON, string analysisBranch, string quarkoniaState, string selCategory, hFitParams hFit, json * fitPar, bool doShapeSystMean = false, bool doShapeSystWidth = false, string shapeSystDirectory = "default") {
  	bool doShapeSyst = (doShapeSystWidth == true || doShapeSystMean == true) ? true : false;
	setTDRStyle();
	cout << "---------------> Shape parameters: " << shapeSystDirectory << endl;

	//////////////////////////////////////////////////////////////////////////////////////
	// LOAD DATA 
  	TFile * outTreeToFitFileData = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_Data_HtoUpsilon_"+selCategory+"_ZZZZZZ.root").c_str());
  	auto * outTreeToFitData = (TTree*)outTreeToFitFileData->Get("default/outTree_HZtoUpsilonPhoton");
  	// RooRealVar mHZData("mHZ", "mHZ", 100, 150, "GeV") ;
  	RooRealVar mHZ("mHZ", "mHZ", 100, 150, "GeV") ;
  	RooRealVar mMuMNU("mMuMNU", "mMuMNU", 8.5, 11., "GeV") ;
  	mHZ.setRange("LS", 100, 115);
  	mHZ.setRange("RS", 135, 150);
	// RooRealVar mHZData("mHZ", "mHZ", 80, 100, "GeV") ;
	// RooRealVar weightsData("mHZWeight", "mHZWeight", -100, 100, "");
	// outTreeToFitData->GetBranch("mHZ")->SetName("mHZData");
	// outTreeToFitData->GetBranch("mMuMNU")->SetName("mMuMNUData");
  	RooDataSet data("data", "", RooArgSet(mHZ, mMuMNU), Import(*outTreeToFitData)); 







	//////////////////////////////////////////////////////////////////////////////////////
	// LOAD DATA - SIGNAL
	TFile * outTreeToFitFileSignal = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_HToUpsilon"+quarkoniaState+"Gamma_HtoUpsilon_"+selCategory+"_ZZZZZZ.root").c_str());
	auto * outTreeToFitSignal = (TTree*)outTreeToFitFileSignal->Get((shapeSystDirectory+"/outTree_HZtoUpsilonPhoton").c_str());
	

	// RooRealVar mHZS("mHZSignal", "mHZSignal", 100, 150, "GeV") ;
	// RooRealVar mMuMNUSignal("mMuMNUSignal", "mMuMNUSignal", 8.5, 11.);
	RooRealVar mHZWeight("mHZWeight", "mHZWeight", -100, 100, "");
	// outTreeToFitSignal->GetBranch("mHZ")->SetName("mHZSignal");
	// outTreeToFitSignal->GetBranch("mMuMNU")->SetName("mMuMNUSignal");
	// outTreeToFitSignal->GetBranch("mHZWeight")->SetName("weightsSignal");
	outTreeToFitSignal->Print();	
	RooDataSet signal("signal", " ", RooArgSet(mHZ, mMuMNU, mHZWeight), Import(*outTreeToFitSignal), WeightVar(mHZWeight));
	signal.Print();

	////////////////////////////////////////////////////////////////////////////////////
	// SIGNAL MODEL
	// CB params
	RooRealVar mean_Higgs_mHZ(("mean_Higgs_mHZ_"+selCategory).c_str(), "Mean" ,125.,100.,150.) ;
	RooRealVar sigma_cb_mHZ(("sigma_cb_mHZ_"+selCategory).c_str(), "Width", 1.5, 1, 2) ;
	RooRealVar n_mHZ("n_mHZ","", 1, 0, 1);
	RooRealVar alpha_cb_mHZ("alpha_cb_mHZ","", 1, 1, 4);
	RooRealVar sigma_gauss_mHZ("sigma_gauss_mHZ", "Width" ,  1., 0, 4) ;
	RooRealVar cb_fraction_mHZ("cb_fraction_mHZ", "cb_fraction",  0.9, .0, 1.0);

	double upsilonMassPDG = 9460.30/1000.;
	if (quarkoniaState == "2S") upsilonMassPDG = 10023.26/1000.;
	if (quarkoniaState == "3S") upsilonMassPDG = 10355.2/1000.;
	
	RooRealVar mean_mMuMNU(("mean_mMuMNU_"+selCategory).c_str(), "Mean" , upsilonMassPDG, 8.5, 11.) ;
	RooRealVar sigma_mMuMNU(("sigma_mMuMNU_"+selCategory).c_str(), "Width" ,  0.1, 0., 0.3) ;
	RooRealVar n1_mMuMNU("n1_mMuMNU","", 5, 3, 10);//dCBPowerL
	RooRealVar n2_mMuMNU("n2_mMuMNU","", 5, 3, 10);//dCBPowerR
	RooRealVar alpha1_mMuMNU("alpha1_mMuMNU","", 1, 0, 10);//dCBCutL
	RooRealVar alpha2_mMuMNU("alpha2_mMuMNU","", 1, 0, 10);//dCBCutR


	// // Upsilon params
	// if (analysisBranch == "Upsilon") {
	// 	sigma_cb_mHZ.setRange(1.0, 2.0) ;
	// 	sigma_cb_mHZ.setVal(1.3) ;

	// 	alpha_cb_mHZ.setRange(1.0, 4.0);
	// 	alpha_cb_mHZ.setVal(2.5);
		
	// 	n_mHZ.setRange(0.0, 1.);
	// 	n_mHZ.setVal(0.5);
		
	// 	sigma_gauss_mHZ.setRange(0.0, 4.) ;
	// 	sigma_gauss_mHZ.setVal(2.) ;

	// 	cb_fraction_mHZ.setRange(0.0, 1.0) ;
	// 	cb_fraction_mHZ.setVal(0.99) ;
	// }

	// // custom params
	// if (analysisBranch == "Upsilon") {
	// 	sigma_cb_mHZ.setRange(0.5, 4.0) ;
	// 	sigma_cb_mHZ.setVal(2) ;

	// 	alpha_cb_mHZ.setRange(0, 5);
	// 	alpha_cb_mHZ.setVal(2);
		
	// 	n_mHZ.setRange(1, 10);
	// 	n_mHZ.setVal(4);
		
	// 	// sigma_gauss_mHZ.setRange(0.0, 4.) ;
	// 	// sigma_gauss_mHZ.setVal(2.) ;

	// 	// cb_fraction_mHZ.setRange(0.0, 1.0) ;
	// 	// cb_fraction_mHZ.setVal(0.99) ;
	// }

	// <BEGIN>  dcball model
	// RooRealVar mean_mHZ(("mean_mHZ_"+selCategory).c_str(), "Mean" ,125.,100.,150.) ;
	// RooRealVar sigma_mHZ(("sigma_mHZ_"+selCategory).c_str(), "Width" ,  2., 0.5, 4.) ;
	// RooRealVar n1_mHZ("n1_mHZ","", 5, 0, 10.);//dCBPowerL
	// RooRealVar n2_mHZ("n2_mHZ","", 5, 0, 10.);//dCBPowerR
	// RooRealVar alpha1_mHZ("alpha1_mHZ","", 1, 0, 10);//dCBCutL
	// RooRealVar alpha2_mHZ("alpha2_mHZ","", 1, 0, 10);//dCBCutR

	// if (analysisBranch == "Upsilon" && (quarkoniaState == "1S" || quarkoniaState == "2S" || quarkoniaState == "3S") && selCategory == "Cat0") {
	// 	// Z
	// 	sigma_mHZ.setRange(0, 3);
	// 	sigma_mHZ.setVal(2.5);
		
	// 	n1_mHZ.setRange(3, 10);
	// 	n1_mHZ.setVal(4);
		
	// 	n2_mHZ.setRange(4, 10) ;
	// 	n2_mHZ.setVal(5) ;

	// 	alpha1_mHZ.setRange(0, 10) ;
	// 	alpha1_mHZ.setVal(1) ;

	// 	alpha2_mHZ.setRange(0, 10) ;
	// 	alpha2_mHZ.setVal(1) ;

	// 	// // dimuon
	// 	// sigma_mMuMNU.setRange(0., 0.3);
	// 	// sigma_mMuMNU.setVal(0.1);
		
	// 	// n1_mMuMNU.setRange(0, 10.);
	// 	// n1_mMuMNU.setVal(5);
		
	// 	// n2_mMuMNU.setRange(2, 10.) ;
	// 	// n2_mMuMNU.setVal(5) ;

	// 	// alpha1_mMuMNU.setRange(1, 5) ;
	// 	// alpha1_mMuMNU.setVal(3) ;

	// 	// alpha2_mMuMNU.setRange(1, 10) ;
	// 	// alpha2_mMuMNU.setVal(3);
	// }

	// RooRealVar  resolution_sigma("resolution_sigma","sigma of the resolution function", 1, 0, 0);
	// RooVoigtian  dcball_mHZ ("gauss", "first gaussian PDF",  mHZSignal, mean_mHZ, sigma_mHZ, resolution_sigma);
 
	// <END> dcball model

	hFitParams hFitTemp = hFit;
	if (doShapeSystMean) {
		// mean_mHZ.setVal(hFitTemp.mean_mHZ);
		// // mean_mHZ.setConstant(true);

		// sigma_mHZ.setVal(hFitTemp.sigma_mHZ);
		// sigma_mHZ.setConstant(true);

		// alpha1_mHZ.setVal(hFitTemp.alpha1_mHZ);
		// alpha1_mHZ.setConstant(true);

		// n1_mHZ.setVal(hFitTemp.n1_mHZ);
		// n1_mHZ.setConstant(true);

		// alpha2_mHZ.setVal(hFitTemp.alpha2_mHZ);
		// alpha2_mHZ.setConstant(true);

		// n2_mHZ.setVal(hFitTemp.n2_mHZ);
		// n2_mHZ.setConstant(true);


		mean_Higgs_mHZ.setVal(hFitTemp.mean_Higgs_mHZ);
		// mean_Higgs_mHZ.setConstant(true);

		sigma_cb_mHZ.setVal(hFitTemp.sigma_cb_mHZ);
		sigma_cb_mHZ.setConstant(true);

		sigma_gauss_mHZ.setVal(hFitTemp.sigma_gauss_mHZ);
		sigma_gauss_mHZ.setConstant(true);

		alpha_cb_mHZ.setVal(hFitTemp.alpha_cb_mHZ);
		alpha_cb_mHZ.setConstant(true);

		n_mHZ.setVal(hFitTemp.n_mHZ);
		n_mHZ.setConstant(true);

		cb_fraction_mHZ.setVal(hFitTemp.cb_fraction_mHZ);
		cb_fraction_mHZ.setConstant(true);

		mean_mMuMNU.setVal(hFitTemp.mean_mMuMNU);
		mean_mMuMNU.setConstant(true);

		sigma_mMuMNU.setVal(hFitTemp.sigma_mMuMNU);
		sigma_mMuMNU.setConstant(true);

		alpha1_mMuMNU.setVal(hFitTemp.alpha1_mMuMNU);
		alpha1_mMuMNU.setConstant(true);

		n1_mMuMNU.setVal(hFitTemp.n1_mMuMNU);
		n1_mMuMNU.setConstant(true);

		alpha2_mMuMNU.setVal(hFitTemp.alpha2_mMuMNU);
		alpha2_mMuMNU.setConstant(true);

		n2_mMuMNU.setVal(hFitTemp.n2_mMuMNU);
		n2_mMuMNU.setConstant(true);
	}
	if (doShapeSystWidth) {

		// mean_mHZ.setVal(hFitTemp.mean_mHZ);
		// mean_mHZ.setConstant(true);

		// sigma_mHZ.setVal(hFitTemp.sigma_mHZ);
		// // sigma_mHZ.setConstant(true);

		// alpha1_mHZ.setVal(hFitTemp.alpha1_mHZ);
		// alpha1_mHZ.setConstant(true);

		// n1_mHZ.setVal(hFitTemp.n1_mHZ);
		// n1_mHZ.setConstant(true);

		// alpha2_mHZ.setVal(hFitTemp.alpha2_mHZ);
		// alpha2_mHZ.setConstant(true);

		// n2_mHZ.setVal(hFitTemp.n2_mHZ);
		// n2_mHZ.setConstant(true);


		mean_Higgs_mHZ.setVal(hFitTemp.mean_Higgs_mHZ);
		mean_Higgs_mHZ.setConstant(true);

		sigma_cb_mHZ.setVal(hFitTemp.sigma_cb_mHZ);
		// sigma_cb_mHZ.setConstant(true);

		sigma_gauss_mHZ.setVal(hFitTemp.sigma_gauss_mHZ);
		// sigma_gauss_mHZ.setConstant(true);

		alpha_cb_mHZ.setVal(hFitTemp.alpha_cb_mHZ);
		alpha_cb_mHZ.setConstant(true);

		n_mHZ.setVal(hFitTemp.n_mHZ);
		n_mHZ.setConstant(true);

		cb_fraction_mHZ.setVal(hFitTemp.cb_fraction_mHZ);
		cb_fraction_mHZ.setConstant(true);

		mean_mMuMNU.setVal(hFitTemp.mean_mMuMNU);
		mean_mMuMNU.setConstant(true);

		sigma_mMuMNU.setVal(hFitTemp.sigma_mMuMNU);
		sigma_mMuMNU.setConstant(true);

		alpha1_mMuMNU.setVal(hFitTemp.alpha1_mMuMNU);
		alpha1_mMuMNU.setConstant(true);

		n1_mMuMNU.setVal(hFitTemp.n1_mMuMNU);
		n1_mMuMNU.setConstant(true);

		alpha2_mMuMNU.setVal(hFitTemp.alpha2_mMuMNU);
		alpha2_mMuMNU.setConstant(true);

		n2_mMuMNU.setVal(hFitTemp.n2_mMuMNU);
		n2_mMuMNU.setConstant(true);
	}



	// RooDoubleCB dcball_mHZ("dcball_signal_mHZ", "dcball_mHZ", mHZSignal, mean_mHZ,sigma_mHZ,alpha1_mHZ,n1_mHZ,alpha2_mHZ,n2_mHZ);
	// RooDoubleCB dcball_mMuMNU("dcball_signal_mMuMNU", "dcball_mMuMNU", mMuMNUSignal, mean_mMuMNU,sigma_mMuMNU,alpha1_mMuMNU,n1_mMuMNU,alpha2_mMuMNU,n2_mMuMNU);
	// RooProdPdf OniaSigHSig  ("OniaSigHSig",   "OniaSigHSig  ", RooArgSet(dcball_mMuMNU, dcball_mHZ)); 

	// cb_fraction_mHZ.setVal(1);
	// cb_fraction_mHZ.setConstant(true);

	// Z signal model
	RooCBShape signal_cb_Signal("signal_cb_Signal", "Crystal Ball Signal component", mHZ, mean_Higgs_mHZ, sigma_cb_mHZ, alpha_cb_mHZ, n_mHZ);
	RooGaussian signal_gauss_Signal("signal_gauss_Signal","Gaussian Signal component", mHZ, mean_Higgs_mHZ, sigma_gauss_mHZ) ;
	// add gaussian to CB
	RooAddPdf signal_model("signal_model", "signal_model", RooArgList(signal_cb_Signal, signal_gauss_Signal), cb_fraction_mHZ); 

	RooDoubleCB dcball_mMuMNU("dcball_signal_mMuMNU", "dcball_mMuMNU", mMuMNU, mean_mMuMNU,sigma_mMuMNU,alpha1_mMuMNU,n1_mMuMNU,alpha2_mMuMNU,n2_mMuMNU);
	RooProdPdf OniaSigHSig  ("OniaSigHSig",   "OniaSigHSig  ", RooArgSet(dcball_mMuMNU, signal_model)); 


	cout << "\n\n---------------------------------------------------------------------------------------------------> Begin Signal Fit (" + analysisBranch + " - " + selCategory + " - " + shapeSystDirectory + ")\n\n" << endl;
	RooFitResult * fitResultSignal = OniaSigHSig.fitTo(signal, RooFit::Minimizer("Minuit","minimize"), Save(kTRUE), SumW2Error(kTRUE) ) ;
	fitResultSignal->Print();
	cout << "----> Saving Fit parameters... (" + analysisBranch + " - " + selCategory + " - " + shapeSystDirectory + ")\n\n" << endl;
    std::vector<RooRealVar> signalParams = {
    	mean_Higgs_mHZ, 
    	sigma_cb_mHZ, 
    	n_mHZ, 
    	alpha_cb_mHZ, 
    	sigma_gauss_mHZ, 
    	cb_fraction_mHZ, 
    	mean_mMuMNU, 
    	sigma_mMuMNU, 
    	n1_mMuMNU, 
    	n2_mMuMNU, 
    	alpha1_mMuMNU,
    	alpha2_mMuMNU
    };

    for (auto i : signalParams) {
		if (shapeSystDirectory == "default") {
			(*fitPar)["HTo"+analysisBranch+quarkoniaState+"Photon"][selCategory]["signal"][(string)i.GetName()] = i.getVal();
		}
    }
   	cout << "----> Saved!" << endl;
	cout << "\n\n---------------------------------------------------------------------------------------------------> End Signal Fit (" + analysisBranch + " - " + selCategory + " - " + shapeSystDirectory + ")\n\n" << endl;



	// hFitTemp.mean_mHZ = mean_mHZ.getValV();
	// hFitTemp.sigma_mHZ = sigma_mHZ.getValV();
	// hFitTemp.alpha1_mHZ = alpha1_mHZ.getValV();
	// hFitTemp.n1_mHZ = n1_mHZ.getValV();
	// hFitTemp.alpha2_mHZ = alpha2_mHZ.getValV();
	// hFitTemp.n2_mHZ = n2_mHZ.getValV();

	hFitTemp.mean_Higgs_mHZ = mean_Higgs_mHZ.getValV();
	hFitTemp.sigma_cb_mHZ = sigma_cb_mHZ.getValV();
	hFitTemp.sigma_gauss_mHZ = sigma_gauss_mHZ.getValV();
	hFitTemp.n_mHZ = n_mHZ.getValV();
	hFitTemp.alpha_cb_mHZ = alpha_cb_mHZ.getValV();
	hFitTemp.cb_fraction_mHZ = cb_fraction_mHZ.getValV();

	hFitTemp.mean_mMuMNU = mean_mMuMNU.getValV();
	hFitTemp.sigma_mMuMNU = sigma_mMuMNU.getValV();
	hFitTemp.alpha1_mMuMNU = alpha1_mMuMNU.getValV();
	hFitTemp.n1_mMuMNU = n1_mMuMNU.getValV();
	hFitTemp.alpha2_mMuMNU = alpha2_mMuMNU.getValV();
	hFitTemp.n2_mMuMNU = n2_mMuMNU.getValV();


	// if (!doShapeSyst) {
	  	////////////////////////////////////////////////////////////////////////////////////
	  	// SIGNAL PLOT

	  	// Higgs
		auto cSignal = new TCanvas("cSignal","cSignal",1050*2.0,750*2.0);
		gPad->SetLeftMargin(0.17); 
		gPad->SetRightMargin(0.05); 
		gPad->SetTopMargin(0.08);

		RooPlot* xframeSignal = mHZ.frame(Title("M_{#mu#mu#gamma}")) ;
		signal.plotOn(xframeSignal,Binning(50),RooFit::Name("signal"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 

		// mHZSignal.setRange("HZRange",100,150);

		OniaSigHSig.plotOn(xframeSignal,RooFit::Name("signal_cb_Signal"), RooFit::Components(signal_cb_Signal), LineColor(kBlack), LineStyle(7));
		OniaSigHSig.plotOn(xframeSignal,RooFit::Name("signal_gauss_Signal"), RooFit::Components(signal_gauss_Signal), LineColor(kBlue), LineStyle(7));
		OniaSigHSig.plotOn(xframeSignal,RooFit::Name("OniaSigHSig"),LineColor(kOrange+8));

		signal.plotOn(xframeSignal,Binning(50),RooFit::Name("signal"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 

		// xframeSignal->SetMinimum(0.00000001);
		xframeSignal->SetMinimum(0.0);
		xframeSignal->GetXaxis()->SetTitle("M_{#mu#mu#gamma} (GeV)");
		xframeSignal->GetXaxis()->SetLabelOffset(0.02);
		xframeSignal->Draw();

		// legend
		auto legendSignal = new TLegend(0.6,0.7,0.9,0.87);
		//  TLegend *leg = new TLegend(0.15+offset,0.40,0.5+offset,0.82);
	  	legendSignal->SetTextSize(0.037);


		legendSignal->SetHeader("H #rightarrow #Upsilon + #gamma Analysis");
		legendSignal->SetBorderSize(0);
		legendSignal->SetFillStyle(0);
		legendSignal->AddEntry(xframeSignal->findObject("signal"), "Signal MC", "lpe");
		legendSignal->AddEntry(xframeSignal->findObject("OniaSigHSig"), "Signal Model", "l");
		legendSignal->AddEntry(xframeSignal->findObject("signal_cb_Signal"), "CB Component", "l");
		legendSignal->AddEntry(xframeSignal->findObject("signal_gauss_Signal"), "Gauss Component", "l");
		legendSignal->Draw();
		
		// CMS decoration
		auto latexSignal = new TLatex();
		latexSignal->SetNDC();
		latexSignal->SetTextFont(61);
		latexSignal->SetTextSize(0.05);
		double labelShift = .1;
		latexSignal->DrawLatex(.17+labelShift, 0.93, "CMS");
		latexSignal->SetTextFont(52);
		latexSignal->SetTextSize(0.04);
		latexSignal->SetTextAlign(11);
		// latexSignal->DrawLatex(.25+labelShift, 0.93, "Preliminary");
		latexSignal->DrawLatex(.25+labelShift, 0.93, "Simulation");
		latexSignal->SetTextFont(42);
		latexSignal->SetTextSize(0.04);
		latexSignal->SetTextAlign(31);
		// latexSignal->DrawLatex(0.96, 0.93, "35.86 fb^{-1} (13 TeV, 2016) ");

		if (analysisBranch == "Upsilon")  {
			system("mkdir -p  fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit");
			cSignal->SaveAs(("fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit/mHZ_HToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_Signal_"+selCategory+"_"+shapeSystDirectory+".root").c_str());
			cSignal->SaveAs(("fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit/mHZ_HToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_Signal_"+selCategory+"_"+shapeSystDirectory+".png").c_str());
			cSignal->SaveAs(("fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit/mHZ_HToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_Signal_"+selCategory+"_"+shapeSystDirectory+".pdf").c_str());
		}

	if (!doShapeSyst) {
	  	// dimuon
		auto cSignal_mMuMNU = new TCanvas("cSignal_mMuMNU","cSignal_mMuMNU",1050*2.0,750*2.0);
		gPad->SetLeftMargin(0.17); 
		gPad->SetRightMargin(0.05); 
		gPad->SetTopMargin(0.08);

		RooPlot* xframeSignal_mMuMNU = mMuMNU.frame(Title("M_{#mu#mu}")) ;
		signal.plotOn(xframeSignal_mMuMNU,Binning(50),RooFit::Name("signal"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 

		OniaSigHSig.plotOn(xframeSignal_mMuMNU,RooFit::Name("OniaSigHSig"),LineColor(kOrange+8));
		signal.plotOn(xframeSignal,Binning(50),RooFit::Name("signal"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 

		// xframeSignal->SetMinimum(0.00000001);
		xframeSignal_mMuMNU->SetMinimum(0.0);
		xframeSignal_mMuMNU->GetXaxis()->SetTitle("M_{#mu#mu} (GeV)");
		xframeSignal_mMuMNU->GetXaxis()->SetLabelOffset(0.02);
		xframeSignal_mMuMNU->Draw();

		// legend
		double legX1 = 0.6;
		double legY1 = 0.7;
		double legX2 = 0.9;
		double legY2 = 0.87;
		if (quarkoniaState == "2S" || quarkoniaState == "3S" ) {
			legX1 = 0.23;
			legY1 = 0.7;
			legX2 = 0.53;
			legY2 = 0.87;
		}
		auto legendSignal_mMuMNU = new TLegend(legX1, legY1, legX2, legY2);
		//  TLegend *leg = new TLegend(0.15+offset,0.40,0.5+offset,0.82);
	  	legendSignal_mMuMNU->SetTextSize(0.037);


		legendSignal_mMuMNU->SetHeader("H #rightarrow #Upsilon + #gamma Analysis");
		legendSignal_mMuMNU->SetBorderSize(0);
		legendSignal_mMuMNU->SetFillStyle(0);
		legendSignal_mMuMNU->AddEntry(xframeSignal_mMuMNU->findObject("signal"), "Signal MC", "lpe");
		legendSignal_mMuMNU->AddEntry(xframeSignal_mMuMNU->findObject("OniaSigHSig"), "Signal Model", "l");
		// legendSignal_mMuMNU->AddEntry(xframeSignal_mMuMNU->findObject("signal_cb"), "CB Component", "l");
		// legendSignal_mMuMNU->AddEntry(xframeSignal_mMuMNU->findObject("signal_gauss"), "Gauss Component", "l");
		legendSignal_mMuMNU->Draw();
		
		// CMS decoration
		auto latexSignal_mMuMNU = new TLatex();
		latexSignal_mMuMNU->SetNDC();
		latexSignal_mMuMNU->SetTextFont(61);
		latexSignal_mMuMNU->SetTextSize(0.05);
		latexSignal_mMuMNU->DrawLatex(.17+labelShift, 0.93, "CMS");
		latexSignal_mMuMNU->SetTextFont(52);
		latexSignal_mMuMNU->SetTextSize(0.04);
		latexSignal_mMuMNU->SetTextAlign(11);
		// latexSignal_mMuMNU->DrawLatex(.25+labelShift, 0.93, "Preliminary");
		latexSignal_mMuMNU->DrawLatex(.25+labelShift, 0.93, "Simulation");
		latexSignal_mMuMNU->SetTextFont(42);
		latexSignal_mMuMNU->SetTextSize(0.04);
		latexSignal_mMuMNU->SetTextAlign(31);
		// latexSignal_mMuMNU->DrawLatex(0.96, 0.93, "35.86 fb^{-1} (13 TeV, 2016) ");

		if (analysisBranch == "Upsilon")  {
			system("mkdir -p fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit");
			cSignal_mMuMNU->SaveAs(("fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit/mMuMNU_HToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_Signal_"+selCategory+".root").c_str());
			cSignal_mMuMNU->SaveAs(("fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit/mMuMNU_HToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_Signal_"+selCategory+".png").c_str());
			cSignal_mMuMNU->SaveAs(("fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit/mMuMNU_HToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_Signal_"+selCategory+".pdf").c_str());
		}
	}
	outTreeToFitFileSignal->Close();
	delete outTreeToFitFileSignal;





























	//////////////////////////////////////////////////////////////////////////////////////
	// LOAD DATA - PEAKING BACKGROUND
	TFile * outTreeToFitFilePeakingBackground = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_HDalitz_HtoUpsilon_"+selCategory+"_ZZZZZZ.root").c_str());
	auto * outTreeToFitPeakingBackground = (TTree*)outTreeToFitFilePeakingBackground->Get("default/outTree_HZtoUpsilonPhoton");

	// RooRealVar mHZPeakingBackground("mHZ", "mHZ", 100, 150, "GeV") ;
	// RooRealVar mMuMNUPeakingBackground("mMuMNU", "mMuMNU", 8.5, 11., "GeV") ;
	// RooRealVar weightsPeakingBackground("mHZWeight", "mHZWeight", -100, 100, "");

	outTreeToFitPeakingBackground->Print();
	RooDataSet peakingBackground("peakingBackground", " ", RooArgSet(mHZ, mMuMNU, mHZWeight), Import(*outTreeToFitPeakingBackground), WeightVar(mHZWeight));
	peakingBackground.Print();

	////////////////////////////////////////////////////////////////////////////////////
	// PEAKING BACKGROUND
	RooRealVar mean_cbPeakingBackground("mean_cbPeakingBackground", "Mean" , 125, 100, 150) ;
	RooRealVar sigma_cbPeakingBackground("sigma_cbPeakingBackground", "Width" ,  2., 0.5, 4.) ;
	RooRealVar alphaPeakingBackground("alphaPeakingBackground","", 3., 0.1, 50.);
	RooRealVar nPeakingBackground("nPeakingBackground","", 0.5, 0.1, 50.);

	RooCBShape PeakingBackground_cb("PeakingBackground_cb", "Crystal Ball Peaking Background component", mHZ, mean_cbPeakingBackground, sigma_cbPeakingBackground, alphaPeakingBackground, nPeakingBackground);


	// dimuon 
	RooRealVar p0("p0", " ", 5, 0., 10); // coefficient of x^0 term
	RooRealVar p1("p1", " ", 5, 0., 10); // coefficient of x^1 term
	RooRealVar p2("p2", " ", 5, 0., 10); // coefficient of x^2 term
	RooRealVar p3("p3", " ", 5, 0., 10); // coefficient of x^3 term
	// RooArgList argumentsList(RooConst(1), p1, p2);
	RooArgList argumentsList(RooConst(1), p1);

	RooBernstein Bernstein_mMuMNU_PeakingBackground("Bernstein_mMuMNU_PeakingBackground", " ", mMuMNU, argumentsList);


	RooProdPdf OniaBkgHSig("OniaBkgHSig", "OniaBkgHSig", RooArgSet(PeakingBackground_cb, Bernstein_mMuMNU_PeakingBackground)); 

	if (!doShapeSyst) {
		cout << "\n\n---------------------------------------------------------------------------------------------------> Begin Peaking Background Fit (" + analysisBranch + " - " + selCategory + ")\n\n" << endl;
		RooFitResult * fitResultPeakingBackground = OniaBkgHSig.fitTo(peakingBackground, RooFit::Minimizer("Minuit","minimize"), Save(kTRUE), SumW2Error(kTRUE) ) ;
		fitResultPeakingBackground->Print();
		cout << "----> Saving Fit parameters... (" + analysisBranch + " - " + selCategory + " - " + shapeSystDirectory + ")\n\n" << endl;
	    std::vector<RooRealVar> signalParams = {
	    	mean_cbPeakingBackground, 
	    	sigma_cbPeakingBackground, 
	    	alphaPeakingBackground, 
	    	nPeakingBackground, 
	    	p0, 
	    	p1, 
	    	p2, 
	    	p3, 
	    };
	    for (auto i : signalParams) {
			if (shapeSystDirectory == "default") {
				(*fitPar)["HTo"+analysisBranch+quarkoniaState+"Photon"][selCategory]["peakingBackground"][(string)i.GetName()] = i.getVal();
			}
	    }
	   	cout << "----> Saved!" << endl;
		cout << "\n\n---------------------------------------------------------------------------------------------------> End Peaking Background Fit (" + analysisBranch + " - " + selCategory + ")\n\n" << endl;


	  	////////////////////////////////////////////////////////////////////////////////////
	  	// Peaking Background PLOT
	  	// Higgs
		auto cPeakingBackground = new TCanvas("cPeakingBackground","cPeakingBackground",1050*2.0,750*2.0);
		gPad->SetLeftMargin(0.17); 
		gPad->SetRightMargin(0.05); 
		gPad->SetTopMargin(0.08);
		RooPlot* xframePeakingBackground = mHZ.frame(Title("M_{#mu#mu#gamma}")) ;
		peakingBackground.plotOn(xframePeakingBackground,Binning(40),RooFit::Name("peakingBackground"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
		// dcballPeakingBackground.plotOn(xframePeakingBackground,RooFit::Name("dcballPeakingBackground"),LineColor(kAzure+7));
		OniaBkgHSig.plotOn(xframePeakingBackground,RooFit::Name("OniaBkgHSig"),LineColor(kAzure+7));
		peakingBackground.plotOn(xframePeakingBackground,Binning(40),RooFit::Name("peakingBackground"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
		// dcballPeakingBackground.paramOn(xframePeakingBackground,Layout(0.65,0.92,0.89));



		// xframePeakingBackground->SetMinimum(0.000000001);
		xframePeakingBackground->SetMinimum(0.0);
		xframePeakingBackground->GetXaxis()->SetTitle("M_{#mu#mu#gamma} (GeV)");
		xframePeakingBackground->GetXaxis()->SetLabelOffset(0.02);
		xframePeakingBackground->Draw();

		// legend
		auto legendPeakingBackground = new TLegend(0.6,0.7,0.9,0.87);
		legendPeakingBackground->SetTextSize(0.037);
		legendPeakingBackground->SetHeader("H #rightarrow #Upsilon + #gamma Analysis");
		legendPeakingBackground->SetBorderSize(0);
		legendPeakingBackground->SetFillStyle(0);
		legendPeakingBackground->AddEntry(xframePeakingBackground->findObject("peakingBackground"), "H #rightarrow #gamma^{*}#gamma", "lpe");
		legendPeakingBackground->AddEntry(xframePeakingBackground->findObject("OniaBkgHSig"), "H #rightarrow #gamma^{*}#gamma Model", "l");

		legendPeakingBackground->Draw();

		// CMS decoration
		auto latexPeakingBackground = new TLatex();
		latexPeakingBackground->SetNDC();
		latexPeakingBackground->SetTextFont(61);
		latexPeakingBackground->SetTextSize(0.05);
		latexPeakingBackground->DrawLatex(.17, 0.93, "CMS");
		latexPeakingBackground->SetTextFont(52);
		latexPeakingBackground->SetTextSize(0.04);
		latexPeakingBackground->SetTextAlign(11);
		// latexPeakingBackground->DrawLatex(.25, 0.93, "Preliminary");
		latexPeakingBackground->DrawLatex(.25, 0.93, "Simulation");
		latexPeakingBackground->SetTextFont(42);
		latexPeakingBackground->SetTextSize(0.04);
		latexPeakingBackground->SetTextAlign(31);
		// latexPeakingBackground->DrawLatex(0.96, 0.93, "35.86 fb^{-1} (13 TeV, 2016) ");


		if (analysisBranch == "Upsilon") {
			system("mkdir -p  fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit");
			cPeakingBackground->SaveAs(("fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit/mHZ_HToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".root").c_str());
			cPeakingBackground->SaveAs(("fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit/mHZ_HToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".png").c_str());
			cPeakingBackground->SaveAs(("fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit/mHZ_HToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".pdf").c_str());
		}


	  	// dimuon
		auto cPeakingBackground_mMuMNU = new TCanvas("cPeakingBackground_mMuMNU","cPeakingBackground_mMuMNU",1050*2.0,750*2.0);
		gPad->SetLeftMargin(0.17); 
		gPad->SetRightMargin(0.05); 
		gPad->SetTopMargin(0.08);
		RooPlot* xframePeakingBackground_mMuMNU = mMuMNU.frame(Title("M_{#mu#mu}")) ;
		peakingBackground.plotOn(xframePeakingBackground_mMuMNU,Binning(40),RooFit::Name("peakingBackground"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
		// dcballPeakingBackground.plotOn(xframePeakingBackground,RooFit::Name("dcballPeakingBackground"),LineColor(kAzure+7));
		OniaBkgHSig.plotOn(xframePeakingBackground_mMuMNU,RooFit::Name("OniaBkgHSig"),LineColor(kAzure+7));
		peakingBackground.plotOn(xframePeakingBackground_mMuMNU,Binning(40),RooFit::Name("peakingBackground"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
		// dcballPeakingBackground.paramOn(xframePeakingBackground,Layout(0.65,0.92,0.89));



		// xframePeakingBackground->SetMinimum(0.000000001);
		xframePeakingBackground_mMuMNU->SetMinimum(0.0);
		xframePeakingBackground_mMuMNU->GetXaxis()->SetTitle("M_{#mu#mu} (GeV)");
		xframePeakingBackground_mMuMNU->GetXaxis()->SetLabelOffset(0.02);
		xframePeakingBackground_mMuMNU->Draw();

		// legend
		auto legendPeakingBackground_mMuMNU = new TLegend(0.6,0.16,0.9,0.33);
		legendPeakingBackground_mMuMNU->SetTextSize(0.037);
		legendPeakingBackground_mMuMNU->SetHeader("H #rightarrow #Upsilon + #gamma Analysis");
		legendPeakingBackground_mMuMNU->SetBorderSize(0);
		legendPeakingBackground_mMuMNU->SetFillStyle(0);
		legendPeakingBackground_mMuMNU->AddEntry(xframePeakingBackground->findObject("peakingBackground"), "H #rightarrow #gamma^{*}#gamma", "lpe");
		legendPeakingBackground_mMuMNU->AddEntry(xframePeakingBackground->findObject("OniaBkgHSig"), "H #rightarrow #gamma^{*}#gamma Model", "l");

		legendPeakingBackground_mMuMNU->Draw();

		// CMS decoration
		auto latexPeakingBackground_mMuMNU = new TLatex();
		latexPeakingBackground_mMuMNU->SetNDC();
		latexPeakingBackground_mMuMNU->SetTextFont(61);
		latexPeakingBackground_mMuMNU->SetTextSize(0.05);
		latexPeakingBackground_mMuMNU->DrawLatex(.17, 0.93, "CMS");
		latexPeakingBackground_mMuMNU->SetTextFont(52);
		latexPeakingBackground_mMuMNU->SetTextSize(0.04);
		latexPeakingBackground_mMuMNU->SetTextAlign(11);
		// latexPeakingBackground_mMuMNU->DrawLatex(.25, 0.93, "Preliminary");
		latexPeakingBackground_mMuMNU->DrawLatex(.25, 0.93, "Simulation");
		latexPeakingBackground_mMuMNU->SetTextFont(42);
		latexPeakingBackground_mMuMNU->SetTextSize(0.04);
		latexPeakingBackground_mMuMNU->SetTextAlign(31);
		// latexPeakingBackground_mMuMNU->DrawLatex(0.96, 0.93, "35.86 fb^{-1} (13 TeV, 2016) ");


		if (analysisBranch == "Upsilon") {
			system("mkdir -p  fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit");
			cPeakingBackground_mMuMNU->SaveAs(("fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit/mMuMNU_HToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".root").c_str());
			cPeakingBackground_mMuMNU->SaveAs(("fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit/mMuMNU_HToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".png").c_str());
			cPeakingBackground_mMuMNU->SaveAs(("fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit/mMuMNU_HToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".pdf").c_str());
		}

	}
	outTreeToFitFilePeakingBackground->Close();
	delete outTreeToFitFilePeakingBackground;
























	if (!doShapeSyst) {
		////////////////////////////////////////////////////////////////////////////////////
		// C r e a t e   w o r k s p a c e ,   i m p o r t   d a t a   a n d   m o d e l 
		// For COMIBINE
		RooWorkspace *w = new RooWorkspace("w","workspace") ;  
		
		// data
		w->import(mHZ);
		w->import(mMuMNU);
		w->import(data,Rename("data_obs"));
		// w->import(Bernstein);
		// signal
		// w->import(mHZSignal,Rename("mHZSignal"));
		w->import(signal,Rename("signal"));
		w->import(OniaSigHSig);
		//peaking background
		// w->import(mHZPeakingBackground);
		w->import(peakingBackground,Rename("peakingBackground"));
		w->import(OniaBkgHSig);

	 	w->importClassCode();


		// set parameters constant
		cout << "Setting paratemers constatnt..." << endl;
		RooArgSet varSet = w->allVars();
		TIterator * varIter = varSet.createIterator();
		RooRealVar* var;
		while( (var=(RooRealVar*)varIter->Next()) ) {
		cout << " --------> var->GetName(): "<< var->GetName() << endl;
			if ((string)var->GetName() != "mHZ" || (string)var->GetName() != "mMuMNU") var->setConstant();
		}
		varIter->Reset();

		while( (var=(RooRealVar*)varIter->Next()) ) {
			cout << " --------> var->GetName(): "<< var->GetName() << endl;
			var->printExtras(cout);
			cout << "\n" << endl;
			var->printValue(cout);
			cout << "\n" << endl;
		}
		varIter->Reset();


		w->Print("t");



		if (analysisBranch == "Upsilon") {
			system("mkdir -p  fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit");
			w->writeToFile(("fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_workspace_"+selCategory+".root").c_str());
		}






		//////////////////////////////////////////
		// <BEGIN> For FlashGG - fTest
		// create file
		TFile * outFile_forFLASHGG = TFile::Open(("fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_workspace_forFLASHGG_"+selCategory+".root").c_str(),"RECREATE");
		TDirectory *cdtof = outFile_forFLASHGG->mkdir("tagsDumper");
	  	cdtof->cd();    // make the "tof" directory the current director

	  	// create and load the WS
	  	RooWorkspace *w_forFLASHGG = new RooWorkspace("cms_hgg_13TeV","cms_hgg_13TeV");

	  	RooRealVar CMS_hgg_mass("CMS_hgg_mass", "CMS_hgg_mass", 100, 150, "GeV") ;
	  	// outTreeToFitData->GetBranch("mHZ")->SetName("CMS_hgg_mass");	
	  	RooDataSet Data_13TeV_UntaggedTag_0("Data_13TeV_UntaggedTag_0", "", RooArgSet(CMS_hgg_mass), Import(*outTreeToFitData));
	  	// for (int i = 0 ; i <190 ; i++) {
	  		// Data_13TeV_UntaggedTag_0.get(i)->Print("v");
	  	// }	
	  	// Data_13TeV_UntaggedTag_0.printValue(cout); 
	  	w_forFLASHGG->import(Data_13TeV_UntaggedTag_0);


	  	RooRealVar IntLumi("IntLumi","IntLumi",35.86);
	  	w_forFLASHGG->import(IntLumi);
	  	RooRealVar weight("weight","weight",1);
	  	w_forFLASHGG->import(weight);
	  	w_forFLASHGG->Print("t");

		// save WS
	  	w_forFLASHGG->Write();
	  	outFile_forFLASHGG->Close();
	  	delete outFile_forFLASHGG;
	  	// <END> For FlashGG - fTest
	  	//////////////////////////////////////////
		
		outTreeToFitFileData->Close();
	  	delete outTreeToFitFileData;
  	}

	if (doShapeSystMean || shapeSystDirectory == "default") (*signalMeanSigmaJSON)["HTo"+analysisBranch+quarkoniaState+"Photon"][selCategory][shapeSystDirectory]["fittedMean"] = hFitTemp.mean_Higgs_mHZ;
	if (doShapeSystWidth || shapeSystDirectory == "default") (*signalMeanSigmaJSON)["HTo"+analysisBranch+quarkoniaState+"Photon"][selCategory][shapeSystDirectory]["fittedSigma"] = hFitTemp.sigma_cb_mHZ;
	// if (doShapeSystWidth || shapeSystDirectory == "default") (*signalMeanSigmaJSON)["HTo"+analysisBranch+quarkoniaState+"Photon"][selCategory][shapeSystDirectory]["fittedSigma"] = hFitTemp.sigma_gauss_mHZ;
	// if (doShapeSystMean || shapeSystDirectory == "default") (*signalMeanSigmaJSON)["HTo"+analysisBranch+quarkoniaState+"Photon"][selCategory][shapeSystDirectory]["fittedMean"] = hFitTemp.mean_mHZ;
	// if (doShapeSystWidth || shapeSystDirectory == "default") (*signalMeanSigmaJSON)["HTo"+analysisBranch+quarkoniaState+"Photon"][selCategory][shapeSystDirectory]["fittedSigma"] = hFitTemp.sigma_mHZ;

	return hFitTemp;



  }