g1=0.5^3;
B=[1 0 0 g1];
g2=0.9^5
A=[1 0 0 0 0 g2];
Nfft=1024; %fft size
Nspec =1+Nfft/2; % show positive 
f=[0:Nspec-1]/Nfft; %freq. axis
Xnum=fft(B,Nfft); % freq. response of FIR
Xden=fft(A,Nfft); % freq. response, feedback part
X =Xnum./Xden;
clf;
figure(1);
plot(f,X(1:Nspec));

[zeros, poles, gain]=tf2zp(B,A);
zplane(zeros,poles);
zeros
poles