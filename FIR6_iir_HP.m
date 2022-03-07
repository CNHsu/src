close all;
clear all;
clf;


f1 = 4000; %stop
f2 = 13000;%pass
delta_f = f2-f1;% transition bandwitdh
Fs = 44100; %sample rate 192K
dB  = 30; %stop band attenuation
N = dB*Fs/(22*delta_f); % filter length
Rpass= 3 %pass band ripple
Rstop= 40 %stop band ripple
Wpass=2/Fs*f2;
Wstop=2/Fs*f1;

f =  [f1 ]/(Fs/2)
% use ellip to design the filter
%hc = fir1(round(N)-1, f,'low')
[n,Wn]=ellipord(Wpass, Wstop,Rpass,Rstop); % get the minimum order n
n
[b,a]=ellip(n,Rpass, Rstop, Wn,"high");
 f = 0:Fs/2;
 W = f * (2 * pi / Fs);
 H = freqz (b, a, W);
 %semilogx (f, 20 * log10 (abs (H)))
 plot (f, 20 * log10 (abs (H)));
 outline_lp_pass_x = [f2(1)  , f2(1), max(f)];
 outline_lp_pass_y = [-80,-Rpass, -Rpass ];
 outline_lp_stop_x = [min(f)  , f1(1), f1(1), max(f)];
 outline_lp_stop_y = [ -Rstop  , -Rstop,0,0];
 hold on
 plot (outline_lp_pass_x, outline_lp_pass_y, "m", outline_lp_stop_x, outline_lp_stop_y, "m");
ylim ([-80, 0],'color','red');
 grid on
 xlabel ("Frequency (Hz)");
 ylabel ("Attenuation (dB)");
 title ("2nd order digital elliptical high-pass (without margin)");




