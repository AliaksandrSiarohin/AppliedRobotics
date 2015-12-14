s = poly(0, 's');
//Time
Dt = 0.001; //time intervall
t = 0:Dt:2;
Kg = 0.1752; 
xi = 1;
o_n = 10.52;


//Plat transfer function

LM = Kg/(s ^ 2 / o_n ^ 2 + 2*xi/o_n * s + 1);
roots((s ^ 2 / o_n ^ 2 + 2*xi/o_n * s + 1))
G = syslin('c', LM);
y = csim('step', t, G);

//Controller 20% overshot, 0.2 - 0.4 setling time
Kc = 10;
C = (s+10)*(s+10)/(s*(s + 21));
//Kc = 2;
//C = (s+10)/s;

//Closed loop
Gc1 = Kc * C * G / (1 + Kc * C * G);
Gc1 = syslin('c', Gc1);
yo1 = csim('step', t, Gc1);


//Plot
scf(1);
clf;
//plot(t, y);
plot(t, yo1, 'r--');
xlabel("time (s)");
ylabel("speed (rad/s)");
legend('Responce to 1(t)');
//Root locus
scf(2);
clf;
evans(C * LM, Kc);
max_overshot = 0.2;
max_settling_time = 0.4;

alpha = 0.05;
oc = -log(0.2)/%pi
sl = log(alpha)/max_settling_time;

plot([sl,-15], [sl/oc,-15/oc], 'r--');
plot([sl,-15], [-sl/oc, 15/oc], 'r--');
plot([sl, sl], [-sl/oc, sl/oc], 'r--');

//Save plot to pdf
xs2pdf(1, "./response.pdf");
xs2pdf(2, "./root_locus.pdf");

//LineSpec and GlobalProperty examples:
//plot([t ;t],[sin(t) ;cos(t)], 'blue');//,'marker','d','markerfac','green','markeredg','yel')

