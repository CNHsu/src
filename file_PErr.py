# coding=utf-8

from tkinter.filedialog import askopenfilename
import re
from  math import *
import tkinter as tk

###################################################
# 2017/12/22 L-NAND
# input: * log file output from RDT read all page Err per 4KB
# output: block page Err
#
# use this file to extrate BB table

import os
root = tk.Tk()
root.title("Page Err parsing 20171222 by CN")
#root.geometry('400x400')
label=tk.Label(root, text="Select Err log file\n Output: Err.txt",pady=20).grid(row=0,sticky='n')
var = tk.StringVar(value="start append text")

text_input = tk.Entry(root,textvariable=var,width=36, bd=5).grid(row=1,sticky='n')

def update_text():
    global filename
    filename = askopenfilename()	
    str1=var.get()
    append_text(str1)
    text_output.delete(0.0,'end')
    text_output.insert(0.0,get_text())
    #text_output.config(text=get_text())
    var.set(' ')


tk.Button(root, text="select Err log file", command=update_text).grid(row=2,sticky='n')
root.bind('<Return>', lambda event:update_text())

#text_output=tk.Message(root,text=' ')
text_output=tk.Text(root,width=50,height=10,wrap='none')
#text_output.pack(side='left',fill='y')
text_output.grid(row=3,sticky='news')

s=tk.Scrollbar(root)
s.grid(row=3,column=1,sticky='news')
s.config(command=text_output.yview)

sd=tk.Scrollbar(root,orient= 'horizontal')
sd.grid(row=4,sticky='news')
sd.config(command=text_output.xview)

text_output.config(xscrollcommand=sd.set,yscrollcommand=s.set)

# udev0~3, plane 0/1
filename1="u0p0.txt"
filename2="u0p1.txt"
filename3="u1p0.txt"
filename4="u1p1.txt"
    
def append_text(text):
    f1 = open(filename1,'a')
    f1.write(text+'\n')
    f1.close()
def append2_text(text):
    f2 = open(filename2,'a')
    f2.write(text+'\n')
    f2.close()
def append3_text(text):
    f3 = open(filename3,'a')
    f3.write(text+'\n')
    f3.close()
def append4_text(text):
    f4 = open(filename4,'a')
    f4.write(text+'\n')
    f4.close()    


def get_text():
    if not os.path.exists(filename):
        append_text('')
    else:
        f =open(filename, mode="r")
        Err1 =99
        while True:
            if Err1 == 99:
             str1 = f.readline()
            else :
             Err1=99               
            if not str1:
                break
            
            for i in range (0,100): #1469
                #print(i)
                for j in range (0,4):
                    str2="Read WERU "+str(i)+" udev "+str(j)
                    nn=str1.find(str2)
                    if (nn != -1):
                        #print(str2,nn,str1[nn:])
                        #print(i,j)
                        str2=str1[nn:]
                        str2=str2.replace('\n',' ')
                        str1 = f.readline() # read PAA ...
                        strEcc=""
                        for k in range(1,5):
                           str1 = f.readline() # read AU ECC
                           nn=str1.find("ECC ")+4
                           if (nn == 3):
                               Err1=j
                               break
                           else:
                               str3=str1.replace('\n','') # replace \n
                               strEcc=strEcc+str3[nn:]+' '
                               
                        str2=str2+strEcc
                        #print(str2,Err1,j)
                        if (j==0) & (Err1 != 4):
                            append_text(str2)
                        if  (j == 1) & (Err1 != 4):
                            append2_text(str2)
                        if  (j == 2) & (Err1 != 4):
                            append3_text(str2)
                        if  (j == 3) & (Err1 != 4):
                            append4_text(str2)
                                    
        print('finish')            
        #f.seek(0,0)
        #text = f.read()
        f.close()
        #return text
        return "done"
        
    if __name__ == '__main__':
        while True:
            text_input =raw_input('write input today:\n')
            if text_input.lower() in ['quit', 'q','exit']:break
            append_text(text_input)
            print ("your diary:\n", get_text())
            
        
#message3=tk.Message(root,text=filename,pady=20).pack()


root.mainloop()
