#insert version

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

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

struct Material {
    vec3 Color;
	float Roughness;
	float Metallicity;
};

uniform samplerCube envMap;
uniform Material material;

uniform LightDirectional directionalLight;

uniform LightPoint pointLights[
#insert num_point_lights
    ];

uniform LightSpot spotLights[
#insert num_spot_lights
    ];

uniform vec3 viewPos;

vec4 CalcDirLight(LightDirectional light, vec3 norm, vec3 viewDir);
vec4 CalcPointLight(LightPoint light, vec3 norm, vec3 fragPos, vec3 viewDir);
vec4 CalcSpotLight(LightSpot light, vec3 norm, vec3 fragPos, vec3 viewDir);

void main(void) {
    //i guess lets start with an envmap reflection
	vec3 I = normalize(FragPos - viewPos);
    vec3 R = reflect(I, normalize(Normal));
	vec4 result = texture(envMap, R);

	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	result *= 1.0f / max(dot(norm, viewDir), 0.0);

    result += CalcDirLight(directionalLight, norm, viewDir);
    for (int i = 0; i < pointLights.length(); i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    for (int i = 0; i < spotLights.length(); i++)
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
    color = normalize(result * vec4(material.Color, 1.0f));
}

vec4 CalcDirLight(LightDirectional light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(-light.Direction);
    vec3 ambient  = light.AmbientColor * light.AmbientIntensity;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.Color * light.Intensity  * diff;

	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 64);
    vec3 specular = light.Color * light.Intensity * spec;

    return vec4(ambient + diffuse + specular, 1.0f);
}

vec4 CalcPointLight(LightPoint light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.Color * light.Intensity * diff;

    vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0f);
    vec3 specular = light.Color * light.Intensity * spec;

    float distance = length(light.Position - fragPos);
    float attenuation = 1.0f / (light.Constant + light.Linear * distance + 
        light.Quadratic * (distance * distance));
    return vec4(diffuse + specular, 1.0f) * attenuation;
}

vec4 CalcSpotLight(LightSpot light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.Color * light.Intensity * diff;

    vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0f);
    vec3 specular = light.Color * light.Intensity * spec;

    float distance = length(light.Position - fragPos);
    float attenuation = 1.0f / (light.Constant + light.Linear * distance + 
	                            light.Quadratic * (distance * distance));
    float theta = dot(lightDir, normalize(-light.Direction));
    float epsilon = light.InnerCutOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
    return intensity * attenuation * vec4(diffuse + specular, 1.0f);
}