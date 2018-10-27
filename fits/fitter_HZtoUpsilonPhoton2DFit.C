#include <iostream>
#include <algorithm>
#include <math.h>  
#include <future>

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


// #include "../plugins/tdrstyle.C"
#include "plugins/tdrstyle.C"
#include "plugins/ZToJPsiUpsilonPhotonSignalAndBackgroundFit2D.C"
#include "plugins/HToJPsiUpsilonPhotonSignalAndBackgroundFit2D.C"
#include "../../HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"


// #include "plugins/getEffSigma.h"

#include "plugins/json.hpp"
using json = nlohmann::json;

// #ifdef __CINT__
// #pragma link C++ class RooDCBShape+;
// #endif

using namespace RooFit;
using namespace std;



json effSigmaJSON2D;
json signalMeanSigmaJSON2D;
json fitParameters;




void runZFit2D (json * effSigmaJSON2D, json * signalMeanSigmaJSON2D, string analysisBranch, string quarkoniaState, string selCategory, json * fitPar = &fitParameters) {

	zFitParams signalFitParams;

	// dummy values
	signalFitParams.mean_mHZ = 91.1876;
	signalFitParams.sigma_mHZ = 2.;
	signalFitParams.alpha1_mHZ = 3.;
	signalFitParams.n1_mHZ = 0.5;
	signalFitParams.alpha2_mHZ = 3.;
	signalFitParams.n2_mHZ = 0.5;

	signalFitParams.mean_mMuMNU = 91.1876;
	signalFitParams.sigma_mMuMNU = 2.;
	signalFitParams.alpha1_mMuMNU = 3.;
	signalFitParams.n1_mMuMNU = 0.5;
	signalFitParams.alpha2_mMuMNU = 3.;
	signalFitParams.n2_mMuMNU = 0.5;


  	signalFitParams =  ZToJPsiUpsilonPhotonSignalAndBackgroundFit2D(effSigmaJSON2D, signalMeanSigmaJSON2D, analysisBranch, quarkoniaState, selCategory, signalFitParams, fitPar);

	// cout << "####################################################" << endl;
	zFitParams fpTemp = signalFitParams;
	// cout << "mean_dcb: " << fpTemp.mean_dcb << endl;
	// cout << "sigma_dcb: " << fpTemp.sigma_dcb << endl;
	// cout << "alpha1: " << fpTemp.alpha1 << endl;
	// cout << "n1: " << fpTemp.n1 << endl;
	// cout << "alpha2: " << fpTemp.alpha2 << endl;
	// cout << "n2: " << fpTemp.n2 << endl;
	// cout << "####################################################" << endl;

	std::vector< std::future< zFitParams > > futureFits;
	vector<string> shapeSystMask =  {
						// "default",
						// "statRocCorError_UP",
						// "statRocCorError_DOWN",
						// "refRocCorError_UP",
						// "refRocCorError_DOWN",
						// "profMassRocCorError_UP",
						// "profMassRocCorError_DOWN",
						// "fitMassRocCorError_UP",
						// "fitMassRocCorError_DOWN",
						"RocCorError_UP",
						"RocCorError_DOWN",
						"phoScale_stat_UP",
						"phoScale_stat_DOWN",
						"phoScale_syst_UP",
						"phoScale_syst_DOWN",
						"phoScale_gain_UP",
						"phoScale_gain_DOWN",
						"phoResol_rho_UP",
						"phoResol_rho_DOWN",
						"phoResol_phi_UP",
						"phoResol_phi_DOWN",
					};
	auto itShapeSystMask = shapeSystMask.begin();
	while(itShapeSystMask != shapeSystMask.end()) {
		futureFits.push_back(std::async(ZToJPsiUpsilonPhotonSignalAndBackgroundFit2D, effSigmaJSON2D, signalMeanSigmaJSON2D, analysisBranch, quarkoniaState, selCategory, signalFitParams, fitPar, true, false,  *itShapeSystMask));
		futureFits.push_back(std::async(ZToJPsiUpsilonPhotonSignalAndBackgroundFit2D, effSigmaJSON2D, signalMeanSigmaJSON2D, analysisBranch, quarkoniaState, selCategory, signalFitParams, fitPar, false, true,  *itShapeSystMask));
		itShapeSystMask++;
	}
	for(auto &e : futureFits) {
		// cout << "####################################################" << endl;
		zFitParams fpTemp = e.get();
		// cout << "mean_dcb: " << fpTemp.mean_dcb << endl;
		// cout << "sigma_dcb: " << fpTemp.sigma_dcb << endl;
		// cout << "alpha1: " << fpTemp.alpha1 << endl;
		// cout << "n1: " << fpTemp.n1 << endl;
		// cout << "alpha2: " << fpTemp.alpha2 << endl;
	 // 	cout << "n2: " << fpTemp.n2 << endl;
		// cout << "####################################################" << endl;
	}	
}


