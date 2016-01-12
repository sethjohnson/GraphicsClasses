 #version 330

in vec2 UV;

out vec4 color;

uniform sampler2D texBackground;
uniform sampler2D texForeground;

void main(){
	vec4 bgc = texture(texBackground, UV);
	vec4 fgc = texture(texForeground, UV);

	//premult
	float bga = clamp(bgc.a,0,1);
	float fga = clamp(fgc.a,0,1);
	bgc = bgc*bga;
	fgc = fgc*fga;

	color = fgc + bgc * (1-fga);
	color.a = fga + bga * (1-fga);
		color = vec4(color.xyz/color.a, color.a);

}