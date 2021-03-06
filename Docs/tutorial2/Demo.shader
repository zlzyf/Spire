// import pipeline definition
using "MultiRate.pipeline";

module VertexInput
{
    // define vertex inputs
    public @rootVert vec3 vert_pos;
    public @rootVert vec3 vert_normal;
    public @rootVert vec3 vert_tangent;
    public @rootVert vec2 vert_uv;   
}

module SystemUniforms
{
    // define engine provided uniforms
    public @modelTransform mat4 modelMatrix; 
    public @modelTransform mat4 normalMatrix;
    
    // define view and environment uniform inputs
    public @viewUniform mat4 viewProjectionMatrix;
    public @viewUniform mat4 viewMatrix;
    public @viewUniform mat4 projectionMatrix;
    public @viewUniform mat4 invViewMatrix;
    public @viewUniform mat4 invViewProjectionMatrix;
    public @viewUniform vec3 cameraPos;
    public @viewUniform vec3 lightColor;
    public @viewUniform vec3 lightDir;
}

module VertexTransform
{
    require mat4 modelMatrix;
    require mat4 viewProjectionMatrix;
    require vec3 vert_pos;
    require vec2 vert_uv;
      
    // compute projected vertex position
    public vec4 position = modelMatrix * vec4(vert_pos, 1.0);
    public vec4 projCoord = viewProjectionMatrix * position;
    
    // pipeline requires a texture space vertex for object space rendering
    // here we require the mesh provides a unique parameterization stored in vert_uv
    public vec4 texSpaceVert = vec4(vert_uv*2.0 - vec2(1.0), 0.0, 1.0);           
    public vec2 screenCoord = projCoord.xy/vec2(projCoord.w)*0.5 + vec2(0.5);
}

module Material
{
    require vec2 vert_uv;
    
    // define material uniform inputs
    @perInstanceUniform sampler2D ground_pebble_map;
    @perInstanceUniform sampler2D ground_pebble_Nmap;    
    
    vec2 uv = vert_uv * 4.0; // tile the texture
    public vec3 albedo = texture(ground_pebble_map, uv).xyz;

    public vec3 normal
    {
        // fetch normal map
        vec3 normalTex = (texture(ground_pebble_Nmap, uv).xyz - 0.5) * 2.0;
        return normalTex;
    }
}

module TangentFrameTransform
{
    require vec3 inputVector;
    require mat4 normalMatrix;
    require vec3 vert_normal;
    require vec3 vert_tangent;
    
    vec3 vNormal = (normalMatrix * vec4(vert_normal, 1.0)).xyz;
    vec3 vTangent = (normalMatrix * vec4(vert_tangent, 1.0)).xyz;
    vec3 vBiTangent = cross(vTangent, vNormal);
    
    public vec3 result = normalize(inputVector.x * vTangent 
                    + inputVector.y * vBiTangent 
                    + inputVector.z * vNormal);
}

module Lighting
{
    require vec3 albedo;
    require vec3 normal;
    require vec3 lightColor;
    require vec3 lightDir;
    require vec4 position;
    require vec3 cameraPos;
    
    vec3 view = normalize(cameraPos - position.xyz);
    // compute lighting
    float diffuse = clamp(dot(lightDir, normal), 0.0, 1.0);
    float specular = ComputeHighlightPhong(lightDir, normal, view, 0.5, 0.5, 0.4);  
    public vec4 result = vec4(lightColor * 
         (albedo * (diffuse * 0.7 + 0.5) * 0.6 + 
         mix(albedo, vec3(1.0), 0.6) * specular), 1.0);  
}

// define the shader for our demo
shader DemoShader
{
    using VertexInput;
    using SystemUniforms;
    using VertexTransform;
    using material = Material();
    using normalTransform = TangentFrameTransform(material.normal);
    using lighting = Lighting(material.albedo, normalTransform.result);
    vec4 outputColor = lighting.result;
}