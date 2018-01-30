#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "TMath.h"


#include "TStopwatch.h"
#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooAddPdf.h"
#include "RooGaussian.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooMsgService.h"
#include "RooMinuit.h"


using namespace std;
using namespace RooFit;

pair<double,double> getEffSigma(RooRealVar *mass, RooAbsPdf *pdf, double wmin=110., double wmax=130., double step=0.002, double epsilon=1.e-2){

  RooAbsReal *cdf = pdf->createCdf(RooArgList(*mass));
  cout << "Computing effSigma...." << endl;
  // TStopwatch sw;
  // sw.Start();
  double point=wmin;
  vector<pair<double,double> > points;

  while (point <= wmax){
    mass->setVal(point);
    // cout << "point: " << point << endl;
    if (pdf->getVal() > epsilon){
      // cout << "#################################" << endl;
      // cout << "point: " << point << endl;
      // cout << "pdf->getVal(): " << pdf->getVal() << endl;
      // cout << "cdf->getVal(): " << cdf->getVal() << endl;
      points.push_back(pair<double,double>(point,cdf->getVal())); 
    }
    point+=step;
  }
  double low = wmin;
  double high = wmax;
  double width = wmax-wmin;
  for (unsigned int i=0; i<points.size(); i++){
    for (unsigned int j=i; j<points.size(); j++){
      double wy = points[j].second - points[i].second;
      double wx = points[j].first - points[i].first;
      
      if (TMath::Abs(wy-0.683) < epsilon && wx < width){
        // cout << "effSigma: [" << low << " - " << high << "] = " << width << endl;
        // cout << "Foi!" << endl;
        // if (wx < width){
          low = points[i].first;
          high = points[j].first;
          width=wx;
        // }
      }
    }
  }
  // sw.Stop();
  cout << "effSigma: [" << low << "-" << high << "] = " << width/2. << endl;
  //cout << "\tTook: "; sw.Print();
  pair<double,double> result(low,high);
  return result;
}