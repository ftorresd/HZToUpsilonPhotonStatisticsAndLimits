#include <iostream>
#include <fstream>
#include <vector>
#include <string>
// #include <regex>
#include <map>

#include "boost/program_options.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
// #include <boost/algorithm/string/iter_find.hpp>
// #include <boost/algorithm/string/finder.hpp>

#include "TFile.h"
#include "TMath.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooHist.h"
#include "RooAbsData.h"
#include "RooAbsPdf.h"
#include "RooArgSet.h"
#include "RooFitResult.h"
#include "RooMinuit.h"
#include "RooMinimizer.h"
#include "RooMsgService.h"
#include "RooDataHist.h"
#include "RooExtendPdf.h"
#include "TRandom3.h"
#include "TLatex.h"
#include "TMacro.h"
#include "TH1F.h"
#include "TH1I.h"
#include "TArrow.h"
#include "TKey.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooGaussian.h"



#include "RooCategory.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooMultiPdf.h"

#include "HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"
#include "RooChebychev.h"
#include "RooCBShape.h"
#include "RooBernstein.h"



#include "../interface/PdfModelBuilder.h"
#include <Math/PdfFuncMathCore.h>
#include <Math/ProbFunc.h>
#include <iomanip>
#include "boost/program_options.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"
#include "boost/algorithm/string/predicate.hpp"

#include "../tdrStyle/tdrstyle.C"
#include "../tdrStyle/CMS_lumi.C"


#include "../../fits/plugins/json.hpp"
using json = nlohmann::json;

#include <boost/regex.hpp>

using namespace std;
using namespace RooFit;
using namespace boost;

namespace po = program_options;





bool hasEnding (std::string const &fullString, std::string const &ending) {
  if (fullString.length() >= ending.length()) {
    return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
  } else {
    return false;
  }
}

bool BLIND = true;
// bool BLIND = false;
bool runFtestCheckWithToys=false;
int mgg_low = 100;
int mgg_mid_low = 115;
int mgg_mid_high = 135;
int mgg_high = 150;
int nBinsForMass = 4*(mgg_high-mgg_low);

RooRealVar *intLumi_ = new RooRealVar("IntLumi","hacked int lumi", 36.86);

TRandom3 *RandomGen = new TRandom3();


// NO PEAKING BCKG INSIDE THE SIGNAL REGION
// RooAbsPdf* getPdf(PdfModelBuilder &pdfsModel, string type, int order, const char* ext=""){

//   if (type=="Bernstein") return pdfsModel.getBernstein(Form("%s_bern%d",ext,order),order);
//   else if (type=="Chebychev") return pdfsModel.getChebychev(Form("%s_cheb%d",ext,order),order);
//   else if (type=="Exponential") return pdfsModel.getExponentialSingle(Form("%s_exp%d",ext,order),order);
//   else if (type=="PowerLaw") return pdfsModel.getPowerLawSingle(Form("%s_pow%d",ext,order),order);
//   else if (type=="Laurent") return pdfsModel.getLaurentSeries(Form("%s_lau%d",ext,order),order);
//   // string order_sufix = "th ";
//   // if (order == 1) order_sufix = "st";
//   // if (order == 2) order_sufix = "nd";
//   // if (order == 3) order_sufix = "rd";

//   // if (type=="Chebychev") return pdfsModel.getChebychev(("Chebychev "+to_string(order)+order_sufix).c_str(),order);
//   // else if (type=="Exponential") return pdfsModel.getExponentialSingle(("Exponential "+to_string(order)+order_sufix).c_str(),order);
//   // else if (type=="PowerLaw") return pdfsModel.getPowerLawSingle(("Power Law "+to_string(order)+order_sufix).c_str(),order);
//   // else if (type=="Laurent") return pdfsModel.getLaurentSeries(("Laurent "+to_string(order)+order_sufix).c_str(),order);
//   else {
//     cerr << "[ERROR] -- getPdf() -- type " << type << " not recognised." << endl;
//     return NULL;
//   }
// }

// WITH PEAKING BCKG INSIDE THE SIGNAL REGION

