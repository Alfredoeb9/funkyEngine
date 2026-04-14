#include "InputManager.h"

FunkyEngine::InputManager::InputManager() {

}

FunkyEngine::InputManager::~InputManager() {

}

void FunkyEngine::InputManager::pressKey(unsigned int keyID) {
    // First key if this key ID is in the map and if its not then set it to true
    _keyMap[keyID] = true;
}

void FunkyEngine::InputManager::releaseKey(unsigned int keyID) {
    _keyMap[keyID] = false;
}

void FunkyEngine::InputManager::setMouseCoords(float x, float y) {
    _mouseCoords.x = x;
    _mouseCoords.y = y;
}

bool FunkyEngine::InputManager::isKeyPressed(unsigned int keyID) {
    auto iter = _keyMap.find(keyID);

    // if it did find the key
    if (iter != _keyMap.end()) {
        return iter->second;
    } else {
        return false;
    }
}