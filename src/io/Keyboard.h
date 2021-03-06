//
// Created by Milan van Zanten on 10.02.18.
//

#ifndef BESTEST_GAME_KEYBOARD_H
#define BESTEST_GAME_KEYBOARD_H

#include <functional>
#include <vector>
#include <unordered_map>
#include <GLFW/glfw3.h>

class Keyboard {
public:
    // control
    /// Tell the keyboard a specific key has been pressed.
    /// \param key The key that is pressed.
    /// \param scancode A code for the pressed key that is unique on this platform.
    /// \param mods The mods used when pressing this key.
    void pressKey(int key, int scancode, int mods);
    /// Tell the keyboard a specific key has been repeated.
    /// \param key The key that is repeated.
    /// \param scancode A code for the repeated key that is unique on this platform.
    /// \param mods The mods used when repeating this key.
    void repeatKey(int key, int scancode, int mods);
    /// Tell the keyboard a specific key has been released.
    /// \param key The key that is released.
    /// \param scancode A code for the released key that is unique on this platform.
    /// \param mods The mods used when releasing this key.
    void releaseKey(int key, int scancode, int mods);

    // query
    /// Add a callback that is triggered whenever a key is pressed.
    /// \param callback The callback to call whenever a key is pressed. This keyboard, key, scancode and modifiers are passed to the callback.
    /// \param key The key for which this callback should be called. (Default is -1 and means the callback is called for any key).
    void addPressedCallback(std::function<void(Keyboard&, int, int, int)> callback, int key = -1);
    /// Add a callback that is triggered whenever a key is repeated.
    /// \param callback The callback to call whenever a key is repeated. This keyboard, key, scancode and modifiers are passed to the callback.
    /// \param key The key for which this callback should be called. (Default is -1 and means the callback is called for any key).
    void addRepeatedCallback(std::function<void(Keyboard&, int, int, int)> callback, int key = -1);
    /// Add a callback that is triggered whenever a key is released.
    /// \param callback The callback to call whenever a key is released. This keyboard, key, scancode and modifiers are passed to the callback.
    /// \param key The key for which this callback should be called. (Default is -1 and means the callback is called for any key).
    void addReleasedCallback(std::function<void(Keyboard&, int, int, int)> callback, int key = -1);
    /// Return whether a key is pressed down currently.
    /// \param window The window on which to query the key state.
    /// \param key The key whose state to query.
    /// \return Whether the key state is GLFW_PRESS.
    bool keyPressed(GLFWwindow *window, int key);

private:
    std::unordered_map<int, std::vector<std::function<void(Keyboard&, int, int, int)>>> keyPressedCallbacks;
    std::unordered_map<int, std::vector<std::function<void(Keyboard&, int, int, int)>>> keyRepeatedCallbacks;
    std::unordered_map<int, std::vector<std::function<void(Keyboard&, int, int, int)>>> keyReleasedCallbacks;
};

#endif //BESTEST_GAME_KEYBOARD_H
