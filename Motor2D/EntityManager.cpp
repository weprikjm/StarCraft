#include "App.h"
#include "EntityManager.h"
#include "ShortcutsManager.h"
#include "Input.h"
#include "Window.h"
#include "p2Log.h"
#include "PathFinding.h"
#include "GameManager.h"

#include "Firebat.h"
#include "Scv.h"
#include "Medic.h"
#include "Zergling.h"
#include "Mutalisk.h"
#include "Ultralisk.h"
#include "Hydralisk.h"
#include "Tank.h"
#include "JimRaynor.h"
#include "Gui.h"
#include "GuiCursor.h"
#include "FogOfWar.h"

#include "Bomb.h"
#include "CommandCenter.h"
#include "Bunker.h"
#include "Barrack.h"
#include "Factory.h"
#include "Blue.h"
#include "Yellow.h"
#include "Red.h"
#include "GuiMinimap.h"
#include "GuiImage.h"

#include <sstream>

#include "Brofiler.h"

using namespace std;

EntityManager::EntityManager(bool enabled) : Module(enabled)
{
	name.assign("entity_manager");
}

// Destructor
EntityManager::~EntityManager()
{ }

// Called before render is available
bool EntityManager::awake(pugi::xml_node &node)
{
	bunker_mineral_cost = node.child("costs").attribute("bunker_mineral_cost").as_int();
	bunker_gas_cost = node.child("costs").attribute("bunker_gas_cost").as_int();
	barrack_mineral_cost = node.child("costs").attribute("barrack_mineral_cost").as_int();
	barrack_gas_cost = node.child("costs").attribute("barrack_gas_cost").as_int();
	factory_mineral_cost = node.child("costs").attribute("factory_mineral_cost").as_int();
	factory_gas_cost = node.child("costs").attribute("factory_gas_cost").as_int();

	marine_mineral_cost = node.child("costs").attribute("marine_mineral_cost").as_int();
	marine_gas_cost = node.child("costs").attribute("marine_gas_cost").as_int();
	firebat_mineral_cost = node.child("costs").attribute("firebat_mineral_cost").as_int();
	firebat_gas_cost = node.child("costs").attribute("firebat_gas_cost").as_int();
	medic_mineral_cost = node.child("costs").attribute("medic_mineral_cost").as_int();
	medic_gas_cost = node.child("costs").attribute("medic_gas_cost").as_int();
	tank_mineral_cost = node.child("costs").attribute("tank_mineral_cost").as_int();
	tank_gas_cost = node.child("costs").attribute("tank_gas_cost").as_int();
	scv_mineral_cost = node.child("costs").attribute("scv_mineral_cost").as_int();
	scv_gas_cost = node.child("costs").attribute("scv_gas_cost").as_int();

	zergling_mineral_cost = node.child("costs").attribute("zergling_mineral_cost").as_int();
	zergling_gas_cost = node.child("costs").attribute("zergling_gas_cost").as_int();
	hydralisk_mineral_cost = node.child("costs").attribute("hydralisk_mineral_cost").as_int();
	hydralisk_gas_cost = node.child("costs").attribute("hydralisk_gas_cost").as_int();
	mutalisk_mineral_cost = node.child("costs").attribute("mutalisk_mineral_cost").as_int();
	mutalisk_gas_cost = node.child("costs").attribute("mutalisk_gas_cost").as_int();
	ultralisk_mineral_cost = node.child("costs").attribute("ultralisk_mineral_cost").as_int();
	ultralisk_gas_cost = node.child("costs").attribute("ultralisk_gas_cost").as_int();

	zergling_mineral_cost_phase2 = node.child("costs").attribute("zergling_mineral_cost_phase2").as_int();
	zergling_gas_cost_phase2 = node.child("costs").attribute("zergling_gas_cost_phase2").as_int();
	hydralisk_mineral_cost_phase2 = node.child("costs").attribute("hydralisk_mineral_cost_phase2").as_int();
	hydralisk_gas_cost_phase2 = node.child("costs").attribute("hydralisk_gas_cost_phase2").as_int();
	mutalisk_mineral_cost_phase2 = node.child("costs").attribute("mutalisk_mineral_cost_phase2").as_int();
	mutalisk_gas_cost_phase2 = node.child("costs").attribute("mutalisk_gas_cost_phase2").as_int();
	ultralisk_mineral_cost_phase2 = node.child("costs").attribute("ultralisk_mineral_cost_phase2").as_int();
	ultralisk_gas_cost_phase2 = node.child("costs").attribute("ultralisk_gas_cost_phase2").as_int();

	return true;
}

void EntityManager::loadEntityTex()
{
	// Entities textures
	marine_tex = app->tex->loadTexture("Terran/Units/Blue_Marine.png");
	scv_tex = app->tex->loadTexture("Terran/Units/scv.png");
	medic_tex = app->tex->loadTexture("Terran/Units/Medic.png");
	firebat_tex = app->tex->loadTexture("Terran/Units/firebat.png");
	jim_raynor_tex = app->tex->loadTexture("Terran/Units/JimRaynor.png");
	tank_tex = app->tex->loadTexture("Terran/Units/Blue_tank.png");

	zergling_tex = app->tex->loadTexture("Zergs/Units/New_Zergling64.png");
	hydralisk_tex = app->tex->loadTexture("Zergs/Units/Hydralisk.png");
	ultralisk_tex = app->tex->loadTexture("Zergs/Units/ultralisk2.png");
	mutalisk_tex = app->tex->loadTexture("Zergs/Units/Mutalisk.png");

	bunker_tex = app->tex->loadTexture("Terran/Building/Bunker.png");
	bomb_tex = app->tex->loadTexture("Terran/Bomb/Bomb_STAR.png"); 
	factory_tex = app->tex->loadTexture("Terran/Building/factory.png");
	command_center_tex = app->tex->loadTexture("Terran/Building/CommandCenter.png");
	barrack_tex = app->tex->loadTexture("Terran/Building/barracks.png");
}

