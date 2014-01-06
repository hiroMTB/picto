#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect prevPosData;  // recive the previus position texture
uniform sampler2DRect velData;      // recive the velocity texture
uniform sampler2DRect springData;

uniform float timestep;
//uniform vec2  offset;
//uniform vec2  pastOffset;


void main(void){
    vec2 st = gl_TexCoord[0].st;
    
    vec2 globalPos = texture2DRect( prevPosData, st ).xy;
//    vec2 localPos = globalPos - pastOffset;
    
    vec3 vela = texture2DRect( velData, st ).xyz;
    vec2 vel = vela.xy;
    

    float attractOn = texture2DRect( springData, st).a;
    float minSpeed =  texture2DRect( springData, st).z;
    float fixCount = texture2DRect( prevPosData, st ).a;


//    if(0<attractOn){
//        globalPos = localPos + offset;
//    }

    globalPos += vel * timestep;
    

    float a = texture2DRect( prevPosData, st ).z;
   
    
    a *= 1.05;
    if(a>0.9){
        a=0.9;
    }

//    a = 1;
    
    gl_FragColor.rgba = vec4(globalPos.x, globalPos.y, a, 1.0);
}