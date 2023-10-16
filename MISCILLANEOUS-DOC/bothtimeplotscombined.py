#!./venv/bin/python3

import datetime as dt
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import math

'''CODE FOR SW1'''

#columns=['sytem_time_raw']
df1=pd.read_csv('/home/zenlab/Documents/MicroC_programs/nimisha_tests/timesw1.csv',header=None,names=['systemtime'],)
#build index as another column for plotting
df1['Time'] = df1.index



df1['hours']=pd.to_datetime(df1['systemtime']).dt.hour
df1['minutes']=pd.to_datetime(df1['systemtime']).dt.minute
df1['seconds']=pd.to_datetime(df1['systemtime']).dt.second

df1['systemtime'] = df1['systemtime'].str.replace(":","",regex=True)
df1['systemtime'] = df1['systemtime'].str.replace(".","",regex=True)
df1['systemtime'] = pd.to_numeric(df1['systemtime'])

df1['differencebwsuccesive']=df1['systemtime'].diff()
df1['differencebwsuccesivecorrect']=df1['differencebwsuccesive']*(math.pow(10,-9))


print(df1)
df1.plot.scatter(x="Time",y="differencebwsuccesivecorrect",ylim=(1.0000000,1.0060000))



'''CODE FOR SW2'''

#columns=['sytem_time_raw']
df2=pd.read_csv('/home/zenlab/Documents/MicroC_programs/nimisha_tests/timesw2.csv',header=None,names=['systemtime'],)
#build index as another column for plotting
df2['Time'] = df2.index

df2['hours']=pd.to_datetime(df2['systemtime']).dt.hour
df2['minutes']=pd.to_datetime(df2['systemtime']).dt.minute
df2['seconds']=pd.to_datetime(df2['systemtime']).dt.second

df2['systemtime'] = df2['systemtime'].str.replace(":","",regex=True)
df2['systemtime'] = df2['systemtime'].str.replace(".","",regex=True)
df2['systemtime'] = pd.to_numeric(df2['systemtime'])

df2['differencebwsuccesive']=df2['systemtime'].diff()
df2['differencebwsuccesivecorrect']=df2['differencebwsuccesive']*(math.pow(10,-9))

print(df2)
df2.plot.scatter(x="Time",y="differencebwsuccesivecorrect",ylim=(1.0000000,1.0060000))



'''CODE TO COMBINE TWO DATAFRAMES TO CREATE THIRD DATAFRAME'''
df1['combineddifference']=df2['differencebwsuccesivecorrect']-df1['differencebwsuccesivecorrect']
df1.plot.scatter(x="Time",y="combineddifference",ylim=(0.000000,0.006000))

# df1['combineddifference2']=df2['systemtime']-df1['systemtime']
# df1.plot.scatter(x="Time",y="combineddifference2")
# #ylim=(0.000000,0.006000)
plt.show()