bool EntityManager::loadEntityFX()
{
	//Marine
	fx_marine_attack = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Attack.wav");
	
	fx_marine_death_1 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Death_1.wav");
	fx_marine_death_2 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Death_2.wav");
	
	fx_marine_acknowledgement_1 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Acknowledgement_1.wav");
	fx_marine_acknowledgement_2 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Acknowledgement_2.wav");
	fx_marine_acknowledgement_3 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Acknowledgement_3.wav");
	fx_marine_acknowledgement_4 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Acknowledgement_4.wav");
	
	fx_marine_affirmation_1 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Affirmation_1.wav");
	fx_marine_affirmation_2 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Affirmation_2.wav");
	fx_marine_affirmation_3 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Affirmation_3.wav");
	fx_marine_affirmation_4 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Affirmation_4.wav");
	
	fx_marine_annoyance_1 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Annoyance_1.wav");
	fx_marine_annoyance_2 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Annoyance_2.wav");
	fx_marine_annoyance_3 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Annoyance_3.wav");
	fx_marine_annoyance_4 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Annoyance_4.wav");
	fx_marine_annoyance_5 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Annoyance_5.wav");
	fx_marine_annoyance_6 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Annoyance_6.wav");
	fx_marine_annoyance_7 = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Annoyance_7.wav");
	
	fx_marine_ready = app->audio->loadFx("Audio/FX/Units/Terran/Marine/Ready.wav");

	//Firebat
	fx_firebat_attack_1 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Attack_1.wav");
	fx_firebat_attack_2 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Attack_2.wav");
	
	fx_firebat_death_1 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Death_1.wav");
	fx_firebat_death_2 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Death_2.wav");
	fx_firebat_death_3 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Death_3.wav");
	
	fx_firebat_acknowledgement_1 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Acknowledgement_1.wav");
	fx_firebat_acknowledgement_2 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Acknowledgement_2.wav");
	fx_firebat_acknowledgement_3 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Acknowledgement_3.wav");
	fx_firebat_acknowledgement_4 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Acknowledgement_4.wav");
	
	fx_firebat_affirmation_1 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Affirmation_1.wav");
	fx_firebat_affirmation_2 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Affirmation_2.wav");
	fx_firebat_affirmation_3 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Affirmation_3.wav");
	fx_firebat_affirmation_4 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Affirmation_4.wav");
	
	fx_firebat_annoyance_1 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Annoyance_1.wav");
	fx_firebat_annoyance_2 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Annoyance_2.wav");
	fx_firebat_annoyance_3 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Annoyance_3.wav");
	fx_firebat_annoyance_4 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Annoyance_4.wav");
	fx_firebat_annoyance_5 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Annoyance_5.wav");
	fx_firebat_annoyance_6 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Annoyance_6.wav");
	fx_firebat_annoyance_7 = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Annoyance_7.wav");
	
	fx_firebat_ready = app->audio->loadFx("Audio/FX/Units/Terran/Firebat/Ready.wav");

	//Medic
	fx_medic_heal = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Heal.wav");
	fx_medic_death = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Death.wav");

	fx_medic_acknowledgement_1 = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Acknowledgement_1.wav");
	fx_medic_acknowledgement_2 = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Acknowledgement_2.wav");
	fx_medic_acknowledgement_3 = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Acknowledgement_3.wav");
	fx_medic_acknowledgement_4 = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Acknowledgement_4.wav");

	fx_medic_affirmation_1 = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Affirmation_1.wav");
	fx_medic_affirmation_2 = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Affirmation_2.wav");
	fx_medic_affirmation_3 = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Affirmation_3.wav");
	fx_medic_affirmation_4 = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Affirmation_4.wav");

	fx_medic_annoyance_1 = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Annoyance_1.wav");
	fx_medic_annoyance_2 = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Annoyance_2.wav");
	fx_medic_annoyance_3 = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Annoyance_3.wav");
	fx_medic_annoyance_4 = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Annoyance_4.wav");
	fx_medic_annoyance_5 = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Annoyance_5.wav");
	fx_medic_annoyance_6 = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Annoyance_6.wav");
	fx_medic_annoyance_7 = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Annoyance_7.wav");

	fx_medic_ready = app->audio->loadFx("Audio/FX/Units/Terran/Medic/Ready.wav");

	//SiegeTank
	fx_tank_sige_mode_on = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/SiegeMode_On.wav");
	fx_tank_sige_mode_off = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/SiegeMode_Off.wav");
	fx_tank_missile_none_siege = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/TankMissileNoneSiege.wav");
	fx_tank_missile_siege = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/TankShootSiegeMode.wav");
	fx_tank_death = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/Death.wav");
																				   
	fx_tank_acknowledgement_1 = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/Acknowledgement_1.wav");
	fx_tank_acknowledgement_2 = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/Acknowledgement_2.wav");
	fx_tank_acknowledgement_3 = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/Acknowledgement_3.wav");
	fx_tank_acknowledgement_4 = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/Acknowledgement_4.wav");

	fx_tank_affirmation_1 = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/Affirmation_1.wav");
	fx_tank_affirmation_2 = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/Affirmation_2.wav");
	fx_tank_affirmation_3 = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/Affirmation_3.wav");
	fx_tank_affirmation_4 = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/Affirmation_4.wav");

	fx_tank_annoyance_1 = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/Annoyance_1.wav");
	fx_tank_annoyance_2 = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/Annoyance_2.wav");
	fx_tank_annoyance_3 = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/Annoyance_3.wav");
	fx_tank_annoyance_4 = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/Annoyance_4.wav");

	fx_tank_ready = app->audio->loadFx("Audio/FX/Units/Terran/SiegeTank/Ready.wav");

	//SCV
	fx_scv_repair_1 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Repair_1.wav");
	fx_scv_repair_2 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Repair_2.wav");
	fx_scv_repair_3 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Repair_3.wav");;
	fx_scv_repair_4 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Repair_4.wav");;
	fx_scv_repair_5 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Repair_5.wav");;

	fx_scv_death = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Death.wav");

	fx_scv_error_1 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Error_1.wav");
	fx_scv_error_2 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Error_2.wav");

	fx_scv_acknowledgement_1 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Acknowledgement_1.wav");
	fx_scv_acknowledgement_2 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Acknowledgement_2.wav");
	fx_scv_acknowledgement_3 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Acknowledgement_3.wav");
	fx_scv_acknowledgement_4 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Acknowledgement_4.wav");

	fx_scv_affirmation_1 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Affirmation_1.wav");
	fx_scv_affirmation_2 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Affirmation_2.wav");
	fx_scv_affirmation_3 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Affirmation_3.wav");
	fx_scv_affirmation_4 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Affirmation_4.wav");

	fx_scv_annoyance_1 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Annoyance_1.wav");
	fx_scv_annoyance_2 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Annoyance_2.wav");
	fx_scv_annoyance_3 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Annoyance_3.wav");
	fx_scv_annoyance_4 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Annoyance_4.wav");
	fx_scv_annoyance_5 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Annoyance_5.wav");
	fx_scv_annoyance_6 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Annoyance_6.wav");
	fx_scv_annoyance_7 = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Annoyance_7.wav");

	fx_scv_ready = app->audio->loadFx("Audio/FX/Units/Terran/SCV/Ready.wav");

	//Jim Raynor
	fx_jimraynor_acknowledgement_1 = app->audio->loadFx("Audio/FX/Units/Terran/JimRaynor/Acknowledgement_1.wav");
	fx_jimraynor_acknowledgement_2 = app->audio->loadFx("Audio/FX/Units/Terran/JimRaynor/Acknowledgement_2.wav");
	fx_jimraynor_acknowledgement_3 = app->audio->loadFx("Audio/FX/Units/Terran/JimRaynor/Acknowledgement_3.wav");
	fx_jimraynor_acknowledgement_4 = app->audio->loadFx("Audio/FX/Units/Terran/JimRaynor/Acknowledgement_4.wav");

	fx_jimraynor_affirmation_1 = app->audio->loadFx("Audio/FX/Units/Terran/JimRaynor/Affirmation_1.wav");
	fx_jimraynor_affirmation_2 = app->audio->loadFx("Audio/FX/Units/Terran/JimRaynor/Affirmation_2.wav");
	fx_jimraynor_affirmation_3 = app->audio->loadFx("Audio/FX/Units/Terran/JimRaynor/Affirmation_3.wav");
	fx_jimraynor_affirmation_4 = app->audio->loadFx("Audio/FX/Units/Terran/JimRaynor/Affirmation_4.wav");

	fx_jimraynor_annoyance_1 = app->audio->loadFx("Audio/FX/Units/Terran/JimRaynor/Annoyance_1.wav");
	fx_jimraynor_annoyance_2 = app->audio->loadFx("Audio/FX/Units/Terran/JimRaynor/Annoyance_2.wav");
	fx_jimraynor_annoyance_3 = app->audio->loadFx("Audio/FX/Units/Terran/JimRaynor/Annoyance_3.wav");
	fx_jimraynor_annoyance_4 = app->audio->loadFx("Audio/FX/Units/Terran/JimRaynor/Annoyance_4.wav");

	//Zergling
	fx_zergling_death = app->audio->loadFx("Audio/FX/Units/Zerg/Zergling/Death.wav");

	//Hydralisk
	fx_hydralisk_attack = app->audio->loadFx("Audio/FX/Units/Zerg/Hydralisk/Attack.wav");
	fx_hydralisk_death = app->audio->loadFx("Audio/FX/Units/Zerg/Hydralisk/Death.wav");

	//Mutalisk
	fx_mutalisk_death = app->audio->loadFx("Audio/FX/Units/Zerg/Mutalisk/Death.wav");

	//Ultralisk
	fx_ultralisk_attack_1 = app->audio->loadFx("Audio/FX/Units/Zerg/Ultralisk/Attack_1.wav");
	fx_ultralisk_attack_2 = app->audio->loadFx("Audio/FX/Units/Zerg/Ultralisk/Attack_2.wav");
	fx_ultralisk_attack_3 = app->audio->loadFx("Audio/FX/Units/Zerg/Ultralisk/Attack_3.wav");
	fx_ultralisk_death = app->audio->loadFx("Audio/FX/Units/Zerg/Ultralisk/Death.wav");

	return true;
}

// Called before the first frame
bool EntityManager::start()
{
	next_ID = 0;
	building_tile = app->tex->loadTexture("maps/PathTiles2.png");

	loadEntityFX();
	loadEntityTex();

	return true;
}

