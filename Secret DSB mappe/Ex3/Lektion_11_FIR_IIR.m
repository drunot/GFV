%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 3.11 Konstruer et båndpasfilter efter vindues- %
% metoden, med knækfrekvenser 1,2 kHz og 3,6 kHz %
% (fs = 48 kHz).                                 %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear;
clc;

fs = 48000;
f_cutoff_l = 1200;
f_cutoff_h = 3600;
M = 300;

f_opl = fs/M;

f_bin_l = f_cutoff_l / f_opl;
f_bin_h = f_cutoff_h /f_opl;
f_bin_rl = round(f_bin_l);
f_bin_rh = round(f_bin_h);

H_left  = [0 zeros(1,f_bin_rl) ones(1,f_bin_rh-f_bin_rl) zeros(1,(M/2)-(f_bin_rh))];
H_right = fliplr(H_left(2:end));

H = [H_left H_right];

%% View

figure(1001); clf
stem(H)
hold on;
xlabel('Filterkoefficienter')
hold off;

%% Invers fft

h = fftshift(real(ifft(H)));
figure(1); clf
plot(h,'b','linewidth',2)
xlabel('Filterkoefficienter')
axis([1 M+1 -inf inf])
hold on;
title('Impulsrespons')

%% Hanning
w_hanning = hanning(M+1)';
h_win = h.*w_hanning;
plot(w_hanning*max(abs(h)),'g','linewidth',2)
hold on;
plot(h_win,'r','linewidth',2);
hold off;

%% Steprespons
f_uhan = fft(h,fs);
f_mhan = fft(h_win,fs);
figure(2); clf
f_vek = (0:fs-1);
plot(f_vek(1:fs/2), abs(f_uhan(1:fs/2)))

hold on;
plot(f_vek(1:fs/2), abs(f_mhan(1:fs/2)))
%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 3.12 Design i Matlab et høj- og et lavpas FIR  %
% filter med knækfrekvens 850 Hz og samplings-   %
% frekvensen 44,1 kHz. Lav fire versioner af     %
% hver, én med orden 24, en med 72, én med 192   %
% og én med orden 512. Plot impulsresponsen samt %
% overføringskarakteristikken (brug gerne freqz  %
% .m) for alle filtre – og diskuter.             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Clear og definer
clc;
clear;

fs = 44100;
f_cutoff = 850;

%% Selve graferne LP
M1 = 24;
M2 = 72;
M3 = 192;
M4 = 512;
b_LP24  = fir1(M1, f_cutoff/(0.5*fs));
b_LP72  = fir1(M2, f_cutoff/(0.5*fs));
b_LP192 = fir1(M3, f_cutoff/(0.5*fs));
b_LP512 = fir1(M4, f_cutoff/(0.5*fs));

figure(1); clf
subplot(4,1,1)
plot(b_LP24)
subplot(4,1,2)
plot(b_LP72,'r')
subplot(4,1,3)
plot(b_LP192,'g')
subplot(4,1,4)
plot(b_LP512,'k')
%% Bodeplot LP


H_LP24  = freqz(b_LP24,1);
H_LP72  = freqz(b_LP72,1);
H_LP192 = freqz(b_LP192,1);
H_LP512 = freqz(b_LP512,1);
plot(20*log10(abs(H_LP24)))
hold on
plot(20*log10(abs(H_LP72)),'r')
plot(20*log10(abs(H_LP192)),'g')
plot(20*log10(abs(H_LP512)),'k')

%% Selve graferne HP

b_HP24  = fir1(M1, f_cutoff/(0.5*fs),'high');
b_HP72  = fir1(M2, f_cutoff/(0.5*fs),'high');
b_HP192 = fir1(M3, f_cutoff/(0.5*fs),'high');
b_HP512 = fir1(M4, f_cutoff/(0.5*fs),'high');

figure(3); clf
subplot(4,1,1)
plot(b_HP24)
subplot(4,1,2)
plot(b_HP72,'r')
subplot(4,1,3)
plot(b_HP192,'g')
subplot(4,1,4)
plot(b_HP512,'k')

%%  Bodeplot HP

figure(4); clf
H_HP24  = freqz(b_HP24,1);
H_HP72  = freqz(b_HP72,1);
H_HP192 = freqz(b_HP192,1);
H_HP512 = freqz(b_HP512,1);
plot(20*log10(abs(H_HP24)))
hold on
plot(20*log10(abs(H_HP72)),'r')
plot(20*log10(abs(H_HP192)),'g')
plot(20*log10(abs(H_HP512)),'k')

%% Diskussion
% Jo højere orden, jo bedre et filter har man. Men til gengæld fylder
% det mere og er mere tungt for CPUen at bruge.

