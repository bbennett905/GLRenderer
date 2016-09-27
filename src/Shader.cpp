#include "Shader.h"

#include "Utils.h"

Shader::Shader(const char * vertexPath, const char * fragPath) :
	TextureCount(0)
{
	std::string vertexSource;
	std::string fragSource;
	std::ifstream vertexFile;
	std::ifstream fragFile;

	vertexFile.exceptions(std::ifstream::badbit);
	fragFile.exceptions(std::ifstream::badbit);
	try
	{
		vertexFile.open(vertexPath);
		fragFile.open(fragPath);
		std::stringstream vertexStream, fragStream;

		vertexStream << vertexFile.rdbuf();
		fragStream << fragFile.rdbuf();

		vertexFile.close();
		fragFile.close();

		vertexSource = vertexStream.str();
		fragSource = fragStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	createShaders(vertexSource.c_str(), fragSource.c_str());
}

void Shader::Use() 
{ 
	glUseProgram(Program); 
	TextureCount = 0;
}

void Shader::createShaders(const char * vertexSource, const char * fragSource)
{
	GLuint vertex, frag;
	GLint success;
	GLchar infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSource, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragSource, NULL);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(frag, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	Program = glCreateProgram();
	glAttachShader(Program, vertex);
	glAttachShader(Program, frag);
	glLinkProgram(Program);
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(frag);
}

//Another way to do this would be a preprocessor - something like #REPLACE VERSION, or #REPLACE NUM_MAT?
Shader::Shader(ShaderCreateInfo & info) :
	TextureCount(0)
{
	std::string fragSource = "";
	std::string vertexSource = "";

	//Add the version macro for each shader at the beginning
	switch (info.Version)
	{
	case ShaderVersion330Core:
		fragSource += "#version 330 core\n";
		vertexSource += "#version 330 core\n";
		break;
	default:
		fragSource += "#version 330 core\n";
		vertexSource += "#version 330 core\n";
		break;
	}

	//Nothing really needs to be changed from default in the vertex shader
	vertexSource +=
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec3 normal;\n"
		"layout (location = 2) in vec2 texCoord;\n"
		"\n"
		"out vec3 Normal;\n"
		"out vec2 TexCoord;\n"
		"out vec3 FragPos;\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"\n"
		"void main(void) {\n"
		"    gl_Position = projection * view * model * vec4(position, 1.0f);\n"
		"    FragPos = vec3(model * vec4(position, 1.0f));\n"
		"    TexCoord = texCoord;\n"
		"    Normal = mat3(transpose(inverse(model))) * normal;\n"
		"}\n";

	fragSource +=
		"in vec3 Normal;\n"
		"in vec2 TexCoord;\n"
		"in vec3 FragPos;\n"
		"\n"
		"out vec4 color;\n"
		"\n"
		"struct LightPoint {\n"
		"    vec3 Position;\n"
		"    vec3 Color;\n"
		"    float Intensity;\n"
		"    float Constant;\n"
		"    float Linear;\n"
		"    float Quadratic;\n"
		"};\n"
		"struct LightDirectional {\n"
		"    vec3 Direction;\n"
		"    vec3 Color;\n"
		"    float Intensity;\n"
		"    vec3 AmbientColor;\n"
		"    float AmbientIntensity;\n"
		"};\n"
		"struct LightSpot {\n"
		"    vec3 Position;\n"
		"    vec3 Direction;\n"
		"    float InnerCutOff;\n"
		"    float OuterCutOff;\n"
		"    vec3 Color;\n"
		"    float Intensity;\n"
		"    float Constant;\n"
		"    float Linear;\n"
		"    float Quadratic;\n"
		"};\n"
		"struct Material {\n"
		"    sampler2D DiffMap;\n"
		"    sampler2D SpecMap;\n"
		"    float AmbientStrength;\n"
		"    float DiffuseStrength;\n"
		"    float SpecularStrength;\n"
		"    float Shininess;\n"
		"};\n"
		"\n"
		"uniform LightDirectional directionalLight;\n"	//TODO what if these are 0?
		"uniform LightPoint pointLights[" + std::to_string(info.NumPointLights) + "];\n"
		"uniform LightSpot spotLights[" + std::to_string(info.NumSpotLights) + "];\n"
		"uniform Material materials[" + std::to_string(info.NumMaterials) + "];\n"
		"uniform vec3 viewPos;\n"
		"\n"
		"vec3 CalcDirLight(LightDirectional light, vec3 norm, vec3 viewDir);\n"
		"vec3 CalcPointLight(LightPoint light, vec3 norm, vec3 fragPos, vec3 viewDir);\n"
		"vec3 CalcSpotLight(LightSpot light, vec3 norm, vec3 fragPos, vec3 viewDir);\n"
		"vec3 SumDiffMaps();\n"
		"vec3 SumSpecMaps();\n"
		"float AvgShininess();\n"
		"float AvgAmbientStrength();\n"
		"\n"
		"void main(void) {\n"
		"    vec3 norm = normalize(Normal);\n"
		"    vec3 viewDir = normalize(viewPos - FragPos);\n"
		"    vec3 result = CalcDirLight(directionalLight, norm, viewDir);\n"
		"    for (int i = 0; i < pointLights.length(); i++)\n"
		"        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);\n"
		"    for (int i = 0; i < spotLights.length(); i++)\n"
		"        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);\n"
		"    color = vec4(result, 1.0f);\n"
		"}\n"
		"\n"
		"vec3 CalcDirLight(LightDirectional light, vec3 norm, vec3 viewDir) {\n"
		"    vec3 lightDir = normalize(-light.Direction);\n"
		"    vec3 ambient  = light.AmbientColor * light.AmbientIntensity * AvgAmbientStrength();\n"
		"    float diff = max(dot(norm, lightDir), 0.0);\n"
		"    vec3 diffuse  = light.Color * light.Intensity  * diff;\n"
		"    vec3 reflectDir = reflect(-lightDir, norm);\n"
		"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), AvgShininess());\n"
		"    vec3 specular = light.Color * light.Intensity * spec;\n"
		"    return ((ambient + diffuse) * SumDiffMaps()) + \n"
		"        (specular * SumSpecMaps());\n"
		"}\n"
		"\n"
		"vec3 CalcPointLight(LightPoint light, vec3 norm, vec3 fragPos, vec3 viewDir) {\n"
		"    vec3 lightDir = normalize(light.Position - fragPos);\n"
		"    float diff = max(dot(norm, lightDir), 0.0);\n"
		"    vec3 diffuse = light.Color * light.Intensity * diff;\n"
		"    vec3 reflectDir = reflect(-lightDir, norm);\n"
		"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), AvgShininess());\n"
		"    vec3 specular = light.Color * light.Intensity * spec;\n"
		"    float distance = length(light.Position - fragPos);\n"
		"    float attenuation = 1.0f / (light.Constant + light.Linear * distance + \n"
		"        light.Quadratic * (distance * distance));\n"
		"    return ( (diffuse * SumDiffMaps()) +\n"
		"        (specular * SumSpecMaps()) ) * attenuation;\n"
		"}\n"
		"\n"
		"vec3 CalcSpotLight(LightSpot light, vec3 norm, vec3 fragPos, vec3 viewDir) {\n"
		"    vec3 lightDir = normalize(light.Position - fragPos);\n"
		"    float diff = max(dot(norm, lightDir), 0.0);\n"
		"    vec3 diffuse = light.Color * light.Intensity * diff;\n"
		"    vec3 reflectDir = reflect(-lightDir, norm);\n"
		"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), AvgShininess());\n"
		"    vec3 specular = light.Color * light.Intensity * spec;\n"
		"    float distance = length(light.Position - fragPos);\n"
		"    float attenuation = 1.0f / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));\n"
		"    float theta = dot(lightDir, normalize(-light.Direction));\n"
		"    float epsilon = light.InnerCutOff - light.OuterCutOff;\n"
		"    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);\n"
		"    return intensity * attenuation * ((diffuse * SumDiffMaps()) + \n"
		"                            (specular * SumSpecMaps()) );\n"
		"}\n"
		"\n"
		"vec3 SumDiffMaps() {\n"
		"    vec3 sum = vec3(0.0f, 0.0f, 0.0f);\n"
		"    for (int i = 0; i < materials.length(); i++)\n"
		"        sum += (texture(materials[i].DiffMap, TexCoord).xyz * materials[i].DiffuseStrength);\n"
		"    return sum;\n"
		"}\n"
		"\n"
		"vec3 SumSpecMaps() {\n"
		"    vec3 sum = vec3(0.0f, 0.0f, 0.0f);\n"
		"    for (int i = 0; i < materials.length(); i++)\n"
		"        sum += (texture(materials[i].SpecMap, TexCoord).xyz * materials[i].SpecularStrength);\n"
		"    return sum;\n"
		"}\n"
		"\n"
		"float AvgShininess() {\n"
		"    float sum = 0.0f;\n"
		"    for (int i = 0; i < materials.length(); i++)\n"
		"        sum += materials[i].Shininess;\n"
		"    sum /= materials.length();\n"
		"    return sum;\n"
		"}\n"
		"\n"
		"float AvgAmbientStrength() {\n"
		"    float sum = 0.0f;\n"
		"    for (int i = 0; i < materials.length(); i++)\n"
		"        sum += materials[i].AmbientStrength;\n"
		"    sum /= materials.length();\n"
		"    return sum;\n"
		"}\n";

	createShaders(vertexSource.c_str(), fragSource.c_str());
}

//TODO finish and implement this
void Shader::preprocessShader(std::string vertexSource, std::string fragSource,
	ShaderCreateInfo info)
{
	switch (info.Version)
	{
	case ShaderVersion330Core:
		FindAndReplaceAll(vertexSource, "#insert version", "#version 330 core");
		FindAndReplaceAll(fragSource, "#insert version", "#version 330 core");
		break;
	default:
		FindAndReplaceAll(vertexSource, "#insert version", "#version 330 core");
		FindAndReplaceAll(fragSource, "#insert version", "#version 330 core");
		break;
	}

	FindAndReplaceAll(vertexSource, "#insert num_point_lights", std::to_string(info.NumPointLights));
	FindAndReplaceAll(fragSource, "#insert num_point_lights", std::to_string(info.NumPointLights));
	FindAndReplaceAll(vertexSource, "#insert num_spot_lights", std::to_string(info.NumSpotLights));
	FindAndReplaceAll(fragSource, "#insert num_spot_lights", std::to_string(info.NumSpotLights));
	FindAndReplaceAll(vertexSource, "#insert num_materials", std::to_string(info.NumMaterials));
	FindAndReplaceAll(fragSource, "#insert num_materials", std::to_string(info.NumMaterials));
}