Entity* const EntityManager::addEntity(iPoint &pos, SPECIALIZATION type, bool direct_creation, uint manual_id)
{ BROFILER_CATEGORY("EntityManager::addEntity", Profiler::Color::Green)
	Entity *e = NULL;
	if (building_mode && !direct_creation)
	{
		building_mode = false;
		building_to_place->coll->to_delete = true;
		RELEASE(building_to_place);
	}

	switch (type)
	{
	// UNITS
		// TERRAN
	case(MARINE) :
		LOG("Creating Marine");
		e = new Marine(pos);
		break;
	case(SCV) :
		LOG("Creating SCV");
		e = new Scv(pos);
		break;
	case(TANK) :
		LOG("Creating Tank");
		e = new Tank(pos);
		break;
	case(MEDIC) :
		LOG("Creating Medic");
		e = new Medic(pos);
		break;
	case(JIM_RAYNOR) :
		LOG("Creating Jym Raynor");
		e = new JimRaynor(pos);
		break;
	case(FIREBAT) :
		LOG("Creating Firebat");
		e = new Firebat(pos);
		break;
		// ZERGLINGS
	case(ZERGLING) :
		LOG("Creating Zergling");
		e = new Zergling(pos);
		break;
	case(MUTALISK) :
		LOG("Creating Mutalisk");
		e = new Mutalisk(pos);
		break;
	case(HYDRALISK) :
		LOG("Creating Hydralisk");
		e = new Hydralisk(pos);
		break;
	case(ULTRALISK) :
		LOG("Creating Ultralisk");
		e = new Ultralisk(pos);
		break;
	// TERRAN BUILDINGS
	case(COMMANDCENTER) :
		LOG("Creating Command Center");
		building_to_place = (Building*)e;
		building_mode = !direct_creation;
		e = new CommandCenter(pos);
		break;
	case(BUNKER) :
		LOG("Creating Bunker");
		e = new Bunker(pos);
		building_to_place = (Building*)e;
		building_mode = !direct_creation;
		create_bunker = false;
		break;
	case(FACTORY) :
		LOG("Creating Factory");
		e = new Factory(pos);
		building_to_place = (Building*)e;
		building_mode = !direct_creation;
		create_factory = false;
		break;
	case(BARRACK) :
		LOG("Creating Barrack");
		e = new Barrack(pos);
		building_to_place = (Building*)e;
		building_mode = !direct_creation;
		create_barrack = false;
		break;
	case(BOMB) :
		LOG("Bomb created");
		e = new Bomb(pos);
		building_to_place = (Building*)e;
		building_mode = !direct_creation;
		break;
	case(BLUE) :
		LOG("Creating Blue");
		e = new Blue(pos);
		building_to_place = (Building*)e;
		building_mode = !direct_creation;
		create_blue = false;
		break;
	case(YELLOW) :
		LOG("Creating Yellow");
		e = new Yellow(pos);
		building_to_place = (Building*)e;
		building_mode = !direct_creation;
		create_yellow = false;
		break;
	case(RED) :
		LOG("Creating Red");
		e = new Red(pos);
		building_to_place = (Building*)e;
		building_mode = !direct_creation;
		create_red = false;
		break;
	}

	if (e != NULL && direct_creation)
	{
		if (manual_id > 0)
			e->id = manual_id;
		else
			e->id = ++next_ID;

		active_entities.insert(pair<uint, Entity*>(e->id, e));

		// Building creation, special treatment
		if (e->type == BUILDING)
		{
 			app->map->changeLogic(e->coll->rect, NO_WALKABLE);
			recalculatePaths(false);
		}
	}

	return e;
}

/*Method that makes the enemy move to this position*/
void EntityManager::SetEnemyToPos(Entity* e, iPoint pos)
{
	if (e->type == UNIT)
	{
		pos = app->map->worldToMap(app->map->data.back(), pos.x, pos.y);

		Unit* unit = (Unit*)e;
		if (unit->flying)
		{
			unit->path.clear();
			unit->path.push_back(pos);
			unit->has_target = true;
			unit->state = MOVE_ALERT;
		}
		else if (app->path->createPath(e->tile_pos, pos + unit->distance_to_center_selector, e->id) != -1)
			unit->state = WAITING_PATH_MOVE_ALERT;
	}
}

// Called each loop iteration
bool EntityManager::preUpdate()
{ BROFILER_CATEGORY("EntityManager::preUpdate", Profiler::Color::Green)

	deletionManager();  // Delete each entity marked to being deleted.

	if (app->game_manager->isGameStarted())
		handleSelection();

	if (debug)
		entityManualCreation();

	if (create_SCV)
	{
		map<uint, Entity*>::iterator it = active_entities.begin();
		fPoint pos_commander;
		// First, we need to know if any unit has been selected. 
		for (; it != active_entities.end(); ++it)
		{
			if (it->second->specialization == COMMANDCENTER)
			{
				iPoint position;
				pos_commander = it->second->pos;
				position.x = pos_commander.x + 20;
				position.y = pos_commander.y + 100;

				app->game_manager->updateResources(-scv_mineral_cost, 0);
				addEntity(position, SCV);
				break;
			}
		}
		create_SCV = false;
	}

	if (create_marine)
	{
		map<uint, Entity*>::iterator it = active_entities.begin();
		fPoint pos_barrack;
		// First, we need to know if any unit has been selected. 
		for (; it != active_entities.end(); ++it)
		{
			if (it->second->specialization == BARRACK)
			{
				iPoint position;
				pos_barrack = it->second->pos;
				app->game_manager->updateResources(-marine_mineral_cost, -marine_gas_cost);
				position.x = pos_barrack.x + 30;
				position.y = pos_barrack.y + 120;

				addEntity(position, MARINE);
				break;		
			}
		}
		create_marine = false;
	}

	if (create_medic)
	{
		map<uint, Entity*>::iterator it = active_entities.begin();
		fPoint pos_barrack;
		// First, we need to know if any unit has been selected. 
		for (; it != active_entities.end(); ++it)
		{
			if (it->second->specialization == BARRACK)
			{
				iPoint position;
				pos_barrack = it->second->pos;
				app->game_manager->updateResources(-medic_mineral_cost, -medic_gas_cost);
				position.x = pos_barrack.x + 30;
				position.y = pos_barrack.y + 120;

				addEntity(position, MEDIC);
				break;
			}
		}
		create_medic = false;
	}

	if (create_firebat)
	{
		map<uint, Entity*>::iterator it = active_entities.begin();
		fPoint pos_barrack;
		// First, we need to know if any unit has been selected. 
		for (; it != active_entities.end(); ++it)
		{
			if (it->second->specialization == BARRACK)
			{
				iPoint position;
				pos_barrack = it->second->pos;
				app->game_manager->updateResources(-firebat_mineral_cost, -firebat_gas_cost);
				position.x = pos_barrack.x + 30;
				position.y = pos_barrack.y + 120;

				addEntity(position, FIREBAT);
				break;
			}
		}
		create_firebat = false;
	}

	if (create_tank)
	{
		map<uint, Entity*>::iterator it = active_entities.begin();
		fPoint pos_factory;
		// First, we need to know if any unit has been selected. 
		for (; it != active_entities.end(); ++it)
		{
			if (it->second->specialization == FACTORY)
			{
				iPoint position;
				pos_factory = it->second->pos;
				app->game_manager->updateResources(-tank_mineral_cost, -tank_gas_cost);
				position.x = pos_factory.x + 30;
				position.y = pos_factory.y + 120;
				addEntity(position, TANK);
				break;
			}
		}
		create_tank = false;
	}

	if (create_bunker)
	{
		if (app->game_manager->mineral_resources >= bunker_mineral_cost && app->game_manager->gas_resources >= bunker_gas_cost)
		{
			iPoint position;
			app->input->getMousePosition(position);
			position = app->render->screenToWorld(position.x, position.y);
			addEntity(position, BUNKER, false);
		}
		create_bunker = false;
	}

	if (create_barrack)
	{
		if (app->gui->barrackAlive == true)
		{
			create_barrack = false;
		}
		else
		{
			if (app->game_manager->mineral_resources >= barrack_mineral_cost && app->game_manager->gas_resources >= barrack_gas_cost)
			{
				iPoint position;
				app->input->getMousePosition(position);
				position = app->render->screenToWorld(position.x, position.y);
				addEntity(position, BARRACK, false);
			}
			create_barrack = false;
		}

	}

	if (create_factory)
	{
		if (app->gui->factoryAlive == true)
		{
			create_factory = false;
		}
		else
		{
			if (app->game_manager->mineral_resources >= factory_mineral_cost && app->game_manager->gas_resources >= factory_gas_cost)
			{
				iPoint position;
				app->input->getMousePosition(position);
				position = app->render->screenToWorld(position.x, position.y);
				addEntity(position, FACTORY, false);
			}
			create_factory = false;
		}
	}

	return true;
}


