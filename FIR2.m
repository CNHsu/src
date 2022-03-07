close all;
clear all;
clf;


f1 = 11200;
f2 = 15000;
delta_f = f2-f1;
Fs = 192000;
dB  = 30;
N = dB*Fs/(22*delta_f);

f =  [f1 ]/(Fs/2)
hc = fir1(round(N)-1, f,'low')
x=(-0.5:1/4096:0.5-1/4096)*Fs;
y=20*log10(abs(fftshift(fft(hc,4096))));
figure
subplot(211)
plot(x,y)
axis([0 20000 -60 20])
title('Filter Frequency Response')
subplot(212)
plot(0:1:N,hc,'color','red')
grid on
%quantized coefficients error
%8-bit case
h_q1 = floor(hc*2^8)/2^8;
q1 = h_q1 - hc;
figure
subplot(211)
plot(q1)
title('Quantization Error')
subplot(212)
plot((-0.5:1/4096:0.5-1/4096)*Fs,20*log10(abs(fftshift(fft(q1,4096)))))
title('Quantization Error Spectrum')

figure
plot((-0.5:1/4096:0.5-1/4096)*Fs,20*log10(abs(fftshift(fft(hc,4096)))))
hold on
plot((-0.5:1/4096:0.5-1/4096)*Fs,20*log10(abs(fftshift(fft(h_q1,4096)))),'color','r')
grid on
axis([-Fs/2 Fs/2 -140 5])
title('Frequency Spectrum - Blue(Octave), Red(8-Bit Quantized)')

%% to min quantized error
%%%%%
h_scl = max(hc); %this is the key to min quantized error
%%%%%

h_q2 = floor((hc/h_scl)*2^8)/2^8;
q2 = h_scl *h_q2 - hc;

h_q2 = h_q2/sum(h_q2);
figure
subplot(211)
plot(q1)
title('Quantization Error')
subplot(212)
plot((-0.5:1/4096:0.5-1/4096)*Fs,20*log10(abs(fftshift(fft(q2,4096)))))
title('Quantization Error Spectrum')

figure
plot((-0.5:1/4096:0.5-1/4096)*Fs,20*log10(abs(fftshift(fft(hc,4096)))))
hold on
plot((-0.5:1/4096:0.5-1/4096)*Fs,20*log10(abs(fftshift(fft(h_q2,4096)))),'color','r')
grid on
axis([-Fs/2 Fs/2 -140 5])
title('Frequency Spectrum W/ Scaling - Blue(Octave), Red(8-Bit Quantized)')