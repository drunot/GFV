%%  
clear
close all

%Load EKG signal
load('ECG_noisy_signal.mat')

%tids akse
L = length(signal_QRS);
fs = f_sample;
N = L/fs;
T = [0:(1/fs):N-1/fs];
f = fs*(0:(L-1))/L;
fig = 1;
nyquist = fs/2;



%% plot signalet

figure(fig); fig = fig + 1; clf;
plot(T,signal_QRS);
freqSignal = fft(signal_QRS);
figure(fig); fig = fig + 1; clf;
semilogx(f(1:L/2),abs(freqSignal(1:L/2)))

%% Filter DC
meanDC = mean(signal_QRS);

signal_QRS = signal_QRS - meanDC;


%% highpass FIR

order = 250;
ffir = [0:1000]/1000;
cut = 30;
afir = [zeros([1,cut]) ones([1,1001-cut])];
LPfir2 = fir2(order, ffir, afir);
signal_QRS = filter(LPfir2, 1, signal_QRS);

figure(fig); fig = fig + 1; clf;
freqz(LPfir2,1,512,f_sample);

%% Highpass IIR 

meanDC = mean(signal_QRS);

signal_QRS = signal_QRS - meanDC;

[b,a] = besself(2, 1, 'high');
signal_QRS = filter(b, a, signal_QRS);




subplot(2,2,1)
plot(T,signal_QRS);
xlabel('tid [s]');
ylabel('amplitude');
title('Filtret signal');

freqSignal = fft(signal_QRS);
subplot(2,2,2)
semilogx(f(1:L/2),abs(freqSignal(1:L/2)))
xlabel('frekvens [Hz]');
ylabel('amplitude');
title('frekvenskaraktistik');

subplot(2,2,3)
[x,y] = freqz(b,a,512);
plot(y,abs(x));
xlabel('frekvens [Hz]');
ylabel('amplitude [dB]');
title('filterkaraktistik');

[b,a] = besself(3, 1, 'high');

subplot(2,2,4)
impz(b, a, 100, f_sample); 
xlabel('tid [s]');
ylabel('amplitude [dB]');
title('impulsrespons');

%% Lowpass FIR

order = 100;
LPfir = fir1(order, 0.005, 'low', hann(order+1));
signal_QRS = filter(LPfir, 1, signal_QRS);

figure(fig); fig = fig + 1; clf;
freqz(LPfir,1,512,f_sample);

%% Lowpass IIR

[b, a] = ellip(3, 5, 50, 50/nyquist);
signal_QRS = filter(b, a, signal_QRS);

figure(fig); fig = fig + 1; clf;
freqz(b,a,512,f_sample);

%% no 50 hz pls

[b, a] = ellip(2, 2, 25, [45/nyquist 55/nyquist], 'stop');
signal_QRS = filter(b, a, signal_QRS);

figure(fig); fig = fig + 1; clf;
freqz(b,a,512,f_sample);

%% Filter typer

%Gennem research er der fundet frem til at et standart EKG har en baandbrede
%mellem 0.05Hz og 100 Hz. De h?jeste frekvenser man m?ler er 1000 Hz


%Stoejtyper:
%DC
%H?jfrekvent
%drift
%50 HZ instr?ling
%mm

%Filtre der skal benyttes:
%FIR
%IIR
%Vindue (Hanning eller lignende)

%FIR2 funktionen
order = 100;


max_freq = 100;
min_freq = 1;

omega_max = max_freq/nyquist;
omega_min = min_freq/nyquist;

ffir2 = [0 omega_max omega_max 1];
afir2 = [1 1 0 0];

LPfir2 = fir2(order, ffir2, afir2);
figure(2); clf;
freqz(LPfir2,1,512,f_sample);

outlo2 = filter(LPfir2,1,signalNoDC);
figure(3); clf;
plot(T,outlo2);

%%

%FIR1 funktionen
LPfir = fir1(order, 0.005, 'low', hann(order+1));

figure(4); clf;
freqz(LPfir,1,512,fs);

outlo = filter(LPfir,1,signalNoDC);

figure(5); clf;
plot(T,outlo);


%FIR2 funktionen (HP)
ffir3 = [0 omega_min omega_min 1];
afir3 = [0 0 1 1];

HPfir2 = fir1(order, omega_min, 'high');
figure(6); clf;
freqz(HPfir2,1,512,f_sample);

outhi = filter(HPfir2,1,signalNoDC);
figure(7); clf;
plot(T, outhi);

outtest = filter(LPfir,1,outhi);
figure(8); clf;
plot(T,outtest);

etEllerAndet = fft(outtest, (L/2)+1);
figure(102); clf;
semilogx(f,abs(etEllerAndet))

%% IIR ALL THE WAY




[b,a] = cheby2(6,100,0.001,'high');


figure(9); clf;
freqz(b,a,512,fs)

figure(10); clf;
no50Hz = filter(b,a,outlo);
plot(T,no50Hz);
hold on
%plot(T,outlo);
etEllerAndet = fft(no50Hz, (L/2)+1);
figure(103); clf;
semilogx(f,abs(etEllerAndet))