// Called each loop iteration
bool EntityManager::update(float dt)
{BROFILER_CATEGORY("EntityManager::update", Profiler::Color::Green)
	// Debug ---
	if (app->input->getKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	map<uint, Entity*>::iterator it = active_entities.begin();
	for (; it != active_entities.end(); ++it)
			it->second->update(dt);

	return true;
}

// Called each loop iteration
bool EntityManager::postUpdate()
{BROFILER_CATEGORY("EntityManager::postUpdate", Profiler::Color::Green)
	// Entities Drawing
	// Look on Gui postUpdate()!

	// Drawing selector (green SDL_Rect)
	if (selector_init && selector.w > 1 && selector.h > 1) app->render->DrawQuad(selector, 35, 114, 48, 255, false, true);

	if (building_mode)
	{
		if (app->game_manager->game_state == WIN || app->game_manager->game_state == LOSE || app->game_manager->game_state == QUIT || app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			building_mode = false;
			building_to_place->coll->to_delete = true;
			RELEASE(building_to_place);
		}
		else
			choosePlaceForBuilding();
	}

	return true;
}

// Called before quitting
bool EntityManager::cleanUp()
{
	map<uint, Entity*>::iterator it = active_entities.begin();
	for (; it != active_entities.end(); it++)
	{
			RELEASE(it->second);
	}
	active_entities.clear();
	selection.clear();
	siege_tanks.clear();

	SDL_DestroyTexture(marine_tex);
	SDL_DestroyTexture(scv_tex);
	SDL_DestroyTexture(medic_tex);
	SDL_DestroyTexture(firebat_tex);
	SDL_DestroyTexture(jim_raynor_tex);
	SDL_DestroyTexture(tank_tex);

	SDL_DestroyTexture(zergling_tex);
	SDL_DestroyTexture(hydralisk_tex);
	SDL_DestroyTexture(mutalisk_tex);
	SDL_DestroyTexture(ultralisk_tex);

	SDL_DestroyTexture(bunker_tex);
	SDL_DestroyTexture(factory_tex);
	SDL_DestroyTexture(bomb_tex);
	SDL_DestroyTexture(command_center_tex);
	SDL_DestroyTexture(barrack_tex);

	return true;
}

// Return ID for the corresponding entity
Entity *EntityManager::getEntity(uint id)
{
	map<uint, Entity*>::iterator it = active_entities.find(id);
	return (it != active_entities.end() ? it->second : NULL);
}

// WhichEntityOnMouse: Returns an entity under the mouse cursor
Entity *EntityManager::whichEntityOnMouse()
{
	iPoint p; app->input->getMousePosition(p);
	p = app->render->screenToWorld(p.x, p.y);

	map<uint, Entity*>::reverse_iterator rit = active_entities.rbegin();
	for (; rit != active_entities.rend(); ++rit)
	{
		if (p.x >= rit->second->coll->rect.x &&
			p.x <= rit->second->coll->rect.x + rit->second->coll->rect.w &&
			p.y >= rit->second->coll->rect.y &&
			p.y <= rit->second->coll->rect.y + rit->second->coll->rect.h)
			return rit->second;
	}
	return NULL;
}

// CalculateSelector: Method that computes the dimensions of the green SDL_Rect selector.
void EntityManager::calculateSelector()
{
	// If dimensions of the selector are less than 1/1 (for width/height), it sets 1. Eliminates 0 width/height to proper alone selection. 
	int selector_width = abs(final_selector_pos.x - initial_selector_pos.x) > 1 ? abs(final_selector_pos.x - initial_selector_pos.x) : 1;
	int selector_height = abs(final_selector_pos.y - initial_selector_pos.y) > 1 ? abs(final_selector_pos.y - initial_selector_pos.y) : 1;
	int selector_pos_x = (initial_selector_pos.x < final_selector_pos.x ? initial_selector_pos.x : final_selector_pos.x);
	int selector_pos_y = (initial_selector_pos.y < final_selector_pos.y ? initial_selector_pos.y : final_selector_pos.y);
	selector = { selector_pos_x, selector_pos_y, selector_width, selector_height };
}

void EntityManager::handleSelection()
{ BROFILER_CATEGORY("EntityManager::handleSelection", Profiler::Color::Green)
	// Clicking and holding left button, starts a selection
	if (!building_mode && app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		//selection.clear();
		app->input->getMousePosition(initial_selector_pos);

		if (initial_selector_pos.y <= 348)  // It's the distance from the top of the screen to the top of the Minimap 
		{
			initial_selector_pos = app->render->screenToWorld(initial_selector_pos.x, initial_selector_pos.y);
			
			//Click and select unit	
			selector_init = true;
			Entity *e = whichEntityOnMouse();

			if (e != NULL)
			{
				selector = { e->coll->rect.x, e->coll->rect.y, 1, 1 };
			}
			else
				selector = { 0, 0, 0, 0 };
		}		
	}

	// Holding left button, updates selector dimensions with calculateSelector
	if (selector_init && app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		app->input->getMousePosition(final_selector_pos);
		final_selector_pos = app->render->screenToWorld(final_selector_pos.x, final_selector_pos.y);

		calculateSelector();
	}

	// Once released left button, the selection is computed. This selection doesn't take buildings and units
	// there are both present. Or buildings (only one) or units (all of them)
	if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		selector_init = false;
		bool units_only = false;    // If only one unit is on selection, buildings will be excluded
		map<uint, Entity*>::iterator it = active_entities.begin(); 
		int delete_selection = 0;

		// First, we need to know if any unit has been selected. 
		for (; it != active_entities.end(); ++it)
		{
			if (it->second->coll->checkCollision(selector))
			{
				if (it->second->type == UNIT)
					units_only = true;
			}
		}

		// Now, we include the entities according to the only_units boolean variable.
		for (it = active_entities.begin(); it != active_entities.end(); ++it)
		{
			if (it->second->coll->checkCollision(selector))
			{
				if (delete_selection == 0)
				{
						delete_selection++;
				}
				// On debug mode, the player will select all the entities.
				if (it->second->faction == COMPUTER && !debug)
					continue;
				else
				{
					if (delete_selection == 1)
					{
						selection.clear();
						delete_selection++;
					}

					if (it->second->type == UNIT)
					{
						Unit *u = (Unit*)it->second;
						u->distance_to_center_selector = u->tile_pos - app->map->worldToMap(app->map->data.back(), selector.x + (selector.w / 2), selector.y + (selector.h / 2));
						selection.insert(pair<uint, Entity*>(it->first, it->second));
					}

					if (!units_only && it->second->type == BUILDING)
					{
						selection.insert(pair<uint, Entity*>(it->first, it->second));
						break;
					}
				}
			}
		}
	}

	// Now, we give destination for the entities englobed by selection.
	if (!selection.empty() && app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		// Target position is where the player has clicked to move his units.
		iPoint target_position;
		app->input->getMousePosition(target_position);
		if (app->gui->mini_map->isMouseInside())
			target_position = app->gui->mini_map->minimapToWorld({ target_position.x, target_position.y });
		else 
			target_position = app->render->screenToWorld(target_position.x, target_position.y);
		target_position = app->map->worldToMap(app->map->data.back(), target_position.x, target_position.y);

		//Bunker and bomb useful method
		Entity* e = whichEntityOnMouse();

		map<uint, Entity*>::iterator it = selection.begin();
		for (; it != selection.end(); ++it)
		{
			if (it->second->type == UNIT && it->second->state != DYING)
			{
				Unit *unit = (Unit*)it->second;

				if (it->second->specialization == TANK)
				{
					if (((Tank*)unit)->siege_mode)
						continue;
					else if (it->second->state == SIEGE_MODE_OFF)
						continue;
				}

				if (selection.size() == 1)
				{
					if (unit->flying)
					{
						unit->path.clear();
						unit->path.push_back(target_position);
						unit->has_target = true;
						if (app->shortcuts->isCommandActive(FOCUS_ATTACK))
							unit->state = MOVE_ALERT;
						else
							unit->state = MOVE;
					}
					else if (app->path->createPath(unit->tile_pos, target_position, unit->id) != -1)
					{
						if (app->shortcuts->isCommandActive(FOCUS_ATTACK))
							unit->state = WAITING_PATH_MOVE_ALERT;
						else
							unit->state = WAITING_PATH_MOVE;
					}
				}
				else
				{
					iPoint target = target_position + unit->distance_to_center_selector;
					if (unit->flying)
					{
						unit->path.clear();
						unit->path.push_back(target);
						unit->has_target = true;
						if (app->shortcuts->isCommandActive(FOCUS_ATTACK))
							unit->state = MOVE_ALERT;
						else
							unit->state = MOVE;
					}
					else if (app->path->createPath(unit->tile_pos, target, unit->id) != -1)
					{
						if (app->shortcuts->isCommandActive(FOCUS_ATTACK))
							unit->state = WAITING_PATH_MOVE_ALERT;
						else
							unit->state = WAITING_PATH_MOVE;
					}
				}
				
				if (it->second->specialization == MARINE || it->second->specialization == FIREBAT || it->second->specialization == JIM_RAYNOR)
				{
					if (it->second->specialization == MARINE)
						((Marine*)unit)->bunker_to_fill = NULL;
					else if (it->second->specialization == FIREBAT)
						((Firebat*)unit)->bunker_to_fill = NULL;
					else //(it->second->specialization == JYM_RAYNOR)
					{
						JimRaynor *jim = (JimRaynor*)it->second;
						jim->bunker_to_fill = NULL;
						jim->bomb = NULL;
						jim->bomb_activated = false;
					}
				}

				if (e != NULL)
				{
					if ((it->second->specialization == MARINE && e->specialization == BUNKER))
					{
						((Marine*)unit)->bunker_to_fill = (Bunker*)e;
					}
					else if ((it->second->specialization == FIREBAT && e->specialization == BUNKER))
					{
						((Firebat*)unit)->bunker_to_fill = (Bunker*)e;
					}
					else if (it->second->specialization == JIM_RAYNOR)
					{
						JimRaynor *jim = (JimRaynor*)it->second;
						if (e->specialization == BOMB)
							jim->bomb = (Bomb*)e;

						else if (jim->bomb_taken && e->specialization == COMMANDCENTER)
							jim->bomb_activated = true;

						else if (e->specialization == BUNKER)
						{
							jim->bunker_to_fill = (Bunker*)e;
						}
					}
					else if (it->second->faction == PLAYER && it->second->type == UNIT && e->faction != PLAYER && e->specialization != BOMB)
					{
						unit->target_to_attack = e;
						unit->has_focus = true;
						unit->newEntityFound();
					}
				}
			}
		}
	}

	// When we are in building mode, clicking left mouse button put the building on the clicked position.
	// The logic is modified to not walk over building.
	if (building_mode && app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (app->map->isAreaWalkable(building_to_place->coll->rect, true) && (!app->fog_of_war->isEnabled() || app->fog_of_war->isVisible(building_to_place->pos.x, building_to_place->pos.y)))
		{
			iPoint first_tile = app->map->worldToMap(app->map->data.back(), building_to_place->coll->rect.x, building_to_place->coll->rect.y) - (iPoint{ 1, 1 });
			iPoint last_tile = app->map->worldToMap(app->map->data.back(), building_to_place->coll->rect.x + building_to_place->coll->rect.w, building_to_place->coll->rect.y + building_to_place->coll->rect.h) + (iPoint{ 1, 1 });
			fPoint point_to_draw;

			bool tank_in_siege_mode = false;
			for (int y = first_tile.y; y < last_tile.y; ++y)
			{
				for (int x = first_tile.x; x < last_tile.x; ++x)
				{
					point_to_draw = app->map->mapToWorld(app->map->data.back(), x, y);
					for (list<iPoint>::iterator it = siege_tanks.begin(); it != siege_tanks.end(); it++)
					{
						if (it._Ptr->_Myval == iPoint{ x, y })
							tank_in_siege_mode = true;
					}
				}
			}
			if (!tank_in_siege_mode)
			{
				building_to_place->id = ++next_ID;
				building_to_place->tile_pos = app->map->worldToMap(app->map->data.back(), building_to_place->center.x, building_to_place->center.y);
				active_entities.insert(pair<uint, Entity*>(next_ID, building_to_place));
				building_mode = false;

				switch (building_to_place->specialization)
				{
				case(BUNKER) :
				{
					app->game_manager->updateResources(-bunker_mineral_cost, -bunker_gas_cost);
					break;
				}
				case(BARRACK) :
				{
					app->game_manager->updateResources(-barrack_mineral_cost, -barrack_gas_cost);
					break;
				}
				case(FACTORY) :
				{
					app->game_manager->updateResources(-factory_mineral_cost, -factory_gas_cost);
					break;
				}
				}

				app->map->changeLogic(building_to_place->coll->rect, NO_WALKABLE);
				recalculatePaths(false);
			}
		}
	}
}

