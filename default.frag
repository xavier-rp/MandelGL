#version 330 core

// function to generate cycling color palettes
// https://iquilezles.org/articles/palettes/
// http://dev.thi.ng/gradients/
vec3 palette(float t)
{
	vec3 a = vec3(0.5, 0.5, 0.5);
	vec3 b = vec3(0.5, 0.5, 0.5); 
	vec3 c = vec3(1.0, 1.0, 1.0);
	vec3 d = vec3(0.263, 0.416, 0.557);
	return a + b*cos( 6.28318*(c*t+d));
}

// Outputs colors in RGBA
out vec4 FragColor;

// Uniforms
uniform float iTime;
uniform vec2  iResolution;

uniform int maxIter;

void main()
{
	// uv coordinates originally go from 0 to 1.
	// with the following operations, we transform the coordinates so they go from -1 to 1
	// and the center of the canvas is now (0, 0).
	vec2 uv0 = gl_FragCoord.xy / iResolution.xy * 2.0 - 1.0;

	// Scale the u coordinate so that it's interval from -1 to 1 spans the same number of pixels as the y coordinate.
	uv0.x *= iResolution.x / iResolution.y;

	float reZ = 0.0;
	float imZ = 0.0;
	float reZtemp = 0.0;
	int	  iter = 0;

	while (reZ*reZ + imZ*imZ <= 2.0f && iter < maxIter )
	{
		reZtemp = reZ*reZ - imZ*imZ + uv0.x;
		imZ = 2*reZ*imZ + uv0.y;
		reZ = reZtemp;
		iter += 1;
	}

	FragColor = vec4(palette(float(iter)/float(maxIter)), 1.0f);
}