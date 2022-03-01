################################################
# https://www.finlab.tw/Python%E6%96%B0%E6%89%8B%E6%95%99%E5%AD%B84%E7%9B%B8%E9%97%9C%E6%80%A7%E5%88%86%E6%9E%90/
# reference above for detail
# 2019/8/8 CN 
import requests
import datetime
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
import io
import time
import seaborn as sns
import numpy as np

#plt.rcParams['figure.figsize']=(18,18)
################################################3
# 抓股票
def crawl_price(stock_id,fname):
    # after csv saved block below to save time
    #
    #now = int(datetime.datetime.now().timestamp())+86400
    #url = "https://query1.finance.yahoo.com/v7/finance/download/" + stock_id + "?period1=0&period2=" + str(now) + "&interval=1d&events=history&crumb=hP2rOschxO0"
    #
    #response = requests.post(url)
    
# data to system mem
#    f = io.StringIO(response.text)
#    df = pd.read_csv(f, index_col='Date', parse_dates=['Date'] )
# data to file
    filename= fname +'.csv'
    filename=filename.replace('/',' ')
    print(filename)
    
# after csv saved block below to save time    
#    with open(filename, 'w') as f: # after 'first' write to file you can block this to save time
#        f.writelines(response.text)      # after  write to file  you can block this to save time
#
#
    df = pd.read_csv(filename, index_col='Date', parse_dates=['Date'])

    return df
################################################
# get world stock info

url = "https://finance.yahoo.com/world-indices/"
response = requests.get(url)
f = io.StringIO(response.text)
dfs = pd.read_html(f)
world_index = dfs[0]

world_index_history = {}
for symbol, name in zip(world_index['Symbol'], world_index['Name']):
    
    print(name)
    
    world_index_history[name] = crawl_price(symbol,name)
    #time.sleep(5) # need delay or yahoo may block
#####################################################3
#
#復權收盤價
#它移除了除權息和增減資的價格波動，更準確的反映了「股票價值」的變化
#
adjclose = {}
for name, price in world_index_history.items():
    if price.index.is_unique:
        adjclose[name] = price['Adj Close']
    
adjclose = pd.DataFrame(adjclose)
adjclose

corr= adjclose.corr()
print(corr)

######################################################################
# plot all corr data
#sns.heatmap(corr, square=True ,vmax=1.0, linecolor='white', annot=True)


#################################3
# only for one stock info    
#site = "https://query1.finance.yahoo.com/v7/finance/download/2886.TW?period1=0&period2=1549258857&interval=1d&events=history&crumb=hP2rOschxO0"
#response = requests.post(site)
#print(response.text)
#
#with open('file.csv', 'w') as f:
#    f.writelines(response.text)
    ##df = pd.read_csv('file.csv')
#    df = pd.read_csv('file.csv', index_col='Date', parse_dates=['Date'])
#    df.head()
#df.Close.plot()

####################################
# select stock index and print
#df= crawl_price("^DJI")
#df.Close.plot()

#for name, history in world_index_history.items():
#    history.Close.plot()

####################################
# plot tw vs uk
#台灣指數TSEC weighted index跟英國前一百大Cboe UK 100超像！相關係數高達0.9
#adjclose['Cboe UK 100 Price Return']['2015':].plot()
#adjclose['TSEC weighted index']['2015':].plot()


####################################
#計算獲利
pct_change = adjclose.pct_change() #這個函示會計算今天漲了x％
profit = pct_change.mean() #將每一天的漲跌都平均起來
print("$$")
profit.sort_values() #由小到大的排序
print(profit.sort_values()) #由小到大的排序

####################################
#風險用標準差
risk = pct_change.std() #標準差
sharpe = profit / risk * (252 ** 0.5) #計算sharpe ratio
sharpe.sort_values()
print("sharpe ratio")
print(sharpe.sort_values())
####################################
#時間移動窗格，每日都計算252天以前的sharpe值
# before
profit = pct_change.mean()
risk = pct_change.std()
sharpe = profit / risk * (252 ** 0.5)

# after
profit = pct_change.rolling(252).mean() # 252 days
risk = pct_change.rolling(252).std()
sharpe = profit / risk * (252 ** 0.5)

#adjclose['TSEC weighted index'].plot()
#sharpe['TSEC weighted index'].plot(secondary_y=True)
####################################
# Sharpe ratio
#先將夏普值平滑一下，不然雜訊太多了
sr = sharpe['TSEC weighted index'].dropna()

d = 60
srsma = sr.rolling(d).mean()
#sr.plot()
#srsma.plot()
####################################
# 夏普曲線的斜率
srsma = sr.rolling(d).mean()
srsmadiff = srsma.diff()
srsma.plot()
srsmadiff.plot(secondary_y=True)
#adjclose['TSEC weighted index'].plot()
####################################
#找轉折點了，就是斜率由正到負，或由負到正的瞬間
buy = (srsmadiff > 0) & (srsmadiff.shift() < 0)
sell = (srsmadiff < 0) & (srsmadiff.shift() > 0)

(buy * 1).plot()
(sell * -1).plot()


####################################
#都用一樣的方式來產生這些訊號
#買賣的結果
hold = pd.Series(np.nan, index=buy.index)
hold[buy] = 1
hold[sell] = -1
hold.ffill(inplace=True)
#hold.plot()
####################################
#回測
twii = adjclose['TSEC weighted index'][buy.index]

pct_change = twii.pct_change() #pct_change是一個每天獲利上下 x％
pct_ratio = (pct_change.shift(-1)+1) # 今天到明天的價格變化,
               #代表買入之後每天的變化（不漲不跌是1，大於1則漲，小於1則跌）
#持有」時間段的pct_ratio全部都乘起來，代表獲利
pct_ratio.fillna(1)[hold == 1].cumprod().plot()



plt.show()

