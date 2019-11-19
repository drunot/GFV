%%  
clear
close all

%Load EKG signal
load('ECG_noisy_signal.mat')

%tids akse
L = length(signal_QRS);
N= L/f_sample;
T = [0:(1/f_sample):N-1/f_sample];
f = f_sample*(0:(L/2))/L;
grafNr = 0;

%% Filter DC
meanDC = mean(signal_QRS);
signal_noDC = signal_QRS - meanDC;

%% FIR1 Lowpass
LPfir = fir1(100, 0.005, 'low', hann(101));
signal_Lowpass = filter(LPfir,1,signal_noDC);

%% IIR Cheby2 Highpass
Wp = 1/(f_sample/2);                                               % Stopband Frequency (Normalised)
Ws = 0.1/(f_sample/2);                                               % Passband Frequency (Normalised)
Rp =   1;                                                   % Passband Ripple (dB)
Rs =  50;                                                   % Stopband Ripple (dB)
[n,Ws] = cheb2ord(Wp,Ws,Rp,Rs);                             % Filter Order
[b,a] = cheby2(n,Rs,Ws,'high');                           % Filter Design, Sepcify Bandstop
signal_Highpass = filter(b, a, signal_Lowpass);

%% Plotter grafen i alle stadier af filtering
close all;
figNr = 1;
figure(figNr); figNr = figNr + 1; clf;
plot(T,signal_QRS)
title('EKG signal ufilteret')
xlabel("Tid [s]");
ylabel("Amplitude");
legend("EKG signal raw");
figure(figNr); figNr = figNr + 1; clf;
plot(T,signal_noDC)
title('1. filtering: Fjern DC')
xlabel("Tid [s]");
ylabel("Amplitude");
legend("EKG signal uden DC");
figure(figNr); figNr = figNr + 1; clf;
plot(T,signal_Lowpass)
title('2. filtering: Fir lowpass')
xlabel("Tid [s]");
ylabel("Amplitude");
legend("EKG signal uden DC med lowpass filter");
figure(figNr); figNr = figNr + 1; clf;
plot(T,signal_Highpass)
title('3. filtering: IIR Cheby2 filter highpass')
xlabel("Tid [s]");
ylabel("Amplitude");
legend("EKG signal uden DC med lowpass - og highpass filter");

%% Plotter alt FFT i log akse
fft_QRS = fft(signal_QRS);
fft_noDC = fft(signal_noDC);
fft_Lowpass = fft(signal_Lowpass);
fft_Highpass = fft(signal_Highpass);
figure(figNr); figNr = figNr + 1; clf;
semilogx(f(1:L/2),abs(fft_QRS(1:L/2)));
title('EKG signal ufilteret')
xlabel("Frekvens [Hz]");
ylabel("Amplitude");
legend("EKG signal raw");
figure(figNr); figNr = figNr + 1; clf;
semilogx(f(1:L/2),abs(fft_noDC(1:L/2)));
title('1. filtering: Fjern DC')
xlabel("Frekvens [Hz]");
ylabel("Amplitude");
legend("EKG signal uden DC");
figure(figNr); figNr = figNr + 1; clf;
semilogx(f(1:L/2),abs(fft_Lowpass(1:L/2)));
title('2. filtering: Fir lowpass')
xlabel("Frekvens [Hz]");
ylabel("Amplitude");
legend("EKG signal uden DC med lowpass filter");
figure(figNr); figNr = figNr + 1; clf;
semilogx(f(1:L/2),abs(fft_Highpass(1:L/2)));
title('3. filtering: IIR Cheby2 filter highpass')
xlabel("Frekvens [Hz]");
ylabel("Amplitude");
legend("EKG signal uden DC med lowpass - og highpass filter");

%% Plottet af FFT for de to f�rst hvor man kan se at DC er v�k.
figure(figNr); figNr = figNr + 1; clf;
plot(f(1:L/2),abs(fft_QRS(1:L/2)));
title('EKG signal ufilteret')
xlabel("Frekvens [Hz]");
ylabel("Amplitude");
legend("EKG signal raw");
figure(figNr); figNr = figNr + 1; clf;
plot(f(1:L/2),abs(fft_noDC(1:L/2)));
title('1. filtering: Fjern DC')
xlabel("Frekvens [Hz]");
ylabel("Amplitude");
legend("EKG signal uden DC");
%% Filterkarakteristiker for begge filte.
figure(figNr); figNr = figNr + 1; clf;
freqz(LPfir,1,2^14,f_sample);
title('FIR lowpass filter')
figure(figNr); figNr = figNr + 1; clf;
freqz(b,a,2^14,f_sample);
title('IIR Cheby2 filter highpass')