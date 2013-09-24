#version 120
#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect posTex;
uniform sampler2DRect iconPrmTex;
uniform sampler2DRect springData;
uniform vec2 screen;
uniform float resolution;
uniform vec2  offset;


varying vec4 iconPrmVS;
varying float alphaVS;
void main() {
    vec2 vertexPos = gl_MultiTexCoord0.xy;
    
    vec4 pixPos = texture2DRect( posTex, vertexPos );

    pixPos.x *= screen.x;
    pixPos.y *= screen.y;

    alphaVS         = pixPos.z;
    

    gl_Position     = pixPos;
    gl_FrontColor   =  gl_Color;
 
    iconPrmVS       = texture2DRect(iconPrmTex, vertexPos);
}