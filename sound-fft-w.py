#CNHsu 2019/6/28
# fft test
#
# read wav file and do fft
# write fft to fft.txt file
#
import math
import wave
#import matplotlib.pyplot as pl
from scipy.fftpack import fft, fftfreq, fftshift
from scipy.io import wavfile # get the api
import numpy as np
import sys
 

#fname=input('wav filename =')
#f=wave.open(fname,"rb")
print("filename "+sys.argv[1])
f=wave.open(sys.argv[1],"rb") # system argument

#f=wave.open(r"soundtest.wav","rb")
#f=wave.open(r"k-hyper-on.wav","rb")
#f=wave.open(r"0_16.wav","rb")


params = f.getparams()
nchannels, sampwidth, framerate, nframes = params[:4]
print(nchannels, sampwidth, framerate, nframes )
str_data = f.readframes(nframes)
f.close()
wave_data = np.fromstring(str_data, dtype=np.short)
wave_data.shape = -1, 2
wave_data = wave_data.T
time = np.arange(0, nframes) * (1.0 / framerate)

# use scipy fft
T=1.0/framerate
N=nframes//2
x=np.linspace(0.0, N*T,N)
yf=fft(wave_data[0])
xf =np.linspace(0.0,1.0/(2.0*T),N)

Y=fftshift(yf)
p = np.angle(Y)*60
p[np.abs(Y) < 1] =0

#dB
dbdata=wave_data[0]/32767.0

#pl.figure()
#pl.subplot(311)
#pl.grid()
#pl.ylabel('Amplitude')
#pl.xlabel('sec')

#pl.plot(time, dbdata, c="g")
#pl.subplot(312)
#pl.grid()
#pl.xscale("symlog")
#pl.plot(xf,2.0/N * np.abs(yf[0:N]))
np.savetxt("fft.txt",np.column_stack((xf,2.0/N * np.abs(yf[0:N]))),fmt='%.2f',delimiter=',')
#pl.xlabel('Hz')
#pl.ylabel('FFT')
#pl.subplot(313)
#pl.grid()
#pl.xscale("symlog")
#pl.plot(xf,p[0:N])
#pl.xlabel('Hz')
#pl.ylabel('phase')


#pl.show()
