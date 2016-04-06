#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"
#include <vector>
using namespace std;

struct SDL_Texture;
class GuiImage;
class GuiCursor;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before the first frame
	bool start();

	// Called before all updates
	bool preUpdate();

	// Called each loop iteration
	bool update(float dt);

	// Called before all updates
	bool postUpdate();

	// Called before quitting
	bool cleanUp();
	//GUI
	void onGui(GuiElements* ui, GUI_EVENTS event);

private:

	//HUD UI
	GuiImage* ui_terran = nullptr;
	GuiImage* SDL_Rect_map_camera = nullptr;
	GuiImage* SDL_Rect_map = nullptr;

public:

	// CRZ PathFinding tests
	/*SDL_Texture *path_tile;
	bool	     path_selected;
	iPoint		 start_position, final_position;*/

};

#endif // __SCENE_H__
