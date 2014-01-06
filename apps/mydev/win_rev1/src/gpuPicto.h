//
//  gpuPicto.h
//  osx_picto
//
//  Created by matobahiroshi on 2013/08/30.
//
//

#pragma once
#include "gpuPictoString.h"
#include "testApp.h"
class gpuPictoChar;

class gpuPicto{
    
public:
    
    static int totalPicto;
    static int begin;
    
    int index;
    gpuPictoChar * parent;

    gpuPicto(int _index):index(_index){
        totalPicto++;
        begin++;
        int t = testApp::gps->textureRes;
        if(begin == t*t){
            begin=0;
        }
    };
    
    ~gpuPicto(){
        totalPicto--;
    }

};