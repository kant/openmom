#include "MainView.h"

#include "Buttons.h"
#include "Gfx.h"
#include "Font.h"
#include "Viewport.h"
#include "UnitDraw.h"

#include "Game.h"
#include "World.h"
#include "LocalPlayer.h"
#include "Army.h"
#include "Unit.h"
#include "Tile.h"
#include "City.h"

#include "ViewManager.h"

MainView::MainView(ViewManager *gvm) : View(gvm)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[GAME] = TristateButton::build("Game", 7, 4, TextureID::UPPER_MENU, 0);
  buttons[SPELLS] = TristateButton::build("Spells", 47, 4, TextureID::UPPER_MENU, 1);
  buttons[ARMIES] = TristateButton::build("Armies", 89, 4, TextureID::UPPER_MENU, 2);
  buttons[CITIES] = TristateButton::build("Cities", 140, 4, TextureID::UPPER_MENU, 3);
  buttons[MAGIC] = TristateButton::build("Magic", 184, 4, TextureID::UPPER_MENU, 4);
  buttons[INFO] = TristateButton::build("Info", 226, 4, TextureID::UPPER_MENU, 5);
  buttons[PLANE] = TristateButton::build("Plane", 270, 4, TextureID::UPPER_MENU, 6);
  
  buttons[NEXT] = TristateButton::build("Next", 240, 173, TextureID::MAIN_LOW_BUTTONS, 0);
  buttons[CANCEL] = TristateButton::build("Cancel", 240, 173, TextureID::MAIN_LOW_BUTTONS, 1);

  buttons[DONE] = TristateButton::build("Done", 246, 176, TextureID::MAIN_SMALL_BUTTONS, 0);
  buttons[PATROL] = TristateButton::build("Patrol", 280, 176, TextureID::MAIN_SMALL_BUTTONS, 1);
  buttons[WAIT] = TristateButton::build("Wait", 246, 186, TextureID::MAIN_SMALL_BUTTONS, 2);
  buttons[BUILD] = TristateButton::build("Build", 280, 186, TextureID::MAIN_SMALL_BUTTONS, 3);
  
  buttons[SPELLS]->setAction([gvm](){ gvm->switchView(VIEW_MAGIC); });
  buttons[ARMIES]->setAction([gvm](){ gvm->switchView(VIEW_ARMIES); });
  buttons[PLANE]->setAction([this](){ player->switchPlane(); switchToNormalState(player); });
  buttons[NEXT]->setAction([this](){ LocalGame::i->nextTurn(); switchToNormalState(player); });
  //buttons[CANCEL_SURVEYOR]->setAction([this](){ if (substate == SPELL_CAST) g->cancelCast(player); switchToNormalState(player); }); // TODO what is this for?
  buttons[DONE]->setAction([this](){ switchToNormalState(player );}); /*if (player.selectedArmy() != null && player.selectedRoute() != null && !player.selectedRoute().completed()) player.saveRoute();*/
  buttons[PATROL]->setAction([this](){ player->getSelectedArmy()->patrol(); });
  buttons[BUILD]->setAction([this](){ g->settleCity(player->getSelectedArmy(), "Test"); player->resetArmy(); });
  
  for (auto e : {DONE,PATROL,WAIT,BUILD} )
    buttons[e]->hide();
  
  buttons[BUILD]->deactivate();
  
  substate = MAIN;
}

void MainView::switchToSpellCast(LocalPlayer *p)
{
	p->resetArmy();
	
	buttons[NEXT]->hide();
	buttons[DONE]->hide();
	buttons[PATROL]->hide();
	buttons[WAIT]->hide();
	buttons[BUILD]->hide();
	buttons[CANCEL_SURVEYOR]->show();
	
	substate = SPELL_CAST;
}

void MainView::switchToUnitSelection(LocalPlayer *p, Army* a)
{
	p->selectArmy(a);
	a->unpatrol();
	p->selectAll();
	
	buttons[CANCEL_SURVEYOR]->hide();
	buttons[NEXT]->hide();
	buttons[DONE]->show();
	buttons[PATROL]->show();
	buttons[WAIT]->show();
	buttons[BUILD]->show();
	
	updateBuildButton(p);
  
	substate = UNIT;
}

void MainView::switchToNormalState(LocalPlayer *p)
{
	p->resetArmy();
	
	buttons[NEXT]->show();
	buttons[DONE]->hide();
	buttons[PATROL]->hide();
	buttons[WAIT]->hide();
	buttons[BUILD]->hide();
	buttons[CANCEL_SURVEYOR]->hide();
	
	// TODO: Surveyor.updateTile(null);
	
	substate = MAIN;
}

