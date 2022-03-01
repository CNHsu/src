from tkinter import *
from tkMessageBox import *
from tkFileDialog import askopenfilename
#import xlrd, xlwt


def newfile():
	print "new file"
	
def openfile():
	name = askopenfilename()
	print name
def About():
	print "This is a simple example"
def answer():
	showerror("Answer", "Sorry, not available")
	
def callback():
	if askyesno('Verify','Really quit?'):
		showwarning('Yes','Not yet implemented')
	else:
		showinfo('No','Quit has been cancelled')
				


root =Tk()
M =Menu(root)
root.config(menu=M)
filemenu=Menu(M)
M.add_cascade(label="File", menu=filemenu)
filemenu.add_command(label="New", command=newfile)
filemenu.add_command(label="Open...", command=openfile)
filemenu.add_separator()
filemenu.add_command(label="Exit", command=root.quit)
helpmenu =Menu(M)
M.add_cascade(label="help", menu=helpmenu)
helpmenu.add_command(label="About...",command=About)

T =Text(root, height=2, width=30)
T.grid(row=0,sticky='news')
T.insert(END, "Jest a test\n")

S= Scrollbar(root)
S.grid(row=0,column=1,sticky='news')
S.config(command=T.yview)
T.config(yscrollcommand=S.set)

Button(text='Quit', command=callback).grid(row=1, column=0,sticky='w')
Button(text='Answer',command=answer).grid(row=1, column=0,sticky='w',padx=50)

errmsg ='Error!'
Button(text='File Open',command=openfile).grid(row=2, sticky='n')

mainloop()