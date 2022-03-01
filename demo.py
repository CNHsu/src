# coding=utf-8

from tkFileDialog import askopenfilename
import re
import Tkinter as tk

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


#filename="elog.txt"

    
def append_text(text):
    f = open(filename,'a')
    f.write(text+'\n')
    f.close()

def get_text():
    if not os.path.exists(filename):
        append_text('')
    else:
        f =open(filename)
        for i in range(1,11):
        	str1=f.readline()
        	str1 =str1.strip('\n')
        	#num=str1.find("(0x91)")
        	for num in re.finditer("(0x91)", str1):
        	    print "***"+str(i)+"="+ str(num.start())
        	    
        text = f.read()
        f.close()
        return text
        
    if __name__ == '__main__':
        while ture:
            text_input =raw_input('write input today:\n')
            if text_input.lower() in ['quit', 'q','exit']:break
            append_text(text_input)
            print "your diary:\n", get_text()
            
        
#message3=tk.Message(root,text=filename,pady=20).pack()


root.mainloop()
