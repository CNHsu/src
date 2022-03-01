# coding=utf-8

from tkinter.filedialog import askopenfilename
import re
from  math import *
import tkinter as tk

import os
root = tk.Tk()
root.title("Log file tool")
#root.geometry('400x400')
label=tk.Label(root, text="Welcome to Log tool",pady=20).grid(row=0,sticky='n')
var = tk.StringVar(value="start append text")

text_input = tk.Entry(root,textvariable=var,width=36, bd=5).grid(row=1,sticky='n')

def update_text():
    global filename
    filename = askopenfilename()	
    str1=var.get()
    #append_text(str1)
    text_output.delete(0.0,'end')
    text_output.insert(0.0,get_text())
    #text_output.config(text=get_text())
    var.set(' ')


tk.Button(root, text="dump file", command=update_text).grid(row=2,sticky='n')
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


filename1="1133_result.txt"
filename2cmp="D:\\src\\py\\data\\1133-log.txt"

    
def append_text(text):
    f1 = open(filename1,'a')
    f1.write(text+'\n')
    f1.close()

# 20170918
# find the same meta string

def get_text():
    if not os.path.exists(filename):
        append_text(filename+"open error")
    else:
        f =open(filename)
        for i in range(1,2305):
        #while True:
                str1 = f.readline()
                if str1 == "":
                    break
                Tstr1=str1.split()
                #print (Tstr1)
                if i % 100 ==0:
                    print("#",i)
               	for num in Tstr1:
                    num=num.strip(',')
                    #print("# ",num)
                    fcmp=open(filename2cmp,'r')
                    #print "fcmp open"
                    linenum=0
                    while True:
                        str1cmp= fcmp.readline()
                        if str1cmp == "":
                            break
                        str1cmp=str1cmp.split()
                        for comp in str1cmp:
                            Tstr1cmp = comp.find(num)
                            if Tstr1cmp != -1:
                                if linenum !=0 :
                                    strout="$"+str(i)+" "+num+" line=" + str(linenum) +",column="+str(Tstr1cmp)+"\n"
                                    print(strout)
                                    append_text(strout)
                                linenum +=1
                fcmp.close()       	    	
        	    	#print num
#                        append_text(astr)
    #f.seek(0,0)
    #text = f.read()
    text ="finished"
    f.close()
    return text
#message3=tk.Message(root,text=filename,pady=20).pack()


root.mainloop()
