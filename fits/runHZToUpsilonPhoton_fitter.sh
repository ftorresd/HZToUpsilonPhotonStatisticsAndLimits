#!/bin/bash

rm -rf fitPlotFiles/*
rm -rf fitPlotFiles2D/*

mkdir -p fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit
mkdir -p fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit

ulimit -n 2048

time root -b -q -l run_Fitter.C+

find fitPlotFiles/ -type d -exec cp plugins/index.php {} \;
find fitPlotFiles2D/ -type d -exec cp plugins/index.php {} \;

