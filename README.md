# HZToUpsilonPhotonStatisticsAndLimits

## Setup

- Setup the Higgs Combine Tool
```
# Higgs COMBINE
cd <PATH TO A CMSSW_8_0_26_patch1>
cd $CMSSW_BASE/src
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
git fetch origin
git checkout v7.0.8
cd $CMSSW_BASE/src
scramv1 b clean
scramv1 b 
```
More info: https://cms-hcomb.gitbooks.io/combine/

- Clone and setup:
```
# Statistics and Limits Subtree
git clone git@github.com:ftorresd/HZToUpsilonPhotonStatisticsAndLimits.git
cd HZToUpsilonPhotonStatisticsAndLimits
mkdir fits/inputData
mkdir ftest/inputData
mkdir limits/inputData
mkdir limits/outputDatacards2D
mkdir limits/outputLimits2D

cd ftest
make

cd $CMSSW_BASE/src/HZToUpsilonPhotonStatisticsAndLimits
```

## Workflow

1) Fits: https://github.com/ftorresd/HZToUpsilonPhotonStatisticsAndLimits/blob/master/fits/README.md
2) F-Test: https://github.com/ftorresd/HZToUpsilonPhotonStatisticsAndLimits/blob/master/ftest/README.md
3) Limits: https://github.com/ftorresd/HZToUpsilonPhotonStatisticsAndLimits/blob/master/limits/README.md