void MainView::switchToSurveyor(LocalPlayer *p)
{
	p->resetArmy();
	
	buttons[CANCEL_SURVEYOR]->show();
	buttons[NEXT]->hide();
	buttons[DONE]->hide();
	buttons[PATROL]->hide();
	buttons[WAIT]->hide();
	buttons[BUILD]->hide();
	
	substate = SURVEYOR;
}

void MainView::updateBuildButton(LocalPlayer *p)
{
	if (p->selectedArmyCanBuildOutpost())
		buttons[BUILD]->activate();
	else
		buttons[BUILD]->deactivate();
}

void MainView::draw()
{
  Fonts::drawString("Antani", FontFace::WHITE_SMALL, 278, 100, ALIGN_CENTER); return;

  
  if (substate != SPELL_CAST && player->getSpellTarget() != Target::NONE)
    switchToSpellCast(player);
  else if (substate == SPELL_CAST && player->getSpellTarget() == Target::NONE)
    switchToNormalState(player);
  
  Gfx::draw(TextureID::MAIN_BACKDROP, 0, 0);
  
  const Army* army = player->getSelectedArmy();
  if (army)
  {
    for (int j = 0; j < army->size(); ++j)
    {
      Unit *unit = army->get(j);
      
      int x = 246+23*(j%3), y = 78+29*(j/3);
      Gfx::draw(TextureID::MAIN_UNITS_RIGHT_BACKDROP, 0, (j/3)*3+(j%3), x, y);
      
      UnitDraw::drawStatic(unit, x+1, y+1, player->isSelectedUnit(unit), false);
      
      // draw unit level
      s16 levelIndex = unit->level ? unit->level->index(): -1;
      if (levelIndex > 0)
      {
        --levelIndex;
        s16 badge = levelIndex / 3;
        s16 badgeCount = levelIndex % 3;
        
        // TODO: in realtà le icone non hanno bordo nero nel gioco anche se nelle png sì
        for (int u = 0; u < badgeCount+1; ++u)
          Gfx::draw(TextureID::UNITS_MISC_SYMBOLS, 1, badge, x + 3 + 4*u, y + 22);
      }
    }
    
    /* compute and draw movement of current selection */
    s16 moves = player->selectedAvailMoves();
    if (moves > 0)
    {
      Fonts::drawString(Fonts::format("Moves: %d%s",moves/2, moves%2 == 0 ? ".5" : "" ), FontFace::WHITE_SMALL, 246, 167, ALIGN_LEFT);
      auto movement = player->selectedArmyMovementType();
      
      /* TODO: non corporeal symbol in game looks like swimming, not flying */
      if (movement.find(&Effects::NON_CORPOREAL) != movement.end() || movement.find(&Effects::FLYING) != movement.end()) // TODO: why?
        movement.erase(movement.find(&Effects::NON_CORPOREAL));

      //TODO: fixare icona movimento che ora usa un Set<SkillEffect.MovementBonus>
      /*if (movement.size() > 0)
       for (int i = 0; i < movement.size(); ++i)
       Gfx.draw(Texture.UNITS_MISC_SYMBOLS, 0, 1+movement.get(i).ordinal(), 306-10*i, 167);
       else
       Gfx.draw(Texture.UNITS_MISC_SYMBOLS, 0, 0, 306, 167);*/
    }
  }
  else if (substate == SURVEYOR)
  {
    Gfx::draw(TextureID::MAIN_RIGHT_BACKDROPS, 0, 1, 240, 76);
    //TODO Surveyor.drawSurveyor();
  }
  else if (substate == SPELL_CAST)
  {
    Gfx::draw(TextureID::MAIN_RIGHT_BACKDROPS, 0, 2, 240, 76);
    Fonts::drawString("Casting", FontFace::SURVEY_SERIF, 240+6+8+4, 76+2+1, ALIGN_LEFT);
    
    //TODO: colors etc
    Fonts::drawStringBounded("^wSelect a friendly unit as the target for a ^yBless^^ ^wspell", FontFace::SMALL_SHORTER, 249, 114, 64, ALIGN_LEFT);
  }
  else
    Gfx::draw(TextureID::MAIN_RIGHT_BACKDROPS, 0, 0, 240, 76);
  
  //Sprites.drawTile(p, Sprites.Texture.RIGHT_BACKDROP, 0, 0, 480, 152);
  Viewport::drawMainViewport(player, g->world);
  //Fonts.drawStringBounded("This shows the current city and the surrounding area. The city does not control/work the darkened corners. If a nearby city and this city control the same square, each city receives only half of the square's value.", Fonts.Face.TEAL_SMALL, 20, 30, 173, Fonts.Align.CENTER);

  const Position vp = player->getViewport();
  Viewport::drawMicroMap(player, 251, 21, 58, 30, vp.x, vp.y, vp.plane); // 58, 30

  if (substate == MAIN)
  {
    int gg = player->goldDelta(), f = player->foodDelta(), m = player->manaDelta();
    
    Fonts::drawString(Fonts::format("%u Gold",gg), gg > 0 ? FontFace::YELLOW_TINY_STROKE : FontFace::RED_TINY_STROKE, 278, 100, ALIGN_CENTER);
    Fonts::drawString(Fonts::format("%u Food",f), f > 0 ? FontFace::YELLOW_TINY_STROKE : FontFace::RED_TINY_STROKE, 278, 132, ALIGN_CENTER);
    Fonts::drawString(Fonts::format("%u Mana",m), m > 0 ? FontFace::YELLOW_TINY_STROKE : FontFace::RED_TINY_STROKE, 278, 164, ALIGN_CENTER);
  }
  
  if (substate == MAIN || substate == UNIT)
  {
    Fonts::drawString(Fonts::format("%u",player->totalGoldPool()), FontFace::WHITE_SMALL, 259, 68, ALIGN_RIGHT);
    Fonts::drawString("GP", FontFace::WHITE_TINY, 268, 67, ALIGN_LEFT);
    Fonts::drawString(Fonts::format("%u",player->totalManaPool()), FontFace::WHITE_SMALL, 297, 68, ALIGN_RIGHT);
    Fonts::drawString("MP", FontFace::WHITE_TINY, 306, 67, ALIGN_LEFT);
  }
}

