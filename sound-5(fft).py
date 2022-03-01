#CNHsu 2019/6/28
# fft test
#
#
import wave
import matplotlib.pyplot as pl
from scipy.fftpack import fft, fftfreq, fftshift
from scipy.io import wavfile # get the api
import numpy as np

f=wave.open(r"file.wav","rb")
params = f.getparams()
nchannels, sampwidth, framerate, nframes = params[:4]
print(nchannels, sampwidth, framerate, nframes )
str_data = f.readframes(nframes)
f.close()
wave_data = np.fromstring(str_data, dtype=np.short)
wave_data.shape = -1, 2
wave_data = wave_data.T
time = np.arange(0, nframes) * (1.0 / framerate)

#
#pl.subplot(211) 
#pl.plot(time, wave_data[0])
#pl.subplot(212) 
##pl.plot(time, wave_data[1], c="g")
#fft_out=fft(wave_data[0])
#pl.plot(np.abs(wave_data[0]), np.abs(fft_out))
#pl.xlabel("freq")
#pl.show()

# use np fft
#sp =np.fft.fft(wave_data[0])
#ampSP = np.abs(sp)
#phaseSP = np.unwrap(np.angle(sp))
#time_step = 1./wave_data[0]
#print(time_step)
#freqs = np.fft.fftfreq(sp.size, time_step)
#idx = np.argsort(freqs) # from negative to positive
#pl.figure()
#pl.subplot(211)
#pl.plot(freqs[idx[len(idx)//2:]], ampSP[idx[len(idx)//2:]])
#pl.xlabel('Hz')
#pl.ylabel('Amplitude')
#pl.subplot(212)
#pl.plot(freqs[idx[len(idx)//2:]], phaseSP[idx[len(idx)//2:]])
#pl.xlabel('Hz')
#pl.ylabel('rad')


# use scipy fft
T=1.0/framerate
N=nframes//2
x=np.linspace(0.0, N*T,N)
yf=fft(wave_data[0])
xf =np.linspace(0.0,1.0/(2.0*T),N)
pl.grid()
pl.xscale("symlog")
pl.plot(xf,2.0/N * np.abs(yf[0:N]))
pl.show()
