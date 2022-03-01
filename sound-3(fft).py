#CNHsu 2019/6/28
# fft test
#
#
import matplotlib.pyplot as plt
from scipy.fftpack import fft
from scipy.io import wavfile # get the api
import numpy as np

fs, data = wavfile.read('0_16.wav') # load the data

print ("Frequency sampling", fs)
l_audio = len(data.shape)
print ("Channels", l_audio)
if l_audio == 2:
    data = data.sum(axis=1) / 2
N = data.shape[0]
NN=int(N/2)
print ("Complete Samplings N", N)
secs = N / float(fs)
print ("secs", secs)


fft_out=fft(data)
plt.plot(data, np.abs(fft_out))
plt.show()