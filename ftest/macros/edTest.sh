{\rtf1\ansi\ansicpg1252\cocoartf1404\cocoasubrtf470
{\fonttbl\f0\fnil\fcharset0 Monaco;}
{\colortbl;\red255\green255\blue255;\red242\green242\blue242;\red60\green95\blue255;\red47\green189\blue29;
\red200\green20\blue201;\red180\green36\blue25;\red46\green174\blue187;}
\paperw11900\paperh16840\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0

\f0\fs36 \cf2 \cb0 \CocoaLigature0 Password: \
mount_osxfuse: mount point /Users/sfonseca/lxplus_user is itself on a OSXFUSE volume\
MacBook-Air-de-Sandro-Fonseca:~ sfonseca$ ssh -X lxplus.cern.ch\
Warning: Permanently added the RSA host key for IP address '137.138.148.251' to the list of known hosts.\
Password: \
* ********************************************************************\
* Welcome to lxplus016.cern.ch, SLC, 6.9\
* Archive of news is available in /etc/motd-archive\
* Reminder: You have agreed to comply with the CERN computing rules\
* https://cern.ch/ComputingRules\
* Puppet environment: production, Roger state: production\
* Foreman hostgroup: lxplus/nodes/login\
* Availability zone: cern-geneva-c\
* LXPLUS Public Login Service\
* ********************************************************************\
[sfonseca@lxplus016 ~]$ cd /afs/cern.ch/work/s/sfonseca/\
ForFelipe/         my_vm_key.pem.pub  public/            ZJpsi_Gamma/\
Muon_POG_low_pT/   PPS/               storage1/          ZUpsilonGamma/\
my_vm_key.pem      private/           VM/                \
[sfonseca@lxplus016 ~]$ cd /afs/cern.ch/work/s/sfonseca/ZUpsilonGamma/\
2016/                   CMSSW_940pre2_DQMtest/  NTuples2017/\
CMSSW_10_0_0_pre1/      github/                 upsilonFit/\
CMSSW_1000pre1_DQMtest/ MC/                     \
CMSSW_9_4_0_pre2/       mc_gen/                 \
[sfonseca@lxplus016 ~]$ cd /afs/cern.ch/work/s/sfonseca/ZUpsilonGamma/2016/\
CMSSW_8_0_26_patch1/ PU/                  \
FTest/               StaticTest/          \
[sfonseca@lxplus016 ~]$ cd /afs/cern.ch/work/s/sfonseca/ZUpsilonGamma/2016/StaticTest/\
CMSSW_7_4_7/ CMSSW_8_1_0/ git/         test/        \
[sfonseca@lxplus016 ~]$ cd /afs/cern.ch/work/s/sfonseca/ZUpsilonGamma/2016/StaticTest/git/CMSSW_7_4_7/\
biglib/    cfipython/ doc/       include/   logs/      python/    test/\
bin/       config/    external/  lib/       objs/      src/       tmp/\
[sfonseca@lxplus016 ~]$ cd /afs/cern.ch/work/s/sfonseca/ZUpsilonGamma/2016/StaticTest/test/CMSSW_8_1_0/\
biglib/    cfipython/ doc/       include/   logs/      python/    test/\
bin/       config/    external/  lib/       objs/      src/       tmp/\
[sfonseca@lxplus016 ~]$ cd /afs/cern.ch/work/s/sfonseca/ZUpsilonGamma/2016/StaticTest/git/CMSSW_7_4_7/\
biglib/    cfipython/ doc/       include/   logs/      python/    test/\
bin/       config/    external/  lib/       objs/      src/       tmp/\
[sfonseca@lxplus016 ~]$ cd /afs/cern.ch/work/s/sfonseca/ZUpsilonGamma/2016/StaticTest/git/CMSSW_7_4_7/src/HiggsAnalysis/\
CombinedLimit/                        HZToUpsilonPhotonStatisticsAndLimits/\
GBRLikelihood/                        \
[sfonseca@lxplus016 ~]$ cd /afs/cern.ch/work/s/sfonseca/ZUpsilonGamma/2016/StaticTest/git/CMSSW_7_4_7/src/HiggsAnalysis/HZToUpsilonPhotonStatisticsAndLimits/\
bias_combine/ fits/         ftest/        limits/       \
[sfonseca@lxplus016 ~]$ cd /afs/cern.ch/work/s/sfonseca/ZUpsilonGamma/2016/StaticTest/git/CMSSW_7_4_7/src/HiggsAnalysis/HZToUpsilonPhotonStatisticsAndLimits/f\
fits/  ftest/ \
[sfonseca@lxplus016 ~]$ cd /afs/cern.ch/work/s/sfonseca/ZUpsilonGamma/2016/StaticTest/git/CMSSW_7_4_7/src/HiggsAnalysis/HZToUpsilonPhotonStatisticsAndLimits/ftest/\
[sfonseca@lxplus016 ftest]$ ls\
\cf3 bin\cf2                                             \cf3 outdir_HToJPsiGamma_example\cf2 \
BuildFile.xml                                  README.md\
CMS-HGG_multipdf_HToJPsiGamma_afterFtest.root  RootDict.cxx\
\cf3 dat\cf2                                             RootDict.o\
\cf3 interface\cf2                                       RootDict_rdict.pcm\
\cf3 lib\cf2                                             \cf3 src\cf2 \
\cf3 macros\cf2                                          \cf3 tdrStyle\cf2 \
makefile                                       \cf3 test\cf2 \
\cf3 obj\cf2 \
[sfonseca@lxplus016 ftest]$ cp ../../../../\
biglib/    cfipython/ doc/       include/   logs/      python/    test/\
bin/       config/    external/  lib/       objs/      src/       tmp/\
[sfonseca@lxplus016 ftest]$ cp ../../../.././\
biglib/    cfipython/ doc/       include/   logs/      python/    test/\
bin/       config/    external/  lib/       objs/      src/       tmp/\
[sfonseca@lxplus016 ftest]$ cp ../../../../../CMSSW_7_4_7/\
biglib/    cfipython/ doc/       include/   logs/      python/    test/\
bin/       config/    external/  lib/       objs/      src/       tmp/\
[sfonseca@lxplus016 ftest]$ cp ../../../../../../\
CMSSW_7_4_7/ CMSSW_8_1_0/ git/         test/        \
[sfonseca@lxplus016 ftest]$ cp ../../../../../../CM\
CMSSW_7_4_7/ CMSSW_8_1_0/ \
[sfonseca@lxplus016 ftest]$ cp ../../../../../../CMSSW_7_4_7/\
biglib/    cfipython/ doc/       include/   logs/      python/    test/\
bin/       config/    external/  lib/       objs/      src/       tmp/\
[sfonseca@lxplus016 ftest]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/\
CombinedLimit/   flashggFinalFit/ GBRLikelihood/   \
[sfonseca@lxplus016 ftest]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/flashggFinalFit/\
Background/             justCopyFiles.sh*       runFinalFitsScripts.sh*\
convertXS.py*           makeEffAcc.py*          Signal/\
Datacard/               Plots/                  tdrStyle/\
edRunCombineScripts.sh* plotweightedbands/      yieldsTableColour.py*\
Examples/               README.md               yieldsTable.py*\
[sfonseca@lxplus016 ftest]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/flashggFinalFit/Background/\
background_pdfs.root \
bckp/\
BiasStudy/\
bin/\
BuildFile.xml \
CMS-HGG_multipdf_HggAnalysis_ICHEP2016_example.root \
CMS-HGG_multipdf_HToJPsiGamma_afterFtest.root \
CMS-HGG_multipdf_HToJPsiGamma_example.root \
dat/\
dumpTreeTotxt.C \
dumpTreeTotxt_C_ACLiC_dict_rdict.pcm \
dumpTreeTotxt_C.d \
dumpTreeTotxt_C.so*\
edRunBackgroundScripts.sh*\
fitBkg.png \
interface/\
lib/\
macros/\
makefile \
makeRooMultiPdfToWS.C \
mHZ_2016.txt \
obj/\
outdir_HggAnalysis_ICHEP2016_example/\
outdir_HToJPsiGamma_example/\
printWS.C \
README.md \
rf402_datahandling.root \
RootDict.cxx \
RootDict.o \
RootDict_rdict.pcm \
runBackgroundScripts.sh*\
scripts/\
src/\
test/\
wspaceread.C \
wspacereadCombine.C \
[sfonseca@lxplus016 ftest]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/flashggFinalFit/Background/d\
dat/                                  dumpTreeTotxt_C.d \
dumpTreeTotxt.C                       dumpTreeTotxt_C.so*\
dumpTreeTotxt_C_ACLiC_dict_rdict.pcm  \
[sfonseca@lxplus016 ftest]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/flashggFinalFit/Background/du\
dumpTreeTotxt.C                       dumpTreeTotxt_C.d \
dumpTreeTotxt_C_ACLiC_dict_rdict.pcm  dumpTreeTotxt_C.so*\
[sfonseca@lxplus016 ftest]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/flashggFinalFit/Background/dumpTreeTotxt.C ma\
macros/   makefile  \
[sfonseca@lxplus016 ftest]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/flashggFinalFit/Background/dumpTreeTotxt.C macros/\
[sfonseca@lxplus016 ftest]$ cd m\
macros/   makefile  \
[sfonseca@lxplus016 ftest]$ cd macros/\
[sfonseca@lxplus016 macros]$ ls\
dumpTreeTotxt.C  makeRooMultiPdfToWS.C  mHZ_2016.txt  printWS.C  wspaceread.C\
[sfonseca@lxplus016 macros]$ vi dumpTreeTotxt.C \
[sfonseca@lxplus016 macros]$ git add dumpTreeTotxt.C \
[sfonseca@lxplus016 macros]$ git commit -m "extracted data information" dumpTreeTotxt.C \
[master 7c622ae] extracted data information\
 1 files changed, 34 insertions(+), 0 deletions(-)\
 create mode 100644 ftest/macros/dumpTreeTotxt.C\
[sfonseca@lxplus016 macros]$ git push orign master\
fatal: 'orign' does not appear to be a git repository\
fatal: The remote end hung up unexpectedly\
[sfonseca@lxplus016 macros]$ git push origin master\
Enter passphrase for key '/afs/cern.ch/user/s/sfonseca/.ssh/id_rsa': \
To git@github.com:ftorresd/HZToUpsilonPhotonStatisticsAndLimits.git\
 ! [rejected]        master -> master (non-fast-forward)\
error: failed to push some refs to 'git@github.com:ftorresd/HZToUpsilonPhotonStatisticsAndLimits.git'\
To prevent you from losing history, non-fast-forward updates were rejected\
Merge the remote changes before pushing again.  See the 'Note about\
fast-forwards' section of 'git push --help' for details.\
[sfonseca@lxplus016 macros]$ cd ..\
[sfonseca@lxplus016 ftest]$ git push origin master\
Enter passphrase for key '/afs/cern.ch/user/s/sfonseca/.ssh/id_rsa': \
To git@github.com:ftorresd/HZToUpsilonPhotonStatisticsAndLimits.git\
 ! [rejected]        master -> master (non-fast-forward)\
error: failed to push some refs to 'git@github.com:ftorresd/HZToUpsilonPhotonStatisticsAndLimits.git'\
To prevent you from losing history, non-fast-forward updates were rejected\
Merge the remote changes before pushing again.  See the 'Note about\
fast-forwards' section of 'git push --help' for details.\
[sfonseca@lxplus016 ftest]$ git -f push origin master\
Unknown option: -f\
usage: git [--version] [--exec-path[=GIT_EXEC_PATH]] [--html-path]\
           [-p|--paginate|--no-pager] [--no-replace-objects]\
           [--bare] [--git-dir=GIT_DIR] [--work-tree=GIT_WORK_TREE]\
           [--help] COMMAND [ARGS]\
[sfonseca@lxplus016 ftest]$ git force push origin master\
git: 'force' is not a git command. See 'git --help'.\
[sfonseca@lxplus016 ftest]$ git push --force origin master\
Enter passphrase for key '/afs/cern.ch/user/s/sfonseca/.ssh/id_rsa': \
Counting objects: 672, done.\
Delta compression using up to 10 threads.\
Compressing objects: 100% (437/437), done.\
Writing objects: 100% (672/672), 12.91 MiB | 3.47 MiB/s, done.\
Total 672 (delta 260), reused 566 (delta 230)\
remote: Resolving deltas: 100% (260/260), done.\
To git@github.com:ftorresd/HZToUpsilonPhotonStatisticsAndLimits.git\
 + cc36efa...7c622ae master -> master (forced update)\
[sfonseca@lxplus016 ftest]$ vi R\
README.md           RootDict.cxx        RootDict.o          RootDict_rdict.pcm \
[sfonseca@lxplus016 ftest]$ vi R\
README.md           RootDict.cxx        RootDict.o          RootDict_rdict.pcm \
[sfonseca@lxplus016 ftest]$ vi README.md \
[sfonseca@lxplus016 ftest]$ ls -lha\
total 694K\
drwxr-xr-x. 12 sfonseca zh 2.0K Jan 29 19:52 \cf3 .\cf2 \
drwxr-xr-x.  7 sfonseca zh 2.0K Jan 29 17:51 \cf3 ..\cf2 \
drwxr-xr-x.  2 sfonseca zh 2.0K Jan 29 17:15 \cf3 bin\cf2 \
-rw-r--r--.  1 sfonseca zh  445 Jan 29 10:48 BuildFile.xml\
-rw-r--r--.  1 sfonseca zh  18K Jan 29 17:18 CMS-HGG_multipdf_HToJPsiGamma_afterFtest.root\
drwxr-xr-x.  2 sfonseca zh 2.0K Jan 29 17:16 \cf3 dat\cf2 \
drwxr-xr-x.  2 sfonseca zh 2.0K Jan 29 10:52 \cf3 interface\cf2 \
drwxr-xr-x.  2 sfonseca zh 2.0K Jan 29 17:07 \cf3 lib\cf2 \
drwxr-xr-x.  2 sfonseca zh 2.0K Jan 29 19:47 \cf3 macros\cf2 \
-rw-r--r--.  1 sfonseca zh 3.2K Jan 29 10:53 makefile\
drwxr-xr-x.  2 sfonseca zh 2.0K Jan 29 17:07 \cf3 obj\cf2 \
drwxr-xr-x.  3 sfonseca zh 2.0K Jan 29 13:43 \cf3 outdir_HToJPsiGamma_example\cf2 \
-rw-r--r--.  1 sfonseca zh    0 Jan 29 17:33 README.md\
-rw-r--r--.  1 sfonseca zh  20K Jan 29 17:05 RootDict.cxx\
-rw-r--r--.  1 sfonseca zh 622K Jan 29 17:06 RootDict.o\
-rw-r--r--.  1 sfonseca zh 4.9K Jan 29 17:05 RootDict_rdict.pcm\
drwxr-xr-x.  2 sfonseca zh 2.0K Jan 29 10:52 \cf3 src\cf2 \
drwxr-xr-x.  2 sfonseca zh 2.0K Jan 29 10:56 \cf3 tdrStyle\cf2 \
drwxr-xr-x.  2 sfonseca zh 2.0K Jan 29 13:39 \cf3 test\cf2 \
[sfonseca@lxplus016 ftest]$ cd test/\
[sfonseca@lxplus016 test]$ ls\
BackgroundProfileTest.cpp  makeBkgPlots.cpp       plotweightedsigEd.cpp\
BiasStudy.cpp              makeFakeData.cpp       pseudodataMaker.cpp\
fTest.cpp                  plotweightedbands.cpp  workspaceTool.cpp\
[sfonseca@lxplus016 test]$ cd ..\
[sfonseca@lxplus016 ftest]$ ls\
\cf3 bin\cf2                                             \cf3 outdir_HToJPsiGamma_example\cf2 \
BuildFile.xml                                  README.md\
CMS-HGG_multipdf_HToJPsiGamma_afterFtest.root  RootDict.cxx\
\cf3 dat\cf2                                             RootDict.o\
\cf3 interface\cf2                                       RootDict_rdict.pcm\
\cf3 lib\cf2                                             \cf3 src\cf2 \
\cf3 macros\cf2                                          \cf3 tdrStyle\cf2 \
makefile                                       \cf3 test\cf2 \
\cf3 obj\cf2 \
[sfonseca@lxplus016 ftest]$ vi README.md \
[sfonseca@lxplus016 ftest]$ vi README.md\
[sfonseca@lxplus016 ftest]$ git commit -m "extracted data information" R\
README.md           RootDict.cxx        RootDict.o          RootDict_rdict.pcm \
[sfonseca@lxplus016 ftest]$ git commit -m "recipes" README.md\
[master 4590718] recipes\
 1 files changed, 25 insertions(+), 0 deletions(-)\
[sfonseca@lxplus016 ftest]$ git push origin master\
Enter passphrase for key '/afs/cern.ch/user/s/sfonseca/.ssh/id_rsa': \
Counting objects: 7, done.\
Delta compression using up to 10 threads.\
Compressing objects: 100% (4/4), done.\
Writing objects: 100% (4/4), 642 bytes, done.\
Total 4 (delta 2), reused 0 (delta 0)\
remote: Resolving deltas: 100% (2/2), completed with 2 local objects.\
To git@github.com:ftorresd/HZToUpsilonPhotonStatisticsAndLimits.git\
   7c622ae..4590718  master -> master\
[sfonseca@lxplus016 ftest]$ pwd\
/afs/cern.ch/work/s/sfonseca/ZUpsilonGamma/2016/StaticTest/git/CMSSW_7_4_7/src/HiggsAnalysis/HZToUpsilonPhotonStatisticsAndLimits/ftest\
[sfonseca@lxplus016 ftest]$ pwd\
/afs/cern.ch/work/s/sfonseca/ZUpsilonGamma/2016/StaticTest/git/CMSSW_7_4_7/src/HiggsAnalysis/HZToUpsilonPhotonStatisticsAndLimits/ftest\
[sfonseca@lxplus016 ftest]$ cd ..\
[sfonseca@lxplus016 HZToUpsilonPhotonStatisticsAndLimits]$ cd bias_combine/\
[sfonseca@lxplus016 bias_combine]$ pwd\
/afs/cern.ch/work/s/sfonseca/ZUpsilonGamma/2016/StaticTest/git/CMSSW_7_4_7/src/HiggsAnalysis/HZToUpsilonPhotonStatisticsAndLimits/bias_combine\
[sfonseca@lxplus016 bias_combine]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/ macros/\
bin/               env_standalone.sh  Makefile           scripts/\
BuildFile.xml      interface/         python/            src/\
data/              macros/            README.md          test/\
[sfonseca@lxplus016 bias_combine]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/data/ macros/\
benchmarks/            biasStudy_HZJPsiGamma/ lhc-hcg/\
biasStudy_Ed/          hbb/                   lhc-hxswg/\
BiasStudyHaoRen/       hww/                   tutorials/\
[sfonseca@lxplus016 bias_combine]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/data/bi macros/\
biasStudy_Ed/          biasStudy_HZJPsiGamma/ \
[sfonseca@lxplus016 bias_combine]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/data/biasStudy_HZJPsiGamma/ macros/\
background_pdfsHaoRen.root \
background_pdfs.root \
BiasStudy_Toy_Datacard.txt*\
cat1_mHZ_fit_b.png \
cat1_mHZ_fit_s.png \
cat1_mHZ_prefit.png \
covariance_fit_b.png \
covariance_fit_s.png \
edCheckBias.sh*\
higgsCombineHiggsToJpsiToyFits2.MaxLikelihoodFit.mH120.1.root \
higgsCombineHiggsToJpsiToyFits.MaxLikelihoodFit.mH120.1.root \
higgsCombineHiggsToJpsiToy.GenerateOnly.mH125.98765.root \
higgsCombineRealToysPdf0Signal0Fit.MaxLikelihoodFit.mH125.123456.root \
higgsCombineRealToysPdf0Signal0Fit.MaxLikelihoodFit.mH125.1.root \
higgsCombineRealToysPdf0Signal0.GenerateOnly.mH125.123456.root \
HiggsToJpsiToyFits2.log \
Hists/\
HToJPsiPhotonSignalAndBackgroundFit_workspace_Cat0.root \
log_mMaxLike.txt \
mlfitHiggsToJpsiToyFits2.root \
mlfitHiggsToJpsiToyFits.root \
mlfitRealToysPdf0Signal0Fit.root \
plotbias.log \
plotBias.py*\
roostats-0L49Su.root \
roostats-43qlJJ.root \
roostats-beeyLo.root \
roostats-BmcpVV.root \
roostats-BmcpVV.root.dot \
roostats-eqxOLD.root \
roostats-gWRW8y.root \
roostats-QDREwq.root \
roostats-TRoPCW.root.dot \
roostats-XA4Cr0.root \
Sandro.log \
testbias_Ed/\
wspaceread.C \
[sfonseca@lxplus016 bias_combine]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/data/biasStudy_HZJPsiGamma/testbias_Ed/ macros/\
background_pdfs.root \
BiasStudy_Toy_Datacard.txt \
cat0_mHZ_prefit.png \
cat1_mHZ_fit_b.png \
cat1_mHZ_fit_s.png \
cat1_mHZ_prefit.png \
covariance_fit_b.png \
covariance_fit_s.png \
higgsCombineRealToysPdf0Signal0Fit.MaxLikelihoodFit.mH125.1.root \
higgsCombineRealToysPdf0Signal0.GenerateOnly.mH125.123456.root \
mlfitHiggsToJpsiToyFits.root \
mlfitRealToysPdf0Signal0Fit.root \
roostats-6E5iw4.root \
roostats-mDZ42E.root \
roostats-oPmiBW.root \
[sfonseca@lxplus016 bias_combine]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/data/biasStudy_HZJPsiGamma/ macros/\
background_pdfsHaoRen.root \
background_pdfs.root \
BiasStudy_Toy_Datacard.txt*\
cat1_mHZ_fit_b.png \
cat1_mHZ_fit_s.png \
cat1_mHZ_prefit.png \
covariance_fit_b.png \
covariance_fit_s.png \
edCheckBias.sh*\
higgsCombineHiggsToJpsiToyFits2.MaxLikelihoodFit.mH120.1.root \
higgsCombineHiggsToJpsiToyFits.MaxLikelihoodFit.mH120.1.root \
higgsCombineHiggsToJpsiToy.GenerateOnly.mH125.98765.root \
higgsCombineRealToysPdf0Signal0Fit.MaxLikelihoodFit.mH125.123456.root \
higgsCombineRealToysPdf0Signal0Fit.MaxLikelihoodFit.mH125.1.root \
higgsCombineRealToysPdf0Signal0.GenerateOnly.mH125.123456.root \
HiggsToJpsiToyFits2.log \
Hists/\
HToJPsiPhotonSignalAndBackgroundFit_workspace_Cat0.root \
log_mMaxLike.txt \
mlfitHiggsToJpsiToyFits2.root \
mlfitHiggsToJpsiToyFits.root \
mlfitRealToysPdf0Signal0Fit.root \
plotbias.log \
plotBias.py*\
roostats-0L49Su.root \
roostats-43qlJJ.root \
roostats-beeyLo.root \
roostats-BmcpVV.root \
roostats-BmcpVV.root.dot \
roostats-eqxOLD.root \
roostats-gWRW8y.root \
roostats-QDREwq.root \
roostats-TRoPCW.root.dot \
roostats-XA4Cr0.root \
Sandro.log \
testbias_Ed/\
wspaceread.C \
[sfonseca@lxplus016 bias_combine]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/data/biasStudy_HZJPsiGamma/edCheckBias.sh  macros/\
cp: cannot create regular file `macros/': Is a directory\
[sfonseca@lxplus016 bias_combine]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/data/biasStudy_HZJPsiGamma/edCheckBias.sh \
background_pdfs.root \
BiasStudy_Toy_Datacard.txt*\
HToJPsiPhotonSignalAndBackgroundFit_workspace_Cat0.root \
plotBias.py*\
wspaceread.C \
[sfonseca@lxplus016 bias_combine]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/data/biasStudy_HZJPsiGamma/edCheckBias.sh \
background_pdfs.root \
BiasStudy_Toy_Datacard.txt*\
HToJPsiPhotonSignalAndBackgroundFit_workspace_Cat0.root \
plotBias.py*\
wspaceread.C \
[sfonseca@lxplus016 bias_combine]$ cp ../../../../../../CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/data/biasStudy_HZJPsiGamma/edCheckBias.sh .\
[sfonseca@lxplus016 bias_combine]$ ls\
background_pdfs.root\
\cf4 BiasStudy_Toy_Datacard.txt\cf2 \
\cf4 edCheckBias.sh\cf2 \
HToJPsiPhotonSignalAndBackgroundFit_workspace_Cat0.root\
\cf4 plotBias.py\cf2 \
wspaceread.C\
[sfonseca@lxplus016 bias_combine]$ vi edCheckBias.sh \
\
combine card_125VBF_nominal.root \cf5 -m\cf2  \cf6 125\cf2  \cf5 -M\cf2  MaxLikelihoodFit \cf5 -t\cf2  \cf6 100\cf2  \cf5 --toysFile\cf2  higgsCombineRealToysPdf4Signal1.GenerateOnly.mH125.123456.root \cf5 --setPhysicsModelParameters\cf2  \cf7 pdf_index\cf2 =\cf6 0\cf2  \cf5 --freezeNuisances\cf2  pdf_index \cf5 -n\cf2  RealToysPdf4Signal1FitWithPdf0 \cf5 --rMin=0\cf2  \cf5 --rMax=10\cf2  \cf5 -v\cf2  \cf6 3\cf2  \cf5 --muTrue=1\cf2 \
combine card_125VBF_nominal.root \cf5 -m\cf2  \cf6 125\cf2  \cf5 -M\cf2  MaxLikelihoodFit \cf5 -t\cf2  \cf6 100\cf2  \cf5 --toysFile\cf2  higgsCombineRealToysPdf4Signal10.GenerateOnly.mH125.123456.root \cf5 --setPhysicsModelParameters\cf2  \cf7 pdf_index\cf2 =\cf6 0\cf2  \cf5 --freezeNuisances\cf2  pdf_index \cf5 -n\cf2  RealToysPdf4Signal10FitWithPdf0 \cf5 --rMin=0\cf2  \cf5 --rMax=20\cf2  \cf5 -v\cf2  \cf6 3\cf2  \cf5 --muTrue=10\cf2 \
combine card_125VBF_nominal.root \cf5 -m\cf2  \cf6 125\cf2  \cf5 -M\cf2  MaxLikelihoodFit \cf5 -t\cf2  \cf6 100\cf2  \cf5 --toysFile\cf2  higgsCombineRealToysPdf0Signal0.GenerateOnly.mH125.123456.root \cf5 --setPhysicsModelParameters\cf2  \cf7 pdf_index\cf2 =\cf6 4\cf2  \cf5 --freezeNuisances\cf2  pdf_index \cf5 -n\cf2  RealToysPdf0Signal0FitWithPdf4 \cf5 --rMin=0\cf2  \cf5 --rMax=10\cf2  \cf5 -v\cf2  \cf6 3\cf2 \
combine card_125VBF_nominal.root \cf5 -m\cf2  \cf6 125\cf2  \cf5 -M\cf2  MaxLikelihoodFit \cf5 -t\cf2  \cf6 100\cf2  \cf5 --toysFile\cf2  higgsCombineRealToysPdf0Signal1.GenerateOnly.mH125.123456.root \cf5 --setPhysicsModelParameters\cf2  \cf7 pdf_index\cf2 =\cf6 4\cf2  \cf5 --freezeNuisances\cf2  pdf_index \cf5 -n\cf2  RealToysPdf0Signal1FitWithPdf4 \cf5 --rMin=0\cf2  \cf5 --rMax=10\cf2  \cf5 -v\cf2  \cf6 3\cf2  \cf5 --muTrue=1\cf2 \
combine card_125VBF_nominal.root \cf5 -m\cf2  \cf6 125\cf2  \cf5 -M\cf2  MaxLikelihoodFit \cf5 -t\cf2  \cf6 100\cf2  \cf5 --toysFile\cf2  higgsCombineRealToysPdf0Signal10.GenerateOnly.mH125.123456.root \cf5 --setPhysicsModelParameters\cf2  \cf7 pdf_index\cf2 =\cf6 4\cf2  \cf5 --freezeNuisances\cf2  pdf_index \cf5 -n\cf2  RealToysPdf0Signal10FitWithPdf4 \cf5 --rMin=0\cf2  \cf5 --rMax=20\cf2  \cf5 -v\cf2  \cf6 3\cf2  \cf5 --muTrue=10}