RooAbsPdf* getPdf(string analysisBranch, string HZToUpsilonPhotonCat_, RooRealVar * obsVarHZ, RooRealVar * obsVarMuMu, PdfModelBuilder &pdfsModel, string type, int order, const char* ext=""){
  // get fit parameters
  std::ifstream fitParamsFile("inputData/fitPlotFiles2D/fitParameters_MC.json");
  json fitParams;
  fitParamsFile >> fitParams;

  // get fit parameters
  std::ifstream upsilonFitParamsFile("inputData/fitPlotFiles2D/UpsilonControlSample/upsilonFitParams.json");
  json upsilonFitParams;
  upsilonFitParamsFile >> upsilonFitParams;


  string analysisBoson = analysisBranch.substr(0,1);

  ////////////////////////////////////////
  // COMBINATORIAL BACKGROUND - mHZ
  RooAbsPdf * combBckg_mHZ;
  if (type=="Bernstein") combBckg_mHZ = pdfsModel.getBernstein(Form("%s_bern%d",ext,order),order);
  else if (type=="Chebychev") combBckg_mHZ = pdfsModel.getChebychev(Form("%s_cheb%d",ext,order),order);
  else if (type=="Exponential") combBckg_mHZ = pdfsModel.getExponentialSingle(Form("%s_exp%d",ext,order),order);
  else if (type=="PowerLaw") combBckg_mHZ = pdfsModel.getPowerLawSingle(Form("%s_pow%d",ext,order),order);
  else if (type=="Laurent") combBckg_mHZ = pdfsModel.getLaurentSeries(Form("%s_lau%d",ext,order),order);
  else {
    cerr << "[ERROR] -- getPdf() -- type " << type << " not recognised." << endl;
    return NULL;
  }
  
  if (!combBckg_mHZ) return NULL;

  ////////////////////////////////////////
  // COMBINATORIAL BACKGROUND - mMuMu
  // mean 
  double M1S = 9460.30/1000.;   //upsilon 1S pgd mass value
  double M2S = 10023.26/1000.;  //upsilon 2S pgd mass value
  double M3S = 10355.2/1000.; //upsilon 3S pgd mass value 

  // RooRealVar * mean_m = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_mean_m").c_str(),"#Upsilon mean",M1S,M1S-0.5,M1S+0.5,"GeV");
  RooRealVar * mean_m = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_mean_m").c_str(),"#Upsilon mean",M1S);
  // mean_m->setVal(fitParams[analysisBoson+"ToUpsilon1SPhoton"][HZToUpsilonPhotonCat_]["signal"]["mean_mMuMNU_"+HZToUpsilonPhotonCat_]);
  mean_m->setConstant(kTRUE);

  RooRealVar * shift21 = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_shift2").c_str(),"mass diff #Upsilon(1,2S)",M2S-M1S);
  RooRealVar * shift31 = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_shift3").c_str(),"mass diff #Upsilon(1,3S)",M3S-M1S);

  RooRealVar * mscale = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_mscale").c_str(),"mass scale factor", 1.0);
  mscale->setVal(upsilonFitParams["ZToUpsilonPhoton"]["Cat0"]["mscale"]);
  mscale->setConstant(kTRUE);  // the def. parameter value is fixed in the fit 

  RooFormulaVar * mean1S = new RooFormulaVar(((string)(combBckg_mHZ->GetName())+"_mean1S").c_str(),"@0*@1", RooArgList(*mean_m, *mscale));
  RooFormulaVar * mean2S = new RooFormulaVar(((string)(combBckg_mHZ->GetName())+"_mean2S").c_str(),"@0*@1+@2", RooArgList(*mean_m,*mscale,*shift21));
  RooFormulaVar * mean3S = new RooFormulaVar(((string)(combBckg_mHZ->GetName())+"_mean3S").c_str(),"@0*@1+@2", RooArgList(*mean_m,*mscale,*shift31));

  // sigma
  // RooRealVar * sigmaUpsilon = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_sigmaUpsilon").c_str(),"sigmaUpsilon",1,0.0,1);
  RooRealVar * sigmaUpsilon = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_sigmaUpsilon").c_str(),"sigmaUpsilon",0.06);
  // sigmaUpsilon->setVal(fitParams[analysisBoson+"ToUpsilon1SPhoton"][HZToUpsilonPhotonCat_]["signal"]["sigma_mMuMNU_"+HZToUpsilonPhotonCat_]);
  sigmaUpsilon->setVal(upsilonFitParams["ZToUpsilonPhoton"]["Cat0"]["sigmaUpsilon"]);
  sigmaUpsilon->setConstant(kTRUE);

  RooFormulaVar * sigma1S = new RooFormulaVar(((string)(combBckg_mHZ->GetName())+"_sigma1S").c_str(),"@0",RooArgList(*sigmaUpsilon));
  RooFormulaVar * sigma2S = new RooFormulaVar(((string)(combBckg_mHZ->GetName())+"_sigma2S").c_str(),"@0*@1",RooArgList(*sigmaUpsilon,RooConst(M2S/M1S)));
  RooFormulaVar * sigma3S = new RooFormulaVar(((string)(combBckg_mHZ->GetName())+"_sigma3S").c_str(),"@0*@1",RooArgList(*sigmaUpsilon,RooConst(M3S/M1S)));


  RooRealVar * alpha1_1S = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_alpha1_1S").c_str(),"tail shift1",1.5);
  alpha1_1S->setVal(fitParams[analysisBoson+"ToUpsilon1SPhoton"][HZToUpsilonPhotonCat_]["signal"]["alpha1_mMuMNU"]);
  alpha1_1S->setConstant(kTRUE);

  RooRealVar * alpha1_2S = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_alpha1_2S").c_str(),"tail shift1",1.5);
  alpha1_2S->setVal(fitParams[analysisBoson+"ToUpsilon2SPhoton"][HZToUpsilonPhotonCat_]["signal"]["alpha1_mMuMNU"]);
  alpha1_2S->setConstant(kTRUE);

  RooRealVar * alpha1_3S = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_alpha1_3S").c_str(),"tail shift1",1.5);
  alpha1_3S->setVal(fitParams[analysisBoson+"ToUpsilon3SPhoton"][HZToUpsilonPhotonCat_]["signal"]["alpha1_mMuMNU"]);
  alpha1_3S->setConstant(kTRUE);

  RooRealVar * alpha2_1S = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_alpha2_1S").c_str(),"tail shift2",1.5);
  alpha2_1S->setVal(fitParams[analysisBoson+"ToUpsilon1SPhoton"][HZToUpsilonPhotonCat_]["signal"]["alpha2_mMuMNU"]);
  alpha2_1S->setConstant(kTRUE);

  RooRealVar * alpha2_2S = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_alpha2_2S").c_str(),"tail shift2",1.5);
  alpha2_2S->setVal(fitParams[analysisBoson+"ToUpsilon2SPhoton"][HZToUpsilonPhotonCat_]["signal"]["alpha2_mMuMNU"]);
  alpha2_2S->setConstant(kTRUE);
  
  RooRealVar * alpha2_3S = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_alpha2_3S").c_str(),"tail shift2",1.5);
  alpha2_3S->setVal(fitParams[analysisBoson+"ToUpsilon3SPhoton"][HZToUpsilonPhotonCat_]["signal"]["alpha2_mMuMNU"]);
  alpha2_3S->setConstant(kTRUE);

  RooRealVar * npow1_1S = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_npow1_1S").c_str(),"power order1",2);
  npow1_1S->setVal(fitParams[analysisBoson+"ToUpsilon1SPhoton"][HZToUpsilonPhotonCat_]["signal"]["n1_mMuMNU"]);
  npow1_1S->setConstant(kTRUE);
 
  RooRealVar * npow1_2S = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_npow1_2S").c_str(),"power order1",2);
  npow1_2S->setVal(fitParams[analysisBoson+"ToUpsilon2SPhoton"][HZToUpsilonPhotonCat_]["signal"]["n1_mMuMNU"]);
  npow1_2S->setConstant(kTRUE);
 
  RooRealVar * npow1_3S = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_npow1_3S").c_str(),"power order1",2);
  npow1_3S->setVal(fitParams[analysisBoson+"ToUpsilon3SPhoton"][HZToUpsilonPhotonCat_]["signal"]["n1_mMuMNU"]);
  npow1_3S->setConstant(kTRUE);

  RooRealVar * npow2_1S = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_npow2_1S").c_str(),"power order2",2);
  npow2_1S->setVal(fitParams[analysisBoson+"ToUpsilon1SPhoton"][HZToUpsilonPhotonCat_]["signal"]["n2_mMuMNU"]);
  npow2_1S->setConstant(kTRUE);
 
  RooRealVar * npow2_2S = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_npow2_2S").c_str(),"power order2",2);
  npow2_2S->setVal(fitParams[analysisBoson+"ToUpsilon2SPhoton"][HZToUpsilonPhotonCat_]["signal"]["n2_mMuMNU"]);
  npow2_2S->setConstant(kTRUE);
 
  RooRealVar * npow2_3S = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_npow2_3S").c_str(),"power order2",2);
  npow2_3S->setVal(fitParams[analysisBoson+"ToUpsilon3SPhoton"][HZToUpsilonPhotonCat_]["signal"]["n2_mMuMNU"]);
  npow2_3S->setConstant(kTRUE);
 

  // npow->setConstant(kTRUE);
  // alpha->setConstant(kTRUE);
  // RooRealVar alpha("alpha","tail shift",2,0,10);
  // RooRealVar npow("npow","power order",2,0,10);
  
  /// relative fraction of the two peak components 
  // RooRealVar sigmaFraction("sigmaFraction","Sigma Fraction",0.3,0.,1.);

  /// Upsilon 1S
  // RooGaussian gauss1S1("gauss1S1","1S Gaussian_1",obsVarMuMu,mean1S,sigma1);
  RooGaussian* bckg1S = new RooGaussian(((string)(combBckg_mHZ->GetName())+"_bckg1S").c_str(), "bckg1S",*obsVarMuMu,*mean1S,*sigma1S); 
  // RooDoubleCB* bckg1S = new RooDoubleCB(((string)(combBckg_mHZ->GetName())+"_bckg1S").c_str(), "bckg1S",*obsVarMuMu,*mean1S,*sigma1S,*alpha1_1S,*npow1_1S,*alpha2_1S,*npow2_1S); 
  // RooCBShape * bckg1S = new RooCBShape(((string)(combBckg_mHZ->GetName())+"_bckg1S").c_str(), "bckg1S",*obsVarMuMu,*mean1S,*sigma1S,*alpha,*npow); 
  // RooAddPdf sig1S("sig1S","1S mass pdf",RooArgList(gauss1S1,gauss1S2),sigmaFraction);
  /// Upsilon 2S
  // RooGaussian gauss2S1("gauss2S1","2S Gaussian_1",obsVarMuMu,mean2S,sigma1);
  RooGaussian * bckg2S = new RooGaussian(((string)(combBckg_mHZ->GetName())+"_bckg2S").c_str(), "bckg2S", *obsVarMuMu,*mean2S,*sigma2S); 
  // RooDoubleCB * bckg2S = new RooDoubleCB(((string)(combBckg_mHZ->GetName())+"_bckg2S").c_str(), "bckg2S", *obsVarMuMu,*mean2S,*sigma2S,*alpha1_2S,*npow1_2S,*alpha2_2S,*npow2_2S); 
  // RooCBShape * bckg2S = new RooCBShape(((string)(combBckg_mHZ->GetName())+"_bckg2S").c_str(), "bckg2S", *obsVarMuMu,*mean2S,*sigma2S,*alpha,*npow); 
  // RooAddPdf sig2S("sig2S","2S mass pdf",RooArgList(gauss2S1,gauss2S2),sigmaFraction);

  /// Upsilon 3S
  // RooGaussian gauss3S1("gauss3S1","3S Gaussian_1",obsVarMuMu,mean3S,sigma1);
  RooGaussian * bckg3S = new RooGaussian(((string)(combBckg_mHZ->GetName())+"_bckg3S").c_str(),"bckg3S",*obsVarMuMu,*mean3S,*sigma3S);
  // RooDoubleCB * bckg3S = new RooDoubleCB(((string)(combBckg_mHZ->GetName())+"_bckg3S").c_str(),"bckg3S",*obsVarMuMu,*mean3S,*sigma3S,*alpha1_3S,*npow1_3S,*alpha2_3S,*npow2_3S);
  // RooCBShape * bckg3S = new RooCBShape(((string)(combBckg_mHZ->GetName())+"_bckg3S").c_str(),"bckg3S",*obsVarMuMu,*mean3S,*sigma3S,*alpha,*npow);
  // RooAddPdf sig3S("sig3S","3S mass pdf",RooArgList(gauss3S1,gauss3S2),sigmaFraction);


  /// Background
  RooRealVar *bkg_a1  = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_bkg_a1").c_str(), "background a1", 0, -1, 1);
  // RooRealVar *bkg_a2  = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_bkg_a2").c_str(), "background a2", 0, -1, 1);
  // RooAbsPdf  *mBkg  = new RooChebychev(((string)(combBckg_mHZ->GetName())+"_mBkg").c_str(),"linear background",*obsVarMuMu, RooArgList(*bkg_a1,*bkg_a2));
  RooAbsPdf  *mBkg  = new RooChebychev(((string)(combBckg_mHZ->GetName())+"_mBkg").c_str(),"linear background",*obsVarMuMu, RooArgList(*bkg_a1));


  RooRealVar * fbckg1S = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_fbckg1S").c_str(), "fbckg1S",  0.1, 0.0, 1.0);
  RooRealVar * fbckg2S = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_fbckg2S").c_str(), "fbckg1S",  0.1, 0.0, 1.0);
  RooRealVar * fbckg3S = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_fbckg3S").c_str(), "fbckg1S",  0.1, 0.0, 1.0);
  RooAbsPdf *  combBckg_mMuMu = new RooAddPdf(((string)(combBckg_mHZ->GetName())+"_combBckg_mMuMu").c_str(),"combBckg_mMuMu",RooArgList(*bckg1S,*bckg2S,*bckg3S,*mBkg),RooArgList(*fbckg1S,*fbckg2S,*fbckg3S),kTRUE);


  ////////////////////////////////////////
  // COMBINATORIAL - total
  RooProdPdf * combTotal = new RooProdPdf(((string)(combBckg_mHZ->GetName())+"_combTotal").c_str(), "CombTotal  ", RooArgSet(*combBckg_mHZ, *combBckg_mMuMu));  

  ////////////////////////////////////////
  // PEAKING BACKGROUND - mHZ 
  RooRealVar * mean_dcbPeakingBackground = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_dcball_Mean").c_str(), "Mean" ,91.1876,70,120) ;
  // RooRealVar * mean_dcbPeakingBackground = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_dcball_Mean").c_str(), "Mean" ,91.1876,70,120) ;
  if (analysisBoson == "Z") mean_dcbPeakingBackground->setVal(fitParams[analysisBoson+"ToUpsilon1SPhoton"][HZToUpsilonPhotonCat_]["peakingBackground"]["mean_mHZ_PeakingBackground"]);
  mean_dcbPeakingBackground->setConstant(kTRUE);

  RooRealVar * sigma_dcbPeakingBackground = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_dcball_Sigma").c_str(), "Width" ,  2., 0.5, 4.) ;
  // RooRealVar * sigma_dcbPeakingBackground = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_dcball_Sigma").c_str(), "Width" ,  2.) ;
  if (analysisBoson == "Z") sigma_dcbPeakingBackground->setVal(fitParams[analysisBoson+"ToUpsilon1SPhoton"][HZToUpsilonPhotonCat_]["peakingBackground"]["sigma_mHZ_PeakingBackground"]);
  sigma_dcbPeakingBackground->setConstant(kTRUE);

  RooRealVar * n1PeakingBackground = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_dcball_n1").c_str(),"", 0.5, 0.1, 50.);//dCBPowerL
  // RooRealVar * n1PeakingBackground = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_dcball_n1").c_str(),"", 0.5);//dCBPowerL
  if (analysisBoson == "Z") n1PeakingBackground->setVal(fitParams[analysisBoson+"ToUpsilon1SPhoton"][HZToUpsilonPhotonCat_]["peakingBackground"]["n1_mHZ_PeakingBackground"]);
  n1PeakingBackground->setConstant(kTRUE);

  RooRealVar * n2PeakingBackground = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_dcball_n2").c_str(),"", 0.5, 0.1, 50.);//dCBPowerR
  // RooRealVar * n2PeakingBackground = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_dcball_n2").c_str(),"", 0.5);//dCBPowerR
  if (analysisBoson == "Z") n2PeakingBackground->setVal(fitParams[analysisBoson+"ToUpsilon1SPhoton"][HZToUpsilonPhotonCat_]["peakingBackground"]["n2_mHZ_PeakingBackground"]);
  n2PeakingBackground->setConstant(kTRUE);

  RooRealVar * alpha1PeakingBackground = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_dcball_alpha1").c_str(),"", 3., 0.0, 10.);//dCBCutL
  // RooRealVar * alpha1PeakingBackground = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_dcball_alpha1").c_str(),"", 3.);//dCBCutL
  if (analysisBoson == "Z") alpha1PeakingBackground->setVal(fitParams[analysisBoson+"ToUpsilon1SPhoton"][HZToUpsilonPhotonCat_]["peakingBackground"]["alpha1_mHZ_PeakingBackground"]);

  RooRealVar * alpha2PeakingBackground = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_dcball_alpha2").c_str(),"", 3., 0.0, 10.);//dCBCutR
  // RooRealVar * alpha2PeakingBackground = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_dcball_alpha2").c_str(),"", 3.);//dCBCutR
  if (analysisBoson == "Z") alpha2PeakingBackground->setVal(fitParams[analysisBoson+"ToUpsilon1SPhoton"][HZToUpsilonPhotonCat_]["peakingBackground"]["alpha2_mHZ_PeakingBackground"]);
  alpha2PeakingBackground->setConstant(kTRUE);

  RooDoubleCB * dcball_PeakingBackground = new RooDoubleCB(
    // "dcballPeakingBackground",  
    ((string)(combBckg_mHZ->GetName())+"_dcball").c_str(),  
    "double sided crystal ball", 
    *obsVarHZ, 
    *mean_dcbPeakingBackground,
    *sigma_dcbPeakingBackground,
    *alpha1PeakingBackground,
    *n1PeakingBackground,
    *alpha2PeakingBackground,
    *n2PeakingBackground
  );
  

  ////////////////////////////////////////
  // PEAKING BACKGROUND - mMuMU
  RooRealVar * p0 = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_p0_Bernstein_mMuMNU").c_str(), " ", 1.0); // coefficient of x^0 term
  p0->setConstant();
  // RooRealVar * p1 = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_p1_Bernstein_mMuMNU").c_str(), " ", 5, 0., 10); // coefficient of x^1 term
  RooRealVar * p1 = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_p1_Bernstein_mMuMNU").c_str(), " ", 5); // coefficient of x^1 term
  if (analysisBoson == "Z") p1->setVal(fitParams[analysisBoson+"ToUpsilon1SPhoton"][HZToUpsilonPhotonCat_]["peakingBackground"]["p1"]);
  p1->setConstant(kTRUE);
   
  RooArgList * argumentsList = new RooArgList(*p0, *p1);

  RooBernstein * bernstein_PeakingBackground = new RooBernstein(((string)(combBckg_mHZ->GetName())+"_bernstein_PeakingBackground").c_str(), " ", *obsVarMuMu, *argumentsList);

  ////////////////////////////////////////
  // PEAKING BACKGROUND - total
  RooProdPdf * peakTotal = new RooProdPdf(((string)(combBckg_mHZ->GetName())+"_peakTotal").c_str(), "peakTotal", RooArgSet(*dcball_PeakingBackground, *bernstein_PeakingBackground));  


  // total pdf
  RooAbsPdf * totalBckg;
  if (analysisBoson == "Z") {
    RooRealVar * combFraction = new RooRealVar(((string)(combBckg_mHZ->GetName())+"_combFraction").c_str(), "combFraction",  0.1, 0.0, 1.0);
    totalBckg = new RooAddPdf(((string)(combBckg_mHZ->GetName())+"_totalBckg").c_str(),"totalBckg_mHZ",RooArgList(*combTotal, *peakTotal), *combFraction); 
  } else {
    combTotal->SetName(((string)(combBckg_mHZ->GetName())+"_totalBckg").c_str());
    totalBckg = combTotal;
  }

  // totalBckg->Print();
  return totalBckg;
  
}

// fit function
void runFit(RooAbsPdf *pdf, RooDataSet *data, double *NLL, int *stat_t, int MaxTries){
  
  int ntries=0;
  RooArgSet *params_test = pdf->getParameters((const RooArgSet*)(0));
  //params_test->Print("v");
  int stat=1;
  double minnll=10e8;
  while (stat!=0){
    if (ntries>=MaxTries) break;
    
    // mass->setRange("unblindReg_1",mgg_low,mgg_mid_low);
    // mass->setRange("unblindReg_2",mgg_mid_high,mgg_high);
    // RooFitResult *fitTest = pdf->fitTo(*data,RooFit::Save(1)
    //  ,RooFit::Minimizer("Minuit2","minimize"),RooFit::SumW2Error(kTRUE), Range("unblindReg_1,unblindReg_2")); //FIXME
    
    cout << "----> Fitting..." << endl;
    RooFitResult *fitTest = pdf->fitTo(*data,RooFit::Save(1)
    ,RooFit::Minimizer("Minuit2","minimize"),RooFit::SumW2Error(kTRUE)); //FIXME
    cout << "----> Fitted..." << endl;
    stat = fitTest->status();
    minnll = fitTest->minNll();
    if (stat!=0) params_test->assignValueOnly(fitTest->randomizePars());
    ntries++;
  }
  *stat_t = stat;
  *NLL = minnll;
}

