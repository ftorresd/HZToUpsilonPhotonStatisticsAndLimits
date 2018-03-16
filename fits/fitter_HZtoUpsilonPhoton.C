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


// #include "../plugins/tdrstyle.C"
#include "plugins/tdrstyle.C"
// #include "plugins/RooDCBShape/RooDCBShape.h"
// #include "../../HiggsAnalysis/CombinedLimit/interface/RooDoubleCBFast.h"
#include "../../HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"


#include "plugins/getEffSigma.h"
#include "plugins/json.hpp"
using json = nlohmann::json;

// #ifdef __CINT__
// #pragma link C++ class RooDCBShape+;
// #endif

using namespace RooFit;
using namespace std;



json effSigmaJSON;


////////////////////////////////////////////////////////////////////////////////////
// 1D Fit
// Z Fit
void ZToJPsiUpsilonPhotonSignalAndBackgroundFit(string analysisBranch, string selCategory) {
	setTDRStyle();
	

	//////////////////////////////////////////////////////////////////////////////////////
	// LOAD DATA 
	TFile * outTreeToFitFileData;
	if (analysisBranch == "JPsi") outTreeToFitFileData = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_Data_ZtoJPsi_"+selCategory+"_ZZZZZ.root").c_str());
	if (analysisBranch == "Upsilon") outTreeToFitFileData = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_Data_ZtoUpsilon_"+selCategory+"_ZZZZZ.root").c_str());
	auto * outTreeToFitData = (TTree*)outTreeToFitFileData->Get("outTree_HZtoUpsilonPhoton");
	// RooRealVar mHZData("mHZ", "mHZ", 70, 120, "GeV") ;
	// RooRealVar mHZData("CMS_hgg_mass", "CMS_hgg_mass", 70, 120, "GeV") ;
	RooRealVar mHZData("CMS_hgg_mass", "CMS_hgg_mass", 70, 120, "GeV") ;
	mHZData.setRange("LS", 70, 80);
	mHZData.setRange("RS", 100, 120);
	// RooRealVar mHZData("mHZ", "mHZ", 80, 100, "GeV") ;
	// RooRealVar weightsData("mHZWeight", "mHZWeight", -100, 100, "");
	outTreeToFitData->GetBranch("mHZ")->SetName("CMS_hgg_mass");	
	RooDataSet data("data", "", RooArgSet(mHZData), Import(*outTreeToFitData)); 

	// data.Print("v");
	// for (int i = 0 ; i <190 ; i++) {
	// 	data.get(i)->Print("v");
	// }
	
	// data.PrintValue();


	// SIGNAL HISTO
	TFile * outHistoSignalFile;
	TH1D * outHistoSignal;
	if (analysisBranch == "JPsi")  {
		outHistoSignalFile = TFile::Open(("inputData/outputHistos/outHistos_HZtoUpsilonPhoton_ZToJPsiGamma_ZtoJPsi_"+selCategory+"_ZZZZZ.root").c_str());
		outHistoSignal = (TH1D*)outHistoSignalFile->Get("outputHistos/withKinCutsHistos/h_withKin_Z_Mass");
	}
	if (analysisBranch == "Upsilon")  {
		outHistoSignalFile = TFile::Open(("inputData/outputHistos/outHistos_HZtoUpsilonPhoton_ZToUpsilon1SGamma_ZtoUpsilon_"+selCategory+"_ZZZZZ.root").c_str());
		outHistoSignal = (TH1D*)outHistoSignalFile->Get("outputHistos/withKinCutsHistos/h_withKin_Z_Mass");
	}

	////////////////////////////////////////////////////////////////////////////////////
	// BACKGROUND MODEL
	// RooRealVar p0("p0", " ", 0, 10); // coefficient of x^0 term
	// RooRealVar p1("p1", " ", 0, 10); // coefficient of x^1 term
	// RooRealVar p2("p2", " ", 0, 10); // coefficient of x^2 term
	// RooRealVar p3("p3", " ", 0, 10); // coefficient of x^3 term
	// RooRealVar p4("p4", " ", 0, 10); // coefficient of x^3 term
	// RooRealVar p0("p0", " ", 1, 0., 10); // coefficient of x^0 term
	// RooRealVar p1("p1", " ", 1, 0., 10); // coefficient of x^1 term
	// RooRealVar p2("p2", " ", 1, 0., 10); // coefficient of x^2 term
	// RooRealVar p3("p3", " ", 1, 0., 10); // coefficient of x^3 term

	// JPsi - Default
	RooRealVar p0("p0", " ", 1, 0., 10); // coefficient of x^0 term
	RooRealVar p1("p1", " ", 1, 0., 10); // coefficient of x^1 term
	RooRealVar p2("p2", " ", 1, 0., 10); // coefficient of x^2 term
	RooRealVar p3("p3", " ", 1, 0., 10); // coefficient of x^3 term
	RooRealVar p4("p4", " ", 1, 0., 10); // coefficient of x^3 term
	RooRealVar p5("p5", " ", 1, 0., 10); // coefficient of x^3 term
	RooRealVar p6("p6", " ", 1, 0., 10); // coefficient of x^3 term
	RooRealVar p7("p7", " ", 1, 0., 10); // coefficient of x^3 term
	RooRealVar p8("p8", " ", 1, 0., 10); // coefficient of x^3 term
	RooRealVar p9("p9", " ", 1, 0., 10); // coefficient of x^3 term
	RooRealVar p10("p10", " ", 1, 0., 10); // coefficient of x^3 term


	// RooBernstein Bernstein("Bernstein", " ", mHZData, argumentsList);
	// RooRealVar b("BernsteinBackground", "", 100, 0, 10000);

	RooArgList argumentsList(RooConst(1), p1, p2, p3);
	
	// Upsilon params
	if (analysisBranch == "Upsilon") {
		argumentsList.add(p4);
		argumentsList.add(p5);
		// argumentsList.add(p6);
		// argumentsList.add(p7);
		// argumentsList.add(p8);
		// argumentsList.add(p9);
		// argumentsList.add(p10);
		// p1.setVal(5) ;
		// p2.setVal(5) ;
		// p3.setVal(5) ;
		// p4.setVal(5) ;
		// p5.setVal(5) ;
		// p2.setVal(1) ;
		// p3.setVal(1) ;
		// p4.setVal(1) ;
		// p5.setVal(1) ;

	}

	RooBernstein Bernstein("Bernstein", " ", mHZData, argumentsList);

	cout << "\n\n---------------------------------------------------------------------------------------------------> Begin Background Fit (" + analysisBranch + " - " + selCategory + ")\n\n" << endl;
	// RooFitResult * fitResultBackground = Bernstein.fitTo(data, Save(kTRUE), Range("LS,RS"));
	RooFitResult * fitResultBackground = Bernstein.fitTo(data, Save(kTRUE) );
	fitResultBackground->Print();
	cout << "\n\n---------------------------------------------------------------------------------------------------> End Background Fit (" + analysisBranch + " - " + selCategory + ")\n\n" << endl;



  	////////////////////////////////////////////////////////////////////////////////////
  	// BAKCGROUND PLOT 
	auto c1 = new TCanvas("c1","c1",1050*2.0,750*2.0);
	gPad->SetLeftMargin(0.17); 
	gPad->SetRightMargin(0.05); 
	gPad->SetTopMargin(0.08);

	// mHZData.setRange("leftSide",70,80) ;
	// mHZData.setRange("rightSide",100,120);
	RooPlot* xframe1 = mHZData.frame(Title("M_{#mu#mu#gamma}")) ;
	data.plotOn(xframe1,Binning(25),CutRange("LS"),RooFit::Name("data"), MarkerSize(3)) ; 
	data.plotOn(xframe1,Binning(25),CutRange("RS"),RooFit::Name("data"), MarkerSize(3)) ; 
	// data.plotOn(xframe1,Binning(20),CutRange("RS"), MarkerSize(3)) ;
	Bernstein.plotOn(xframe1,RooFit::Name("Bernstein"),VisualizeError(*fitResultBackground,2),FillColor(kYellow), Range("full"), NormRange("full") ) ;
	Bernstein.plotOn(xframe1,RooFit::Name("Bernstein"),VisualizeError(*fitResultBackground,1),FillColor(kGreen), Range("full"), NormRange("full") ) ;
	Bernstein.plotOn(xframe1,RooFit::Name("Bernstein"),LineColor(kAzure+7), Range("full"), NormRange("full")   );
	// Bernstein.plotOn(xframe1,RooFit::Name("Bernstein"),LineColor(kAzure+7) );
	// Bernstein.plotOn(xframe1,RooFit::Name("Bernstein"),LineColor(kAzure+7)   );
	// Bernstein.paramOn(xframe1,Layout(0.65,0.92,0.89));
	// data.plotOn(xframe1,Binning(25),RooFit::Name("data"), MarkerSize(3)) ; 
	data.plotOn(xframe1,Binning(25),CutRange("LS"),RooFit::Name("data"), MarkerSize(3)) ; 
	data.plotOn(xframe1,Binning(25),CutRange("RS"),RooFit::Name("data"), MarkerSize(3)) ;
	xframe1->SetMinimum(0.00001);
	xframe1->GetXaxis()->SetTitle("M_{#mu#mu#gamma} (GeV)");
	xframe1->GetXaxis()->SetLabelOffset(0.02);
	xframe1->Draw();


	//signal yield
	outHistoSignal->SetLineWidth(3);
	outHistoSignal->SetLineColor(kOrange+8);
	outHistoSignal->SetLineStyle(1);
	// outHistoSignal->Scale(30.0/(outHistoSignal->Integral()));
	outHistoSignal->Scale(30.0);
	outHistoSignal->Rebin(5);
	outHistoSignal->Draw("hist same");

	// legend
	auto legend = new TLegend(0.6,0.7,0.9,0.87);
	legend->SetTextSize(0.037);
	if (analysisBranch == "JPsi") legend->SetHeader("Z #rightarrow J/#Psi + #gamma Analysis");
	if (analysisBranch == "Upsilon") legend->SetHeader("Z #rightarrow #Upsilon + #gamma Analysis");
	legend->SetBorderSize(0);
	legend->SetFillStyle(0);
	legend->AddEntry(xframe1->findObject("data"), "Data", "lpe");
	legend->AddEntry(xframe1->findObject("Bernstein"), "Background Model", "l");
	legend->AddEntry(outHistoSignal, "Expected Signal (#times 30)", "l");
	legend->Draw();

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

	if (analysisBranch == "JPsi")  {
		system(("mkdir -p  `dirname fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_Data.png`"));
		c1->SaveAs(("fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_Data_"+selCategory+".root").c_str());
		c1->SaveAs(("fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_Data_"+selCategory+".png").c_str());
		c1->SaveAs(("fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_Data_"+selCategory+".pdf").c_str());
	}

	if (analysisBranch == "Upsilon")  {
		system(("mkdir -p  `dirname fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_Data.png`"));
		c1->SaveAs(("fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_Data_"+selCategory+".root").c_str());
		c1->SaveAs(("fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_Data_"+selCategory+".png").c_str());
		c1->SaveAs(("fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_Data_"+selCategory+".pdf").c_str());
	}


	//////////////////////////////////////////////////////////////////////////////////////
	// LOAD DATA - SIGNAL
	TFile * outTreeToFitFileSignal;
	if (analysisBranch == "JPsi") outTreeToFitFileSignal = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_ZToJPsiGamma_ZtoJPsi_"+selCategory+"_ZZZZZ.root").c_str());
	if (analysisBranch == "Upsilon") outTreeToFitFileSignal = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_ZToUpsilon1SGamma_ZtoUpsilon_"+selCategory+"_ZZZZZ.root").c_str());
	auto * outTreeToFitSignal = (TTree*)outTreeToFitFileSignal->Get("outTree_HZtoUpsilonPhoton");
	RooRealVar mHZSignal("mHZ", "mHZ", 70, 120, "GeV") ;
	RooRealVar weightsSignal("mHZWeight", "mHZWeight", -100, 100, "");

	// RooDataSet signal("signal", " ", RooArgSet(mHZSignal, weightsSignal), Import(*outTreeToFitSignal), WeightVar(weightsSignal));
	outTreeToFitSignal->GetBranch("mHZ")->SetName("CMS_hgg_mass");	
	RooDataSet signal("signal", " ", RooArgSet(mHZData, weightsSignal), Import(*outTreeToFitSignal), WeightVar(weightsSignal));
	
	signal.Print();

	////////////////////////////////////////////////////////////////////////////////////
	// SIGNAL MODEL
	RooRealVar mean_dcb("mean_dcb", "Mean" ,91.1876,70,120) ;
	RooRealVar sigma_dcb("sigma_dcb", "Width" ,  2., 0.5, 4.) ;
	// RooRealVar sigma_dcb("sigma_dcb", "Width" ,  2.4952) ;
	// ///////////////
	// RooRealVar n1("n1","", 0.5, 0.1, 50.);//dCBPowerL
	// RooRealVar n2("n2","", 0.5, 0.1, 50.);//dCBPowerR
	// RooRealVar alpha1("alpha1","", 3., 0.1, 50.);//dCBCutL
	// RooRealVar alpha2("alpha2","", 3., 0.1, 50.);//dCBCutR
	///////////////
	RooRealVar n1("n1","", 0.5, 0.1, 50.);//dCBPowerL
	RooRealVar n2("n2","", 0.5, 0.1, 50.);//dCBPowerR
	RooRealVar alpha1("alpha1","", 3., 0.0, 10.);//dCBCutL
	RooRealVar alpha2("alpha2","", 3., 0.0, 10.);//dCBCutR
	// RooRealVar s_dcb("signal_dcb", "signal", 100, 0, 10000);


	// RooDCBShape dcball("dcball", "double sided crystal ball", mHZSignal, mean_dcb,sigma_dcb,alpha1,alpha2,n1,n2);
	RooDoubleCB dcball("dcball", "double sided crystal ball", mHZData, mean_dcb,sigma_dcb,alpha1,n1,alpha2,n2);


	cout << "\n\n---------------------------------------------------------------------------------------------------> Begin Signal Fit (" + analysisBranch + " - " + selCategory + ")\n\n" << endl;
	RooFitResult * fitResultSignal = dcball.fitTo(signal, RooFit::Minimizer("Minuit","minimize"), Save(kTRUE), SumW2Error(kTRUE) ) ;
	// RooFitResult * fitResultSignal = dcball.fitTo(signal, Save(kTRUE), SumW2Error(kFALSE) ) ;
	fitResultSignal->Print();
	cout << "\n\n---------------------------------------------------------------------------------------------------> End Signal Fit (" + analysisBranch + " - " + selCategory + ")\n\n" << endl;


  	////////////////////////////////////////////////////////////////////////////////////
  	// SIGNAL PLOT
	auto cSignal = new TCanvas("c2","c2",1050*2.0,750*2.0);
	gPad->SetLeftMargin(0.17); 
	gPad->SetRightMargin(0.05); 
	gPad->SetTopMargin(0.08);
	RooPlot* xframeSignal = mHZData.frame(Title("M_{#mu#mu#gamma}")) ;
	// getEffSigma(RooRealVar *mass, RooAbsPdf *pdf, double wmin=110., double wmax=130., double step=0.002, double epsilon=1.e-4){
	signal.plotOn(xframeSignal,Binning(50),RooFit::Name("signal"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 


	pair<double,double> effSigma_ZSignal = getEffSigma(&mHZData, &dcball, 70, 120);
	effSigmaJSON["ZTo"+analysisBranch+"Photon"][selCategory]["signal"] = 0.5*(effSigma_ZSignal.second-effSigma_ZSignal.first);
	mHZData.setRange("HZRange",70,120);
	dcball.plotOn(xframeSignal,NormRange("HZRange"),Range(effSigma_ZSignal.first,effSigma_ZSignal.second),FillColor(19),DrawOption("F"),LineWidth(2),FillStyle(1001),VLines(),LineColor(15));
	TObject *seffLeg = xframeSignal->getObject(int(xframeSignal->numItems()-1));
	dcball.plotOn(xframeSignal,NormRange("HZRange"),Range(effSigma_ZSignal.first,effSigma_ZSignal.second),LineColor(15),LineWidth(2),FillStyle(1001),VLines());

	dcball.plotOn(xframeSignal,RooFit::Name("dcball"),LineColor(kOrange+8));
	signal.plotOn(xframeSignal,Binning(50),RooFit::Name("signal"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
	// dcball.paramOn(xframeSignal,Layout(0.65,0.92,0.89));



	xframeSignal->SetMinimum(0.00001);
	xframeSignal->GetXaxis()->SetTitle("M_{#mu#mu#gamma} (GeV)");
	xframeSignal->GetXaxis()->SetLabelOffset(0.02);
	xframeSignal->Draw();

	// legend
	auto legendSignal = new TLegend(0.6,0.7,0.9,0.87);
	legendSignal->SetTextSize(0.037);
	if (analysisBranch == "JPsi") legendSignal->SetHeader("Z #rightarrow J/#Psi + #gamma Analysis");
	if (analysisBranch == "Upsilon") legendSignal->SetHeader("Z #rightarrow #Upsilon + #gamma Analysis");
	legendSignal->SetBorderSize(0);
	legendSignal->SetFillStyle(0);
	legendSignal->AddEntry(xframeSignal->findObject("signal"), "Signal MC", "lpe");
	legendSignal->AddEntry(xframeSignal->findObject("dcball"), "Signal Model", "l");
	// legendSignal->AddEntry(outHistoSignal, "Expected Signal (#times 30)", "l");

	// pair<double,double> effSigma_ZSignal = getEffSigma(&mHZSignal, &dcball, 70, 120);
	// effSigmaJSON["ZTo"+analysisBranch+"Photon"][selCategory]["signal"] = 0.5*(effSigma_ZSignal.second-effSigma_ZSignal.first);
	
	// legendSignal->AddEntry((TObject*)0,Form("#sigma_{eff} = %1.2f GeV",0.5*(effSigma_ZSignal.second-effSigma_ZSignal.first)),"");
	legendSignal->AddEntry(seffLeg,Form("#sigma_{eff} = %1.2f GeV",0.5*(effSigma_ZSignal.second-effSigma_ZSignal.first)),"fl");
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
	latexSignal->DrawLatex(.25, 0.93, "Preliminary");
	latexSignal->SetTextFont(42);
	latexSignal->SetTextSize(0.04);
	latexSignal->SetTextAlign(31);
	latexSignal->DrawLatex(0.96, 0.93, "35.86 fb^{-1} (13 TeV, 2016) ");


	if (analysisBranch == "JPsi")  {
		system(("mkdir -p  `dirname fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_Signal.png`"));
		cSignal->SaveAs(("fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_Signal_"+selCategory+".root").c_str());
		cSignal->SaveAs(("fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_Signal_"+selCategory+".png").c_str());
		cSignal->SaveAs(("fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_Signal_"+selCategory+".pdf").c_str());
	}

	if (analysisBranch == "Upsilon")  {
		system(("mkdir -p  `dirname fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_Signal.png`"));
		cSignal->SaveAs(("fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_Signal_"+selCategory+".root").c_str());
		cSignal->SaveAs(("fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_Signal_"+selCategory+".png").c_str());
		cSignal->SaveAs(("fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_Signal_"+selCategory+".pdf").c_str());
	}

	//////////////////////////////////////////////////////////////////////////////////////
	// LOAD DATA - PEAKING BACKGROUND
	TFile * outTreeToFitFilePeakingBackground;
	if (analysisBranch == "JPsi") outTreeToFitFilePeakingBackground = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_ZGTo2MuG_MMuMu-2To15_ZtoJPsi_"+selCategory+"_ZZZZZ.root").c_str());
	if (analysisBranch == "Upsilon") outTreeToFitFilePeakingBackground = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_ZGTo2MuG_MMuMu-2To15_ZtoUpsilon_"+selCategory+"_ZZZZZ.root").c_str());
	auto * outTreeToFitPeakingBackground = (TTree*)outTreeToFitFilePeakingBackground->Get("outTree_HZtoUpsilonPhoton");
	RooRealVar mHZPeakingBackground("mHZ", "mHZ", 70, 120, "GeV") ;
	RooRealVar weightsPeakingBackground("mHZWeight", "mHZWeight", -100, 100, "");

	// RooDataSet peakingBackground("peakingBackground", " ", RooArgSet(mHZPeakingBackground, weightsPeakingBackground), Import(*outTreeToFitPeakingBackground), WeightVar(weightsPeakingBackground));
	outTreeToFitPeakingBackground->GetBranch("mHZ")->SetName("CMS_hgg_mass");
	RooDataSet peakingBackground("peakingBackground", " ", RooArgSet(mHZData, weightsPeakingBackground), Import(*outTreeToFitPeakingBackground), WeightVar(weightsPeakingBackground));
	
	peakingBackground.Print();

	////////////////////////////////////////////////////////////////////////////////////
	// PEAKING BACKGROUND
	RooRealVar mean_dcbPeakingBackground("mean_dcbPeakingBackground", "Mean" ,91.1876,70,120) ;
	RooRealVar sigma_dcbPeakingBackground("sigma_dcbPeakingBackground", "Width" ,  2., 0.5, 4.) ;
	RooRealVar n1PeakingBackground("n1PeakingBackground","", 0.5, 0.1, 50.);//dCBPowerL
	// RooRealVar n2("n2","", 1., 0.1, 50.);//dCBPowerR
	RooRealVar n2PeakingBackground("n2PeakingBackground","", 0.5, 0.1, 50.);//dCBPowerR
	RooRealVar alpha1PeakingBackground("alpha1PeakingBackground","", 3., 0.0, 10.);//dCBCutL
	RooRealVar alpha2PeakingBackground("alpha2PeakingBackground","", 3., 0.0, 10.);//dCBCutR



	// RooRealVar s_dcb("signal_dcb", "signal", 100, 0, 10000);


	// RooDCBShape dcballPeakingBackground(
	// 	"dcballPeakingBackground", 
	// 	"double sided crystal ball", 
	// 	mHZPeakingBackground, 
	// 	mean_dcbPeakingBackground,
	// 	sigma_dcbPeakingBackground,
	// 	alpha1PeakingBackground,
	// 	alpha2PeakingBackground,
	// 	n1PeakingBackground,
	// 	n2PeakingBackground
	// 	);

	RooDoubleCB dcballPeakingBackground(
		"dcballPeakingBackground", 
		"double sided crystal ball", 
		mHZData, 
		mean_dcbPeakingBackground,
		sigma_dcbPeakingBackground,
		alpha1PeakingBackground,
		n1PeakingBackground,
		alpha2PeakingBackground,
		n2PeakingBackground
		);


	cout << "\n\n---------------------------------------------------------------------------------------------------> Begin Peaking Background Fit (" + analysisBranch + " - " + selCategory + ")\n\n" << endl;
	RooFitResult * fitResultPeakingBackground = dcballPeakingBackground.fitTo(peakingBackground, RooFit::Minimizer("Minuit","minimize"), Save(kTRUE), SumW2Error(kTRUE) ) ;
	fitResultPeakingBackground->Print();
	cout << "\n\n---------------------------------------------------------------------------------------------------> End Peaking Background Fit (" + analysisBranch + " - " + selCategory + ")\n\n" << endl;


  	////////////////////////////////////////////////////////////////////////////////////
  	// Peaking Background PLOT
	auto cPeakingBackground = new TCanvas("cPeakingBackground","cPeakingBackground",1050*2.0,750*2.0);
	gPad->SetLeftMargin(0.17); 
	gPad->SetRightMargin(0.05); 
	gPad->SetTopMargin(0.08);
	RooPlot* xframePeakingBackground = mHZData.frame(Title("M_{#mu#mu#gamma}")) ;
	peakingBackground.plotOn(xframePeakingBackground,Binning(50),RooFit::Name("peakingBackground"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
	// dcballPeakingBackground.plotOn(xframePeakingBackground,RooFit::Name("dcballPeakingBackground"),LineColor(kAzure+7));
	dcballPeakingBackground.plotOn(xframePeakingBackground,RooFit::Name("dcballPeakingBackground"),LineColor(kAzure+7));
	peakingBackground.plotOn(xframePeakingBackground,Binning(50),RooFit::Name("peakingBackground"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
	// dcballPeakingBackground.paramOn(xframePeakingBackground,Layout(0.65,0.92,0.89));



	xframePeakingBackground->SetMinimum(0.00001);
	xframePeakingBackground->GetXaxis()->SetTitle("M_{#mu#mu#gamma} (GeV)");
	xframePeakingBackground->GetXaxis()->SetLabelOffset(0.02);
	xframePeakingBackground->Draw();

	// legend
	auto legendPeakingBackground = new TLegend(0.6,0.7,0.9,0.87);
	legendPeakingBackground->SetTextSize(0.037);
	if (analysisBranch == "JPsi") legendPeakingBackground->SetHeader("Z #rightarrow J/#Psi + #gamma Analysis");
	if (analysisBranch == "Upsilon") legendPeakingBackground->SetHeader("Z #rightarrow #Upsilon + #gamma Analysis");
	legendPeakingBackground->SetBorderSize(0);
	legendPeakingBackground->SetFillStyle(0);
	legendPeakingBackground->AddEntry(xframePeakingBackground->findObject("peakingBackground"), "Z #rightarrow #mu#mu + #gamma MC", "lpe");
	legendPeakingBackground->AddEntry(xframePeakingBackground->findObject("dcballPeakingBackground"), "Peaking Background Model", "l");
	// legendSignal->AddEntry(outHistoSignal, "Expected Signal (#times 30)", "l");
	
	pair<double,double> effSigma_ZPBckg = getEffSigma(&mHZData, &dcballPeakingBackground, 70, 120);
	effSigmaJSON["ZTo"+analysisBranch+"Photon"][selCategory]["peakingBackground"] = 0.5*(effSigma_ZSignal.second-effSigma_ZSignal.first);
	
	// legendPeakingBackground->AddEntry((TObject*)0,Form("#sigma_{eff} = %1.2f GeV",0.5*(effSigma_ZPBckg.second-effSigma_ZPBckg.first)),"");
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
	latexPeakingBackground->DrawLatex(.25, 0.93, "Preliminary");
	latexPeakingBackground->SetTextFont(42);
	latexPeakingBackground->SetTextSize(0.04);
	latexPeakingBackground->SetTextAlign(31);
	latexPeakingBackground->DrawLatex(0.96, 0.93, "35.86 fb^{-1} (13 TeV, 2016) ");

	if (analysisBranch == "JPsi") {
		system(("mkdir -p  `dirname fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_PeakingBackground.png`"));
		cPeakingBackground->SaveAs(("fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".root").c_str());
		cPeakingBackground->SaveAs(("fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".png").c_str());
		cPeakingBackground->SaveAs(("fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".pdf").c_str());
	}

	if (analysisBranch == "Upsilon") {
		system(("mkdir -p  `dirname fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_PeakingBackground.png`"));
		cPeakingBackground->SaveAs(("fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".root").c_str());
		cPeakingBackground->SaveAs(("fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".png").c_str());
		cPeakingBackground->SaveAs(("fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".pdf").c_str());
	}

	////////////////////////////////////////////////////////////////////////////////////
	// C r e a t e   w o r k s p a c e ,   i m p o r t   d a t a   a n d   m o d e l 
	RooWorkspace *w = new RooWorkspace("w","workspace") ;  
	
	// data
	w->import(mHZData);
	w->import(data,Rename("data_obs"));
	w->import(Bernstein);
	// signal
	// w->import(mHZSignal,Rename("mHZSignal"));
	w->import(signal,Rename("signal"));
	w->import(dcball);
	//peaking background
	// w->import(mHZPeakingBackground);
	w->import(peakingBackground,Rename("peakingBackground"));
	w->import(dcballPeakingBackground);
	w->importClassCode();

	w->var("alpha1")->setConstant(true);
	w->var("alpha1PeakingBackground")->setConstant(true);
	w->var("alpha2")->setConstant(true);
	w->var("alpha2PeakingBackground")->setConstant(true);
	w->var("mean_dcb")->setConstant(true);
	w->var("mean_dcbPeakingBackground")->setConstant(true);
	w->var("n1")->setConstant(true);
	w->var("n1PeakingBackground")->setConstant(true);
	w->var("n2")->setConstant(true);
	w->var("n2PeakingBackground")->setConstant(true);
	w->var("p1")->setConstant(true);
	w->var("p2")->setConstant(true);
	w->var("p3")->setConstant(true);
	if (analysisBranch == "Upsilon") {
		w->var("p4")->setConstant(true);
		w->var("p5")->setConstant(true);
	}
	w->var("sigma_dcb")->setConstant(true);
	w->var("sigma_dcbPeakingBackground")->setConstant(true);

	w->Print("t");

	if (analysisBranch == "JPsi") {
		system(("mkdir -p  `dirname fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_workspace.root`"));
		w->writeToFile(("fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_workspace_"+selCategory+".root").c_str());
	}

	if (analysisBranch == "Upsilon") {
		system(("mkdir -p  `dirname fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_workspace.root`"));
		w->writeToFile(("fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_workspace_"+selCategory+".root").c_str());
	}


	// For FlashGG
	// create file
	TFile * outFile_forFLASHGG;
	if (analysisBranch == "JPsi") outFile_forFLASHGG = TFile::Open(("fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_workspace_forFLASHGG_"+selCategory+".root").c_str(),"RECREATE");
	if (analysisBranch == "Upsilon") outFile_forFLASHGG = TFile::Open(("fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_workspace_forFLASHGG_"+selCategory+".root").c_str(),"RECREATE");
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




  }

////////////////////////////////////////////////////////////////////////////////////
// Higgs Fit
void HToJPsiUpsilonPhotonSignalAndBackgroundFit(string analysisBranch, string selCategory) {
  	setTDRStyle();

	//////////////////////////////////////////////////////////////////////////////////////
	// LOAD DATA 
  	TFile * outTreeToFitFileData;
  	if (analysisBranch == "JPsi") outTreeToFitFileData = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_Data_HtoJPsi_"+selCategory+"_ZZZZZ.root").c_str());
  	if (analysisBranch == "Upsilon") outTreeToFitFileData = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_Data_HtoUpsilon_"+selCategory+"_ZZZZZ.root").c_str());
  	auto * outTreeToFitData = (TTree*)outTreeToFitFileData->Get("outTree_HZtoUpsilonPhoton");
  	// RooRealVar mHZData("mHZ", "mHZ", 100, 150, "GeV") ;
  	RooRealVar mHZData("CMS_hgg_mass", "CMS_hgg_mass", 100, 150, "GeV") ;
  	mHZData.setRange("LS", 100, 115);
  	mHZData.setRange("RS", 135, 150);
	// RooRealVar mHZData("mHZ", "mHZ", 80, 100, "GeV") ;
	// RooRealVar weightsData("mHZWeight", "mHZWeight", -100, 100, "");
	outTreeToFitData->GetBranch("mHZ")->SetName("CMS_hgg_mass");
  	RooDataSet data("data", "", RooArgSet(mHZData), Import(*outTreeToFitData)); 

	// data.Print("v");
	// for (int i = 0 ; i <190 ; i++) {
	// 	data.get(i)->Print("v");
	// }

	// data.PrintValue();


	// SIGNAL HISTO
  	TFile * outHistoSignalFile;
  	TH1D * outHistoSignal;
  	if (analysisBranch == "JPsi")  {
  		outHistoSignalFile = TFile::Open(("inputData/outputHistos/outHistos_HZtoUpsilonPhoton_HToJPsiGamma_HtoJPsi_"+selCategory+"_ZZZZZ.root").c_str());
  		outHistoSignal = (TH1D*)outHistoSignalFile->Get("outputHistos/withKinCutsHistos/h_withKin_Z_Mass");
  	}
  	if (analysisBranch == "Upsilon")  {
  		outHistoSignalFile = TFile::Open(("inputData/outputHistos/outHistos_HZtoUpsilonPhoton_HToUpsilonGamma_HtoUpsilon_"+selCategory+"_ZZZZZ.root").c_str());
  		outHistoSignal = (TH1D*)outHistoSignalFile->Get("outputHistos/withKinCutsHistos/h_withKin_Z_Mass");
  	}

	////////////////////////////////////////////////////////////////////////////////////
	// BACKGROUND MODEL
	// RooRealVar p0("p0", " ", 0, 10); // coefficient of x^0 term
	// RooRealVar p1("p1", " ", 0, 10); // coefficient of x^1 term
	// RooRealVar p2("p2", " ", 0, 10); // coefficient of x^2 term
	// RooRealVar p3("p3", " ", 0, 10); // coefficient of x^3 term
	// RooRealVar p4("p4", " ", 0, 10); // coefficient of x^3 term
	RooRealVar p0("p0", " ", 5, 0., 10); // coefficient of x^0 term
	RooRealVar p1("p1", " ", 5, 0., 10); // coefficient of x^1 term
	RooRealVar p2("p2", " ", 5, 0., 10); // coefficient of x^2 term
	RooRealVar p3("p3", " ", 5, 0., 10); // coefficient of x^3 term
	RooRealVar p4("p4", " ", 5, 0., 10); // coefficient of x^4 term
	RooRealVar p5("p5", " ", 5, 0., 10); // coefficient of x^5 term
	RooRealVar p6("p6", " ", 5, 0., 10); // coefficient of x^6 term
	RooRealVar p7("p7", " ", 5, 0., 10); // coefficient of x^7 term
	RooRealVar p8("p8", " ", 5, 0., 10); // coefficient of x^8 term
	RooRealVar p9("p9", " ", 5, 0., 10); // coefficient of x^9 term
	RooRealVar p10("p01", " ", 5, 0., 10); // coefficient of x^10 term

	// RooBernstein Bernstein("Bernstein", " ", mHZData, RooArgList(p0, p1, p2));
	RooArgList argumentsList(RooConst(1), p1, p2);
	
	// RooBernstein Bernstein("Bernstein", " ", mHZData, RooArgList(RooConst(1), p1, p2, p3));
	// RooBernstein Bernstein("Bernstein", " ", mHZData, RooArgList(RooConst(1), RooConst(1), RooConst(1.12), p3));
	// RooBernstein Bernstein("Bernstein", " ", mHZData, RooArgList(RooConst(1), p1, p2));
	// RooBernstein Bernstein("Bernstein", " ", mHZData, RooArgList(RooConst(1), p1 ) );
	// RooRealVar b("BernsteinBackground", "", 100, 0, 10000);



	// Upsilon params
	if (analysisBranch == "Upsilon") {
		// argumentsList.add(p3);
		// argumentsList.add(p4);
		// argumentsList.add(p5);
		// argumentsList.add(p6);
		// argumentsList.add(p7);
		// argumentsList.add(p8);
		// argumentsList.add(p9);
		// argumentsList.add(p10);
		// p1.setVal(1) ;
		// p2.setVal(1) ;
		// p3.setVal(1) ;
		// p4.setVal(1) ;
		// p5.setVal(1) ;

	}

	RooBernstein Bernstein("Bernstein", " ", mHZData, argumentsList);

	cout << "\n\n---------------------------------------------------------------------------------------------------> Begin Background Fit (" + analysisBranch + " - " + selCategory + ")\n\n" << endl;
	// RooFitResult * fitResultBackground = Bernstein.fitTo(data, Save(kTRUE), Range("LS,RS"));
	RooFitResult * fitResultBackground = Bernstein.fitTo(data, Save(kTRUE) );
	fitResultBackground->Print();
	cout << "\n\n---------------------------------------------------------------------------------------------------> End Background Fit (" + analysisBranch + " - " + selCategory + ")\n\n" << endl;



  	////////////////////////////////////////////////////////////////////////////////////
  	// BAKCGROUND PLOT 
	auto c1 = new TCanvas("c1","c1",1050*2.0,750*2.0);
	gPad->SetLeftMargin(0.17); 
	gPad->SetRightMargin(0.05); 
	gPad->SetTopMargin(0.08);

	// mHZData.setRange("leftSide",70,80) ;
	// mHZData.setRange("rightSide",100,120);
	RooPlot* xframe1 = mHZData.frame(Title("M_{#mu#mu#gamma}")) ;
	data.plotOn(xframe1,Binning(25),CutRange("LS"),RooFit::Name("data"), MarkerSize(3)) ; 
	data.plotOn(xframe1,Binning(25),CutRange("RS"),RooFit::Name("data"), MarkerSize(3)) ; 
	// data.plotOn(xframe1,Binning(20),CutRange("RS"), MarkerSize(3)) ;
	Bernstein.plotOn(xframe1,RooFit::Name("Bernstein"),VisualizeError(*fitResultBackground,2),FillColor(kYellow), Range("full"), NormRange("full") ) ;
	Bernstein.plotOn(xframe1,RooFit::Name("Bernstein"),VisualizeError(*fitResultBackground,1),FillColor(kGreen), Range("full"), NormRange("full") ) ;
	Bernstein.plotOn(xframe1,RooFit::Name("Bernstein"),LineColor(kAzure+7), Range("full"), NormRange("full")   );
	// Bernstein.plotOn(xframe1,RooFit::Name("Bernstein"),LineColor(kAzure+7)   );
	// Bernstein.paramOn(xframe1,Layout(0.65,0.92,0.89));
	// data.plotOn(xframe1,Binning(25),RooFit::Name("data"), MarkerSize(3)) ; 
	data.plotOn(xframe1,Binning(25),CutRange("LS"),RooFit::Name("data"), MarkerSize(3)) ; 
	data.plotOn(xframe1,Binning(25),CutRange("RS"),RooFit::Name("data"), MarkerSize(3)) ;
	xframe1->SetMinimum(0.00001);
	xframe1->GetXaxis()->SetTitle("M_{#mu#mu#gamma} (GeV)");
	xframe1->GetXaxis()->SetLabelOffset(0.02);
	xframe1->Draw();


	//signal yield
	outHistoSignal->SetLineWidth(3);
	outHistoSignal->SetLineColor(kOrange+8);
	outHistoSignal->SetLineStyle(1);
	// outHistoSignal->Scale(30.0/(outHistoSignal->Integral()));
	if (analysisBranch == "JPsi") outHistoSignal->Scale(120.0);
	if (analysisBranch == "Upsilon") outHistoSignal->Scale(100000.0);
	outHistoSignal->Rebin(5);
	outHistoSignal->Draw("hist same");

	// legend
	auto legend = new TLegend(0.6,0.7,0.9,0.87);
	legend->SetTextSize(0.037);
	if (analysisBranch == "JPsi") legend->SetHeader("H #rightarrow J/#Psi + #gamma Analysis");
	if (analysisBranch == "Upsilon") legend->SetHeader("H #rightarrow #Upsilon + #gamma Analysis");
	legend->SetBorderSize(0);
	legend->SetFillStyle(0);
	legend->AddEntry(xframe1->findObject("data"), "Data", "lpe");
	legend->AddEntry(xframe1->findObject("Bernstein"), "Background Model", "l");
	if (analysisBranch == "JPsi") legend->AddEntry(outHistoSignal, "Expected Signal (#times 120)", "l");
	if (analysisBranch == "Upsilon") legend->AddEntry(outHistoSignal, "Expected Signal (#times 100000)", "l");
	legend->Draw();

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

	if (analysisBranch == "JPsi")  {
		system(("mkdir -p  `dirname fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_Data_"+selCategory+".png`").c_str());
		c1->SaveAs(("fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_Data_"+selCategory+".root").c_str());
		c1->SaveAs(("fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_Data_"+selCategory+".png").c_str());
		c1->SaveAs(("fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_Data_"+selCategory+".pdf").c_str());
	}

	if (analysisBranch == "Upsilon")  {
		system(("mkdir -p  `dirname fitPlotFiles/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilonPhotonSignalAndBackgroundFit_Data_"+selCategory+".png`").c_str());
		c1->SaveAs(("fitPlotFiles/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilonPhotonSignalAndBackgroundFit_Data_"+selCategory+".root").c_str());
		c1->SaveAs(("fitPlotFiles/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilonPhotonSignalAndBackgroundFit_Data_"+selCategory+".png").c_str());
		c1->SaveAs(("fitPlotFiles/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilonPhotonSignalAndBackgroundFit_Data_"+selCategory+".pdf").c_str());
	}


	//////////////////////////////////////////////////////////////////////////////////////
	// LOAD DATA - SIGNAL
	TFile * outTreeToFitFileSignal;
	if (analysisBranch == "JPsi") outTreeToFitFileSignal = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_HToJPsiGamma_HtoJPsi_"+selCategory+"_ZZZZZ.root").c_str());
	if (analysisBranch == "Upsilon") outTreeToFitFileSignal = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_HToUpsilonGamma_HtoUpsilon_"+selCategory+"_ZZZZZ.root").c_str());
	auto * outTreeToFitSignal = (TTree*)outTreeToFitFileSignal->Get("outTree_HZtoUpsilonPhoton");
	RooRealVar mHZSignal("mHZ", "mHZ", 100, 150, "GeV") ;
	RooRealVar weightsSignal("mHZWeight", "mHZWeight", -100, 100, "");

	// RooDataSet signal("signal", " ", RooArgSet(mHZSignal, weightsSignal), Import(*outTreeToFitSignal), WeightVar(weightsSignal));
	outTreeToFitSignal->GetBranch("mHZ")->SetName("CMS_hgg_mass");
	RooDataSet signal("signal", " ", RooArgSet(mHZData, weightsSignal), Import(*outTreeToFitSignal), WeightVar(weightsSignal));
	
	signal.Print();

	////////////////////////////////////////////////////////////////////////////////////
	// SIGNAL MODEL
	// CB params

	// JPsi - Default
	RooRealVar mean_Higgs("mean_Higgs", "Mean" ,125,100,150) ;
	RooRealVar sigma_cb("sigma_cb", "Width", 2., 0.5, 4.) ;
	RooRealVar n("n","", 0.5, 0.1, 50.);
	RooRealVar alpha_cb("alpha_cb","", 3., 0.1, 50.);
	RooRealVar sigma_gauss("sigma_gauss", "Width" ,  2., 0.5, 4.) ;

	// Upsilon params
	if (analysisBranch == "Upsilon") {
		sigma_cb.setRange(0.5, 3.0) ;
		sigma_cb.setVal(1.5) ;

		alpha_cb.setRange(0.01, 10.);
		alpha_cb.setVal(1.5);
		
		n.setRange(0.1, 10.);
		n.setVal(4.);
		
		sigma_gauss.setRange(0.5, 4.) ;
		sigma_gauss.setVal(2.) ;
	}


	RooCBShape signal_cb("signal_cb", "Crystal Ball Signal component", mHZData, mean_Higgs, sigma_cb, alpha_cb, n);
	RooGaussian signal_gauss("signal_gauss","Gaussian Signal component", mHZData, mean_Higgs, sigma_gauss) ;

	// add gaussian to CB
	RooRealVar cb_fraction("cb_fraction", "cb_fraction",  0.9, 0.0, 100.0);
	RooAddPdf signal_model("signal_model", "signal_model", RooArgList(signal_cb, signal_gauss), cb_fraction); 


	cout << "\n\n---------------------------------------------------------------------------------------------------> Begin Signal Fit (" + analysisBranch + " - " + selCategory + ")\n\n" << endl;
	// RooFitResult * fitResultSignal = signal_model.fitTo(signal, Save(kTRUE), SumW2Error(kTRUE) ) ;
	RooFitResult * fitResultSignal = signal_cb.fitTo(signal, RooFit::Minimizer("Minuit","minimize"), Save(kTRUE), SumW2Error(kTRUE) ) ;
	fitResultSignal->Print();
	cout << "\n\n---------------------------------------------------------------------------------------------------> End Signal Fit (" + analysisBranch + " - " + selCategory + ")\n\n" << endl;


  	////////////////////////////////////////////////////////////////////////////////////
  	// SIGNAL PLOT
	auto cSignal = new TCanvas("cSignal","cSignal",1050*2.0,750*2.0);
	gPad->SetLeftMargin(0.17); 
	gPad->SetRightMargin(0.05); 
	gPad->SetTopMargin(0.08);
	RooPlot* xframeSignal = mHZData.frame(Title("M_{#mu#mu#gamma}")) ;
	signal.plotOn(xframeSignal,Binning(50),RooFit::Name("signal"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
	// dcball.plotOn(xframeSignal,RooFit::Name("dcball"),LineColor(kAzure+7));

	pair<double,double> effSigma_HSignal = getEffSigma(&mHZData, &signal_cb, 100, 150);
	effSigmaJSON["HTo"+analysisBranch+"Photon"][selCategory]["signal"] = 0.5*(effSigma_HSignal.second-effSigma_HSignal.first);
	mHZData.setRange("HZRange",100,150);
	signal_model.plotOn(xframeSignal,NormRange("HZRange"),Range(effSigma_HSignal.first,effSigma_HSignal.second),FillColor(19),DrawOption("F"),LineWidth(2),FillStyle(1001),VLines(),LineColor(15));
	TObject *seffLeg = xframeSignal->getObject(int(xframeSignal->numItems()-1));
	signal_model.plotOn(xframeSignal,NormRange("HZRange"),Range(effSigma_HSignal.first,effSigma_HSignal.second),LineColor(15),LineWidth(2),FillStyle(1001),VLines());

	signal_model.plotOn(xframeSignal,RooFit::Name("signal_cb"), RooFit::Components(signal_cb), LineColor(kBlack), LineStyle(7));
	signal_model.plotOn(xframeSignal,RooFit::Name("signal_gauss"), RooFit::Components(signal_gauss), LineColor(kBlue), LineStyle(7));
	signal_model.plotOn(xframeSignal,RooFit::Name("signal_model"),LineColor(kOrange+8));
	// signal_cb.plotOn(xframeSignal,RooFit::Name("signal_model"),LineColor(kOrange+8));
	
	// signal_cb.plotOn(xframeSignal,RooFit::Name("signal_cb"),LineColor(kAzure+7));
	// signal_gauss.plotOn(xframeSignal,RooFit::Name("signal_gauss"),LineColor(kBlack));
	signal.plotOn(xframeSignal,Binning(50),RooFit::Name("signal"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
	// dcball.paramOn(xframeSignal,Layout(0.65,0.92,0.89));



	// xframeSignal->SetMinimum(0.00000001);
	xframeSignal->SetMinimum(0.0);
	xframeSignal->GetXaxis()->SetTitle("M_{#mu#mu#gamma} (GeV)");
	xframeSignal->GetXaxis()->SetLabelOffset(0.02);
	xframeSignal->Draw();

	// legend
	auto legendSignal = new TLegend(0.6,0.7,0.9,0.87);
	//  TLegend *leg = new TLegend(0.15+offset,0.40,0.5+offset,0.82);
  	legendSignal->SetTextSize(0.037);


	if (analysisBranch == "JPsi") legendSignal->SetHeader("H #rightarrow J/#Psi + #gamma Analysis");
	if (analysisBranch == "Upsilon") legendSignal->SetHeader("H #rightarrow #Upsilon + #gamma Analysis");
	legendSignal->SetBorderSize(0);
	legendSignal->SetFillStyle(0);
	legendSignal->AddEntry(xframeSignal->findObject("signal"), "Signal MC", "lpe");
	legendSignal->AddEntry(xframeSignal->findObject("signal_model"), "Signal Model", "l");
	legendSignal->AddEntry(xframeSignal->findObject("signal_cb"), "CB Component", "l");
	legendSignal->AddEntry(xframeSignal->findObject("signal_gauss"), "Gauss Component", "l");
	// legendSignal->AddEntry(outHistoSignal, "Expected Signal (#times 30)", "l");
	
	// pair<double,double> effSigma_HSignal = getEffSigma(&mHZSignal, &signal_cb, 100, 150);
	// effSigmaJSON["HTo"+analysisBranch+"Photon"][selCategory]["signal"] = 0.5*(effSigma_HSignal.second-effSigma_HSignal.first);
	
	legendSignal->AddEntry(seffLeg,Form("#sigma_{eff} = %1.2f GeV",0.5*(effSigma_HSignal.second-effSigma_HSignal.first)),"fl");
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
	latexSignal->DrawLatex(.25, 0.93, "Preliminary");
	latexSignal->SetTextFont(42);
	latexSignal->SetTextSize(0.04);
	latexSignal->SetTextAlign(31);
	latexSignal->DrawLatex(0.96, 0.93, "35.86 fb^{-1} (13 TeV, 2016) ");


	if (analysisBranch == "JPsi")  {
		system(("mkdir -p  `dirname fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_Signal_"+selCategory+".png").c_str());
		cSignal->SaveAs(("fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_Signal_"+selCategory+".root").c_str());
		cSignal->SaveAs(("fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_Signal_"+selCategory+".png").c_str());
		cSignal->SaveAs(("fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_Signal_"+selCategory+".pdf").c_str());
	}

	if (analysisBranch == "Upsilon")  {
		system(("mkdir -p  `dirname fitPlotFiles/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilonPhotonSignalAndBackgroundFit_Signal_"+selCategory+".png").c_str());
		cSignal->SaveAs(("fitPlotFiles/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilonPhotonSignalAndBackgroundFit_Signal_"+selCategory+".root").c_str());
		cSignal->SaveAs(("fitPlotFiles/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilonPhotonSignalAndBackgroundFit_Signal_"+selCategory+".png").c_str());
		cSignal->SaveAs(("fitPlotFiles/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilonPhotonSignalAndBackgroundFit_Signal_"+selCategory+".pdf").c_str());
	}

	//////////////////////////////////////////////////////////////////////////////////////
	// LOAD DATA - PEAKING BACKGROUND
	TFile * outTreeToFitFilePeakingBackground;
	if (analysisBranch == "JPsi") outTreeToFitFilePeakingBackground = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_HDalitz_HtoJPsi_"+selCategory+"_ZZZZZ.root").c_str());
	if (analysisBranch == "Upsilon") outTreeToFitFilePeakingBackground = TFile::Open(("inputData/outputHistos/outTreeToFit_HZtoUpsilonPhoton_HDalitz_HtoUpsilon_"+selCategory+"_ZZZZZ.root").c_str());
	auto * outTreeToFitPeakingBackground = (TTree*)outTreeToFitFilePeakingBackground->Get("outTree_HZtoUpsilonPhoton");
	RooRealVar mHZPeakingBackground("CMS_hgg_mass", "CMS_hgg_mass", 100, 150, "GeV") ;
	RooRealVar weightsPeakingBackground("mHZWeight", "mHZWeight", -100, 100, "");

	// RooDataSet peakingBackground("peakingBackground", " ", RooArgSet(mHZPeakingBackground, weightsPeakingBackground), Import(*outTreeToFitPeakingBackground), WeightVar(weightsPeakingBackground));
	outTreeToFitPeakingBackground->GetBranch("mHZ")->SetName("CMS_hgg_mass");
	RooDataSet peakingBackground("peakingBackground", " ", RooArgSet(mHZData, weightsPeakingBackground), Import(*outTreeToFitPeakingBackground), WeightVar(weightsPeakingBackground));
	
	peakingBackground.Print();

	////////////////////////////////////////////////////////////////////////////////////
	// PEAKING BACKGROUND
	RooRealVar mean_cbPeakingBackground("mean_cbPeakingBackground", "Mean" , 125, 100, 150) ;
	RooRealVar sigma_cbPeakingBackground("sigma_cbPeakingBackground", "Width" ,  2., 0.5, 4.) ;
	RooRealVar alphaPeakingBackground("alphaPeakingBackground","", 3., 0.1, 50.);
	RooRealVar nPeakingBackground("nPeakingBackground","", 0.5, 0.1, 50.);

	

	RooCBShape PeakingBackground_cb("PeakingBackground_cb", "Crystal Ball Peaking Background component", mHZData, mean_cbPeakingBackground, sigma_cbPeakingBackground, alphaPeakingBackground, nPeakingBackground);


	cout << "\n\n---------------------------------------------------------------------------------------------------> Begin Peaking Background Fit (" + analysisBranch + " - " + selCategory + ")\n\n" << endl;
	RooFitResult * fitResultPeakingBackground = PeakingBackground_cb.fitTo(peakingBackground, RooFit::Minimizer("Minuit","minimize"), Save(kTRUE), SumW2Error(kTRUE) ) ;
	fitResultPeakingBackground->Print();
	cout << "\n\n---------------------------------------------------------------------------------------------------> End Peaking Background Fit (" + analysisBranch + " - " + selCategory + ")\n\n" << endl;


  	////////////////////////////////////////////////////////////////////////////////////
  	// Peaking Background PLOT
	auto cPeakingBackground = new TCanvas("cPeakingBackground","cPeakingBackground",1050*2.0,750*2.0);
	gPad->SetLeftMargin(0.17); 
	gPad->SetRightMargin(0.05); 
	gPad->SetTopMargin(0.08);
	RooPlot* xframePeakingBackground = mHZData.frame(Title("M_{#mu#mu#gamma}")) ;
	peakingBackground.plotOn(xframePeakingBackground,Binning(40),RooFit::Name("peakingBackground"), MarkerSize(3), DataError(RooAbsData::SumW2)) ; 
	// dcballPeakingBackground.plotOn(xframePeakingBackground,RooFit::Name("dcballPeakingBackground"),LineColor(kAzure+7));
	PeakingBackground_cb.plotOn(xframePeakingBackground,RooFit::Name("PeakingBackground_cb"),LineColor(kAzure+7));
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
	if (analysisBranch == "JPsi") legendPeakingBackground->SetHeader("H #rightarrow J/#Psi + #gamma Analysis");
	if (analysisBranch == "Upsilon") legendPeakingBackground->SetHeader("H #rightarrow #Upsilon + #gamma Analysis");
	legendPeakingBackground->SetBorderSize(0);
	legendPeakingBackground->SetFillStyle(0);
	legendPeakingBackground->AddEntry(xframePeakingBackground->findObject("peakingBackground"), "Higgs Dalitz MC", "lpe");
	legendPeakingBackground->AddEntry(xframePeakingBackground->findObject("PeakingBackground_cb"), "Higgs Dalitz Model", "l");
	// legendSignal->AddEntry(outHistoSignal, "Expected Signal (#times 30)", "l");
	
	pair<double,double> effSigma_HPBckg = getEffSigma(&mHZData, &PeakingBackground_cb, 100, 150);
	effSigmaJSON["HTo"+analysisBranch+"Photon"][selCategory]["peackingBackground"] = 0.5*(effSigma_HPBckg.second-effSigma_HPBckg.first);
	
	// legendPeakingBackground->AddEntry((TObject*)0,Form("#sigma_{eff} = %1.2f GeV",0.5*(effSigma_HPBckg.second-effSigma_HPBckg.first)),"");
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
	latexPeakingBackground->DrawLatex(.25, 0.93, "Preliminary");
	latexPeakingBackground->SetTextFont(42);
	latexPeakingBackground->SetTextSize(0.04);
	latexPeakingBackground->SetTextAlign(31);
	latexPeakingBackground->DrawLatex(0.96, 0.93, "35.86 fb^{-1} (13 TeV, 2016) ");

	if (analysisBranch == "JPsi") {
		system(("mkdir -p  `dirname fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".png`").c_str());
		cPeakingBackground->SaveAs(("fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".root").c_str());
		cPeakingBackground->SaveAs(("fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".png").c_str());
		cPeakingBackground->SaveAs(("fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".pdf").c_str());
	}

	if (analysisBranch == "Upsilon") {
		system(("mkdir -p  `dirname fitPlotFiles/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilonPhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".png`").c_str());
		cPeakingBackground->SaveAs(("fitPlotFiles/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilonPhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".root").c_str());
		cPeakingBackground->SaveAs(("fitPlotFiles/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilonPhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".png").c_str());
		cPeakingBackground->SaveAs(("fitPlotFiles/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilonPhotonSignalAndBackgroundFit_PeakingBackground_"+selCategory+".pdf").c_str());
	}

	////////////////////////////////////////////////////////////////////////////////////
	// C r e a t e   w o r k s p a c e ,   i m p o r t   d a t a   a n d   m o d e l 
	// For COMIBINE
	RooWorkspace *w = new RooWorkspace("w","workspace") ;  
	
	// data
	w->import(mHZData);
	w->import(data,Rename("data_obs"));
	w->import(Bernstein);
	// signal
	// w->import(mHZSignal,Rename("mHZSignal"));
	w->import(signal,Rename("signal"));
	w->import(signal_model);
	//peaking background
	// w->import(mHZPeakingBackground);
	w->import(peakingBackground,Rename("peakingBackground"));
	w->import(PeakingBackground_cb);

 	w->importClassCode();


	w->var("alphaPeakingBackground")->setConstant(true);
	w->var("alpha_cb")->setConstant(true);
	w->var("cb_fraction")->setConstant(true);
	w->var("mean_Higgs")->setConstant(true);
	w->var("mean_cbPeakingBackground")->setConstant(true);
	w->var("n")->setConstant(true);
	w->var("nPeakingBackground")->setConstant(true);
	w->var("p1")->setConstant(true);
	w->var("p2")->setConstant(true);
	w->var("sigma_cb")->setConstant(true);
	w->var("sigma_cbPeakingBackground")->setConstant(true);
	w->var("sigma_gauss")->setConstant(true);


	w->Print("t");

	if (analysisBranch == "JPsi") {
		system(("mkdir -p  `dirname fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_workspace_"+selCategory+".root").c_str());
		w->writeToFile(("fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_workspace_"+selCategory+".root").c_str());
	}

	if (analysisBranch == "Upsilon") {
		system(("mkdir -p  `dirname fitPlotFiles/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilonPhotonSignalAndBackgroundFit_workspace_"+selCategory+".root").c_str());
		w->writeToFile(("fitPlotFiles/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilonPhotonSignalAndBackgroundFit_workspace_"+selCategory+".root").c_str());
	}

	// For FlashGG
	// create file
	TFile * outFile_forFLASHGG;
	if (analysisBranch == "JPsi") outFile_forFLASHGG = TFile::Open(("fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_workspace_forFLASHGG_"+selCategory+".root").c_str(),"RECREATE");
	if (analysisBranch == "Upsilon") outFile_forFLASHGG = TFile::Open(("fitPlotFiles/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilonPhotonSignalAndBackgroundFit_workspace_forFLASHGG_"+selCategory+".root").c_str(),"RECREATE");
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
	// workspace_DCBZPeakUpsilonfit2Dpsilonfit2D();
  	ZToJPsiUpsilonPhotonSignalAndBackgroundFit("JPsi", "Cat0");
  	ZToJPsiUpsilonPhotonSignalAndBackgroundFit("Upsilon", "Cat0");

  	ZToJPsiUpsilonPhotonSignalAndBackgroundFit("JPsi", "Cat1");
  	ZToJPsiUpsilonPhotonSignalAndBackgroundFit("Upsilon", "Cat1");

  	ZToJPsiUpsilonPhotonSignalAndBackgroundFit("JPsi", "Cat2");
  	ZToJPsiUpsilonPhotonSignalAndBackgroundFit("Upsilon", "Cat2");

  	ZToJPsiUpsilonPhotonSignalAndBackgroundFit("JPsi", "Cat3");
  	ZToJPsiUpsilonPhotonSignalAndBackgroundFit("Upsilon", "Cat3");

  	HToJPsiUpsilonPhotonSignalAndBackgroundFit("JPsi", "Cat0");
  	HToJPsiUpsilonPhotonSignalAndBackgroundFit("Upsilon", "Cat0");

  	cout << "\n\n-----------------> [BEGIN] effSigmaJSON\n" << endl;
  	cout << effSigmaJSON.dump(4) << endl;
  	cout << "\n-----------------> [END] effSigmaJSON\n" << endl;

  	std::ofstream effSigmaJSONFile("fitPlotFiles/effSigmaJSON.json");
    effSigmaJSONFile << effSigmaJSON.dump(4);

} //end plotter_ZtoUpsilonPhoton




