# 2019/6/28 CNHsu
#https://stackoverflow.com/questions/23377665/python-scipy-fft-wav-files
# 
import matplotlib.pyplot as plt
from scipy.fftpack import fft
import scipy.io.wavfile as wavfile
import scipy
import numpy as np

tupleWav = wavfile.read('file.wav')
sp =np.fft.fft(tupleWav[1])
ampSP = np.abs(sp)
phaseSP = np.unwrap(np.angle(sp))
time_step = 1./tupleWav[0]
freqs = np.fft.fftfreq(sp.size, time_step)
idx = np.argsort(freqs) # from negative to positive
plt.figure()
plt.subplot(211)
plt.plot(freqs[idx[len(idx)//2:]], ampSP[idx[len(idx)//2:]])
plt.xlabel('Hz')
plt.ylabel('Amplitude')
plt.subplot(212)
plt.plot(freqs[idx[len(idx)//2:]], phaseSP[idx[len(idx)//2:]])
plt.xlabel('Hz')
plt.ylabel('rad')
plt.show()