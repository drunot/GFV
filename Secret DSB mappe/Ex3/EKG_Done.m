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
%% Nuke 50 Hz
[b1, a1] = ellip(2, 2, 25, [45/(f_sample/2) 55/(f_sample/2)], 'stop');
signal_no50Hz = filter(b1, a1, signal_noDC);
%% FIR1 Lowpass 100 Hz
LPfir = fir1(100, 0.1, 'low', hann(101));
signal_Lowpass = filter(LPfir,1,signal_no50Hz);
%% FIR1 Lowpass 5 Hz
LPfir = fir1(100, 0.005, 'low', hann(101));
signal_Lowpass = filter(LPfir,1,signal_noDC);
%% IIR Cheby2 Highpass
Wp = 1/(f_sample/2);                                        % Passband Frequency (Normalised)
Ws = 0.01/(f_sample/2);                                      % Stopband Frequency (Normalised)
Rp =   1;                                                   % Passband Ripple (dB)
Rs = 50;                                                   % Stopband Ripple (dB)
[n,Ws] = ellipord(Wp,Ws,Rp,Rs);                             % Filter Order
[b,a] = ellip(n,Rp,Rs,Ws,'high');                             % Filter Design, Specify Bandstop
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

%% Plottet af FFT for de to først hvor man kan se at DC er væk.
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
legend('frekvenskarakteristik')
xlabel("Frekvens [Hz]");
ylabel("Magnetude [dB]");
subplot(2,1,2)
legend('faseforskydning');
xlabel("Frekvens [Hz]");
ylabel("Fase [grader]");
figure(figNr); figNr = figNr + 1; clf;
freqz(b,a,2^14,f_sample);
title('IIR Cheby2 filter highpass')
xlabel("Frekvens [Hz]");
ylabel("Magnetude [dB]");
legend('frekvenskarakteristik')
subplot(2,1,2)
legend('faseforskydning');
xlabel("Frekvens [Hz]");
ylabel("Fase [grader]");

%%
figure(figNr); figNr = figNr + 1; clf;
[h,f]=freqz(b,a,2^14,f_sample);
subplot(2,1,1)
semilogx(f,20*log(abs(h)));
legend('frekvenskarakteristik')
title('IIR Cheby2 filter highpass')
xlabel("Frekvens [Hz]");
ylabel("Magnetude [dB]");
subplot(2,1,2)
semilogx(f,360/(2*pi)*unwrap(angle(h)));
legend('faseforskydning');
xlabel("Frekvens [Hz]");
ylabel("Fase [grader]");