#!/usr/bin/env python

import json
import StringIO 
import os
from decimal import Decimal



if __name__ == "__main__":
	# SM values
    # REFs: https://docs.google.com/spreadsheets/d/1zP8P9kp-yFrkMu9bGt4fpIirKAKYlH-w2em_kVkYYKw/edit#gid=1026998768
	# Page: 2 
	xSecSM = {}
	xSecSM["ZToJPsiGamma_BR"] = 8.9600E-08
	xSecSM["ZToUpsilon1SPhoton_BR"] = 4.8000E-08
	xSecSM["ZToUpsilon2SPhoton_BR"] = 2.4400E-08
	xSecSM["ZToUpsilon3SPhoton_BR"] = 1.8800E-08

	xSecSM["HToJPsiGamma_BR"] = 2.9900E-06
	xSecSM["HToUpsilon1SPhoton_BR"] = 5.2200E-09
	xSecSM["HToUpsilon2SPhoton_BR"] = 1.4200E-09
	xSecSM["HToUpsilon3SPhoton_BR"] = 9.1000E-10

	xSecSM["ppToZ_xSec"] = 5.7095E+04
	xSecSM["ppToH_xSec"] = 5.5614E+01

	xSecSM["JPsiToMuMu_BR"] = 5.9610E-02
	xSecSM["Upsilon1SToMuMu_BR"] = 2.4800E-02
	xSecSM["Upsilon2SToMuMu_BR"] = 1.9300E-02
	xSecSM["Upsilon3SToMuMu_BR"] = 2.1800E-02

	# BRLimits = json.load(open("outputLimits_v03/BRLimits.json"))
	BRLimits = json.load(open("outputLimits2D/BRLimits.json"))

	# Only Upsilon Table 
