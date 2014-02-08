#version 120
#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect posTex;
uniform sampler2DRect iconPrmTex;
uniform vec2 screen;
uniform float resolution;

varying float iconTypeVS;

void main() {
    vec2 vertexPos = gl_MultiTexCoord0.xy;
    gl_Position =  ftransform();
    
    vec4 iconPrm = texture2DRect(iconPrmTex, vertexPos);
    gl_FrontColor = iconPrm;
    
}