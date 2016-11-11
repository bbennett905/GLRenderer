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

uniform struct Material {
    vec3 Color;
	float Roughness;
	float Metallicity;
} material;

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

vec4 CookTorrance(vec3 norm, vec3 lightDir, vec3 lightColor, vec3 viewDir);

void main(void) {
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec4 result = CalcDirLight(directionalLight, norm, viewDir);
    for (int i = 0; i < pointLights.length(); i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    for (int i = 0; i < spotLights.length(); i++)
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);

    color = result * vec4(material.Color, 1.0f);
}

vec4 CalcDirLight(LightDirectional light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(-light.Direction);
    vec3 ambient = light.AmbientColor * light.AmbientIntensity;

    return CookTorrance(norm, lightDir, light.Color, viewDir) + vec4(ambient, 1.0f);
}

vec4 CalcPointLight(LightPoint light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.Position - fragPos);

    float dist = length(light.Position - fragPos);
    float attenuation = 1.0f / (light.Constant + light.Linear * dist + 
        light.Quadratic * (dist * dist));

    return CookTorrance(norm, lightDir, light.Color, viewDir) * attenuation;// * light.Intensity;
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
		CookTorrance(norm, normalize(-light.Direction), light.Color, viewDir);// * light.Intensity;
}

vec4 CookTorrance(vec3 norm, vec3 lightDir, vec3 lightColor, vec3 viewDir)
{
    // set important material values
    float roughnessValue = material.Roughness; // 0 : smooth, 1: rough
    float F0 = material.Metallicity; // fresnel reflectance at normal incidence
    float k = 0.2; // fraction of diffuse reflection (specular reflection = 1 - k)
    
    // do the lighting calculation for each fragment.
    float NdotL = max(dot(norm, lightDir), 0.0);
    
    float specular = 0.0;
    if(NdotL > 0.0)
    {
        // calculate intermediary values
        vec3 halfVector = normalize(lightDir + viewDir);
        float NdotH = max(dot(norm, halfVector), 0.0); 
        float NdotV = max(dot(norm, viewDir), 0.0); // note: this could also be NdotL, which is the same value
        float VdotH = max(dot(viewDir, halfVector), 0.0);
        float mSquared = roughnessValue * roughnessValue;
        
        // geometric attenuation
        float NH2 = 2.0 * NdotH;
        float g1 = (NH2 * NdotV) / VdotH;
        float g2 = (NH2 * NdotL) / VdotH;
        float geoAtt = min(1.0, min(g1, g2));
     
        // roughness (or: microfacet distribution function)
        // beckmann distribution function
        float r1 = 1.0 / ( 4.0 * mSquared * pow(NdotH, 4.0));
        float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
        float roughness = r1 * exp(r2);
        
        // fresnel
        // Schlick approximation
        float fresnel = pow(1.0 - VdotH, 5.0);
        fresnel *= (1.0 - F0);
        fresnel += F0;
        
        specular = (fresnel * geoAtt * roughness) / (NdotV * NdotL * 3.14);
    }
    
    vec3 finalValue = lightColor * ((NdotL * k) + (specular * (1.0 - k)));
    return vec4(finalValue, 1.0);
}