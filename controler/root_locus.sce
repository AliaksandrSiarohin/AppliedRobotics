s = poly(0, 's');
//Time
Dt = 0.001;
t = 0:Dt:2;
Kg = 0.1752;
xi = 1;
o_n = 10.52;


//Plat transfer dunction

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
plot(t, y);
plot(t, yo1, 'r--');

//Root locus
scf(2);
clf;
evans(C * LM, Kc);
