{
	gSystem->AddIncludePath("-Iexternal");
	gSystem->SetBuildDir("tmpdir", kTRUE);
	gStyle->SetOptStat(0);


	// gSystem->AddIncludePath("-I$ROOFITSYS/include");
	// gROOT->GetInterpreter()->AddIncludePath("$ROOFITSYS/include");
	// gSystem->SetIncludePath("-I$ROOFITSYS/include");
	// gSystem->Load("$ROOFITSYS/lib/libRooFit.so") ;
	// gSystem->Load("$ROOFITSYS/lib/libRooFitCore.so") ;
	// gSystem->Load("$ROOFITSYS/lib/libRooStats.so") ;
	gROOT->ProcessLine(".L plugins/RooDCBShape/RooDCBShape.cxx+");


	gROOT->ProcessLine(".L fitter_HZtoUpsilonPhoton.C+"); 
	gROOT->ProcessLine(".L fitter_HZtoUpsilonPhoton2DFit.C+"); 

	// fitter_HZtoUpsilonPhoton(
	// 	"outputHistos/outHistos_ZtoUpsilonPhoton_Data.root",
	// 	"outputHistos/outHistos_ZtoUpsilonPhoton_ZToUpsilon1SGamma.root",
	// 	// "outputHistos/outHistos_ZtoUpsilonPhoton_ZToJPsiGamma.root",
	// 	"outputHistos/outHistos_ZtoUpsilonPhoton_ZGTo2MuG_MMuMu-2To15.root"
	// 	);
 	fitter_HZtoUpsilonPhoton(); 

}

