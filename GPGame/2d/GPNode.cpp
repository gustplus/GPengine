//
//  GPNode.cpp
//  GPEngine
//
//  Created by shizhan on 14/11/2.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#include "GPNode.h"

namespace GPEngine3D {
    Node::Node(float x = 0, float y = 0):
    _position(x, y),
    _worldPosition(0,0),
    _anthorPoint(0,0),
    _visible(true),
    _scaleX(1),
    _scaleY(1),
    _rotation(0),
    _zOrder(0),
    _tag(0),
    _depth(0),
    _isTransformCurrent(false),
    _children(10),
    _parent(nullptr),
    _childrenCount(0),
    _worldTransform(),
    _selfTransform()
    {
        _contentBound.x      = 0;
        _contentBound.y      = 0;
        _contentBound.width     = 0;
        _contentBound.height    = 0;
        
        _selfBound.x         = 0;
        _selfBound.y         = 0;
        _selfBound.width        = 0;
        _selfBound.height       = 0;
    }
    
    Node::~Node()
    {
        
    }
    
    Node *Node::getChildByTag(unsigned int tag)
    {
        for(auto child : _children)
        {
            if (child->getTag() == tag)
            {
                return child;
            }
        }
        return nullptr;
    }
    
    bool Node::addChild(Node *child)
    {
        if (child->getParent()){
#ifdef DEBUG
            GP_ASSERT(false, "the child has a parent, can't add again!");
#endif
            return false;
        }else{
            this->_children.pushBack(child);
            child->setParent(this);
            this->updateChildrenDepth();
            return true;
        }
    }
    
    bool Node::addChild(Node *child, int zOrder)
    {
        if (this->addChild(child) )
        {
            this->_zOrder = zOrder;
            return true;
        }
        return false;
    }
    
    bool Node::addChild(Node *child, int zOrder, unsigned int tag)
    {
        if ( this->addChild(child, zOrder) )
        {
            this->_tag = tag;
            return true;
        }
        return false;
    }
    
    void Node::removeChild(Node *child)
    {
        if(!_children.empty()){
            _children.eraseObject(child);
        }
    }
    
    void Node::removeChildByTag(unsigned int tag)
    {
        if(_children.empty()){return;}
        Node *child = this->getChildByTag(tag);
        if (child){
            this->removeChild(child);
        }
    }
    
    void Node::removeFromParent()
    {
        if(_children.empty()){return;}
        auto parent = this->getParent();
        if (parent){
            parent->removeChild(this);
        }
    }
    
    void Node::updateTransformState(const Transform &parentTransform, bool isParentChange)
    {
        if (!_isTransformCurrent)
        {
            _selfTransform.scale(vec3f(_scaleX, _scaleY, 1));
            _selfTransform.rotate(_rotation, vec3f(0, 0 ,1));
            _selfTransform.translate(vec3f(_position, 0));
            _worldTransform = _selfTransform * _worldTransform;
            _worldPosition = _worldTransform.transformVector2(_position);
        }
        if (isParentChange)
        {
            _worldTransform = _selfTransform * _worldTransform;
            _worldPosition = _worldTransform.transformVector2(_position);
            
        }
        if(!_children.empty())
        {
            for(auto child : _children)
            {
                child->updateTransformState(_worldTransform, isParentChange || _isTransformCurrent);
            }
        }
        _isTransformCurrent = true;
    }
}