void runHFit2D (json * effSigmaJSON2D, json * signalMeanSigmaJSON2D, string analysisBranch, string quarkoniaState, string selCategory, json * fitPar = &fitParameters) {

	hFitParams signalFitParams;

	// dummy values
	signalFitParams.mean_Higgs_mHZ = 125.0;
	signalFitParams.sigma_cb_mHZ = 2.;
	signalFitParams.n_mHZ = 0.5;
	signalFitParams.alpha_cb_mHZ = 3.0;
	signalFitParams.sigma_gauss_mHZ = 2.;
	signalFitParams.cb_fraction_mHZ = 0.9;

	// signalFitParams.mean_mHZ = 91.1876;
	// signalFitParams.sigma_mHZ = 2.;
	// signalFitParams.alpha1_mHZ = 3.;
	// signalFitParams.n1_mHZ = 0.5;
	// signalFitParams.alpha2_mHZ = 3.;
	// signalFitParams.n2_mHZ = 0.5;
	
	signalFitParams.mean_mMuMNU = 91.1876;
	signalFitParams.sigma_mMuMNU = 2.;
	signalFitParams.alpha1_mMuMNU = 3.;
	signalFitParams.n1_mMuMNU = 0.5;
	signalFitParams.alpha2_mMuMNU = 3.;
	signalFitParams.n2_mMuMNU = 0.5;

  	signalFitParams =  HToJPsiUpsilonPhotonSignalAndBackgroundFit2D(effSigmaJSON2D, signalMeanSigmaJSON2D, analysisBranch, quarkoniaState, selCategory, signalFitParams, fitPar);

	// // cout << "####################################################" << endl;
	// hFitParams fpTemp1 = signalFitParams;
	// // cout << "mean_Higgs: " << fpTemp1.mean_Higgs << endl;
	// // cout << "sigma_cb: " << fpTemp1.sigma_cb << endl;
	// // cout << "n: " << fpTemp1.n << endl;
	// // cout << "alpha_cb: " << fpTemp1.alpha_cb << endl;
	// // cout << "sigma_gauss: " << fpTemp1.sigma_gauss << endl;
	// // cout << "cb_fraction: " << fpTemp1.cb_fraction << endl;
	// // cout << "####################################################" << endl;

	// std::vector< std::future< hFitParams > > futureFits;
	// vector<string> shapeSystMask =  {
	// 					// "default",
	// 					// "statRocCorError_UP",
	// 					// "statRocCorError_DOWN",
	// 					// "refRocCorError_UP",
	// 					// "refRocCorError_DOWN",
	// 					// "profMassRocCorError_UP",
	// 					// "profMassRocCorError_DOWN",
	// 					// "fitMassRocCorError_UP",
	// 					// "fitMassRocCorError_DOWN",
	// 					"RocCorError_UP",
	// 					"RocCorError_DOWN",
	// 					"phoScale_stat_UP",
	// 					"phoScale_stat_DOWN",
	// 					"phoScale_syst_UP",
	// 					"phoScale_syst_DOWN",
	// 					"phoScale_gain_UP",
	// 					"phoScale_gain_DOWN",
	// 					"phoResol_rho_UP",
	// 					"phoResol_rho_DOWN",
	// 					"phoResol_phi_UP",
	// 					"phoResol_phi_DOWN",
	// 				};
	// auto itShapeSystMask = shapeSystMask.begin();
	// while(itShapeSystMask != shapeSystMask.end()) {
	// 	futureFits.push_back(std::async(HToJPsiUpsilonPhotonSignalAndBackgroundFit2D, effSigmaJSON2D, signalMeanSigmaJSON2D, analysisBranch, quarkoniaState, selCategory, signalFitParams, fitPar, true, false,  *itShapeSystMask));
	// 	futureFits.push_back(std::async(HToJPsiUpsilonPhotonSignalAndBackgroundFit2D, effSigmaJSON2D, signalMeanSigmaJSON2D, analysisBranch, quarkoniaState, selCategory, signalFitParams, fitPar, false, true,  *itShapeSystMask));
	// 	itShapeSystMask++;
	// }
	// for(auto &e : futureFits) {
	// 	// cout << "####################################################" << endl;
	// 	hFitParams fpTemp = e.get();
	// 	// cout << "mean_Higgs: " << fpTemp.mean_Higgs << endl;
	// 	// cout << "sigma_cb: " << fpTemp.sigma_cb << endl;
	// 	// cout << "n: " << fpTemp.n << endl;
	// 	// cout << "alpha_cb: " << fpTemp.alpha_cb << endl;
	// 	// cout << "sigma_gauss: " << fpTemp.sigma_gauss << endl;
	// 	// cout << "cb_fraction: " << fpTemp.cb_fraction << endl;
	// 	// cout << "####################################################" << endl;
	// }	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DOES THE ACTUAL FITTING
  void fitter_HZtoUpsilonPhoton2DFit()  
  {
  	setTDRStyle();
	// system("rm -fr fitPlotFiles; mkdir fitPlotFiles");
  	system("rm -fr fitPlotFiles/*");
  	system("rm -fr fitPlotFiles2D/*");

	// fitter
	// Z
	// // runZFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "", "Cat0");
	// runZFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "1S", "Cat0");
	// runZFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "2S", "Cat0");
	// runZFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "3S", "Cat0");

	// // runZFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "", "Cat1");
	// runZFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "1S", "Cat1");
	// runZFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "2S", "Cat1");
	// runZFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "3S", "Cat1");

	// // runZFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "", "Cat2");
	// runZFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "1S", "Cat2");
	// runZFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "2S", "Cat2");
	// runZFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "3S", "Cat2");

	// // runZFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "", "Cat3");
	// runZFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "1S", "Cat3");
	// runZFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "2S", "Cat3");
	// runZFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "3S", "Cat3");

	// Higgs
	// runHFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "", "Cat0");
	runHFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "1S", "Cat0");
	runHFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "2S", "Cat0");
	runHFit2D(&effSigmaJSON2D, &signalMeanSigmaJSON2D, "Upsilon", "3S", "Cat0");
	

  	// cout << "\n\n-----------------> [BEGIN] effSigmaJSON2D\n" << endl;
  	// cout << effSigmaJSON2D.dump(4) << endl;
  	// cout << "\n-----------------> [END] effSigmaJSON2D\n" << endl;

  	// std::ofstream effSigmaJSON2DFile("fitPlotFiles/effSigmaJSON2D.json");
    //  effSigmaJSON2DFile << effSigmaJSON2D.dump(4);


  	cout << "\n\n-----------------> [BEGIN] signalMeanSigmaJSON2D\n" << endl;
  	cout << signalMeanSigmaJSON2D.dump(4) << endl;
  	cout << "\n-----------------> [END] signalMeanSigmaJSON2D\n" << endl;

  	std::ofstream signalMeanSigmaJSON2DFile("fitPlotFiles2D/signalMeanSigmaJSON2D.json");
    signalMeanSigmaJSON2DFile << signalMeanSigmaJSON2D.dump(4);

    std::ofstream fitParametersFile("fitPlotFiles2D/fitParameters_MC.json");
    fitParametersFile << fitParameters.dump(4);


} //end




