#include "common.h"

#include "resources.h"
#include "vdp.h"
#include "vdp_pal.h"

#include "tables.h"

// Determines which member of the union in credits_info is used
enum CreditCmd {
	TEXT,		// Add a string of text ([)
	ICON,		// Show an icon from casts.pbm (])
	WAIT,		// Delay until next instruction (-)
	MOVE,		// Sets the text X position (+)
	SONG,		// Change the music (!)
	SONG_FADE,	// Fade out music (~)
	FLAG_JUMP,	// Jump to label if a flag is set (f)
	JUMP,		// Jump to label (j)
	LABEL,		// Label that can be jumped to (l)
	PALETTE,	// Swap one of the palettes (no CS equivalent)
	END,		// Stop scrolling and pause indefinitely (/)
};

#define C_TEXT(str)			{ .cmd = TEXT, .text = { str } }
#define C_ICON(id, pal)		{ .cmd = ICON, .icon = { id, pal } }
#define C_WAIT(ticks)		{ .cmd = WAIT, .wait = { TIME(ticks) } }
#define C_SONG(id)			{ .cmd = SONG, .song = { id } }
#define C_SONG_FADE()		{ .cmd = SONG_FADE }
#define C_FLAG_JUMP(f, l)	{ .cmd = FLAG_JUMP, .fjump = { f, l } }
#define C_JUMP(l)			{ .cmd = JUMP, .jump = { l } }
#define C_LABEL(n)			{ .cmd = LABEL, .label = { n } }
#define C_MOVE(x)			{ .cmd = MOVE, .move = { x } }
#define C_END()				{ .cmd = END }

enum Casts {
	SUE,		KAZUMA,		MOMORIN,	BOOSTER,	ITOH,		DOCTOR,
	CURLY,		COLON,		JENKA,		PUPPY,		MISERY,		BALROG,
	TOROKO,		KING,		JACK,		MAHIN,		SANTA,		CHACO,
	SANDAIME,	KANPACHI,	SHOVELBR,	MEGANE,		CHIE,		ZETT,
	MALCO,		ROBOT,		CHABA,		GERO,		HASUMI,		POOHBLK,
	LITTLES,	CTHULHU,	BA2,		SKYDRAGN,	GUNSMITH,	MUSIC,
	DEBUGCAT,	CHINFISH,	CRITTER,	BAT,		THEDOOR,	PIGNON,
	GPIGNON,	GKEEPER,	MAPIGNON,	BEETLE,		BASU,		BEHEMOTH,
	BASIL,		PCRITTER,	MANNAN,		JELLY,		KULALA,		PUCHI,
	FROG,		PRESS,		RAVIL,		BABY,		POLISH,		ARMADILL,
	SANDCROC,	SKELETON,	SKULHEAD,	CROW,		SKULSTEP,	FLOWER,
	GAUDI,		ARMOR,		GAUDIEGG,	MONSTERX,	FIREWHIR,	BUYO,
	BUYOBASE,	FUZZ,		FUZZCORE,	BLOWFISH,	IRONHEAD,	CNTRBOMB,
	DRAGONZ,	SISTERS,	NIGHTSPR,	HOPPY,		ORANGEBL,	DROLL,
	MIDORIN,	GUNFISH,	STUMPY,		NUMAHCHI,	REDDEMON,	IGOR,
	ROLLING,	DELEET,		BUTE,		MESA,		GRNDEVIL,	BALLOS,
	BALFROG,	OMEGA,		CORE,		UNDCORE,	TOROKOF,	DOCTORF,
	BALLOS2,	HVPRESS,	NAO,		KUROI,		OKAMI,		SWORD,
	GIDEON,		THANKS,
};

