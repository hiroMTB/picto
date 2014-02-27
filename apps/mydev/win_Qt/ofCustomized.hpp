//
//  graphics/ofCairoRenderer.cpp
//
void ofCairoRenderer::viewport(float x, float y, float width, float height, bool invertY){

    //...

    if (invertY){
        // CUSTOMIZED
        // y = ofGetWindowHeight() - (y + height);      // ofGetWindowHeight returns wrong value.
        y = height - (y + height);
    }

    // ...
}

