%%
clc
close all
clear

data = readtable('capture6.txt');

newdata = table2array(data);

time = height(data);

t = [0:1/3:(time-2)/3];

riseTimeLow = find(newdata(2:end, 2) > ((0.1 * 20) + 30), 1) / 3;
riseTimeHigh = find(newdata(2:end, 2) > ((0.9 * 20) + 30), 1) / 3;
riseTime = (riseTimeHigh - riseTimeLow); 

overShoot = (max(newdata(2:end, 2)) - 50) / 30 * 100;

figure(1); clf;
hold on
plot(t,newdata(2:end,1:3));
legend('TargetTemp','CurrTemp','Difference');
title("Temperatur m�ling. Kp = 2, Ki = 1/30, Kd = 0");
xlabel("Tid [s]");
ylabel("Amplitude [deg]");
xline(riseTimeLow);
xline(riseTimeHigh);
hold off

figure(2); clf;
plot(t,newdata(2:end,4:end));
legend('PWM','Partial','Integral','Differential');
title("PWM m�ling. Kp = 2, Ki = 1/30, Kd = 0");
xlabel("Tid [s]");
ylabel("Amplitude [%duty]");
%%