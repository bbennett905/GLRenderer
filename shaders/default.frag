#insert version

in vec3 Normal;
in vec2 TexCoord;
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
	bool HasDiffMap;
    sampler2D DiffMap;
	bool HasSpecMap;
    sampler2D SpecMap;
    float AmbientStrength;
    float DiffuseStrength;
    float SpecularStrength;
    float Shininess;
};

uniform LightDirectional directionalLight;

uniform LightPoint pointLights[
#insert num_point_lights
    ];

uniform LightSpot spotLights[
#insert num_spot_lights
    ];

uniform Material materials[MAX_MATERIALS];
uniform int numMaterials;
uniform bool hasMaterials;

uniform vec3 viewPos;

vec4 CalcDirLight(LightDirectional light, vec3 norm, vec3 viewDir);
vec4 CalcPointLight(LightPoint light, vec3 norm, vec3 fragPos, vec3 viewDir);
vec4 CalcSpotLight(LightSpot light, vec3 norm, vec3 fragPos, vec3 viewDir);
vec4 SumDiffMaps();
vec4 SumSpecMaps();
float AvgShininess();
float AvgAmbientStrength();

void main(void) {
	if (!hasMaterials)
	{
		color = vec4(Normal, 1.0f);
		return;
	}
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec4 result = CalcDirLight(directionalLight, norm, viewDir);
    for (int i = 0; i < pointLights.length(); i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    for (int i = 0; i < spotLights.length(); i++)
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
    color = result;
}

vec4 CalcDirLight(LightDirectional light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(-light.Direction);
    vec3 ambient  = light.AmbientColor * light.AmbientIntensity * AvgAmbientStrength();

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.Color * light.Intensity  * diff;

	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), AvgShininess());
    vec3 specular = light.Color * light.Intensity * spec;

    return (vec4((ambient + diffuse), 1.0f) * SumDiffMaps()) + 
        (vec4(specular, 1.0f) * SumSpecMaps());
}

vec4 CalcPointLight(LightPoint light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.Color * light.Intensity * diff;

    vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), AvgShininess());
    vec3 specular = light.Color * light.Intensity * spec;

    float distance = length(light.Position - fragPos);
    float attenuation = 1.0f / (light.Constant + light.Linear * distance + 
        light.Quadratic * (distance * distance));
    return ( (vec4(diffuse, 1.0f) * SumDiffMaps()) + 
	    (vec4(specular, 1.0f) * SumSpecMaps()) ) * attenuation;
}

vec4 CalcSpotLight(LightSpot light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.Color * light.Intensity * diff;

    vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), AvgShininess());
    vec3 specular = light.Color * light.Intensity * spec;

    float distance = length(light.Position - fragPos);
    float attenuation = 1.0f / (light.Constant + light.Linear * distance + 
	                            light.Quadratic * (distance * distance));
    float theta = dot(lightDir, normalize(-light.Direction));
    float epsilon = light.InnerCutOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
    return intensity * attenuation * ( (vec4(diffuse, 1.0f) * SumDiffMaps()) + 
                                       (vec4(specular, 1.0f) * SumSpecMaps()) );
}

vec4 SumDiffMaps() {
    vec4 sum = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < numMaterials; i++)
	{
	    if (materials[i].HasDiffMap)
            sum += texture(materials[i].DiffMap, TexCoord) * materials[i].DiffuseStrength;
	}
    return sum;
}

vec4 SumSpecMaps() {
    vec4 sum = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < numMaterials; i++)
	{
	    if (materials[i].HasSpecMap)
            sum += texture(materials[i].SpecMap, TexCoord) * materials[i].SpecularStrength;
	}
    return sum;
}

float AvgShininess() {
    float sum = 0.0f;
    for (int i = 0; i < numMaterials; i++)
        sum += materials[i].Shininess;
    sum /= numMaterials;
    return sum;
}

float AvgAmbientStrength() {
    float sum = 0.0f;
    for (int i = 0; i < numMaterials; i++)
        sum += materials[i].AmbientStrength;
    sum /= numMaterials;
    return sum;
}