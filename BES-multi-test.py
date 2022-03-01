# coding=utf-8

# 2019/4/29 update for Q1,Q3 change to pmose
#           uA_SET signal need reverse
#2019/5/29 BT module HC-05
# 2020/1/7 test mega

import time
import serial  # 引用pySerial模組
import serial.tools.list_ports

import tkinter as tk
from tkinter import ttk


data= 'start'

COM_PORT = 'COM1'    # 指定通訊埠名稱
BAUD_RATES = 115200    # 設定傳輸速率

comlist=serial.tools.list_ports.comports()
connected= []
vv = []
for element in comlist:
    connected.append(element.device)
    print("com ports:" +str(connected))
try:    
 ser = serial.Serial(COM_PORT, BAUD_RATES, timeout=3)   # 初始化序列通訊埠
 print("UART port: ",ser.name)
except:
 print("UART port error")
 ser=-1
###################################################
# 2019/3/12 BES hw test tool UI
# arduino nano v3.0
# Orcad: BES POWER TSET TOOL_V10_0312.dsn
# by CNHsu
# 

import os
root = tk.Tk()
root.title("mega Test TOOL 2020/1/7")
#root.geometry('400x400')
var = tk.StringVar()
#########################################################
# text input window
#
#label=tk.Label(root, text="COM :",padx=10,width=5).grid(row=0,sticky='w')
#var = tk.StringVar(value='12')
#text_input = tk.Entry(root,textvariable=var,width=10,bd=5).grid(row=0,column=0,sticky='e')
#########################################################
# list box
#

def check_cbox():
    print( pinput.current())
    
label=tk.Label(root, text="COM :",padx=5,width=5).grid(row=0,sticky='w')
listval =tk.StringVar()
listval.set(connected)
pinput = ttk.Combobox(root,values=connected,width=10)
pinput.grid(row=0,column=0,sticky='e')
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
        #var.set(ser.name)
        #print("select port open ",var.get())
    except:
        print("error")
        ser=-1
    
bbt = tk.Button(root, text="select", command=bt,width=7).grid(column=1,row=0,sticky='w')
########################################
# exit COM port button
def btexit():
    ser.close()    # 清除序列通訊物件
    text_output.insert(tk.END,"\n exit COM")
    
bbt = tk.Button(root, text="close COM port", command=btexit).grid(column=2,row=0,columnspan=2,sticky='w')


tvar5VI= tk.StringVar()
tvar4VI= tk.StringVar()
tvar5VI.set("5V I(mA/uA)")
tvar4VI.set("4V I(mA/uA)")
#try:
#    label=tk.Label(root, text="UART port : "+var.get(),pady=20).grid(column=0,row=1,sticky='n')
#except:
#    label=tk.Label(root, text="UART port : Error",pady=20).grid(column=0,row=1,sticky='n')
#    
#rowi=2
rowi=2

lb1=tk.Label(root, text="UART input",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi)
#lb2=tk.Label(root, text="4V sw ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+1)
#lb3=tk.Label(root, text="5V =  ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+2)
#lb4=tk.Label(root, text="4V =  ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+3)
#lb5=tk.Label(root, textvariable=tvar5VI,borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+4)
#lb6=tk.Label(root, textvariable=tvar4VI,borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+5)
#lb7=tk.Label(root, text="  K1   ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+6)
#lb8=tk.Label(root, text="  K2   ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+7)
#lb9=tk.Label(root, text="  K3   ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+8)
#lb10=tk.Label(root, text="BT-init ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+9)
#lb11=tk.Label(root, text="BT-scan ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+10)
#lb12=tk.Label(root, text="LED3 ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+11)
#lb13=tk.Label(root, text="FLT : ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+12)
#########################################################
# text output window
#

text_output=tk.Text(root,width=50,height=10,wrap='none')
text_output.grid(row=rowi+13,sticky='news',columnspan=4)

s=tk.Scrollbar(root)
s.grid(row=rowi+13,column=4,sticky='news')
s.config(command=text_output.yview)

sd=tk.Scrollbar(root,orient= 'horizontal')
sd.grid(row=rowi+14,sticky='news',columnspan=4)
sd.config(command=text_output.xview)

text_output.config(xscrollcommand=sd.set,yscrollcommand=s.set)



text_output.insert(0.0,"start"+'\n')

########################################
# 5V switch
tSW5V =tk.StringVar()
sSW5V =tk.IntVar()   

def SW5V():
    print(tSW5V.get())
    ser.write(str(tSW5V.get()).encode()) #serial send
    text_output.insert(tk.END,(tSW5V.get())+"\n")
    text_output.insert(tk.END,"serial read:")
    tSW5V.set("")
    time.sleep(2)
    while ser.in_waiting:# 若收到序列資料…
        data_raw = ser.readline()  # 讀取一行
        data_raw2 = data_raw.decode("utf8","ignore")
        #print(data_raw)
        print(data_raw2)
        text_output.insert(tk.END,data_raw2+'\n')        
    text_output.yview(tk.END)
    ser.reset_input_buffer()
    
sSW5V = tk.Entry(root, textvariable=tSW5V).grid(column=1,row=rowi)
#sSW5V.bind("<Return>",SW5V)
b2=tk.Button(root,text="send",command=SW5V).grid(column=2,row=rowi)





root.mainloop()
