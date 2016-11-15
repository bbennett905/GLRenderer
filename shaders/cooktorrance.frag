#insert version

//Use default.vert with this shader

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
	vec3 Color;	//used if no diff map
	bool HasMetalAndRoughMap; //metal R, rough G
    sampler2D MetalAndRoughMap;
	//bool HasRoughMap;
    //sampler2D RoughMap;
    float Roughness;
	float Metallicity;
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
vec4 AvgMetalAndRoughMaps();


vec4 CookTorrance(vec3 norm, vec3 lightDir, vec3 lightColor, vec3 viewDir, float width = 0);

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

	result *= SumDiffMaps();
	
    color = result;
}

vec4 CalcDirLight(LightDirectional light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(-light.Direction);
    vec3 ambient = light.AmbientColor * light.AmbientIntensity;

    return CookTorrance(norm, lightDir, light.Color, viewDir) * light.Intensity + 
		vec4(ambient, 1.0f);
}

vec4 CalcPointLight(LightPoint light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.Position - fragPos);

    float dist = length(light.Position - fragPos);
    float attenuation = 1.0f / (light.Constant + light.Linear * dist + 
        light.Quadratic * (dist * dist));

    return CookTorrance(norm, lightDir, light.Color, viewDir) * attenuation * light.Intensity;
}

vec4 CalcSpotLight(LightSpot light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.Position - fragPos);

    float dist = length(light.Position - fragPos);
    float attenuation = 1.0f / (light.Constant + light.Linear * dist + 
        light.Quadratic * (dist * dist));
	
    float theta = dot(lightDir, normalize(-light.Direction));
    float epsilon = light.InnerCutOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);

    return intensity * attenuation * 
		CookTorrance(norm, normalize(-light.Direction), light.Color, viewDir) * light.Intensity;
}

vec4 SumDiffMaps() {
    vec4 sum = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < numMaterials; i++)
	{
	    if (materials[i].HasDiffMap)
		{
            sum += texture(materials[i].DiffMap, TexCoord);// * materials[i].DiffuseStrength;
		}
		else
		{
			sum += vec4(materials[i].Color, 1.0f);
		}
	}
    return sum;
}

vec4 AvgMetalAndRoughMaps() {
    vec4 sum = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	int numMaps = 0;
    for (int i = 0; i < numMaterials; i++)
	{
	    if (materials[i].HasMetalAndRoughMap)
		{
            sum += vec4(texture(materials[i].MetalAndRoughMap, TexCoord).x * materials[i].Metallicity,
						texture(materials[i].MetalAndRoughMap, TexCoord).y * materials[i].Roughness,
						0.0, 0.0);
			numMaps++;
		}
	}
    return sum / numMaps;
}

vec4 CookTorrance(vec3 norm, vec3 lightDir, vec3 lightColor, vec3 viewDir, float width)
{
	//this time we're trying the unreal implementation - or something close to it
	vec4 materialProps = AvgMetalAndRoughMaps();
	vec3 halfVector = normalize(lightDir + viewDir);
    float NdotH = max(dot(norm, halfVector), 0); 
	float roughnessValue = clamp(materialProps.y, 0.1, 1.0);
	float alpha = roughnessValue * roughnessValue;
	float NdotV = max(dot(norm, viewDir), 0);	
	float VdotH = max(dot(viewDir, halfVector), 0);
	float NdotL = max(dot(norm, lightDir), 0);
	float F0 = clamp(materialProps.x, 0.05, 0.95);

	//Distribution
	float distDenominator = ( (NdotH * NdotH) * ( (alpha * alpha) - 1) + 1);
	distDenominator *= distDenominator;
	float distribution = (alpha * alpha) / (3.14 * distDenominator);

	//Geometric - using smith-schlick-beckmann
	float geometric;
	float k = ((roughnessValue + 1) * (roughnessValue + 1)) / 8;
	float g1 = NdotV / ( (NdotV * (1 - k)) + k );
	float g2 = NdotL / ( (NdotL * (1 - k)) + k );
	//geometric = g1 * g2;

	//Geometric - using cook-torrance
	geometric = min(1, min(2 * (NdotV * NdotH) / max(VdotH, 0.001), 2 * (NdotL * NdotH) / max(VdotH, 0.001)));

	//Fresnel - using spherical gaussian approx
	float exponent;
	exponent = VdotH * ( (VdotH * -5.55473) - 6.98316);
	float fresnel = F0 + (1 - F0) * (pow(2, exponent));

	float specular = (distribution * geometric * fresnel) / (4 * max(NdotL, 0.001) * max(NdotV, 0.001));

	float kS = fresnel;

	return vec4(lightColor, 1.0) * ( (specular * kS) + NdotL * SumDiffMaps() * (1.0 - kS) );
}
