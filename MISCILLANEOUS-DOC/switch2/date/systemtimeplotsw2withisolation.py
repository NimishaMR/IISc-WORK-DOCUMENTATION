#!./venv/bin/python3
import datetime as dt
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import math

#columns=['sytem_time_raw']
df=pd.read_csv('/home/zenlab/Documents/MicroC_programs/nimisha_tests/timesw2withisolation.csv',header=None,names=['systemtime'],)
#build index as another column for plotting
df['Time'] = df.index

df['hours']=pd.to_datetime(df['systemtime']).dt.hour
df['minutes']=pd.to_datetime(df['systemtime']).dt.minute
df['seconds']=pd.to_datetime(df['systemtime']).dt.second

df['systemtime'] = df['systemtime'].str.replace(":","",regex=True)
df['systemtime'] = df['systemtime'].str.replace(".","",regex=True)
df['systemtime'] = pd.to_numeric(df['systemtime'])

df['differencebwsuccesive']=df['systemtime'].diff()
df['differencebwsuccesivecorrect']=df['differencebwsuccesive']*(math.pow(10,-9))

print(df)
df.plot.scatter(x="Time",y="differencebwsuccesivecorrect",ylim=(1.0000000,1.0060000))
plt.show()