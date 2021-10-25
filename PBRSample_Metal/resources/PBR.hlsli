float Fresnel(float4 lightDir, float4 normal, float n1, float n2)
{
    
    float result = 0.0f;
    float4 outGoing = normalize(reflect(lightDir, normal));
    
    float4 h = normalize((outGoing + lightDir) / length(outGoing + lightDir));
    float theta = dot(outGoing, h);
    
    float f1 = 0.1f;
    
    result = f1 + (1.0f - f1) * pow(1.0f - theta, 5.0f);
    
    return result;
}