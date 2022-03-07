%https://ccrma.stanford.edu/~jos/filters/Sine_Wave_Analysis.html
% study the book
%y(n)= x(n)+x(n-1)
close
B=[1,1];
A=[1];
fs=4000; % sample
fpass=fs/2;
fstop=fs-fs/10;
N =20; % test number
T=1/fs;
fmax=fs/2;
df=fmax/(N-2); 
f=0:df:fmax;
tmax=1/df;
t=0:T:tmax;

s= 2+cos(2*pi*fpass*t)+sin(2*pi*2*fstop*t);

y=filter(B,A,s);
plot(t,s);
hold on;
plot(t,y);
##figure(1);
##%freqz_plot(W,H);
##
##semilogx(f:10:Fs/2,20*log(abs(H)));
##grid on;
##figure(2);
##semilogx(f,angle(abs(H)))
##grid on;