// We delete the entities marked with to_delete
void EntityManager::deletionManager()
{ BROFILER_CATEGORY("EntityManager::deletionManager", Profiler::Color::Green)
	bool need_recalculatePath = false;
	map<uint, Entity*>::iterator it = active_entities.begin();
	for (; it != active_entities.end();)
	{
		if (it->second->to_delete)
		{
			// Maybe, the entity removed is someone's entity_to_attack. Now, it's not.  CRZ
			for (map<uint, Entity*>::iterator it2 = active_entities.begin(); it2 != active_entities.end(); ++it2)
			{
				if (it->second == it2->second->target_to_attack)
					it2->second->target_to_attack = NULL;
				if (it2->second->specialization == MUTALISK)
				{
					if (it->second == ((Mutalisk*)it2->second)->second_target_to_attack)
						((Mutalisk*)it2->second)->second_target_to_attack == NULL;
				}
			}

			app->path->erase(it->first);

			if (it->second->type == BUILDING)
			{
				if (it->second->specialization == COMMANDCENTER)
				{
					if (!loading_game)
						app->game_manager->command_center_destroyed = true;
				}
				else if (it->second->specialization == BUNKER)
				{
					//app->gui->bunker_to_leave.remove((Bunker*)(it->second));
					for (list<Bunker*>::iterator itb = app->gui->bunker_to_leave.begin(); itb != app->gui->bunker_to_leave.end();)
					{
						if (it->first == itb._Ptr->_Myval->id)
							itb = app->gui->bunker_to_leave.erase(itb);
						else
							itb++;
					}

					for (map<uint, Entity*>::iterator it2 = active_entities.begin(); it2 != active_entities.end(); ++it2)
					{
						if (it2->second->specialization == MARINE)
						{
							if (!((Marine*)it2->second)->inside_bunker)
								((Marine*)it2->second)->bunker_to_fill == NULL;
						}
						else if (it2->second->specialization == FIREBAT)
						{
							if (!((Firebat*)it2->second)->inside_bunker)
								((Firebat*)it2->second)->bunker_to_fill == NULL;
						}
						else if (it2->second->specialization == JIM_RAYNOR)
						{
							if (!((JimRaynor*)it2->second)->inside_bunker)
								((JimRaynor*)it2->second)->bunker_to_fill == NULL;
						}
					}
				}
				app->map->changeLogic(it->second->coll->rect, LOW_GROUND);
				need_recalculatePath = true;
			}
			else if (it->second->specialization == TANK)
				siege_tanks.remove(it->second->tile_pos);

			// Very disgusting code to mantain Marines inside a bunker // CRZ
			selection.erase(it->first);
			switch (it->second->specialization)
			{
			case(MARINE):
				if (!((Marine*)it->second)->inside_bunker)
					RELEASE(it->second);
				break;
			case(FIREBAT):
				if (!((Firebat*)it->second)->inside_bunker)
					RELEASE(it->second);
				break;
			case(JIM_RAYNOR):
				if (!((JimRaynor*)it->second)->inside_bunker)
				{
					if (!loading_game) 
						app->game_manager->jim_raynor_dead = true;
					app->game_manager->jim_raynor = NULL;
					RELEASE(it->second);
				}
				break;
			default:
				RELEASE(it->second);
			}
		
			it = active_entities.erase(it);
		}
		else
			++it;
	}

	if (need_recalculatePath)
	{
		app->entity_manager->recalculatePaths(true);
	}

	if (loading_game)
		loading_game = false;
}

/*------------------WAVE RELATED METHODS--------------------------*/
Entity* EntityManager::searchNearestEntityInRange(Entity* e, bool search_only_in_same_faction, float range) //The method ONLY search and return the nearest entity
{ BROFILER_CATEGORY("EntityManager::searchNearestEntityInRange", Profiler::Color::Green)
	Entity* ret = NULL;
	if (e != NULL)
	{
		float value = range;
		if (value == -1.0f)
			value = e->range_of_vision;
		map<uint, Entity*>::iterator it = active_entities.begin();
		for (; it != active_entities.end(); ++it)
		{
			if (it->second != e && it->second->state != DYING &&
				((!search_only_in_same_faction || e->faction == it->second->faction) && (search_only_in_same_faction || e->faction != it->second->faction)) && it->second->specialization != BOMB)
			{
				float d = abs(e->center.x - it->second->center.x) + abs(e->center.y - it->second->center.y);
				d -= ((e->coll->rect.w / 2 + e->coll->rect.h / 2) / 2 + (it->second->coll->rect.w / 2 + it->second->coll->rect.h / 2) / 2);
				if (d <= value)
				{
					ret = &(*it->second);
					value = d;
				}
			}
		}
	}
	return ret;
}

list<Entity*> EntityManager::searchEntitiesInRange(Entity* e, bool search_only_in_same_faction, float range, bool can_attack_to_flying) //The method search and return the entity in the area
{ BROFILER_CATEGORY("EntityManager::searchEntitiesInRange", Profiler::Color::Green)
	list<Entity*> ret;
	if (e != NULL)
	{
		float value = range;
		if (value == -1.0f)
			value = e->range_of_vision;
		map<uint, Entity*>::iterator it = active_entities.begin();
		for (; it != active_entities.end(); ++it)
		{
			if (it->second != e && it->second->state != DYING && (!search_only_in_same_faction || e->faction == it->second->faction) && it->second->specialization != BOMB)
			{
				if (!can_attack_to_flying)
				{
					if (it->second->type == UNIT && ((Unit*)it->second)->flying)
					{
						continue;
					}
				}
				float d = abs(e->center.x - it->second->center.x) + abs(e->center.y - it->second->center.y);
				d -= ((e->coll->rect.w / 2 + e->coll->rect.h / 2) / 2 + (it->second->coll->rect.w / 2 + it->second->coll->rect.h / 2) / 2);
				if (d <= value)
				{
					ret.push_back(it->second);
				}
			}
		}
	}
	return ret;
}

