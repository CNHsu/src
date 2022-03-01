# coding=utf-8
import Tkinter as tk
import os
from ScrolledText import ScrolledText

root = tk.Tk()
root.title("Log file tool")
#root.geometry('400x400')
label=tk.Label(root, text="Welcome to Log tool",pady=20).pack()
var = tk.StringVar(value="start append text")

text_input = tk.Entry(root,textvariable=var,width=36, bd=5).pack()
text_output=ScrolledText(root,width=46)
def update_text():
    str1=var.get()
    append_text(str1)
    text_output.delete(0.0,'end')
    text_output.insert(0.0,get_text())
    #text_output.config(text=get_text())
    var.set(' ')


tk.Button(root, text="dump file", command=update_text).pack()
root.bind('<Return>', lambda event:update_text())

#text_output=tk.Message(root,text=' ')
#text_output=tk.Text(root,height=10)
#text_output.pack(side='left',fill='y')
text_output.pack()

#s=tk.Scrollbar(root)
#s.pack(side='right',fill='both')
#s.config(command=text_output.yview)
#text_output.config(yscrollcommand=s.set)

#sd=tk.Scrollbar(root)
#sd.pack(side='bottom',fill='x')
#sd.config(command=text_output.xview)
#text_output.config(xscrollcommand=sd.set)


filename="demo_test.log"

    
def append_text(text):
    f = open(filename,'a')
    f.write(text+'\n')
    f.close()

def get_text():
    if not os.path.exists(filename):
        append_text('')
    else:
        f =open(filename)
        text= f.read()
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
