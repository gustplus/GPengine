//
//  GPViewController.h
//  GPEngine
//
//  Created by shizhan on 14-8-30.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#import "GPGLShaderProgram.h"

@interface GPViewController : GLKViewController
{
//    GLuint _program;
    GPEngine3D::GLShaderProgramPtr program;
    
    GLKMatrix4 _modelViewProjectionMatrix;
    GLKMatrix3 _normalMatrix;
    float _rotation;
    
    GLuint _vertexArray;
    GLuint _vertexBuffer;
}
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;

- (BOOL)loadShaders;
@end
