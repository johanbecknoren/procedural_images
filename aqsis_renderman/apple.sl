surface apple() {
//	color white = color(1,1,1);
//	color stripecolor = cellnoise(5*s,5*t);
	float stripe_coord = mod(s*6, 1.0);
	float stripes = step(stripe_coord, 0.3);
	
	float stripecoord = 10*s + cellnoise(100*s,t);
	color stripecolor = cellnoise(stripecoord);
	color noise_col = 0.9*noise(P);// + 0.07*noise(20*P) + 0.03*noise(50*P);
	color basic_col = color(0.7,0,0) + color(0,1,0) * noise_col;// Basic red/green/yellow colors for apple
	
	Ci = basic_col * diffuse(N);//stripecolor;
	Oi = Os;
}
