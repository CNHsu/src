close all;
clear all;
clf;


f1 = [9800 14126]; %low pass@11200Hz
f2 = [8500 15072];%stop band @ 15K
delta_f = f2-f1;% transition bandwitdh
Fs = 44100; %sample rate 192K
dB  = 30; %stop band attenuation
%N = dB*Fs/(22*delta_f); % no use in iir ,filter length
Rpass= 0.5 %pass band ripple
Rstop= 40 %stop band ripple
Wpass=2/Fs*f1;
Wstop=2/Fs*f2;

f =  [f1 ]/(Fs/2)
% use ellip to design the filter
%hc = fir1(round(N)-1, f,'low')
[n,Wn]=ellipord(Wpass, Wstop,Rpass,Rstop); % get the minimum order n
n
[b,a]=ellip(n,Rpass, Rstop, Wn,"stop");
 %f = 0:Fs/2;% for plot
  f = 5000:Fs/2; % for semilogx
 W = f * (2 * pi / Fs);
 H = freqz (b, a, W);

 semilogx (f, 20 * log10 (abs (H)))
 %plot (f, 20 * log10 (abs (H)));
 outline_lp_pass_x = [f1(1)  , f1(1), f1(2), f1(2)];
 outline_lp_pass_y = [-80, -Rpass, -Rpass  , -80];
 outline_lp_stop_x = [min(f)  , f2(1), f2(1),f2(2),f2(2), max(f)];
 outline_lp_stop_y = [-Rstop,-Rstop, 0     , 0       , -Rstop  , -Rstop];
 hold on
 plot (outline_lp_pass_x, outline_lp_pass_y, "m", outline_lp_stop_x, outline_lp_stop_y, "m");
 xlim([f(1),f(end)]);
 ylim ([-80, 0],'color','red');
 grid on
 xlabel ("Frequency (Hz)");
 ylabel ("Attenuation (dB)");
 title ("2nd order digital elliptical low-pass (without margin)");