const credits_info_def credits_info[] = {
	C_MOVE(192),
	C_SONG(33),
	C_TEXT("  = CAST =  "),
	C_WAIT(320),
	C_TEXT("- Surface Dwellers -"),						C_WAIT(64),
	C_TEXT("Plucky, looks up"), 						C_WAIT(16),
	C_TEXT(" to her brother"),							C_WAIT(16),
	C_TEXT("  Sue Sakamoto"),		C_ICON(SUE,		3), C_WAIT(64),
	C_TEXT("Her occasionally"), 						C_WAIT(16),
	C_TEXT(" reliable brother"),						C_WAIT(16),
	C_TEXT("  Kazuma Sakamoto"),	C_ICON(KAZUMA,	3), C_WAIT(64),
	C_TEXT("Sue's Mother"), 							C_WAIT(32),
	C_TEXT("  Momorin Sakamoto"),	C_ICON(MOMORIN,	3), C_WAIT(64),
	C_TEXT("Sue's Grandfather"), 						C_WAIT(16),
	C_TEXT(" figure"), 									C_WAIT(16),
	C_TEXT("  Booster"),			C_ICON(BOOSTER,	3), C_WAIT(64),
	C_TEXT("The Mimiga-shaped"), 						C_WAIT(16),
	C_TEXT(" coward"), 									C_WAIT(16),
	C_TEXT("  Itoh"),				C_ICON(ITOH,	3), C_WAIT(64),
	C_WAIT(64),
	C_TEXT("- Mimiga Village -"),						C_WAIT(64),
	C_TEXT("Sweet and brave"), 							C_WAIT(32),
	C_TEXT("  Toroko"),				C_ICON(TOROKO,	3), C_WAIT(64),
	C_TEXT("The village leader"), 						C_WAIT(32),
	C_TEXT("  King"),				C_ICON(KING,	3), C_WAIT(64),
	C_TEXT("The Number-Two"), 							C_WAIT(32),
	C_TEXT("  Jack"),				C_ICON(JACK,	3), C_WAIT(64),
	C_TEXT("Never stops eating"), 						C_WAIT(32),
	C_TEXT("  Mahin"),				C_ICON(MAHIN,	3), C_WAIT(64),
	C_TEXT("Loves to fish"), 							C_WAIT(32),
	C_TEXT("  Kanpachi"),			C_ICON(KANPACHI,3), C_WAIT(64),
	C_TEXT("Mans the farm"), 							C_WAIT(32),
	C_TEXT("  Sandaime"),			C_ICON(SANDAIME,3), C_WAIT(64),
	C_WAIT(64),
	C_TEXT("- Grasstown -"),							C_WAIT(64),
	C_TEXT("Lives alone"), 								C_WAIT(32),
	C_TEXT("  Santa"),				C_ICON(SANTA,	3), C_WAIT(64),
	C_TEXT("The colorful Mimiga"), 						C_WAIT(32),
	C_TEXT("  Chaco"),				C_ICON(CHACO,	3), C_WAIT(64),
	C_TEXT("Power Room watcher"), 						C_WAIT(32),
	C_TEXT("  Malco"),				C_ICON(MALCO,	3), C_WAIT(64),
	C_WAIT(64),
	C_TEXT("- Sand Zone -"),							C_WAIT(64),
	C_TEXT("Ally of the Mimigas"), 						C_WAIT(32),
	C_TEXT("  Curly"),				C_ICON(CURLY,	3), C_WAIT(64),
	C_TEXT("Raised by Curly"), 							C_WAIT(32),
	C_TEXT("  Colon"),				C_ICON(COLON,	3), C_WAIT(64),
	C_TEXT("Original keeper of"), 						C_WAIT(16),
	C_TEXT(" the Sand Zone"), 							C_WAIT(16),
	C_TEXT("  Jenka"),				C_ICON(JENKA,	3), C_WAIT(64),
	C_WAIT(32),
	C_TEXT("Jenka's Puppies"), 							C_WAIT(32),
	C_TEXT("  Hajime"),				C_ICON(PUPPY,	3), C_WAIT(32),
	C_TEXT("  Shinobu"),			C_ICON(PUPPY,	3), C_WAIT(32),
	C_TEXT("  Kakeru"),				C_ICON(PUPPY,	3), C_WAIT(32),
	C_TEXT("  Mick"),				C_ICON(PUPPY,	3), C_WAIT(32),
	C_TEXT("  Nene"),				C_ICON(PUPPY,	3), C_WAIT(64),
	C_WAIT(64),
	C_TEXT("- Labyrinth -"),							C_WAIT(64),
	C_TEXT("The labyrinth's"), 							C_WAIT(16),
	C_TEXT(" weapons dealer"), 							C_WAIT(16),
	C_TEXT("  Chaba"),				C_ICON(CHABA,	3), C_WAIT(64),
	C_TEXT("The labyrinth's"), 							C_WAIT(16),
	C_TEXT(" resident physician"), 						C_WAIT(16),
	C_TEXT("  Dr. Gero"),			C_ICON(GERO,	3), C_WAIT(64),
	C_TEXT("I am a nurse!"), 							C_WAIT(32),
	C_TEXT("  Nurse Hasumi"),		C_ICON(HASUMI,	3), C_WAIT(64),
	C_WAIT(64),
	C_TEXT("- Plantation -"),							C_WAIT(64),
	C_TEXT("The girl Mimiga"), 							C_WAIT(32),
	C_TEXT("  Chie"),				C_ICON(CHIE,	3), C_WAIT(64),
	C_TEXT("The grandpa Mimiga"), 						C_WAIT(32),
	C_TEXT("  Zett"),				C_ICON(ZETT,	3), C_WAIT(64),
	C_TEXT("Sprinkler Manager"), 						C_WAIT(32),
	C_TEXT("  Megane"),				C_ICON(MEGANE,	3), C_WAIT(64),
	C_TEXT("Believes in the"), 							C_WAIT(16),
	C_TEXT(" Doctor"), 									C_WAIT(16),
	C_TEXT("  The Shovel Brigade"),	C_ICON(SHOVELBR,3), C_WAIT(64),
	C_WAIT(64),
	// Normal Ending villains/others
	C_FLAG_JUMP(2000, 90),
	C_TEXT("- Villains -"),								C_WAIT(64),
	C_TEXT("The power-snatching"), 						C_WAIT(16),
	C_TEXT(" betrayer"), 								C_WAIT(16),
	C_TEXT("  The Doctor"),			C_ICON(DOCTOR,	3), C_WAIT(64),
	C_TEXT("The Doctor's servant"), 					C_WAIT(32),
	C_TEXT("  Misery"),				C_ICON(MISERY,	3), C_WAIT(64),
	C_TEXT("Misery's punching bag"), 					C_WAIT(32),
	C_TEXT("  Balrog"),				C_ICON(BALROG,	3), C_WAIT(64),
	C_WAIT(64),
	C_TEXT("- Others -"),								C_WAIT(64),
	C_TEXT("The Hermit Gunsmith"), 						C_WAIT(32),
	C_TEXT("  Tetsuzou Kamadani"),	C_ICON(GUNSMITH,3), C_WAIT(64),
	C_TEXT("The mysterious"), 							C_WAIT(16),
	C_TEXT(" mushroom"), 								C_WAIT(16),
	C_TEXT("  Ma Pignon"),			C_ICON(MAPIGNON,3), C_WAIT(64),
	C_TEXT("The family living in"), 					C_WAIT(16),
	C_TEXT(" the outer wall"), 							C_WAIT(16),
	C_TEXT("  The Littles"),		C_ICON(LITTLES,	3), C_WAIT(64),
	C_TEXT("The mysterious"), 							C_WAIT(16),
	C_TEXT(" native inhabitants"), 						C_WAIT(16),
	C_TEXT("  Cthulhu"),			C_ICON(CTHULHU,	3), C_WAIT(64),
	C_TEXT("The statue carver"), 						C_WAIT(32),
	C_TEXT("  Numahachi"),			C_ICON(NUMAHCHI,3), C_WAIT(64),
	C_JUMP(100),
	// Best Ending villains/others
	C_LABEL(90),
	C_TEXT("- Villains -"),								C_WAIT(64),
	C_TEXT("Entranced by the crown"), 					C_WAIT(32),
	C_TEXT("  The Doctor"),			C_ICON(DOCTOR,	3), C_WAIT(64),
	C_TEXT("Cursed by the crown"), 						C_WAIT(32),
	C_TEXT("  Misery"),				C_ICON(MISERY,	3), C_WAIT(64),
	C_TEXT("Ultimately a friend"), 						C_WAIT(32),
	C_TEXT("  Balrog"),				C_ICON(BALROG,	3), C_WAIT(64),
	C_WAIT(64),
	C_TEXT("- Others -"),								C_WAIT(64),
	C_TEXT("The Hermit Gunsmith"), 						C_WAIT(32),
	C_TEXT("  Tetsuzou Kamadani"),	C_ICON(GUNSMITH,3), C_WAIT(64),
	C_TEXT("The memory-restoring"), 					C_WAIT(16),
	C_TEXT(" mushroom"), 								C_WAIT(16),
	C_TEXT("  Ma Pignon"),			C_ICON(MAPIGNON,3), C_WAIT(64),
	C_TEXT("The family living in"), 					C_WAIT(16),
	C_TEXT(" the outer wall"), 							C_WAIT(16),
	C_TEXT("  The Littles"),		C_ICON(LITTLES,	3), C_WAIT(64),
	C_TEXT("The mysterious"), 							C_WAIT(16),
	C_TEXT(" native inhabitants"), 						C_WAIT(16),
	C_TEXT("  Cthulhu"),			C_ICON(CTHULHU,	3), C_WAIT(64),
	C_TEXT("The statue carver"), 						C_WAIT(32),
	C_TEXT("  Numahachi"),			C_ICON(NUMAHCHI,3), C_WAIT(64),
	C_WAIT(64),
	C_TEXT("- Bloody Holy Land -"),						C_WAIT(64),
	C_TEXT("Jenka's sorrowful"), 						C_WAIT(16),
	C_TEXT(" younger brother"), 						C_WAIT(16),
	C_TEXT("  Ballos"),				C_ICON(BALLOS,	3), C_WAIT(64),
	C_LABEL(100),
	// Transition to second part which has short silent cutscenes in various stages
	C_WAIT(100),
	C_SONG_FADE(),
	C_WAIT(250),
	C_SONG(1),
	C_MOVE(64),
	
	C_TEXT(" = Monsters = "),											C_WAIT(32),
	C_WAIT(64),
	C_TEXT(" Leaps and hops: Critter"),				C_ICON(CRITTER,	3), C_WAIT(48),
	C_TEXT(" Every cave needs a: Bat"),				C_ICON(BAT,		3), C_WAIT(48),
	C_TEXT(" Possessed by something: The Door"),	C_ICON(THEDOOR,	3), C_WAIT(48),
	C_TEXT(" Charges when angered: Behemoth"),		C_ICON(BEHEMOTH,3), C_WAIT(48),
	C_TEXT(" White mushroom: Pignon"),				C_ICON(PIGNON,	3), C_WAIT(48),
	C_TEXT(" Worth eating: Giant Pignon"),			C_ICON(GPIGNON,	3), C_WAIT(48),
	C_TEXT(" There's only one: Chinfish"),			C_ICON(CHINFISH,3), C_WAIT(48),
	C_TEXT(" Slices you up: Gravekeeper"),			C_ICON(GKEEPER,	3), C_WAIT(48),
	C_TEXT(" Floor-hugging flash: Basil"),			C_ICON(BASIL,	3), C_WAIT(48),
	C_TEXT(" A model insect: Beetle"),				C_ICON(BEETLE,	3), C_WAIT(48),
	C_TEXT(" The big flyer: Basu"),					C_ICON(BASU,	3), C_WAIT(48),
	C_TEXT(" The crusher: Power Critter"),			C_ICON(PCRITTER,3), C_WAIT(48),
	C_TEXT(" White mold ghost: Mannan"),			C_ICON(MANNAN,	3), C_WAIT(48),
	C_TEXT(" Tiny frog: Puchi"),					C_ICON(PUCHI,	3), C_WAIT(48),
	C_TEXT(" Big croaker: Frog"),					C_ICON(FROG,	3), C_WAIT(48),
	C_TEXT(" Floats about: Jelly"),					C_ICON(JELLY,	3), C_WAIT(48),
	C_TEXT(" Queen Jelly: Kulala"),					C_ICON(KULALA,	3), C_WAIT(48),
	C_TEXT(" Violent Mimiga: Rabil"),				C_ICON(RAVIL,	3), C_WAIT(48),
	C_TEXT(" Instant deathtrap: Press"),			C_ICON(PRESS,	3), C_WAIT(48),
	C_TEXT(" Sudden chomper: Sandcroc"),			C_ICON(SANDCROC,3), C_WAIT(48),
	C_TEXT(" Wandering skull: Skullhead"),			C_ICON(SKULHEAD,3), C_WAIT(48),
	C_TEXT(" Sand runner: Skullstep"),				C_ICON(SKULSTEP,3), C_WAIT(48),
	C_TEXT(" White for: Skeleton"),					C_ICON(SKELETON,3), C_WAIT(48),
	C_TEXT(" Sand Zone Hunter: Crow"),				C_ICON(CROW,	3), C_WAIT(48),
	C_TEXT(" Tough missile: Armadillo"),			C_ICON(ARMADILL,3), C_WAIT(48),
	C_TEXT(" From one, many: Polish"),				C_ICON(POLISH,	3), C_WAIT(48),
	C_TEXT(" Scattering everywhere: Baby"),			C_ICON(BABY,	3), C_WAIT(48),
	C_TEXT(" From Frenzied Toroko: Flower"),		C_ICON(FLOWER,	3), C_WAIT(48),
	C_TEXT(" Labyrinth dweller: Gaudi"),			C_ICON(GAUDI,	3), C_WAIT(48),
	C_TEXT(" Labyrinth warrior: Armor"),			C_ICON(ARMOR,	3), C_WAIT(48),
	C_TEXT(" Labyrinth baby: Gaudi Egg"),			C_ICON(GAUDIEGG,3), C_WAIT(48),
	C_TEXT(" Fiery fan: Fire Whirr"),				C_ICON(FIREWHIR,3), C_WAIT(48),
	C_TEXT(" Unknown organism: Buyobuyo Base"),		C_ICON(BUYOBASE,3), C_WAIT(48),
	C_TEXT(" And: Buyobuyo"),						C_ICON(BUYO,	3), C_WAIT(48),
	C_TEXT(" Gaudi spirit: Fuzz"),					C_ICON(FUZZ,	3), C_WAIT(48),
	C_TEXT(" Spirit clump: Fuzz Core"),				C_ICON(FUZZCORE,3), C_WAIT(48),
	C_TEXT(" An old friend: Porcupine Fish"),		C_ICON(BLOWFISH,3), C_WAIT(48),
	C_TEXT(" Hatching failure: Dragon Zombie"),		C_ICON(DRAGONZ,	3), C_WAIT(48),
	C_TEXT(" Timed explosive: Counter Bomb"),		C_ICON(CNTRBOMB,3), C_WAIT(48),
	C_TEXT(" Outer wall flutterer: Night Spirit"),	C_ICON(NIGHTSPR,3), C_WAIT(48),
	C_TEXT(" Jumps: Hoppy"),						C_ICON(HOPPY,	3), C_WAIT(48),
	C_TEXT(" Photosynthetic warrior: Midorin"),		C_ICON(MIDORIN,	3), C_WAIT(48),
	C_TEXT(" Born of earth: Droll"),				C_ICON(DROLL,	3), C_WAIT(48),
	C_TEXT(" Stepping stone: Gunfish"),				C_ICON(GUNFISH,	3), C_WAIT(48),
	C_TEXT(" Mother bat: Orangebell"),				C_ICON(ORANGEBL,3), C_WAIT(48),
	C_TEXT(" Plantation dragonfly: Stumpy"),		C_ICON(STUMPY,	3), C_WAIT(48),
	// Best end only
	C_FLAG_JUMP(2000, 190), C_JUMP(200), C_LABEL(190),
	C_WAIT(2),
	C_TEXT(" Hell's messenger: Bute"),				C_ICON(BUTE,	3), C_WAIT(48),
	C_TEXT(" Hell's messenger: Mesa"),				C_ICON(MESA,	3), C_WAIT(48),
	C_TEXT(" Innocent demon: Green Devil"),			C_ICON(GRNDEVIL,3), C_WAIT(48),
	C_TEXT(" Rolls along the walls: Rolling"),		C_ICON(ROLLING,	3), C_WAIT(48),
	C_TEXT(" Blocks passages: Deleet"),				C_ICON(DELEET,	3), C_WAIT(48),
	C_LABEL(200),
	
	C_WAIT(100),
	C_TEXT("= Bosses ="),								C_WAIT(32),
	C_WAIT(64),
	C_TEXT("Frenzied Mimiga that"), 					C_WAIT(16),
	C_TEXT(" kidnapped Sue"),							C_WAIT(16),
	C_TEXT("  Igor"),				C_ICON(IGOR,	3), C_WAIT(64),
	C_TEXT("Misery transformed"), 						C_WAIT(16),
	C_TEXT(" Balrog into"),								C_WAIT(16),
	C_TEXT("  Balfrog"),			C_ICON(BALFROG,	3), C_WAIT(64),
	C_TEXT("Machine-monster that"), 					C_WAIT(16),
	C_TEXT(" lurks in the sand"),						C_WAIT(16),
	C_TEXT("  Omega"),				C_ICON(OMEGA,	3), C_WAIT(64),
	C_TEXT("Dosed with red flowers"), 					C_WAIT(32),
	C_TEXT("  Toroko +"),			C_ICON(TOROKOF,	3), C_WAIT(64),
	C_TEXT("Ghost that invaded"), 						C_WAIT(16),
	C_TEXT(" the clinic"),								C_WAIT(16),
	C_TEXT("  Pooh Black"),			C_ICON(POOHBLK,	3), C_WAIT(64),
	C_TEXT("Big boss of the"), 							C_WAIT(16),
	C_TEXT(" labyrinth"),								C_WAIT(16),
	C_TEXT("  Monster X"),			C_ICON(MONSTERX,3), C_WAIT(64),
	C_TEXT("Heart of the island"), 						C_WAIT(32),
	C_TEXT("  Core"),				C_ICON(CORE,	3), C_WAIT(64),
	C_TEXT("Big fish in the"), 							C_WAIT(16),
	C_TEXT(" island's artery"),							C_WAIT(16),
	C_TEXT("  Ironhead"),			C_ICON(IRONHEAD,3), C_WAIT(64),
	C_TEXT("Twin dragon surprise"), 					C_WAIT(16),
	C_TEXT(" attackers"),								C_WAIT(16),
	C_TEXT("  Sisters"),			C_ICON(SISTERS,	3), C_WAIT(64),
	C_TEXT("True heroes meet the"), 					C_WAIT(32),
	C_TEXT("  Red Demon"),			C_ICON(REDDEMON,3), C_WAIT(64),
	C_TEXT("The red crystal"), 							C_WAIT(16),
	C_TEXT(" runs wild"),								C_WAIT(16),
	C_TEXT("  Muscle Doctor"),		C_ICON(DOCTORF,	3), C_WAIT(64),
	C_TEXT("The Doctor and the"), 						C_WAIT(16),
	C_TEXT(" island's heart"),							C_WAIT(16),
	C_TEXT("  Undead Core"),		C_ICON(UNDCORE,	3), C_WAIT(64),
	// Best end only
	C_FLAG_JUMP(2000, 290), C_JUMP(300), C_LABEL(290),
	C_WAIT(2),
	C_TEXT("The swollen mech"), 						C_WAIT(32),
	C_TEXT("  Heavy Press"),		C_ICON(HVPRESS,	3), C_WAIT(64),
	C_TEXT("Hate and madness"), 						C_WAIT(16),
	C_TEXT(" given flesh"),								C_WAIT(16),
	C_TEXT("  Ballos"),				C_ICON(BALLOS2,	3), C_WAIT(64),
	C_LABEL(300),
	
	C_WAIT(80),
	C_MOVE(40),
	C_TEXT(" = BGM = "),								C_WAIT(32),
	C_WAIT(32),
	C_TEXT("Access"),				C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Gestation"),			C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Mimiga Village"),		C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Plant"),				C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Balrog's Theme"),		C_ICON(MUSIC,	3), C_WAIT(32),
	C_WAIT(32),
	C_TEXT("Gravity"),				C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Cemetery"),				C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Safety"),				C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Mischievous Robot"),	C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Pulse"),				C_ICON(MUSIC,	3), C_WAIT(32),
	C_WAIT(32),
	C_TEXT("On to Grasstown"),		C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Eyes of Flame"),		C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Meltdown 2"),			C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Tyrant"),				C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Run!"),					C_ICON(MUSIC,	3), C_WAIT(32),
	C_WAIT(32),
	C_TEXT("Jenka 1"),				C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Jenka 2"),				C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Labyrinth Fight"),		C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Geothermal"),			C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Oppression"),			C_ICON(MUSIC,	3), C_WAIT(32),
	C_WAIT(32),
	C_TEXT("Living Waterway"),		C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Quiet"),				C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Scorching Back"),		C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Moonsong"),				C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Hero's End"),			C_ICON(MUSIC,	3), C_WAIT(32),
	C_WAIT(32),
	C_TEXT("Cave Story"),			C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Last Cave"),			C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Balcony"),				C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Charge"),				C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Zombie"),				C_ICON(MUSIC,	3), C_WAIT(32),
	C_WAIT(32),
	C_TEXT("Last Battle"),			C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Break Down"),			C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Running Hell"),			C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("Seal Chamber"),			C_ICON(MUSIC,	3), C_WAIT(32),
	C_TEXT("The Way Back Home"),	C_ICON(MUSIC,	3), C_WAIT(32),
	
	C_WAIT(100),
	C_TEXT("= Special Thanks ="),						C_WAIT(32),
	C_MOVE(48),
	C_WAIT(32),
	C_TEXT("Torai"),				C_ICON(DEBUGCAT,3), C_WAIT(40),
	C_TEXT("Naoku"),				C_ICON(SWORD,	3), C_WAIT(40),
	C_TEXT("Kuroihito"),			C_ICON(KUROI,	3), C_WAIT(40),
	C_TEXT("BA2"),					C_ICON(BA2,		3), C_WAIT(40),
	C_TEXT("Okami"),				C_ICON(OKAMI,	3), C_WAIT(40),
	C_TEXT("Nao"),					C_ICON(NAO,		3), C_WAIT(40),
	C_TEXT("Shih Tzu"),				C_ICON(PUPPY,	3), C_WAIT(40),
	C_TEXT("Gideon Zhi"),			C_ICON(GIDEON,	3), C_WAIT(40),
	C_TEXT("And many others"),							C_WAIT(40),
	// TODO: Probably add some MD specific credits before the end
	C_WAIT(64),
	C_TEXT("Thank you very much."), C_ICON(THANKS,	3), C_WAIT(32),
	C_WAIT(240),
	C_MOVE(120),
	C_TEXT("Cave Story ~ The End"),
	C_WAIT(280),
	C_END()
};

const illustration_info_def illustration_info[20] = {
	{ NULL,      NULL       },
	{ &TS_Ill01, &PAL_Ill01 },
	{ &TS_Ill02, &PAL_Ill02 },
	{ &TS_Ill03, &PAL_Ill03 },
	{ &TS_Ill04, &PAL_Ill04 },
	{ &TS_Ill05, &PAL_Ill05 },
	{ &TS_Ill06, &PAL_Ill06 },
	{ &TS_Ill07, &PAL_Ill07 },
	{ &TS_Ill08, &PAL_Ill08 },
	{ &TS_Ill09, &PAL_Ill09 },
	{ &TS_Ill10, &PAL_Ill10 },
	{ &TS_Ill11, &PAL_Ill11 },
	{ &TS_Ill12, &PAL_Ill12 },
	{ NULL,      NULL       },
	{ &TS_Ill14, &PAL_Ill14 },
	{ &TS_Ill15, &PAL_Ill15 },
	{ &TS_Ill16, &PAL_Ill16 },
	{ &TS_Ill17, &PAL_Ill17 },
	{ &TS_Ill18, &PAL_Ill18 },
	{ NULL,      NULL       },
};
