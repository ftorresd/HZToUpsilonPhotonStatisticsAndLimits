#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooAddPdf.h"
#include "RooGaussian.h"
#include "RooChebychev.h"
#include "RooCBShape.h"
#include "RooPlot.h"
#include "RooMCStudy.h"
#include "RooFitResult.h"
#include "RooThresholdCategory.h"
#include "RooBinningCategory.h"
#include "RooWorkspace.h"
#include "RooArgList.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TAxis.h"
#include "TGraphAsymmErrors.h"
#include "TPaveLabel.h"
#include "TFolder.h"

#include "tdrstyle.C"

using namespace RooFit;


double *UpsilonFit(int bkgOpt=1, bool simple=false, TString title="signal+bckg", TString xtitle="m_{#mu^{-}#mu^{+}} [GeV/c^{2}]",bool extend=true)
{

//	gROOT->ProcessLine(".L tdrstyle.C");
	setTDRStyle();
	//gStyle->SetPadRightMargin(0.05);



	//TFile* file = new TFile("outHistos_ZtoUpsilonPhoton_Data.root","read");
	//	TH1D *massUpsilonHist = (TH1D *)file->Get("h_withKin_Upsilon_Mass");

	TFile* _file0 = new TFile("outHistos_ZtoUpsilonPhoton_Data.root","read");
	//TH1D *massUpsilonHist = (TH1D*)_file0->Get("outputHistos/noKinCutsHistos/h_noKin_Upsilon_Mass"); 

	TH1D *massUpsilonHist = (TH1D*)_file0->Get("outputHistos/withKinCutsHistos/h_withKin_Upsilon_Mass");



	const double M1S = 9.46;   //upsilon 1S pgd mass value
	const double M2S = 10.02;  //upsilon 2S pgd mass value
	const double M3S = 10.35; //upsilon 3S pgd mass value
	const double M4S = 10.579; //upsilon 4S pgd mass value

	double minMass=8.0;
	double maxMass=12.0;


	RooRealVar mass(xtitle,xtitle,minMass,maxMass);
	RooDataHist rooNoCutMass("rooNoCutMass","rooNoCutMass",mass,Import(*massUpsilonHist));


	RooRealVar mean("mass_mean","#Upsilon mean",M1S,M1S-0.5,M1S+0.5,"GeV");
	RooRealVar shift21("shift2","mass diff #Upsilon(1,2S)",M2S-M1S);
	RooRealVar shift31("shift3","mass diff #Upsilon(1,3S)",M3S-M1S);
	RooRealVar shift41("shift4","mass diff #Upsilon(1,4S)",M4S-M1S);

	RooRealVar mscale("mscale","mass scale factor",1.,1.0,1.0);
	mscale.setConstant(kTRUE); /* the def. parameter value is fixed in the fit */
	// double mscale = 1.0;

	RooFormulaVar mean1S("mean1S","@0",RooArgList(mean));
	RooFormulaVar mean2S("mean2S","@0+@1*@2",RooArgList(mean,mscale,shift21));
	RooFormulaVar mean3S("mean3S","@0+@1*@2",RooArgList(mean,mscale,shift31));
	RooFormulaVar mean4S("mean4S","@0+@1*@2",RooArgList(mean,mscale,shift41));


	RooRealVar sigma1("sigma1","Sigma_1",0.06,0.01,0.30);
	RooRealVar sigma2("sigma2","Sigma_2",0.10,0.01,0.30);

	/// to describe final state radiation tail on the left of the peaks
	RooRealVar alpha("alpha","tail shift",1.5,0.2,4);
	RooRealVar npow("npow","power order",2);
	npow.setConstant(kTRUE);
	alpha.setConstant(kTRUE);

	/// relative fraction of the two peak components 
	RooRealVar sigmaFraction("sigmaFraction","Sigma Fraction",0.3,0.,1.);

	/// Upsilon 1S
	RooGaussian gauss1S1("gauss1S1","1S Gaussian_1",mass,mean1S,sigma1);
	RooCBShape  gauss1S2("gauss1S2", "FSR cb 1s",mass,mean1S,sigma2,alpha,npow); 
	RooAddPdf sig1S("sig1S","1S mass pdf",RooArgList(gauss1S1,gauss1S2),sigmaFraction);

	/// Upsilon 2S
	RooGaussian gauss2S1("gauss2S1","2S Gaussian_1",mass,mean2S,sigma1);
	RooCBShape  gauss2S2("gauss2S2", "FSR cb 2s", mass,mean2S,sigma2,alpha,npow); 
	RooAddPdf sig2S("sig2S","2S mass pdf",RooArgList(gauss2S1,gauss2S2),sigmaFraction);

	/// Upsilon 3S
	RooGaussian gauss3S1("gauss3S1","3S Gaussian_1",mass,mean3S,sigma1);
	RooGaussian gauss3S2("gauss3S2","3S Gaussian_2",mass,mean3S,sigma2);
	RooAddPdf sig3S("sig3S","3S mass pdf",RooArgList(gauss3S1,gauss3S2),sigmaFraction);

	/// Upsilon 4S
	RooGaussian gauss4S1("gauss4S1","4S Gaussian_1",mass,mean4S,sigma1);
	RooGaussian gauss4S2("gauss4S2","4S Gaussian_2",mass,mean4S,sigma2);
	RooAddPdf sig4S("sig4S","4S mass pdf",RooArgList(gauss4S1,gauss4S2),sigmaFraction);


	/// Background
	RooRealVar *bkg_a1  = new RooRealVar("bkg_a1", "background a1", 0, -1, 1);
	RooRealVar *bkg_a2  = new RooRealVar("bkg_a2", "background a2", 0, -1, 1);
	RooAbsPdf  *bkgPdf  = new RooChebychev("bkg","linear background",mass, RooArgList(*bkg_a1,*bkg_a2));


	/// Combined pdf
	int nt = 30000;
	RooRealVar *nsig1 = new RooRealVar("nsig1","signal 1s yield",nt*0.25*0.6,0,10*nt); 
	RooRealVar *nsig2 = new RooRealVar("nsig2","signal 2s yield",nt*0.25*0.3,0,10*nt); 
	RooRealVar *nsig3 = new RooRealVar("nsig3","signal 3s yield",nt*0.25*0.1,0,10*nt); 
	RooRealVar *nsig4 = new RooRealVar("nsig4","signal 3s yield",nt*0.25*0.05,0,10*nt);
	RooRealVar *nbkgd = new RooRealVar("nbkgd","brackground yield",nt*0.75,0,10*nt); 
	RooAbsPdf  *pdf   = new RooAddPdf ("pdf","total signal+background pdf",RooArgList(sig1S,sig2S,sig3S,sig4S,*bkgPdf),RooArgList(*nsig1,*nsig2,*nsig3,*nsig4,*nbkgd));


	//	w.import(*pdf);

	if(extend){ pdf->fitTo( rooNoCutMass, Extended() );}
	else{pdf->fitTo( rooNoCutMass);}
	RooPlot* frame = mass.frame(Title(title));
	rooNoCutMass.plotOn(frame,Name("theData"),DataError(RooDataHist::SumW2));
	pdf->plotOn(frame,Name("thePdf"));
	// Adding Dataset box and PDF parameters box
	//        pdf->paramOn(frame,Name("thePdf"));
	//mass.statOn(frame); 

	pdf->plotOn(frame,Components("sig1S"),LineStyle(kDashed),LineColor(kGreen));
	pdf->plotOn(frame,Components("sig2S"),LineStyle(kDashed),LineColor(kMagenta));
	pdf->plotOn(frame,Components("sig3S"),LineStyle(kDashed),LineColor(kYellow));
	pdf->plotOn(frame,Components("sig4S"),LineStyle(kDashed),LineColor(kOrange)); 
	pdf->plotOn(frame,Components("bkg"),LineStyle(kDashed),LineColor(kRed));


        // Adding Dataset box and PDF parameters box 
	RooArgSet * pars = pdf->getParameters(rooNoCutMass);
	int nfloatpars = pars->selectByAttrib("Constant",kFALSE)->getSize();
	double mychsq = frame->chiSquare("thePdf","theData", nfloatpars);
	double myndof = massUpsilonHist->GetNbinsX() - nfloatpars;
	pdf->paramOn(frame,Layout(0.65,0.99,0.99),Format("NE"),Label(Form("#chi^{2}/ndf = %2.0f/%2.0f", myndof*mychsq, myndof))
		    );




	frame->Draw();
	double *N=new double[6];

	return(N);


}