# 	latexBRLimits = """
# \\begin{tabular}{l|llll}
# \\multicolumn{5}{c}{95\% C.L. Upper Limit} \\\\
# \\hline
# \\hline
# & \\multicolumn{4}{c}{$\mathcal{B}(Z \\rightarrow \Upsilon\gamma)$ $[\\times10^{-6}]$}      \\\\
# \cline{2-5}
# &  $\Upsilon(1S)$ & $\Upsilon(2S)$ & $\Upsilon(3S)$ & $ \sum_n \Upsilon(nS)$  \\\\
# \hline
# Expected     & $@@Z_1S_EXPC@@^{+@@Z_1S_EXPC_PLUS@@}_{-@@Z_1S_EXPC_MINUS@@}$ &  $@@Z_2S_EXPC@@^{+@@Z_2S_EXPC_PLUS@@}_{-@@Z_2S_EXPC_MINUS@@}$  & $@@Z_3S_EXPC@@^{+@@Z_3S_EXPC_PLUS@@}_{-@@Z_3S_EXPC_MINUS@@}$  & $@@Z_NS_EXPC@@^{+@@Z_NS_EXPC_PLUS@@}_{-@@Z_NS_EXPC_MINUS@@}$             \\\\
# Observed     & @@Z_1S_OBS@@ &  @@Z_2S_OBS@@  & @@Z_3S_OBS@@  & @@Z_NS_OBS@@     \\\\
# \hline
# SM Prediction $[\\times10^{-8}]$ & @@Z_1S_SM@@  &  @@Z_2S_SM@@  & @@Z_3S_SM@@  & -      \\\\
# \\hline
# \\hline
# & \multicolumn{4}{c}{$\mathcal{B}(H \\rightarrow \Upsilon\gamma)$ $[\\times10^{-4}]$}       \\\\
# \\cline{2-5}
# &  $\Upsilon(1S)$ & $\Upsilon(2S)$ & $\Upsilon(3S)$ & $ \sum_n \Upsilon(nS)$  \\\\
# \hline
# Expected     & $@@H_1S_EXPC@@^{+@@H_1S_EXPC_PLUS@@}_{-@@H_1S_EXPC_MINUS@@}$ &  $@@H_2S_EXPC@@^{+@@H_2S_EXPC_PLUS@@}_{-@@H_2S_EXPC_MINUS@@}$  & $@@H_3S_EXPC@@^{+@@H_3S_EXPC_PLUS@@}_{-@@H_3S_EXPC_MINUS@@}$  & $@@H_NS_EXPC@@^{+@@H_NS_EXPC_PLUS@@}_{-@@H_NS_EXPC_MINUS@@}$             \\\\
# Observed     & @@H_1S_OBS@@ &  @@H_2S_OBS@@  & @@H_3S_OBS@@  & @@H_NS_OBS@@      \\\\
# \hline
# SM Prediction $[\\times10^{-9}]$ & @@H_1S_SM@@  &  @@H_2S_SM@@  & @@H_3S_SM@@  & -      \\\\
# \\hline
# \\hline
# \\end{tabular}
# 	"""

	latexBRLimits = """
\\begin{tabular}{l|llll}
\\multicolumn{4}{c}{95\% C.L. Upper Limit} \\\\
\\hline
\\hline
& \\multicolumn{3}{c}{$\mathcal{B}(Z \\rightarrow \Upsilon\gamma)$ $[\\times10^{-6}]$}      \\\\
\cline{2-4}
&  $\Upsilon(1S)$ & $\Upsilon(2S)$ & $\Upsilon(3S)$  \\\\
\hline
Expected     & $@@Z_1S_EXPC@@^{+@@Z_1S_EXPC_PLUS@@}_{-@@Z_1S_EXPC_MINUS@@}$ &  $@@Z_2S_EXPC@@^{+@@Z_2S_EXPC_PLUS@@}_{-@@Z_2S_EXPC_MINUS@@}$  & $@@Z_3S_EXPC@@^{+@@Z_3S_EXPC_PLUS@@}_{-@@Z_3S_EXPC_MINUS@@}$            \\\\
Observed     & @@Z_1S_OBS@@ &  @@Z_2S_OBS@@  & @@Z_3S_OBS@@      \\\\
\hline
SM Prediction $[\\times10^{-8}]$ & @@Z_1S_SM@@  &  @@Z_2S_SM@@  & @@Z_3S_SM@@      \\\\
\\hline
\\hline
& \multicolumn{3}{c}{$\mathcal{B}(H \\rightarrow \Upsilon\gamma)$ $[\\times10^{-4}]$}       \\\\
\\cline{2-4}
&  $\Upsilon(1S)$ & $\Upsilon(2S)$ & $\Upsilon(3S)$ &   \\\\
\hline
Expected     & $@@H_1S_EXPC@@^{+@@H_1S_EXPC_PLUS@@}_{-@@H_1S_EXPC_MINUS@@}$ &  $@@H_2S_EXPC@@^{+@@H_2S_EXPC_PLUS@@}_{-@@H_2S_EXPC_MINUS@@}$  & $@@H_3S_EXPC@@^{+@@H_3S_EXPC_PLUS@@}_{-@@H_3S_EXPC_MINUS@@}$        \\\\
Observed     & @@H_1S_OBS@@ &  @@H_2S_OBS@@  & @@H_3S_OBS@@     \\\\
\hline
SM Prediction $[\\times10^{-9}]$ & @@H_1S_SM@@  &  @@H_2S_SM@@  & @@H_3S_SM@@      \\\\
\\hline
\\hline
\\end{tabular}
	"""

	# Z
	latexBRLimits = latexBRLimits.replace("@@Z_1S_SM@@", str(round(xSecSM["ZToUpsilon1SPhoton_BR"]/1E-8, 1)))
	latexBRLimits = latexBRLimits.replace("@@Z_2S_SM@@", str(round(xSecSM["ZToUpsilon2SPhoton_BR"]/1E-8, 1)))
	latexBRLimits = latexBRLimits.replace("@@Z_3S_SM@@", str(round(xSecSM["ZToUpsilon3SPhoton_BR"]/1E-8, 1)))

	latexBRLimits = latexBRLimits.replace("@@Z_1S_OBS@@", str(round(BRLimits["ZToUpsilon1SPhoton"]["observedLimit"]/1E-6, 1)))
	latexBRLimits = latexBRLimits.replace("@@Z_2S_OBS@@", str(round(BRLimits["ZToUpsilon2SPhoton"]["observedLimit"]/1E-6, 1)))
	latexBRLimits = latexBRLimits.replace("@@Z_3S_OBS@@", str(round(BRLimits["ZToUpsilon3SPhoton"]["observedLimit"]/1E-6, 1)))
	latexBRLimits = latexBRLimits.replace("@@Z_NS_OBS@@", str(round(BRLimits["ZToUpsilonPhoton"]["observedLimit"]/1E-6, 1)))

	latexBRLimits = latexBRLimits.replace("@@Z_1S_EXPC@@", str(round(BRLimits["ZToUpsilon1SPhoton"]["expectedLimit"]/1E-6, 1)))
	latexBRLimits = latexBRLimits.replace("@@Z_2S_EXPC@@", str(round(BRLimits["ZToUpsilon2SPhoton"]["expectedLimit"]/1E-6, 1)))
	latexBRLimits = latexBRLimits.replace("@@Z_3S_EXPC@@", str(round(BRLimits["ZToUpsilon3SPhoton"]["expectedLimit"]/1E-6, 1)))
	latexBRLimits = latexBRLimits.replace("@@Z_NS_EXPC@@", str(round(BRLimits["ZToUpsilonPhoton"]["expectedLimit"]/1E-6, 1)))

	latexBRLimits = latexBRLimits.replace("@@Z_1S_EXPC_PLUS@@", str(round((BRLimits["ZToUpsilon1SPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["ZToUpsilon1SPhoton"]["expectedLimit"])/1E-6, 1)))
	latexBRLimits = latexBRLimits.replace("@@Z_2S_EXPC_PLUS@@", str(round((BRLimits["ZToUpsilon2SPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["ZToUpsilon2SPhoton"]["expectedLimit"])/1E-6, 1)))
	latexBRLimits = latexBRLimits.replace("@@Z_3S_EXPC_PLUS@@", str(round((BRLimits["ZToUpsilon3SPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["ZToUpsilon3SPhoton"]["expectedLimit"])/1E-6, 1)))
	latexBRLimits = latexBRLimits.replace("@@Z_NS_EXPC_PLUS@@", str(round((BRLimits["ZToUpsilonPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["ZToUpsilonPhoton"]["expectedLimit"])/1E-6, 1)))

	latexBRLimits = latexBRLimits.replace("@@Z_1S_EXPC_MINUS@@", str(round(abs((BRLimits["ZToUpsilon1SPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["ZToUpsilon1SPhoton"]["expectedLimit"]))/1E-6, 1)))
	latexBRLimits = latexBRLimits.replace("@@Z_2S_EXPC_MINUS@@", str(round(abs((BRLimits["ZToUpsilon2SPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["ZToUpsilon2SPhoton"]["expectedLimit"]))/1E-6, 1)))
	latexBRLimits = latexBRLimits.replace("@@Z_3S_EXPC_MINUS@@", str(round(abs((BRLimits["ZToUpsilon3SPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["ZToUpsilon3SPhoton"]["expectedLimit"]))/1E-6, 1)))
	latexBRLimits = latexBRLimits.replace("@@Z_NS_EXPC_MINUS@@", str(round(abs((BRLimits["ZToUpsilonPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["ZToUpsilonPhoton"]["expectedLimit"]))/1E-6, 1)))


	# Higgs
	latexBRLimits = latexBRLimits.replace("@@H_1S_SM@@", str(round(xSecSM["HToUpsilon1SPhoton_BR"]/1E-9, 1)))
	latexBRLimits = latexBRLimits.replace("@@H_2S_SM@@", str(round(xSecSM["HToUpsilon2SPhoton_BR"]/1E-9, 1)))
	latexBRLimits = latexBRLimits.replace("@@H_3S_SM@@", str(round(xSecSM["HToUpsilon3SPhoton_BR"]/1E-9, 1)))

	latexBRLimits = latexBRLimits.replace("@@H_1S_OBS@@", str(round(BRLimits["HToUpsilon1SPhoton"]["observedLimit"]/1E-4, 1)))
	latexBRLimits = latexBRLimits.replace("@@H_2S_OBS@@", str(round(BRLimits["HToUpsilon2SPhoton"]["observedLimit"]/1E-4, 1)))
	latexBRLimits = latexBRLimits.replace("@@H_3S_OBS@@", str(round(BRLimits["HToUpsilon3SPhoton"]["observedLimit"]/1E-4, 1)))
	latexBRLimits = latexBRLimits.replace("@@H_NS_OBS@@", str(round(BRLimits["HToUpsilonPhoton"]["observedLimit"]/1E-4, 1)))

	latexBRLimits = latexBRLimits.replace("@@H_1S_EXPC@@", str(round(BRLimits["HToUpsilon1SPhoton"]["expectedLimit"]/1E-4, 1)))
	latexBRLimits = latexBRLimits.replace("@@H_2S_EXPC@@", str(round(BRLimits["HToUpsilon2SPhoton"]["expectedLimit"]/1E-4, 1)))
	latexBRLimits = latexBRLimits.replace("@@H_3S_EXPC@@", str(round(BRLimits["HToUpsilon3SPhoton"]["expectedLimit"]/1E-4, 1)))
	latexBRLimits = latexBRLimits.replace("@@H_NS_EXPC@@", str(round(BRLimits["HToUpsilonPhoton"]["expectedLimit"]/1E-4, 1)))

	latexBRLimits = latexBRLimits.replace("@@H_1S_EXPC_PLUS@@", str(round((BRLimits["HToUpsilon1SPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["HToUpsilon1SPhoton"]["expectedLimit"])/1E-4, 1)))
	latexBRLimits = latexBRLimits.replace("@@H_2S_EXPC_PLUS@@", str(round((BRLimits["HToUpsilon2SPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["HToUpsilon2SPhoton"]["expectedLimit"])/1E-4, 1)))
	latexBRLimits = latexBRLimits.replace("@@H_3S_EXPC_PLUS@@", str(round((BRLimits["HToUpsilon3SPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["HToUpsilon3SPhoton"]["expectedLimit"])/1E-4, 1)))
	latexBRLimits = latexBRLimits.replace("@@H_NS_EXPC_PLUS@@", str(round((BRLimits["HToUpsilonPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["HToUpsilonPhoton"]["expectedLimit"])/1E-4, 1)))

	latexBRLimits = latexBRLimits.replace("@@H_1S_EXPC_MINUS@@", str(round(abs((BRLimits["HToUpsilon1SPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["HToUpsilon1SPhoton"]["expectedLimit"]))/1E-4, 1)))
	latexBRLimits = latexBRLimits.replace("@@H_2S_EXPC_MINUS@@", str(round(abs((BRLimits["HToUpsilon2SPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["HToUpsilon2SPhoton"]["expectedLimit"]))/1E-4, 1)))
	latexBRLimits = latexBRLimits.replace("@@H_3S_EXPC_MINUS@@", str(round(abs((BRLimits["HToUpsilon3SPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["HToUpsilon3SPhoton"]["expectedLimit"]))/1E-4, 1)))
	latexBRLimits = latexBRLimits.replace("@@H_NS_EXPC_MINUS@@", str(round(abs((BRLimits["HToUpsilonPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["HToUpsilonPhoton"]["expectedLimit"]))/1E-4, 1)))



	os.system("rm outputLimits2D/latexBRLimits.txt")
	with open("outputLimits2D/latexBRLimits.txt", 'w') as out_file:
		out_file.write(latexBRLimits)
	print "\n"+latexBRLimits+"\n"


# 	# Full Table 
# 	latexBRLimitsFull = """
# \\begin{tabular}{l|lllll}
# \\multicolumn{6}{c}{95\% C.L. Upper Limit} \\\\
# \\hline
# \\hline
# & \\multicolumn{5}{c}{$\mathcal{B}(Z \\rightarrow \mathcal{Q}\gamma)$ $[\\times10^{-6}]$}      \\\\
# \cline{2-6}
# &  $J/\psi$ &  $\Upsilon(1S)$ & $\Upsilon(2S)$ & $\Upsilon(3S)$ & $ \sum_n \Upsilon(nS)$  \\\\
# \hline
# Expected   & $@@Z_JPSI_EXPC@@^{+@@Z_JPSI_EXPC_PLUS@@}_{-@@Z_JPSI_EXPC_MINUS@@}$ & $@@Z_1S_EXPC@@^{+@@Z_1S_EXPC_PLUS@@}_{-@@Z_1S_EXPC_MINUS@@}$ &  $@@Z_2S_EXPC@@^{+@@Z_2S_EXPC_PLUS@@}_{-@@Z_2S_EXPC_MINUS@@}$  & $@@Z_3S_EXPC@@^{+@@Z_3S_EXPC_PLUS@@}_{-@@Z_3S_EXPC_MINUS@@}$  & $@@Z_NS_EXPC@@^{+@@Z_NS_EXPC_PLUS@@}_{-@@Z_NS_EXPC_MINUS@@}$            \\\\
# Observed   & @@Z_JPSI_OBS@@ & @@Z_1S_OBS@@ &  @@Z_2S_OBS@@  & @@Z_3S_OBS@@  & @@Z_NS_OBS@@      \\\\
# \hline
# SM Prediction $[\\times10^{-8}]$ &@@Z_JPSI_SM@@ & @@Z_1S_SM@@  &  @@Z_2S_SM@@  & @@Z_3S_SM@@  & -      \\\\
# \\hline
# \\hline
# & \multicolumn{5}{c}{$\mathcal{B}(H \\rightarrow \mathcal{Q}\gamma)$ $[\\times10^{-4}]$}       \\\\
# \\cline{2-6}
# & $J/\psi$ &$\Upsilon(1S)$ & $\Upsilon(2S)$ & $\Upsilon(3S)$ & $ \sum_n \Upsilon(nS)$  \\\\
# \hline
# Expected  &  $@@H_JPSI_EXPC@@^{+@@H_JPSI_EXPC_PLUS@@}_{-@@H_JPSI_EXPC_MINUS@@}$ & $@@H_1S_EXPC@@^{+@@H_1S_EXPC_PLUS@@}_{-@@H_1S_EXPC_MINUS@@}$ &  $@@H_2S_EXPC@@^{+@@H_2S_EXPC_PLUS@@}_{-@@H_2S_EXPC_MINUS@@}$  & $@@H_3S_EXPC@@^{+@@H_3S_EXPC_PLUS@@}_{-@@H_3S_EXPC_MINUS@@}$  & $@@H_NS_EXPC@@^{+@@H_NS_EXPC_PLUS@@}_{-@@H_NS_EXPC_MINUS@@}$             \\\\
# Observed  &  @@H_JPSI_OBS@@ & @@H_1S_OBS@@ &  @@H_2S_OBS@@  & @@H_3S_OBS@@  & @@H_NS_OBS@@      \\\\
# \hline
# SM Prediction $[\\times10^{-9}]$ & @@H_JPSI_SM@@ & @@H_1S_SM@@  &  @@H_2S_SM@@  & @@H_3S_SM@@  & -      \\\\
# \\hline
# \\hline
# \\end{tabular}
# 	"""

# 	# Z
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_JPSI_SM@@", str(round(xSecSM["ZToJPsiGamma_BR"]/1E-8, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_1S_SM@@", str(round(xSecSM["ZToUpsilon1SPhoton_BR"]/1E-8, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_2S_SM@@", str(round(xSecSM["ZToUpsilon2SPhoton_BR"]/1E-8, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_3S_SM@@", str(round(xSecSM["ZToUpsilon3SPhoton_BR"]/1E-8, 1)))

# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_JPSI_OBS@@", str(round(BRLimits["ZToJPsiPhoton"]["observedLimit"]/1E-6, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_1S_OBS@@", str(round(BRLimits["ZToUpsilon1SPhoton"]["observedLimit"]/1E-6, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_2S_OBS@@", str(round(BRLimits["ZToUpsilon2SPhoton"]["observedLimit"]/1E-6, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_3S_OBS@@", str(round(BRLimits["ZToUpsilon3SPhoton"]["observedLimit"]/1E-6, 1)))

# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_JPSI_EXPC@@", str(round(BRLimits["ZToJPsiPhoton"]["expectedLimit"]/1E-6, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_1S_EXPC@@", str(round(BRLimits["ZToUpsilon1SPhoton"]["expectedLimit"]/1E-6, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_2S_EXPC@@", str(round(BRLimits["ZToUpsilon2SPhoton"]["expectedLimit"]/1E-6, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_3S_EXPC@@", str(round(BRLimits["ZToUpsilon3SPhoton"]["expectedLimit"]/1E-6, 1)))

# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_JPSI_EXPC_PLUS@@", str(round((BRLimits["ZToJPsiPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["ZToJPsiPhoton"]["expectedLimit"])/1E-6, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_1S_EXPC_PLUS@@", str(round((BRLimits["ZToUpsilon1SPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["ZToUpsilon1SPhoton"]["expectedLimit"])/1E-6, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_2S_EXPC_PLUS@@", str(round((BRLimits["ZToUpsilon2SPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["ZToUpsilon2SPhoton"]["expectedLimit"])/1E-6, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_3S_EXPC_PLUS@@", str(round((BRLimits["ZToUpsilon3SPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["ZToUpsilon3SPhoton"]["expectedLimit"])/1E-6, 1)))

# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_JPSI_EXPC_MINUS@@", str(round(abs((BRLimits["ZToJPsiPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["ZToJPsiPhoton"]["expectedLimit"]))/1E-6, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_1S_EXPC_MINUS@@", str(round(abs((BRLimits["ZToUpsilon1SPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["ZToUpsilon1SPhoton"]["expectedLimit"]))/1E-6, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_2S_EXPC_MINUS@@", str(round(abs((BRLimits["ZToUpsilon2SPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["ZToUpsilon2SPhoton"]["expectedLimit"]))/1E-6, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_3S_EXPC_MINUS@@", str(round(abs((BRLimits["ZToUpsilon3SPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["ZToUpsilon3SPhoton"]["expectedLimit"]))/1E-6, 1)))


# 	# Higgs
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_JPSI_SM@@", str(round(xSecSM["HToJPsiGamma_BR"]/1E-9, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_1S_SM@@", str(round(xSecSM["HToUpsilon1SPhoton_BR"]/1E-9, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_2S_SM@@", str(round(xSecSM["HToUpsilon2SPhoton_BR"]/1E-9, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_3S_SM@@", str(round(xSecSM["HToUpsilon3SPhoton_BR"]/1E-9, 1)))

# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_JPSI_OBS@@", str(round(BRLimits["HToJPsiPhoton"]["observedLimit"]/1E-4, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_1S_OBS@@", str(round(BRLimits["HToUpsilon1SPhoton"]["observedLimit"]/1E-4, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_2S_OBS@@", str(round(BRLimits["HToUpsilon2SPhoton"]["observedLimit"]/1E-4, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_3S_OBS@@", str(round(BRLimits["HToUpsilon3SPhoton"]["observedLimit"]/1E-4, 1)))

# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_JPSI_EXPC@@", str(round(BRLimits["HToJPsiPhoton"]["expectedLimit"]/1E-4, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_1S_EXPC@@", str(round(BRLimits["HToUpsilon1SPhoton"]["expectedLimit"]/1E-4, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_2S_EXPC@@", str(round(BRLimits["HToUpsilon2SPhoton"]["expectedLimit"]/1E-4, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_3S_EXPC@@", str(round(BRLimits["HToUpsilon3SPhoton"]["expectedLimit"]/1E-4, 1)))

# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_JPSI_EXPC_PLUS@@", str(round((BRLimits["HToJPsiPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["HToJPsiPhoton"]["expectedLimit"])/1E-4, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_1S_EXPC_PLUS@@", str(round((BRLimits["HToUpsilon1SPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["HToUpsilon1SPhoton"]["expectedLimit"])/1E-4, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_2S_EXPC_PLUS@@", str(round((BRLimits["HToUpsilon2SPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["HToUpsilon2SPhoton"]["expectedLimit"])/1E-4, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_3S_EXPC_PLUS@@", str(round((BRLimits["HToUpsilon3SPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["HToUpsilon3SPhoton"]["expectedLimit"])/1E-4, 1)))

# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_JPSI_EXPC_MINUS@@", str(round(abs((BRLimits["HToJPsiPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["HToJPsiPhoton"]["expectedLimit"]))/1E-4, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_1S_EXPC_MINUS@@", str(round(abs((BRLimits["HToUpsilon1SPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["HToUpsilon1SPhoton"]["expectedLimit"]))/1E-4, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_2S_EXPC_MINUS@@", str(round(abs((BRLimits["HToUpsilon2SPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["HToUpsilon2SPhoton"]["expectedLimit"]))/1E-4, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_3S_EXPC_MINUS@@", str(round(abs((BRLimits["HToUpsilon3SPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["HToUpsilon3SPhoton"]["expectedLimit"]))/1E-4, 1)))


# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_NS_OBS@@", str(round(BRLimits["ZToUpsilonPhoton"]["observedLimit"]/1E-6, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_NS_EXPC@@", str(round(BRLimits["ZToUpsilonPhoton"]["expectedLimit"]/1E-6, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_NS_EXPC_PLUS@@", str(round((BRLimits["ZToUpsilonPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["ZToUpsilonPhoton"]["expectedLimit"])/1E-6, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@Z_NS_EXPC_MINUS@@", str(round(abs((BRLimits["ZToUpsilonPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["ZToUpsilonPhoton"]["expectedLimit"]))/1E-6, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_NS_OBS@@", str(round(BRLimits["HToUpsilonPhoton"]["observedLimit"]/1E-4, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_NS_EXPC@@", str(round(BRLimits["HToUpsilonPhoton"]["expectedLimit"]/1E-4, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_NS_EXPC_PLUS@@", str(round((BRLimits["HToUpsilonPhoton"]["expectedLimitPlus1Sigma"] - BRLimits["HToUpsilonPhoton"]["expectedLimit"])/1E-4, 1)))
# 	latexBRLimitsFull = latexBRLimitsFull.replace("@@H_NS_EXPC_MINUS@@", str(round(abs((BRLimits["HToUpsilonPhoton"]["expectedLimitMinus1Sigma"] - BRLimits["HToUpsilonPhoton"]["expectedLimit"]))/1E-4, 1)))


# 	os.system("rm outputLimits_v03/latexBRLimitsFull.txt")
# 	with open("outputLimits_v03/latexBRLimitsFull.txt", 'w') as out_file:
# 		out_file.write(latexBRLimitsFull)
# 	print "\n\n\n"
# 	print "#"*30
# 	print "\n\n"+latexBRLimitsFull+"\n"







