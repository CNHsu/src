# coding=utf-8
# by CNHsu
# 2019/4/29 update for Q1,Q3 change to pmose
#           uA_SET signal need reverse
#2019/5/29 BT module HC-05
#2019/8/5 try auto-run performance
#2019/8/7 modify for servo robot
# arduino code servo_robot


import time
import serial  # 引用pySerial模組
import serial.tools.list_ports

import tkinter as tk
from tkinter import ttk



data= 'start'

COM_PORT = 'COM1'    # 指定通訊埠名稱
BAUD_RATES = 38400    # 設定傳輸速率
global cntrun
cntrun=0 # servo continue run flag


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
# 
# arduino nano v3.0
# by CNHsu
# 

import os
root = tk.Tk()
root.title("Servo robot 2019/8/7")
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
pinput = ttk.Combobox(root,values=connected,width=6)
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
server1= tk.StringVar()
server2= tk.StringVar()
server3= tk.StringVar()
server4= tk.StringVar()
server1.set("m1 angle")
server2.set("m2 angle")
server3.set("m3 angle")
server4.set("m4 angle")
try:
    label=tk.Label(root, text="UART port : "+ser.name,pady=20).grid(column=0,row=1,sticky='n')
except:
    label=tk.Label(root, text="UART port : Error",pady=20).grid(column=0,row=1,sticky='n')
    
rowi=2

lb1=tk.Label(root, text="m1 0-180",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi)
lb2=tk.Label(root, text="m2 0-180",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+1)
lb3=tk.Label(root, text="m3 0-180",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+2)
lb4=tk.Label(root, text="m4 0-180",borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+3)
lb5=tk.Label(root, textvariable=server1,borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+4)
lb6=tk.Label(root, textvariable=server2,borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+5)
lb5=tk.Label(root, textvariable=server3,borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+6)
lb6=tk.Label(root, textvariable=server4,borderwidth=2, relief="groove",pady=5).grid(column=0,row=rowi+7)


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

print("!! run here!!")
#sMm =tk.IntVar()
def fsMm(self):
    if  cntrun ==1:
        time.sleep(0.4)
        print (s5Vmu.get())
        nm = [sM1.get()]        # def list
        nm.append(sM2.get())    # append list data
        nm.append(sM3.get())
        nm.append(sM4.get())
        print (": ",pm[0]," ", pm[1]," ",pm[2]," ",pm[3])
        print (": ",nm[0]," ", nm[1]," ",nm[2]," ",nm[3])
        for i in range(0,4):
            if pm[i] != nm[i]:
                pm[i]=nm[i]
                print("need mov:",servoname[i]+nm[i])
                ser.write(str(servoname[i]+nm[i]).encode())
                text_output.insert(tk.END,res4)

                #time.sleep(0.1)
#scalemm=tk.Scale(root,from_=0,to=180,orient="horizontal",variable=sMm, command=fsMm).grid(column=3,row=rowi+8)    


########################################
# servor m1 input
sM1=tk.StringVar()
res1=""
txt1 = tk.Entry(root,width=10, textvariable=sM1).grid(column=1,row=rowi)
def clicked():
    res1=sM1.get()
    if int(res1) <0 or int(res1) >180:
        print("out of range")
    else:
        res1='m1'+res1
        print(res1)
        ser.write(str(res1).encode())
        text_output.insert(tk.END,res1)
btn1 = tk.Button(root, text="enter",command=clicked).grid(column=2,row=rowi)
scalem1=tk.Scale(root,from_=0,to=180,orient="horizontal",variable=sM1,command=fsMm).grid(column=3,row=rowi)

########################################
# servor m2 input
sM2=tk.StringVar()
res2=""
txt2 = tk.Entry(root,width=10, textvariable=sM2).grid(column=1,row=rowi+1)
def clicked2():
    res2=sM2.get()
    if int(res2) <0 or int(res2) >180:
        print("out of range")
    else:
        res2='m2'+res2
        print(res2)
        ser.write(str(res2).encode())
        text_output.insert(tk.END,res2)
