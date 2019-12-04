%%
clc
close all
clear

data = readtable('1midterste.csv');

newdata = table2array(data(1:end-2,1:2));

newdata(:,1) = flipud(newdata(:,1));

plot(newdata(:,1),newdata(:,2));

hold on
plot(15,newdata(end-4,2),'ks');

TT = (newdata(1,2) - newdata(end,2))*0.66;
TT = TT + newdata(end,2);

plot(520, newdata(end-(520/5),2),'rS');

legend("M?ling 1 - midterste", "Tidsforsinkelse - 15 sekunder", "Termisk tidskonstant med forsinkelsen - 520 sekunder, 0,4823 V");

xlabel("Tid [s]");
ylabel("Sp?nding [V]");
title("M?ling 1 - midterste");

%%
%%
clc
close all
clear

data = readtable('1fjern.csv');


newdata = table2array(data(1:end-7,1:2));

newdata(:,1) = flipud(newdata(:,1));

plot(newdata(:,1),newdata(:,2));

hold on
plot(50,newdata(end-(50/5),2),'ks');

TT = (newdata(1,2) - newdata(end,2))*0.66;
TT = TT + newdata(end,2);

plot(605, newdata(end-(605/5),2),'rS');

legend("M?ling 1 - fjern", "Tidsforsinkelse - 50 sekunder", "Termisk tidskonstant med forsinkelsen - 605 sekunder, 0,4626 V");

xlabel("Tid [s]");
ylabel("Sp?nding [V]");
title("M?ling 1 - fjern");


%%
clc
close all
clear

data = readtable('2_3K3.csv');


newdata = table2array(data(1:end-(1195-860)/5,1:2));

newdata(:,1) = flipud(newdata(:,1));

plot(newdata(:,1),newdata(:,2));

hold on
plot(20,newdata(end-(20/5),2),'ks');

middel = mean(newdata(1:(120/5)+1,2));
yline(middel);

legend("M?ling 2 - 3K3", "Tidsforsinkelse - 20 sekunder", "Middel slutv?rid - 38,24C");

xlabel("Tid [s]");
ylabel("Sp?nding [V]");
title("M?ling 2 - 3K3");


%%
clc
close all
clear

data = readtable('2_82K.csv');


newdata = table2array(data(1:end-4,1:2));

newdata(:,1) = flipud(newdata(:,1));

plot(newdata(:,1),newdata(:,2));

hold on
plot(10,newdata(end-(10/5),2),'ks');

middel = mean(newdata(1:(120/5)+1,2));
yline(middel);

legend("M?ling 2 - 82K", "Tidsforsinkelse - 10 sekunder", "Middel slutv?rdi - 38,62C");

xlabel("Tid [s]");
ylabel("Sp?nding [V]");
title("M?ling 2 - 82K");

%%
clc
close all
clear

data = readtable('3_82K.csv');


newdata = table2array(data(1:end-6,1:2));

newdata(:,1) = flipud(newdata(:,1));

plot(newdata(:,1),newdata(:,2));

hold on
plot(25,newdata(end-(25/5),2),'ks');

middel = mean(newdata(1:(240/5)+1,2));
yline(middel);

legend("M?ling 3 - 82K", "Tidsforsinkelse - 25 sekunder", "Middel slutv?rdi - 38,91C");

xlabel("Tid [s]");
ylabel("Sp?nding [V]");
title("M?ling 3 - 82K");
%%