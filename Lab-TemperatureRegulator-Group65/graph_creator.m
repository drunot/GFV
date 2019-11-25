%%
clc
close all
clear

data = readtable('capture10.txt');

newdata = table2array(data);

time = height(data);

t = [0:1/3:(time-2)/3];

figure(1); clf;
plot(t,newdata(2:end,1:3));
legend('TargetTemp','CurrTemp','Difference');
title("Temperatur m?lingerne. Kp = 20, Ki = 5/30, Kd = 1");
xlabel("Tid [s]");
ylabel("Amplitude [deg]");

figure(2); clf;
plot(t,newdata(2:end,4:end));
legend('PWM','Partial','Integral','Differential');
title("PWM m?lingerne. Kp = 20, Ki = 5/30, Kd = 1");
xlabel("Tid [s]");
ylabel("Amplitude [%duty]");
%%