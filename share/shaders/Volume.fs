 #version 330

in vec2 UV;

out vec4 color;

uniform sampler2D FrontMap;
uniform sampler2D BackMap;
uniform sampler3D VolumeMap;


void main(){

	
	vec3 frontPoint = texture( FrontMap, UV ).xyz;
	vec3 backPoint = texture( BackMap, UV ).xyz;
	if (frontPoint.x == 0 && frontPoint.y == 0 && frontPoint.z == 0)
	{
		color = vec4(0,0,0,0);
	}
	else if (backPoint.x < 0 || backPoint.y < 0 || backPoint.z < 0 ||
		backPoint.x > 1 || backPoint.y > 1 || backPoint.z > 1)
	{
		color = vec4(0,0,0,0);
	}
	else 
	{
		vec3 path = backPoint - frontPoint;
		float pathLength = length(path);
		vec3 dir = path/pathLength;
		float resolution = 256;
		vec3 step = dir/resolution;
		float stepCount = resolution*pathLength;
		float weight = 0.5;//1/resolution;

		vec3 position = frontPoint;
		vec4 sum = vec4(0,0,0,0);
		float a_prod = 0;

		int first = 1;


		for (int i = 0; i < stepCount; i++)
		{
			position = frontPoint + i*step;
			float val = texture(VolumeMap,position).x;
			float red = ((int(val)%17) + 7)%5/5.0;
			float green = ((int(val)%12) + 4)%7/7.0;
			float blue = ((int(val)%10) + 12)%9/9.0;
			float alpha = int(val) > 0? (1-((int(val)%13) + 12)%8/8.0)*0.3 : 0;


			vec4 fgc = sum;
			vec4 bgc = vec4(red,green,blue,alpha);
	//premult
	float bga = clamp(bgc.a,0,1);
	float fga = clamp(fgc.a,0,1);
	bgc = bgc*bga;
	fgc = fgc*fga;

			// vec4 newColor = (fgc * fgc.a + bgc*bgc.a*(1-fgc.a))/(fgc.a + bgc.a*(1-fgc.a));



	//premult

			if(bgc.a > 0)
			{
				sum = vec4(fgc.rgb*fgc.a + bgc.rgb*(1-fgc.a), fgc.a + bgc.a*(1-fgc.a));

				sum = fgc + bgc * (1-fga);
				sum.a = fga + bga * (1-fga);
				sum = vec4(sum.xyz/sum.a, sum.a);
			}

			// c_sum =  fgc.rgb*fgc.a + bgc.rgb*(1-fgc.a);
			// a_prod = fgc.a  *fgc.a + bgc.a  *(1-fgc.a);
		}


		color = vec4(sum.rgb,  sum.a) ;

	}

}