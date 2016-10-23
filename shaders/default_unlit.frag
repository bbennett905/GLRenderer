#insert version

in vec3 Normal;
in vec2 TexCoord;

out vec4 color;

struct Material {
	bool HasDiffMap;
    sampler2D DiffMap;
	bool HasSpecMap;
    sampler2D SpecMap;
};

uniform Material materials[MAX_MATERIALS];
uniform int numMaterials;
uniform bool hasMaterials;

vec4 SumDiffMaps();
vec4 SumSpecMaps();

void main(void) {
	if (!hasMaterials)
	{
		color = vec4(Normal, 1.0f);
		return;
	}

    color = SumDiffMaps() + SumSpecMaps();
}

vec4 SumDiffMaps() {
    vec4 sum = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < numMaterials; i++)
	{
	    if (materials[i].HasDiffMap)
            sum += texture(materials[i].DiffMap, TexCoord);
	}
    return sum;
}

vec4 SumSpecMaps() {
    vec4 sum = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < numMaterials; i++)
	{
	    if (materials[i].HasSpecMap)
            sum += texture(materials[i].SpecMap, TexCoord);
	}
    return sum;
}
