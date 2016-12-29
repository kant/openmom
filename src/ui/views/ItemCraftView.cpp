//
//  ItemCraftView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "ItemCraftView.h"

#include "Font.h"
#include "Buttons.h"
#include "Texture.h"
#include "Gfx.h"
#include "GfxData.h"
#include "ColorMap.h"

#include "Util.h"

const Item::TypeID ItemCraftView::ITEM_TYPES[] = {
  Item::TypeID::SWORD, Item::TypeID::MACE, Item::TypeID::AXE, Item::TypeID::BOW, Item::TypeID::STAFF,
  Item::TypeID::WAND, Item::TypeID::MISC, Item::TypeID::SHIELD, Item::TypeID::CHAIN, Item::TypeID::PLATE
};

sprite_ref TYPE_BUTTONS[][2] = {
  { LBXI(SPELLSCR, 14), LBXI(SPELLSCR, 25) },
  { LBXI(SPELLSCR, 15), LBXI(SPELLSCR, 26) },
  { LBXI(SPELLSCR, 16), LBXI(SPELLSCR, 27) },
  { LBXI(SPELLSCR, 17), LBXI(SPELLSCR, 28) },
  { LBXI(SPELLSCR, 18), LBXI(SPELLSCR, 29) },
  { LBXI(SPELLSCR, 19), LBXI(SPELLSCR, 30) },
  { LBXI(SPELLSCR, 20), LBXI(SPELLSCR, 31) },
  { LBXI(SPELLSCR, 21), LBXI(SPELLSCR, 32) },
  { LBXI(SPELLSCR, 22), LBXI(SPELLSCR, 33) },
  { LBXI(SPELLSCR, 23), LBXI(SPELLSCR, 34) }
};

ItemCraftView::ItemCraftView(ViewManager* gvm) : View(gvm), school(NATURE), currentType(Item::TypeID::SWORD), currentItemGfx(0)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[PREV_GFX] = NormalButton::buildBistate("Prev Gfx", 4, 24, LSI(SPELLSCR, 35))->setAction([this](){
    --currentItemGfx;
    if (currentItemGfx < 0)
      currentItemGfx = Item::typeForItem(currentType)->count();
    updateItemName();
  });
  
  buttons[NEXT_GFX] = NormalButton::buildBistate("Next Gfx", 17, 24, LSI(SPELLSCR, 36))->setAction([this](){
    ++currentItemGfx;
    currentItemGfx %= Item::typeForItem(currentType)->count();
    updateItemName();
  });
  
  itemType = new RadioButtonGroup<Item::TypeID>();
  itemType->setAction([this](RadioButton<Item::TypeID>* b){
    currentType = b->getData();
    currentItemGfx = 0;
    school = static_cast<School>(Util::randomIntInclusive(1, 5)); // TODO: why?
    updateItemName();
  });
  
  for (size_t i = 0; i < sizeof(ITEM_TYPES)/sizeof(ITEM_TYPES[0]); ++i)
  {
    RadioButton<Item::TypeID>* button = RadioButton<Item::TypeID>::build("type", ITEM_TYPES[i], itemType, 156 + 33*(i%5), 3 + 15*(i/5), TYPE_BUTTONS[i][0], TYPE_BUTTONS[i][1]);
    buttons.push_back(button);
    itemType->add(button);
  }
  
  itemType->set(0);
}

void ItemCraftView::updateItemName()
{
  
}

void ItemCraftView::draw()
{
  Gfx::draw(LSI(SPELLSCR, 13), 0, 0);
  
  const auto itemGfx = GfxData::itemGfxSpec(currentType, currentItemGfx);
  
  Gfx::draw(itemGfx, 7, 6);
  Gfx::drawGlow(itemGfx, 7, 6, school);
  
  Fonts::drawString(itemName, FontFaces::Small::GRAY_ITEM_CRAFT, 29, 12, ALIGN_LEFT);

}
