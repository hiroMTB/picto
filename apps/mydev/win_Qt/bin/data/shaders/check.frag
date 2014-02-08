#version 120
#extension GL_ARB_texture_rectangle : enable

// This fill the billboard made on the Geometry Shader with a texture

uniform sampler2DRect sparkTex;
uniform float imgWidth, imgHeight;

varying float iconType;

void main() {

    gl_FragColor = gl_Color;
}