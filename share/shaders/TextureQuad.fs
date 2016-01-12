 #version 330

in vec2 UV;

out vec4 color;

uniform sampler2D tex;



void main( ){
	vec4 fgc = texture(tex, UV);
	vec4 bgc = vec4(0,0,0,1);


	//premult
	float bga = clamp(bgc.a,0,1);
	float fga = clamp(fgc.a,0,1);
	bgc = bgc*bga;
	fgc = fgc*fga;
	color = fgc + bgc * (1-fga);
	color.a = fga + bga * (1-fga);
	color = vec4(color.xyz/color.a, color.a);

}