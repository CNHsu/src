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
    
    now = int(datetime.datetime.now().timestamp())+86400
    url = "https://query1.finance.yahoo.com/v7/finance/download/" + stock_id + "?period1=0&period2=" + str(now) + "&interval=1d&events=history&crumb=hP2rOschxO0"
    
    response = requests.post(url)
    
# data to system mem
    f = io.StringIO(response.text)
    df = pd.read_csv(f, index_col='Date', parse_dates=['Date'] )
# data to file
    #filename= fname +'.csv'
    #filename=filename.replace('/',' ')
    #print(filename)
    
# after csv saved block below to save time    
#    with open(filename, 'w') as f: # after 'first' write to file you can block this to save time
#        f.writelines(response.text)      # after  write to file  you can block this to save time
#
#
#    df = pd.read_csv(filename, index_col='Date', parse_dates=['Date'])

    return df
################################################
# a rolling
# 
def backtest(a, b, c, d):
    sr = sharpe
    srsma = sr.rolling(a).mean()

    srsmadiff = srsma.diff() * 100
    ub = srsmadiff.quantile(b) #概念：把给定的乱序数值由小到大排列并分成四等份，处于三个分割点位置的数值就是四分位数。
    lb = srsmadiff.quantile(c)
    
    buy = ((srsmadiff.shift(d) < lb) & (srsmadiff > ub))
    sell = ((srsmadiff.shift(d) > ub) & (srsmadiff < lb))

    hold = pd.Series(np.nan, index=buy.index)
    hold[buy] = 1
    hold[sell] = 0

    hold.ffill(inplace=True)
    
    adj = twii['Adj Close'][buy.index]

    # eq = (adj.pct_change().shift(-1)+1).fillna(1)[hold == 1].cumprod().plot()
    # hold.plot()
    eq = (adj.pct_change().shift(-1)+1).fillna(1)[hold == 1].cumprod()
    if len(eq) > 0:
        return eq.iloc[-1]
    else:
        return 1
################################################
# get world stock info

#url = "https://finance.yahoo.com/world-indices/"
#response = requests.get(url)
#f = io.StringIO(response.text)
#dfs = pd.read_html(f)
#world_index = dfs[0]

#world_index_history = {}
#for symbol, name in zip(world_index['Symbol'], world_index['Name']):
    
#    print(name)
    
#    world_index_history[name] = crawl_price(symbol,name)
#    time.sleep(5) # need delay or yahoo may block

twii =crawl_price("2886.TW",'taiwan')
twii.head()
twii.Close.plot()
#####################################################3
#
#復權收盤價
#它移除了除權息和增減資的價格波動，更準確的反映了「股票價值」的變化
#
#adjclose = {}
#for name, price in world_index_history.items():
#    if price.index.is_unique:
#        adjclose[name] = price['Adj Close']
    
#adjclose = pd.DataFrame(adjclose)
#adjclose

#corr= adjclose.corr()
#print(corr)

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
mean = twii['Adj Close'].pct_change().rolling(252).mean()
std = twii['Adj Close'].pct_change().rolling(252).std()

sharpe = mean / std

twii.Close.plot()
sharpe.plot(secondary_y=True)
eq=backtest(252,0.4,0.6,4)

####################################
# auto find the best
maxeq = 0

for a in range(100,200,20):
    for b in np.arange(0.3, 0.9, 0.03):
        for c in np.arange(0.3, 0.6, 0.03):
            for d in range(60, 180, 10):
                
                eq = backtest(a,b,c,d) # a:rolling,b: buy, c: sell,
                
                if maxeq < eq:
                    maxeq = eq
                    print(eq, a,b,c,d)



plt.show()

