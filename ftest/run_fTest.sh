#!/usr/bin/env bash


rm -rf ftestOutput/*
rm -rf *.root 
rm -rf outdir* 
rm -rf output_*.txt 


#####################
#####################



# # TEST
# ./bin/fTest \
# -i inputData/fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_workspace_forFLASHGG_Cat0.root \
# --saveMultiPdf CMS-HGG_multipdf_HToJPsiPhoton_Cat0_afterFtest.root \
# -D outdir_HToJPsiPhoton_Cat0/bkgfTest-Data \
# -f UntaggedTag_0 \
# --isData 1 \
# --isZ 0 \
# --HZToUpsilonPhotonCat Cat0\ 
# --analysisBranch HToJPsiPhoton 


# HToJPsiPhoton - Cat0
./bin/fTest \
-i inputData/fitPlotFiles/HToJPsiPhotonSignalAndBackgroundFit/HToJPsiPhotonSignalAndBackgroundFit_workspace_forFLASHGG_Cat0.root \
--saveMultiPdf CMS-HGG_multipdf_HToJPsiPhoton_Cat0_afterFtest.root \
-D outdir_HToJPsiPhoton_Cat0/bkgfTest-Data \
-f UntaggedTag_0 \
--isData 1 \
--isZ 0 \
--HZToUpsilonPhotonCat Cat0 \
--analysisBranch HToJPsiPhoton >> output_HToJPsiPhoton_Cat0.txt 2>&1

cat output_HToJPsiPhoton_Cat0.txt
 

# HToUpsilonPhoton - Cat0
./bin/fTest \
-i inputData/fitPlotFiles/HToUpsilonPhotonSignalAndBackgroundFit/HToUpsilonPhotonSignalAndBackgroundFit_workspace_forFLASHGG_Cat0.root \
--saveMultiPdf CMS-HGG_multipdf_HToUpsilonPhoton_Cat0_afterFtest.root \
-D outdir_HToUpsilonPhoton_Cat0/bkgfTest-Data \
-f UntaggedTag_0 \
--isData 1 \
--isZ 0 \
--HZToUpsilonPhotonCat Cat0 \
--analysisBranch HToUpsilonPhoton >> output_HToUpsilonPhoton_Cat0.txt 2>&1

cat output_HToUpsilonPhoton_Cat0.txt


# ZToJPsiPhoton - Cat0
./bin/fTest \
-i inputData/fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_workspace_forFLASHGG_Cat0.root \
--saveMultiPdf CMS-HGG_multipdf_ZToJPsiPhoton_Cat0_afterFtest.root \
-D outdir_ZToJPsiPhoton_Cat0/bkgfTest-Data \
-f UntaggedTag_0 \
--isData 1 \
--isZ 1 \
--HZToUpsilonPhotonCat Cat0 \
--analysisBranch ZToJPsiPhoton >> output_ZToJPsiPhoton_Cat0.txt 2>&1

cat output_ZToJPsiPhoton_Cat0.txt


# ZToJPsiPhoton - Cat1
./bin/fTest \
-i inputData/fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_workspace_forFLASHGG_Cat1.root \
--saveMultiPdf CMS-HGG_multipdf_ZToJPsiPhoton_Cat1_afterFtest.root \
-D outdir_ZToJPsiPhoton_Cat1/bkgfTest-Data \
-f UntaggedTag_0 \
--isData 1 \
--isZ 1 \
--HZToUpsilonPhotonCat Cat1 \
--analysisBranch ZToJPsiPhoton >> output_ZToJPsiPhoton_Cat1.txt 2>&1

cat output_ZToJPsiPhoton_Cat1.txt


# ZToJPsiPhoton - Cat2
./bin/fTest \
-i inputData/fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_workspace_forFLASHGG_Cat2.root \
--saveMultiPdf CMS-HGG_multipdf_ZToJPsiPhoton_Cat2_afterFtest.root \
-D outdir_ZToJPsiPhoton_Cat2/bkgfTest-Data \
-f UntaggedTag_0 \
--isData 1 \
--isZ 1 \
--HZToUpsilonPhotonCat Cat2 \
--analysisBranch ZToJPsiPhoton >> output_ZToJPsiPhoton_Cat2.txt 2>&1

cat output_ZToJPsiPhoton_Cat2.txt



# ZToJPsiPhoton - Cat3
./bin/fTest \
-i inputData/fitPlotFiles/ZToJPsiPhotonSignalAndBackgroundFit/ZToJPsiPhotonSignalAndBackgroundFit_workspace_forFLASHGG_Cat3.root \
--saveMultiPdf CMS-HGG_multipdf_ZToJPsiPhoton_Cat3_afterFtest.root \
-D outdir_ZToJPsiPhoton_Cat3/bkgfTest-Data \
-f UntaggedTag_0 \
--isData 1 \
--isZ 1 \
--HZToUpsilonPhotonCat Cat3 \
--analysisBranch ZToJPsiPhoton >> output_ZToJPsiPhoton_Cat3.txt 2>&1

cat output_ZToJPsiPhoton_Cat3.txt



# ZToUpsilonPhoton - Cat0
./bin/fTest \
-i inputData/fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_workspace_forFLASHGG_Cat0.root \
--saveMultiPdf CMS-HGG_multipdf_ZToUpsilonPhoton_Cat0_afterFtest.root \
-D outdir_ZToUpsilonPhoton_Cat0/bkgfTest-Data \
-f UntaggedTag_0 \
--isData 1 \
--isZ 1 \
--HZToUpsilonPhotonCat Cat0 \
--analysisBranch ZToUpsilonPhoton >> output_ZToUpsilonPhoton_Cat0.txt 2>&1

cat output_ZToUpsilonPhoton_Cat0.txt


# ZToUpsilonPhoton - Cat1
./bin/fTest \
-i inputData/fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_workspace_forFLASHGG_Cat1.root \
--saveMultiPdf CMS-HGG_multipdf_ZToUpsilonPhoton_Cat1_afterFtest.root \
-D outdir_ZToUpsilonPhoton_Cat1/bkgfTest-Data \
-f UntaggedTag_0 \
--isData 1 \
--isZ 1 \
--HZToUpsilonPhotonCat Cat1 \
--analysisBranch ZToUpsilonPhoton >> output_ZToUpsilonPhoton_Cat1.txt 2>&1

cat output_ZToUpsilonPhoton_Cat1.txt


# ZToUpsilonPhoton - Cat2
./bin/fTest \
-i inputData/fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_workspace_forFLASHGG_Cat2.root \
--saveMultiPdf CMS-HGG_multipdf_ZToUpsilonPhoton_Cat2_afterFtest.root \
-D outdir_ZToUpsilonPhoton_Cat2/bkgfTest-Data \
-f UntaggedTag_0 \
--isData 1 \
--isZ 1 \
--HZToUpsilonPhotonCat Cat2 \
--analysisBranch ZToUpsilonPhoton >> output_ZToUpsilonPhoton_Cat2.txt 2>&1

cat output_ZToUpsilonPhoton_Cat2.txt



# ZToUpsilonPhoton - Cat3
./bin/fTest \
-i inputData/fitPlotFiles/ZToUpsilonPhotonSignalAndBackgroundFit/ZToUpsilonPhotonSignalAndBackgroundFit_workspace_forFLASHGG_Cat3.root \
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
mv *.root ftestOutput/.
mv outdir* ftestOutput/.
mv output_*.txt ftestOutput/.
