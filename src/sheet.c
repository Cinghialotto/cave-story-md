#include "sheet.h"

#include "player.h"
#include "resources.h"
#include "stage.h"
#include "tables.h"

#ifndef KDB_SHEET
#define puts(x) /**/
#define printf(...) /**/
#endif

void sheets_init() {
	// Bullets
	sheets[0] = (Sheet){ SHEET_PSTAR, 2*4, TILE_SHEETINDEX, 2, 2 };
	sheets[1] = (Sheet){ SHEET_MGUN, 2*4, sheets[0].index + sheets[0].size, 2, 2 };
	sheets[2] = (Sheet){ SHEET_FBALL, 3*4, sheets[1].index + sheets[1].size, 2, 2 };
	// Power ups
	sheets[3] = (Sheet){ SHEET_HEART, 4*4, sheets[2].index + sheets[2].size, 2, 2 };
	sheets[4] = (Sheet){ SHEET_MISSILE, 4*4, sheets[3].index + sheets[3].size, 2, 2 };
	sheets[5] = (Sheet){ SHEET_ENERGY, 6*1, sheets[4].index + sheets[4].size, 1, 1 };
	sheets[6] = (Sheet){ SHEET_ENERGYL, 6*4, sheets[5].index + sheets[5].size, 2, 2 };
	// The rest are blank
	for(u8 i = 7; i < 10; i++) sheets[i] = (Sheet) {};
	// Actually load the tiles - assume the VDP is disabled
	Weapon *pstar = player_find_weapon(WEAPON_POLARSTAR);
	Weapon *mgun =  player_find_weapon(WEAPON_MACHINEGUN);
	Weapon *fball = player_find_weapon(WEAPON_FIREBALL);
	sheets_refresh_polarstar(pstar ? pstar->level : 1);
	sheets_refresh_machinegun(mgun ? mgun->level : 1);
	sheets_refresh_fireball(fball ? fball->level : 1);
	// Power ups
	SHEET_LOAD(&SPR_Heart,  4,4, sheets[3].index, 1, 0,0, 0,1, 1,0, 1,1);
	SHEET_LOAD(&SPR_MisslP, 4,4, sheets[4].index, 1, 0,0, 0,1, 1,0, 1,1);
	SHEET_LOAD(&SPR_EnergyS,6,1, sheets[5].index, 1, 0,0, 0,1, 0,2, 0,3, 0,4, 0,5);
	SHEET_LOAD(&SPR_EnergyL,6,4, sheets[6].index, 1, 0,0, 0,1, 0,2, 0,3, 0,4, 0,5);
}

void sheets_refresh_polarstar(u8 level) {
	const SpriteDefinition *def;
	if(level == 1) def = &SPR_PolarB1;
	else if(level == 2) def = &SPR_PolarB2;
	else if(level == 3) def = &SPR_PolarB3;
	else {
		printf("Polar Star has no level %hu", level);
		return;
	}
	VDP_loadTileData(SPR_TILES(def, 0, 0), sheets[0].index, 4, 1);
	VDP_loadTileData(SPR_TILES(def, 1, 0), sheets[0].index + 4, 4, 1);
}

void sheets_refresh_machinegun(u8 level) {
	const SpriteDefinition *def;
	if(level == 1) def = &SPR_MGunB1;
	else if(level == 2) def = &SPR_MGunB2;
	else if(level == 3) def = &SPR_MGunB3;
	else {
		printf("Machine Gun has no level %hu", level);
		return;
	}
	VDP_loadTileData(SPR_TILES(def, 0, 0), sheets[1].index, 4, 1);
	VDP_loadTileData(SPR_TILES(def, 1, 0), sheets[1].index + 4, 4, 1);
}

