# coding=utf-8

from tkinter.filedialog import askopenfilename
import re
from  math import *
import tkinter as tk

###################################################
# 2017/11/20
# input: *.bin file output from FTA FD binary file
# output: binout.txt, bin to string file
#
# use this file to extrate BB table

import os
root = tk.Tk()
root.title("LA file check")
#root.geometry('400x400')
label=tk.Label(root, text="Select LA txt file\n Output: out.txt",pady=20).grid(row=0,sticky='n')
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


tk.Button(root, text="select LA file", command=update_text).grid(row=2,sticky='n')
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


filename1="out.txt"

    
def append_text(text):
    f1 = open(filename1,'w')
    f1.write(text+'\n')
    f1.close()

def get_text():
    if not os.path.exists(filename):
        append_text('')
    else:
        f =open(filename, mode="r")
        #data = f.readline() # read all to data
        for line in f.readlines():
            sbin=line.split(", ")
            append_text(sbin)
            print(sbin)
            
            
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
