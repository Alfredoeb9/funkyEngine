#include "InputManager.h"

FunkyEngine::InputManager::InputManager() {

}

FunkyEngine::InputManager::~InputManager() {

}

void FunkyEngine::InputManager::updateKeyMaps() {
    // Update the key pressed map to the current key map for the next frame
    for (auto& key : _keyMap) {
        _keyPressedMap[key.first] = key.second;
    }
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

bool FunkyEngine::InputManager::isKeyDown(unsigned int keyID) {
    auto iter = _keyMap.find(keyID);

    // if it did find the key
    if (iter != _keyMap.end()) {
        return iter->second;
    } else {
        return false;
    }
}

bool FunkyEngine::InputManager::isKeyPressed(unsigned int keyID) {
    // If it was pressed in the current frame but not previous frame, return true
    if (isKeyDown(keyID) == true && wasKeyDown(keyID) == false) {
        return true;
    } 
    return false;
}

bool FunkyEngine::InputManager::wasKeyDown(unsigned int keyID) {
    auto iter = _keyPressedMap.find(keyID);

    // if it did find the key
    if (iter != _keyPressedMap.end()) {
        return iter->second;
    }

    return false;
    
}