#version 120
#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable

uniform int size;
uniform float imgWidth, imgHeight;

varying in float iconTypeVS[gl_VerticesIn];
varying out float iconType;

void main(void){
   for(int i = 0; i < gl_VerticesIn; i++){

        gl_FrontColor = gl_FrontColorIn[i];
       
        iconType = iconTypeVS[i];
        
        
        gl_Position = gl_ModelViewProjectionMatrix * ( gl_PositionIn[i] + vec4(-size,-size,0.0,0.0));
        gl_TexCoord[0].x = 0.0;
        gl_TexCoord[0].y = 0.0;
        
        EmitVertex();
        
        gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(size,-size,0.0,0.0));
        gl_TexCoord[0].x = imgWidth;
        gl_TexCoord[0].y = 0.0;

        EmitVertex();
        
        gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(size,size,0.0,0.0));
        gl_TexCoord[0].x = imgWidth;
        gl_TexCoord[0].y = imgHeight;

        EmitVertex();
        EndPrimitive();

        gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(-size,-size,0.0,0.0));
        gl_TexCoord[0].x = 0.0;
        gl_TexCoord[0].y = 0.0;

        EmitVertex();
        
        gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(-size,size,0.0,0.0));
        gl_TexCoord[0].x = 0.0;
        gl_TexCoord[0].y = imgHeight;

        EmitVertex();
        
        gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(size,size,0.0,0.0));
        gl_TexCoord[0].x = imgWidth;
        gl_TexCoord[0].y = imgHeight;

        EmitVertex();
        EndPrimitive();
 
    }
    

}