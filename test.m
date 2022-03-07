fs=48000;
fpass=2400;
fstop=9600;
t=0:1/fs:0.005;
signal=sin(2*pi*fpass*t);
figure(1)
plot(t,signal);