varying vec4 ShadowCoord;
varying vec2 texCoord;
	
void main()
{
	ShadowCoord = gl_TextureMatrix[7] * gl_Vertex;
	texCoord = gl_MultiTexCoord0.st;

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