void MainView::drawPost()
{
  /*Gfx.draw(Texture.RESEARCH_LAB_BACKDROP, 0, 0);
   Gfx.draw(Texture.RESEARCH_WIZARDS, player.wizard.ordinal(), 69, 75);
   Gfx.bindColorMap(ColorMap.researchBeam);
   Gfx.drawAnimated(Texture.RESEARCH_BEAM, 0, 132, -3, 0);
   Gfx.unbindColorMap();
   Gfx.draw(Texture.RESEARCH_TOME,149,133);
   //TODO: real major school of wizard
   Gfx.draw(Texture.RESEARCH_MASCOTS,4,190,160);*/
  
  //ColorMap.BlinkMap blink = new ColorMap.BlinkMap(new int[]{Gfx.color(223, 150, 28)}, 0,0,0,    180,30,0,  6);
  //Fonts.drawString("Test Blink", Fonts.Face.YELLOW_SMALL, 50, 50, Fonts.Align.LEFT, blink);
  
  /*Gfx.bindBuffer();
   Gfx.resetBuffer(179,136);
   Gfx.draw(Texture.SUMMON_BACKDROP, 0, 0);
   Gfx.drawAnimated(Texture.SUMMON_ANIMATION, 1, 14+82-31, 26+97-43, 0);
   Gfx.drawAnimated(Texture.SUMMON_ANIMATION, 0, 0+82-31, 0+97-43, 0);
   Gfx.colorMapBuffer(179, 136, ColorMap.summonAnimationRed);
   Gfx.mergeBuffer(0, 0, 31, 43, 179, 136);
   Gfx.bindCanvas();
   Gfx.drawClippedFromHeight(Texture.SUMMON_WIZARDS, 0, 7, 39, 46+5, 5);*/
}

void MainView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  const Position pos = Viewport::hoveredPosition(g->world, player, x, y);
  
  if (pos.x != -1)
  {
    Tile* t = g->world->get(pos);
    City* c = t->city;
    Army* a = t->army;
    
    if (b == BUTTON_RIGHT)
    {
      if (a && (!c || (c && !a->isPatrolling())) && a->getOwner() == player)
      {
        switchToUnitSelection(player,a);
      }
      /*else if (c)
      {
        if (!c->isOutpost())
        {
          GameViewManager.instance.cityView().setCity(c);
          GameViewManager.instance.switchView(GameViewManager.ViewType.CITY);
        }
        else
        {
          GameViewManager.instance.outpostView().setCity(c);
          GameViewManager.instance.switchOverView(GameViewManager.ViewType.OUTPOST);
        }
        
        player.resetArmy();
        switchToNormalState(player);
      }
      else
      {
        int upperBoundY = t.y - WorldViewport.viewportH/2;
        int lowerBoundY = t.y + (WorldViewport.viewportH  - WorldViewport.viewportH/2);
        int ty = t.y;
        
        if (upperBoundY < 0)
          ty = WorldViewport.viewportH/2;
        else if (lowerBoundY >= g.world().height())
          ty = g.world().height() - (WorldViewport.viewportH  - WorldViewport.viewportH/2);
        
        player.setViewport(t.x, ty);
      }*/
    }
  }
}