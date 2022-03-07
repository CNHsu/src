[B,A] = ellip(4,1,20,0.5); % Design the lowpass filter
B1 = B .* (0.95).^[1:length(B)]; % contract zeros by 0.9
[H,w] = freqz(B,A);        % Compute its frequency response
[H1,w1]= freqz(B1,A);

% Plot the frequency response H(w):
%
clf
grid on
figure(1);
plot(w,abs(H));
hold on
plot(w1,abs(H1));
figure(2);
plot(w,angle(H));
hold on
plot(w1,angle(H1));
hold on
unwrap(angle(H));
hold on
unwrap(angle(H1));
% Plot frequency response in a "multiplot" like Matlab uses:
%
figure(3);
plot(H,w/(2*pi));
