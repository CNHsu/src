# -*- coding: UTF-8 -*-
import matplotlib.pyplot as plt
import numpy as np

# draw 0-180

x = np.arange(0,360)
y = np.sin(x * np.pi / 180.0)



plt.plot(x,y)
    # define draw area
plt.xlim(-30,390)
plt.ylim(-1.5,1.5)
    # x,y and title
plt.xlabel("x-axis")
plt.ylabel("y-axis")
plt.title("The Title")



plt.show()


plt.savefig("filename.png",dpi=300,format="png") 
