#version 330

smooth in vec4 theColor;
smooth in vec3 theNormal;

out vec4 fragColor;

void main()
{
	float diffuse = max(0.0, dot(theNormal, vec3(0, 0, 1)));
	fragColor = theColor * vec4(diffuse, diffuse, diffuse, 1.0);
}