double getProbabilityFtest(double chi2, int ndof,RooAbsPdf *pdfNull, RooAbsPdf *pdfTest, RooRealVar *mass, RooRealVar *mass_MuMu, RooDataSet *data, std::string name){
  
  double prob_asym = TMath::Prob(chi2,ndof);
  if (!runFtestCheckWithToys) return prob_asym;
  
  int ndata = data->sumEntries();
  
  // fit the pdfs to the data and keep this fit Result (for randomizing)
  RooFitResult *fitNullData = pdfNull->fitTo(*data,RooFit::Save(1),RooFit::Strategy(1)
  ,RooFit::Minimizer("Minuit2","minimize"),RooFit::SumW2Error(kTRUE),RooFit::PrintLevel(-1)); //FIXME
  RooFitResult *fitTestData = pdfTest->fitTo(*data,RooFit::Save(1),RooFit::Strategy(1)
  ,RooFit::Minimizer("Minuit2","minimize"),RooFit::SumW2Error(kTRUE),RooFit::PrintLevel(-1)); //FIXME
  
  // Ok we want to check the distribution in toys then
  // Step 1, cache the parameters of each pdf so as not to upset anything
  RooArgSet *params_null = pdfNull->getParameters((const RooArgSet*)(0));
  RooArgSet preParams_null;
  params_null->snapshot(preParams_null);
  RooArgSet *params_test = pdfTest->getParameters((const RooArgSet*)(0));
  RooArgSet preParams_test;
  params_test->snapshot(preParams_test);
  
  int ntoys =5000;
  TCanvas *can = new TCanvas();
  can->SetLogy();
  TH1F toyhist(Form("toys_fTest_%s.pdf",pdfNull->GetName()),";Chi2;",60,-2,10);
  TH1I toyhistStatN(Form("Status_%s.pdf",pdfNull->GetName()),";FitStatus;",8,-4,4);
  TH1I toyhistStatT(Form("Status_%s.pdf",pdfTest->GetName()),";FitStatus;",8,-4,4);
  
  TGraph *gChi2 = new TGraph();
  gChi2->SetLineColor(kGreen+2);
  double w = toyhist.GetBinWidth(1);
  
  int ipoint=0;
  
  for (int b=0;b<toyhist.GetNbinsX();b++){
    double x = toyhist.GetBinCenter(b+1);
    if (x>0){
      gChi2->SetPoint(ipoint,x,(ROOT::Math::chisquared_pdf(x,ndof)));
      ipoint++;
    }
  }
  int npass =0; int nsuccesst =0;
  mass->setBins(nBinsForMass);
  for (int itoy = 0 ; itoy < ntoys ; itoy++){
    
    params_null->assignValueOnly(preParams_null);
    params_test->assignValueOnly(preParams_test);
    RooDataHist *binnedtoy = pdfNull->generateBinned(RooArgSet(*mass, *mass_MuMu),ndata,0,1);
    
    int stat_n=1;
    int stat_t=1;
    int ntries = 0;
    double nllNull,nllTest;
    // Iterate on the fit
    int MaxTries = 2;
    while (stat_n!=0){
      if (ntries>=MaxTries) break;
      RooFitResult *fitNull = pdfNull->fitTo(*binnedtoy,RooFit::Save(1),RooFit::Strategy(1),RooFit::SumW2Error(kTRUE) //FIXME
      ,RooFit::Minimizer("Minuit2","minimize"),RooFit::Minos(0),RooFit::Hesse(0),RooFit::PrintLevel(-1));
      //,RooFit::Optimize(0));
      
      nllNull = fitNull->minNll();
      stat_n = fitNull->status();
      if (stat_n!=0) params_null->assignValueOnly(fitNullData->randomizePars());
      ntries++;
    }
    
    ntries = 0;
    while (stat_t!=0){
      if (ntries>=MaxTries) break;
      RooFitResult *fitTest = pdfTest->fitTo(*binnedtoy,RooFit::Save(1),RooFit::Strategy(1),RooFit::SumW2Error(kTRUE) //FIXME
      ,RooFit::Minimizer("Minuit2","minimize"),RooFit::Minos(0),RooFit::Hesse(0),RooFit::PrintLevel(-1));
      nllTest = fitTest->minNll();
      stat_t = fitTest->status();
      if (stat_t!=0) params_test->assignValueOnly(fitTestData->randomizePars());
      ntries++;
    }
    
    toyhistStatN.Fill(stat_n);
    toyhistStatT.Fill(stat_t);
    
    if (stat_t !=0 || stat_n !=0) continue;
    nsuccesst++;
    double chi2_t = 2*(nllNull-nllTest);
    if (chi2_t >= chi2) npass++;
    toyhist.Fill(chi2_t);
  }
  
  double prob=0;
  if (nsuccesst!=0)  prob = (double)npass / nsuccesst;
  toyhist.Scale(1./(w*toyhist.Integral()));
  toyhist.Draw();
  TArrow lData(chi2,toyhist.GetMaximum(),chi2,0);
  lData.SetLineWidth(2);
  lData.Draw();
  gChi2->Draw("L");
  TLatex *lat = new TLatex();
  lat->SetNDC();
  lat->SetTextFont(42);
  lat->DrawLatex(0.1,0.91,Form("Prob (asymptotic) = %.4f (%.4f)",prob,prob_asym));
  can->SaveAs(name.c_str());
  
  TCanvas *stas =new TCanvas();
  toyhistStatN.SetLineColor(2);
  toyhistStatT.SetLineColor(1);
  TLegend *leg = new TLegend(0.2,0.6,0.4,0.87); leg->SetFillColor(0);
  leg->SetTextFont(42);
  leg->AddEntry(&toyhistStatN,"Null Hyp","L");
  leg->AddEntry(&toyhistStatT,"Test Hyp","L");
  toyhistStatN.Draw();
  toyhistStatT.Draw("same");
  leg->Draw();
  stas->SaveAs(Form("%s_fitstatus.pdf",name.c_str()));
  //reassign params
  params_null->assignValueOnly(preParams_null);
  params_test->assignValueOnly(preParams_test);
  
  delete can; delete stas;
  delete gChi2;
  delete leg;
  delete lat;
  
  // Still return the asymptotic prob (usually its close to the toys one)
  return prob_asym;
  
}

double getGoodnessOfFit(RooRealVar *mass, RooRealVar *mass_MuMu, RooAbsPdf *mpdf, RooDataSet *data, std::string name){
  
  double prob;
  int ntoys = 500;
  // Routine to calculate the goodness of fit.
  name+="_gofTest.pdf";
  RooRealVar norm("norm","norm",data->sumEntries(),0,10E6);
  //norm.removeRange();
  
  RooExtendPdf *pdf = new RooExtendPdf("ext","ext",*mpdf,norm);
  
  // get The Chi2 value from the data
  RooPlot *plot_chi2 = mass->frame();
  data->plotOn(plot_chi2,Binning(nBinsForMass),Name("data"));
  
  pdf->plotOn(plot_chi2,Name("pdf"));
  int np = pdf->getParameters(*data)->getSize();
  
  double chi2 = plot_chi2->chiSquare("pdf","data",np);
  std::cout << "[INFO] Calculating GOF for pdf " << pdf->GetName() << ", using " <<np << " fitted parameters" <<std::endl;
  
  // The first thing is to check if the number of entries in any bin is < 5
  // if so, we don't rely on asymptotic approximations
  
  // if ((double)data->sumEntries()/nBinsForMass < 5 ){
  if (false){
    
    std::cout << "[INFO] Running toys for GOF test " << std::endl;
    // store pre-fit params
    RooArgSet *params = pdf->getParameters(*data);
    RooArgSet preParams;
    params->snapshot(preParams);
    int ndata = data->sumEntries();
    
    int npass =0;
    std::vector<double> toy_chi2;
    for (int itoy = 0 ; itoy < ntoys ; itoy++){
       std::cout << "[INFO] " <<Form("\t.. %.1f %% complete\r",100*float(itoy)/ntoys) << std::flush;
      params->assignValueOnly(preParams);
      int nToyEvents = RandomGen->Poisson(ndata);
      RooDataHist *binnedtoy = pdf->generateBinned(RooArgSet(*mass, *mass_MuMu),nToyEvents,0,1);
      pdf->fitTo(*binnedtoy,RooFit::Minimizer("Minuit2","minimize"),RooFit::Minos(0),RooFit::Hesse(0),RooFit::PrintLevel(-1),RooFit::Strategy(0),RooFit::SumW2Error(kTRUE)); //FIXME
      
      RooPlot *plot_t = mass->frame();
      binnedtoy->plotOn(plot_t);
      pdf->plotOn(plot_t);//,RooFit::NormRange("fitdata_1,fitdata_2"));
      
      double chi2_t = plot_t->chiSquare(np);
      if( chi2_t>=chi2) npass++;
      toy_chi2.push_back(chi2_t*(nBinsForMass-np));
      delete plot_t;
    }
    std::cout << "[INFO] complete" << std::endl;
    prob = (double)npass / ntoys;
    
    TCanvas *can = new TCanvas();
    double medianChi2 = toy_chi2[(int)(((float)ntoys)/2)];
    double rms = TMath::Sqrt(medianChi2);
    
    TH1F toyhist(Form("gofTest_%s.pdf",pdf->GetName()),";Chi2;",50,medianChi2-5*rms,medianChi2+5*rms);
    for (std::vector<double>::iterator itx = toy_chi2.begin();itx!=toy_chi2.end();itx++){
      toyhist.Fill((*itx));
    }
    toyhist.Draw();
    
    TArrow lData(chi2*(nBinsForMass-np),toyhist.GetMaximum(),chi2*(nBinsForMass-np),0);
    lData.SetLineWidth(2);
    lData.Draw();
    can->SaveAs(name.c_str());
    
    // back to best fit
    params->assignValueOnly(preParams);
  } else {
    prob = TMath::Prob(chi2*(nBinsForMass-np),nBinsForMass-np);
  }
  std::cout << "[INFO] Chi2 in Observed =  " << chi2*(nBinsForMass-np) << std::endl;
  std::cout << "[INFO] p-value  =  " << prob << std::endl;
  delete pdf;
  return prob;
  
}

void plot(RooRealVar *mass, RooRealVar *mass_MuMu, RooAbsPdf *pdf, RooDataSet *data, string name,vector<string> flashggCats_, int status, double *prob){
  
  // Chi2 taken from full range fit
  RooPlot *plot_chi2 = mass->frame();
  data->plotOn(plot_chi2,Binning(nBinsForMass));
  pdf->plotOn(plot_chi2);
  
  int np = pdf->getParameters(*data)->getSize()+1; //Because this pdf has no extend
  double chi2 = plot_chi2->chiSquare(np);
  cout << "---> getting GoodnessOfFit..." << endl;
  *prob = getGoodnessOfFit(mass, mass_MuMu,pdf,data,name);
  RooPlot *plot = mass->frame();
  // mass->setRange("unblindReg_1",mgg_low,115);
  mass->setRange("unblindReg_1",mgg_low,mgg_mid_low);
  // mass->setRange("unblindReg_2",135,mgg_high);
  mass->setRange("unblindReg_2",mgg_mid_high,mgg_high);
  if (BLIND) {
    data->plotOn(plot,Binning(mgg_high-mgg_low),CutRange("unblindReg_1"));
    data->plotOn(plot,Binning(mgg_high-mgg_low),CutRange("unblindReg_2"));
    data->plotOn(plot,Binning(mgg_high-mgg_low),Invisible());
  }
  else data->plotOn(plot,Binning(mgg_high-mgg_low));
  
  // data->plotOn(plot,Binning(mgg_high-mgg_low));
  TCanvas *canv = new TCanvas();
  pdf->plotOn(plot);//,RooFit::NormRange("fitdata_1,fitdata_2"));
  pdf->paramOn(plot,RooFit::Layout(0.34,0.96,0.89),RooFit::Format("NEA",AutoPrecision(1)));
  if (BLIND) plot->SetMinimum(0.0001);
  plot->SetTitle("");
  // plot->GetXaxis()->SetTitle("E [keV]");
  plot->Draw();
  TLatex *lat = new TLatex();
  lat->SetNDC();
  lat->SetTextFont(42);
  lat->DrawLatex(0.1,0.92,Form("#chi^{2} = %.3f, Prob = %.2f, Fit Status = %d ",chi2*(nBinsForMass-np),*prob,status));
  // canv->SaveAs(name.c_str());

  canv->SaveAs(Form("%s_mHZ.pdf",name.c_str()));
  canv->SaveAs(Form("%s_mHZ.png",name.c_str()));
  canv->SaveAs(Form("%s_mHZ.root",name.c_str()));
  
  //plot_chi2->Draw();
  //canv->SaveAs((name+"debug").c_str());
  
  delete canv;
  delete lat;
}

