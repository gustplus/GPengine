//
//  Shader.vsh
//  GPEngine
//
//  Created by shizhan on 14-8-30.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

attribute vec4 position;
attribute vec3 normal;

varying lowp vec4 colorVarying;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

void main()
{
    vec4 diffuseColor = vec4(0.4, 0.4, 1.0, 1.0);
    colorVarying = diffuseColor
    
    gl_Position = modelViewProjectionMatrix * position;
}
