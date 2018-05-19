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
#include "plugins/ZToJPsiUpsilonPhotonSignalAndBackgroundFit.C"
#include "plugins/HToJPsiUpsilonPhotonSignalAndBackgroundFit.C"
#include "../../HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"


// #include "plugins/getEffSigma.h"

#include "plugins/json.hpp"
using json = nlohmann::json;

// #ifdef __CINT__
// #pragma link C++ class RooDCBShape+;
// #endif

using namespace RooFit;
using namespace std;



json effSigmaJSON;
json signalMeanSigmaJSON;



void runZFit (json * effSigmaJSON, json * signalMeanSigmaJSON, string analysisBranch, string quarkoniaState, string selCategory) {

	zFitParams signalFitParams;

	signalFitParams.mean_dcb = 91.1876;
	signalFitParams.sigma_dcb = 2.;
	signalFitParams.alpha1 = 3.;
	signalFitParams.n1 = 0.5;
	signalFitParams.alpha2 = 3.;
	signalFitParams.n2 = 0.5;

  	signalFitParams =  ZToJPsiUpsilonPhotonSignalAndBackgroundFit(effSigmaJSON, signalMeanSigmaJSON, analysisBranch, quarkoniaState, selCategory, signalFitParams);

	cout << "####################################################" << endl;
	zFitParams fpTemp = signalFitParams;
	cout << "mean_dcb: " << fpTemp.mean_dcb << endl;
	cout << "sigma_dcb: " << fpTemp.sigma_dcb << endl;
	cout << "alpha1: " << fpTemp.alpha1 << endl;
	cout << "n1: " << fpTemp.n1 << endl;
	cout << "alpha2: " << fpTemp.alpha2 << endl;
	cout << "n2: " << fpTemp.n2 << endl;
	cout << "####################################################" << endl;

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
		futureFits.push_back(std::async(ZToJPsiUpsilonPhotonSignalAndBackgroundFit, effSigmaJSON, signalMeanSigmaJSON, analysisBranch, quarkoniaState, selCategory, signalFitParams, true, false,  *itShapeSystMask));
		futureFits.push_back(std::async(ZToJPsiUpsilonPhotonSignalAndBackgroundFit, effSigmaJSON, signalMeanSigmaJSON, analysisBranch, quarkoniaState, selCategory, signalFitParams, false, true,  *itShapeSystMask));
		itShapeSystMask++;
	}
	for(auto &e : futureFits) {
		cout << "####################################################" << endl;
		zFitParams fpTemp = e.get();
		cout << "mean_dcb: " << fpTemp.mean_dcb << endl;
		cout << "sigma_dcb: " << fpTemp.sigma_dcb << endl;
		cout << "alpha1: " << fpTemp.alpha1 << endl;
		cout << "n1: " << fpTemp.n1 << endl;
		cout << "alpha2: " << fpTemp.alpha2 << endl;
	 	cout << "n2: " << fpTemp.n2 << endl;
		cout << "####################################################" << endl;
	}	
}


