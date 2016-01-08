 #version 330

in vec2 UV;

out vec4 color;

uniform sampler2D FrontMap;
uniform sampler2D BackMap;
uniform sampler2D Background;

uniform sampler3D VolumeMap;

uniform float time;

void main(){
	vec3 back = texture( BackMap, UV ).xyz;

	if (false &&  int(time)%6 <2 ){

		color = texture( FrontMap, UV );
		vec3 coord =  texture( BackMap, UV ).xyz;

		color = vec4(vec3(1,1,1)*texture(VolumeMap,coord).x,1) ;

	}else if  (false && int(time)%6 <4 ){
		color = texture( BackMap, UV );
		vec3 coord =  texture( FrontMap, UV ).xyz;

		color = vec4(vec3(1,1,1)*texture(VolumeMap,coord).x,1) ;

	} else
	{
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
			vec3 step = dir/512.0;
			float stepCount = 512.0*pathLength;

			vec3 position = frontPoint;
			float sum = 0;
			for (int i = 0; i < stepCount; i++)
			{
				position = frontPoint + i*step;
				sum = sum + texture(VolumeMap,position).x;
			}


			color = vec4(vec3(sum,sum,sum),1*sum) ;
		}
		

		color = color +texture(Background, UV)*clamp(1-color.z,0,1);



	}


     // if(back.x < -0.5 ||back.y < -0.5 || back.z < -0.5 || back.x >0.5 ||back.y >0.5 || back.z >0.5)
     // 	color = vec4(1,0.1,0.5,0);

}