btn2 = tk.Button(root, text="enter",command=clicked2).grid(column=2,row=rowi+1)
scalem2=tk.Scale(root,from_=0,to=180,orient="horizontal",variable=sM2,command=fsMm).grid(column=3,row=rowi+1)
########################################
# servor m3 input
sM3=tk.StringVar()
res3=""
txt3 = tk.Entry(root,width=10, textvariable=sM3).grid(column=1,row=rowi+2)
def clicked3():
    res3=sM3.get()
    if int(res3) <0 or int(res3) >180:
        print("out of range")
    else:
        res3='m3'+res3
        print(res3)
        ser.write(str(res3).encode())
        text_output.insert(tk.END,res3)
btn3 = tk.Button(root, text="enter",command=clicked3).grid(column=2,row=rowi+2)
scalem3=tk.Scale(root,from_=0,to=180,orient="horizontal",variable=sM3,command=fsMm).grid(column=3,row=rowi+2)
########################################
# servor m4 input
sM4=tk.StringVar()
res4=""
txt4 = tk.Entry(root,width=10, textvariable=sM4).grid(column=1,row=rowi+3)
def clicked4():
    res4=sM4.get()
    if int(res4) <0 or int(res4) >180:
        print("out of range")
    else:
        res4='m4'+res4
        print(res4)
        ser.write(str(res4).encode())
        text_output.insert(tk.END,res4)
btn4 = tk.Button(root, text="enter",command=clicked4).grid(column=2,row=rowi+3) 
scalem4=tk.Scale(root,from_=0,to=180,orient="horizontal",variable=sM4,command=fsMm).grid(column=3,row=rowi+3)


########################################
# read m1
tRm1 =tk.StringVar()
sRm1 =tk.IntVar()
tRm1.set("measure")    
lbRm1 = tk.Label(root, textvariable=tRm1,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi+4)

def CRm1():
    tRm1.set('checking')
    data_raw="waiting"
    print("read m1 angle")
    ser.write(str("r1").encode())
    time.sleep(2)
    while ser.in_waiting:# 若收到序列資料…
        data_raw = ser.readline()  # 讀取一行
        data_raw2 = data_raw.decode("utf8","ignore")
        print(data_raw2)
        text_output.insert(tk.END,data_raw2)
        f=data_raw2.find("read1")
        if f !=-1:      
            pt=data_raw2.index(":")
            data_raw2 =data_raw2.replace('\n','')            
            tRm1.set(data_raw2[pt+1:])
            print('o ',data_raw2)
            text_output.insert(tk.END,'ok ',data_raw2)
        else:
            #ser.write(str("7").encode())
            #t4VV.set(data_raw2)
            print('x ',data_raw2)
            text_output.insert(tk.END,'x ',data_raw2)
    print("end")
    ser.flushInput()
    
bRm1 = tk.Button(root, text="check", command=CRm1,pady=5).grid(column=2,row=rowi+4)

########################################
# read m2
tRm2 =tk.StringVar()
sRm2 =tk.IntVar()
tRm2.set("measure")    
lbRm2 = tk.Label(root, textvariable=tRm2,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi+5)

def CRm2():
    tRm2.set('checking')
    data_raw="waiting"
    print("read m2 angle")
    ser.write(str("r2").encode())
    time.sleep(2)
    while ser.in_waiting:# 若收到序列資料…
        data_raw = ser.readline()  # 讀取一行
        data_raw2 = data_raw.decode("utf8","ignore")
        print(data_raw2)
        text_output.insert(tk.END,data_raw2)
        f=data_raw2.find("read2")
        if f !=-1:      
            pt=data_raw2.index(":")
            data_raw2 =data_raw2.replace('\n','')            
            tRm2.set(data_raw2[pt+1:])
            print('o ',data_raw2)
            text_output.insert(tk.END,'ok ',data_raw2)
        else:
            #ser.write(str("7").encode())
            #t4VV.set(data_raw2)
            print('x ',data_raw2)
            text_output.insert(tk.END,'x ',data_raw2)
    print("end")
    ser.flushInput()
    
