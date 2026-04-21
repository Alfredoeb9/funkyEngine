#include "InputManager.h"

FunkyEngine::InputManager::InputManager() {

}

FunkyEngine::InputManager::~InputManager() {

}

void FunkyEngine::InputManager::updateKeyMaps() {
    for (auto& key : _keyPressedMap) {
        // If the key was pressed in the current frame, set it to true in the key map
        if (key.second) {
            _keyMap[key.first] = true;
        }
    }
}


void FunkyEngine::InputManager::pressKey(unsigned int keyID) {
    // First key if this key ID is in the map and if its not then set it to true
    _keyPressedMap[keyID] = true;
    _keyMap[keyID] = true;
}

void FunkyEngine::InputManager::releaseKey(unsigned int keyID) {
    _keyMap[keyID] = false;
    _keyPressedMap[keyID] = false;
}

void FunkyEngine::InputManager::setMouseCoords(float x, float y) {
    _mouseCoords.x = x;
    _mouseCoords.y = y;
}

bool FunkyEngine::InputManager::isKeyPressed(unsigned int keyID) {
    auto iter = _keyPressedMap.find(keyID);

    // if it did find the key
    if (iter != _keyPressedMap.end()) {
        return iter->second;
    } else {
        return false;
    }
}

bool FunkyEngine::InputManager::isKeyDown(unsigned int keyID) {
    return isKeyPressed(keyID) || _keyMap[keyID];
}
