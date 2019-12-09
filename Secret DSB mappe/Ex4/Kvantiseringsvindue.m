

filename = 'korobeiniki.wav';
info = audioinfo(filename);
[input, fs] = audioread(filename);
bitDepth = info.BitsPerSample;
channels = info.NumChannels;

%%Lav akser til beregning
T = 1/fs;
L = length(input);
t = (0:L-1)*T;

%%Check sample raten
if (fs < 40001)
    disp('Sample Rate too low');
    return
end

DFT = fft(input);
f = fs*(0:(L/2))/L;

s1 = find(f>100,1);
s2 = find(f>500,1);
s3 = find(f>1500,1);
s4 = find(f>3000,1);
s5 = find(f>6000,1);
s6 = find(f>10000,1);
s7 = find(f>20000, 1);

f = cat(2, f(1:s7), zeros(1, L-s7));


K1 = ones(1,100)*4;
K2 = ones(1,500-101)*6;
K3 = ones(1,1500-501)*12;
K4 = ones(1,3000-1501)*16;
K5 = ones(1,6000-3001)*12;
K6 = ones(1,10000-6001)*6;
K7 = ones(1,20000-10001)*4;

vindue = [K1, K2, K3, K4, K5, K6, K7, 0];

figure('visible','on');
plot(vindue);
xlabel('Frekvens [Hz]');
ylabel('kvantisering [bits]');
legend('Kvantiseringskarakteristik');
title('Kvantiseringsintervaller');
