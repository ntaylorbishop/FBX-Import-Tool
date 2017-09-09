

Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);

SamplerState samLinear : register(s0);

static float4 g_ambientLight = float4(1.f, 1.f, 1.f, 0.8f);
static float g_specPower = 128.f;

struct Light3D {
	float4	color;
	float	minLightDistangentce;
	float	maxLightDistangentce;
	float	powerAtMin;
	float	powerAtMax;
	float3	position;
};


//---------------------------------------------------------------------------------------------------------------------------
struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float4 passPos : POSITION;
	float4 color : COLOR0;
	float2 tex : TEXCOORD0;
	float3 tan : TANGENT0;
	float3 bitan : BINORMAL0;
	float3 norm : NORMAL0;
};


//---------------------------------------------------------------------------------------------------------------------------
cbuffer ConstangenttBuffer : register(b0) {
	matrix uModel;
	matrix uView;
	matrix uProjection;
}


//---------------------------------------------------------------------------------------------------------------------------
cbuffer LightConstBuffer : register(b1) {
	float4	color;
	float	minLightDistance;
	float	maxLightDistance;
	float	powerAtMin;
	float	powerAtMax;
	float3	position;
	float3	CameraPos;
}


//---------------------------------------------------------------------------------------------------------------------------
float3 GetSurfaceNormal(float3 passTan, float3 passBitan, float3 passNormal, float4 txNormal) {

	float3x3 tbn = float3x3(passTan, passBitan, passNormal);
	tbn = transpose(tbn);

	float3 normal = float3(txNormal.x, txNormal.y, txNormal.z);
	normal = normal * float3(2.f, 2.f, 1.f) - float3(1.f, 1.f, 0.f);
	//normal = (passNormal + float3(1.f, 1.f, 1.f)) / float3(2.f, 2.f, 2.f);

	//return normal;
	return mul(tbn, normal);
}


//---------------------------------------------------------------------------------------------------------------------------
float4 CalculateLight(float3 passPosition, float3 passTan, float3 passBitan, float3 passNormal, float4 txNormal, float4 txDiffuse) {

	float3 ambientLight = g_ambientLight.rgb * g_ambientLight.a;
	float3 surfaceLight = float3(0.f, 0.f, 0.f);
	float3 specColor	= float3(0.f, 0.f, 0.f);

	float attenuation;
	float amountBasedOnNormalFromLightAngle;

	float3 lightPosition = position;
	float3 lightColor = color.rgb;

	float3 vec_to_light = lightPosition - passPosition;
	float dist = length(vec_to_light);
	vec_to_light = normalize(vec_to_light);
	float3 normal = GetSurfaceNormal(passTan, passBitan, passNormal, txNormal);

	//ATTENUATION
	float minLightDist	= minLightDistance;
	float maxLightDist	= maxLightDistance;
	float minLightPower = powerAtMin;
	float maxLightPower = powerAtMax;

	float distAttenuated = smoothstep(minLightDist, maxLightDist, dist);
	attenuation = lerp(minLightPower, maxLightPower, distAttenuated);

	amountBasedOnNormalFromLightAngle = saturate(dot(vec_to_light, normal));

	surfaceLight += lightColor * amountBasedOnNormalFromLightAngle * attenuation;

	//SPECULAR
	float specular_intensity = 1.f;
	float3 vec_to_eye = normalize(CameraPos - passPosition);
	float3 half_vector = vec_to_light + vec_to_eye;
	half_vector = normalize(half_vector);
	float half_dot_normal = saturate(dot(half_vector, normal));
	float intensity = pow(half_dot_normal, g_specPower) * specular_intensity * attenuation;

	specColor += intensity * lightColor;

	float4 ambient4 = float4(ambientLight.r, ambientLight.g, ambientLight.b, 0.f);
	float4 surface4 = float4(surfaceLight.r, surfaceLight.g, surfaceLight.b, 0.f);
	float4 spec4 = float4(specColor.r, specColor.g, specColor.b, 0.f);

	//return float4(normal.x, normal.y, normal.z, 0.f);
	return txDiffuse * (ambient4 + surface4) + spec4;
}

//---------------------------------------------------------------------------------------------------------------------------
VS_OUTPUT VS(float3 pos : POSITION, float4 color : COLOR, float2 tex : TEXCOORD, float4 tan : TANGENT, float4 bitan : BINORMAL, float4 norm : NORMAL) {

	VS_OUTPUT output = (VS_OUTPUT)0;

	output.pos		= mul(float4(pos.x, pos.y, pos.z, 1.f), uModel);
	output.passPos	= mul(float4(pos.x, pos.y, pos.z, 1.f), uModel);
	output.pos = mul(output.pos, uView);
	output.pos = mul(output.pos, uProjection);

	output.color = color;
	output.tex = tex;

	output.tan = float3(tan.x, tan.y, tan.z);
	output.bitan = float3(bitan.x, bitan.y, bitan.z);
	output.norm = float3(norm.x, norm.y, norm.z);


	return output;
}


//---------------------------------------------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target {

	float4 texNorm = txNormal.Sample(samLinear, input.tex);
	float4 texDiffuse = txDiffuse.Sample(samLinear, input.tex);
	float3 pos3 = float3(input.passPos.x, input.passPos.y, input.passPos.z);
	return CalculateLight(pos3, input.tan, input.bitan, input.norm, texNorm, texDiffuse);

	//return float4(surfaceNormal.x, surfaceNormal.y, surfaceNormal.z, 1.f);
	//return txNormal.Sample(samLinear, input.tex);
}