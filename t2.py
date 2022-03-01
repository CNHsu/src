#from Tkinter import *
import Tkinter as tk

master = tk.Tk()

def var_states():
   print("male: %d,\nfemale: %d" % (var1.get(), var2.get()))

tk.Label(master, text="Your sex:").grid(row=0, sticky='W')
var1 = tk.IntVar()
tk.Checkbutton(master, text="male", variable=var1).grid(row=1, sticky='W')
var2 = tk.IntVar()
tk.Checkbutton(master, text="female", variable=var2).grid(row=2, sticky='W')
tk.Button(master, text='Quit', command=master.quit).grid(row=3, sticky='W', pady=4)
tk.Button(master, text='Show', command=var_states).grid(row=4, sticky='W', pady=4)
master.mainloop()