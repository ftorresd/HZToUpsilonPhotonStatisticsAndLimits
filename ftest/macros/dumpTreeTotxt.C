#include "TROOT.h"
#include "TBrowser.h"
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include <fstream>
using namespace std;

void dumpTreeTotxt(){
  TFile *f=new TFile("/afs/cern.ch/user/f/ftorresd/workSpace/public/paraElizaESandro/outputHistos/outTreeToFit_HZtoUpsilonPhoton_Data_HtoJPsi_Cat0_ZZZZZ.root"); // opens the root file
  // TTree  tr=(TTree)f->Get("outTree_HZtoUpsilonPhoton"); // creates the TTree object 
 //TTree* tr=(*TTree)f->Get("outTree_HZtoUpsilonPhoton"); // creates the TTree object
 TTree *tr = (TTree*)f->Get("outTree_HZtoUpsilonPhoton");

// TTree *tree = new TTree(name, title) 
 tr->Scan(); // prints the content on the screen

  double  mHZ, mHZWeight; // create variables of the same type as the branches you want to access

  tr->SetBranchAddress("mHZ",&mHZ); // for all the TTree branches you need this
  tr->SetBranchAddress("mHZWeight",&mHZWeight);
  // tr->SetBranchAddress("nserr",&c);

  ofstream myfile;
  myfile.open ("mHZ_2016.txt");
  //myfile << "TS ns nserr\n";

  for (int i=0;i<tr->GetEntries();i++){
    // loop over the tree
    tr->GetEntry(i);
    myfile << mHZ << "\n"; //write to file
  }
  myfile.close();
}
