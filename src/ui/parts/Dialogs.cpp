#include "Dialogs.h"

#include "Gfx.h"
#include "Font.h"
#include "LBX.h"

using namespace std;
using namespace dialogs;

void dialogs::drawErrorDialog(const std::string& text)
{
  constexpr u32 MESSAGE_WIDTH = 174;
  constexpr u32 DIALOG_WIDTH = 186;
  constexpr u32 DIALOG_X = 68;
  
  Gfx::resetBuffer();
  Gfx::bindBuffer();
  int h = Fonts::drawStringBounded(text, FontFaces::Serif::GOLD_ERROR_MESSAGE, 4+MESSAGE_WIDTH/2, 5, MESSAGE_WIDTH, ALIGN_CENTER) + 3;
  int y = HEIGHT/2 - (h+29)/2;
  Gfx::bindCanvas();
  
  Gfx::bindPalette(LSI(RESOURCE,38));
  Gfx::drawClipped(LSI(RESOURCE,39), DIALOG_X, y+h, 0, 0, DIALOG_WIDTH, 9);
  Gfx::drawClipped(LSI(RESOURCE,38), DIALOG_X, y, 0, 0, DIALOG_WIDTH, h);
  Gfx::mergeBuffer(4, 4, DIALOG_X + (DIALOG_WIDTH - MESSAGE_WIDTH)/2, y+8, DIALOG_WIDTH, h+10);
  Gfx::unbindPalette();
}

static const SpriteInfo leftTop = LSI(RESOURCE, 5);
static const SpriteInfo leftBottom = LSI(RESOURCE, 6);
static const SpriteInfo top = LSI(RESOURCE, 7);
static const SpriteInfo rightTop = LSI(RESOURCE, 8);
static const SpriteInfo bottom = LSI(RESOURCE, 9);
static const SpriteInfo rightBottom = LSI(RESOURCE, 10);

InfoMenu::InfoMenu(const Point& position, u16 rows, u16 buttonWidth) : b(position)
{
  
}

void InfoMenu::draw()
{
  const Palette* palette = top.palette();
  
  //static const auto leftWidth = leftTop.sw();
  Gfx::bindPalette(palette);
  Gfx::drawClipped(leftTop, b.x, b.y, 0, 0, leftTop.sw(), h - leftBottom.sh());
  Gfx::draw(leftBottom, b.x, b.y + h - leftBottom.sh());
  Gfx::drawClipped(top, b.x + leftTop.sw(), b.y, 0, 0, w - leftTop.sw() - rightTop.sw(), top.sh());
  Gfx::drawClipped(rightTop, b.x + w - rightTop.sw(), b.y, 0, 0, rightTop.sw(), h - rightBottom.sh());
  Gfx::draw(rightBottom, b.x + w - rightBottom.sw(), b.y + h - rightBottom.sh());
  Gfx::drawClipped(bottom, b.x + leftBottom.sw(), b.y + h - bottom.sh(), 0, 0, w - leftBottom.sw() - rightBottom.sw(), bottom.sh());
  Gfx::unbindPalette();
  
  const int rows = 3;
  const int buttonLength = 100;
  constexpr s16 BORDER_LENGTH = 2, BUTTON_HEIGHT = 14;
  for (u16 index = 0; index < rows; ++index)
  {
    const auto buttonGfx = LSI(RESOURCE, 12 + index), borderGfx = LSI(RESOURCE, 22 + index);
    const Point position = Point(b.x + leftTop.sw(), b.y + top.sh() + BUTTON_HEIGHT*index);
    
    
    Gfx::drawClipped(buttonGfx, position.x, position.y, 0, 0, buttonLength - BORDER_LENGTH, buttonGfx.sh());
    Gfx::draw(borderGfx, position + Point(buttonLength - BORDER_LENGTH, 0));
    
    Fonts::drawString(Fonts::format("foobar %d", index), FontFaces::Serif::BLACK_INFO_MENU, position.delta(buttonLength/2, 1), ALIGN_CENTER);
    
  }
}


void ItemDetailDialog::draw()
{
  static const SpriteInfo background = LSI(ITEMISC, 25);
  static const SpriteInfo pin = LSI(ITEMISC, 26);
}
