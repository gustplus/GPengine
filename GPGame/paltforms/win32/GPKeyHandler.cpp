#include "stdafx.h"
#include "GPKeyHandler.h"
#include <string>

using namespace GPEngine3D;

KeyHandler::KeyHandler(void):rollDir(0), lMouseButton(false), rMouseButton(false),mMouseButton(false)
{
	memset(keys, 0, 256);
}

KeyHandler::~KeyHandler(void)
{
}

KeyHandler* KeyHandler::keyHandler = new KeyHandler();  

KeyHandler *KeyHandler::getInstance(){
	return keyHandler;
}

bool KeyHandler::isKeyDown(unsigned char key){
	return keys[key];
}

bool KeyHandler::isMouseLeftDown(){
	return lMouseButton;
}

bool KeyHandler::isMouseRightDown(){
	return rMouseButton;
}

