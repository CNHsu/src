
B=[1 0 0 0.125];
A=[1 0 0 0 0 0.9^5];

[r,p,f]=residuez(B,A)
[sos,g]=tf2sos(B,A)

Fs=100;
f=0:Fs/2;
w=f*(2*pi/Fs);
H=freqz(B,A,w);
figure(1);
freqz_plot(w,H);
figure(2);
[x,t]=impz(B,A);
plot(t,x);