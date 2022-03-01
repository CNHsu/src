# coding=utf-8
from tkinter.filedialog import askopenfilename
import re
from  math import *
import tkinter as tk
from tkinter import ttk
import time
import serial  # 引用pySerial模組
import serial.tools.list_ports

data= 'start'

COM_PORT = 'COM1'    # 8 指定通訊埠名稱
BAUD_RATES = 921600    # 9600 設定傳輸速率
comlist=serial.tools.list_ports.comports()
connected= []
vv = []
for element in comlist:
    connected.append(element.device)
    print("com ports:" +str(connected))
try:    
 ser = serial.Serial(COM_PORT, BAUD_RATES)   # 初始化序列通訊埠
 print("UART port: ",ser.name)
except:
 print("UART port error")
 ser=-1
###################################################
# 2019/3/13 BES UART message test tool UI
#  
# BES tx,rx message output
# by CNHsu
#
import os
root = tk.Tk()
stitle='BES UART Test TOOL 2019/3/12'
root.title(stitle)

#########################################################
# list box
#

def check_cbox():
    print( pinput.current())
    
label=tk.Label(root, text="COM :",padx=10,width=5).grid(row=0,sticky='w')
listval =tk.StringVar()
listval.set(connected)
pinput = ttk.Combobox(root,values=connected,width=10)
pinput.grid(row=0,column=0,padx=80,sticky='w')
print(pinput.current(1))
########################################
# select COM port button

def bt():
    scom= str(pinput.get())
    print(scom)
    global ser
    try:
        ser = serial.Serial(scom, BAUD_RATES)   # 初始化序列通訊埠
        print("select port open ",ser.name)
        text_output.insert(tk.END,ser.name)
    except:
        print("error")
        ser=-1
    
bbt = tk.Button(root, text="select", command=bt,width=7).grid(column=1,row=0,sticky='w')
########################################
# exit COM port button
def btexit():
    ser.close()    # 清除序列通訊物件
    text_output.insert(tk.END,"\n exit COM")
    
bbt = tk.Button(root, text="close COM port", command=btexit).grid(column=1,row=0,padx=100,columnspan=2,sticky='w')



#########################################################
# text input window
#
label=tk.Label(root, text="input :",padx=10,width=5).grid(row=1,sticky='w')
var = tk.StringVar(value="input find string")
text_input = tk.Entry(root,textvariable=var,width=40,bd=5).grid(row=1,column=0,sticky='e')

#########################################################
# text output window
#

#text_output=tk.Text(root,width=50,height=10,wrap='none')
text_output=tk.Text(root,wrap='none')
text_output.grid(row=2,column=0,columnspan=2,sticky='news')

s=tk.Scrollbar(root)
s.grid(row=2,column=2,sticky='news')
s.config(command=text_output.yview)

sd=tk.Scrollbar(root,orient= 'horizontal')
sd.grid(row=3,column=0,columnspan=2,sticky='news')
sd.config(command=text_output.xview)

text_output.config(xscrollcommand=sd.set,yscrollcommand=s.set)

text_output.insert(0.0,"UART",COM_PORT,' ',BAUD_RATES)

########################################
# exit button
stopflag =0

def bt():
    text_output.insert(tk.END,"[3S] waiting to quit\n")
    root.update()
    #f=-1
    #while f ==-1:
    while ser.in_waiting:# 若收到序列資料…
            data_raw = ser.readline()  # 讀取一行
            data_raw2 = data_raw.decode("utf8","ignore")
            print('1接收到的原始資料：', data_raw2)
            text_output.insert(tk.END,data_raw2)
            st1=data_raw2
            sfind=var.get()
            f=st1.find(sfind)
            if f !=-1:
                print("got key word start process")
                ssfind="[3S]get key word"+' '+sfind
                text_output.insert(tk.END,ssfind)
            else:
                 print('1>>接收到的資料：', st1)
                 text_output.insert(tk.END,"[3S x]: ",st1)
                 #time.sleep(2)
                 #ser.flushInput()
    
bbt = tk.Button(root, text="start", command=bt,width=7).grid(column=1,row=1,sticky='w')

def bt2():
    text_output.insert(tk.END,"close serial port")
    stopflag=3;
    print("end")
    ser.close()    # 清除序列通訊物件
    print('再見！')
bbt2 = tk.Button(root, text="EXIT", command=bt2,pady=5).grid(column=0,row=4,sticky='news')

#except KeyboardInterrupt:
#    ser.close()    # 清除序列通訊物件
#    print('再見！')
root.mainloop()