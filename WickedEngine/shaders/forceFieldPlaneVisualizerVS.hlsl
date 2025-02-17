#include "globals.hlsli"

struct PSIn
{
	float4 pos : SV_POSITION;
	float4 pos3D : TEXCOORD0;
	float4 pos2D : TEXCOORD1;
};

PSIn main(uint vID : SV_VERTEXID)
{
	PSIn Out;

	Out.pos = float4(CreateCube(vID) * 2 - 1, 1);

	uint forceFieldID = g_xFrame.ForceFieldArrayOffset + (uint)g_xColor.w;
	ShaderEntity forceField = load_entity(forceFieldID);

	Out.pos.xyz *= forceField.GetConeAngleCos(); // range...
	Out.pos.xyz += forceField.position;

	Out.pos3D = Out.pos;

	Out.pos = mul(g_xTransform, Out.pos);
	Out.pos2D = Out.pos;

	return Out;
}