void plotGOFMuMu(RooRealVar *mass, RooRealVar *mass_MuMu, RooAbsPdf *pdf, RooDataSet *data, string name,vector<string> flashggCats_, int status, double *prob){
  
  // Chi2 taken from full range fit
  int nBinsForMassMuMu = 50;
  RooPlot *plot_chi2 = mass_MuMu->frame();
  data->plotOn(plot_chi2,Binning(nBinsForMassMuMu));
  pdf->plotOn(plot_chi2);
  
  int np = pdf->getParameters(*data)->getSize()+1; //Because this pdf has no extend
  double chi2 = plot_chi2->chiSquare(np);
  cout << "---> getting GoodnessOfFit..." << endl;
  *prob = getGoodnessOfFit(mass, mass_MuMu,pdf,data,name);
  RooPlot *plot = mass_MuMu->frame();
  // mass->setRange("unblindReg_1",mgg_low,115);
  // mass->setRange("unblindReg_1",mgg_low,mgg_mid_low);
  // mass->setRange("unblindReg_2",135,mgg_high);
  // mass->setRange("unblindReg_2",mgg_mid_high,mgg_high);
  // if (BLIND) {
  //   data->plotOn(plot,Binning(mgg_high-mgg_low),CutRange("unblindReg_1"));
  //   data->plotOn(plot,Binning(mgg_high-mgg_low),CutRange("unblindReg_2"));
  //   data->plotOn(plot,Binning(mgg_high-mgg_low),Invisible());
  // }
  // else data->plotOn(plot);
  
  data->plotOn(plot,Binning(nBinsForMassMuMu));
  TCanvas *canv = new TCanvas();
  pdf->plotOn(plot);//,RooFit::NormRange("fitdata_1,fitdata_2"));
  // pdf->paramOn(plot,RooFit::Layout(0.34,0.96,0.89),RooFit::Format("NEA",AutoPrecision(1)));
  if (BLIND) plot->SetMinimum(0.0001);
  plot->SetTitle("");
  // plot->GetXaxis()->SetTitle("E [keV]");
  plot->Draw();
  TLatex *lat = new TLatex();
  lat->SetNDC();
  lat->SetTextFont(42);
  lat->DrawLatex(0.1,0.92,Form("#chi^{2} = %.3f, Prob = %.2f, Fit Status = %d ",chi2*(nBinsForMassMuMu-np),*prob,status));
  // canv->SaveAs(name.c_str());


  canv->SaveAs(Form("%s_MuMu.pdf",name.c_str()));
  canv->SaveAs(Form("%s_MuMu.png",name.c_str()));
  canv->SaveAs(Form("%s_MuMu.root",name.c_str()));
  
  //plot_chi2->Draw();
  //canv->SaveAs((name+"debug").c_str());
  
  delete canv;
  delete lat;
}

// the one for multipdf plot - mHZ
void plot(string analysisBranch, RooRealVar *mass, RooRealVar *mass_MuMu, RooMultiPdf *pdfs, RooCategory *catIndex, RooDataSet *data, string name, vector<string> flashggCats_, int cat, int bestFitPdf=-1){
  
  int color[7] = {kBlue,kRed,kMagenta,kGreen+1,kOrange+7,kAzure+10,kBlack};
  TLegend *leg = new TLegend(0.5,0.55,0.92,0.88);
  cout << "------> analysisBranch: " << analysisBranch << endl;
  if (analysisBranch == "ZToJPsiPhoton") leg->SetHeader("Z #rightarrow J/#Psi + #gamma Analysis");
  if (analysisBranch == "ZToUpsilonPhoton") leg->SetHeader("Z #rightarrow #Upsilon + #gamma Analysis");
  if (analysisBranch == "HToJPsiPhoton") leg->SetHeader("H #rightarrow J/#Psi + #gamma Analysis");
  if (analysisBranch == "HToUpsilonPhoton") leg->SetHeader("H #rightarrow #Upsilon + #gamma Analysis");
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  // leg->SetFillColor(0);
  // leg->SetLineColor(1);
  RooPlot *plot = mass->frame();
  
  // mass->setRange("unblindReg_1",mgg_low,115);
  mass->setRange("unblindReg_1",mgg_low,mgg_mid_low);
  cout << "##### unblindReg_1: " << mgg_low << " - " << mgg_mid_low << endl;
  // mass->setRange("unblindReg_2",135,mgg_high);
  mass->setRange("unblindReg_2",mgg_mid_high,mgg_high);
  cout << "##### unblindReg_2: " << mgg_mid_high << " - " << mgg_high << endl;
  if (BLIND) {
    data->plotOn(plot,Binning((int)((mgg_high-mgg_low)/2.)),CutRange("unblindReg_1"));
    data->plotOn(plot,Binning((int)((mgg_high-mgg_low)/2.)),CutRange("unblindReg_2"));
    data->plotOn(plot,Binning((int)((mgg_high-mgg_low)/2.)),Invisible());
  }
  else data->plotOn(plot,Binning((int)((mgg_high-mgg_low)/2.)));
  TCanvas *canv = new TCanvas("canv","canv",1050*2.0,750*2.0);
  gPad->SetLeftMargin(0.17);
  gPad->SetRightMargin(0.05);
  gPad->SetTopMargin(0.08);
  // /start extra bit for ratio plot///
  RooHist *plotdata = (RooHist*)plot->getObject(plot->numItems()-1);
  // bool doRatioPlot_=1;
  // TPad *pad1 = new TPad("pad1","pad1",0,0.25,1,1);
  // TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.35);
  // pad1->SetBottomMargin(0.18);
  // pad2->SetTopMargin(0.00001);
  // pad2->SetBottomMargin(0.25);
  // pad1->Draw();
  // pad2->Draw();
  // pad1->cd();
  // enf extra bit for ratio plot///
  
  int currentIndex = catIndex->getIndex();
  TObject *datLeg = plot->getObject(int(plot->numItems()-1));
  leg->AddEntry(datLeg,"Data","LEP");
  int style=1;
  RooAbsPdf *pdf;
  // RooCurve *nomBkgCurve;
  // int bestcol= -1;
  for (int icat=0;icat<catIndex->numTypes();icat++){
    int col;
    if (icat<=6) col=color[icat];
    else {col=kBlack; style++;}
    catIndex->setIndex(icat);
    pdfs->getCurrentPdf()->fitTo(*data,RooFit::Minos(0),RooFit::Minimizer("Minuit2","minimize"),RooFit::SumW2Error(kTRUE));  //FIXME
    // pdfs->getCurrentPdf()->fitTo(*data,RooFit::Minos(0),RooFit::Minimizer("Minuit2","minimize"),RooFit::SumW2Error(kTRUE), Range("unblindReg_1,unblindReg_2"));   //FIXME
    
    // pdfs->getCurrentPdf()->plotOn(plot,LineColor(col),LineStyle(style), Range("full"), NormRange("unblindReg_1,unblindReg_2"));//,RooFit::NormRange("fitdata_1,fitdata_2"));
    pdfs->getCurrentPdf()->plotOn(plot,LineColor(col),LineStyle(style));//,RooFit::NormRange("fitdata_1,fitdata_2"));
    
    TObject *pdfLeg = plot->getObject(int(plot->numItems()-1));
    std::string ext = "";
    if (bestFitPdf==icat) {
      ext=" (Best Fit Pdf)";
      pdf= pdfs->getCurrentPdf();
      // nomBkgCurve = (RooCurve*)plot->getObject(plot->numItems()-1);
      // bestcol = col;
    }
    // leg->AddEntry(pdfLeg,Form("%s%s",pdfs->getCurrentPdf()->GetName(),ext.c_str()),"L"); //flashGG
    
    
    vector<string> pdfNameVec;
    string pdfName = pdfs->getCurrentPdf()->GetName();
    cout << "-----------> pdfName: " << pdfName << endl;
    boost::split(pdfNameVec,pdfName, boost::is_any_of("_"));
    
    string pdfOrderSufix = (pdfNameVec.back()).back()+"th";
    if ((pdfNameVec[4]).back() == '1') pdfOrderSufix = "1st";
    if ((pdfNameVec[4]).back() == '2') pdfOrderSufix = "2nd";
    if ((pdfNameVec[4]).back() == '3') pdfOrderSufix = "3rd";
    
    (pdfNameVec[4]).pop_back();
    string pdfFancyName = "";
    if ((pdfNameVec[4]).compare("bern") == 0) pdfFancyName = "Bernstein " + pdfOrderSufix;
    if ((pdfNameVec[4]).compare("exp") == 0) pdfFancyName = "Exponencial " + pdfOrderSufix;
    if ((pdfNameVec[4]).compare("pow") == 0) pdfFancyName = "Power Law " + pdfOrderSufix;
    if ((pdfNameVec[4]).compare("lau") == 0) pdfFancyName = "Laurent " + pdfOrderSufix;
    
    cout << "-----------> pdfFancyName+ext: " << pdfFancyName+ext << endl;
    
    leg->AddEntry(pdfLeg,(pdfFancyName+ext).c_str(),"L");
    
    
  }
  
  
  plot->SetTitle(Form("Category %s",flashggCats_[cat].c_str()));
  if (BLIND) plot->SetMinimum(0.0001);
  
  if (BLIND) {
    data->plotOn(plot,Binning((int)((mgg_high-mgg_low)/2.)),CutRange("unblindReg_1"));
    data->plotOn(plot,Binning((int)((mgg_high-mgg_low)/2.)),CutRange("unblindReg_2"));
    data->plotOn(plot,Binning((int)((mgg_high-mgg_low)/2.)),Invisible());
  }
  else data->plotOn(plot,Binning((int)((mgg_high-mgg_low)/2.)));
  
  plot->GetXaxis()->SetTitle("M_{#mu#mu#gamma} (GeV)");
  plot->Draw();
  leg->Draw("same");
  // CMS_lumi( canv, 0, 0);
  
  // CMS decoration
  auto latex = new TLatex();
  latex->SetNDC();
  latex->SetTextFont(61);
  latex->SetTextSize(0.05);
  latex->DrawLatex(.17, 0.93, "CMS");
  latex->SetTextFont(52);
  latex->SetTextSize(0.04);
  latex->SetTextAlign(11);
  latex->DrawLatex(.25, 0.93, "Preliminary");
  latex->SetTextFont(42);
  latex->SetTextSize(0.04);
  latex->SetTextAlign(31);
  latex->DrawLatex(0.96, 0.93, "35.86 fb^{-1} (13 TeV, 2016) ");
  
  //  ///start extra bit for ratio plot///
  TH1D *hbplottmp = (TH1D*) pdf->createHistogram("hbplottmp",*mass,Binning(mgg_high-mgg_low,mgg_low,mgg_high));
  hbplottmp->Scale(plotdata->Integral());
  //  hbplottmp->Draw("same");
  //  int npoints = plotdata->GetN();
  //  double xtmp,ytmp;//
  //  int point =0;
  //  TGraphAsymmErrors *hdatasub = new TGraphAsymmErrors(npoints);
  //  //hdatasub->SetMarkerSize(defmarkersize);
  //  for (int ipoint=0; ipoint<npoints; ++ipoint) {
  //  //double bkgval = hbplottmp->GetBinContent(ipoint+1);
  //  plotdata->GetPoint(ipoint, xtmp,ytmp);
  // double bkgval = nomBkgCurve->interpolate(xtmp);
  //  if (BLIND) {
  //   // if ((xtmp > 115 ) && ( xtmp < 135) ) continue;
  //   if ((xtmp > mgg_mid_low ) && ( xtmp < mgg_mid_high) ) continue;
  //  }
  //  std::cout << "[INFO] plotdata->Integral() " <<  plotdata->Integral() << " ( bins " << npoints  << ") hbkgplots[i]->Integral() " << hbplottmp->Integral() << " (bins " << hbplottmp->GetNbinsX() << std::endl;
  // double errhi = plotdata->GetErrorYhigh(ipoint);
  // double errlow = plotdata->GetErrorYlow(ipoint);
  
  // //std::cout << "[INFO]  Channel " << name  << " errhi " << errhi << " errlow " << errlow  << std::endl;
  // std::cout << "[INFO] Channel  " << name << " setting point " << point <<" : xtmp "<< xtmp << "  ytmp " << ytmp << " bkgval  " << bkgval << " ytmp-bkgval " << ytmp-bkgval << std::endl;
  // bool drawZeroBins_ =1;
  // if (!drawZeroBins_) if(fabs(ytmp)<1e-5) continue;
  // hdatasub->SetPoint(point,xtmp,ytmp-bkgval);
  // hdatasub->SetPointError(point,0.,0.,errlow,errhi );
  // point++;
  //  }
  // pad2->cd();
  // TH1 *hdummy = new TH1D("hdummyweight","",mgg_high-mgg_low,mgg_low,mgg_high);
  // hdummy->SetMaximum(hdatasub->GetHistogram()->GetMaximum()+1);
  // hdummy->SetMinimum(hdatasub->GetHistogram()->GetMinimum()-1);
  // hdummy->GetYaxis()->SetTitle("data - best fit PDF");
  // hdummy->GetYaxis()->SetTitleSize(0.12);
  // hdummy->GetXaxis()->SetTitle("m_{#mu#mu#gamma} (GeV)");
  // hdummy->GetXaxis()->SetTitleSize(0.12);
  // hdummy->Draw("HIST");
  // hdummy->GetYaxis()->SetNdivisions(808);
  
  // TLine *line3 = new TLine(mgg_low,0.,mgg_high,0.);
  // line3->SetLineColor(bestcol);
  // line3->SetLineStyle(kDashed);
  // line3->SetLineWidth(5.0);
  // line3->Draw();
  // hdatasub->Draw("PESAME");
  // enf extra bit for ratio plot///
  canv->SaveAs(Form("%s.pdf",name.c_str()));
  canv->SaveAs(Form("%s.png",name.c_str()));
  canv->SaveAs(Form("%s.root",name.c_str()));
  catIndex->setIndex(currentIndex);
  delete canv;
}

