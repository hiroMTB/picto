#version 120
#extension GL_ARB_texture_rectangle : enable
#define KERNEL_SIZE 9

uniform sampler2DRect backbuffer;   // recive the previus velocity texture
uniform sampler2DRect posData;      // recive the position texture
uniform sampler2DRect springData;
uniform sampler2DRect randomData;
uniform sampler2DRect targetData;

uniform vec2  screen;
uniform vec2  offset;
uniform vec2  pastOffset;

uniform float timestep;
uniform float ACCEL;
uniform float SPEED;
uniform float VIBRATION;

void main(void){
    vec2 st = gl_TexCoord[0].st;
    
    vec2 globalPos  = texture2DRect( posData, st).xy;

//    vec2 localPos   = globalPos - pastOffset;
    vec4 velf      = texture2DRect( backbuffer, st );
    vec2 vel        = velf.xy;
    float fixCount  = velf.z;

    float length    = length(vel);

    vec4 spring     = texture2DRect( springData, st );
    vec4 random     = texture2DRect( randomData, st );
    
    float K         = spring.x * (ACCEL*100);
    float topSpeed  = spring.y  * SPEED*0.1;
    float minSpeed  = spring.z  * VIBRATION;
    float attractOn = spring.a;

    vec2 target     = vec2(0,0);
    vec2 dir        = vec2(0,0);


    if(0<attractOn){
        // local
        target = offset + (random.xy-vec2(0.5, 0.5));
        dir = target - globalPos;
    }else{
        // global
        target = texture2DRect( targetData, st ).xy;
        dir = target - globalPos;
    }


    if(0<attractOn){
//        if(length(dir)>random.z*0.8){
//            vel += dir;
//        }
        vec2 acc = dir * K * 0.000167;
        vel += acc*1.2;


        fixCount = 0;
    }else{

        if(length>minSpeed){
            vec2 acc = dir * K * 0.000167 * 4.0;
            vel *= 0.98;
            vel += acc;
        }else{
 
            if(fixCount<0.5){
                fixCount = fixCount+0.01;
                float sp = length(vel);
                vel = normalize(dir) * sp;
                vel*=0.99;
            }
            
            vec2 acc = dir * K * 0.000167;
            vel += acc;
            
            if(length(vel)<minSpeed*0.1){
                vel = normalize(vel) * minSpeed*0.1;
            }
        }
        topSpeed *= 0.7;
    }


    length = length(vel);

    // LIMIT
    if(length>(topSpeed)){
        vel = normalize(vel) * topSpeed;
    }


    gl_FragColor = vec4(vel.x, vel.y, fixCount, 1);   // Then save the vel data into the velocity FBO
}