import Tkinter as tk

languages = [
    ("Python",1),
    ("Perl  ",2),
    ("Java  ",3),
    ("C++   ",4),
    ("C     ",5)
]

def ShowChoice():
    print languages[v.get()-1][0]

def var_states():
   print("male: %d,\nfemale: %d" % (var1.get(), var2.get()))
    
counter = 0 
def counter_label(label):
  def count():
    global counter
    counter += 1
    label.config(text=str(counter))
    label.after(1000, count)
  count()
 
 
 

 
root = tk.Tk()
root.title("Counting Seconds")
label = tk.Label(root, fg="green", font ="Times 12")
label.pack()
counter_label(label)

logo =tk.PhotoImage(file="D:/src/py/image/200.gif")
#w1=tk.Label(root, image=logo).pack(side="right")
str1 = """Whatever you do will be insignificant, 
but it is very important that you do it.\n(Mahatma Gandhi)"""
w2= tk.Label(root,compound ='center',padx=10,text=str1,image=logo).pack(side="right")
tk.Label(root,text="Times Font", fg ="red", font ="Times").pack()
tk.Label(root,text="Helvetica  Font", fg ="light green",bg="dark green", font ="Helvetica 16 bold italic").pack()
tk.Label(root,text="Verdana   Font", fg ="blue",bg="yellow", font ="Verdana 10 bold").pack()


button = tk.Button(root, text='Stop', width=25, command=root.destroy)
button.pack()

msg= tk.Message(root, text = str1)
msg.config(bg="light green", font="Times 24 italic")
msg.pack()

v = tk.IntVar()
v.set(1)
tk.Label(root,
	text=" Choose one:",
	justify= 'left',
	padx=20).pack()
for txt,val in languages:
 tk.Radiobutton(root,
	text=txt,
	justify='left',
	#padx=20,
	variable=v,
	value=val).pack()
		
for txt,val in languages:
 tk.Radiobutton(root,
	text=txt,
	indicatoron=0,
	width=20,
	padx=20,
	variable=v,
	command=ShowChoice,
	value=val).pack(anchor='w')

var1 = tk.IntVar()
tk.Checkbutton(root, 
	text="male", 
	variable=var1).pack(anchor='w')
	
var2 = tk.IntVar()
tk.Checkbutton(root, 
	text="female", 
	variable=var2).pack(anchor='w')
tk.Button(root, text='Quit', command=root.quit).pack(anchor='w')
tk.Button(root, text='Show', command=var_states).pack(anchor='w')

d1 = tk.Canvas(root, width=200, height=5)
d1.pack(expand=1)
d1.create_line(1, 1, 200, 1,width=4)

istr1=tk.StringVar(value="Rachel")
istr2=tk.StringVar(value="Hsu")

tk.Label(root, text="first name:").pack(side='left',expand=1)
e1=tk.Entry(root,textvariable=istr1).pack(side='left',expand=1,fill='x')

tk.Label(root, text="last name:").pack(side='left',expand=1)
e2=tk.Entry(root,textvariable=istr2).pack(side='left',expand=1,fill='x')



root.mainloop()