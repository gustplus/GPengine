#version 330

uniform float shininess;
uniform mat4 vMV;
uniform vec3 vLightPos;
smooth in vec3 fPos;
smooth in vec3 fNormal;

layout(location=0) out vec4 Frag_Color;

const vec4 diffuseC = vec4(0, 0, 0.5, 1);
const vec4 ambientC = vec4(0, 0, 0.3, 1);
const vec4 specularC = vec4(1, 1, 1, 1);

void main(){
	vec3 eyeLightPos = vLightPos;
	vec3 viewDir = -fPos;
	vec3 eyeDir = normalize(viewDir);
	vec3 lightDir = normalize(eyeLightPos - fPos);
	vec3 halfV = normalize(eyeDir + lightDir);

	float diffuse = max(0, dot(eyeDir, fNormal));
	float specular = max(0, pow(dot(fNormal, halfV), 60));

	//Frag_Color = vec4(diffuse, diffuse, diffuse, 1);
	//Frag_Color = vec4(specular, specular, specular, 1);
	//Frag_Color = vec4(halfV, 1);
	Frag_Color = diffuseC * diffuse + specularC * specular + ambientC;
}