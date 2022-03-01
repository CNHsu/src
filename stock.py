#-*- coding: UTF-8 -*-
import sys
import os
#from openpyxl import load_workbook
#from tkinter import filedialog
import tkinter as tk
import matplotlib.pyplot as plt
from matplotlib.finance import candlestick2_ohlc
import numpy as np


import twstock

def update_text():
    global stockno
    global stock
    stockno=var1.get()
    stock=twstock.realtime.get(stockno)
    #text_output.delete(0.0,'end')
    #text_output.insert(0.0,stock)
    str1 = str(stock)+"\n"
    text_output.insert('end',str1)

def show():
    x =[]
    stock1=twstock.Stock(stockno)
    bfp = twstock.BestFourPoint(stock1)
    bbuy=bfp.best_four_point_to_buy()
    bsell=bfp.best_four_point_to_sell()
    print ("Buy=",bbuy,"Sell=",bsell)
    #print (stock1.price, len(stock1.price))
    #print (stock1.fetch_from(2017,1))
    stock1.fetch_from(2017,1)
    size= len(stock1.price)
    
    fig = plt.figure()
    ax1 = plt.subplot2grid((1,1), (0,0))
    plt.xlabel("x-axis")
    plt.ylabel("y-axis")
    plt.title(stock1.sid)
    #plt.xlim(0,size)
    #plt.ylim(0,50)
    for i in range(1,size+1):
        x.append(i)        
    y= stock1.price
    plt.plot(x, y)
    candlestick2_ohlc(ax1,stock1.open,stock1.high,stock1.low,stock1.close,
                      colorup = "red", colordown="green",width=0.6)

    #plt.plot(40,50)
    plt.show()




root = tk.Tk()
root.title("Stock")

tk.Label(root, text="stock #").grid(row=1,column=0,sticky='w')

var1 = tk.StringVar(value="2892")
text_input = tk.Entry(root,textvariable=var1,width=36, bd=5).grid(row=1,column=0,sticky='e')

tk.Button(root, text="got info", command=update_text).grid(row=2,column=0,sticky='w')
root.bind('<Return>', lambda event:update_text())

tk.Button(root, text="show", command=show).grid(row=2,column=0,sticky='e')
root.bind('<Return>', lambda event:show())


text_output=tk.Text(root,width=50,height=10,wrap='none')
text_output.grid(row=6,sticky='news')
s=tk.Scrollbar(root)
s.grid(row=6,column=1,sticky='news')
s.config(command=text_output.yview)

sd=tk.Scrollbar(root,orient= 'horizontal')
sd.grid(row=7,sticky='news')
sd.config(command=text_output.xview)

text_output.config(xscrollcommand=sd.set,yscrollcommand=s.set)



root.mainloop()