// the one for multipdf plot - mMuMU
void plotMuMU(string analysisBranch, RooRealVar *mass, RooRealVar *mass_MuMu, RooMultiPdf *pdfs, RooCategory *catIndex, RooDataSet *data, string name, vector<string> flashggCats_, int cat, int bestFitPdf=-1) {
  
  int color[7] = {kBlue,kRed,kMagenta,kGreen+1,kOrange+7,kAzure+10,kBlack};
  TLegend *leg = new TLegend(0.6,0.60,0.9,0.90);
  cout << "------> analysisBranch: " << analysisBranch << endl;
  if (analysisBranch == "ZToJPsiPhoton") leg->SetHeader("Z #rightarrow J/#Psi + #gamma Analysis");
  if (analysisBranch == "ZToUpsilonPhoton") leg->SetHeader("Z #rightarrow #Upsilon + #gamma Analysis");
  if (analysisBranch == "HToJPsiPhoton") leg->SetHeader("H #rightarrow J/#Psi + #gamma Analysis");
  if (analysisBranch == "HToUpsilonPhoton") leg->SetHeader("H #rightarrow #Upsilon + #gamma Analysis");
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  // leg->SetFillColor(0);
  // leg->SetLineColor(1);
  RooPlot *plot = mass_MuMu->frame();
  
  // mass->setRange("unblindReg_1",mgg_low,115);
  // mass->setRange("unblindReg_1",mgg_low,mgg_mid_low);
  // cout << "unblindReg_1: " << mgg_low << " - " << mgg_mid_low << endl;
  // mass->setRange("unblindReg_2",135,mgg_high);
  // mass->setRange("unblindReg_2",mgg_mid_high,mgg_high);
  // cout << "unblindReg_2: " << mgg_mid_high << " - " << mgg_high << endl;
  // if (BLIND) {
  //   data->plotOn(plot,Binning((int)((mgg_high-mgg_low)/2.)),CutRange("unblindReg_1"));
  //   data->plotOn(plot,Binning((int)((mgg_high-mgg_low)/2.)),CutRange("unblindReg_2"));
  //   data->plotOn(plot,Binning((int)((mgg_high-mgg_low)/2.)),Invisible());
  // }
  data->plotOn(plot,Binning(40));
  TCanvas *canv = new TCanvas("canv","canv",1050*2.0,750*2.0);
  gPad->SetLeftMargin(0.17);
  gPad->SetRightMargin(0.05);
  gPad->SetTopMargin(0.08);
  
  int currentIndex = catIndex->getIndex();
  TObject *datLeg = plot->getObject(int(plot->numItems()-1));
  leg->AddEntry(datLeg,"Data","LEP");
  int styleSolid=1;
  int styleDashed=9;







  // for (int icat=0;icat<1;icat++){
    // get color 
  int col = 0;
    // if (icat<=6) {
    //   col=color[icat];
    // } else {
    //   col=kBlack; style++;
    // }
    
    // catIndex->setIndex(icat);
  catIndex->setIndex(bestFitPdf);
  pdfs->getCurrentPdf()->fitTo(*data,RooFit::Minos(0),RooFit::Minimizer("Minuit2","minimize"),RooFit::SumW2Error(kTRUE));  //FIXME

  string envPrefix = ((string)pdfs->getCurrentPdf()->GetName());
  envPrefix.erase(envPrefix.end()-10, envPrefix.end());
  cout << "pdfs->getCurrentPdf()->GetName(): " << pdfs->getCurrentPdf()->GetName() << endl;
  cout << "envPrefix: " << envPrefix << endl;

  string analysisBoson = analysisBranch.substr(0,1);
  
  // pdfs->getCurrentPdf()->getComponents()->find( (envPrefix+"_bernstein_PeakingBackground").c_str() )->plotOn(plot,LineColor(color[col]),LineStyle(styleDashed));
  // pdfs->getCurrentPdf()->getComponents()->find( (envPrefix+"_bernstein_PeakingBackground").c_str() )->Print();
  // list pdf components
  pdfs->getCurrentPdf()->getComponents()->Print();

  // plot peak_bckg
  if (analysisBoson == "Z") {
    pdfs->getCurrentPdf()->plotOn(
                      plot,
                      LineColor(color[col]),
                      LineStyle(styleDashed), 
                      RooFit::Name("peak_bckg"), 
                      RooFit::Components( *pdfs->getCurrentPdf()->getComponents()->find( (envPrefix+"_bernstein_PeakingBackground").c_str() ) )
                      );

    leg->AddEntry( plot->getObject(int(plot->numItems()-1)) , "Peaking Bckg" ,"L");
    col++;

    // plot comb_pol
    pdfs->getCurrentPdf()->plotOn(
                      plot,
                      LineColor(color[col]),
                      LineStyle(styleDashed), 
                      RooFit::Name("comb_pol"), 
                      RooFit::Components( *pdfs->getCurrentPdf()->getComponents()->find( (envPrefix+"_mBkg").c_str() ) ),
                      AddTo("peak_bckg")
                      );
    leg->AddEntry( plot->getObject(int(plot->numItems()-1)) , "Combinatorial Bckg" ,"L");
    col++;
  } else {
    // plot comb_pol
    pdfs->getCurrentPdf()->plotOn(
                      plot,
                      LineColor(color[col]),
                      LineStyle(styleDashed), 
                      RooFit::Name("comb_pol"), 
                      RooFit::Components( *pdfs->getCurrentPdf()->getComponents()->find( (envPrefix+"_mBkg").c_str() ) )
                      );
    leg->AddEntry( plot->getObject(int(plot->numItems()-1)) , "Combinatorial Bckg" ,"L");
    col++;
    }

  // plot 1S
  pdfs->getCurrentPdf()->plotOn(
                    plot,
                    LineColor(color[col]),
                    LineStyle(styleDashed), 
                    RooFit::Name("1S"), 
                    RooFit::Components( *pdfs->getCurrentPdf()->getComponents()->find( (envPrefix+"_bckg1S").c_str() ) ),
                    AddTo("comb_pol")
                    );
  leg->AddEntry( plot->getObject(int(plot->numItems()-1)) , "#Upsilon(1S) Combinatorial" ,"L");
  col++;

  // plot 2S
  pdfs->getCurrentPdf()->plotOn(
                    plot,
                    LineColor(color[col]),
                    LineStyle(styleDashed), 
                    RooFit::Name("2S"), 
                    RooFit::Components( *pdfs->getCurrentPdf()->getComponents()->find( (envPrefix+"_bckg2S").c_str() ) ),
                    AddTo("comb_pol")
                    );
  leg->AddEntry( plot->getObject(int(plot->numItems()-1)) , "#Upsilon(2S) Combinatorial" ,"L");
  col++;


  // plot 3S
  pdfs->getCurrentPdf()->plotOn(
                    plot,
                    LineColor(color[col]),
                    LineStyle(styleDashed), 
                    RooFit::Name("3S"), 
                    RooFit::Components( *pdfs->getCurrentPdf()->getComponents()->find( (envPrefix+"_bckg3S").c_str() ) ),
                    AddTo("comb_pol")
                    );
  leg->AddEntry( plot->getObject(int(plot->numItems()-1)) , "#Upsilon(3S) Combinatorial" ,"L");
  col++;

  pdfs->getCurrentPdf()->plotOn(plot,LineColor(kBlack),LineStyle(styleSolid));//,RooFit::NormRange("fitdata_1,fitdata_2"));
  col++;

  
  plot->SetTitle(Form("Category %s",flashggCats_[cat].c_str()));

  data->plotOn(plot,Binning(40));
  
  plot->GetXaxis()->SetTitle("M_{#mu#mu} (GeV)");
  plot->Draw();
  leg->Draw("same");
  // CMS_lumi( canv, 0, 0);
  
  // CMS decoration
  auto latex = new TLatex();
  latex->SetNDC();
  latex->SetTextFont(61);
  latex->SetTextSize(0.05);
  latex->DrawLatex(.17, 0.93, "CMS");
  latex->SetTextFont(52);
  latex->SetTextSize(0.04);
  latex->SetTextAlign(11);
  latex->DrawLatex(.25, 0.93, "Preliminary");
  latex->SetTextFont(42);
  latex->SetTextSize(0.04);
  latex->SetTextAlign(31);
  latex->DrawLatex(0.96, 0.93, "35.86 fb^{-1} (13 TeV, 2016) ");

  canv->SaveAs(Form("%s.pdf",name.c_str()));
  canv->SaveAs(Form("%s.png",name.c_str()));
  canv->SaveAs(Form("%s.root",name.c_str()));
  catIndex->setIndex(currentIndex);
  delete canv;
}

// fit function
// void runFitExt(RooMultiPdf *pdfs, RooCategory *catIndex, RooDataSet *data, double *NLL, int *stat_t, int MaxTries, int bestFitPdf=-1){
void runFitExt(string analysisBranch, string HZToUpsilonPhotonCat_, RooMultiPdf *pdfs, RooCategory *catIndex, RooDataSet *data, int bestFitPdf, json * bckgNormalization) {
  cout << "-----> <BEGIN> Extended Fit..." << endl;
  string analysisBoson = analysisBranch.substr(0,1);
  catIndex->setIndex(bestFitPdf);
  RooAbsPdf * pdf = pdfs->getCurrentPdf();
  // pdf->Print();

  string envPrefix = ((string)pdfs->getCurrentPdf()->GetName());
  envPrefix.erase(envPrefix.end()-10, envPrefix.end());

  RooRealVar * nTotalBCKG = new RooRealVar("nTotalBCKG","nTotalBCKG", 10, 0, 4000);
  RooRealVar * nCombBCKG = new RooRealVar("nCombBCKG","nCombBCKG", 10, 0, 4000);
  RooRealVar * nPeakBCKG = new RooRealVar("nPeakBCKG","nPeakBCKG", 10, 0, 4000);

  // RooAbsPdf * totalBckg_Ext;
  // if (analysisBoson == "Z") {
  //   RooExtendPdf * combTotal_Ext = new RooExtendPdf(
  //       (envPrefix+"_combTotal_Ext").c_str(), 
  //       "_combTotal_Ext", 
  //       *((RooAbsPdf*)(pdf->getComponents()->find( (envPrefix+"_combTotal").c_str() ))),  
  //       *nCombBCKG
  //       ); 

  //     RooExtendPdf * peakTotal_Ext = new RooExtendPdf(
  //       (envPrefix+"_peakTotal_Ext").c_str(), 
  //       "_peakTotal_Ext", 
  //       *((RooAbsPdf*)(pdf->getComponents()->find( (envPrefix+"_peakTotal").c_str() ))),  
  //       *nPeakBCKG
  //       ); 

  //     // RooExtendPdf * _totalBckg_Ext = new RooExtendPdf(
  //     //   (envPrefix+"_totalBckg_Ext").c_str(), 
  //     //   "_totalBckg_Ext", 
  //     //   *(pdf->getComponents()->find( (envPrefix+"_totalBckg").c_str() )),  
  //     //   *nTotalBCKG
  //     //   ); 

  //     totalBckg_Ext = new RooAddPdf(
  //           (envPrefix+"_totalBckg_Ext").c_str(), 
  //           (envPrefix+"_totalBckg_Ext").c_str(), 
  //           RooArgList(*combTotal_Ext, *peakTotal_Ext)
  //           );
  //   } else {
  //     totalBckg_Ext = new RooExtendPdf(
  //       (envPrefix+"_totalBckg_Ext").c_str(), 
  //       "_totalBckg_Ext", 
  //       *((RooAbsPdf*)(pdf->getComponents()->find( (envPrefix+"_totalBckg").c_str() ))),  
  //       *nTotalBCKG
  //       ); 
  //   }

      RooExtendPdf * totalBckg_Ext = new RooExtendPdf(
        (envPrefix+"_totalBckg_Ext").c_str(), 
        "_totalBckg_Ext", 
        *((RooAbsPdf*)(pdf->getComponents()->find( (envPrefix+"_totalBckg").c_str() ))),  
        *nTotalBCKG
        ); 

  // pdf->fitTo(*data,RooFit::Minos(0),RooFit::Minimizer("Minuit2","minimize"),RooFit::SumW2Error(kTRUE), RooFit::Extended(kTRUE));
  totalBckg_Ext->fitTo(*data,RooFit::Save(kTRUE), RooFit::Extended(kTRUE));

  (*bckgNormalization)[analysisBoson+"ToUpsilonPhoton"][HZToUpsilonPhotonCat_]["nCombBCKG"] = nCombBCKG->getVal();
  (*bckgNormalization)[analysisBoson+"ToUpsilonPhoton"][HZToUpsilonPhotonCat_]["nPeakBCKG"] = nPeakBCKG->getVal();
  (*bckgNormalization)[analysisBoson+"ToUpsilonPhoton"][HZToUpsilonPhotonCat_]["nTotalBCKG"] = nTotalBCKG->getVal();

  
  // int ntries=0;
  // RooArgSet *params_test = pdf->getParameters((const RooArgSet*)(0));
  // //params_test->Print("v");
  // int stat=1;
  // double minnll=10e8;
  // while (stat!=0){
  //   if (ntries>=MaxTries) break;
    
  //   // mass->setRange("unblindReg_1",mgg_low,mgg_mid_low);
  //   // mass->setRange("unblindReg_2",mgg_mid_high,mgg_high);
  //   // RooFitResult *fitTest = pdf->fitTo(*data,RooFit::Save(1)
  //   //  ,RooFit::Minimizer("Minuit2","minimize"),RooFit::SumW2Error(kTRUE), Range("unblindReg_1,unblindReg_2")); //FIXME
    
  //   cout << "----> Fitting..." << endl;
  //   RooFitResult *fitTest = pdf->fitTo(*data,RooFit::Save(1)
  //   ,RooFit::Minimizer("Minuit2","minimize"),RooFit::SumW2Error(kTRUE)); //FIXME
  //   cout << "----> Fitted..." << endl;
  //   stat = fitTest->status();
  //   minnll = fitTest->minNll();
  //   if (stat!=0) params_test->assignValueOnly(fitTest->randomizePars());
  //   ntries++;
  // }
  // *stat_t = stat;
  // *NLL = minnll;
  cout << "-----> <END> Extended Fit..." << endl;
}

