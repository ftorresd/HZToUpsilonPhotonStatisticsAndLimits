
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


void SetConstantParams(const RooArgSet* params) {
	// set constant parameters for signal fit, ... NO IDEA !!!!                                                                            
	TIterator* iter(params->createIterator());
	for (TObject *a = iter->Next(); a != 0; a = iter->Next()) {
		RooRealVar *rrv = dynamic_cast<RooRealVar *>(a);
		if (rrv) { rrv->setConstant(true); std::cout << " " << rrv->GetName(); }
	}
} // close set const parameters    

TTree* makeTTree(string fname)
{
	// Create ROOT TTree filled with a Gaussian distribution in x and a uniform distribution in y

	Double_t* mHZ = new Double_t;
	Double_t* weights = new Double_t;

	TTree *T1 = new TTree("T1", "data from ascii file");
	T1->Branch("mHZ", &mHZ, "mHZ/D"); // The branch name is m
	T1->ReadFile(fname.c_str(), "mHZ");

	return T1 ;
}

void makeRooMultiPdfToWS()
{
	// Load the combine Library 
	gSystem->Load("libHiggsAnalysisCombinedLimit.so");

	setTDRStyle();

	//TTree* DataTree = makeTTree("../Inputs/HiggsMass_2016.txt");
	TTree* DataTree = makeTTree("mHZ_2016.txt");

	RooRealVar mHZ("mHZ", "mHZ", 110, 150, "GeV") ;
	RooDataSet data_2016("data_2016", "2016 dataset with Higgs mass", RooArgSet(mHZ), Import(*DataTree));

	// polynomial
	RooRealVar p0("p0"," ",5,0,10);
	RooRealVar p1("p1", " ", 5, 0, 10); 
	//RooBernstein Poly_1st("Poly_1st", " ", mH, RooArgList(RooConst(1),p1));
	RooBernstein Poly_1st("Poly_1st", " ", mHZ, RooArgList(p0,p1));
	RooRealVar p0_2("p0_2"," ",5,0,10);
	RooRealVar p1_2("p1_2", " ", 5, 0, 10);
	RooRealVar p2_2("p2_2", " ", 5, 0, 10);
	//RooBernstein Poly_2nd("Poly_2nd", " ", mH, RooArgList(RooConst(1),p1_2,p2_2));
	RooBernstein Poly_2nd("Poly_2nd", " ", mHZ, RooArgList(p0_2,p1_2,p2_2));
	RooRealVar p0_3("p0_3"," ",5,0,10);
	RooRealVar p1_3("p1_3", " ", 5, 0, 10);
	RooRealVar p2_3("p2_3", " ", 5, 0, 10);
	RooRealVar p3_3("p3_3", " ", 5, 0, 10);
	//RooBernstein Poly_3rd("Poly_3rd", " ", mH, RooArgList(RooConst(1),p1_3,p2_3,p3_3));
	RooBernstein Poly_3rd("Poly_3rd", " ", mHZ, RooArgList(p0_3,p1_3,p2_3,p3_3));
	RooRealVar p0_4("p0_4"," ",5,0,10);
	RooRealVar p1_4("p1_4", " ", 5, 0, 10);
	RooRealVar p2_4("p2_4", " ", 5, 0, 10);
	RooRealVar p3_4("p3_4", " ", 5, 0, 10);
	RooRealVar p4_4("p4_4", " ", 5, 0, 10);
	//RooBernstein Poly_4th("Poly_4th", " ", mH, RooArgList(RooConst(1),p1_4,p2_4,p3_4,p4_4));
	RooBernstein Poly_4th("Poly_4th", " ", mHZ, RooArgList(p0_4,p1_4,p2_4,p3_4,p4_4));
	// polynomial 
	/*
	   RooRealVar p1("p1", " ", -10, 10); 
	   RooPolynomial Poly_1st("Poly_1st", " ", mH, RooArgList(p1));
	   RooRealVar p1_2("p1_2", " ", -10, 10);
	   RooRealVar p2_2("p2_2", " ", -10, 10);
	   RooPolynomial Poly_2nd("Poly_2nd", " ", mH, RooArgList(p1_2,p2_2));
	   RooRealVar p1_3("p1_3", " ", -10, 10);
	   RooRealVar p2_3("p2_3", " ", -10, 10);
	   RooRealVar p3_3("p3_3", " ", -10, 10);
	   RooPolynomial Poly_3rd("Poly_3rd", " ", mH, RooArgList(p1_3,p2_3,p3_3));
	   RooRealVar p1_4("p1_4", " ", -10, 10);
	   RooRealVar p2_4("p2_4", " ", -10, 10);
	   RooRealVar p3_4("p3_4", " ", -10, 10);
	   RooRealVar p4_4("p4_4", " ", -10, 10);
	   RooPolynomial Poly_4th("Poly_4th", " ", mH, RooArgList(p1_4,p2_4,p3_4,p4_4));
	   */

	// exponential
	RooRealVar alpha("alpha"," ",-1.,-100.,0.) ;
	RooExponential exp("exp"," ",mHZ,alpha) ;
	RooRealVar pdf_norm("pdf_norm"," ",101,0,1000) ;
	RooExtendPdf exp_alter("exp_alter"," ",exp,pdf_norm," ");
	// power law
	RooRealVar a("a"," ",1.,0.,10.);
	RooRealVar b("b"," ",-1.,-10.,0.);
	RooGenericPdf powerlaw("powerlaw","a*mHZ^b",RooArgList(mHZ,a,b));

	// Laurent polynomials
	RooRealVar a1("a1"," ",1,0,10);
	RooRealVar b1("b1"," ",1,0,10);
	RooGenericPdf Laurent("Laurent","a1*mHZ^(-4)+b1*mHZ^(-6)",RooArgList(mHZ,a1,b1));

	RooFitResult* r_Poly1st = Poly_1st.fitTo(data_2016, Save(kTRUE)) ;
	RooFitResult* r_Poly2nd = Poly_2nd.fitTo(data_2016, Save(kTRUE)) ;
	RooFitResult* r_Poly3rd = Poly_3rd.fitTo(data_2016, Save(kTRUE)) ;
	RooFitResult* r_Poly4th = Poly_4th.fitTo(data_2016, Save(kTRUE)) ;
	RooFitResult* r_exp = exp_alter.fitTo(data_2016, Save(kTRUE)) ;
	RooFitResult* r_powerlaw = powerlaw.fitTo(data_2016, Save(kTRUE)) ;
	RooFitResult* r_Laurent = Laurent.fitTo(data_2016, Save(kTRUE)) ;

	p0.setConstant(true);
	p1.setConstant(true);
	p0_2.setConstant(true);
	p1_2.setConstant(true);	
	p2_2.setConstant(true);
	p0_3.setConstant(true);
	p1_3.setConstant(true);
	p2_3.setConstant(true);
	p3_3.setConstant(true);
	p0_4.setConstant(true);
	p1_4.setConstant(true);
	p2_4.setConstant(true);
	p3_4.setConstant(true);
	p4_4.setConstant(true);
	alpha.setConstant(true);
	a.setConstant(true);
	b.setConstant(true);
	a1.setConstant(true);
	b1.setConstant(true);

	//Poly_1st.fitTo(data_2016);
	//Poly_2nd.fitTo(data_2016);/
	//Poly_3rd.fitTo(data_2016);
	//Poly_4th.fitTo(data_2016);
	//exp.fitTo(data_2016);
	//powerlaw.fitTo(data_2016);
	//Laurent.fitTo(data_2016);

	//cout << alpha.getVal() << endl;

	//bgrfit1->fitTo(data_2016);

	// Make a RooCategory object. This will control which of the pdfs is "active"
	RooCategory cat("pdf_index","Index of Pdf which is active");

	// Make a RooMultiPdf object. The order of the pdfs will be the order of their index, ie for below 
	// 0 == exponential
	// 1 == linear function
	// 2 == powerlaw
	RooArgList mypdfs;
	mypdfs.add(Poly_1st);
	mypdfs.add(Poly_2nd);
	mypdfs.add(Poly_3rd);
	mypdfs.add(Poly_4th);
	mypdfs.add(exp_alter);
	mypdfs.add(powerlaw);
	mypdfs.add(Laurent);

	RooMultiPdf multipdf("roomultipdf","All Pdfs",cat,mypdfs);

	// As usual make an extended term for the background with _norm for freely floating yield
	RooRealVar norm("roomultipdf_norm","Number of background events",101,101*0.25,101*1.75);

	// Save to a new workspace
	TFile *fout = new TFile("background_pdfs.root","RECREATE");
	RooWorkspace *wout = new RooWorkspace("backgrounds"," ");
	wout->import(cat);
	wout->import(norm);
	wout->import(multipdf);
	//wout->import(exp);
	//wout->defineSet("SigPdfParams",RooArgSet(*wout->var("alpha")));
	//SetConstantParams(wout->set("SigPdfParams"));
	wout->var("p0")->setConstant(true);
	wout->var("p1")->setConstant(true);
	wout->var("p0_2")->setConstant(true);
	wout->var("p1_2")->setConstant(true);
	wout->var("p2_2")->setConstant(true);
	wout->var("p0_3")->setConstant(true);
	wout->var("p1_3")->setConstant(true);
	wout->var("p2_3")->setConstant(true);
	wout->var("p3_3")->setConstant(true);
	wout->var("p0_4")->setConstant(true);
	wout->var("p1_4")->setConstant(true);
	wout->var("p2_4")->setConstant(true);
	wout->var("p3_4")->setConstant(true);
	wout->var("p4_4")->setConstant(true);
	wout->var("alpha")->setConstant(true);
	wout->var("a")->setConstant(true);
	wout->var("b")->setConstant(true);
	wout->var("a1")->setConstant(true);
	wout->var("b1")->setConstant(true);
	wout->Print();
	wout->Write();
	fout->Write();
	fout->Close();
	//wout->writeToFile("background_pdfs.root");

	/*
	   RooWorkspace *w = new RooWorkspace("w","workspace") ;
	   w->import(data_2016, Rename("data2016_obs"));
	   w->import();
	   */





	//------------------//
	// Plotting section //
	//------------------//

	TCanvas* c1 = new  TCanvas( "c1", "", 800, 600);
	gPad->SetRightMargin(0.05); 
	gPad->SetTopMargin(0.08);

	mHZ.setRange("blind1",110,115) ;
	mHZ.setRange("blind2",135,150);
	RooPlot* xframe = mHZ.frame() ;
	data_2016.plotOn(xframe,Binning(40),CutRange("blind1"),RooFit::Name("Data_2016")) ; 
	data_2016.plotOn(xframe,Binning(40),CutRange("blind2")) ;
	Poly_1st.plotOn(xframe,LineColor(kRed),RooFit::Name("Poly_1st"));
	Poly_2nd.plotOn(xframe,LineColor(kGreen),RooFit::Name("Poly_2nd"));
	Poly_3rd.plotOn(xframe,LineColor(kBlue),RooFit::Name("Poly_3rd"));
	Poly_4th.plotOn(xframe,LineColor(kOrange),RooFit::Name("Poly_4th"));
	exp.plotOn(xframe,LineColor(7),RooFit::Name("exp"));
	powerlaw.plotOn(xframe,LineColor(6),RooFit::Name("powerlaw"));
	Laurent.plotOn(xframe,LineColor(13),RooFit::Name("Laurent"));
	//bgrfit1->plotOn(xframe,Line(kBlue));
	c1->cd();
	xframe->Draw();
	xframe->SetMinimum(0.00001);
	TLatex* ltx1 = new TLatex();
	ltx1->SetNDC();
	ltx1->SetTextFont(61);
	ltx1->SetTextSize(0.05);
	ltx1->DrawLatex(0.19,0.85,"CMS");
	ltx1->SetTextFont(52);
	ltx1->SetTextSize(0.04);
	ltx1->DrawLatex(0.19,0.81,"Preliminary");
	ltx1->SetTextFont(42);
	ltx1->SetTextSize(0.03);
	ltx1->DrawLatex(0.16,0.94,"H#rightarrowJ/#psi#gamma#rightarrow#mu#mu#gamma");
	ltx1->SetTextFont(42);
	ltx1->SetTextSize(0.04);
	ltx1->DrawLatex(0.68,0.94,"2016 35.85 fb^{-1} (13TeV)");
	TLegend* leg1 = new TLegend(0.5,0.7,0.9,0.9);
	leg1->AddEntry(xframe->findObject("Data_2016"), "2016 Data", "lep");
	leg1->AddEntry(xframe->findObject("Poly_1st"), "Poly 1st", "l");
	leg1->AddEntry(xframe->findObject("Poly_2nd"), "Poly 2nd", "l");
	leg1->AddEntry(xframe->findObject("Poly_3rd"), "Poly 3rd", "l");
	leg1->AddEntry(xframe->findObject("Poly_4th"), "Poly 4th", "l");
	leg1->AddEntry(xframe->findObject("exp"), "exponential", "l");
	leg1->AddEntry(xframe->findObject("powerlaw"), "power law", "l");
	leg1->AddEntry(xframe->findObject("Laurent"), "Laurent form", "l");
	leg1->Draw("same");
	c1->SaveAs("fitBkg.png");


	/*cout << p1_2.getVal() << endl;
	  cout << "alpha=" << alpha.getVal() << endl;
	  cout << "a=" << a.getVal() << endl;
	  cout << "b=" << b.getVal() << endl;*/

	r_Poly1st->Print("v");
	r_Poly2nd->Print("v");
	r_Poly3rd->Print("v");
	r_Poly4th->Print("v");
	r_exp->Print("v");
	r_powerlaw->Print("v");
	r_Laurent->Print("v");

}
