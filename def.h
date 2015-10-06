#define WINDOWX 800
#define WINDOWY 600

//Choice 
#define POSX       0 
#define POSY       1
#define ORGX       2 
#define ORGY       3
#define MVvalue    4
#define STATE      5
#define WIDTH      6
#define HEIGHT     7
#define ISMOVE     8
#define CURSORON   9
#define SELECT     10
#define HPvalue    11
#define ACTvalue   12
#define ATTvalue   13
#define EVvalue    14
#define HITvalue   15
#define CTvalue    16
#define LVvalue    17
#define EXPvalue   18
#define TYPE       19
#define SPECIAL    20
#define SKIN       21
#define DEAD	   22
#define VIEWANGLE  23
#define GAMETYPE   24
#define VIEW       25

//SoldierState
#define WAITING         0
#define ACTIVE          1 
#define MOVEING         2
#define FIGHT		    3

//Soldier Action
#define IDLE				0
#define FORWARD				1
#define ATTACK				2
#define SPECIALATTACK		4
#define DODGE				5
#define HURT				6
#define FLASH				7
#define NEARSPECIAL			8
#define MIDDLEATTACK		9  
#define MIDDLESPECIAL		10
#define FARATTACK			11
#define FARSPECIAL			12
#define BEAMSPECIAL			13
#define LOCK				14
#define NOTSHOW             15

//GameState 
#define STARTMENU      0
#define LOADING        1 
#define OURTURN        2
#define ENEMYTURN      3
#define TOOURTURN      4
#define TOENEMYTURN    5
#define GAMEMENU       6
#define FIGHTSTAGE     7
#define LVUP           8
#define SAVE           9
#define LOAD           10
#define MAPTOSTAGE     11
#define FIGHTEND       12
#define SHOWMAP        13
#define GAMEOVER	   14
#define ALLCLEAR       15

//Special  
#define NONE           0
#define SHADOW         1
#define WALL           2
#define SERVICE        3
#define NIGHT          4
#define BATTLECONTROL  5
#define BEAM           6
#define RECOVER        7
#define HITUP          8


//AI
#define NORMAL 0
#define HARD 1

//key 
#define ENTER    0
#define Z        1
#define X        2
#define C        3
#define A        4
#define D        5
#define W        6
#define S        7
#define Q        8
#define E        9
#define UP       10
#define DOWN     11
#define LEFT     12
#define RIGHT    13

//View
#define ORTHO2D 0
#define PERSPECTIVE 1

//SOUND	FUNCTION
#define PLAY		  1
#define STOP		  2
#define	PAUSE		  3

//PLAY LIST
#define MUSIC_START			0
#define MUSIC_OUR			1
#define MUSIC_ENEMY			2
#define MUSIC_BATTLE		3
#define MUSIC_BOSS			4
#define MUSIC_GAMEOVER		5
#define MUSIC_LVUP			6
#define MUSIC_ALLCLEAR		7

//FIGHT STAGE
#define FIGHTING	1
#define ROUND		2

//FIGHT STAGE STATE
#define SEL_MEMBER	0
#define SEL_ATT		1
#define CHK_ACT		2
#define CAL_RESULT	3
#define DRAW_ACTION 4
#define BATTLE_OVER 5

//
#define	FRIENDS	0
#define	ENEMYS	1