list<Entity*> EntityManager::searchEnemiesInRange(Entity* e, float range, bool can_attack_to_flying)
{ BROFILER_CATEGORY("EntityManager::searchEnemiesInRange", Profiler::Color::Green)
	list<Entity*> ret;
	if (e != NULL)
	{
		float value = range;
		if (value == -1.0f)
			value = e->range_of_vision;
		map<uint, Entity*>::iterator it = active_entities.begin();
		for (; it != active_entities.end(); ++it)
		{
			if (it->second != e && it->second->state != DYING && it->second->faction == COMPUTER && it->second->specialization != BOMB)
			{
				if (!can_attack_to_flying)
				{
					if (it->second->type == UNIT && ((Unit*)it->second)->flying)
					{
						continue;
					}
				}
				float d = abs(e->center.x - it->second->center.x) + abs(e->center.y - it->second->center.y);
				d -= ((e->coll->rect.w / 2 + e->coll->rect.h / 2) / 2 + (it->second->coll->rect.w / 2 + it->second->coll->rect.h / 2) / 2);
				if (d <= value)
				{
					ret.push_back(it->second);
				}
			}
		}
	}
	return ret;
}

bool EntityManager::checkFocus(Unit* e)
{ BROFILER_CATEGORY("EntityManager::checkFocus", Profiler::Color::Green)
	bool ret = false;
	if (e->target_to_attack != NULL && e->target_to_attack->state != DYING)
	{
		ret = true;
	}
	if (!ret)
		e->has_focus = false;
	return ret;
}

Entity* EntityManager::searchEnemyToAttack(Entity* e, bool can_attack_to_flying, float min_area_range)
{ BROFILER_CATEGORY("EntityManager::searchEnemyToAttack", Profiler::Color::Green)
	Entity* ret = NULL;
	float value = e->range_of_vision;
	map<uint, Entity*>::iterator it = active_entities.begin();
	float previousMaxHP = 100.0f;
	for (; it != active_entities.end(); ++it)
	{
		if (it->second != e && it->second->state != DYING && e->faction != it->second->faction && it->second->specialization != BOMB)
		{
			if (!can_attack_to_flying)
			{
				if (it->second->type == UNIT && ((Unit*)it->second)->flying)
				{
					continue;
				}
			}
			float d = abs(e->center.x - it->second->center.x) + abs(e->center.y - it->second->center.y);
			d -= ((e->coll->rect.w / 2 + e->coll->rect.h / 2) / 2 + (it->second->coll->rect.w / 2 + it->second->coll->rect.h / 2) / 2);
			float maxHP = (it->second->current_hp * 100.0f) / it->second->max_hp;

			if (ret == NULL && d > min_area_range && d <= value && maxHP <= previousMaxHP)
			{
				value = d;
				previousMaxHP = maxHP;
				ret = it->second;
			}
			else if (ret != NULL)
			{
				//Only search entities with same type or if type is building, it search units
				if ((ret->type == it->second->type && d > min_area_range && ((d <= value && maxHP <= previousMaxHP) || d <= e->range_of_vision && maxHP <= (previousMaxHP + 25.0f))) ||
					(ret->type == BUILDING && it->second->type == UNIT && d > min_area_range &&  d <= e->range_of_vision && maxHP <= (previousMaxHP + 50.0f)))
				{
					value = d;
					previousMaxHP = maxHP;
					ret = it->second;
				}
			}
		}
	}
	return ret;
}

Entity* EntityManager::searchAllyToHeal(Entity* e, bool search_only_buildings)
{ BROFILER_CATEGORY("EntityManager::searchAllyToHeal", Profiler::Color::Green)
	Entity* ret = NULL;
	float value = e->range_of_vision;
	map<uint, Entity*>::iterator it = active_entities.begin();
	float previousMaxHP = 100.0f;
	for (; it != active_entities.end(); ++it)
	{
		if (it->second != e && it->second->state != DYING && e->faction == it->second->faction && it->second->specialization != BOMB &&
			((!search_only_buildings && it->second->type == UNIT && it->second->specialization != TANK) ||
			(search_only_buildings && (it->second->type == BUILDING || it->second->specialization == TANK))) 
			&& it->second->current_hp < it->second->max_hp)
		{
			float d = abs(e->center.x - it->second->center.x) + abs(e->center.y - it->second->center.y);
			d -= ((e->coll->rect.w / 2 + e->coll->rect.h / 2) / 2 + (it->second->coll->rect.w / 2 + it->second->coll->rect.h / 2) / 2);
			float maxHP = (it->second->current_hp * 100.0f) / it->second->max_hp;

			if (d <= value && maxHP <= previousMaxHP)
			{
				value = d;
				previousMaxHP = maxHP;
				ret = it->second;
			}
		}
	}
	return ret;
}

void EntityManager::choosePlaceForBuilding()
{ BROFILER_CATEGORY("EntityManager::choosePlaceForBuilding", Profiler::Color::Green)
	iPoint p; app->input->getMousePosition(p);
	iPoint pos = app->render->screenToWorld( p.x - (building_to_place->tex_width / 2), p.y - (building_to_place->tex_height / 2));
	building_to_place->pos = { (float)pos.x, (float)pos.y };
	
	iPoint center = app->render->screenToWorld(p.x, p.y);
	building_to_place->center = { (float)center.x, (float)center.y };
	
	building_to_place->coll->setPos(building_to_place->pos.x + building_to_place->collider_offset.x, building_to_place->pos.y + building_to_place->collider_offset.y);

	iPoint first_tile = app->map->worldToMap(app->map->data.back(), building_to_place->coll->rect.x, building_to_place->coll->rect.y) - (iPoint{ 1, 1 });
	iPoint last_tile = app->map->worldToMap(app->map->data.back(), building_to_place->coll->rect.x + building_to_place->coll->rect.w, building_to_place->coll->rect.y + building_to_place->coll->rect.h) + (iPoint{ 1, 1 });
	fPoint point_to_draw;

	for (int y = first_tile.y; y < last_tile.y; ++y)
	{
		for (int x = first_tile.x; x < last_tile.x; ++x)
		{
			point_to_draw = app->map->mapToWorld(app->map->data.back(), x, y);
			bool tank_in_siege_mode = false;
			for (list<iPoint>::iterator it = siege_tanks.begin(); it != siege_tanks.end(); it++)
			{
				if (it._Ptr->_Myval == iPoint{ x, y })
					tank_in_siege_mode = true;
			}
			if (app->path->isWalkable({ x, y }) && !tank_in_siege_mode && (!app->fog_of_war->isEnabled() || app->fog_of_war->isVisible(building_to_place->pos.x, building_to_place->pos.y)))
			{
				SDL_Rect r = { 0, 0, 16, 16 };
				app->render->blit(building_tile, point_to_draw.x, point_to_draw.y, &r);
			}
			else
			{
				SDL_Rect r = { 16, 0, 16, 16 };
				app->render->blit(building_tile, point_to_draw.x, point_to_draw.y, &r);
			}
		}
	}

	SDL_Texture *tex = NULL;
	switch (building_to_place->specialization)
	{
	case(BARRACK) :
		tex = barrack_tex;
		break;
	case(BUNKER) :
		tex = bunker_tex;
		break;
	case(FACTORY) :
		tex = factory_tex;
		break;
	}

	app->render->blit(tex, building_to_place->pos.x, building_to_place->pos.y, &building_to_place->current_animation->getCurrentFrame());
}

