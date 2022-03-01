#from tkinter import *
forom Tkinter import *
cw=500
ch=150
px=0
py=0
def start(event):
    global px, py
    px=event.x
    py=event.y
    
    
def paint(event):
	global px, py
	python_green ="#476042"
	x1,y1=(event.x-1), (event.y-1)
	x2,y2=(event.x+1), (event.y+1)
	d.create_oval(x1,y1,x2,y2,fill=python_green)
	if px+py !=0: 
	   d.create_line(px,py,x2,y2,fill=python_green)
	px=x1
	py=y1
	return
	


w= Tk()
d=Canvas(w,width=cw, height=ch)
d.pack(expand=YES,fill=BOTH)
d.bind("<B1-Motion>",paint)
d.bind("<Double-Button-1>",start)

message=Label(w,text="press and Drag")
message.pack(side=BOTTOM)
d.create_rectangle(50, 20, 150, 80, fill="#476042")
d.create_rectangle(65, 35, 135, 65, fill="yellow")
d.create_line(0, 0, 50, 20, fill="#476042", width=3)
d.create_line(0, 100, 50, 80, fill="#476042", width=3)
d.create_line(150,20, 200, 0, fill="#476042", width=3)
d.create_line(150, 80, 200, 100, fill="#476042", width=3)

d.create_text(cw / 2,
              ch / 2,
              text="Python")

id=d.create_oval(50,50,100,100)

bitmaps = ["error", "gray75", 
	"gray50", "gray25", 
	"gray12", "hourglass", 
	"info", "questhead", 
	"question", "warning"]
nsteps =len(bitmaps)
step_x=int(cw/nsteps)
for i in range(0,nsteps):
	d.create_bitmap((i+1)*step_x -step_x/2, 50, bitmap=bitmaps[i])

img = PhotoImage(file="D:/src/py/image/200.gif")
d.create_image(120,120, anchor=N, image=img)
mainloop()