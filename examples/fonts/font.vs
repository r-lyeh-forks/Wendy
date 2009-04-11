
struct Out
{
  float4 position : POSITION;
  float2 mapping;
};

Out main(uniform float4x4 P, in float2 position, in float2 mapping)
{
  Out result;
  result.position = mul(P, float4(position, 0, 1));
  result.mapping = mapping;
  return result;
}
