//
//  gpuPicto.h
//  osx_picto
//
//  Created by matobahiroshi on 2013/08/30.
//
//

#pragma once
class gpuPictoChar;


class gpuPicto{
    
public:
    
    static int totalPicto;
    
    int index;
    gpuPictoChar * parent;

    gpuPicto(int _index):index(_index){
        totalPicto++;
    };
    
    ~gpuPicto(){
        totalPicto--;
    }

};