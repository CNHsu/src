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
root.title("BB extrate tool 20171121")
#root.geometry('400x400')
label=tk.Label(root, text="Select BB binary file\n Output: binout.txt",pady=20).grid(row=0,sticky='n')
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


tk.Button(root, text="select binary file", command=update_text).grid(row=2,sticky='n')
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


filename1="binout.txt"

    
def append_text(text):
    f1 = open(filename1,'a')
    f1.write(text+'\n')
    f1.close()

def get_text():
    if not os.path.exists(filename):
        append_text('')
    else:
        f =open(filename, mode="rb")
        #data = f.read() # read all to data
        for i in range (128,1791): # range 0x80 to 0x6ff
            f.seek(i) #goto offset 0x80
            bb = f.read(1)
            print(bb)
            bin = "{0:b}".format(ord(bb))
            ssbin=str(bin)
            sbin="'"+'{:0>8}'.format(ssbin)
            print(bin)
            print(sbin)
            append_text(sbin)
            
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
