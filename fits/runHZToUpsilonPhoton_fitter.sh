#!/bin/bash

time root -b -q -l run_Fitter.C+

find fitPlotFiles/ -type d -exec cp ../plugins/index.php {} \;