%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 3.13 Design et 6. ordens båndpas Butterworth   %
% IIR filter. Pasbånd mellem 3kHz og 4kHz, og fs %
% = 24 kHz. Plot og diskuter filterets fase-     %
% karakteristik. Bestem filterets group delay    %
% (GPD). Plot og diskuter: Hvad sker der ved     %
% filtrerets knækfrekvenser? Hvad er max GPD?    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clc
clear

f_c_l = 3000;%Low cutoff
f_c_h = 4000;%Heigh cutoff

fs = 24000; %Sample freq.
n = 6; %Filteret orden

[b,a] = butter(n,[f_c_l/(fs/2),f_c_h/(fs/2)],'bandpass');

figure(1); clf;
stem(a);
title('Denominator koefficienter (a) for butterworthfilteret');
legend('Koefficienter');
figure(2); clf;
stem(b, 'r');
title('Numernator koefficienter (b) for butterworthfilteret');
legend('Koefficienter');
%% frekvens karakteristikken:

figure(3); clf
freqz(b,a,512,fs);
title('Frekvenskarakteristik og faseforskydning')
legend('Frekvenskarakteristik')
subplot(2,1,2)
legend('Faseforskydning')

%Manuel plot af freqz

%[h,f]=freqz(b,a,512,fs);
%figure(100); clf
%subplot(2,1,1)
%plot(f,20*log(abs(h)));
%title('frekvenskarakteristik og faseforskydning')
%legend('frekvenskarakteristik')
%subplot(2,1,2)
%plot(f,360/(2*pi)*unwrap(angle(h)));
%legend('faseforskydning');

%% GPD
figure(4); clf;
[gd,f] = grpdelay(b,a,512,fs);
plot(f,gd/fs*1000); %Divison med fs*1000 for at få resultatet i ms.
hold on;
[M,I] = max(gd/fs*1000);
plot(f(I),M,'o');
title('Group delay (GPD)');
legend('Group delay (GPD)', 'Max GPD delay');
ylabel('Group delay (miliseconds)')
xlabel('Frequency (Hz)')

%% Diskussion
%
%Group-delay er forsinkelse pr. frekvens. Det vil sige, at alt efter
%hvilken frekvens der kommer ind i systemet, så vil der være et forskelligt
%delay for hvor længe det tager for signalet at bevæge sig igennem
%filteret.
%Dette kan resulterer i uklarheder i filteret. Efter som dette filter ikke
%er lineært i sit pasbånd, vil dette ikke være et optimalt filter, hvis
%delayet er vigtigt for signalet. Ligeledes kan man også se i
%fasekarakteristikken at der sker et ret stort fasedrej i pasbåndet, dette 
%vil ligeledes vise at der inde for pasbåndet sker en stor ændrig af
%hvornår de forskellige frekvenser kommer igennem filteret.
%Max GPD er 2.3073

%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Opgave 3.14: Givet det analoge shelving filter %
% beskrevet som Laplace overføringsfunktion      %
% nedenfor:                                      %
%                      s + ?_1                   %
%               H(s) = -------                   %
%                      s + ?_2                   %
% a) Bestem vha. bilineær transformation         %
% (substitution) den ækvivalente                 %
% overføringsfunktion H(z)                       %
% b) Udregn henholdsvis b og a koefficienterne   %
% for H(z). Sampletiden Ts sættes til 1 ms,      %
% ?1 = 565 og ?2 = 942.                          %
% c) Vis frekvensresponsen for H(z) vha.         % 
% freqz.m i Matlab                               %
% d) Er H(z) filteret stabilt? Hvorfor /         %
% hvorfor ikke.                                  %
% e) Find differensligningen der repræsenterer   %
% H(z), altså y(n) = ...                         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% a) Bestem vha. bilineær transformation (substitution) den ækvivalente 
% overføringsfunktion H(z)
% Se mathcad dokumentet 'Lektion_11_FIR_IIR.mcdx'

% b)Udregn henholdsvis b og a koefficienterne for H(z). Sampletiden Ts 
% sættes til 1 ms, ?1 = 565 og ?2 = 942.
% Se mathcad dokumentet 'Lektion_11_FIR_IIR.mcdx'

% c) Vis frekvensresponsen for H(z) vha. freqz.m i Matlab.

% fra mathcad dokumentet 'Lektion_11_FIR_IIR.mcdx' ved vi:

clc
clear
a=[2942; -1058];
b=[2565; -1435];
ts=1*10^(-3);
fs=1/ts;

figure(3); clf
freqz(b,a,512,fs);
title('Frekvenskarakteristik og faseforskydning')
legend('Frekvenskarakteristik')
subplot(2,1,2)
legend('Faseforskydning')

% d) Er H(z) filteret stabilt? Hvorfor / hvorfor ikke.

% Det er stabilt. Se mathcad dokumentet 'Lektion_11_FIR_IIR.mcdx'.

% e) Find differensligningen der repræsenterer H(z), altså y(n)

% Se mathcad dokumentet 'Lektion_11_FIR_IIR.mcdx'
