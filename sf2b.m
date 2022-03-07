% 2020/9/30
% dec to binary
%
a = 0.0315246582031250; % your float point number
n = 1;         % number bits for integer part of your number      
m = 16;         % number bits for fraction part of your number
% binary number
d2b = fix(rem(a*pow2(-(n-1):m),2)); 
% the inverse transformation
b2d = d2b*pow2(n-1:-1:-m).';
b2d
d2b