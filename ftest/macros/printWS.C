#include "../../CombinedLimit/interface/RooMultiPdf.h"

#include "../tdrStyle/tdrstyle.C"

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
#include "TROOT.h"
#include "TH2.h"
#include "TH1.h"
#include "TCanvas.h"
// #include "TDirectory.h"
// #include "THStack.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPad.h"

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
#include "RooExponential.h"

#include "RooDataSet.h"
#include "RooGenericPdf.h"
#include "RooExtendPdf.h"


using namespace RooFit ;

void printWS(){


	TFile *f = new TFile("CMS-HGG_multipdf_HToJPsiGamma_afterFtest.root") ;

	// Retrieve workspace from file
	//      RooWorkspace* w = (RooWorkspace*) f->Get("tagsDumper/cms_hgg_13TeV") ;
	RooWorkspace* w = (RooWorkspace*) f->Get("multipdf") ;
	w->Print();
	//       RooRealVar * m = w->var("CMS_hgg_mass");
	RooRealVar * mHZ = w->var("CMS_hgg_mass");
	//mHZ->GetName();
	RooRealVar *mHZZ ;//= new RooRealVar("mHZ", "mHZ", 110, 150, "GeV") ;
	mHZZ = (RooRealVar*) mHZ->clone("mHZZ") ;

	mHZ->setMin(110);
	mHZ->setMax(150);

	mHZ->Print();
	cout <<"-------- Clone RooRealVar -------"<< endl;
	mHZZ->Print();

	//      RooRealVar mH("mH", "mH", 110, 150, "GeV") ;
	//      RooDataSet data_2016("data_2016", "2016 dataset with Higgs mass", RooArgSet("CMS_hgg_mass"), Import(*DataTree));

	RooDataSet *data_2016;
	RooDataSet *data_2016_clone;
	//  data_2016 =(RooDataSet*)w->data(Form("Data_13TeV_UntaggedTag_0"));    
	data_2016 = (RooDataSet*)w->data("Data_13TeV_UntaggedTag_0");

	data_2016->Print();

	cout <<"-------- Clone RooDataSet -------"<< endl;          
	data_2016_clone = (RooDataSet*) data_2016->Clone() ;
	data_2016_clone->Print();

	mHZZ->setRange("blind1",110,115) ;
	mHZZ->setRange("blind2",135,150); 
	RooPlot* xframe = mHZZ->frame() ;
	data_2016_clone->plotOn(xframe);
	xframe->Draw();         


}

