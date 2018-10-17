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


#include "RooDataSet.h"
// #include "RooPolynomial.h"
// #include "TAxis.h"
// #include "RooTrace.h"


// #include "plugins/tdrstyle.C"
// #include "plugins/ZToJPsiUpsilonPhotonSignalAndBackgroundFit.C"
// #include "plugins/HToJPsiUpsilonPhotonSignalAndBackgroundFit.C"
#include "../../../HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"

#include "getEffSigma.h"

#include "json.hpp"
using json = nlohmann::json;

// // #ifdef __CINT__
// // #pragma link C++ class RooDCBShape+;
// // #endif

using namespace RooFit;
using namespace std;


struct zFitParams
{
	double mean_mHZ;
	double sigma_mHZ;
	double alpha1_mHZ;
	double n1_mHZ;
	double alpha2_mHZ;
	double n2_mHZ;

	double mean_mMuMNU;
	double sigma_mMuMNU;
	double alpha1_mMuMNU;
	double n1_mMuMNU;
	double alpha2_mMuMNU;
	double n2_mMuMNU;
};


////////////////////////////////////////////////////////////////////////////////////
// Z Fit
zFitParams ZToJPsiUpsilonPhotonSignalAndBackgroundFit2D(json * effSigmaJSON, json * signalMeanSigmaJSON, string analysisBranch, string quarkoniaState, string selCategory, zFitParams zFit, json * fitPar, bool doShapeSystMean = false, bool doShapeSystWidth = false, string shapeSystDirectory = "default") {
	bool doShapeSyst = (doShapeSystWidth == true || doShapeSystMean == true) ? true : false;
	setTDRStyle();	
	cout << "---------------> Shape parameters: " << shapeSystDirectory << endl;

	//////////////////////////////////////////////////////////////////////////////////////
	// LOAD DATA 
	TFile * outTreeToFitFileData = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_Data_ZtoUpsilon_"+selCategory+"_ZZZZZZ.root").c_str());
	auto * outTreeToFitData = (TTree*)outTreeToFitFileData->Get("default/outTree_HZtoUpsilonPhoton");

  	// RooRealVar mHZData("mHZData", "mHZData", 70, 120, "GeV") ;
  	// RooRealVar mMuMNUData("mMuMNUData", "mMuMNUData", 8.5, 11., "GeV") ;

  	RooRealVar mHZ("mHZ", "mHZ", 70, 120, "GeV") ;
  	RooRealVar mMuMNU("mMuMNU", "mMuMNU", 8.5, 11., "GeV") ;

	mHZ.setRange("LS", 70, 80);
	mHZ.setRange("RS", 100, 120);
	// RooRealVar mHZData("mHZ", "mHZ", 80, 100, "GeV") ;
	// RooRealVar weightsData("mHZWeight", "mHZWeight", -100, 100, "");
	// outTreeToFitData->GetBranch("mHZ")->SetName("mHZData");
	// outTreeToFitData->GetBranch("mMuMNU")->SetName("mMuMNUData");
	RooDataSet data("data", "", RooArgSet(mHZ, mMuMNU), Import(*outTreeToFitData)); 














	//////////////////////////////////////////////////////////////////////////////////////
	// LOAD DATA - SIGNAL
	TFile * outTreeToFitFileSignal = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_ZToUpsilon"+quarkoniaState+"Gamma_ZtoUpsilon_"+selCategory+"_ZZZZZZ.root").c_str());
	auto * outTreeToFitSignal = (TTree*)outTreeToFitFileSignal->Get((shapeSystDirectory+"/outTree_HZtoUpsilonPhoton").c_str());

	// RooRealVar mHZSignal("mHZSignal", "mHZSignal", 70, 120, "GeV") ;
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
	// DCBall - Z
	RooRealVar mean_mHZ(("mean_mHZ_"+selCategory).c_str(), "Mean" ,91.1876,70,120) ;
	RooRealVar sigma_mHZ(("sigma_mHZ_"+selCategory).c_str(), "Width" ,  2., 0.5, 4.) ;
	RooRealVar n1_mHZ("n1_mHZ","", 0.5, 0.1, 50.);//dCBPowerL
	RooRealVar n2_mHZ("n2_mHZ","", 0.5, 0.1, 50.);//dCBPowerR
	RooRealVar alpha1_mHZ("alpha1_mHZ","", 3., 0.0, 10.);//dCBCutL
	RooRealVar alpha2_mHZ("alpha2_mHZ","", 3., 0.0, 10.);//dCBCutR

	double upsilonMassPDG = 9460.30/1000.;
	if (quarkoniaState == "2S") upsilonMassPDG = 10023.26/1000.;
	if (quarkoniaState == "3S") upsilonMassPDG = 10355.2/1000.;
	
	RooRealVar mean_mMuMNU(("mean_mMuMNU_"+selCategory).c_str(), "Mean" , upsilonMassPDG, 8.5, 11.) ;
	RooRealVar sigma_mMuMNU(("sigma_mMuMNU_"+selCategory).c_str(), "Width" ,  0.1, 0., 0.3) ;
	RooRealVar n1_mMuMNU("n1_mMuMNU","", 1, 0, 50);//dCBPowerL
	RooRealVar n2_mMuMNU("n2_mMuMNU","", 1, 0, 50);//dCBPowerR
	RooRealVar alpha1_mMuMNU("alpha1_mMuMNU","", 3, 0, 10);//dCBCutL
	RooRealVar alpha2_mMuMNU("alpha2_mMuMNU","", 3, 0, 10);//dCBCutR

	////////////////////////////////////////////////////////////////////////
	// customized params
	if (analysisBranch == "Upsilon" && (quarkoniaState == "2S" || quarkoniaState == "3S") && selCategory == "Cat0") {
		// Z
		sigma_mHZ.setRange(0.5, 4.);
		sigma_mHZ.setVal(2.0);
		
		n1_mHZ.setRange(2, 10.);
		n1_mHZ.setVal(5);
		
		n2_mHZ.setRange(2, 10.) ;
		n2_mHZ.setVal(5) ;

		alpha1_mHZ.setRange(0.0, 5) ;
		alpha1_mHZ.setVal(3) ;

		alpha2_mHZ.setRange(0.0, 10) ;
		alpha2_mHZ.setVal(3) ;

		// dimuon
		sigma_mMuMNU.setRange(0., 0.3);
		sigma_mMuMNU.setVal(0.1);
		
		n1_mMuMNU.setRange(2, 10.);
		n1_mMuMNU.setVal(5);
		
		n2_mMuMNU.setRange(2, 10.) ;
		n2_mMuMNU.setVal(5) ;

		alpha1_mMuMNU.setRange(0.0, 5) ;
		alpha1_mMuMNU.setVal(3) ;

		alpha2_mMuMNU.setRange(0.0, 10) ;
		alpha2_mMuMNU.setVal(3);
	}

	if (analysisBranch == "Upsilon" && quarkoniaState == "1S" && selCategory == "Cat1") {
		// Z
		sigma_mHZ.setRange(0.5, 4.);
		sigma_mHZ.setVal(2.0);
		
		n1_mHZ.setRange(5, 10.);
		n1_mHZ.setVal(5);
		
		n2_mHZ.setRange(5, 10.) ;
		n2_mHZ.setVal(5) ;

		alpha1_mHZ.setRange(0.0, 5) ;
		alpha1_mHZ.setVal(3) ;

		alpha2_mHZ.setRange(0, 10) ;
		alpha2_mHZ.setVal(3) ;

		// dimuon
		sigma_mMuMNU.setRange(0., 0.3);
		sigma_mMuMNU.setVal(0.1);
		
		n1_mMuMNU.setRange(0, 10.);
		n1_mMuMNU.setVal(5);
		
		n2_mMuMNU.setRange(2, 10.) ;
		n2_mMuMNU.setVal(5) ;

		alpha1_mMuMNU.setRange(1, 5) ;
		alpha1_mMuMNU.setVal(3) ;

		alpha2_mMuMNU.setRange(0.0, 10) ;
		alpha2_mMuMNU.setVal(3) ;

	}

	if (analysisBranch == "Upsilon" && quarkoniaState == "2S" && selCategory == "Cat1") {
		// Z
		sigma_mHZ.setRange(0.5, 4.);
		sigma_mHZ.setVal(2.0);
		
		n1_mHZ.setRange(2, 10.);
		n1_mHZ.setVal(5);
		
		n2_mHZ.setRange(2, 10.) ;
		n2_mHZ.setVal(5) ;

		alpha1_mHZ.setRange(0.0, 5) ;
		alpha1_mHZ.setVal(3) ;

		alpha2_mHZ.setRange(0.0, 10) ;
		alpha2_mHZ.setVal(3) ;

		// dimuon
		sigma_mMuMNU.setRange(0., 0.3);
		sigma_mMuMNU.setVal(0.1);
		
		n1_mMuMNU.setRange(3, 10.);
		n1_mMuMNU.setVal(5);
		
		n2_mMuMNU.setRange(3, 10.) ;
		n2_mMuMNU.setVal(5) ;

		alpha1_mMuMNU.setRange(1, 5) ;
		alpha1_mMuMNU.setVal(3) ;

		alpha2_mMuMNU.setRange(2, 10) ;
		alpha2_mMuMNU.setVal(3);
	}


	if (analysisBranch == "Upsilon" && quarkoniaState == "3S" && selCategory == "Cat1") {
		// Z
		sigma_mHZ.setRange(0.5, 4.);
		sigma_mHZ.setVal(2.0);
		
		n1_mHZ.setRange(2, 10.);
		n1_mHZ.setVal(5);
		
		n2_mHZ.setRange(2, 10.) ;
		n2_mHZ.setVal(5) ;

		alpha1_mHZ.setRange(0.0, 5) ;
		alpha1_mHZ.setVal(3) ;

		alpha2_mHZ.setRange(0.0, 10) ;
		alpha2_mHZ.setVal(3) ;

		// dimuon
		sigma_mMuMNU.setRange(0., 0.3);
		sigma_mMuMNU.setVal(0.1);
		
		n1_mMuMNU.setRange(2, 10.);
		n1_mMuMNU.setVal(5);
		
		n2_mMuMNU.setRange(2, 10.) ;
		n2_mMuMNU.setVal(5) ;

		alpha1_mMuMNU.setRange(0.0, 5) ;
		alpha1_mMuMNU.setVal(3) ;

		alpha2_mMuMNU.setRange(0.0, 10) ;
		alpha2_mMuMNU.setVal(3);
	}

	if (analysisBranch == "Upsilon" && quarkoniaState == "1S" && selCategory == "Cat2") {
		// Z
		sigma_mHZ.setRange(0.5, 4.);
		sigma_mHZ.setVal(2.0);
		
		n1_mHZ.setRange(2, 10.);
		n1_mHZ.setVal(5);
		
		n2_mHZ.setRange(2, 10.) ;
		n2_mHZ.setVal(5) ;

		alpha1_mHZ.setRange(0.0, 5) ;
		alpha1_mHZ.setVal(3) ;

		alpha2_mHZ.setRange(0.0, 10) ;
		alpha2_mHZ.setVal(3) ;

		// dimuon
		sigma_mMuMNU.setRange(0., 0.3);
		sigma_mMuMNU.setVal(0.1);
		
		n1_mMuMNU.setRange(3, 10.);
		n1_mMuMNU.setVal(5);
		
		n2_mMuMNU.setRange(3, 10.) ;
		n2_mMuMNU.setVal(5) ;

		alpha1_mMuMNU.setRange(0.0, 5) ;
		alpha1_mMuMNU.setVal(3) ;

		alpha2_mMuMNU.setRange(0.0, 10) ;
		alpha2_mMuMNU.setVal(3);
	}

	if (analysisBranch == "Upsilon" && quarkoniaState == "2S" && selCategory == "Cat2") {
		// Z
		sigma_mHZ.setRange(0.5, 4.);
		sigma_mHZ.setVal(2.0);
		
		n1_mHZ.setRange(0, 5.);
		n1_mHZ.setVal(5);
		
		n2_mHZ.setRange(0, 5.) ;
		n2_mHZ.setVal(5) ;

		alpha1_mHZ.setRange(0.0, 5) ;
		alpha1_mHZ.setVal(3) ;

		alpha2_mHZ.setRange(0.0, 10) ;
		alpha2_mHZ.setVal(3) ;

		// dimuon
		// sigma_mMuMNU.setRange(0., 0.3);
		// sigma_mMuMNU.setVal(0.1);
		
		// n1_mMuMNU.setRange(3, 10.);
		// n1_mMuMNU.setVal(5);
		
		// n2_mMuMNU.setRange(3, 10.) ;
		// n2_mMuMNU.setVal(5) ;

		// alpha1_mMuMNU.setRange(0.0, 5) ;
		// alpha1_mMuMNU.setVal(3) ;

		// alpha2_mMuMNU.setRange(0.0, 10) ;
		// alpha2_mMuMNU.setVal(3);
	}

	if (analysisBranch == "Upsilon" && quarkoniaState == "3S" && selCategory == "Cat2") {
		// Z
		sigma_mHZ.setRange(0.5, 4.);
		sigma_mHZ.setVal(2.0);
		
		n1_mHZ.setRange(0, 10.);
		n1_mHZ.setVal(5);
		
		n2_mHZ.setRange(3, 10.) ;
		n2_mHZ.setVal(5) ;

		alpha1_mHZ.setRange(0.0, 5) ;
		alpha1_mHZ.setVal(3) ;

		alpha2_mHZ.setRange(0.0, 10) ;
		alpha2_mHZ.setVal(3) ;

		// dimuon
		sigma_mMuMNU.setRange(0., 0.3);
		sigma_mMuMNU.setVal(0.1);
		
		n1_mMuMNU.setRange(2, 10.);
		n1_mMuMNU.setVal(5);
		
		n2_mMuMNU.setRange(2, 10.) ;
		n2_mMuMNU.setVal(5) ;

		alpha1_mMuMNU.setRange(1, 5) ;
		alpha1_mMuMNU.setVal(3) ;

		alpha2_mMuMNU.setRange(0.0, 10) ;
		alpha2_mMuMNU.setVal(3);
	}


	if (analysisBranch == "Upsilon" && quarkoniaState == "1S" && selCategory == "Cat3") {
		// Z
		sigma_mHZ.setRange(0.5, 4.);
		sigma_mHZ.setVal(2.0);
		
		n1_mHZ.setRange(0, 10.);
		n1_mHZ.setVal(5);
		
		n2_mHZ.setRange(3, 10.) ;
		n2_mHZ.setVal(5) ;

		alpha1_mHZ.setRange(0.0, 5) ;
		alpha1_mHZ.setVal(3) ;

		alpha2_mHZ.setRange(0.0, 10) ;
		alpha2_mHZ.setVal(3) ;

		// dimuon
		sigma_mMuMNU.setRange(0., 0.3);
		sigma_mMuMNU.setVal(0.1);
		
		n1_mMuMNU.setRange(2, 10.);
		n1_mMuMNU.setVal(5);
		
		n2_mMuMNU.setRange(2, 10.) ;
		n2_mMuMNU.setVal(5) ;

		alpha1_mMuMNU.setRange(1, 5) ;
		alpha1_mMuMNU.setVal(3) ;

		alpha2_mMuMNU.setRange(0.0, 10) ;
		alpha2_mMuMNU.setVal(3);
	}

	if (analysisBranch == "Upsilon" && quarkoniaState == "2S" && selCategory == "Cat3") {
		// Z
		sigma_mHZ.setRange(0.5, 4.);
		sigma_mHZ.setVal(2.0);
		
		n1_mHZ.setRange(0, 10.);
		n1_mHZ.setVal(5);
		
		n2_mHZ.setRange(3, 10.) ;
		n2_mHZ.setVal(5) ;

		alpha1_mHZ.setRange(0.0, 5) ;
		alpha1_mHZ.setVal(3) ;

		alpha2_mHZ.setRange(0.0, 10) ;
		alpha2_mHZ.setVal(3) ;

		// dimuon
		sigma_mMuMNU.setRange(0., 0.3);
		sigma_mMuMNU.setVal(0.1);
		
		n1_mMuMNU.setRange(0, 10.);
		n1_mMuMNU.setVal(5);
		
		n2_mMuMNU.setRange(2, 10.) ;
		n2_mMuMNU.setVal(5) ;

		alpha1_mMuMNU.setRange(1, 5) ;
		alpha1_mMuMNU.setVal(3) ;

		alpha2_mMuMNU.setRange(1, 10) ;
		alpha2_mMuMNU.setVal(3);
	}

	if (analysisBranch == "Upsilon" && quarkoniaState == "3S" && selCategory == "Cat3") {
		// Z
		sigma_mHZ.setRange(0.5, 4.);
		sigma_mHZ.setVal(2.0);
		
		n1_mHZ.setRange(1, 10.);
		n1_mHZ.setVal(5);
		
		n2_mHZ.setRange(1, 10.) ;
		n2_mHZ.setVal(5) ;

		alpha1_mHZ.setRange(0.0, 5) ;
		alpha1_mHZ.setVal(3) ;

		alpha2_mHZ.setRange(0, 10) ;
		alpha2_mHZ.setVal(3) ;

		// dimuon
		sigma_mMuMNU.setRange(0., 0.3);
		sigma_mMuMNU.setVal(0.1);
		
		n1_mMuMNU.setRange(2, 10.);
		n1_mMuMNU.setVal(5);
		
		n2_mMuMNU.setRange(2, 10.) ;
		n2_mMuMNU.setVal(5) ;

		alpha1_mMuMNU.setRange(2, 5) ;
		alpha1_mMuMNU.setVal(3) ;

		alpha2_mMuMNU.setRange(2, 10) ;
		alpha2_mMuMNU.setVal(3);
	}

	// <END> customized params
	////////////////////////////////////////////////////////////////////////

	zFitParams zFitTemp = zFit;
	if (doShapeSystMean) {
		mean_mHZ.setVal(zFitTemp.mean_mHZ);
		// mean_mHZ.setConstant(true);

		sigma_mHZ.setVal(zFitTemp.sigma_mHZ);
		sigma_mHZ.setConstant(true);

		alpha1_mHZ.setVal(zFitTemp.alpha1_mHZ);
		alpha1_mHZ.setConstant(true);

		n1_mHZ.setVal(zFitTemp.n1_mHZ);
		n1_mHZ.setConstant(true);

		alpha2_mHZ.setVal(zFitTemp.alpha2_mHZ);
		alpha2_mHZ.setConstant(true);

		n2_mHZ.setVal(zFitTemp.n2_mHZ);
		n2_mHZ.setConstant(true);


		mean_mMuMNU.setVal(zFitTemp.mean_mMuMNU);
		mean_mMuMNU.setConstant(true);

		sigma_mMuMNU.setVal(zFitTemp.sigma_mMuMNU);
		sigma_mMuMNU.setConstant(true);

		alpha1_mMuMNU.setVal(zFitTemp.alpha1_mMuMNU);
		alpha1_mMuMNU.setConstant(true);

		n1_mMuMNU.setVal(zFitTemp.n1_mMuMNU);
		n1_mMuMNU.setConstant(true);

		alpha2_mMuMNU.setVal(zFitTemp.alpha2_mMuMNU);
		alpha2_mMuMNU.setConstant(true);

		n2_mMuMNU.setVal(zFitTemp.n2_mMuMNU);
		n2_mMuMNU.setConstant(true);


	}
	if (doShapeSystWidth) {
		mean_mHZ.setVal(zFitTemp.mean_mHZ);
		mean_mHZ.setConstant(true);

		sigma_mHZ.setVal(zFitTemp.sigma_mHZ);
		// sigma_mHZ.setConstant(true);

		alpha1_mHZ.setVal(zFitTemp.alpha1_mHZ);
		alpha1_mHZ.setConstant(true);

		n1_mHZ.setVal(zFitTemp.n1_mHZ);
		n1_mHZ.setConstant(true);

		alpha2_mHZ.setVal(zFitTemp.alpha2_mHZ);
		alpha2_mHZ.setConstant(true);

		n2_mHZ.setVal(zFitTemp.n2_mHZ);
		n2_mHZ.setConstant(true);


		mean_mMuMNU.setVal(zFitTemp.mean_mMuMNU);
		mean_mMuMNU.setConstant(true);

		sigma_mMuMNU.setVal(zFitTemp.sigma_mMuMNU);
		sigma_mMuMNU.setConstant(true);

		alpha1_mMuMNU.setVal(zFitTemp.alpha1_mMuMNU);
		alpha1_mMuMNU.setConstant(true);

		n1_mMuMNU.setVal(zFitTemp.n1_mMuMNU);
		n1_mMuMNU.setConstant(true);

		alpha2_mMuMNU.setVal(zFitTemp.alpha2_mMuMNU);
		alpha2_mMuMNU.setConstant(true);

		n2_mMuMNU.setVal(zFitTemp.n2_mMuMNU);
		n2_mMuMNU.setConstant(true);
	}

	RooDoubleCB dcball_mHZ("dcball_signal_mHZ", "dcball_mHZ", mHZ, mean_mHZ,sigma_mHZ,alpha1_mHZ,n1_mHZ,alpha2_mHZ,n2_mHZ);
	RooDoubleCB dcball_mMuMNU("dcball_signal_mMuMNU", "dcball_mMuMNU", mMuMNU, mean_mMuMNU,sigma_mMuMNU,alpha1_mMuMNU,n1_mMuMNU,alpha2_mMuMNU,n2_mMuMNU);
	RooProdPdf OniaSigZSig  ("OniaSigZSig",   "OniaSigZSig  ", RooArgSet(dcball_mMuMNU, dcball_mHZ));  



	cout << "\n\n---------------------------------------------------------------------------------------------------> Begin Signal Fit (" + analysisBranch + " - " + selCategory + " - " + shapeSystDirectory + ")\n\n" << endl;
	RooFitResult * fitResultSignal = OniaSigZSig.fitTo(signal, RooFit::Minimizer("Minuit","minimize"), Save(kTRUE), SumW2Error(kTRUE) ) ;
	// RooFitResult * fitResultSignal = dcball.fitTo(signal, Save(kTRUE), SumW2Error(kFALSE) ) ;
	fitResultSignal->Print();
	cout << "----> Saving Fit parameters... (" + analysisBranch + " - " + selCategory + " - " + shapeSystDirectory + ")\n\n" << endl;
    std::vector<RooRealVar> signalParams = {
    	mean_mHZ, 
    	sigma_mHZ, 
    	n1_mHZ, 
    	n2_mHZ, 
    	alpha1_mHZ, 
    	alpha2_mHZ, 
    	mean_mMuMNU, 
    	sigma_mMuMNU, 
    	n1_mMuMNU, 
    	n2_mMuMNU, 
    	alpha1_mMuMNU, 
    	alpha2_mMuMNU
    };
    for (auto i : signalParams) {
		if (shapeSystDirectory == "default") {
			(*fitPar)["ZTo"+analysisBranch+quarkoniaState+"Photon"][selCategory]["signal"][(string)i.GetName()] = i.getVal();
		}
    }
   	cout << "----> Saved!" << endl;

	cout << "\n\n---------------------------------------------------------------------------------------------------> End Signal Fit (" + analysisBranch + " - " + selCategory + " - " + shapeSystDirectory + ")\n\n" << endl;

	zFitTemp.mean_mHZ = mean_mHZ.getValV();
	zFitTemp.sigma_mHZ = sigma_mHZ.getValV();
	zFitTemp.alpha1_mHZ = alpha1_mHZ.getValV();
	zFitTemp.n1_mHZ = n1_mHZ.getValV();
	zFitTemp.alpha2_mHZ = alpha2_mHZ.getValV();
	zFitTemp.n2_mHZ = n2_mHZ.getValV();

	zFitTemp.mean_mMuMNU = mean_mMuMNU.getValV();
	zFitTemp.sigma_mMuMNU = sigma_mMuMNU.getValV();
	zFitTemp.alpha1_mMuMNU = alpha1_mMuMNU.getValV();
	zFitTemp.n1_mMuMNU = n1_mMuMNU.getValV();
	zFitTemp.alpha2_mMuMNU = alpha2_mMuMNU.getValV();
	zFitTemp.n2_mMuMNU = n2_mMuMNU.getValV();

	

	if (!doShapeSyst) {
	  	////////////////////////////////////////////////////////////////////////////////////
	  	// SIGNAL PLOT
		auto cSignal = new TCanvas("c2","c2",1050*2.0,750*2.0);
		gPad->SetLeftMargin(0.17); 
		gPad->SetRightMargin(0.05); 
		gPad->SetTopMargin(0.08);

		// mHZ Plot
		RooPlot* xframeSignal = mHZ.frame(Title("M_{#mu#mu#gamma}")) ;
		signal.plotOn(xframeSignal,Binning(50),RooFit::Name("signal"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
		mHZ.setRange("HZRange",70,120);
		OniaSigZSig.plotOn(xframeSignal,RooFit::Name("OniaSigZSig"),LineColor(kOrange+8));
		signal.plotOn(xframeSignal,Binning(50),RooFit::Name("signal"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
		// dcball.paramOn(xframeSignal,Layout(0.65,0.92,0.89));



		xframeSignal->SetMinimum(0.00001);
		xframeSignal->GetXaxis()->SetTitle("M_{#mu#mu#gamma} (GeV)");
		xframeSignal->GetXaxis()->SetLabelOffset(0.02);
		xframeSignal->Draw();

		// legend
		auto legendSignal = new TLegend(0.6,0.7,0.9,0.87);
		legendSignal->SetTextSize(0.037);
		legendSignal->SetHeader("Z #rightarrow #Upsilon + #gamma Analysis");
		legendSignal->SetBorderSize(0);
		legendSignal->SetFillStyle(0);
		legendSignal->AddEntry(xframeSignal->findObject("signal"), "Signal MC", "lpe");
		legendSignal->AddEntry(xframeSignal->findObject("OniaSigZSig"), "Signal Model", "l");
		legendSignal->Draw();

		// CMS decoration
		auto latexSignal = new TLatex();
		latexSignal->SetNDC();
		latexSignal->SetTextFont(61);
		latexSignal->SetTextSize(0.05);
		latexSignal->DrawLatex(.17, 0.93, "CMS");
		latexSignal->SetTextFont(52);
		latexSignal->SetTextSize(0.04);
		latexSignal->SetTextAlign(11);
		// latexSignal->DrawLatex(.25, 0.93, "Preliminary");
		latexSignal->DrawLatex(.25, 0.93, "Simulation");
		latexSignal->SetTextFont(42);
		latexSignal->SetTextSize(0.04);
		latexSignal->SetTextAlign(31);
		latexSignal->DrawLatex(0.96, 0.93, "35.86 fb^{-1} (13 TeV, 2016) ");


		if (analysisBranch == "Upsilon")  {
			system("mkdir -p  fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/");
			cSignal->SaveAs(("fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/mHZ_ZToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_Signal_"+selCategory+".root").c_str());
			cSignal->SaveAs(("fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/mHZ_ZToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_Signal_"+selCategory+".png").c_str());
			cSignal->SaveAs(("fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/mHZ_ZToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_Signal_"+selCategory+".pdf").c_str());
		}
		delete xframeSignal;

		// mMuMNU Plot
		auto cSignal_mMuMNU = new TCanvas("c2_mMuMNU","c2_mMuMNU",1050*2.0,750*2.0);
		gPad->SetLeftMargin(0.17); 
		gPad->SetRightMargin(0.05); 
		gPad->SetTopMargin(0.08);

		RooPlot* xframeSignal_mMuMNU = mMuMNU.frame(Title("M_{#mu#mu}")) ;
		signal.plotOn(xframeSignal_mMuMNU,Binning(50),RooFit::Name("signal"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 

		OniaSigZSig.plotOn(xframeSignal_mMuMNU,RooFit::Name("OniaSigZSig"),LineColor(kOrange+8));
		signal.plotOn(xframeSignal_mMuMNU,Binning(50),RooFit::Name("signal"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
		// dcball.paramOn(xframeSignal_mMuMNU,Layout(0.65,0.92,0.89));


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
		legendSignal_mMuMNU->SetTextSize(0.037);
		legendSignal_mMuMNU->SetHeader("Z #rightarrow #Upsilon + #gamma Analysis");
		legendSignal_mMuMNU->SetBorderSize(0);
		legendSignal_mMuMNU->SetFillStyle(0);
		legendSignal_mMuMNU->AddEntry(xframeSignal_mMuMNU->findObject("signal"), "Signal MC", "lpe");
		legendSignal_mMuMNU->AddEntry(xframeSignal_mMuMNU->findObject("OniaSigZSig"), "Signal Model", "l");
		legendSignal_mMuMNU->Draw();

		// CMS decoration
		auto latexSignal_mMuMNU = new TLatex();
		latexSignal_mMuMNU->SetNDC();
		latexSignal_mMuMNU->SetTextFont(61);
		latexSignal_mMuMNU->SetTextSize(0.05);
		latexSignal_mMuMNU->DrawLatex(.17, 0.93, "CMS");
		latexSignal_mMuMNU->SetTextFont(52);
		latexSignal_mMuMNU->SetTextSize(0.04);
		latexSignal_mMuMNU->SetTextAlign(11);
		// latexSignal_mMuMNU->DrawLatex(.25, 0.93, "Preliminary");
		latexSignal_mMuMNU->DrawLatex(.25, 0.93, "Simulation");
		latexSignal_mMuMNU->SetTextFont(42);
		latexSignal_mMuMNU->SetTextSize(0.04);
		latexSignal_mMuMNU->SetTextAlign(31);
		latexSignal_mMuMNU->DrawLatex(0.96, 0.93, "35.86 fb^{-1} (13 TeV, 2016) ");


		if (analysisBranch == "Upsilon")  {
			system(("mkdir -p  `dirname fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/dummy`"));
			cSignal_mMuMNU->SaveAs(("fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/mMuMNU_ZToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_Signal_"+selCategory+".root").c_str());
			cSignal_mMuMNU->SaveAs(("fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/mMuMNU_ZToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_Signal_"+selCategory+".png").c_str());
			cSignal_mMuMNU->SaveAs(("fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/mMuMNU_ZToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_Signal_"+selCategory+".pdf").c_str());
		}
		delete xframeSignal_mMuMNU;
	}
	outTreeToFitFileSignal->Close();
	delete outTreeToFitFileSignal;





























	//////////////////////////////////////////////////////////////////////////////////////
	// LOAD DATA - PEAKING BACKGROUND
	TFile * outTreeToFitFilePeakingBackground = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_ZGTo2MuG_MMuMu-2To15_ZtoUpsilon_"+selCategory+"_ZZZZZZ.root").c_str());
	auto * outTreeToFitPeakingBackground = (TTree*)outTreeToFitFilePeakingBackground->Get("default/outTree_HZtoUpsilonPhoton");

	// RooRealVar mHZPeakingBackground("mHZPeakingBackground", "mHZPeakingBackground", 70, 120, "GeV") ;
	// RooRealVar mMuMNUPeakingBackground("mMuMNUPeakingBackground", "mMuMNUPeakingBackground", 8.5, 11., "GeV") ;
	// RooRealVar weightsPeakingBackground("weightsPeakingBackground", "weightsPeakingBackground", -100, 100, "");

	// outTreeToFitPeakingBackground->GetBranch("mHZ")->SetName("mHZPeakingBackground");
	// outTreeToFitPeakingBackground->GetBranch("mMuMNU")->SetName("mMuMNUPeakingBackground");
	// outTreeToFitPeakingBackground->GetBranch("mHZWeight")->SetName("weightsPeakingBackground");
	outTreeToFitPeakingBackground->Print();	

	RooDataSet peakingBackground("peakingBackground", " ", RooArgSet(mHZ, mMuMNU, mHZWeight), Import(*outTreeToFitPeakingBackground), WeightVar(mHZWeight));
	peakingBackground.Print();


	////////////////////////////////////////////////////////////////////////////////////
	// PEAKING BACKGROUND
	// Z
	RooRealVar mean_mHZ_PeakingBackground("mean_mHZ_PeakingBackground", "Mean" ,91.1876,70,120) ;
	RooRealVar sigma_mHZ_PeakingBackground("sigma_mHZ_PeakingBackground", "Width" ,  2., 0.5, 4.) ;
	RooRealVar n1_mHZ_PeakingBackground("n1_mHZ_PeakingBackground","", 0.5, 0.1, 50.);//dCBPowerL
	RooRealVar n2_mHZ_PeakingBackground("n2_mHZ_PeakingBackground","", 0.5, 0.1, 50.);//dCBPowerR
	RooRealVar alpha1_mHZ_PeakingBackground("alpha1_mHZ_PeakingBackground","", 3., 0.0, 10.);//dCBCutL
	RooRealVar alpha2_mHZ_PeakingBackground("alpha2_mHZ_PeakingBackground","", 3., 0.0, 10.);//dCBCutR

	RooDoubleCB dcball_mHZ_PeakingBackground(
		"dcball_mHZ_PeakingBackground", 
		"dcball_mHZ_PeakingBackground", 
		mHZ, 
		mean_mHZ_PeakingBackground,
		sigma_mHZ_PeakingBackground,
		alpha1_mHZ_PeakingBackground,
		n1_mHZ_PeakingBackground,
		alpha2_mHZ_PeakingBackground,
		n2_mHZ_PeakingBackground
		);

	// dimuon 
	RooRealVar p0("p0", " ", 5, 0., 10); // coefficient of x^0 term
	RooRealVar p1("p1", " ", 5, 0., 10); // coefficient of x^1 term
	RooRealVar p2("p2", " ", 5, 0., 10); // coefficient of x^2 term
	RooRealVar p3("p3", " ", 5, 0., 10); // coefficient of x^3 term
	// RooArgList argumentsList(RooConst(1), p1, p2);
	RooArgList argumentsList(RooConst(1), p1);

	RooBernstein Bernstein_mMuMNU_PeakingBackground("Bernstein_mMuMNU_PeakingBackground", " ", mMuMNU, argumentsList);

	
	RooProdPdf OniaBkgZSig("OniaBkgZSig",   "OniaBkgZSig  ", RooArgSet(dcball_mHZ_PeakingBackground, Bernstein_mMuMNU_PeakingBackground)); 


	if (!doShapeSyst) {
		cout << "\n\n---------------------------------------------------------------------------------------------------> Begin Peaking Background Fit (" + analysisBranch + " - " + selCategory + ")\n\n" << endl;
		RooFitResult * fitResultPeakingBackground = OniaBkgZSig.fitTo(peakingBackground, RooFit::Minimizer("Minuit","minimize"), Save(kTRUE), SumW2Error(kTRUE) ) ;
		fitResultPeakingBackground->Print();
		cout << "----> Saving Fit parameters... (" + analysisBranch + " - " + selCategory + " - " + shapeSystDirectory + ")\n\n" << endl;
	    std::vector<RooRealVar> signalParams = {
	    	mean_mHZ_PeakingBackground, 
	    	sigma_mHZ_PeakingBackground, 
	    	n1_mHZ_PeakingBackground, 
	    	n2_mHZ_PeakingBackground, 
	    	alpha1_mHZ_PeakingBackground, 
	    	alpha2_mHZ_PeakingBackground, 
	    	p0, 
	    	p1, 
	    	p2, 
	    	p3, 
	    };
	    for (auto i : signalParams) {
			if (shapeSystDirectory == "default") {
				(*fitPar)["ZTo"+analysisBranch+quarkoniaState+"Photon"][selCategory]["peakingBackground"][(string)i.GetName()] = i.getVal();
			}
	    }
	   	cout << "----> Saved!" << endl;
		cout << "\n\n---------------------------------------------------------------------------------------------------> End Peaking Background Fit (" + analysisBranch + " - " + selCategory + ")\n\n" << endl;


	  	////////////////////////////////////////////////////////////////////////////////////
	  	// Peaking Background PLOT
	  	// Z
		auto cPeakingBackground = new TCanvas("cPeakingBackground","cPeakingBackground",1050*2.0,750*2.0);
		gPad->SetLeftMargin(0.17); 
		gPad->SetRightMargin(0.05); 
		gPad->SetTopMargin(0.08);
		RooPlot* xframePeakingBackground = mHZ.frame(Title("M_{#mu#mu#gamma}")) ;
		peakingBackground.plotOn(xframePeakingBackground,Binning(50),RooFit::Name("peakingBackground"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
		// dcballPeakingBackground.plotOn(xframePeakingBackground,RooFit::Name("dcballPeakingBackground"),LineColor(kAzure+7));
		OniaBkgZSig.plotOn(xframePeakingBackground,RooFit::Name("OniaBkgZSig"),LineColor(kAzure+7));
		peakingBackground.plotOn(xframePeakingBackground,Binning(50),RooFit::Name("peakingBackground"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
		// dcballPeakingBackground.paramOn(xframePeakingBackground,Layout(0.65,0.92,0.89));



		xframePeakingBackground->SetMinimum(0.00001);
		xframePeakingBackground->GetXaxis()->SetTitle("M_{#mu#mu#gamma} (GeV)");
		xframePeakingBackground->GetXaxis()->SetLabelOffset(0.02);
		xframePeakingBackground->Draw();

		// legend
		auto legendPeakingBackground = new TLegend(0.6,0.7,0.9,0.87);
		legendPeakingBackground->SetTextSize(0.037);
		if (analysisBranch == "Upsilon") legendPeakingBackground->SetHeader("Z #rightarrow #Upsilon + #gamma Analysis");
		legendPeakingBackground->SetBorderSize(0);
		legendPeakingBackground->SetFillStyle(0);
		legendPeakingBackground->AddEntry(xframePeakingBackground->findObject("peakingBackground"), "Z #rightarrow #mu#mu#gamma_{FSR} MC", "lpe");
		legendPeakingBackground->AddEntry(xframePeakingBackground->findObject("OniaBkgZSig"), "Peaking BCKG Model", "l");
		// legendSignal->AddEntry(outHistoSignal, "Expected Signal (#times 30)", "l");
		
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
		latexPeakingBackground->DrawLatex(0.96, 0.93, "35.86 fb^{-1} (13 TeV, 2016) ");

		if (analysisBranch == "Upsilon") {
			system(("mkdir -p  `dirname fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/mHZ_ZToUpsilonPhotonSignalAndBackgroundFit_PeakingBackground.png`"));
			cPeakingBackground->SaveAs(("fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/mHZ_ZToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".root").c_str());
			cPeakingBackground->SaveAs(("fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/mHZ_ZToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".png").c_str());
			cPeakingBackground->SaveAs(("fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/mHZ_ZToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".pdf").c_str());
		}

	  	// dimuon
		auto cPeakingBackground_mMuMNU = new TCanvas("cPeakingBackground_mMuMNU","cPeakingBackground",1050*2.0,750*2.0);
		gPad->SetLeftMargin(0.17); 
		gPad->SetRightMargin(0.05); 
		gPad->SetTopMargin(0.08);
		RooPlot* xframePeakingBackground_mMuMNU = mMuMNU.frame(Title("M_{#mu#mu}")) ;
		peakingBackground.plotOn(xframePeakingBackground_mMuMNU,Binning(50),RooFit::Name("peakingBackground"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
		// dcballPeakingBackground.plotOn(xframePeakingBackground,RooFit::Name("dcballPeakingBackground"),LineColor(kAzure+7));
		OniaBkgZSig.plotOn(xframePeakingBackground_mMuMNU,RooFit::Name("OniaBkgZSig"),LineColor(kAzure+7));
		peakingBackground.plotOn(xframePeakingBackground_mMuMNU,Binning(50),RooFit::Name("peakingBackground"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
		// dcballPeakingBackground.paramOn(xframePeakingBackground,Layout(0.65,0.92,0.89));



		xframePeakingBackground_mMuMNU->SetMinimum(0.00001);
		xframePeakingBackground_mMuMNU->GetXaxis()->SetTitle("M_{#mu#mu} (GeV)");
		xframePeakingBackground_mMuMNU->GetXaxis()->SetLabelOffset(0.02);
		xframePeakingBackground_mMuMNU->Draw();

		// legend
		auto legendPeakingBackground_mMuMNU = new TLegend(0.6,0.16,0.9,0.33);
		legendPeakingBackground_mMuMNU->SetTextSize(0.037);
		if (analysisBranch == "Upsilon") legendPeakingBackground_mMuMNU->SetHeader("Z #rightarrow #Upsilon + #gamma Analysis");
		legendPeakingBackground_mMuMNU->SetBorderSize(0);
		legendPeakingBackground_mMuMNU->SetFillStyle(0);
		legendPeakingBackground_mMuMNU->AddEntry(xframePeakingBackground_mMuMNU->findObject("peakingBackground"), "Z #rightarrow #mu#mu#gamma_{FSR} MC", "lpe");
		legendPeakingBackground_mMuMNU->AddEntry(xframePeakingBackground_mMuMNU->findObject("OniaBkgZSig"), "Peaking BCKG Model", "l");
		// legendSignal->AddEntry(outHistoSignal, "Expected Signal (#times 30)", "l");
		
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
		latexPeakingBackground_mMuMNU->DrawLatex(0.96, 0.93, "35.86 fb^{-1} (13 TeV, 2016) ");

		if (analysisBranch == "Upsilon") {
			system(("mkdir -p  `dirname fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/mMuMNU_ZToUpsilonPhotonSignalAndBackgroundFit_PeakingBackground.png`"));
			cPeakingBackground_mMuMNU->SaveAs(("fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/mMuMNU_ZToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".root").c_str());
			cPeakingBackground_mMuMNU->SaveAs(("fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/mMuMNU_ZToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".png").c_str());
			cPeakingBackground_mMuMNU->SaveAs(("fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/mMuMNU_ZToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".pdf").c_str());
		}
	}
	outTreeToFitFilePeakingBackground->Close();
	delete outTreeToFitFilePeakingBackground;



















	if (!doShapeSyst) {
		////////////////////////////////////////////////////////////////////////////////////
		// C r e a t e   w o r k s p a c e ,   i m p o r t   d a t a   a n d   m o d e l 
		RooWorkspace *w = new RooWorkspace("w","workspace") ;  
		
		// data
		w->import(mHZ);
		w->import(mMuMNU);
		w->import(data,Rename("data_obs"));
		// signal
		// w->import(mHZSignal,Rename("mHZSignal"));
		w->import(signal,Rename("signal"));
		w->import(OniaSigZSig);
		//peaking background
		// w->import(mHZPeakingBackground);
		w->import(peakingBackground,Rename("peakingBackground"));
		w->import(OniaBkgZSig);
		
		w->importClassCode();


		// set parameters constant
		cout << "Setting paratemers constants..." << endl;
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
			system("mkdir -p fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit");
			w->writeToFile(("fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_workspace_"+selCategory+".root").c_str());
		}


		// For FlashGG - fTest
		// create file
		TFile * outFile_forFLASHGG = TFile::Open(("fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilon"+quarkoniaState+"PhotonSignalAndBackgroundFit_workspace_forFLASHGG_"+selCategory+".root").c_str(),"RECREATE");
		TDirectory *cdtof = outFile_forFLASHGG->mkdir("tagsDumper");
	  	cdtof->cd();    // make the "tof" directory the current director

	  	// create and load the WS
	  	RooWorkspace *w_forFLASHGG = new RooWorkspace("cms_hgg_13TeV","cms_hgg_13TeV");

	  	RooRealVar CMS_hgg_mass("CMS_hgg_mass", "CMS_hgg_mass", 70, 120, "GeV") ;
	  	// outTreeToFitData->GetBranch("mHZ")->SetName("CMS_hgg_mass");	
	  	RooDataSet Data_13TeV_UntaggedTag_0("Data_13TeV_UntaggedTag_0", "", RooArgSet(CMS_hgg_mass), Import(*outTreeToFitData)); 
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
	  	outTreeToFitFileData->Close();
	  	delete outTreeToFitFileData;
  	}


	if (doShapeSystMean || shapeSystDirectory == "default") (*signalMeanSigmaJSON)["ZTo"+analysisBranch+quarkoniaState+"Photon"][selCategory][shapeSystDirectory]["fittedMean"] = zFitTemp.mean_mHZ;
	if (doShapeSystWidth || shapeSystDirectory == "default") (*signalMeanSigmaJSON)["ZTo"+analysisBranch+quarkoniaState+"Photon"][selCategory][shapeSystDirectory]["fittedSigma"] = zFitTemp.sigma_mHZ;
	return zFitTemp;



  }