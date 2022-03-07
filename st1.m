%https://ccrma.stanford.edu/~jos/filters/Sine_Wave_Analysis.html
% study the book
%y(n)= x(n)+x(n-1)
B=[1,1];
A=[1];
Fs=400; % sample
f1=100; % stop freq.
f2=200; % pass
bw=f2-f1; % band width
dB=20; %stop band attenuation
N=dB*Fs/(22*bw);%filter length
f=0:10:Fs/2;
w=f*(2*pi/Fs);
[H,W]=freqz(B,A,w);
figure(1);
%freqz_plot(W,H);

semilogx(f:10:Fs/2,20*log(abs(H)));
grid on;
figure(2);
semilogx(f,angle(abs(H)))
grid on;
