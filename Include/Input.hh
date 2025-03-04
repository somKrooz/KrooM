#pragma once

#include <KObject.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Shape.hh>
#include <stdio.h>
#include <string>
#include <Text.hh>
#include <memory>

class KroozInput : public KObject {
private:
    float x, y, width, height;
    GLFWwindow* window;
    std::unique_ptr<KroozFont> fnt;
    float padding = 5.0f; // Padding inside the text box

public:
    inline static std::string keyLog = "";

    KroozInput(GLFWwindow* window, float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height), window(window), fnt(std::make_unique<KroozFont>()) {
        glfwSetKeyCallback(this->window, keyCallback);
        fnt->loadDefaultFont();
    }

    void DrawBox() {
        Rect rect = {x, y, width, height, 0.1f, 0.1f, 0.1f};
        Shape::DrawRect(rect);
    }

  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_BACKSPACE && !keyLog.empty()) {
            // Handle backspace
            keyLog.pop_back();
        } else if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
            char c = 'a' + (key - GLFW_KEY_A);
            if (mods & GLFW_MOD_SHIFT) {
                c = toupper(c);
            }
            keyLog += c;
        } else if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
            keyLog += '0' + (key - GLFW_KEY_0);
        } else if (key == GLFW_KEY_SPACE) {
            keyLog += ' ';
        } else if (key == GLFW_KEY_ENTER) {
            keyLog += '\n';
        } else if (key == GLFW_KEY_PERIOD) {
            keyLog += '.';
        } else if (key == GLFW_KEY_COMMA) {
            keyLog += ',';
        } else if (key == GLFW_KEY_END) {
            // Handle Ctrl+V (Paste)
            const char* clipboardText = glfwGetClipboardString(window);
            if (clipboardText) {
                keyLog += clipboardText;
            }
        }
    }
  }

    void Update(float deltaTime, GLFWwindow* window, double mx, double my) override {}

    void Render() override {
        DrawBox();

        float fontSize = height * 0.6f;
        fnt->SetFontSize(fontSize);

        // Handle text overflow
        std::string displayText = keyLog;
        float textWidth = fnt->getTextWidth(displayText);

        // If text is too wide, trim it from the beginning
        while (textWidth > width - 2 * padding && !displayText.empty()) {
            displayText = displayText.substr(1);
            textWidth = fnt->getTextWidth(displayText);
        }

        // Calculate text position for vertical and horizontal centering
        float textHeight = fnt->getTextHeight();
        float centeredX = x + padding;
        float centeredY = y + (height - textHeight) / 2;

        fnt->SetColor(0.9f, 0.9f, 0.9f);
        fnt->renderText(displayText, centeredX, centeredY);
    }

    std::string getText() const {
        return keyLog;
    }

    void setText(const std::string& text) {
        keyLog = text;
    }

    void clear() {
        keyLog.clear();
    }
};
