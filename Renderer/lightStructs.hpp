struct lightAmbient
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 0.0f;
};
struct lightDiffuse
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 0.0f;
};
struct lightSpecular
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 0.0f;
	float range = 100;
	float falloff = 0;
	float attenuation0 = 1;
	float attenuation1 = 0;
	float attenuation2 = 0;
	float power = 0;
};