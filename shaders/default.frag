#version 330 core
in vec3 position;
in vec2 TexCoord;
in vec3 normal;
in vec3 fragPos;

//This matches the C++ Material struct/class
struct Material {
	sampler2D Texture1;
	sampler2D Texture2;
	float MixRatio;
	sampler2D SpecMap;
    float AmbientStrength;
    float DiffuseStrength;
    float SpecularStrength;
    float Shininess;
}; 

uniform Material material;

//Basic light, shines in all directions
struct LightPoint {
	vec3 Position;
	vec3 Color;
	float Intensity;

	float Constant;
    float Linear;
    float Quadratic;
};

//"Sunlight" - only one can exist in a scene
struct LightDirectional {
	vec3 Direction;
	vec3 Color;
	float Intensity;
	vec3 AmbientColor;
	float AmbientIntensity;
};

//Spot light, aimed at a point
struct LightSpot {
	vec3 Position;
	vec3 Direction;
	float InnerCutOff;	//angle to cut off, "fov"
	float OuterCutOff;
	vec3 Color;
	float Intensity;

	float Constant;
    float Linear;
    float Quadratic;
};

uniform LightDirectional directionalLight;
#define NR_POINT_LIGHTS 4  
uniform LightPoint pointLights[NR_POINT_LIGHTS];
#define NR_SPOT_LIGHTS 4  
uniform LightSpot spotLights[NR_POINT_LIGHTS];

uniform vec3 viewPos;

//Light Properties - eventually to struct, matching c++
uniform vec3 lightPos;
uniform vec3 ambientColor;
uniform float ambientIntensity;
uniform vec3 lightColor;
uniform float lightIntensity;

out vec4 color;


vec3 CalcDirLight(LightDirectional light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(LightPoint light, vec3 normal, vec3 fragPos, vec3 viewDir);  
vec3 CalcSpotLight(LightSpot light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(void) 
{
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diffFactor = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = material.DiffuseStrength * lightIntensity * diffFactor * lightColor;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float specFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
	vec3 specular = material.SpecularStrength * specFactor * lightColor;  

	vec3 ambient = material.AmbientStrength * ambientIntensity * ambientColor;

	color = vec4((ambient + diffuse) * mix(texture(material.Texture1, TexCoord), texture(material.Texture2, TexCoord), material.MixRatio).xyz, 1.0) +
			vec4(specular * texture(material.SpecMap, TexCoord).xyz, 1.0);
}

vec3 CalcDirLight(LightDirectional light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.Direction);

	vec3 ambient  = light.AmbientColor * light.AmbientIntensity * material.AmbientStrength;

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse  = light.Color * light.Intensity * material.DiffuseStrength * diff;
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
	vec3 specular = light.Color * light.Intensity * material.SpecularStrength * spec;
	
	return ((ambient + diffuse) * mix(texture(material.Texture1, TexCoord), texture(material.Texture2, TexCoord), material.MixRatio).xyz) + 
		(specular * vec3(texture(material.SpecMap, TexCoord)));
}  

vec3 CalcPointLight(LightPoint light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.Position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.Color * light.Intensity * material.DiffuseStrength * diff;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
	vec3 specular = light.Color * light.Intensity * material.SpecularStrength * spec;

    float distance = length(light.Position - fragPos);
    float attenuation = 1.0f / (light.Constant + light.Linear * distance + 
		light.Quadratic * (distance * distance));    

    return ( (diffuse * mix(texture(material.Texture1, TexCoord), texture(material.Texture2, TexCoord), material.MixRatio).xyz) +
		(specular * vec3(texture(material.SpecMap, TexCoord))) ) * attenuation;
} 

vec3 CalcSpotLight(LightSpot light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.Position - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.Color * light.Intensity * material.DiffuseStrength * diff;

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
	vec3 specular = light.Color * light.Intensity * material.SpecularStrength * spec;

	float distance = length(light.Position - fragPos);
	float attenuation = 1.0f / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));    

    float theta = dot(lightDir, normalize(-light.Direction)); 
    float epsilon = light.InnerCutOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);

    return attenuation * (	(diffuse * mix(texture(material.Texture1, TexCoord), 
								texture(material.Texture2, TexCoord), 
								material.MixRatio).xyz) + 
							(specular * vec3(texture(material.SpecMap, TexCoord))) );
}