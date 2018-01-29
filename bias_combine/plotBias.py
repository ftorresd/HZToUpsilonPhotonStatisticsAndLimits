#!/usr/bin/env python

# Usual reasons to love python imports
import os
import sys
import ROOT as r
from math import sqrt,floor
from numpy import median

r.gROOT.SetBatch(True)
#fit with true
theFile = r.TFile('mlfitHiggsToJpsiToyFits2.root')
#theFile = r.TFile('mlfitRealToysPdf4Signal1Fit.root')
#theFile = r.TFile('mlfitRealToysPdf4Signal10Fit.root')
#theFile = r.TFile('mlfitRealToysPdf0Signal0Fit.root')
#theFile = r.TFile('mlfitRealToysPdf0Signal1Fit.root')
#theFile = r.TFile('mlfitRealToysPdf0Signal10Fit.root')
#fit with other
#theFile = r.TFile('mlfitRealToysPdf4Signal0FitWithPdf0.root')
#theFile = r.TFile('mlfitRealToysPdf4Signal1FitWithPdf0.root')
#theFile = r.TFile('mlfitRealToysPdf4Signal10FitWithPdf0.root')
#theFile = r.TFile('mlfitRealToysPdf0Signal0FitWithPdf4.root')
#theFile = r.TFile('mlfitRealToysPdf0Signal1FitWithPdf4.root')
#theFile = r.TFile('mlfitRealToysPdf0Signal10FitWithPdf4.root')

sbTree  = theFile.Get('tree_fit_sb')
bTree   = theFile.Get('tree_fit_b')

biasHist = r.TH1F('biasHist','biasHist',20,1,0)
signedBiasHist = r.TH1F('signedBiasHist','signedBiasHist',20,1,0)
muHist = r.TH1F('muHist','muHist',20,1,0)
muEntries= []
biasEntries = []
signedBiasEntries = []
theCoverage = 0

nEntries = sbTree.GetEntries()
print 'nEntries in tree = %g'%nEntries
for i,entry in enumerate(sbTree):
  sbTree.GetEntry(i)
  bTree.GetEntry(i)
  mu       = getattr(sbTree,'mu')
  muTrue   = getattr(bTree,'mu')
  deltaNll = getattr(sbTree,'nll_nll0')
  if deltaNll < 0.:
    theBias = sqrt(-2.*deltaNll)
  elif abs(deltaNll) < 0.01:
    theBias = 0.
  else:
    continue
  signedBias = 0.
  if mu!=muTrue:
    signedBias = theBias * (mu-muTrue)/abs(mu-muTrue)
  muHist.Fill(mu)
  muEntries.append(mu)
  biasHist.Fill(theBias)
  biasEntries.append(theBias)
  signedBiasHist.Fill(signedBias)
  signedBiasEntries.append(signedBias)
  if theBias > 1:
    theCoverage+=1
  print 'mu, deltaNll, bias, signedBias:   %1.3f, %1.3f, %1.3f, %1.3f'%(mu, deltaNll, theBias, signedBias)

nHistEntries = biasHist.GetEntries()
print 'nEntries in hists = %g'%nHistEntries
print 'mean, median of mu is = %1.3f, %1.3f'%(muHist.GetMean(), median(muEntries))
theMedian    = median(biasEntries)
signedMedian = median(signedBiasEntries)
print 'median, signedMedian = %1.3f, %1.3f'%(theMedian,signedMedian)
theCoverage = 1 - float(theCoverage)/float(nHistEntries)
print 'coverage is %1.3f'%theCoverage

canv = r.TCanvas()
canv.cd()
biasHist.Draw()
canv.Print('Hists/biasHist_%s.pdf'%theFile.GetName().split('mlfit')[1].split('.root')[0])
canv.Print('Hists/biasHist_%s.png'%theFile.GetName().split('mlfit')[1].split('.root')[0])
signedBiasHist.Draw()
canv.Print('Hists/signedBiasHist_%s.pdf'%theFile.GetName().split('mlfit')[1].split('.root')[0])
canv.Print('Hists/signedBiasHist_%s.png'%theFile.GetName().split('mlfit')[1].split('.root')[0])
muHist.Draw()
canv.Print('Hists/muHist_%s.pdf'%theFile.GetName().split('mlfit')[1].split('.root')[0])
canv.Print('Hists/muHist_%s.png'%theFile.GetName().split('mlfit')[1].split('.root')[0])
