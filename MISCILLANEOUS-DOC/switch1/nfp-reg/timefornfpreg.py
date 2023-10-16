#!./venv/bin/python3
import datetime as dt
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import math

columns=['timevalue']
df=pd.read_csv('/home/zenlab/Documents/MicroC_programs/nimisha_tests/switch1/nfpreg command/timenfpregwithoutcoreiso.csv',header=None,names=columns)
df['timevalue'] = df['timevalue'].str.replace('real', '')
df['timevalue'] = df['timevalue'].str.replace('\t', '')
df['timevalue'] = df['timevalue'].str.replace('0m0.', '')
df['timevalue'] = df['timevalue'].str.replace('s', '')
df["timevalue"] = pd.to_numeric(df["timevalue"])

#build index as another column for plotting
df['Time'] = df.index

print(df)

df.plot.scatter(x="Time",y="timevalue",ylim=(10,46))
plt.show()
