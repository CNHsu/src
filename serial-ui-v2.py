# coding=utf-8

# 2019/4/29 update for Q1,Q3 change to pmose
#           uA_SET signal need reverse

import time
import serial  # 引用pySerial模組
import serial.tools.list_ports

import tkinter as tk
from tkinter import ttk


data= 'start'

COM_PORT = 'COM1'    # 指定通訊埠名稱
BAUD_RATES = 9600    # 設定傳輸速率

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
# 2019/3/12 BES hw test tool UI
# arduino nano v3.0
# Orcad: BES POWER TSET TOOL_V10_0312.dsn
# by CNHsu
# 

import os
root = tk.Tk()
root.title("BES PWR Test TOOL 2019/3/12")
#root.geometry('400x400')

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
    
label=tk.Label(root, text="COM :",padx=10,width=5).grid(row=0,sticky='w')
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

var = tk.StringVar()
tvar5VI= tk.StringVar()
tvar4VI= tk.StringVar()
tvar5VI.set("5V I(mA/uA)")
tvar4VI.set("4V I(mA/uA)")
try:
    label=tk.Label(root, text="UART port : "+ser.name,pady=20).grid(column=0,row=1,sticky='n')
except:
    label=tk.Label(root, text="UART port : Error",pady=20).grid(column=0,row=1,sticky='n')
    
rowi=2