void sheets_refresh_fireball(u8 level) {
	const SpriteDefinition *def;
	if(level == 1) def = &SPR_FirebB1;
	else if(level == 2) def = &SPR_FirebB1;
	else if(level == 3) def = &SPR_FirebB3;
	else {
		printf("Fireball has no level %hu", level);
		return;
	}
	VDP_loadTileData(SPR_TILES(def, 0, 0), sheets[2].index, 4, 1);
	VDP_loadTileData(SPR_TILES(def, 0, 1), sheets[2].index + 4, 4, 1);
	VDP_loadTileData(SPR_TILES(def, 0, 2), sheets[2].index + 8, 4, 1);
}

void sheets_load_stage(u16 sid) {
	// Reset values
	sheet_num = 7;
	tiloc_index = sheets[6].index + sheets[6].size;
	memset(tilocs, 0, MAX_TILOCS);
	memset(&sheets[7], 0, sizeof(Sheet) * (MAX_SHEETS - 7));
	switch(sid) {
		case 0x0C: // First Cave
		{	// Bat
			SHEET_ADD(SHEET_BAT, 6, 2, 2);
			//sheets[7] = (Sheet){ SHEET_BAT, 6*4, sheets[6].index + sheets[6].size, 2, 2 };
			SHEET_LOAD(&SPR_Bat, 6,4, sheets[7].index, 1, 0,0, 0,1, 0,2, 1,0, 2,0, 3,0);
			// Cave Critter
			SHEET_ADD(SHEET_CRITTER, 3, 2, 2);
			//sheets[8] = (Sheet){ SHEET_CRITTER, 3*4, sheets[7].index + sheets[7].size, 2, 2 };
			SHEET_LOAD(&SPR_CritHB, 3,4, sheets[8].index, 1, 0,0, 1,0, 2,0);
			// Nothing
			//sheets[9] = (Sheet) {};
		}
		break;
		case 0x10: // Graveyard
		{
			// Pignon
			SHEET_ADD(SHEET_PIGNON, 5, 2, 2);
			//sheets[7] = (Sheet){ SHEET_PIGNON, 5*4, sheets[6].index + sheets[6].size, 2, 2 };
			SHEET_LOAD(&SPR_Pignon, 5,4, sheets[7].index, 1, 0,0, 1,0, 1,2, 2,0, 3,0);
		}
		break;
		case 0x02: // Egg Corridor
		{
			// Green Critter (no hover)
			SHEET_ADD(SHEET_CRITTER, 3, 2, 2);
			//sheets[7] = (Sheet){ SHEET_CRITTER, 3*4, sheets[6].index + sheets[6].size, 2, 2 };
			SHEET_LOAD(&SPR_CritHG, 3,4, sheets[7].index, 1, 0,0, 1,0, 2,0);
			// Beetle
			SHEET_ADD(SHEET_BEETLE, 2, 2, 2);
			//sheets[8] = (Sheet) {};
			SHEET_LOAD(&SPR_BtlHG, 2,4, sheets[8].index, 1, 0,0, 1,0);
			// Behemoth
			sheets[9] = (Sheet) {};
		}
		break;
		case 0x06: // Grasstown
		case 0x30: // Waterway
		// Jelly
		sheets[7] = (Sheet){ SHEET_JELLY, 5*4, sheets[6].index + sheets[6].size };
		SHEET_LOAD(&SPR_Jelly, 5,4, sheets[7].index, true, 0,0, 0,1, 0,2, 0,3, 0,4);
		// Bat
		sheets[8] = (Sheet){ SHEET_BAT, 6*4, sheets[7].index + sheets[7].size };
		SHEET_LOAD(&SPR_Bat, 6,4, sheets[8].index, true, 0,0, 0,1, 0,2, 1,0, 2,0, 3,0);
		// Green Critter
		sheets[9] = (Sheet){ SHEET_CRITTER, 6*4, sheets[8].index + sheets[8].size };
		SHEET_LOAD(&SPR_CritHB, 6,4, sheets[9].index, true, 0,0, 1,0, 2,0, 3,0, 3,1, 3,2);
		break;
		case 0x1C: // Gum
		// Balfrog jumping frame
		sheets[7] = (Sheet){ SHEET_BALFROG, 9*11, sheets[6].index + sheets[6].size };
		VDP_loadTileData(SPR_TILES(&SPR_Balfrog2,0,0),sheets[7].index,9*11,true);
		// Nothing
		sheets[8] = (Sheet) {};
		sheets[9] = (Sheet) {};
		break;
		case 0x0A: // Sand Zone
		case 0x25:
		// Crow
		sheets[7] = (Sheet){ SHEET_CROW, 3*16, sheets[6].index + sheets[6].size };
		SHEET_LOAD(&SPR_Crow, 3,16, sheets[7].index, true, 0,0, 0,1, 2,0);
		// Skullhead
		sheets[8] = (Sheet) {};
		// Omega Projectile
		sheets[9] = (Sheet) {};
		break;
		case 0x09: // Labyrinth I
		case 0x26: // Labyrinth H
		case 0x27: // Labyrinth W
		// Purple Critter
		sheets[7] = (Sheet){ SHEET_CRITTER, 6*4, sheets[6].index + sheets[6].size };
		SHEET_LOAD(&SPR_CritterP, 6,4, sheets[7].index, true, 0,0, 1,0, 2,0, 3,0, 3,1, 3,2);
		// Gaudi (Walking / Flying, but not death frames)
		sheets[8] = (Sheet) {};
		// Monster X Wheels
		sheets[9] = (Sheet) {};
		break;
		case 0x29: // Clinic Ruins
		// Pooh Black's Bubbles
		sheets[7] = (Sheet) {};
		// Nothing
		sheets[8] = (Sheet) {};
		sheets[9] = (Sheet) {};
		break;
		case 0x2D: // Labyrinth M
		// Purple Critter
		sheets[7] = (Sheet){ SHEET_CRITTER, 6*4, sheets[6].index + sheets[6].size };
		SHEET_LOAD(&SPR_CritterP, 6,4, sheets[7].index, true, 0,0, 1,0, 2,0, 3,0, 3,1, 3,2);
		// Armored Gaudi (no death frames)
		sheets[8] = (Sheet) {};
		// Fuzz
		sheets[9] = (Sheet) {};
		break;
		case 0x2F: // Core
		// Minicore
		sheets[7] = (Sheet) {};
		// Nothing
		sheets[8] = (Sheet) {};
		sheets[9] = (Sheet) {};
		break;
		case 0x31: // Egg Corridor?
		// Critter
		sheets[7] = (Sheet) {};
		// Nothing
		sheets[8] = (Sheet) {};
		sheets[9] = (Sheet) {};
		break;
		case 0x35: // Outer Wall
		// Projectile
		sheets[7] = (Sheet) {};
		// Nothing
		sheets[8] = (Sheet) {};
		sheets[9] = (Sheet) {};
		break;
		case 0x38: // Plantation
		// Bat
		sheets[7] = (Sheet) {};
		// Sideways Press
		sheets[8] = (Sheet) {};
		// Nothing
		sheets[9] = (Sheet) {};
		break;
		case 0x3F: // Last Cave
		case 0x43:
		// Orange Bat
		sheets[7] = (Sheet) {};
		// Orange Critter
		sheets[8] = (Sheet) {};
		// Proximity Press?
		sheets[9] = (Sheet) {};
		break;
		case 0x40: // Throne Room
		// Bat
		sheets[7] = (Sheet) {};
		// Black Orb
		sheets[8] = (Sheet) {};
		// Lightning
		sheets[9] = (Sheet) {};
		break;
		case 0x41: // King's Table
		// Red Bat
		sheets[7] = (Sheet) {};
		// Projectile
		sheets[8] = (Sheet) {};
		// Cage
		sheets[9] = (Sheet) {};
		break;
		case 0x44: // Black Space
		// Orange Bat
		sheets[7] = (Sheet) {};
		// Orange Critter
		sheets[8] = (Sheet) {};
		// Projectile
		sheets[9] = (Sheet) {};
		break;
		default:
		printf("Stage %hu has no sheet set", sid);
		// Nothing
		sheets[7] = (Sheet) {};
		sheets[8] = (Sheet) {};
		sheets[9] = (Sheet) {};
	}
}