void EntityManager::recalculatePaths(bool walkable)
{ BROFILER_CATEGORY("EntityManager::recalculatePaths", Profiler::Color::Green)
	map<uint, Entity*>::iterator it = active_entities.begin();
	for (; it != active_entities.end(); ++it)
	{
		if (it->second->type == UNIT && it->second->state != DYING && it->second->to_delete == false && 
			it->second->state != SIEGE_MODE_OFF && it->second->state != IDLE_SIEGE_MODE && it->second->state != ATTACK_SIEGE_MODE)
		{
			Unit *unit = (Unit*)it->second;
			if (!unit->flying)
			{
				if ((unit->state == WAITING_PATH_MOVE || unit->state == WAITING_PATH_MOVE_ALERT || unit->state == WAITING_PATH_MOVE_ALERT_TO_ATTACK) && 
					app->path->recalculatePath(it->second->tile_pos, it->first));
				else if (unit->path.size() > 0 && app->path->createPath(it->second->tile_pos, unit->path.back(), it->first) != -1)
				{
					if (unit->state == MOVE)
						unit->state = WAITING_PATH_MOVE;
					else if (unit->state == MOVE_ALERT)
						unit->state = WAITING_PATH_MOVE_ALERT;
					else if (unit->state == MOVE_ALERT_TO_ATTACK)
						unit->state = WAITING_PATH_MOVE_ALERT_TO_ATTACK;
				}
				else if (walkable == false)
				{
					if (unit->path.size() > 0)
					{
						if (!app->path->isWalkable(unit->path.back()))//if the origin and the destination isn't walkable
						{
							unit->path.clear();
							unit->path.push_back(app->path->findNearestWalkableTile(unit->tile_pos, app->game_manager->command_center_position, 50));
							unit->has_target = true;
							unit->state = MOVE;
						}
						else if (it->second->state != MOVE || it->second->state != MOVE_ALERT || it->second->state != MOVE_ALERT_TO_ATTACK)
						{
							unit->has_target = true;
							unit->state = MOVE;
						}
					}
					else if (!app->path->isWalkable(unit->tile_pos))//if without path and they are no walkable tiles
					{
						unit->path.push_back(app->path->findNearestWalkableTile(unit->tile_pos, app->game_manager->command_center_position, 50));
						unit->has_target = true;
						unit->state = MOVE;
					}
				}
			}
		}
	}
}

void EntityManager::onCollision(Collider* c1, Collider* c2)
{ BROFILER_CATEGORY("EntityManager::onCollision", Profiler::Color::Green)
	Entity *e1, *e2;
	e1 = e2 = NULL;

	map<uint, Entity*>::iterator it = active_entities.begin();
	for (; it != active_entities.end(); ++it)
	{
		if (it->second->coll == c1) e1 = it->second;
		else if (it->second->coll == c2) e2 = it->second;
	}
	
	if (e1 != NULL && e2 != NULL && e1->type == UNIT && e1->state == IDLE && e2->type == UNIT && e2->state == IDLE)
	{
		if (app->path->createPathToAdjacent(e2->tile_pos, 2) != -1)
		{
			Unit *unit = (Unit*)e2;
			unit->path = app->path->getLastPath();
			unit->has_target = true;
			unit->state = MOVE;
		}
	}
	else if (e1 != NULL && e2 != NULL && e1->type == UNIT && e1->state == IDLE_SIEGE_MODE && e2->type == UNIT && e2->state == IDLE)
	{
		if (app->path->createPathToAdjacent(e2->tile_pos, 2) != -1)
		{
			Unit *unit = (Unit*)e2;
			unit->path = app->path->getLastPath();
			unit->has_target = true;
			unit->state = MOVE;
		}
	}
	else if (e1 != NULL && e2 != NULL && e1->type == UNIT && e1->state == IDLE && e2->type == UNIT && e2->state == IDLE_SIEGE_MODE)
	{
		if (app->path->createPathToAdjacent(e1->tile_pos, 2) != -1)
		{
			Unit *unit = (Unit*)e1;
			unit->path = app->path->getLastPath();
			unit->has_target = true;
			unit->state = MOVE;
		}
	}

}

void EntityManager::entityManualCreation()
{
	iPoint position;
	//if (app->input->getKey(SDL_SCANCODE_KP_0) == KEY_DOWN)
	//{
	//	app->input->getMousePosition(position);
	//	position = app->render->screenToWorld(position.x, position.y);
	//	addEntity(position, BLUE);
	//}

	if (app->input->getKey(SDL_SCANCODE_KP_1) == KEY_DOWN)
	{
		app->input->getMousePosition(position);
		position = app->render->screenToWorld(position.x, position.y);
		addEntity(position, MARINE);
	}

	if (app->input->getKey(SDL_SCANCODE_KP_2) == KEY_DOWN)
	{
		app->input->getMousePosition(position);
		position = app->render->screenToWorld(position.x, position.y);
		addEntity(position, ZERGLING);
	}

	if (app->input->getKey(SDL_SCANCODE_KP_3) == KEY_DOWN)
	{
		app->input->getMousePosition(position);
		position = app->render->screenToWorld(position.x, position.y);
		addEntity(position, SCV);
	}

	if (app->input->getKey(SDL_SCANCODE_KP_4) == KEY_DOWN)
	{
		app->input->getMousePosition(position);
		position = app->render->screenToWorld(position.x, position.y);
		addEntity(position, MUTALISK);
	}

	if (app->input->getKey(SDL_SCANCODE_KP_5) == KEY_DOWN)
	{
		app->input->getMousePosition(position);
		position = app->render->screenToWorld(position.x, position.y);
		addEntity(position, HYDRALISK);
	}

	if (app->input->getKey(SDL_SCANCODE_KP_6) == KEY_DOWN)
	{
		app->input->getMousePosition(position);
		position = app->render->screenToWorld(position.x, position.y);
		addEntity(position, TANK);
	}

	if (app->input->getKey(SDL_SCANCODE_KP_7) == KEY_DOWN)
	{
		app->input->getMousePosition(position);
		position = app->render->screenToWorld(position.x, position.y);
		addEntity(position, MEDIC);
	}

	if (app->input->getKey(SDL_SCANCODE_KP_8) == KEY_DOWN)
	{
		app->input->getMousePosition(position);
		position = app->render->screenToWorld(position.x, position.y);
		addEntity(position, FIREBAT);
	}

	if (app->input->getKey(SDL_SCANCODE_KP_9) == KEY_DOWN)
	{
		app->input->getMousePosition(position);
		position = app->render->screenToWorld(position.x, position.y);
		addEntity(position, ULTRALISK);
	}

	//if (app->input->getKey(SDL_SCANCODE_J) == KEY_DOWN)
	//{
	//	app->input->getMousePosition(position);
	//	position = app->render->screenToWorld(position.x, position.y);
	//	addEntity(position, JIM_RAYNOR);
	//}

	//if (app->input->getKey(SDL_SCANCODE_B) == KEY_DOWN)
	//{
	//	app->input->getMousePosition(position);
	//	position = app->render->screenToWorld(position.x, position.y);
	//	addEntity(position, BARRACK, false);
	//}

	if (app->input->getKey(SDL_SCANCODE_N) == KEY_DOWN)
	{
		app->input->getMousePosition(position);
		position = app->render->screenToWorld(position.x, position.y);
		addEntity(position, BOMB, false);
	}

	if (app->input->getKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		app->input->getMousePosition(position);
		position = app->render->screenToWorld(position.x, position.y);
		addEntity(position, BUNKER, true);
	}
}

void EntityManager::updateFogOfWar()
{ BROFILER_CATEGORY("EntityManager::updateFogOfWar", Profiler::Color::Green)
	//FOG_OF_WAR 2 - "Update Fog of War" function. Called once every frame.
	//Drawing all a circle on the map for each allied unit and building
	
	if (active_entities.empty() == false)
	{
		map<uint, Entity*>::iterator it = active_entities.begin();
		while (it != active_entities.end())
		{
			 if (it->second->faction == PLAYER)
				app->fog_of_war->drawCircle(it->second->center.x, it->second->center.y, it->second->range_of_vision);
			++it;
		}
	}
}

