%%
clc
close all
clear

data = readtable('capture1.txt');

newdata = table2array(data);

time = height(data);

t = [0:1/3:(time-1)/3];

figure(1); clf;
plot(t,newdata(:,1:3));
legend('TargetTemp','CurrTemp','Difference');
title("Temperatur m?lingerne. Kp = 2, Ki = 1/30, Kd = 0");
xlabel("Tid [s]");
ylabel("Amplitude [deg]");

figure(2); clf;
plot(t,newdata(:,4:end));
legend('PWM','Partial','Integral','Differential');
title("PWM m?lingerne. Kp = 2, Ki = 1/30, Kd = 0");
xlabel("Tid [s]");
ylabel("Amplitude [%duty]");
%%