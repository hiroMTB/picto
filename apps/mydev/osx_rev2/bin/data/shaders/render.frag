#version 120
#extension GL_ARB_texture_rectangle : enable

// This fill the billboard made on the Geometry Shader with a texture

uniform sampler2DRect sparkTex;
varying vec4 iconPrm;
varying float alpha;
uniform float imgWidth, imgHeight;

void main() {
    vec2 st = gl_TexCoord[0].st;

    vec4 color = vec4(0,0,0,0);
    vec4 prm = gl_Color;
    float colorType = iconPrm.x;
       
    if(0<=colorType && colorType<0.01){
        color.r = 0;            //   0      // water blue
        color.g = 0.7607843137; // 194
        color.b = 0.8980392157; // 229
    }else if(0.01<=colorType && colorType<0.02){
        color.r = 1;            // 255      // yellow
        color.g = 0.8039215686; // 205
        color.b = 0.03921568627;//  10
    }else if(0.02<=colorType && colorType<0.03){
        color.r = 0.6470588235; // 165      // purple
        color.g = 0.3607843137; // 92
        color.b = 0.7529411765; // 192
    }else if(0.03<=colorType && colorType<0.04){
        color.r = 0.03921568627;//  10      // light green
        color.g = 0.7647058824; // 195
        color.b = 0.3607843137; //  92
    }else if(0.04<=colorType && colorType<0.05){
        color.r = 0.9803921569; // 250      // light red
        color.g = 0.2745098039; //  70
        color.b = 0.3254901961; //  83
    }else if(0.05<colorType){
        // error
        color.r = 1;
        color.g = 1;
        color.b = 0.2;
    }

    
    int iconType = int( iconPrm.y*100.0);  // 0-43, 44 type

    float w = 0;
    float h = 0;
    
    w = float(iconType);
    while (w>=10.0) {
        w -= 10.0;
    }
    

    h= imgHeight * int(iconType/10.0);
    w = imgWidth * int(w);
    
    st.x += w;
    st.y += h;
    
    vec4 imgColor = texture2DRect(sparkTex, st);
    imgColor.a *= alpha;
    
    gl_FragColor = color + imgColor;
    
}