from Tkinter import *

def showfield():
	print("First Name: %s\nLast Name: %s" %(e1.get(), e2.get()))
	e1.delete(0,END)
	e2.delete(0,END)
	
win=Tk()
Label(win, text="First Name").grid(row=0)
Label(win, text="Last Name").grid(row=1)

e1= Entry(win)
e2= Entry(win)
e1.insert(10,"Rachel")
e2.insert(10,"Hsu")

e1.grid(row=0, column=1)
e2.grid(row=1, column=1)

Button(win, text='Quit', command=win.quit).grid(row=3, column=0, sticky=W, pady=4)
Button(win, text='Show', command=showfield).grid(row=3, column=1, sticky=W)

mainloop()