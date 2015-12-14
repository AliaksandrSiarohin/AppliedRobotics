s = poly(0, 's');
//Time
Dt = 0.001; //time intervall
t = 0:Dt:2;

//Parameters from system identification part
Kg = 0.1752; //q in slides
xi = 1;
o_n = 10.52;


//Plat transfer function, look in 6 lecture.

LM = Kg/(s ^ 2 / o_n ^ 2 + 2*xi/o_n * s + 1);
G = syslin('c', LM);
y = csim('step', t, G);

//Controller
Kc = 10;
C = (s+10)*(s+10)/(s*(s + 21));

//Closed loop, look in 7 lecture.
Gc1 = Kc * C * G / (1 + Kc * C * G);
Gc1 = syslin('c', Gc1);
yo1 = csim('step', t, Gc1);


//Plot responce of our system with controller to step, e.g our system should track step.
scf(1);
clf;
plot(t, yo1, 'r--');
xlabel("time (s)");
ylabel("speed (rad/s)");
legend('Responce to 1(t)');

//Root locus
scf(2);
clf;
evans(C * LM, Kc);
//20% overshot, 0.2 - 0.4 setling time - proffesor controller requirments
max_overshot = 0.2; //20% overshot
max_settling_time = 0.4; //0.4 settling time

alpha = 0.05;
//Line that defines constraint on overshot RE/oc = IM
oc = -log(0.2)/%pi;
//Line that defines constaint on settling time e.g. RE = sl
sl = log(alpha)/max_settling_time;


//Plot constrains lines.
plot([sl,-15], [sl/oc,-15/oc], 'r--');
plot([sl,-15], [-sl/oc, 15/oc], 'r--');
plot([sl, sl], [-sl/oc, sl/oc], 'r--');

//Save plot to pdf
xs2pdf(1, "./response.pdf");
xs2pdf(2, "./root_locus.pdf");

