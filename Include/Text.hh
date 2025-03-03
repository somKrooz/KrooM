#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_truetype.h"
#include  "KObject.h"

#ifdef _WIN32
    #define DEFAULT_FONT "C:\\Windows\\Fonts\\ARIALBD.TTF"
#else
    #define DEFAULT_FONT "Where Linux Store Its Fonts I Dont Know"
#endif

#define FONT_BITMAP_W 512
#define FONT_BITMAP_H 512

class KroozFont{
private:
  unsigned char ttf_buffer[1 << 20];
  unsigned char font_bitmap[FONT_BITMAP_W * FONT_BITMAP_H];
  GLuint font_texture;
  stbtt_bakedchar cdata[96];

  const char* text;
  float x;
  float y;
  float size;

  float r,g,b;

public:
  KroozFont() :
  r(.3f) ,g(.3f) ,b(.3f),text(text), x(x) , y(y) , font_texture(0) , size(20.0f){}

  bool loadDefaultFont() {
      return loadFont(DEFAULT_FONT);
  }

  bool loadFont(const std::string& fontPath) {
    FILE* fontFile = fopen(fontPath.c_str(), "rb");
    if (!fontFile) {
        std::cerr << "Failed to open font file: " << fontPath << std::endl;
        return false;
    }

    fread(ttf_buffer, 1, sizeof(ttf_buffer), fontFile);
    fclose(fontFile);

    stbtt_BakeFontBitmap(ttf_buffer, 0, this->size, font_bitmap, FONT_BITMAP_W, FONT_BITMAP_H, 32, 96, cdata);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenTextures(1, &font_texture);
    glBindTexture(GL_TEXTURE_2D, font_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, FONT_BITMAP_W, FONT_BITMAP_H, 0, GL_ALPHA, GL_UNSIGNED_BYTE, font_bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return true;
  }

  void renderText(const std::string& text, float x, float y) {
    if (font_texture == 0) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, font_texture);
    glBegin(GL_QUADS);
    glColor3f(this->r,this->g,this->b);
    float startX = x;
    for (char c : text) {
      if (c >= 32 && c < 128) {
        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(cdata, FONT_BITMAP_W, FONT_BITMAP_H, c - 32, &x, &y, &q, 1);

        float yOffset = cdata[c - 32].yoff;
        glTexCoord2f(q.s0, q.t1); glVertex2f(q.x0, q.y0 - yOffset);
        glTexCoord2f(q.s1, q.t1); glVertex2f(q.x1, q.y0 - yOffset);
        glTexCoord2f(q.s1, q.t0); glVertex2f(q.x1, q.y1 - yOffset);
        glTexCoord2f(q.s0, q.t0); glVertex2f(q.x0, q.y1 - yOffset);
      }
    }
      glEnd();
      glDisable(GL_TEXTURE_2D);
      glColor3f(0.f,0.f,0.f);
  }

  void SetFontSize(float value){
    this->size = value;
  }

    float getTextWidth(const std::string& text) {
      float width = 0.0f;
      for (char c : text) {
          if (c >= 32 && c < 128) {
              width += cdata[c - 32].xadvance;
          }
      }
      return width;
  }

  float getTextHeight() {
      float maxHeight = 0.0f;
      for (int i = 32; i < 128; ++i) {
          float charHeight = cdata[i - 32].y1 - cdata[i - 32].y0;
          if (charHeight > maxHeight) {
              maxHeight = charHeight;
          }
      }
      return maxHeight;
  }

  void SetColor(float r , float g , float b){
    this->r = r;
    this->g = g;
    this->b = b;
  }

  ~KroozFont() {
    if (font_texture) {
        glDeleteTextures(1, &font_texture);
    }
  }
};
