## Ftest recipes
```

https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideHiggsAnalysisCombinedLimit#ROOT6_SLC6_release_CMSSW_7_4_X

export SCRAM_ARCH=slc6_amd64_gcc491
cmsrel CMSSW_7_4_7
cd CMSSW_7_4_7/src 
cmsenv
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit

cd $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit
git fetch origin
git checkout v6.3.1
scramv1 b clean; scramv1 b # always make a clean build, as scram doesn't always see updates to src/LinkDef.h


```

### Git commands for HZGamma Limits package
```
cd $CMSSW_BASE/src/HiggsAnalysis
git clone git@github.com:ftorresd/HZToUpsilonPhotonStatisticsAndLimits.git HZToUpsilonPhotonStatisticsAndLimits
cd HiggsAnalysis/HZToUpsilonPhotonStatisticsAndLimits/ftest
make

```

### Using F-Test macro adapted of the flashgg packaged
```
##Example to produce f-test using flashgg macros.
./bin/fTest -i /afs/cern.ch/user/f/ftorresd/workSpace/public/paraElizaESandro/combineIssue/HZToUpsilonPhotonStatisticsAndLimits/inputData/fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_workspace_forFLASHGG_Cat0.root --saveMultiPdf CMS-HGG_multipdf_HToJPsiGamma_afterFtest.root -D outdir_HToJPsiGamma_example/bkgfTest-Data -f UntaggedTag_0 --isData 1

```

### Instruction to use combine packaged to make Bias Studies
 The combine version is 74X, rather than 81X which I see you are using, but the commands are very similar. If you want to run the code as is, you would also need to include this small modification I made in this private link on git 
```
https://github.com/edjtscott/HiggsAnalysis-CombinedLimit/commit/a76d683aaf72ef92d468714bfddb9e2181e13ae7


```
### Bias Studies recipes following Combine tutorial 2015: 
https://indico.cern.ch/event/456547/contributions/1126037/attachments/1188555/1724451/biasstudy.pdf


```
cd $CMSSW_BASE/src/HiggsAnalysis/HZToUpsilonPhotonStatisticsAndLimits/bias_combine
-Step 0
combine BiasStudy_Toy_Datacard.txt -M GenerateOnly --setPhysicsModelParameters pdf_index=0,r=200 -t 5000 --expectSignal 200 --saveToys -m 125 --freezeNuisances pdf_index,r -s 98765 --freezeNuisanceGroups signal -n HiggsToJpsiToy

- Step 1
 combine BiasStudy_Toy_Datacard.txt -M MaxLikelihoodFit -s 1 --plots --saveShapes --setPhysicsModelParameters pdf_index=0,r=200.0 --toysFile higgsCombineHiggsToJpsiToy.GenerateOnly.mH125.98765.root -t 5000 --rMin -5000.0 --rMax 5000.0 --freezeNuisances pdf_index --freezeNuisanceGroups signal -n HiggsToJpsiToyFits2 -v4 > & HiggsToJpsiToyFits2.log
