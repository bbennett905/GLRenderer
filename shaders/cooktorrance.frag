#insert version

//Use default.vert with this shader

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

out vec4 color;

struct Material {
	bool HasDiffMap;
	sampler2D DiffMap;
	vec3 Color;	//used if no diff map
	bool HasMetalAndRoughMap; //metal R, rough G
    sampler2D MetalAndRoughMap;
	bool HasNormalMap;
	sampler2D NormalMap;
    float Roughness;
	float Metallicity;
};

in LightDirectionalF {
	vec3 Direction;
    vec3 Color;
    float Intensity;
    vec3 AmbientColor;
    float AmbientIntensity;
} directionalLightF;

in LightPointF {
	vec3 Position;
    vec3 Color;
    float Intensity;
    float Constant;
    float Linear;
    float Quadratic;
} pointLightsF[
#insert num_point_lights
    ];
in LightSpotF {
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

uniform Material materials[MAX_MATERIALS];
uniform int numMaterials;
uniform bool hasMaterials;

in vec3 viewPosF;

vec4 CalcDirLight(vec3 norm, vec3 viewDir);
vec4 CalcPointLight(int i, vec3 norm, vec3 fragPos, vec3 viewDir);
vec4 CalcSpotLight(int i, vec3 norm, vec3 fragPos, vec3 viewDir);

vec4 SumDiffMaps();
vec4 AvgMetalAndRoughMaps();
vec3 AvgNormalMaps();

vec4 CookTorrance(vec3 norm, vec3 lightDir, vec3 lightColor, vec3 viewDir);

float gamma = 1.5;

void main(void) {
	if (!hasMaterials)
	{
		color = vec4(Normal, 1.0f);
		return;
	}

	//case 1: convert normal map to world space and calculate in frag shader
	//if we have normal maps, avg them and convert it to world space
	//O(numPixels), matrix * vec mult
	//For large, flat surfaces, this is going to be a lot of these, when its not necessary
	//After testing, this took ~22% longer from basic benchmarks

	//case 2: convert everything else to tangent space using inverted TBN matrix, and 
	//do most of the matrix calculations in vert shader - convert everything to tangent space
	//in vert, and use same calculations here
	//O(numVerts * numLights), matrix * vert mult
	//So this might actually be slower for high-poly models, at a distance (fewer pixels/poly)
	//especially with large numbers of lights in a scene

	vec3 norm = AvgNormalMaps();

	if (norm.z > 0.0)
	{
		norm = normalize(norm * 2.0 - 1.0);  
	}
	else
	{
		norm = normalize(Normal);
	}

	
	vec3 viewDir = normalize(viewPosF - FragPos);

	vec4 result = CalcDirLight(norm, viewDir);
	
    for (int i = 0; i < pointLightsF.length(); i++)
        result += CalcPointLight(i, norm, FragPos, viewDir);
    for (int i = 0; i < spotLightsF.length(); i++)
        result += CalcSpotLight(i, norm, FragPos, viewDir);
	
	result.w = 1.0;
	
	result *= SumDiffMaps();
	
	result.rgb = pow(result.rgb, vec3(1.0/gamma));
    color = result;
}

vec4 CalcDirLight(vec3 norm, vec3 viewDir)  {
	vec3 lightDir = normalize(-directionalLightF.Direction);
    vec3 ambient = directionalLightF.AmbientColor * directionalLightF.AmbientIntensity;

    return CookTorrance(norm, lightDir, directionalLightF.Color, viewDir) * directionalLightF.Intensity + 
		vec4(ambient, 0.0f);
}

vec4 CalcPointLight(int i, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(pointLightsF[i].Position - fragPos);

    float dist = length(pointLightsF[i].Position - fragPos);
    float attenuation = 1.0f / (pointLightsF[i].Constant + pointLightsF[i].Linear * dist + 
        pointLightsF[i].Quadratic * (dist * dist));

    return CookTorrance(norm, lightDir, pointLightsF[i].Color, viewDir) * 
		attenuation * pointLightsF[i].Intensity;
}

vec4 CalcSpotLight(int i, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(spotLightsF[i].Position - fragPos);

    float dist = length(spotLightsF[i].Position - fragPos);
    float attenuation = 1.0f / (spotLightsF[i].Constant + spotLightsF[i].Linear * dist + 
        spotLightsF[i].Quadratic * (dist * dist));
	
    float theta = dot(lightDir, normalize(-spotLightsF[i].Direction));
    float epsilon = spotLightsF[i].InnerCutOff - spotLightsF[i].OuterCutOff;
    float intensity = clamp((theta - spotLightsF[i].OuterCutOff) / epsilon, 0.0, 1.0);

    return intensity * attenuation * 
		CookTorrance(norm, lightDir, spotLightsF[i].Color, viewDir) * spotLightsF[i].Intensity;
}

vec4 SumDiffMaps() {
    vec4 sum = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < numMaterials; i++)
	{
	    if (materials[i].HasDiffMap)
		{
			vec4 diffuseColor = texture(materials[i].DiffMap, TexCoord);
			diffuseColor.xyz = pow(diffuseColor.xyz, vec3(gamma));
            sum += diffuseColor;
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
		else
		{
			sum += vec4(materials[i].Metallicity, materials[i].Roughness, 0.0f, 0.0f);
		}
	}
    return sum / numMaps;
}

vec3 AvgNormalMaps()
{
	vec3 sum = vec3(0.0f, 0.0f, 0.0f);
	int numMaps = 0;
	for (int i = 0; i < numMaterials; i++)
	{
		if (materials[i].HasNormalMap)
		{
			sum += texture(materials[i].NormalMap, TexCoord).xyz;
			numMaps++;
		}	
	}
	vec3 result = vec3(0.0, 0.0, 0.0);
	if (numMaps > 0)
	{
		result = sum / numMaps;
	}
	return result;
}

vec4 CookTorrance(vec3 norm, vec3 lightDir, vec3 lightColor, vec3 viewDir)
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

	//Geometric - using smith-schlick-beckmann - PREFERRED
	float geometric;
	float k = ((roughnessValue + 1) * (roughnessValue + 1)) / 8;
	float g1 = NdotV / ( (NdotV * (1 - k)) + k );
	float g2 = NdotL / ( (NdotL * (1 - k)) + k );
	geometric = g1 * g2;

	//Geometric - using cook-torrance
	//geometric = min(1, min(2 * (NdotV * NdotH) / max(VdotH, 0.001), 2 * (NdotL * NdotH) / max(VdotH, 0.001)));

	//Fresnel - using spherical gaussian approx
	float exponent;
	exponent = VdotH * ( (VdotH * -5.55473) - 6.98316);
	float fresnel = F0 + (1 - F0) * (pow(2, exponent));

	float specular = (distribution * geometric * fresnel) / (4 * max(NdotL, 0.001) * max(NdotV, 0.001));

	float kS = fresnel;

	return vec4(lightColor * (specular * kS + NdotL * (1.0 - kS)), 0.0);
}
