using "../DemoEngine.pipeline";

shader Rock
{
    using Header;
    @perInstanceUniform sampler2D baseNormalMap;
    @perInstanceUniform sampler2D detailNormalMap;
    @perInstanceUniform sampler2D albedoMap;
    @perInstanceUniform sampler2D aoMap;
    @perInstanceUniform sampler2D roughnessMap;

    vec3 Normal
    {
         vec3 binormal = cross(vert_tangent, vert_normal);
         vec3 detailNormal = vec3(texture(detailNormalMap, vert_uv*2.0).xy, 0.0)*2.0-vec3(1.0,1.0,0.0);
         return texture(baseNormalMap, vert_uv).xyz*2.0-vec3(1.0,1.0,1.0) + detailNormal;
    }

    float Roughness = texture(roughnessMap, vert_uv).x;
    float Metallic = 0.4;
    float Specular = 0.7;
    float AO = texture(aoMap, vert_uv).x;
    vec3 Albedo = texture(albedoMap, vert_uv).xyz * AO * 1.5;
    using Footer;
}