// the one that plots the truths
void plot(string analysisBranch, RooRealVar *mass, map<string,RooAbsPdf*> pdfs, RooDataSet *data, string name, vector<string> flashggCats_, int cat, int bestFitPdf=-1){
  
  int color[7] = {kBlue,kRed,kMagenta,kGreen+1,kOrange+7,kAzure+10,kBlack};
  // TCanvas *canv = new TCanvas();
  auto canv = new TCanvas("canv","canv",1050*2.0,750*2.0);
  // auto canv = new TCanvas("canv","canv",1050,750);
  gPad->SetLeftMargin(0.17);
  gPad->SetRightMargin(0.05);
  gPad->SetTopMargin(0.08);
  
  TLegend *leg = new TLegend(0.6,0.65,0.88,0.88);
  cout << "------> analysisBranch: " << analysisBranch << endl;
  if (analysisBranch == "ZToJPsiPhoton") leg->SetHeader("Z #rightarrow J/#Psi + #gamma Analysis");
  if (analysisBranch == "ZToUpsilonPhoton") leg->SetHeader("Z #rightarrow #Upsilon + #gamma Analysis");
  if (analysisBranch == "HToJPsiPhoton") leg->SetHeader("H #rightarrow J/#Psi + #gamma Analysis");
  if (analysisBranch == "HToUpsilonPhoton") leg->SetHeader("H #rightarrow #Upsilon + #gamma Analysis");
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  RooPlot *plot = mass->frame();
  
  // mass->setRange("unblindReg_1",mgg_low,115);
  mass->setRange("unblindReg_1",mgg_low,mgg_mid_low);
  
  // mass->setRange("unblindReg_2",135,mgg_high);
  mass->setRange("unblindRegf_2",mgg_mid_high,mgg_high);
  if (BLIND) {
    data->plotOn(plot,Binning((int)((mgg_high-mgg_low)/2.)),CutRange("unblindReg_1"));
    data->plotOn(plot,Binning((int)((mgg_high-mgg_low)/2.)),CutRange("unblindReg_2"));
    data->plotOn(plot,Binning((int)((mgg_high-mgg_low)/2.)),Invisible());
  }
  else data->plotOn(plot,Binning((int)((mgg_high-mgg_low)*2.)));
  
  TObject *datLeg = plot->getObject(int(plot->numItems()-1));
  if(flashggCats_.size() >0){
    // leg->AddEntry(datLeg,Form("Data - %s",flashggCats_[cat].c_str()),"LEP");
    leg->AddEntry(datLeg,"Data","LEP");
  } else {
    // leg->AddEntry(datLeg,Form("Data - %d",cat),"LEP");
    leg->AddEntry(datLeg,"Data","LEP");
  }
  int i=0;
  int style=1;
  for (map<string,RooAbsPdf*>::iterator it=pdfs.begin(); it!=pdfs.end(); it++){
    int col;
    if (i<=6) col=color[i];
    else {col=kBlack; style++;}
    it->second->plotOn(plot,LineColor(col),LineStyle(style));//,RooFit::NormRange("fitdata_1,fitdata_2"));
    TObject *pdfLeg = plot->getObject(int(plot->numItems()-1));
    std::string ext = "";
    if (bestFitPdf==i) ext=" (Best Fit Pdf) ";
    leg->AddEntry(pdfLeg,Form("%s%s",it->first.c_str(),ext.c_str()),"L");
    i++;
  }
  plot->SetTitle(Form(" %s",flashggCats_[cat].c_str()));
  if (BLIND) plot->SetMinimum(0.0001);
  
  // plot->GetXaxis()->SetTitle("#mu#mu#gamma Mass (GeV)");
  // plot->GetXaxis()->SetLabelOffset(0.05);
  plot->GetXaxis()->SetTitle("M_{#mu#mu#gamma} (GeV)");
  
  plot->Draw();
  leg->Draw("same");
  // CMS_lumi( canv, 0, 0);
  
  // CMS decoration
  auto latex = new TLatex();
  latex->SetNDC();
  latex->SetTextFont(61);
  latex->SetTextSize(0.05);
  latex->DrawLatex(.17, 0.93, "CMS");
  latex->SetTextFont(52);
  latex->SetTextSize(0.04);
  latex->SetTextAlign(11);
  latex->DrawLatex(.25, 0.93, "Preliminary");
  latex->SetTextFont(42);
  latex->SetTextSize(0.04);
  latex->SetTextAlign(31);
  latex->DrawLatex(0.96, 0.93, "35.86 fb^{-1} (13 TeV, 2016) ");
  
  
  canv->SaveAs(Form("%s.pdf",name.c_str()));
  canv->SaveAs(Form("%s.png",name.c_str()));
  canv->SaveAs(Form("%s.root",name.c_str()));
  delete canv;
}

void transferMacros(TFile *inFile, TFile *outFile){
  
  TIter next(inFile->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)next())){
    if (string(key->ReadObj()->ClassName())=="TMacro") {
      //cout << key->ReadObj()->ClassName() << " : " << key->GetName() << endl;
      TMacro *macro = (TMacro*)inFile->Get(key->GetName());
      outFile->cd();
      macro->Write();
    }
  }
}
int getBestFitFunction(RooMultiPdf *bkg, RooDataSet *data, RooCategory *cat, bool silent=false){
  
  
  double global_minNll = 1E10;
  int best_index = 0;
  int number_of_indeces = cat->numTypes();
  
  RooArgSet snap,clean;
  RooArgSet *params = bkg->getParameters((const RooArgSet*)0);
  params->remove(*cat);
  params->snapshot(snap);
  params->snapshot(clean);
  if (!silent) {
    //params->Print("V");
  }
  
  //bkg->setDirtyInhibit(1);
  //RooAbsReal *nllm = bkg->createNLL(*data);
  //RooMinimizer minim(*nllm);
  //minim.setStrategy(1);
  
  for (int id=0;id<number_of_indeces;id++){
    params->assignValueOnly(clean);
    cat->setIndex(id);
    
    //RooAbsReal *nllm = bkg->getCurrentPdf()->createNLL(*data);
    
    if (!silent) {
      /*
      std::cout << "BEFORE  MAKING FIT" << std::endl;
      params->Print("V");
      std::cout << "-----------------------" << std::endl;
      */
    }
    
    //minim.minimize("Minuit2","minimize");
    double minNll=0; //(nllm->getVal())+bkg->getCorrection();
    int fitStatus=1;
    runFit(bkg->getCurrentPdf(),data,&minNll,&fitStatus,/*max iterations*/3);
    // Add the penalty
    
    minNll=minNll+bkg->getCorrection();
    
    if (!silent) {
      /*
      std::cout << "After Minimization ------------------  " <<std::endl;
      std::cout << bkg->getCurrentPdf()->GetName() << " " << minNll <<std::endl;
      bkg->Print("v");
      bkg->getCurrentPdf()->getParameters(*data)->Print("V");
      std::cout << " ------------------------------------  " << std::endl;
      
      params->Print("V");
      */
      std::cout << "[INFO] AFTER FITTING" << std::endl;
      std::cout << "[INFO] Function was " << bkg->getCurrentPdf()->GetName() <<std::endl;
      std::cout << "[INFO] Correction Applied is " << bkg->getCorrection() <<std::endl;
      std::cout << "[INFO] NLL + c = " <<  minNll << std::endl;
      std::cout << "-----------------------" << std::endl;
    }
    
    if (minNll < global_minNll){
      global_minNll = minNll;
      snap.assignValueOnly(*params);
      best_index=id;
    }
  }
  cat->setIndex(best_index);
  params->assignValueOnly(snap);
  
  if (!silent) {
    std::cout << "[INFO] Best fit Function -- " << bkg->getCurrentPdf()->GetName() << " " << cat->getIndex() <<std::endl;
    //bkg->getCurrentPdf()->getParameters(*data)->Print("v");
  }
  return best_index;
}

