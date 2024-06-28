#version 330 core

precision highp float;

// function to generate cycling color palettes
// https://iquilezles.org/articles/palettes/
// http://dev.thi.ng/gradients/
vec3 palette(float t)
{
	vec3 a = vec3(0.446, 0.643, 0.160);
	vec3 b = vec3(0.380, 0.702, 0.684); 
	vec3 c = vec3(0.351, 1.378, 0.247);
	vec3 d = vec3(5.039, 2.212, 1.041);
	return a + b*cos( 6.28318*(c*t+d));
}

// Outputs colors in RGBA
out vec4 FragColor;

// Uniforms
uniform float iTime;
uniform vec2  iResolution;

uniform int   maxIter;
uniform float xPos;
uniform float yPos;
uniform float zoomFactor;
uniform vec2  c;

void main()
{
	// uv coordinates originally go from 0 to 1.
	// with the following operations, we transform the coordinates so they go to a new rang
	vec2 uv0;
	
	uv0.x = (gl_FragCoord.x / iResolution.x - 0.5f) * 3.0f * zoomFactor + xPos;

	uv0.y = (gl_FragCoord.y / iResolution.y - 0.5f) * 3.0f * zoomFactor + yPos;

	// Fix the aspect ratio
	uv0.x *= iResolution.x / iResolution.y;

	vec2 z = uv0;
	int	 iter = 0;

	while (dot(z, z) <= 65536.0f && iter < maxIter)
	{
		z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;
		iter += 1;
	}

	float smoothIter = iter - log2(log2(dot(z, z))) + 4.0;
	FragColor = vec4(vec3(palette(smoothstep(0.0f, 0.01f, float(smoothIter)/float(maxIter)))), 1.0f);

}