%%
clc
close all
clear

data = readtable('capture17.txt');

newdata = table2array(data);

time = height(data);

t = [0:1/3:(time-2)/3];

figure(1); clf;
plot(t,newdata(2:end,1:3));
legend('TargetTemp','CurrTemp','Difference');
title("Temperatur måling. Kp = 10, Ki = 3/30, Kd = 1");
xlabel("Tid [s]");
ylabel("Amplitude [deg]");

figure(2); clf;
plot(t,newdata(2:end,4:end));
legend('PWM','Partial','Integral','Differential');
title("PWM måling. Kp = 10, Ki = 3/30, Kd = 1");
xlabel("Tid [s]");
ylabel("Amplitude [%duty]");
%%