int main(int argc, char* argv[]){
  
  setTDRStyle();
  // cout << "Oi!" << endl;
  writeExtraText = true;       // if extra text
  extraText  = "Preliminary";  // default extra text is "Preliminary"
  lumi_8TeV  = "19.1 fb^{-1}"; // default is "19.7 fb^{-1}"
  lumi_7TeV  = "4.9 fb^{-1}";  // default is "5.1 fb^{-1}"
  lumi_sqrtS = "13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)
  
  string fileName;
  int ncats;
  int singleCategory;
  string datfile;
  string outDir;
  string outfilename;
  bool is2011=false;
  bool verbose=false;
  bool saveMultiPdf=false;
  int isFlashgg_ =1;
  string flashggCatsStr_;
  vector<string> flashggCats_;
  bool isData_ =0;
  bool isZ_ = 0;
  string HZToUpsilonPhotonCat_;
  string analysisBranch;
  
  po::options_description desc("Allowed options");
  desc.add_options()
  ("help,h",                                                                                  "Show help")
  ("infilename,i", po::value<string>(&fileName),                                              "In file name")
  ("ncats,c", po::value<int>(&ncats)->default_value(5),                                       "Number of categories")
  ("singleCat", po::value<int>(&singleCategory)->default_value(-1),                           "Run A single Category")
  ("datfile,d", po::value<string>(&datfile)->default_value("dat/fTest.dat"),                  "Right results to datfile for BiasStudy")
  ("outDir,D", po::value<string>(&outDir)->default_value("plots/fTest"),                      "Out directory for plots")
  ("saveMultiPdf", po::value<string>(&outfilename),                   "Save a MultiPdf model with the appropriate pdfs")
  ("runFtestCheckWithToys",                   "When running the F-test, use toys to calculate pvals (and make plots) ")
  ("is2011",                                                                                  "Run 2011 config")
  ("is2012",                                                                                  "Run 2012 config")
  ("unblind",                            "Dont blind plots")
  ("isFlashgg",  po::value<int>(&isFlashgg_)->default_value(1),                                "Use Flashgg output ")
  
  ("isData",  po::value<bool>(&isData_)->default_value(0),                                "Use Data not MC ")
  ("isZ",  po::value<bool>(&isZ_)->default_value(0),                                "Analysing Z boson or Higgs")
  ("analysisBranch", po::value<string>(&analysisBranch)->default_value(""),                      "Analysis Branch")
  ("flashggCats,f", po::value<string>(&flashggCatsStr_)->default_value("UntaggedTag_0,UntaggedTag_1,UntaggedTag_2,UntaggedTag_3,UntaggedTag_4,VBFTag_0,VBFTag_1,VBFTag_2,TTHHadronicTag,TTHLeptonicTag,VHHadronicTag,VHTightTag,VHLooseTag,VHEtTag"),       "Flashgg category names to consider")
  ("HZToUpsilonPhotonCat", po::value<string>(&HZToUpsilonPhotonCat_)->default_value("Cat0"), "HZToUpsilonPhoton category name")
  ("verbose,v",                                                                               "Run with more output")
  ;
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc,argv,desc),vm);
  po::notify(vm);
  if (vm.count("help")) { cout << desc << endl; exit(1); }
  if (vm.count("is2011")) is2011=true;
  if (vm.count("unblind")) BLIND=false;
  saveMultiPdf = vm.count("saveMultiPdf");
  
  if (vm.count("verbose")) verbose=true;
  if (vm.count("runFtestCheckWithToys")) runFtestCheckWithToys=true;
  
  if (!verbose) {
    RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);
    RooMsgService::instance().setSilentMode(true);
    gErrorIgnoreLevel=kWarning;
  }
  split(flashggCats_,flashggCatsStr_,boost::is_any_of(","));
  
  int startingCategory=0;
  if (singleCategory >-1){
    ncats=singleCategory+1;
    startingCategory=singleCategory;
  }
  if (isFlashgg_==1){
    
    ncats= flashggCats_.size();
    
  }
  // is Z boson?
  if (isZ_) {
    cout << "-------> It a Z analysis!" << endl;
    mgg_low = 70;
    mgg_mid_low = 80;
    mgg_mid_high = 100;
    mgg_high = 120;
    nBinsForMass = 2*(mgg_high-mgg_low);
  }
  cout << "---> unblindReg_1: " << mgg_low << " - " << mgg_mid_low << endl;
  cout << "---> unblindReg_2: " << mgg_mid_high << " - " << mgg_high << endl;
  
  
  if(verbose) std::cout << "[INFO] SaveMultiPdf? " << saveMultiPdf << std::endl;
  TFile *outputfile;
  RooWorkspace *outputws;
  
  if (saveMultiPdf){
    outputfile = new TFile(outfilename.c_str(),"RECREATE");
    outputws = new RooWorkspace(); outputws->SetName("multipdf");
  }
  
  system(Form("mkdir -p %s",outDir.c_str()));
  TFile *inFile = TFile::Open(fileName.c_str());
  RooWorkspace *inWS;
  if(isFlashgg_){
    if (isData_){
      // inWS = (RooWorkspace*)inFile->Get("tagsDumper/cms_hgg_13TeV");
      inWS = (RooWorkspace*)inFile->Get("w");
    } else {
      inWS = (RooWorkspace*)inFile->Get("cms_hgg_workspace");
    }
  } else {
    inWS = (RooWorkspace*)inFile->Get("w");//FIXME
  }
  if (verbose) std::cout << "[INFO]  inWS open " << inWS << std::endl;
  if (saveMultiPdf){
    transferMacros(inFile,outputfile);
    
    RooRealVar *intL;
    RooRealVar *sqrts;
    
    if (isFlashgg_){
      //intL  = (RooRealVar*)inWS->var("IntLumi");
      intL  = intLumi_;
      sqrts = (RooRealVar*)inWS->var("SqrtS");
      if (!sqrts){ sqrts = new RooRealVar("SqrtS","SqrtS",13); }
      std::cout << "[INFO] got intL and sqrts " << intL << ", " << sqrts << std::endl;
      
      
    } else {
      //intL  = (RooRealVar*)inWS->var("IntLumi");
      intL  = intLumi_;
      sqrts = (RooRealVar*)inWS->var("Sqrts");
    }
    outputws->import(*intL);
    outputws->import(*sqrts);
    std::cout << "[INFO] got intL and sqrts " << intL << ", " << sqrts << std::endl;
  }
  
  vector<string> functionClasses;
  functionClasses.push_back("Bernstein");
  functionClasses.push_back("Exponential");
  functionClasses.push_back("PowerLaw");
  functionClasses.push_back("Laurent");
  map<string,string> namingMap;
  namingMap.insert(pair<string,string>("Bernstein","pol"));
  namingMap.insert(pair<string,string>("Exponential","exp"));
  namingMap.insert(pair<string,string>("PowerLaw","pow"));
  namingMap.insert(pair<string,string>("Laurent","lau"));
  
  // store results here
  
  FILE *resFile ;
  if  (singleCategory >-1) resFile = fopen(Form("%s/fTestResults_%s.txt",outDir.c_str(),flashggCats_[singleCategory].c_str()),"w");
  else resFile = fopen(Form("%s/fTestResults.txt",outDir.c_str()),"w");
  vector<map<string,int> > choices_vec;
  vector<map<string,std::vector<int> > > choices_envelope_vec;
  vector<map<string,RooAbsPdf*> > pdfs_vec;
  
  PdfModelBuilder pdfsModel;
  // RooRealVar *mass = (RooRealVar*)inWS->var("CMS_hgg_mass");
  // RooRealVar * mass = (RooRealVar*)inWS->var("mHZData");
  // RooRealVar * mass_MuMu = (RooRealVar*)inWS->var("mMuMNUData");

  RooRealVar * mass = (RooRealVar*)inWS->var("mHZ");
  RooRealVar * mass_MuMu = (RooRealVar*)inWS->var("mMuMNU");

  mass->setRange("unblindReg_1",mgg_low,mgg_mid_low);
  mass->setRange("unblindReg_2",mgg_mid_high,mgg_high);
  
  // mass->setRange("fullRange",mgg_low,mgg_high);
  
  //        RooRealVar *mass = (RooRealVar*)inWS->var("mHZ");
  
  std:: cout << "[INFO] Got mass from ws " << mass << std::endl;
  pdfsModel.setObsVar(mass);
  double upperEnvThreshold = 0.1; // upper threshold on delta(chi2) to include function in envelope (looser than truth function)
  
  fprintf(resFile,"Truth Model & d.o.f & $\\Delta NLL_{N+1}$ & $p(\\chi^{2}>\\chi^{2}_{(N\\rightarrow N+1)})$ \\\\\n");
  fprintf(resFile,"\\hline\n");
  
  std::string ext = is2011 ? "7TeV" : "8TeV";
  if (isFlashgg_) ext = "13TeV";
  for (int cat=startingCategory; cat<ncats; cat++) {
    
    map<string,int> choices;
    map<string,std::vector<int> > choices_envelope;
    map<string,RooAbsPdf*> pdfs;
    map<string,RooAbsPdf*> allPdfs;
    string catname;
    if (isFlashgg_){
      catname = Form("%s",flashggCats_[cat].c_str());
    } else {
      catname = Form("cat%d",cat);
    }
    RooDataSet *dataFull;
    // RooDataSet *dataFull0;
    if (isData_) {
      dataFull = (RooDataSet*)inWS->data("data_obs");
      // dataFull = (RooDataSet*)inWS->data(Form("Data_13TeV_%s",catname.c_str()));
      //dataFull = (RooDataSet*)inWS->data(Form("Data_13TeV_forFLASHGG"));
      /*dataFull= (RooDataSet*) dataFull0->emptyClone();
      for (int i =0 ; i < dataFull0->numEntries() ; i++){
      double m = dataFull0->get(i)->getRealValue("CMS_hgg_mass");
      //if (m <(mgg_low+0.01) or m > (mgg_high-0.01))
      
      if (m==mgg_low){
      std::cout << "dataset mass m="<< m << std::endl;
      continue;
      }
      dataFull->add(*dataFull0->get(),1.0);
       }*/
      if (verbose) std::cout << "[INFO] opened data for  "  << Form("Data_%s",catname.c_str()) <<" - " << dataFull <<std::endl;
  } else { 
      dataFull = (RooDataSet*)inWS->data(Form("data_mass_%s",catname.c_str()));
      if (verbose) std::cout << "[INFO] opened data for  "  << Form("data_mass_%s",catname.c_str()) <<" - " << dataFull <<std::endl;
  }


mass->setBins(nBinsForMass);
RooDataSet *data;
//  RooDataHist thisdataBinned(Form("roohist_data_mass_cat%d",cat),"data",*mass,*dataFull);
//  RooDataSet *data = (RooDataSet*)&thisdataBinned;
string thisdataBinned_name;

if ( isFlashgg_){
  thisdataBinned_name =Form("roohist_data_mass_%s",flashggCats_[cat].c_str());
  //  RooDataHist thisdataBinned(Form("roohist_data_mass_cat%d",cat),"data",*mass,*dataFull);
  //  data = (RooDataSet*)&thisdataBinned;
  //    std::cout << "debug " << thisdataBinned.GetName() << std::endl;
  
  //RooDataSet *data = (RooDataSet*)dataFull;
} else {
  thisdataBinned_name= Form("roohist_data_mass_cat%d",cat);
  //RooDataSet *data = (RooDataSet*)dataFull;
}
RooDataHist thisdataBinned(thisdataBinned_name.c_str(),"data",RooArgList(*mass,*mass_MuMu), *dataFull);
// data = (RooDataSet*)&thisdataBinned;
data = dataFull;

RooArgList storedPdfs("store");

fprintf(resFile,"\\multicolumn{4}{|c|}{\\textbf{Category %d}} \\\\\n",cat);
fprintf(resFile,"\\hline\n");

double MinimimNLLSoFar=1e10;
int simplebestFitPdfIndex = 0;

// Standard F-Test to find the truth functions
for (vector<string>::iterator funcType=functionClasses.begin(); funcType!=functionClasses.end(); funcType++) {
  double thisNll=0.;
  double prevNll=0.;
  double chi2=0.;
  double prob=0.;
  int order=1;
  int prev_order=0;
  int cache_order=0;
  
  RooAbsPdf *prev_pdf=NULL;
  RooAbsPdf *cache_pdf=NULL;
  std::vector<int> pdforders;
  
  int counter =0;
  //  while (prob<0.05){
  while (prob<0.05 && order < 7) { //FIXME
    // RooAbsPdf *bkgPdf = getPdf(pdfsModel,*funcType,order,Form("ftest_pdf_%d_%s",cat,ext.c_str()));

    RooAbsPdf *bkgPdf = getPdf(analysisBranch, HZToUpsilonPhotonCat_, mass, mass_MuMu, pdfsModel,*funcType,order,Form("ftest_pdf_%d_%s",cat,ext.c_str()));
    // bkgPdf->Print();
    
    if (!bkgPdf){
      // assume this order is not allowed
      order++;
    } else {
      //RooFitResult *fitRes = bkgPdf->fitTo(*data,Save(true),RooFit::Minimizer("Minuit2","minimize"));
      
      int fitStatus = 0;
      //thisNll = fitRes->minNll();
      
      bkgPdf->Print();
      
      runFit(bkgPdf,data,&thisNll,&fitStatus,/*max iterations*/3); //bkgPdf->fitTo(*data,Save(true),RooFit::Minimizer("Minuit2","minimize"));
      
      if (fitStatus!=0) std::cout << "[WARNING] Warning -- Fit status for " << bkgPdf->GetName() << " at " << fitStatus <<std::endl;
      
      chi2 = 2.*(prevNll-thisNll);
      if (chi2<0. && order>1) chi2=0.;
      if (prev_pdf!=NULL){
        prob = getProbabilityFtest(chi2,order-prev_order,prev_pdf,bkgPdf,mass, mass_MuMu, data ,Form("%s/Ftest_from_%s%d_cat%d.pdf",outDir.c_str(),funcType->c_str(),order,cat));
        std::cout << "[INFO]  F-test Prob(chi2>chi2(data)) == " << prob << std::endl;
      } else {
        prob = 0;
      }
      // double gofProb=0;
      // otherwise we get it later ...
      // if (!saveMultiPdf) plot(mass,bkgPdf,data,Form("%s/%s%d_cat%d.pdf",outDir.c_str(),funcType->c_str(),order,cat),flashggCats_,fitStatus,&gofProb);
      cout << "[INFO]\t " << *funcType << " " << order << " " << prevNll << " " << thisNll << " " << chi2 << " " << prob << endl;
      fprintf(resFile,"%15s && %d && %10.2f && %10.2f && %10.2f \\\\\n",funcType->c_str(),order,thisNll,chi2,prob);
      prevNll=thisNll;
      cache_order=prev_order;
      cache_pdf=prev_pdf;
      prev_order=order;
      prev_pdf=bkgPdf;
      order++;
    }
    counter++;
  }
  
  fprintf(resFile,"%15s & %d & %5.2f & %5.2f \\\\\n",funcType->c_str(),cache_order+1,chi2,prob);
  choices.insert(pair<string,int>(*funcType,cache_order));
  pdfs.insert(pair<string,RooAbsPdf*>(Form("%s%d",funcType->c_str(),cache_order),cache_pdf));
  
  int truthOrder = cache_order;
  
  // Now run loop to determine functions inside envelope
  if (saveMultiPdf){
    chi2=0.;
    thisNll=0.;
    prevNll=0.;
    prob=0.;
    order=1;
    prev_order=0;
    cache_order=0;
    std::cout << "[INFO] Determining Envelope Functions for Family " << *funcType << ", cat " << cat << std::endl;
    std::cout << "[INFO] Upper end Threshold for highest order function " << upperEnvThreshold <<std::endl;
    
    while (prob<upperEnvThreshold){
      // RooAbsPdf *bkgPdf = getPdf(pdfsModel,*funcType,order,Form("env_pdf_%d_%s",cat,ext.c_str()));
      RooAbsPdf *bkgPdf = getPdf(analysisBranch, HZToUpsilonPhotonCat_, mass, mass_MuMu, pdfsModel,*funcType,order,Form("env_pdf_%d_%s",cat,ext.c_str()));
      // RooAbsPdf *bkgPdf = getPdf(pdfsModel,*funcType,order,Form("%s",ext.c_str()));
      if (!bkgPdf ){
        // assume this order is not allowed
        if (order >6) { std::cout << " [WARNING] could not add ] " << std::endl; break ;}
        order++;
      } else {
        //RooFitResult *fitRes;
        int fitStatus=0;
        runFit(bkgPdf,data,&thisNll,&fitStatus,/*max iterations*/3); //bkgPdf->fitTo(*data,Save(true),RooFit::Minimizer("Minuit2","minimize"));
        //thisNll = fitRes->minNll();
        if (fitStatus!=0) std::cout << "[WARNING] Warning -- Fit status for " << bkgPdf->GetName() << " at " << fitStatus <<std::endl;
        double myNll = 2.*thisNll;
        chi2 = 2.*(prevNll-thisNll);
        if (chi2<0. && order>1) chi2=0.;
        prob = TMath::Prob(chi2,order-prev_order);
        
        cout << "[INFO] \t " << *funcType << " " << order << " " << prevNll << " " << thisNll << " " << chi2 << " " << prob << endl;
        prevNll=thisNll;
        cache_order=prev_order;
        cache_pdf=prev_pdf;
        
        // Calculate goodness of fit for the thing to be included (will use toys for lowstats)!
        double gofProb =0;
        plot(mass, mass_MuMu,bkgPdf,data,Form("%s/%s%d_cat%d.pdf",outDir.c_str(),funcType->c_str(),order,cat),flashggCats_,fitStatus,&gofProb);
        plotGOFMuMu(mass, mass_MuMu,bkgPdf,data,Form("%s/%s%d_cat%d.pdf",outDir.c_str(),funcType->c_str(),order,cat),flashggCats_,fitStatus,&gofProb);

        cout << "-------> gofProb: " << gofProb << endl;         
        if ((prob < upperEnvThreshold) ) { // Looser requirements for the envelope
          
          // if (gofProb > 0.01 || order == truthOrder ) {  // Good looking fit or one of our regular truth functions
            if (order == truthOrder ) {  // Good looking fit or one of our regular truth functions
            
            std::cout << "[INFO] Adding to Envelope " << bkgPdf->GetName() << " "<< gofProb << " 2xNLL + c is " << myNll + bkgPdf->getVariables()->getSize() <<  std::endl;
            allPdfs.insert(pair<string,RooAbsPdf*>(Form("%s%d",funcType->c_str(),order),bkgPdf));
            storedPdfs.add(*bkgPdf);
            pdforders.push_back(order);
            
            // Keep track but we shall redo this later
            if ((myNll + bkgPdf->getVariables()->getSize()) < MinimimNLLSoFar) {
              simplebestFitPdfIndex = storedPdfs.getSize()-1;
              MinimimNLLSoFar = myNll + bkgPdf->getVariables()->getSize();
            }
          }
        }
        
        prev_order=order;
        prev_pdf=bkgPdf;
        order++;
      }
    }
    
    fprintf(resFile,"%15s & %d & %5.2f & %5.2f \\\\\n",funcType->c_str(),cache_order+1,chi2,prob);
    choices_envelope.insert(pair<string,std::vector<int> >(*funcType,pdforders));
  }
} // end F-Test

fprintf(resFile,"\\hline\n");
choices_vec.push_back(choices);
choices_envelope_vec.push_back(choices_envelope);
pdfs_vec.push_back(pdfs);

plot(analysisBranch, mass,pdfs,data,Form("%s/truths_cat%d",outDir.c_str(),cat),flashggCats_,cat);

if (saveMultiPdf) {
  
  
  // Put selectedModels into a MultiPdf
  string catindexname;
  string catname;
  if (isFlashgg_){
    catindexname = Form("pdfindex_%s_%s_%s",flashggCats_[cat].c_str(),ext.c_str(), HZToUpsilonPhotonCat_.c_str());
    catname = Form("%s",flashggCats_[cat].c_str());
  } else {
    catindexname = Form("pdfindex_%d_%s_%s",cat,ext.c_str(), HZToUpsilonPhotonCat_.c_str());
    catname = Form("cat%d",cat);
  }
  RooCategory catIndex(catindexname.c_str(),"c");
  RooMultiPdf *pdf = new RooMultiPdf(Form("CMS_HZToUpsilonPhoton_%s_%s_bkgshape",catname.c_str(),ext.c_str()),"all pdfs",catIndex,storedPdfs);
  
  
  
  
  std::ifstream i("inputData/evtsCountFiles/FinalYields.json");
  json evtCounts;
  i >> evtCounts;
  
  // RooRealVar nBackground(Form("CMS_hgg_%s_%s_bkgshape_norm",catname.c_str(),ext.c_str()),"nbkg",data->sumEntries(),0,10E8);
  // RooRealVar nBackground(Form("CMS_hgg_%s_%s_bkgshape_norm",catname.c_str(),ext.c_str()),"nbkg",data->sumEntries(),0,3*data->sumEntries());
  // RooRealVar nBackground(Form("CMS_hgg_%s_%s_bkgshape_norm",catname.c_str(),ext.c_str()),"nbkg",data->sumEntries(),0.75*data->sumEntries(),1.75*data->sumEntries());
  // RooRealVar nBackground(Form("CMS_hgg_%s_%s_bkgshape_norm",catname.c_str(),ext.c_str()),"nbkg",data->sumEntries("fullRange"));
  string strTemp = regex_replace(analysisBranch, regex("To"), "to")+"_"+HZToUpsilonPhotonCat_;
  strTemp = regex_replace(strTemp, regex("Photon"), "");
  // cout << "strTemp: " << strTemp << endl;
  
  // RooRealVar nBackground(Form("CMS_hgg_%s_%s_bkgshape_norm",catname.c_str(),ext.c_str()),"nbkg",evtCounts[strTemp][0]["Final Yield/Count"]);
  RooRealVar nBackground(Form("CMS_HZToUpsilonPhoton_%s_%s_bkgshape_norm",catname.c_str(),ext.c_str()),"nbkg",data->sumEntries(), 0, 500);
  // RooRealVar nBackground(Form("CMS_HZToUpsilonPhoton_%s_%s_bkgshape_norm",catname.c_str(),ext.c_str()),"nbkg",1);

  cout << "\n\n-----> nBackground:" << endl;
  nBackground.Print();
  cout << "From analysis code: " << evtCounts[strTemp][0]["Final Yield/Count"] << endl;
  // data->write((analysisBranch+"_"+HZToUpsilonPhotonCat_+ ".txt").c_str());

  
  
  
  //nBackground.removeRange(); // bug in roofit will break combine until dev branch brought in
  //double check the best pdf!
  int bestFitPdfIndex = getBestFitFunction(pdf,data,&catIndex,!verbose);
  catIndex.setIndex(bestFitPdfIndex);
  std::cout << "// ------------------------------------------------------------------------- //" <<std::endl;
  std::cout << "[INFO] Created MultiPdf " << pdf->GetName() << ", in Category " << cat << " with a total of " << catIndex.numTypes() << " pdfs"<< std::endl;
  storedPdfs.Print();
  std::cout << "[INFO] Best Fit Pdf = " << bestFitPdfIndex << ", " << storedPdfs.at(bestFitPdfIndex)->GetName() << std::endl;
  std::cout << "// ------------------------------------------------------------------------- //" <<std::endl;
  std::cout << "[INFO] Simple check of index "<< simplebestFitPdfIndex <<std::endl;
  
  mass->setBins(nBinsForMass);
  RooDataHist dataBinned(Form("roohist_data_mass_%s",catname.c_str()),"data",*mass,*dataFull);
  RooDataSet *dataObsBin = (RooDataSet*)dataFull;
  // Save it (also a binned version of the dataset)
  outputws->import(*pdf);
  outputws->import(nBackground);
  outputws->import(catIndex);
  outputws->import(dataBinned);
  outputws->import(*data);
  outputws->import(*dataObsBin);
  
  // cout << " ######################################################## " << endl;
  
  RooArgSet varSet = outputws->allVars();
  TIterator * varIter = varSet.createIterator();
  RooRealVar* var;
  // while( (var=(RooRealVar*)varIter->Next()) ) {
  //   cout << " --------> var->GetName(): "<< var->GetName() << endl;
  //   var->printExtras(cout);
  //   cout << "\n" << endl;
  // }
  // varIter->Reset();
  
  
  // cout << " ######################################################## " << endl;
  
  
  while( (var=(RooRealVar*)varIter->Next()) ) {
    // cout << " --------> var->GetName(): "<< var->GetName() << endl;
    size_t found = (string(var->GetName())).find("env_");
    if(found == 0 || hasEnding ((string(var->GetName())), "_norm") ) {
    // if(found == 0) {
      if ((string)var->GetName() != "mHZ" || (string)var->GetName() != "mMuMNU") {
        var->setConstant();        
      }
    }
  }
  varIter->Reset();
  
  cout << " ######################################################## " << endl;
  
  
  while( (var=(RooRealVar*)varIter->Next()) ) {
    cout << " --------> var->GetName(): "<< var->GetName() << endl;
    var->printExtras(cout);
    cout << "\n" << endl;
    var->printValue(cout);
    cout << "\n" << endl;
  }
  varIter->Reset();
  
  cout << " ######################################################## " << endl;
  
  
  outputws->Print("t");
  
  plot(analysisBranch, mass, mass_MuMu,pdf,&catIndex,data,Form("%s/mHZ_multipdf_%s",outDir.c_str(),catname.c_str()),flashggCats_,cat,bestFitPdfIndex);
  plotMuMU(analysisBranch, mass, mass_MuMu, pdf, &catIndex,data,Form("%s/mMuMU_multipdf_%s",outDir.c_str(),catname.c_str()),flashggCats_,cat,bestFitPdfIndex);

  // get normalizations
// read a JSON file
  std::ifstream inBckgNormalization("ftestOutput2D/bckgNormalization.json");
  json bckgNormalization;
  inBckgNormalization >> bckgNormalization;
  runFitExt(analysisBranch, HZToUpsilonPhotonCat_, pdf, &catIndex, data, bestFitPdfIndex, &bckgNormalization); //bkgPdf->fitTo(*data,Save(true),RooFit::Minimizer("Minuit2","minimize"));
  cout << bckgNormalization.dump(4) << endl;
  std::ofstream bckgNormalizationFile("ftestOutput2D/bckgNormalization.json");
  bckgNormalizationFile << bckgNormalization.dump(4);

  
}

}
if (saveMultiPdf){
  outputfile->cd();
  outputws->Write();
  outputfile->Close();
}

