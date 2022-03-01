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
    append_text(str1)
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


filename1="eee.txt"

    
def append_text(text):
    f1 = open(filename1,'a')
    f1.write(text+'\n')
    f1.close()

def get_text():
    if not os.path.exists(filename):
        append_text('')
    else:
        f =open(filename)
        #for i in range(1,50):
        while True:
            str1 = f.readline()
            if not str1:
                break
        	#str1 =str1.strip('\n')
        	#num=str1.find("(0x91)")
            Tstr1=str1.split()
            for num in Tstr1:        
        	    if num.find("(0x91)") > 0 :
        	    	#str2=num.strip('(0x91)')
        	    	str2=num.replace('(0x91)',' ')
        	    	#temp=int(str2,16)
        	    	temp=int(str2[6:],16)
        	    	wl=temp/16/3
        	    	page=ceil(temp/16)
        	    	LMH=page%3
        	    	#astr=str(i)+'=='+num+"=="+str2[6:]+"*WL: "+str(wl)+"p:"+str(page)+" LMH "+str(LMH)
        	    	astr='=='+num+"=="+str2[6:]+"*WL: "+str(int(wl))+" p: "+str(page)+" LMH: "+str(LMH)
        	    	astr2 = str(int(wl))+','+str(page)+','+str(LMH)
        	    	#print (astr)
        	    	append_text(astr2)
        print('finish')    
        #append_text(astr)
        f.seek(0,0)
        text = f.read()
        f.close()
        return text
        
    if __name__ == '__main__':
        while True:
            text_input =raw_input('write input today:\n')
            if text_input.lower() in ['quit', 'q','exit']:break
            append_text(text_input)
            print ("your diary:\n", get_text())
            
        
#message3=tk.Message(root,text=filename,pady=20).pack()


root.mainloop()
