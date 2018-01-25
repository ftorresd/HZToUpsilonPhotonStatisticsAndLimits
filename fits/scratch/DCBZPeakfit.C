#include "RooDCBShape/RooDCBShape.h"

using namespace RooFit;

void DCBZPeakfit() {

	TFile* _file0 = new TFile("outHistos_ZtoUpsilonPhoton_Data.root","read");
	TCanvas c; 
	c.SetTitle("");
	TH2* mass2DHist = (TH2D*)_file0->Get("outputHistos/withKinCutsHistos/h_withKin_Upsilon_Mass_Z_Mass");
	TH1 * hh = mass2DHist->ProjectionY();// Boson Z

       // double max = hh->GetXaxis()->GetBinCenter(hh->GetMaximumBin());
        //std::cout << "max: " << max << std::endl;
        //91.25

	RooRealVar x("x", "Mass (GeV/c^{2})", 70, 130);

//	RooRealVar mean_bw("mean_bw", "Mean" ,91.1876,70,130) ;
	
          RooRealVar mean_dcb("mean_dcb", "Mean" ,91.1876,70,130) ;
         //RooRealVar mean_dcb("mean_dcb", "Mean" ,91.1876,80,100) ;

	
        //RooRealVar sigma_bw("sigma_bw", "Width" , 6.5, 1.0,260.0) ;
	
        //RooRealVar sigma_dcb("sigma_dcb", "Width" , 2.3, 1.0,260.0) ;
        //RooRealVar sigma_dcb("sigma_dcb", "Width" , 2.27, 2.27,2.27) ;
          RooRealVar sigma_dcb("sigma_dcb", "Width" ,  2., 0.5, 4.) ;
          // RooRealVar sigma_dcb("sigma_dcb", "Width" ,  2., 0.5, 3.) ;

	
//        RooRealVar n1("n1","", 0.0, 5.0);

  //      RooRealVar n2("n2","", 0.0, 5.0);
       
        RooRealVar n1("n1","", 0.5, 0.1, 50.);//dCBPowerL

        RooRealVar n2("n2","", 1., 0.1, 50.);//dCBPowerR


	//RooRealVar alpha1("alpha1","", 0.0, 5.0);
       
        //RooRealVar alpha2("alpha2","", 0.0, 5.0);

        RooRealVar alpha1("alpha1","", 3., 0.1, 50.);//dCBCutL

        RooRealVar alpha2("alpha2","", 3., 0.1, 50.);//dCBCutR


	RooRealVar lambda("lambda", "slope", -0.1, -5., 0.);
	//RooRealVar s_bw("signal_bw", "signal", 100, 0, 10000);
	RooRealVar s_dcb("signal_dcb", "signal", 100, 0, 10000);

	RooRealVar b("background", "background yield", 100, 0, 10000);


	//RooBreitWigner gauss_bw("gauss_bw","gauss_bw",x,mean_bw,sigma_bw);

	//RooCBShape cball("cball", "crystal ball", x, mean_cb, sigma_cb, alpha, n);
        RooDCBShape cball("cball", "double crystal ball", x, mean_dcb,sigma_dcb,alpha1,alpha2,n1,n2);

	RooDataHist data("data", "dataset with x", x, hh);
	RooExponential expo("expo", "exponential PDF", x, lambda);

//	RooAddPdf sum("sum", "gaussian double crystal ball and exponential PDF", RooArgList(gauss_bw, cball, expo), RooArgList(s_bw, s_dcb, b));
        RooAddPdf sum("sum", "double crystal ball and exponential PDF", RooArgList(cball, expo), RooArgList(s_dcb, b));


	RooPlot * xFrame = x.frame(Title("Z-peak")) ;


	sum.fitTo(data, RooFit::Extended());  
	data.plotOn(xFrame) ;
	sum.plotOn(xFrame) ;
	sum.plotOn(xFrame, RooFit::Components(expo),RooFit::LineStyle(kDashed),LineColor(kRed)) ;  
	sum.paramOn(xFrame,Layout(0.65), Format("NEU", AutoPrecision(1)), Parameters( RooArgList(s_dcb, b, mean_dcb,sigma_dcb )));

        // Adding Dataset box and PDF parameters box 
        RooArgSet * pars = sum.getParameters(data);
        int nfloatpars = pars->selectByAttrib("Constant",kFALSE)->getSize();
        double mychsq = xFrame->chiSquare(nfloatpars);
        double myndof = hh->GetNbinsX() - nfloatpars;
        sum.paramOn(xFrame,Layout(0.65,0.99,0.99),Format("NE"),Label(Form("#chi^{2}/ndf = %2.0f/%2.0f", myndof*mychsq, myndof))
                    );


	xFrame->Draw();
//	mean_bw.Print() ;
	mean_dcb.Print() ;
//	sigma_bw.Print();
	sigma_dcb.Print();
//	s_bw.Print();
	s_dcb.Print();
	b.Print();

	  c.SaveAs("DCB_ExpZpeakfit.gif");


}
