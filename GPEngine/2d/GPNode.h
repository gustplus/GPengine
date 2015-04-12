//
//  GPNode.h
//  GPEngine
//
//  Created by shizhan on 14/11/2.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#ifndef __GPEngine__GPNode__
#define __GPEngine__GPNode__

#include <stdio.h>
#include "GPObject.h"
#include "GPVector.h"
#include "GPTransform.h"
#include "GPArrayList.h"
#include "gp.h"
#include "GPRect.h"

namespace GPEngine3D{
    class Node;
    
    DEFINE_SMARTPTR(Node, NodePtr);
    
    class Node :
        public Object
    {
    public:
        Node();
        Node(float x, float y);
        virtual ~Node();
        
        bool addChild(Node *child);
        bool addChild(Node *child, int zOrder);
        bool addChild(Node *child, int zOrder, unsigned int tag);
        
        void removeChild(Node *child);
        void removeChildByTag(unsigned int tag);
        
        Node *getChildByTag(unsigned int tag);
        
        void updateTransformState(const Transform &parentTransform, bool isParentChange);
        
        void removeFromParent();
        
        const Transform &getWorldTransform()
        {
            return this->_worldTransform;
        }
        
        const Transform &getTransform()
        {
            return _selfTransform;
        }
        
        const vec2f &getPosition() const
        {
            return _position;
        }
        
        void setPosition(vec2f &pos)
        {
            if (pos != _position)
            {
                this->_position = pos;
                this->_isTransformCurrent = false;
            }
        }
        
        void setPosition(float x, float y)
        {
            if (x != _position.x || y != _position.y)
            {
                this->_position.x = x;
                this->_position.y = y;
                this->_isTransformCurrent = false;
            }
        }
        
        void setPositionX(float x)
        {
            if (x != _position.x)
            {
                this->_position.x = x;
                this->_isTransformCurrent = false;
            }
        }
        
        void setPositionY(float y)
        {
            if (y != _position.y)
            {
                this->_position.y = y;
                this->_isTransformCurrent = false;
            }
        }
        
        void setTag(unsigned int tag)
        {
            this->_tag = tag;
        }
        
        unsigned int getTag()
        {
            return _tag;
        }
        
        void setZOrder(int zOrder)
        {
            this->_zOrder = zOrder;
        }
        
        float getZOrder() const
        {
            return this->_zOrder;
        }
        
        virtual void setWidth(float width)
        {
            _selfBound.width = 0;
        }
        
        float getWidth() const
        {
            return _selfBound.width;
        }
        
        virtual void setHeight(float height)
        {
            _selfBound.height = 0;
        }
        
        float getHeight() const
        {
            return _selfBound.height;
        }
        
        void setScaleX(float scaleX)
        {
            if (scaleX != _scaleX)
            {
                this->_isTransformCurrent = false;
                this->_scaleX = scaleX;
            }
        }
        
        void setScale(float scaleX, float scaleY)
        {
            if (scaleX != _scaleX || scaleY != _scaleY)
            {
                this->_scaleX = scaleX;
                this->_scaleY = scaleY;
                this->_isTransformCurrent = false;
            }
        }
        
        float getScaleX() const
        {
            return _scaleX;
        }
        
        float getScaleY() const
        {
            return _scaleY;
        }
        
        void setScaleY(float scaleY)
        {
            if (scaleY != _scaleY)
            {
                this->_isTransformCurrent = false;
                this->_scaleY = scaleY;
            }
        }
        
        void setRotation(float rotation)
        {
            if (rotation != _rotation)
            {
                this->_isTransformCurrent = false;
                this->_rotation = rotation;
            }
        }
        
        float getRotation() const
        {
            return this->_rotation;
        }
        
        NodePtr &getParent()
        {
            return this->_parent;
        }
        
        void setVisible(bool visible)
        {
            this->_visible = visible;
        }
        
        bool isVisible()
        {
            return this->_visible;
        }
        /*
         if the node or it's root parent doesn't attach to a scene, the depth value has no means
         */
        unsigned int getDepth()
        {
            return _depth;
        }
        
    protected:
        void setParent(const NodePtr &parent)
        {
            this->_parent = parent;
        }
        
        void setDepth(unsigned int depth)
        {
            this->_depth = depth;
            this->updateChildrenDepth();
        }
        
        void updateChildrenDepth()
        {
            if (!_children.empty()) {
                for (auto child : _children)
                {
                    child->setDepth(_depth + 1);
                }
            }
        }
        
        unsigned int _depth;              //处于场景树的深度
        vec2f _position;
        vec2f _worldPosition;
        vec2f _anthorPoint;
        
        Rect _contentBound;      //包含子节点的包围盒
        Rect _selfBound;         //自己的包围盒
        
        float _scaleX;
        float _scaleY;
        float _rotation;
        
        int _zOrder;
        unsigned int _tag;
        
        bool _isTransformCurrent;
        bool _isBoundBoxCurrent;
        bool _visible;
        
        Transform _worldTransform;
        Transform _selfTransform;
        
        Vector<NodePtr> _children;
        NodePtr _parent;
        unsigned int _childrenCount;
    };
}

#endif /* defined(__GPEngine__GPNode__) */