FILE *dfile = fopen(datfile.c_str(),"w");
cout << "[RESULT] Recommended options" << endl;

for (int cat=startingCategory; cat<ncats; cat++){
  cout << "Cat " << cat << endl;
  fprintf(dfile,"cat=%d\n",cat);
  for (map<string,int>::iterator it=choices_vec[cat-startingCategory].begin(); it!=choices_vec[cat-startingCategory].end(); it++){
    cout << "\t" << it->first << " - " << it->second << endl;
    fprintf(dfile,"truth=%s:%d:%s%d\n",it->first.c_str(),it->second,namingMap[it->first].c_str(),it->second);
  }
  for (map<string,std::vector<int> >::iterator it=choices_envelope_vec[cat-startingCategory].begin(); it!=choices_envelope_vec[cat-startingCategory].end(); it++){
    std::vector<int> ords = it->second;
    for (std::vector<int>::iterator ordit=ords.begin(); ordit!=ords.end(); ordit++){
      fprintf(dfile,"paul=%s:%d:%s%d\n",it->first.c_str(),*ordit,namingMap[it->first].c_str(),*ordit);
    }
  }
  fprintf(dfile,"\n");
}
inFile->Close();

// cout << "------> anaysisBranch: " << analysisBranch << endl;


return 0;
}