void runHFit (json * effSigmaJSON, json * signalMeanSigmaJSON, string analysisBranch, string quarkoniaState, string selCategory) {

	hFitParams signalFitParams;

	signalFitParams.mean_Higgs = 125.0;
	signalFitParams.sigma_cb = 2.;
	signalFitParams.n = 0.5;
	signalFitParams.alpha_cb = 3.0;
	signalFitParams.sigma_gauss = 2.;
	signalFitParams.cb_fraction = 0.9;

  	signalFitParams =  HToJPsiUpsilonPhotonSignalAndBackgroundFit(effSigmaJSON, signalMeanSigmaJSON, analysisBranch, quarkoniaState, selCategory, signalFitParams);

	cout << "####################################################" << endl;
	hFitParams fpTemp1 = signalFitParams;
	cout << "mean_Higgs: " << fpTemp1.mean_Higgs << endl;
	cout << "sigma_cb: " << fpTemp1.sigma_cb << endl;
	cout << "n: " << fpTemp1.n << endl;
	cout << "alpha_cb: " << fpTemp1.alpha_cb << endl;
	cout << "sigma_gauss: " << fpTemp1.sigma_gauss << endl;
	cout << "cb_fraction: " << fpTemp1.cb_fraction << endl;
	cout << "####################################################" << endl;

	std::vector< std::future< hFitParams > > futureFits;
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
		futureFits.push_back(std::async(HToJPsiUpsilonPhotonSignalAndBackgroundFit, effSigmaJSON, signalMeanSigmaJSON, analysisBranch, quarkoniaState, selCategory, signalFitParams, true, false,  *itShapeSystMask));
		futureFits.push_back(std::async(HToJPsiUpsilonPhotonSignalAndBackgroundFit, effSigmaJSON, signalMeanSigmaJSON, analysisBranch, quarkoniaState, selCategory, signalFitParams, false, true,  *itShapeSystMask));
		itShapeSystMask++;
	}
	for(auto &e : futureFits) {
		cout << "####################################################" << endl;
		hFitParams fpTemp = e.get();
		cout << "mean_Higgs: " << fpTemp.mean_Higgs << endl;
		cout << "sigma_cb: " << fpTemp.sigma_cb << endl;
		cout << "n: " << fpTemp.n << endl;
		cout << "alpha_cb: " << fpTemp.alpha_cb << endl;
		cout << "sigma_gauss: " << fpTemp.sigma_gauss << endl;
		cout << "cb_fraction: " << fpTemp.cb_fraction << endl;
		cout << "####################################################" << endl;
	}	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DOES THE ACTUAL FITTING
  void fitter_HZtoUpsilonPhoton()  
  {
  	setTDRStyle();
	// system("rm -fr fitPlotFiles; mkdir fitPlotFiles");
  	system("rm -fr fitPlotFiles/*");

	// fitter
	runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "JPsi", "", "Cat0");
	// runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "", "Cat0");
	// runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "1S", "Cat0");
	// runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "2S", "Cat0");
	// runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "3S", "Cat0");

	runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "JPsi", "", "Cat1");
	// runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "", "Cat1");
	// runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "1S", "Cat1");
	// runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "2S", "Cat1");
	// runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "3S", "Cat1");

	runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "JPsi", "", "Cat2");
	// runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "", "Cat2");
	// runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "1S", "Cat2");
	// runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "2S", "Cat2");
	// runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "3S", "Cat2");

	runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "JPsi", "", "Cat3");
	// runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "", "Cat3");
	// runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "1S", "Cat3");
	// runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "2S", "Cat3");
	// runZFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "3S", "Cat3");

	runHFit(&effSigmaJSON, &signalMeanSigmaJSON, "JPsi", "", "Cat0");
	// runHFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "", "Cat0");
	// runHFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "1S", "Cat0");
	// runHFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "2S", "Cat0");
	// runHFit(&effSigmaJSON, &signalMeanSigmaJSON, "Upsilon", "3S", "Cat0");




  	cout << "\n\n-----------------> [BEGIN] effSigmaJSON\n" << endl;
  	cout << effSigmaJSON.dump(4) << endl;
  	cout << "\n-----------------> [END] effSigmaJSON\n" << endl;

  	std::ofstream effSigmaJSONFile("fitPlotFiles/effSigmaJSON.json");
    effSigmaJSONFile << effSigmaJSON.dump(4);





  	cout << "\n\n-----------------> [BEGIN] signalMeanSigmaJSON\n" << endl;
  	cout << signalMeanSigmaJSON.dump(4) << endl;
  	cout << "\n-----------------> [END] signalMeanSigmaJSON\n" << endl;

  	std::ofstream signalMeanSigmaJSONFile("fitPlotFiles/signalMeanSigmaJSON.json");
    signalMeanSigmaJSONFile << signalMeanSigmaJSON.dump(4);



} //end




