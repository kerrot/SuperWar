#include<gl/glut.h>
#include<gl/gl.h>
#include"glm.h"
#include<stack>
#define CURSORSPEED 0.1
using namespace std;


typedef struct _Rect{
	int HIT_rate;
	int EV_rate; 
	int who_on_it; 
	char type; 
}Rect;

typedef struct{
	int org; //­E»¦h¤Ö 
	int now; //²{¦b¦h¤Ö 
}Value;

typedef struct{
	int x;
	int y;
}IntPoint;

typedef struct{
	float x;
	float y;
}FloatPoint;

class Cursor;

class Map{
	private:
		GLuint mappic;
		Rect ** rect;
		int width;
		int height;
	public:
		Map();
		void init();//  «Øºc¦a¹Ï  ­n·s¼W¨C­Órectªº 
		void display(float x, float y);
		void showmap();
		int get(int choice);
		void setdata(int x, int y, int who_on_it);
		Rect getdata(int x, int y); 
		// ²MªÅ©Ò¦³ªº°O¾ĞÅE 
		~Map();
};

class Soldier{
	public:
		Soldier();
		void init(int value[]);
		virtual bool move(int d, Map &map);								//¦b¦a¹Ï¤W²¾°Ê d:¤è¦V
		void stop(bool type);						//¥ğ®§©Î«İ¾E
		void over();								//²¾°Êµ²§E
		void attack(Soldier &s);					//§ğÀ» Soldier &s 
		void act();										//³Q§ğÀ»®É 
		void display(float cursorx, float cursory, bool who); 
		template<typename returntype> returntype get(int choice);						//¨ú°Ñ¼Æ
		Value getvalue(int choice);
		void setvalue(int choice, int n1, int n2);
		template<typename returntype> void set(int choice, returntype n);				//³]©w°Ñ¼Æ 
		inline int getType(){return type;}		//ºØÃş 
		//virtual void special(Soldier &s) = 0;		//¹EÄ¤èªº¯S®úÃğÀ»
		void showstate();
		virtual bool canmove(char maptype, int who_on_it);
		~Soldier();
		static void load();
	private:
		static GLMmodel* modelPtr[7];
		static GLuint soldierstate;
		int faced;
		void correct(int d);
		bool MVempty(float tmpx, float tmpy);
		bool dead;
		int special;//¯S®úËà¤OÃş«¬
		bool cursor_on; //¥Ñ¼Ğ«EEÚ
		bool ismove; 
		int state; //²{¦bª¬ºA¡G¡@°±¤û½@²¾°Ê¡@¦æ°Ê
		int type; //1:ªE2:¤¤ 3:»· 
		float x;   //x ®y¼Ğ 
		float y;	//y ®y¼Ğ
		int org_x;
		int org_y;
		int skin;
		Value HP;  //¥Í©R¤O 
		Value MV;  //²¾°Ê¤O 
		Value ACT; //¦æ°Ê­È 
		Value ATT; //§ğÀ»¤O 
		Value EV;  //°{Á×¤O
		Value HIT; //©R¤¤¤O
		Value CT;
		int LV;  //µ¥¯Å
		int EXP;		//friend: ²{¦bªº¸gÅç­È enemy:¥´­Ë©ÒÀò±oªº¸gÅç­È 
};

class Friend : public Soldier{
	public:
		Friend();
		~Friend();
		void display(float cursorx, float cursory);
		//virtual void special(Soldier &s);
		void levelup(); 
};

class Enemy : public Soldier{
	public:
		Enemy();
		virtual bool move(int d, Map &map);
		virtual bool canmove(char maptype, int who_on_it);
		void display(float cursorx, float cursory);
		~Enemy();
		//virtual void special(Soldier &s);
};

class Cursor{
	public:
		Cursor();
		bool correct(int d, Map &map, bool change);
		template<typename returntype> returntype get(int choice);
		template<typename returntype> void set(int choice, returntype n);
		void move(int d, Friend* friends, Enemy* enemys, Map &map);
		void enter(Map &map, Friend* friends, Enemy* enemys, int key);//¥s¥X¿Eæ©Î¿Ew¤h§L
		void cancel(Map &map, Friend* friends, Enemy* enemys);
		void display();
		~Cursor();
	private:
		float x;
		float y;
		int select;//¿EF½Ö 
};

class FightTurn{
	public:
		FightTurn();
		void load(Map *map);
		void init();
		void addfriend(int i,int num);
		void addenemy(int i,int num);
		template<typename returntype> returntype get(int choice);
		template<typename returntype> void set(int choice, returntype n);
		void drawstage(Enemy *enemys, Friend *friends);		//µe¾Ô°«³õ´º

