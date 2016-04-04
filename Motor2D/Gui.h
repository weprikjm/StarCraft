#ifndef __GUI_H__
#define __GUI_H__

#include "App.h"
#include "Module.h"
#include "Render.h"
#include "Point2d.h"
#include "p2Defs.h"
#include "Animation.h"
#include <string>
#include <list>

#define CURSOR_WIDTH 2
using namespace std;

enum GUI_EVENTS
{
	LISTENING_ENDS,
	MOUSE_ENTERS,
	MOUSE_LEAVES,
	MOUSE_LCLICK_DOWN,
	MOUSE_LCLICK_DOWN_REPEAT,
	MOUSE_LCLICK_UP,
	MOUSE_RCLICK_DOWN,
	MOUSE_RCLICK_UP,
	GAIN_FOCUS,
	LOST_FOCUS,
    INPUT_CHANGED,
	INPUT_SUBMIT,
    VALUE_CHANGED,
	SAVE_POS_THUMB,
	LOAD_POS_THUMB
};

enum GUI_TYPES
{
	UNKNOWN,
	IMAGE,
	LABEL,
	CURSOR,
	BUTTON,
	HSCROLLBAR
};

// ---------------------------------------------------
class GuiElements
{
public:
	GuiElements();
	virtual ~GuiElements()
	{}

	virtual void draw() const
	{}
	virtual void debugDraw() const;
	virtual void update(const GuiElements* mouse_hover, const GuiElements* focus)
	{}
	virtual void updatePosition()
	{}
	void checkInput(const GuiElements* mouse_hover, const GuiElements* focus);
	void setLocalPos(int x, int y);
	void center();
	rectangle getScreenRect() const;
	rectangle getLocalRect() const;
	iPoint getScreenPos() const;
	iPoint getLocalPos() const;
	void setListener(Module* module);
	GUI_TYPES getType()const;
	Module* getListener()const;

protected:
	void setSize(int w, int h);

public:
	bool draggable = false;
	bool interactive = false;
	bool cut_childs = false;
	bool can_focus = false;
	bool draw_element = true;
	GuiElements* parent = nullptr;
protected:
	GUI_TYPES type = UNKNOWN;
	Module* listener = nullptr;
	bool have_focus = false;
private:
	bool mouse_inside = false;
	rectangle rect;
};

// ---------------------------------------------------
class GuiImage : public GuiElements
{
public:
	GuiImage(const SDL_Texture* texture);
	GuiImage(const SDL_Texture* texture, const rectangle& section);
	~GuiImage();

	void setSection(const rectangle& section);
	void draw() const;

private:

	rectangle section;
	const SDL_Texture* texture = nullptr;
};

// ---------------------------------------------------
class GuiCursor : public GuiElements
{
public:
	GuiCursor(const SDL_Texture* texture, Animation* animation);

	~GuiCursor();

	void setPosition(iPoint coords);
	void updatePosition();
	void draw()const;
	void setTexture(SDL_Texture* tmp_texture);

private:

	const SDL_Texture* texture = nullptr;
	Animation* cursor_anim;
	rectangle section;
	iPoint position;
};
	

//-----------------------------------------------------------------------
//CLASS GUI

struct SDL_Texture;

// ---------------------------------------------------
class Gui : public Module
{
public:

	Gui();

	// Destructor
	virtual ~Gui();

	// Called when before render is available
	bool awake(pugi::xml_node&);

	// Call before first frame
	bool start();

	// Called before all Updates
	bool preUpdate();

	// Called after all Updates
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	// Gui creation functions
	GuiImage* createImage(const char* filename);
	GuiImage* createImage(const SDL_Texture* texture, const rectangle& atlas_section);
	GuiCursor* createCursor(const SDL_Texture* texture, Animation* animation);
	/*GuiLabel* CreateLabel(const char* text);
	GuiHScrollBar* CreateHScrollBar(const rectangle& bar, const rectangle& thumb, const rectangle& bar_offset = { 0, 0, 0, 0 }, const iPoint& thumb_margins = { 0, 0 });
	GuiHScrollBarVertical* CreateHScrollBarVertical(const rectangle& bar, const rectangle& thumb, const rectangle& bar_offset = { 0, 0, 0, 0 }, const iPoint& thumb_margins = { 0, 0 });*/

	const GuiElements* findMouseHover();
	const SDL_Texture* getAtlas() const;

	SDL_Rect mouseQuad(iPoint init_mouse);

private:

	bool debug = false;
	list<GuiElements*> elements;
	const GuiElements* focus = nullptr;
	SDL_Texture* atlas = nullptr;
	string atlas_file_name;
};

#endif