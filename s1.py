#Sliders

from Tkinter import *

def show_val():
	print (s.get(), s1.get())

w = Tk()
s=Scale(w,from_=0, to=42, tickinterval=8)
s.set(10)
s.pack()
s1=Scale(w, from_=0, to=200,length=300,tickinterval=50, orient=HORIZONTAL)
s1.set(100)
s1.pack()

Button(w, text='Show', command=show_val).pack()
mainloop()