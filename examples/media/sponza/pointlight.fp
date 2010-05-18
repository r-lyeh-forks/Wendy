
struct Light
{
  float3 position;
  float3 color;
};

float4 main(uniform samplerRECT colorbuffer,
            uniform samplerRECT normalbuffer,
            uniform samplerRECT depthbuffer,
            uniform float nearZ,
            uniform float nearOverFarZminusOne,
            uniform Light light,
            in float2 mapping,
            in float2 clipOverF) : COLOR
{
  float minusPz = nearZ / (texRECT(depthbuffer, mapping).r * nearOverFarZminusOne + 1);

  float3 P = float3(clipOverF.x * minusPz, clipOverF.y * minusPz, -minusPz);
  float3 L = normalize(light.position - P);
  float3 N = normalize(texRECT(normalbuffer, mapping).xyz - 0.5);

  float Ia = 0.2;
  float Id = saturate(dot(N, L));

  float3 Cs = texRECT(colorbuffer, mapping).rgb;

  return float4(Cs * light.color * (Ia + Id), 1);
}
