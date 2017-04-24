//
//  ResearchView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "View.h"

class IntroView : public View
{
private:
  u32 currentFrame;
  
  void draw() override;
  void drawPost() override { }
  
public:
  IntroView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
};
