%%
clc
close all
clear

data = readtable('capture17.txt');
labelstr = ("Kp = 10, Ki = 3/30, Kd = 1");

newdata = table2array(data);

time = height(data);

t = [0:1/3:(time-2)/3];

riseTimeLow = find(newdata(2:end, 2) > ((0.1 * 20) + 30), 1) / 3;
riseTimeHigh = find(newdata(2:end, 2) > ((0.9 * 20) + 30), 1) / 3;
riseTime = riseTimeHigh - riseTimeLow;

overShoot = (max(newdata(2:end, 2)) - 50) / 30 * 100;

S = stepinfo(newdata(2:end, 2), 'SettlingTimeThreshold', 0.02); 
settlingTime = (S.SettlingTime / 3) - (find(newdata(2:end, 1) == 50, 1) / 3);

figure(1); clf;
hold on
plot(t,newdata(2:end,1:3));
xline(riseTimeLow, '--r');
xline(riseTimeHigh, '--b');
xline(S.SettlingTime / 3, '--g');
legend('TargetTemp','CurrTemp','Difference', '10%', '90%', "<2% err");
str = sprintf("Temperatur måling. %s", labelstr);
title(str);
xlabel("Tid [s]");
ylabel("Amplitude [deg]");
dim = [.6 .2 .3 .3];
str1 = sprintf('%s: %.2fs,', 'Rise time', riseTime);
str2 = sprintf('%s: %.2f%', '%Overshoot', overShoot);
str3 = sprintf('%s: %.2fs', 'Settling time', settlingTime);
str = {str1, str2, str3};
annotation('textbox', dim, 'String', str, 'FitBoxToText', 'on');
hold off

figure(2); clf;
plot(t,newdata(2:end,4:end));
legend('PWM','Partial','Integral','Differential');
str = sprintf("PWM måling. %s", labelstr);
title(str);
xlabel("Tid [s]");
ylabel("Amplitude [%duty]");
%%