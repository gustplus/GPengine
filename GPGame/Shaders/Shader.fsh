//
//  Shader.fsh
//  GPEngine
//
//  Created by shizhan on 14-8-30.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
