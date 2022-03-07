close all;
clear all;
clf
f1 = 1000;
f2 = 1500;
delta_f = f2-f1;
Fs = 48000;
dB  = 30;
order=5;
t=0:1/Fs:0.005;
N = dB*Fs/(22*delta_f)
f =  [f1 ]/(Fs/2)
hc = fir1(round(N)-1, f,'low')
figure
plot((-0.5:1/4096:0.5-1/4096)*Fs,20*log10(abs(fftshift(fft(hc,4096)))))
axis([0 20000 -60 20])
title('Filter Frequency Response')
grid on

x = sin(2*pi*[1:1000]*5000/Fs) +  sin(2*pi*[1:1000]*2000/Fs) + sin(2*pi*[1:1000]*13000/Fs)  + sin(2*pi*[1:1000]*18000/Fs);

sig = 20*log10(abs(fftshift(fft(x,4096))));
xf = filter(hc,1,x)
figure
subplot(211)
plot(x)
title('Sinusoid with frequency components 2000, 5000, 13000, and 18000 Hz')
subplot(212)
plot(xf)
title('Filtered Signal')
xlabel('time')
ylabel('amplitude')
x= (x/sum(x))/20
sig = 20*log10(abs(fftshift(fft(x,4096))));
xf = filter(hc,1,x)

figure
subplot(211)
plot((-0.5:1/4096:0.5-1/4096)*Fs,sig)
hold on
plot((-0.5:1/4096:0.5-1/4096)*Fs,20*log10(abs(fftshift(fft(hc,4096)))),'color','r')
hold off
axis([0 20000 -60 10])
title('Input to filter - 4 Sinusoids')
grid on
subplot(212)
plot((-0.5:1/4096:0.5-1/4096)*Fs,20*log10(abs(fftshift(fft(xf,4096)))))
axis([0 20000 -60 10])
title('Output from filter')
xlabel('Hz')
ylabel('dB')
grid on
