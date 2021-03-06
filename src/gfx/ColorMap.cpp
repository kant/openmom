//
//  ColorMap.cpp
//  OpenMoM
//
//  Created by Jack on 7/18/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "ColorMap.h"

#include "Gfx.h"
#include "SDLHelper.h"

using namespace std;


color_d BlinkMap::get(u32 k) const
{
  if (set.find(k) == set.end()) return 0x00000000;
  else
  {
    s16 phase = Gfx::ticks % ticks;
    
    if (updatedForTick != phase)
    {
      float progress = phase / (float) ticks;
      
      if (progress <= 0.5f) progress *= 2.0f;
      else progress = (0.5f - (progress - 0.5f)) * 2.0f;
      
      u8 red = s[0] + (u8)(d[0]*progress);
      u8 green = s[1] + (u8)(d[1]*progress);
      u8 blue = s[2] + (u8)(d[2]*progress);
      
      return Gfx::color(red,green,blue);
    }
    
    return 0x00000000;
  }
}

const MiscMaps::GrayscaleMap MiscMaps::GRAYSCALE = MiscMaps::GrayscaleMap();


// palette index 216 217 218
const enum_simple_map<PlayerColor, HashColorMap, 6> MiscMaps::FLAG_COLORS_MAP = enum_simple_map<PlayerColor, HashColorMap, 6>({
  { PlayerColor::GREEN, HashColorMap() },
  { PlayerColor::BLUE, HashColorMap({
    {0,188,0}, {150,182,215},
    {0,164,0}, {113,154,190},
    {0,124,0}, {85,125,166}
  }) },
  { PlayerColor::RED, HashColorMap({
    {0,188,0}, {215,0,0},
    {0,164,0}, {211,28,0},
    {0,124,0}, {142,56,56}
  }) },
  { PlayerColor::PURPLE, HashColorMap({
    {0,188,0}, {211,60,255},
    {0,164,0}, {199,0,255},
    {0,124,0}, {146,0,190}
  }) },
  { PlayerColor::YELLOW, HashColorMap({
    {0,188,0}, {255,231,0},
    {0,164,0}, {231,211,0},
    {0,124,0}, {166,154,0}
  }) },
  { PlayerColor::NEUTRAL, HashColorMap({
    {0,188,0}, {117,77,36},
    {0,164,0}, {93,52,16},
    {0,124,0}, {69,36,4}
  }) },
});

const enum_simple_map<School, std::array<Color, 5>, SCHOOL_COUNT> MiscMaps::SCHOOL_GLOW_COLORS = enum_simple_map<School, std::array<Color, 5>, SCHOOL_COUNT>({
  { School::ARCANE, {} },
  { School::CHAOS, {{
    {215,150,150},
    {190,117,117},
    {166,85,85},
    {142,56,56},
    {117,36,36}
  }} },
  { School::DEATH, {{
    {239,195,215},
    {215,154,182},
    {190,117,150},
    {166,85,125},
    {142,56,97}
  }} },
  { School::LIFE, {{
    {255,255,255},
    {243,235,231},
    {227,219,215},
    {211,203,199},
    {195,186,182}
  }} },
  { School::NATURE, {{
    {215,239,190},
    {186,215,154},
    {154,190,117},
    {125,166,85},
    {97,142,92}
  }} },
  { School::SORCERY, {{
    {195,219,239},
    {150,182,215},
    {113,154,190},
    {85,125,166},
    {56,101,142}
  }} }
});


Color BlinkingPalette::get(index_t index) const
{
  if (indices.find(index) == indices.end()) return 0x00000000;
  else
  {
    s16 phase = Gfx::ticks % ticks;
    
    if (updatedForTick != phase)
    {
      float progress = phase / (float) ticks;
      
      if (progress <= 0.5f) progress *= 2.0f;
      else progress = (0.5f - (progress - 0.5f)) * 2.0f;
      
      u8 red = s[0] + (u8)(d[0]*progress);
      u8 green = s[1] + (u8)(d[1]*progress);
      u8 blue = s[2] + (u8)(d[2]*progress);
      
      return Gfx::color(red,green,blue);
    }
    
    return 0x00000000;
  }
}
