## F-test macro extracted of flashgg packaged and adapted for HZGamma Analysis

## Recipe using Combine 74X
```
https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideHiggsAnalysisCombinedLimit#ROOT6_SLC6_release_CMSSW_7_4_X
```

## git commands
```
cd $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit
git clone git@github.com:ftorresd/HZToUpsilonPhotonStatisticsAndLimits.git HZToUpsilonPhotonStatisticsAndLimits
cd HZToUpsilonPhotonStatisticsAndLimits/ftest/
make
```
Example to produce f-test: Exponential, Laurent, PowerLaw
```
/bin/fTest -i /afs/cern.ch/user/f/ftorresd/workSpace/public/paraElizaESandro/combineIssue/HZToUpsilonPhotonStatisticsAndLimits/inputData/fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_workspace_forFLASHGG_Cat0.root --saveMultiPdf CMS-HGG_multipdf_HToJPsiGamma_afterFtest.root -D outdir_HToJPsiGamma_example/bkgfTest-Data -f UntaggedTag_0 --isData 1
```
These f-test macro was modify to produce only  Exponential, Laurent, PowerLaw PDF for Higgs->J/Psi+Gamma. You should have to modify a mass range limits and blind regions for Z case.

## More information about f-test macro you can find:
```
https://github.com/cms-analysis/flashggFinalFit/tree/master/Background
```

## Bias Studies using using Combine package

```
- step0
cd HZToUpsilonPhotonStatisticsAndLimits/bias_combine
combine BiasStudy_Toy_Datacard.txt -M GenerateOnly --setPhysicsModelParameters pdf_index=0,r=200 -t 5000 --expectSignal 200 --saveToys -m 125 --freezeNuisances pdf_index,r -s 98765 --freezeNuisanceGroups signal -n HiggsToJpsiToy
```
```
- step1
 combine BiasStudy_Toy_Datacard.txt -M MaxLikelihoodFit -s 1 --plots --saveShapes --setPhysicsModelParameters pdf_index=0,r=200.0 --toysFile higgsCombineHiggsToJpsiToy.GenerateOnly.mH125.98765.root -t 5000 --rMin -5000.0 --rMax 5000.0 --freezeNuisances pdf_index --freezeNuisanceGroups signal -n HiggsToJpsiToyFits2 -v4 > & HiggsToJpsiToyFits2.log
```

P.S:
The combine version is 74X, rather than 81X which I see you are using, but the commands are very similar. If you want to run the code as is, you would also need to include this small modification I made here:
```
https://github.com/edjtscott/HiggsAnalysis-CombinedLimit/commit/a76d683aaf72ef92d468714bfddb9e2181e13ae7
```
