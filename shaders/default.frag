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
    sampler2D DiffMap;
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

uniform Material materials[
#insert num_materials
    ];

uniform vec3 viewPos;

vec3 CalcDirLight(LightDirectional light, vec3 norm, vec3 viewDir);
vec3 CalcPointLight(LightPoint light, vec3 norm, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(LightSpot light, vec3 norm, vec3 fragPos, vec3 viewDir);
vec3 SumDiffMaps();
vec3 SumSpecMaps();
float AvgShininess();
float AvgAmbientStrength();

void main(void) {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = CalcDirLight(directionalLight, norm, viewDir);
    for (int i = 0; i < pointLights.length(); i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    for (int i = 0; i < spotLights.length(); i++)
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
    color = vec4(result, 1.0f);
}

vec3 CalcDirLight(LightDirectional light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(-light.Direction);
    vec3 ambient  = light.AmbientColor * light.AmbientIntensity * AvgAmbientStrength();
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.Color * light.Intensity  * diff;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), AvgShininess());
    vec3 specular = light.Color * light.Intensity * spec;
    return ((ambient + diffuse) * SumDiffMaps()) + 
        (specular * SumSpecMaps());
}

vec3 CalcPointLight(LightPoint light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.Color * light.Intensity * diff;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), AvgShininess());
    vec3 specular = light.Color * light.Intensity * spec;
    float distance = length(light.Position - fragPos);
    float attenuation = 1.0f / (light.Constant + light.Linear * distance + 
        light.Quadratic * (distance * distance));
    return ( (diffuse * SumDiffMaps()) +
        (specular * SumSpecMaps()) ) * attenuation;
}

vec3 CalcSpotLight(LightSpot light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.Color * light.Intensity * diff;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), AvgShininess());
    vec3 specular = light.Color * light.Intensity * spec;
    float distance = length(light.Position - fragPos);
    float attenuation = 1.0f / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));
    float theta = dot(lightDir, normalize(-light.Direction));
    float epsilon = light.InnerCutOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
    return intensity * attenuation * ((diffuse * SumDiffMaps()) + 
                            (specular * SumSpecMaps()) );
}

vec3 SumDiffMaps() {
    vec3 sum = vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < materials.length(); i++)
        sum += (texture(materials[i].DiffMap, TexCoord).xyz * materials[i].DiffuseStrength);
    return sum;
}

vec3 SumSpecMaps() {
    vec3 sum = vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < materials.length(); i++)
        sum += (texture(materials[i].SpecMap, TexCoord).xyz * materials[i].SpecularStrength);
    return sum;
}

float AvgShininess() {
    float sum = 0.0f;
    for (int i = 0; i < materials.length(); i++)
        sum += materials[i].Shininess;
    sum /= materials.length();
    return sum;
}

float AvgAmbientStrength() {
    float sum = 0.0f;
    for (int i = 0; i < materials.length(); i++)
        sum += materials[i].AmbientStrength;
    sum /= materials.length();
    return sum;
}