bRm2 = tk.Button(root, text="check", command=CRm2,pady=5).grid(column=2,row=rowi+5)
########################################
# read m3
tRm3 =tk.StringVar()
sRm3 =tk.IntVar()
tRm3.set("measure")    
lbRm3 = tk.Label(root, textvariable=tRm3,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi+6)

def CRm3():
    tRm3.set('checking')
    data_raw="waiting"
    print("read m3 angle")
    ser.write(str("r3").encode())
    time.sleep(2)
    while ser.in_waiting:# 若收到序列資料…
        data_raw = ser.readline()  # 讀取一行
        data_raw2 = data_raw.decode("utf8","ignore")
        print(data_raw2)
        text_output.insert(tk.END,data_raw2)
        f=data_raw2.find("read3")
        if f !=-1:      
            pt=data_raw2.index(":")
            data_raw2 =data_raw2.replace('\n','')            
            tRm3.set(data_raw2[pt+1:])
            print('o ',data_raw2)
            text_output.insert(tk.END,'ok ',data_raw2)
        else:
            #ser.write(str("7").encode())
            #t4VV.set(data_raw2)
            print('x ',data_raw2)
            text_output.insert(tk.END,'x ',data_raw2)
    print("end")
    ser.flushInput()
    
bRm3 = tk.Button(root, text="check", command=CRm3,pady=5).grid(column=2,row=rowi+6)
########################################
# read m4
tRm4 =tk.StringVar()
sRm4 =tk.IntVar()
tRm4.set("measure")    
lbRm4 = tk.Label(root, textvariable=tRm4,borderwidth=2, relief="groove",pady=5).grid(column=1,row=rowi+7)

def CRm4():
    tRm4.set('checking')
    data_raw="waiting"
    print("read m4 angle")
    ser.write(str("r4").encode())
    time.sleep(2)
    while ser.in_waiting:# 若收到序列資料…
        data_raw = ser.readline()  # 讀取一行
        data_raw2 = data_raw.decode("utf8","ignore")
        print(data_raw2)
        text_output.insert(tk.END,data_raw2)
        f=data_raw2.find("read4")
        if f !=-1:      
            pt=data_raw2.index(":")
            data_raw2 =data_raw2.replace('\n','')            
            tRm4.set(data_raw2[pt+1:])
            print('o ',data_raw2)
            text_output.insert(tk.END,'ok ',data_raw2)
        else:
            #ser.write(str("7").encode())
            #t4VV.set(data_raw2)
            print('x ',data_raw2)
            text_output.insert(tk.END,'x ',data_raw2)
    print("end")
    ser.flushInput()
    
bRm4 = tk.Button(root, text="check", command=CRm4,pady=5).grid(column=2,row=rowi+7)


########################################
# check for continue move
s5Vmu =tk.IntVar()
servoname= ['m1','m2','m3','m4']
pm = [sM1.get()]        # def list
pm.append(sM2.get())    # append list data
pm.append(sM3.get())
pm.append(sM4.get())
nm = [sM1.get()]        # def list
nm.append(sM2.get())    # append list data
nm.append(sM3.get())
nm.append(sM4.get())
def f5Vmu():
    global cntrun
    if  s5Vmu.get() ==1:
            cntrun=1;
            print (s5Vmu.get())
            nm = [sM1.get()]        # def list
            nm.append(sM2.get())    # append list data
            nm.append(sM3.get())
            nm.append(sM4.get())
            print (": ",pm[0]," ", pm[1]," ",pm[2]," ",pm[3])
            print (": ",nm[0]," ", nm[1]," ",nm[2]," ",nm[3])
            print("cntrun=",cntrun)
            for i in range(0,4):
                if pm[i] != nm[i]:
                    pm[i]=nm[i]
                    print("need mov:",servoname[i]+nm[i])
                    ser.write(str(servoname[i]+nm[i]).encode())
                    text_output.insert(tk.END,res4)
                    time.sleep(3)
    else:
        cntrun=0
        print(cntrun)
        
cb5Vmu = tk.Checkbutton(root, text="continue",variable=s5Vmu, command=f5Vmu,pady=5).grid(column=3,row=rowi-1)


root.mainloop()
