surface apple() {
	float Ka = 1;
	float Kd = 1.;
	float Ks = 0.3;
	float roughness = 0.4;
	color white = 1;
	color black = color(0,0,0);
	color brown = color(210/255, 180/255, 140/255)*0.5;
	color spots = (1-brown) * (1-step(noise(30*P), 0.8));

	// Noise
	color low_freq = noise(1.*P);
	color mid_freq = noise(4.*P);
	color high_freq = noise(20.*P);
	color stripes = noise(300*s, 40*t);
	// Combine noise
	color noise_col = 0.7*low_freq + 0.2*mid_freq + 0.1*stripes;
	color basic_col = color(0.8,0,0) + (color(0,1,0) * noise_col);// Basic red/green/yellow colors for apple
	P = P - N * (0.6*mid_freq+0.4*high_freq) * 0.002; // small scale displacement
	
	// Displacement along sphere poles for apple shape

	float factor = 0.6 - 0.6 * smoothstep(-0.2,0.2,v);
	P = P - N * (factor);

	// Lighting
	N = calculatenormal(P);
	normal Nf = N;
	Ci = (basic_col-0.5*spots) * diffuse(N);
	Oi = Os;
    Ci = Os * ( Ci * (Ka*ambient() + Kd*diffuse(Nf)) +	white * Ks*specular(Nf,-normalize(I),roughness));
}
