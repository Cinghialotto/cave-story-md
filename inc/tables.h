/*
 * This is a database of most of the game's content. Wherever possible, the indexes match
 * the IDs used by the original game.
 * Any time you see a const pointer, that is pointing to ROM data
 */

// Information about each stage, indexed by stageID
#define STAGE_COUNT 95
typedef struct {
	const uint8_t *PXM; // PXM is the layout, each byte is an index of the current tileset
	const uint8_t *PXE; // PXE is the entity list
	const uint8_t *TSC; // TSC is the script
	// Which palette to load for PAL3. Most use PAL_Regu but some differ
	const Palette *npcPalette;
	uint8_t tileset; // Which tileset in tileset_info to use
	uint8_t background; // Which background in background_info to use
	uint8_t name[24]; // The name of the map, as displayed to the player
} stage_info_def;
extern const stage_info_def stage_info[STAGE_COUNT];

// Information about each tileset, indexed by stageTileset
#define TILESET_COUNT 29
typedef struct {
	const TileSet *tileset; // The graphical tile data to load
	const Palette *palette; // The palette, which will be loaded to PAL2
	const uint8_t *PXA; // Tile options, how objects interact with different tiles
} tileset_info_def;

// Information about each background, indexed by stageBackground
// The IDs deviate from the original game. I do not know the "correct" order
#define BACKGROUND_COUNT 17
typedef struct {
	const TileSet *tileset; // Graphical tile data to load
	// Backgrounds do not use their own palette and instead "share" with any other of the
	// 4 already loaded. This value specifies which, like PAL0, PAL1, etc
	uint16_t palette;
	// The "type" is which behavior/algorithm to use when loading/scrolling the background
	// 0 - Draw a plain tiled image into PLAN_B
	// 1 - Moon/Fog - Draw from a specified pattern into PLAN_B and scroll the clouds
	// 2 - Clear PLAN_B to black, no background
	// 3 - Scroll tiled image automatically (Ironhead boss)
	uint8_t type;
	uint8_t width, height; // Size of tiled image for type 0
} background_info_def;

// Information about each music track for song_play()
#define SONG_COUNT 42
typedef struct {
	const uint8_t *song; // Location of XGM to be loaded
	char name[24]; // Name of the track (sound test)
} song_info_def;

// Information about each sound effect for sound_play()
#define SOUND_COUNT 118
typedef struct {
	const uint8_t *sound; // Location of PCM data to playback
	uint32_t length; // Number of frames (sound frames, not screen frames)
} sound_info_def;

// Information about each character face image
#define FACE_COUNT 30
typedef struct {
	const TileSet *tiles; // Graphical tile data
	// Like backgrounds, faces share palettes. This is which of the 4 currently loaded to use
	uint16_t palette;
} face_info_def;

// Information about each NPC type, indexes are the same as npc.tbl
typedef struct {
	// SpriteDef containing tile data, ignored if a sheet is used
	const SpriteDefinition *sprite;
	uint8_t sheet; // Sheet ID or NOSHEET
	uint16_t palette; // Any of the 4 loaded palettes to use for the sprite
	uint8_t sprite_count; // Size of sprite[0], must be known beforehand
	EntityMethod onSpawn, onFrame, onDeath; // AI
} npc_info_def;

// Information about each weapon, indexes match <AM+ and ArmsImage
typedef struct {
	const SpriteDefinition *sprite; // Sprite to display beneath player
	uint16_t palette;
	uint8_t experience[3]; // Amount of exp required to level up the weapon
} weapon_info_def;

typedef struct {
	uint8_t	cmd; // Determines which member of the union is used
	union {
		struct { // Add a string of text ([)
			char string[16];
		} text;
		struct  { // Show an icon from casts.pbm (])
			uint8_t id;
			uint8_t pal;
		} icon;
		struct { // Delay until next instruction (-)
			uint16_t ticks;
		} wait;
		struct { // I think the sets the text X position? (+)
			uint16_t x;
		} pos;
		struct { // Change the music (!)
			uint8_t id;
		} song;
		struct { // Swap one of the palettes (no CS equivalent)
			uint8_t id;
			const uint16_t *data;
		} palette;
	};
} credits_info_def;

extern const tileset_info_def tileset_info[];

extern const background_info_def background_info[];

extern const song_info_def song_info[];

extern const sound_info_def sound_info[];

extern const npc_info_def npc_info[];

extern const weapon_info_def weapon_info[];

extern const face_info_def face_info[];
