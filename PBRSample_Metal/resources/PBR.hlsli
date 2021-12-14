float Fresnel(float4 lightDir, float4 normal, float n1, float n2)
{
    
    //float result = 0.0f;
    //float4 outGoing = reflect(lightDir, normal);
    
    //float4 h = normalize((outGoing + lightDir) / length(outGoing + lightDir));
    //float theta = dot(outGoing, h);
    
    //float f1 = 0.1f;
    
    //result = f1 + (1.0f - f1) * pow(1.0f - theta, 5.0f);
    
    float result = 0.0f;
    float r0 = (n1 - n2) / (n1 + n2);
    float r0sq = r0 * r0;
    
    float cosTheta = saturate(dot(lightDir, normal));
    
    float rTheta = r0 + (1.0f - r0) * pow((1.0f - cosTheta), 5.0f);
    
    return rTheta;
}