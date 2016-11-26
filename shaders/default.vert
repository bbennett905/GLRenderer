#insert version

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

struct LightPoint {
    vec3 Position;
    vec3 Color;
    float Intensity;
    float Constant;
    float Linear;
    float Quadratic;
};
struct LightDirectional {
    vec3 Direction;
    vec3 Color;
    float Intensity;
    vec3 AmbientColor;
    float AmbientIntensity;
};
struct LightSpot {
    vec3 Position;
    vec3 Direction;
    float InnerCutOff;
    float OuterCutOff;
    vec3 Color;
    float Intensity;
    float Constant;
    float Linear;
    float Quadratic;
};

uniform LightDirectional directionalLight;

uniform LightPoint pointLights[
#insert num_point_lights
    ];

uniform LightSpot spotLights[
#insert num_spot_lights
    ];

//And pass to the out variables
out LightDirectionalF {
	vec3 Direction;
    vec3 Color;
    float Intensity;
    vec3 AmbientColor;
    float AmbientIntensity;
} directionalLightF;

out LightPointF {
	vec3 Position;
    vec3 Color;
    float Intensity;
    float Constant;
    float Linear;
    float Quadratic;
} pointLightsF[
#insert num_point_lights
    ];
out LightSpotF {
	vec3 Position;
    vec3 Direction;
    float InnerCutOff;
    float OuterCutOff;
    vec3 Color;
    float Intensity;
    float Constant;
    float Linear;
    float Quadratic;
} spotLightsF[
#insert num_spot_lights
    ];
	

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
out vec3 viewPosF;

void main(void) {
	vec3 T = normalize((model * vec4(tangent, 0.0)).xyz);
	vec3 N = normalize((model * vec4(normal, 0.0)).xyz);
	vec3 B = normalize((model * vec4(bitangent, 0.0)).xyz);

	mat3 TBN;
	TBN = transpose(mat3(T, B, N));
	viewPosF = TBN * viewPos;

	directionalLightF.Direction = TBN * directionalLight.Direction;
	directionalLightF.Color = directionalLight.Color;
	directionalLightF.Intensity = directionalLight.Intensity;
	directionalLightF.AmbientColor = directionalLight.AmbientColor;
	directionalLightF.AmbientIntensity = directionalLight.AmbientIntensity;

	for (int i = 0; i < pointLights.length(); i++)
	{
		pointLightsF[i].Position = TBN * pointLights[i].Position;
		pointLightsF[i].Color = pointLights[i].Color;
		pointLightsF[i].Intensity = pointLights[i].Intensity;
		pointLightsF[i].Constant = pointLights[i].Constant;
		pointLightsF[i].Linear = pointLights[i].Linear;
		pointLightsF[i].Quadratic = pointLights[i].Quadratic;
	}
	for (int i = 0; i < spotLights.length(); i++)
	{
		spotLightsF[i].Position = TBN * spotLights[i].Position;
		spotLightsF[i].Direction = TBN * spotLights[i].Direction;
		spotLightsF[i].InnerCutOff = spotLights[i].InnerCutOff;
		spotLightsF[i].OuterCutOff = spotLights[i].OuterCutOff;
		spotLightsF[i].Color = spotLights[i].Color;
		spotLightsF[i].Intensity = spotLights[i].Intensity;
		spotLightsF[i].Constant = spotLights[i].Constant;
		spotLightsF[i].Linear = spotLights[i].Linear;
		spotLightsF[i].Quadratic = spotLights[i].Quadratic;
	}
	
    gl_Position = projection * view * model * vec4(position, 1.0f);
	FragPos = TBN * vec3(model * vec4(position, 1.0f));
    TexCoord = texCoord;
    Normal = TBN * (mat3(transpose(inverse(model))) * normal);
}
