struct VS_INPUT
{
	float4 position: POSITION;
	float3 color: COLOR;
	float3 color1: COLOR1;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float3 color: COLOR;
	float3 color1: COLOR1;
};


cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;

	unsigned int m_time;
};



VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;


	//World Space
	output.position = mul(input.position, m_world);

	//VIEW Space
	output.position = mul(output.position, m_view);

	//SCREEN Space
	output.position = mul(output.position, m_proj);


	//converting steping up 1 by 1 
	//local to world
	//word to view
	//view to screen

	output.color = input.color;
	output.color1 = input.color1;
	return output;
}