		bool draw_action(Enemy *enemys, Friend *friends);		//µe°Ê§@
		void draw_background(GLuint bg);
		void draw_sel_man(Enemy *enemys, Friend *friends);
		void draw_all_member(Enemy *enemys, Friend *friends);
		void control(int key, Friend *friends);					//«öÁä±±¨E
		void ai(Enemy *enemys, Friend *friends);
		void smartai(Enemy *enemys, Friend *friends);
		float cal_probability(int ev, int hit);
		void cal_result(Enemy *enemys, Friend *friends);
		void value_cal(Enemy *enemys, Friend *friends);
		void round_init(Enemy *enemys, Friend *friends);
		bool special_effect(Enemy *enemys, Friend *friends);
		void over(Enemy *enemys, Friend *friends);
		void check_dead(Enemy *enemys, Friend *friends);
		void value_reset(Enemy *enemys, Friend *friends);
		void prev_special_effect(Enemy *enemys, Friend *friends);
		int sel_bg(Enemy *enemys, Friend *friends);
		~FightTurn();
	private:
		bool actioncontrol(float posx[], float posy[], Enemy *enemys, Friend *friends);
		void cpudrawpos(float pos[], int nowstate, int &width, int &height, bool clear);
		GLuint friend_skin;						//¤Hª«ÀY¹³1
		GLuint friend_skin2;					//¤Hª«ÀY¹³2
		GLuint fight_bg;						//³õ´º
		GLuint f_bg1[4];						//­I´º1
		GLuint f_bg2;							//­I´º2
		GLuint motion_pic[4];					//¤Hª«°Ê§@
		GLuint sel_frame1;						//¿EÜ®Ø1
		GLuint sel_frame2;						//¿EÜ®Ø2
		GLuint att_type;
		GLuint special_att;
		GLuint att_range;
		GLuint Num_Table2;
		GLuint Num_Table;
		GLuint Num_Table3;
		GLuint battle_over;
		GLuint special_detail;
		Map *map;
		bool isload;
		int result_table[6];
		int round;
		int state;								//¿EÜª¬ºA
		int select;								//¿EÜ¹EH
		int enemy_select;
		int fdrawstate;
		int edrawstate;
		int feffectstate;
		int eeffectstate;
		bool e_special;
		bool special;
		bool fighting;							//¾Ô°«¤¤
		int friend_list[8];						//¾Ô³õ¤¤ªºfriend¦CªE
		int enemy_list[8];						//¾Ô³õ¤¤ªºenemy¦CªE
};

class AI {
public:
	AI();
	void construct(Map *m, Cursor *c, Friend *f, Enemy *e, int fn, int en, int s);
	void action();
	void Init();
	bool isFinish();
	~AI();
private:
	void InitNormal();
	void InitHard();
	void MovePathMatrix();
	void InitMap();
	void ConstructStackAttackPath();
	int AI01();
	void AI02(int soldier, int * bigMap);
	void AI03(int ** bigMap2, int ** bigMap3, int iHeight, int iWidth, IntPoint &toBigMap);
	void AI04();
	void AI05(int ** bigMap2);
	bool AI06(int ** bigMap2, int ** bigMap3, int iHeight, int iWidth, IntPoint toBigMap);
	int current;
	stack<IntPoint> pathAttack;
	int ** mapTable;
	int ** mapAttack;
	int * enemysGroups;
	IntPoint * groupsAction;
	bool isFin;
	bool isConstructed;
	bool isInit;
	bool isStarted;
	bool isGrouped;
	int enemysNum;
	int friendsNum;
	IntPoint * enemyPoint;
	IntPoint * friendPoint;
	IntPoint attackTarget;
	Map * map;
	Friend *friends;
	Enemy *enemys;
	Cursor * cursor;
	int mapHeight;
	int mapWidth;
	int state;
	int iSteps;
};

class Game{
	public:
		Game();
		static int get(int choice);//¥]¬AWINDOWªºªø¼e 
		void set(int choice, int n);
		void start();
		static void init(int tostate);//§EÜª¬ºA¥Î
		void goon();
		void input(int key);//±µ¨Eä½L¿é¤J 
		void display();//¶}©lµe­±  SAVE/LOAD ¤É¯Åµe­± 
		bool load(int choice);
		void save(int choice);
		void enter();
		void jump(bool next);
		void selectchoice(int key);
		bool startfight();		//ÀË¬d¬O§_¦³¾Ô°«µo¥Í
		static void changeview(bool type);//true: 3D, false: 2D
		template<typename returntype> returntype* getclass(int choice);
		static void drawtext(char *str, int posx, int posy);
		bool showfps;
		static bool check(float x, float y, float z, float r);
		~Game();
	private:
		static int gametype;
		static int prevstate;
		static int state;//gameª¬ºA  ¶}©lµe­±  SAVE/LOAD MAP²¾°Êµe­± §ğÀ»»R¥x ¤É¯Åµe­± ´«ª¬ºAµe­±  ¿EE
		static int select;
		int loadfile;
		static int scale;
		static int zRot;
		static int view;
		static float planeabcd[6][4];
		int friendnum;
		int enemynum;
		static void compute();
		void mapscene();
		void startmenuscene();
		void fightscene();
		void LV_UP(char);
		void gamemenuscene();
		void loadscene();
		void savescene();
		void mapover();
		void checkmap();
		void gameoverscene();
		void ai();
		int fps;
		GLuint gameoverpic;
		GLuint allclearpic;
		GLuint startmenupic;
		GLuint textpic;
		GLuint levelup;
		GLuint levelup_info;
		GLuint Num_Table;
		GLuint friend_skin;						//¤Hª«ÀY¹³
		GLuint mapstate;
		Cursor cursor;
		FightTurn fightturn;
		Enemy* enemys;
		Friend* friends;
		Map map;
		AI Ai;
};
