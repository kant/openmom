//
//  Buttons.h
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include "Common.h"
#include "Texture.h"

#include <string>
#include <functional>
#include <vector>
#include <SDL2/SDL.h>

enum FontFace : u16;

typedef std::function<void()> Action;

class Clickable
{
  protected:
    u16 x, y, w, h;
    const MouseButton button;
    Action action;
    bool active;
  
  public:
    Clickable(u16 x, u16 y, u16 w, u16 h, MouseButton b = BUTTON_LEFT) : x(x), y(y), w(w), h(h), button(b), active(true) { }

    bool isCorrectButton(MouseButton b) { return b == button; }
    bool isInside(u16 x, u16 y) { return x >= this->x && x <= this->x+w && y >= this->y && y <= this->y+h; }
    virtual bool isActive() { return active; }
    inline void activate() { active = true; }
    inline void deactivate() { active = false; }
    inline void activateIf(bool condition) { active = condition; }
  
    void setPosition(u16 x, u16 y) { this->x = x; this->y = y; }
  
    virtual inline Clickable* setAction(Action action) { this->action = action; return this; }
    inline Action getAction() { return action; }
  
    void draw();
};

class Button : public Clickable
{
  protected:
    const std::string name;
    bool pressed, visible;
    SpriteInfo normalCoords;
  
  public:
    Button(const std::string name, u16 x, u16 y, SpriteInfo info) :
      Clickable(x,y,Texture::get(info.texture).span(info.x),Texture::get(info.texture).h), name(name), pressed(false), visible(true), normalCoords(info)
    {
    
    }
  
    virtual void draw() = 0;
  
    inline void execute() { if (action) action(); }
  
    inline void showIf(bool condition) { visible = condition;}
    inline Button* hide() { visible = false; return this; }
    inline Button* show() { visible = true; return this; }
    inline bool isVisible() { return visible;}
  
    bool isActive() override { return active && visible; }

    inline Button* setAction(Action action) override { this->action = action; return this; }

  
    inline void press() { pressed = true;}
    inline void release() { pressed = false; }
    
    virtual void click() {
      //TODO log clicked
      execute();
    }
};

class SimpleButton : public Button
{
  public:
    SimpleButton(const std::string name, u16 x, u16 y, SpriteInfo info) : Button(name, x, y, info) { }
    void draw() override;
};

class OffsetButton : public Button
{
  public:
    OffsetButton(const std::string name, u16 x, u16 y, SpriteInfo info) : Button(name, x, y, info) { }
    void draw() override;
};

class BistateButton : public Button
{
  protected:
    SpriteInfo pressedCoords;

  public:
    BistateButton(const std::string name, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed) : Button(name, x, y, normal), pressedCoords(pressed) { }
    void draw() override;
  
    static BistateButton* build(const std::string name, u16 x, u16 y, TextureID texture, u16 c)
    {
      return new BistateButton(name, x, y, SpriteInfo(texture, c, 0), SpriteInfo(texture, c, 1));
    }
};

class TristateButton : public BistateButton
{
  private:
    TristateButton(const std::string name, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed, SpriteInfo inactive) : BistateButton(name, x, y, normal, pressed), inactiveCoords(inactive) { }


  protected:
    SpriteInfo inactiveCoords;
    
  public:
    void draw() override;
  
    static TristateButton* build(const std::string name, u16 x, u16 y, TextureID texture, u16 c)
    {
      return new TristateButton(name, x, y, SpriteInfo(texture, c, 0), SpriteInfo(texture, c, 1), SpriteInfo(texture, c, 2));
    }
};

class LabeledSimpleButton : public OffsetButton
{
  protected:
    std::string label;
    u16 textX, textY;
    FontFace font;
  
  public:
    LabeledSimpleButton(const std::string name, u16 x, u16 y, SpriteInfo info, std::string label, FontFace font);
    void setLabel(std::string label) { this->label = label; }
    void draw() override;
};

template<typename T>
class RadioButton;

template<typename T>
class RadioButtonGroup
{
private:
  std::vector<RadioButton<T>*> buttons;
  RadioButton<T>* current;
  std::function<void(RadioButton<T>*)> lambda;
public:
  RadioButtonGroup() : current(nullptr) { }
  
  RadioButton<T>* getCurrent() { return current; }
  void set(u16 index) { current = buttons[index]; }
  void click(RadioButton<T>* button)
  {
    if (current != button)
    {
      current = button;
      lambda(current);
    }
  }
  
  void add(RadioButton<T>* button)
  {
    buttons.push_back(button);
  }
  
  void setAction(std::function<void(RadioButton<T>*)> lambda) { this->lambda = lambda; }
};

template<typename T>
class RadioButton : public Button
{
private:
  RadioButtonGroup<T>* group;
  u8 toggledOffset[2];
  SpriteInfo pressedCoords;
  bool toggled;
  T data;
  
public:
  RadioButton(const std::string name, T data, RadioButtonGroup<T>* group, int x, int y, SpriteInfo normal, SpriteInfo pressed, u8 offsetX, u8 offsetY) :
    Button(name, x, y, normal), data(data), toggledOffset{offsetX, offsetY}, pressedCoords(pressed), toggled(false), group(group) { }
  
  void draw();
  
  const T getData() const { return data; }
  
  void click() override { group->click(this); }
  
  static RadioButton* build(const std::string name, T data, RadioButtonGroup<T>* group, u16 x, u16 y, TextureID texture, u16 c, u8 offsetX, u8 offsetY)
  {
    return new RadioButton(name, data, group, x, y, SpriteInfo(texture, c, 0), SpriteInfo(texture, c, 1), offsetX, offsetY);
  }
};

#include "Items.h" // FIXME: forced for explicit declaration of template method

template class RadioButton<Item::TypeID>;

#endif