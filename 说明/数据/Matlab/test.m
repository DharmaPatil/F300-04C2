a = xlsread('F:\WORK\´Ôº£Ðñ\5kg.xls','c2:c1400');
b = xlsread('F:\WORK\´Ôº£Ðñ\10kg1.xls','c2:c4000');
c = xlsread('F:\WORK\´Ôº£Ðñ\5kg.xls','a2:a1400');
d = xlsread('F:\WORK\´Ôº£Ðñ\10kg1.xls','a2:a4000');
x = [2:1:1400];
plot(x,a,x,c*1000);
grid on;
