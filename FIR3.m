close all;
clear all;
clf;


f1 = 11200; %low pass@11200Hz
f2 = 15000;%stop band @ 15K
delta_f = f2-f1;% transition bandwitdh
Fs = 192000; %sample rate 192K
dB  = 30; %stop band attenuation
N = dB*Fs/(22*delta_f); % filter length

order=5;
[b,a]=butter(order, 2*f1/Fs);
figure(1)
figure(1)
%%%%% option0
%below code same as this
%w=linspace(0,1,128);
w=logspace(-1,1,128);
freqs(b,a,w);
%%%%% option1
figure(2)
w=logspace(-1,1);
h=freqs(b,a,w);
mag=abs(h);
phase=angle(h);
phasedeg=phase*180/pi;
subplot(3,1,1)
loglog(w,mag)
grid on
xlabel('Freq.(rad/s)');
ylabel('Magnitude');
subplot(3,1,2)
semilogx(w,phasedeg);
grid on
xlabel('Freq. (rad/s)');
ylabel('Phase(degrees)');
subplot(3,1,3);
zplane(b,a)

figure(3)
w=linspace(0,4,128);
freqz(b,a,128,20000)% freqz(b,a,number,freq), freq for plot 
