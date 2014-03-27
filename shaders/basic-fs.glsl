#version 330

smooth in vec4 theColor;
smooth in vec2 theNormalTexel;

out vec4 fragColor;

uniform sampler2D normalMap;

void main()
{
	vec3 normal = texture(normalMap, theNormalTexel).xyz;
	float diffuse = max(0.0, dot(normal, vec3(0, 0, 1)));
	fragColor = theColor * vec4(diffuse, diffuse, diffuse, 1.0);
}
