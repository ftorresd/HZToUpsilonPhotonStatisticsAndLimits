#!/bin/bash

rm fitPlotFiles/effSigmaJSON.json
rm fitPlotFiles/signalMeanSigmaJSON.json

ulimit -n 2048

time root -b -q -l run_Fitter.C+

find fitPlotFiles/ -type d -exec cp plugins/index.php {} \;

