#!/usr/bin/env python

import os

os.system("rm -rf outputWorkspaces")
os.system("mkdir outputWorkspaces")

def execString(b, s, c):
	toExecString  = ""
	toExecString += "cd outputWorkspaces/workspace_"+b+"ToUpsilon"+s+"SPhoton_Cat"+c+"\n"
	toExecString += "combineTool.py -M Impacts -d workspace_"+b+"ToUpsilon"+s+"SPhoton_Cat"+c+".root -m 125 --doInitialFit\n"
	toExecString += "combineTool.py -M Impacts -d workspace_"+b+"ToUpsilon"+s+"SPhoton_Cat"+c+".root -m 125 --doFits --parallel 4\n"
	toExecString += "combineTool.py -M Impacts -d workspace_"+b+"ToUpsilon"+s+"SPhoton_Cat"+c+".root -m 125 -o impacts.json\n"
	toExecString += "plotImpacts.py -i impacts.json -o impacts\n"
	toExecString += "mv impacts.pdf impacts_"+b+"ToUpsilon"+s+"SPhoton_Cat"+c+".pdf"
	os.system(toExecString)


boson = ["Z", "H"]
upsilonState = ["1", "2", "3"]  
category = ["0", "123"]  

for b in boson:
	for s in upsilonState:
		for c in category:
			if (b == "H"):
				if (c == "0"):
					os.system("mkdir -p outputWorkspaces/workspace_"+b+"ToUpsilon"+s+"SPhoton_Cat"+c)
					os.system("text2workspace.py outputDatacards2D/datacard_"+b+"ToUpsilon"+s+"SPhoton_Cat"+c+".txt -m 125 -o outputWorkspaces/workspace_"+b+"ToUpsilon"+s+"SPhoton_Cat"+c+"/workspace_"+b+"ToUpsilon"+s+"SPhoton_Cat"+c+".root")
			else:
				os.system("mkdir -p outputWorkspaces/workspace_"+b+"ToUpsilon"+s+"SPhoton_Cat"+c)
				os.system("text2workspace.py outputDatacards2D/datacard_"+b+"ToUpsilon"+s+"SPhoton_Cat"+c+".txt -m 125 -o outputWorkspaces/workspace_"+b+"ToUpsilon"+s+"SPhoton_Cat"+c+"/workspace_"+b+"ToUpsilon"+s+"SPhoton_Cat"+c+".root")



for b in boson:
	for s in upsilonState:
		for c in category:
			if (b == "H"):
				if (c == "0"):
					execString(b, s, c)
			else:
				execString(b, s, c)