// Load / Save
bool EntityManager::load(pugi::xml_node &node)
{
	// First, we must clean all the units of the current game:
	map<uint, Entity*>::const_iterator it = active_entities.begin();
	for (; it != active_entities.end(); ++it)
	{
		it->second->to_delete = true;
		it->second->coll->to_delete = true;
	}

	loading_game = true;
	deletionManager();

	// Now, loading the entities stored on the XML file.
	for (pugi::xml_node tmp = node.child("active_entities").child("entity"); tmp; tmp = tmp.next_sibling("entity"))
	{
		uint id = tmp.attribute("id").as_uint();
		iPoint pos = { tmp.attribute("pos_x").as_int(), tmp.attribute("pos_y").as_int() };
		SPECIALIZATION spec = (SPECIALIZATION)tmp.attribute("type").as_int();

		Entity *reload_entity = addEntity(pos, spec, true, id);
		reload_entity->current_hp = tmp.attribute("current_hp").as_float();
		reload_entity->tile_pos = {tmp.attribute("tile_pos_x").as_int(), tmp.attribute("tile_pos_y").as_int() };
		reload_entity->to_delete = tmp.attribute("to_delete").as_bool();
		reload_entity->state = (STATE)tmp.attribute("state").as_int();
		reload_entity->damage = tmp.attribute("damage").as_float();
		reload_entity->damage_multiplier = tmp.attribute("damage_multiplier").as_float();
		reload_entity->speed_multiplier = tmp.attribute("speed_multiplier").as_float();

		if (reload_entity->type == UNIT)
		{
			Unit* u = (Unit*)reload_entity;
			u->flying = tmp.attribute("flying").as_bool();
			u->target_pos.x = tmp.attribute("target_pos_x").as_int();
			u->target_pos.y = tmp.attribute("target_pos_y").as_int();
		}
		
		// Loading special variables according its state.
		switch (reload_entity->state)
		{
		case(MOVE):
		case(MOVE_ALERT):
		{
			Unit* u = (Unit*)reload_entity;
			u->has_target = tmp.attribute("has_target").as_bool();
			u->grouped = tmp.attribute("grouped").as_bool();
			u->group_speed = tmp.attribute("group_speed").as_float();
			for (pugi::xml_node pos = tmp.child("position_to_move"); pos; pos = pos.next_sibling("position_to_move"))
				u->path.push_back({ pos.attribute("x").as_int(), pos.attribute("y").as_int() });
			break;
		}
		}

		// Loading special attributes for some units : bunker, jim_raynor, tank...
		switch (spec)
		{
		// For Bunker, we check for entities inside this bunker and we fill its units_inside.
		case(BUNKER) :
		{
			Bunker* b = (Bunker*)reload_entity;
			b->raynor_inside = tmp.attribute("raynor_inside").as_bool();
			if (b->raynor_inside)
			{
				app->gui->raynor_indicator->setLocalPos(b->center.x - 16, b->pos.y);
				app->gui->raynor_indicator->enable_element();
			}
			app->gui->bunker_to_leave.push_back(b);

			for (pugi::xml_node units_bunker = tmp.child("unit_inside_bunker"); units_bunker; units_bunker = units_bunker.next_sibling("unit_inside_bunker"))
			{
				uint id = units_bunker.attribute("id").as_uint();
				iPoint pos = { units_bunker.attribute("pos_x").as_int(), units_bunker.attribute("pos_y").as_int() };
				SPECIALIZATION spec = (SPECIALIZATION)units_bunker.attribute("type").as_int();

				Entity *reload_entity = addEntity(pos, spec, true, id);
				reload_entity->tile_pos = { units_bunker.attribute("tile_pos_x").as_int(), units_bunker.attribute("tile_pos_y").as_int() };
				reload_entity->current_hp = units_bunker.attribute("current_hp").as_float();
				reload_entity->to_delete = units_bunker.attribute("to_delete").as_bool();

				switch (reload_entity->specialization)
				{
				case(MARINE):
					((Marine*)reload_entity)->inside_bunker = true; 
					break;
				case(FIREBAT):
					((Firebat*)reload_entity)->inside_bunker = true;
					break;
				case(JIM_RAYNOR):
					((JimRaynor*)reload_entity)->inside_bunker = true;
					break;
				}
				
				reload_entity->coll->disable();
				b->units_inside.insert(pair<uint, Unit*>(id, (Unit*)reload_entity));
			}
			break;
		}
		case(JIM_RAYNOR) :
		{
			JimRaynor* j = (JimRaynor*)reload_entity;
			app->game_manager->jim_raynor = j;
			j->bomb_activated = tmp.attribute("bomb_activated").as_bool();
			j->bomb_taken = tmp.attribute("bomb_taken").as_bool();
			app->game_manager->jim_raynor = j;
			break;
		}
		case(TANK) :
		{
			Tank* t = (Tank*)reload_entity;
			t->siege_mode = tmp.attribute("siege_mode").as_bool();
			if (t->siege_mode)
			{
				t->siege_mode_on.current_frame = t->siege_mode_on.frames.size() - 1;
				t->current_animation = &t->siege_mode_on;
			}
				
			break;
		}
		}
	}

	// Now, we recover the entities that forms the waves.
	app->game_manager->current_wave_entities.clear();
	for (uint i = 0; i < app->game_manager->list_ids_enemies.size(); ++i)
	{
		uint id = app->game_manager->list_ids_enemies[i];
		Entity *e = active_entities.at(app->game_manager->list_ids_enemies[i]);
		app->game_manager->current_wave_entities.insert(pair<uint, Entity*>(id, e));
	}		

	// And the selection on the saved_game
	// Once we have all the loaded entities, we redo the selection
	selection.clear();
	istringstream in(string(node.child("list_ids_selection").attribute("ids").as_string()));
	uint id;
	while (in >> id)
		selection.insert(pair<uint, Entity*>(id, active_entities.at(id)));

	next_ID = node.child("active_entities").attribute("last_id").as_uint();

	return true;
}

bool EntityManager::save(pugi::xml_node &node) const
{
	// Saving IDs of selection
	// Here I save the ids of the zergs that conforms the current wave.
	pugi::xml_node list_ids_selection = node.append_child("list_ids_selection");
	stringstream ss;
	for (map<uint, Entity*>::const_iterator it = selection.begin(); it != selection.end(); ++it)
		ss << it->second->id << ' ';
	string ids;
	getline(ss, ids);
	list_ids_selection.append_attribute("ids") = ids.data();

	// Saving all the information about all entities on the game
	pugi::xml_node entities = node.append_child("active_entities");
	entities.append_attribute("last_id") = next_ID;

	map<uint, Entity*>::const_iterator it = active_entities.begin();
	for (; it != active_entities.end(); ++it)
	{
		pugi::xml_node e = entities.append_child("entity");

		e.append_attribute("id") = it->second->id;
		e.append_attribute("pos_x") = it->second->center.x;
		e.append_attribute("pos_y") = it->second->center.y;
		e.append_attribute("tile_pos_x") = it->second->tile_pos.x;
		e.append_attribute("tile_pos_y") = it->second->tile_pos.y;
		e.append_attribute("type") = it->second->specialization;
		e.append_attribute("current_hp") = it->second->current_hp;
		e.append_attribute("to_delete") = it->second->to_delete;
		e.append_attribute("state") = it->second->state;
		e.append_attribute("damage") = it->second->damage;
		e.append_attribute("damage_multiplier") = it->second->damage_multiplier;
		e.append_attribute("speed_multiplier") = it->second->speed_multiplier;

		if (it->second->type == UNIT)
		{
			Unit *u = (Unit*)it->second;
			e.append_attribute("flying") = u->flying;
			e.append_attribute("target_pos_x") = u->target_pos.x;
			e.append_attribute("target_pos_y") = u->target_pos.y;
		}

		// Saving the necessary variables according to the tropezientos mil states that exists...:(
		switch (it->second->state)
		{
		case(MOVE) :
		case(MOVE_ALERT) :
		{
			Unit* u = (Unit*)it->second;
			e.append_attribute("has_target") = u->has_target;
			e.append_attribute("grouped") = u->grouped;
			e.append_attribute("group_speed") = u->group_speed;
			for (uint i = 0; i < u->path.size(); ++i)
			{
				pugi::xml_node pos = e.append_child("position_to_move");
				pos.append_attribute("x") = u->path[i].x;
				pos.append_attribute("y") = u->path[i].y;
			}
			break;
		}
		}

		// Special treatment for some entities: bunker, jim_raynor and tank
		switch (it->second->specialization)
		{
		case(BUNKER) :
		{
			Bunker* b = (Bunker*)it->second;
			e.append_attribute("raynor_inside") = b->raynor_inside;
			for (map<uint, Unit*>::iterator inside = b->units_inside.begin(); inside != b->units_inside.end(); ++inside)
			{
				pugi::xml_node node_unit = e.append_child("unit_inside_bunker");
				node_unit.append_attribute("id") = inside->second->id;
				node_unit.append_attribute("pos_x") = inside->second->pos.x;
				node_unit.append_attribute("pos_y") = inside->second->pos.y;
				node_unit.append_attribute("tile_pos_x") = inside->second->tile_pos.x;
				node_unit.append_attribute("tile_pos_y") = inside->second->tile_pos.y;
				node_unit.append_attribute("type") = inside->second->specialization;
				node_unit.append_attribute("current_hp") = inside->second->current_hp;
				node_unit.append_attribute("to_delete") = inside->second->to_delete;
			}
			break;
		}
		case(JIM_RAYNOR) :
		{
			JimRaynor* j = (JimRaynor*)it->second;
			e.append_attribute("bomb_taken") = j->bomb_taken;
			e.append_attribute("bomb_activated") = j->bomb_activated;			
			break;
		}
		case(TANK) :
		{
			Tank* t = (Tank*)it->second;
			e.append_attribute("siege_mode") = t->siege_mode;
			break;
		}		 
		}
	}		

	return true;
}