uniform sampler2D ShadowMap;
uniform sampler2D TextureMap;

varying vec4 ShadowCoord;
varying vec2 texCoord;

void main()
{	
	vec4 shadowCoordinateWdivide = ShadowCoord / ShadowCoord.w ;

	shadowCoordinateWdivide.z += 0.0005;
	float distanceFromLight = texture2D(ShadowMap,shadowCoordinateWdivide.st).z;
	vec4 texColor = texture2D(TextureMap,texCoord);
		
 	float shadow = 1.0;
 	if (ShadowCoord.w > 0.0)
 		shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.5 : 1.0 ;

	gl_FragColor =	 shadow * texColor;
}