lb1=tk.Label(root, text="5V sw ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi)
lb2=tk.Label(root, text="4V sw ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+1)
lb3=tk.Label(root, text="5V =  ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+2)
lb4=tk.Label(root, text="4V =  ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+3)
lb5=tk.Label(root, textvariable=tvar5VI,borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+4)
lb6=tk.Label(root, textvariable=tvar4VI,borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+5)
lb7=tk.Label(root, text="  K1   ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+6)
lb8=tk.Label(root, text="  K2   ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+7)
lb9=tk.Label(root, text="  K3   ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+8)
lb10=tk.Label(root, text="LED1 ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+9)
lb11=tk.Label(root, text="LED2 ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+10)
lb12=tk.Label(root, text="LED3 ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+11)
lb13=tk.Label(root, text="FLT : ",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+12)
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

text_output.insert(0.0,"start")

########################################
# 5V switch
tSW5V =tk.StringVar()
sSW5V =tk.IntVar()
tSW5V.set("on/off")    
lb5V = tk.Label(root, textvariable=tSW5V,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi)
def SW5V():
    if sSW5V.get() == 1:
        tSW5V.set('ON')
        ser.write(str("1").encode())
        text_output.insert(tk.END,"\n 1 5V on")
    else:
        tSW5V.set('OFF')
        ser.write(str("2").encode())
        text_output.insert(tk.END,"\n 2 5V off")
        
cbSW5V = tk.Checkbutton(root, variable=sSW5V, command=SW5V,pady=5).grid(column=2,row=rowi)
########################################
# 4V switch
tSW4V =tk.StringVar()
sSW4V =tk.IntVar()
tSW4V.set("on/off")    
lb4V = tk.Label(root, textvariable=tSW4V,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi+1)
def SW4V():
    if sSW4V.get() == 1:
        tSW4V.set('ON')
        ser.write(str("3").encode())
        text_output.insert(tk.END,"\n 3 4V on")
    else:
        tSW4V.set('OFF')
        ser.write(str("4").encode())
        text_output.insert(tk.END,"\n 4 4V off")
        
cbSW4V = tk.Checkbutton(root, variable=sSW4V, command=SW4V,pady=5).grid(column=2,row=rowi+1)

########################################
# 5V voltage
t5VV =tk.StringVar()
s5VV =tk.IntVar()
t5VV.set("measure")    
lb5VV = tk.Label(root, textvariable=t5VV,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi+2)

def C5VV():
    t5VV.set('checking')
    ser.write(str("5").encode())
    data_raw="waiting"
    print("5 read serial...")
    text_output.insert(tk.END,"5 read 5V voltage...")
    while ser.in_waiting:# 若收到序列資料…
        data_raw = ser.readline()  # 讀取一行
        data_raw2 = data_raw.decode("utf8","ignore")
        print(data_raw2)
        text_output.insert(tk.END,data_raw2)
        f=data_raw2.find("5VV")
        if f !=-1:
            data_raw = ser.readline()  # 讀取一行
            data_raw2 = data_raw.decode("utf8","ignore")
            data_raw2 =data_raw2.replace('\n','')
            t5VV.set(data_raw2)
            print('o ',data_raw2)
            text_output.insert(tk.END,'ok ',data_raw2)
        else:
            #ser.write(str("5").encode())
            #t5VV.set(data_raw2)
            print('x ',data_raw2)
            text_output.insert(tk.END,'x ',data_raw2)
    print("end")
    ser.flushInput()
    
b5VV = tk.Button(root, text="check", command=C5VV,pady=5).grid(column=2,row=rowi+2)
########################################
# 5V current
t5VI =tk.StringVar()
s5VI =tk.IntVar()
t5VI.set("measure")    
lb5VI = tk.Label(root, textvariable=t5VI,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi+4)

def C5VI():
    t5VI.set('checking')
    data_raw="waiting"
    print("6 read serial...")
    ser.write(str("6").encode())
    while ser.in_waiting:# 若收到序列資料…
        data_raw = ser.readline()  # 讀取一行
        data_raw2 = data_raw.decode("utf8","ignore")
        print(data_raw2)
        text_output.insert(tk.END,data_raw2)
        f=data_raw2.find("5VI")
        if f !=-1:
            data_raw = ser.readline()  # 讀取一行
            data_raw2 = data_raw.decode("utf8","ignore")
            data_raw2 =data_raw2.replace('\n','')            
            t5VI.set(data_raw2)
            print('o ',data_raw2)
            text_output.insert(tk.END,'ok ',data_raw2)
        else:
            #ser.write(str("6").encode())
            #t5VI.set(data_raw2)
            print('x ',data_raw2)
            text_output.insert(tk.END,'x ',data_raw2)
    print("end")
    ser.flushInput()

    
b5VI = tk.Button(root, text="check", command=C5VI,pady=5).grid(column=2,row=rowi+4)
########################################
# 5VI mA uA
t5Vmu =tk.StringVar()
s5Vmu =tk.IntVar()
t5Vmu.set("mA/uA")    
lb5Vmu = tk.Label(root, text="mA/uA",borderwidth=2, relief="groove",pady=5).grid(column=3,row=rowi+4)
def f5Vmu():
    if s5Vmu.get() == 1:
        tvar5VI.set("5V(uA)")
        ser.write(str("9").encode())
        text_output.insert(tk.END,"\n9 5V uA")
    else:
        tvar5VI.set("5V(mA)")
        ser.write(str("a").encode())
        text_output.insert(tk.END,"\na 5V mA")
        
cb5Vmu = tk.Checkbutton(root, variable=s5Vmu, command=f5Vmu,pady=5).grid(column=4,row=rowi+4)


########################################
# 4V voltage
t4VV =tk.StringVar()
s4VV =tk.IntVar()
t4VV.set("measure")    
lb4VV = tk.Label(root, textvariable=t4VV,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi+3)

def C4VV():
    t4VV.set('checking')
    data_raw="waiting"
    print("7 read serial...")
    ser.write(str("7").encode())
    while ser.in_waiting:# 若收到序列資料…
        data_raw = ser.readline()  # 讀取一行
        data_raw2 = data_raw.decode("utf8","ignore")
        print(data_raw2)
        text_output.insert(tk.END,data_raw2)
        f=data_raw2.find("4VV")
        if f !=-1:
            data_raw = ser.readline()  # 讀取一行
            data_raw2 = data_raw.decode("utf8","ignore")
            data_raw2 =data_raw2.replace('\n','')            
            t4VV.set(data_raw2)
            print('o ',data_raw2)
            text_output.insert(tk.END,'ok ',data_raw2)
        else:
            #ser.write(str("7").encode())
            #t4VV.set(data_raw2)
            print('x ',data_raw2)
            text_output.insert(tk.END,'x ',data_raw2)
    print("end")
    ser.flushInput()
    
b4VV = tk.Button(root, text="check", command=C4VV,pady=5).grid(column=2,row=rowi+3)
########################################
# 4V current
t4VI =tk.StringVar()
s4VI =tk.IntVar()
t4VI.set("measure")    
lb4VI = tk.Label(root, textvariable=t4VI,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi+5)

def C4VI():
    t4VI.set('checking')
    data_raw="waiting"
    print("8 read serial...")
    text_output.insert(tk.END,"\n8 read 4V current")
    ser.write(str("8").encode())
    while ser.in_waiting:# 若收到序列資料…
        data_raw = ser.readline()  # 讀取一行
        data_raw2 = data_raw.decode("utf8","ignore")
        print(data_raw2)
        f=data_raw2.find("4VI")
        if f !=-1:
            data_raw = ser.readline()  # 讀取一行
            data_raw2 = data_raw.decode("utf8","ignore")
            data_raw2 =data_raw2.replace('\n','')            
            t4VI.set(data_raw2)
            print('o ',data_raw2)
            text_output.insert(tk.END,'ok ',data_raw2)
        else:
            #ser.write(str("8").encode())
            #t4VI.set(data_raw2)
            print('x ',data_raw2)
            text_output.insert(tk.END,'x ',data_raw2)
    print("end")
    ser.flushInput()
    
b4VI = tk.Button(root, text="check", command=C4VI,pady=5).grid(column=2,row=rowi+5)
########################################
# 4VI mA uA
t4Vmu =tk.StringVar()
s4Vmu =tk.IntVar()
t4Vmu.set("mA/uA")    
lb4Vmu = tk.Label(root, text="mA/uA",borderwidth=2, relief="groove",pady=5).grid(column=3,row=rowi+5)
def f4Vmu():
    if s4Vmu.get() == 1:
        tvar4VI.set("4V(uA)")
        ser.write(str("b").encode())
        text_output.insert(tk.END,"\nb 4V uA")
    else:
        tvar4VI.set("4V(mA)")
        ser.write(str("c").encode())
        text_output.insert(tk.END,"\nc 4V mA")
cb4Vmu = tk.Checkbutton(root, variable=s4Vmu, command=f4Vmu,pady=5).grid(column=4,row=rowi+5)




########################################
# k1 switch
tSWk1 =tk.StringVar()
sSWk1 =tk.IntVar()
tSWk1.set("on/off")    
lbk1 = tk.Label(root, textvariable=tSWk1,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi+6)
def SWk1():
    if sSWk1.get() == 1:
        tSWk1.set('ON')
        ser.write(str("d").encode())
        text_output.insert(tk.END,"\nd k1 ON")
    else:
        tSWk1.set('OFF')
        ser.write(str("e").encode())
        text_output.insert(tk.END,"\ne k1 OFF")
        
cbSWk1 = tk.Checkbutton(root, variable=sSWk1, command=SWk1,pady=5).grid(column=2,row=rowi+6)

########################################
# k2 switch
tSWk2 =tk.StringVar()
sSWk2 =tk.IntVar()
tSWk2.set("on/off")    
lbk2 = tk.Label(root, textvariable=tSWk2,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi+7)
def SWk2():
    if sSWk2.get() == 1:
        tSWk2.set('ON')
        ser.write(str("f").encode())
        text_output.insert(tk.END,"\nf K2 on")
    else:
        tSWk2.set('OFF')
        ser.write(str("g").encode())
        text_output.insert(tk.END,"\ng K2 off")
        
cbSWk2 = tk.Checkbutton(root, variable=sSWk2, command=SWk2,pady=5).grid(column=2,row=rowi+7)

########################################
# k3 switch
tSWk3 =tk.StringVar()
sSWk3 =tk.IntVar()
tSWk3.set("on/off")    
lbk3 = tk.Label(root, textvariable=tSWk3,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi+8)
def SWk3():
    if sSWk3.get() == 1:
        tSWk3.set('ON')
        ser.write(str("h").encode())
        text_output.insert(tk.END,"\nh K3 on")
    else:
        tSWk3.set('OFF')
        ser.write(str("i").encode())
        text_output.insert(tk.END,"\ni K3 off")
        
cbSWk3 = tk.Checkbutton(root, variable=sSWk3, command=SWk3,pady=5).grid(column=2,row=rowi+8)


########################################
# LED1 switch
tLED1 =tk.StringVar()
sLED1 =tk.IntVar()
tLED1.set("on/off")    
lbLED1 = tk.Label(root, textvariable=tLED1,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi+9)
def LED1():
    if sLED1.get() == 1:
        tLED1.set('ON')
        ser.write(str("u").encode())
        text_output.insert(tk.END,"\nu LED1 on")
    else:
        tLED1.set('OFF')
        ser.write(str("v").encode())
        text_output.insert(tk.END,"\nv ED1 off")
        
cLED1 = tk.Checkbutton(root, variable=sLED1, command=LED1,pady=5).grid(column=2,row=rowi+9)

########################################
# LED2 switch
tLED2 =tk.StringVar()
sLED2 =tk.IntVar()
tLED2.set("on/off")    
lbLED2 = tk.Label(root, textvariable=tLED2,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi+10)
def LED2():
    if sLED2.get() == 1:
        tLED2.set('ON')
        ser.write(str("w").encode())
        text_output.insert(tk.END,"\nw LED2 on")
    else:
        tLED2.set('OFF')
        ser.write(str("x").encode())
        text_output.insert(tk.END,"\nx LED2 off")
        
cLED2 = tk.Checkbutton(root, variable=sLED2, command=LED2,pady=5).grid(column=2,row=rowi+10)

########################################
# LED3 switch
tLED3 =tk.StringVar()
sLED3 =tk.IntVar()
tLED3.set("on/off")    
lbLED3 = tk.Label(root, textvariable=tLED3,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi+11)
def LED3():
    if sLED3.get() == 1:
        tLED3.set('ON')
        ser.write(str("y").encode())
        text_output.insert(tk.END,"\ny LED3 on")
    else:
        tLED3.set('OFF')
        ser.write(str("z").encode())
        text_output.insert(tk.END,"\nz LED3 off")
        
cLED3 = tk.Checkbutton(root, variable=sLED3, command=LED3,pady=5).grid(column=2,row=rowi+11)

########################################
# FLT state
tflt =tk.StringVar()
sflt =tk.IntVar()
tflt.set("measure")    
lflt = tk.Label(root, textvariable=tflt,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi+12)

def FLT():
    tflt.set('checking')
    data_raw="waiting"
    print("s read serial...")
    text_output.insert(tk.END,"\n check FLT status...")
    ser.write(str("s").encode())
    while ser.in_waiting:# 若收到序列資料…
        data_raw = ser.readline()  # 讀取一行
        data_raw2 = data_raw.decode("utf8","ignore")
        print(data_raw2)
        text_output.insert(tk.END,data_raw2)
        f=data_raw2.find("FLT")
        if f !=-1:
            data_raw = ser.readline()  # 讀取一行
            data_raw2 = data_raw.decode("utf8","ignore")
            data_raw2 =data_raw2.replace('\n','')            
            tflt.set(data_raw2)
            print('o ',data_raw2)
            text_output.insert(tk.END,'ok ',data_raw2)
        else:
            #ser.write(str("8").encode())
            #t4VI.set(data_raw2)
            print('x ',data_raw2)
            text_output.insert(tk.END,'x ',data_raw2)
    print("end")
    ser.flushInput()
    
bflt = tk.Button(root, text="check", command=FLT,pady=5).grid(column=2,row=rowi+12)





root.mainloop()
