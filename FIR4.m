close all;
clear all;
clf;


f1 = 11200; %low pass@11200Hz
f2 = 15000;%stop band @ 15K
delta_f = f2-f1;% transition bandwitdh
Fs = 44100; %sample rate 192K
dB  = 30; %stop band attenuation
N = dB*Fs/(22*delta_f); % filter length
Rpass= 3 %pass band ripple
Rstop= 40 %stop band ripple
Wpass=2/Fs*f1;
Wstop=2/Fs*f2;

f =  [f1 ]/(Fs/2)
% use ellip to design the filter
%hc = fir1(round(N)-1, f,'low')
[n,Wn]=ellipord(Wpass, Wstop,Rpass,Rstop);
[b,a]=ellip(n,Rpass, Rstop, Wn);

##figure
##plot((-0.5:1/4096:0.5-1/4096)*Fs,20*log10(abs(fftshift(fft(hc,4096)))))
##axis([0 20000 -60 20])
##title('Filter Frequency Response')
##grid on
freqz(b,a);
figure;
[h,w]=freqz(b,a);
freqz_plot(w,abs(h));
freqz_plot(w,angle(h));



