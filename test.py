# encoding: utf-8
import Tkinter as tk
win=tk.Tk()     #建立視窗容器物件
win.title("Tk GUI")
label=tk.Label(win, text="Hello World!")   #建立標籤物件
label.pack()       #顯示元件
button=tk.Button(win, text="OK")
button.pack()     #顯示元件
win.mainloop()
