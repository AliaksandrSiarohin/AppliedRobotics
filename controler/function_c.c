double T;
double gain = 10;
double controller(double u_2) {
	static double u_0 = 0.0, u_1 = 0.0;
	static double y_0 = 0.0, y_1 = 0.0;

	double y_2 = 1/(4 + 42 * T) * 
		((gain * (u_2 * (4 + 100*T*T + 40*T) + u_1 * (-8 + 200*T*T) + u_0 * (4 - 40*T + 100*T*T))) - y_1 * (-8) - y_0 * (4 - 42*T))
}