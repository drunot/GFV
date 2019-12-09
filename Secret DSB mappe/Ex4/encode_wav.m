%% Loading af lyd

function[Compression, Size16bit, SizeEncoded] = encode_wav(filename)

%%Faa info fra filen
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


%% DFT af signal og fjern alt data over 20 kHz (maksimum hoerbar frekvens)

DFT = fft(input);
f = fs*(0:(L/2))/L;
stopPoint = find(f>20000, 1);
DFT = cat(1, DFT(1:stopPoint,:), zeros(L-stopPoint, 2));
f = cat(2, f(1:stopPoint), zeros(1, L-stopPoint));

%% Kvantisering til 16 bits

DFT16bit = quantise_n_bits(DFT, 16);
IFT = ifft(DFT16bit);
encoded_file = sprintf("16bit_%s", filename); 
audiowrite(encoded_file, real(IFT), fs);
bitDepth = 16;
Size16bit = bitDepth * L * channels; %fil stoerelse

%% Adskilning af signal

point100Hz = find(f>100,1);
point500Hz = find(f>500,1);
point1500Hz = find(f>1500,1);
point3000Hz = find(f>3000,1);
point6000Hz = find(f>6000,1);
point10000Hz = find(f>10000,1);


p0_100Hz = DFT16bit(1:point100Hz,:);
p101_500Hz = DFT16bit(point100Hz+1:point500Hz,:);
p501_1500Hz = DFT16bit(point500Hz+1:point1500Hz,:);
p1501_3000Hz = DFT16bit(point1500Hz+1:point3000Hz,:);
p3001_6000Hz = DFT16bit(point3000Hz+1:point6000Hz,:);
p6001_10000Hz = DFT16bit(point6000Hz+1:point10000Hz,:);
p10001_20000Hz = DFT16bit(point10000Hz+1:stopPoint,:);


%% Kvantisering af dele
%til 4 bit, 0-100 og 10001-20000 Hz
kp0_100Hz = quantise_n_bits(p0_100Hz, 4);
kp10001_20000Hz = quantise_n_bits(p10001_20000Hz, 4);
%til 6 bit, 101-500 og 6001-10000 Hz
kp101_500Hz = quantise_n_bits(p101_500Hz, 6);
kp6001_10000Hz = quantise_n_bits(p6001_10000Hz, 6);
%til 12 bit, 501-1500 og 3001-6000Hz
kp501_1500Hz = quantise_n_bits(p501_1500Hz, 12);
kp3001_6000Hz = quantise_n_bits(p3001_6000Hz, 12);
%ubehandlet 1501-3000 Hz
kp1501_3000Hz = p1501_3000Hz;

%% sammensaetning
encoded = cat(1, kp0_100Hz, kp101_500Hz, kp501_1500Hz, kp1501_3000Hz, kp3001_6000Hz, kp6001_10000Hz, kp10001_20000Hz, zeros(L-stopPoint, 2));

IFT = ifft(encoded);
encoded_file = sprintf("encoded_%s", filename);
audiowrite(encoded_file, real(IFT), fs);

% beregn samlede bitlaengde
length4bit = length(kp0_100Hz) + length(kp10001_20000Hz);
length6bit = length(kp101_500Hz) + length(kp6001_10000Hz);
length12bit = length(kp501_1500Hz) + length(kp3001_6000Hz);
length16bit = length(kp1501_3000Hz);
SizeEncoded = channels * (4 * length4bit + 6 * length6bit + 12 * length12bit + 16 * length16bit);   %Fil storelse efter komprimering
Compression = ((Size16bit-SizeEncoded) / Size16bit) * 100;  %Kompresionsrate i procent

