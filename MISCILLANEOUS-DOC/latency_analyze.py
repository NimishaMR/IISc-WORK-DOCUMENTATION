#!/home/zenlab/Documents/venv/bin/python3

import sys
import datetime
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns



dftx = pd.read_csv("/tmp/TXv1.csv")
dfrx = pd.read_csv("/tmp/RXv1.csv")

if dftx['ip.id']==dfrx['ip.id]']:
    print('same')
else :
    print('different') 