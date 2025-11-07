
struct VS_INPUT 
{
    float3 position : LOCATION0;
    float3 normals : LOCATION1;
    float3 tangents : LOCATION2;
    float3 bitangents : LOCATION3;
    float2 textureCoord : LOCATION4;
};


struct VS_OUTPUT 
{
    float3 position : LOCATION0;
    float3 normals : LOCATION1;
    float3 tangents : LOCATION2;
    float3 bitangents : LOCATION3;
    float2 textureCoord : LOCATION4;
    float4 gl_Position : SV_Position;
};


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.position = input.position;
    output.normals = input.normals;
    output.tangents = input.tangents;
    output.bitangents = input.bitangents;
    output.textureCoord = input.textureCoord;
    output.gl_Position = float4(input.position, 1.0f);

    return output;
}
