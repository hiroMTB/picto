#version 120
#extension GL_ARB_texture_rectangle : enable

// This fill the billboard made on the Geometry Shader with a texture

uniform sampler2DRect sparkTex;
varying vec4 iconPrm;
varying float alpha;
uniform float imgWidth, imgHeight;

void main() {
    vec2 st = gl_TexCoord[0].st;

    vec4 color = vec4(1,0,0,0);
    vec4 prm = gl_Color;
    float colorType = iconPrm.x;
    int iconType = int( iconPrm.y / 0.02272 );  // 0-43

    float w = 0;
    float h = 0;

    w = float(iconType);
    while (w>10.0) {
        w -= 10.0;
    }

    h = imgHeight * int(iconType/10);
    w = imgWidth * int(w);
    
    if(0<colorType){
        if(colorType<0.25){
            color.r = 0;            //   0      // water blue
            color.g = 0.7607843137; // 194
            color.b = 0.8980392157; // 229
        }else if(colorType<0.4){
            color.r = 1;            // 255      // yellow
            color.g = 0.8039215686; // 205
            color.b = 0.03921568627;//  10
        }else if(colorType<0.55){
            color.r = 0.6470588235; // 165      // purple
            color.g = 0.3607843137; // 92
            color.b = 0.7529411765; // 192
        }else if(colorType<0.65){
            color.r = 0.03921568627;//  10      // light green
            color.g = 0.7647058824; // 195
            color.b = 0.3607843137; //  92
        }else if(colorType<1.0){
            color.r = 0.9803921569; // 250      // light red
            color.g = 0.2745098039; //  70
            color.b = 0.3254901961; //  83
        }
    }else{
        color.r = 1;
        color.g = 1;
        color.b = 1;
    }
    
    st.x += w;
    st.y += h;
    
    vec4 imgColor = texture2DRect(sparkTex, st);
    imgColor.a *= alpha;
    
    gl_FragColor = color + imgColor;
    
}


//    GANSOSU MOSS COLOR
//
//    if(colorType<0.2){
//        color.r = 0.4724409449; // 120
//        color.g = 0.5333333333; // 136
//        color.b = 0.1764705882; //  45
//    }else if(colorType<0.4){
//        color.r = 0.5450980392; // 139
//        color.g = 0.537254902; // 137
//        color.b = 0.2156862745; // 55
//    }else if(colorType<0.6){
//        color.r = 0.1137254902; // 29
//        color.g = 0.3098039216; // 79
//        color.b = 0.05490196078; // 14
//    }else if(colorType<0.8){
//        color.r = 0.2980392157; // 76
//        color.g = 0.5764705882; // 147
//        color.b = 0.06274509804; // 16
//    }else if(colorType<1.0){
//        color.r = 0.4196078431; // 107
//        color.g = 0.5294117647; // 135
//        color.b = 0.3019607843; // 77
//    }else{
//        color.r = 1;
//        color.g = 1;
//        color.b = 1;
//    }