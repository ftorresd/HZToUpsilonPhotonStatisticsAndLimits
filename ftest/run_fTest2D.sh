#!/usr/bin/env bash


rm -rf ftestOutput2D/*
rm -rf *.root 
rm -rf outdir* 
rm -rf output_*.txt 

mkdir ftestOutput2D
touch ftestOutput2D/bckgNormalization.json
echo "{}" > ftestOutput2D/bckgNormalization.json

#####################
#####################


 #
# HToUpsilonPhoton - Cat0
./bin/fTest2D \
-i inputData/fitPlotFiles2D/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilon1SPhotonSignalAndBackgroundFit_workspace_Cat0.root \
--saveMultiPdf CMS-HGG_multipdf_HToUpsilonPhoton_Cat0_afterFtest.root \
-D outdir_HToUpsilonPhoton_Cat0/bkgfTest-Data \
-f UntaggedTag_0 \
--isData 1 \
--isZ 0 \
--HZToUpsilonPhotonCat Cat0 \
--analysisBranch HToUpsilonPhoton >> output_HToUpsilonPhoton_Cat0.txt 2>&1 

cat output_HToUpsilonPhoton_Cat0.txt



# ZToUpsilonPhoton - Cat0
./bin/fTest2D \
-i inputData/fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilon1SPhotonSignalAndBackgroundFit_workspace_Cat0.root \
--saveMultiPdf CMS-HGG_multipdf_ZToUpsilonPhoton_Cat0_afterFtest.root \
-D outdir_ZToUpsilonPhoton_Cat0/bkgfTest-Data \
-f UntaggedTag_0 \
--isData 1 \
--isZ 1 \
--HZToUpsilonPhotonCat Cat0 \
--analysisBranch ZToUpsilonPhoton >> output_ZToUpsilonPhoton_Cat0.txt 2>&1 

cat output_ZToUpsilonPhoton_Cat0.txt


# ZToUpsilonPhoton - Cat1
./bin/fTest2D \
-i inputData/fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilon1SPhotonSignalAndBackgroundFit_workspace_Cat1.root \
--saveMultiPdf CMS-HGG_multipdf_ZToUpsilonPhoton_Cat1_afterFtest.root \
-D outdir_ZToUpsilonPhoton_Cat1/bkgfTest-Data \
-f UntaggedTag_0 \
--isData 1 \
--isZ 1 \
--HZToUpsilonPhotonCat Cat1 \
--analysisBranch ZToUpsilonPhoton >> output_ZToUpsilonPhoton_Cat1.txt 2>&1 

cat output_ZToUpsilonPhoton_Cat1.txt


# ZToUpsilonPhoton - Cat2
./bin/fTest2D \
-i inputData/fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilon1SPhotonSignalAndBackgroundFit_workspace_Cat2.root \
--saveMultiPdf CMS-HGG_multipdf_ZToUpsilonPhoton_Cat2_afterFtest.root \
-D outdir_ZToUpsilonPhoton_Cat2/bkgfTest-Data \
-f UntaggedTag_0 \
--isData 1 \
--isZ 1 \
--HZToUpsilonPhotonCat Cat2 \
--analysisBranch ZToUpsilonPhoton >> output_ZToUpsilonPhoton_Cat2.txt 2>&1 

cat output_ZToUpsilonPhoton_Cat2.txt



# ZToUpsilonPhoton - Cat3
./bin/fTest2D \
-i inputData/fitPlotFiles2D/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilon1SPhotonSignalAndBackgroundFit_workspace_Cat3.root \
--saveMultiPdf CMS-HGG_multipdf_ZToUpsilonPhoton_Cat3_afterFtest.root \
-D outdir_ZToUpsilonPhoton_Cat3/bkgfTest-Data \
-f UntaggedTag_0 \
--isData 1 \
--isZ 1 \
--HZToUpsilonPhotonCat Cat3 \
--analysisBranch ZToUpsilonPhoton >> output_ZToUpsilonPhoton_Cat3.txt 2>&1 

cat output_ZToUpsilonPhoton_Cat3.txt


############################
############################
# rm -rf ftestOutput/*
mv *.root ftestOutput2D/.
mv outdir* ftestOutput2D/.
mv output_*.txt ftestOutput2D/.


find ftestOutput2D/ -type d -exec cp plugins/index.php {} \;
