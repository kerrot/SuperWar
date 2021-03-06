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
	int org; //��E誚h少 
	int now; //現在多少 
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
		void init();//  建構地圖  要新增每個rect的 
		void display(float x, float y);
		void showmap();
		int get(int choice);
		void setdata(int x, int y, int who_on_it);
		Rect getdata(int x, int y); 
		// 清空所有的記憶��E 
		~Map();
};

class Soldier{
	public:
		Soldier();
		void init(int value[]);
		virtual bool move(int d, Map &map);								//在地圖上移動 d:方向
		virtual void stop(bool type);						//休息或待��E
		void over();								//移動結��E
		void attack(Soldier &s);					//攻擊 Soldier &s 
		void act();										//被攻擊時 
		void display(float cursorx, float cursory, bool who); 
		template<typename returntype> returntype get(int choice);						//取參數
		Value getvalue(int choice);
		void setvalue(int choice, int n1, int n2);
		template<typename returntype> void set(int choice, returntype n);				//設定參數 
		inline int getType(){return type;}		//種類 
		//virtual void special(Soldier &s) = 0;		//��E臚隤滲S浦鏢擊
		void showstate();
		virtual bool canmove(char maptype, int who_on_it);
		~Soldier();
		static void load();
	protected:
		static GLMmodel* modelPtr[7];
		static GLuint soldierstate;
		int faced;
		void correct(int d);
		bool MVempty(float tmpx, float tmpy);
		bool dead;
		int special;//特浦咈力類型
		bool cursor_on; //由標��E�E�
		bool ismove; 
		int state; //現在狀態：　停牛瑾移動　行動
		int type; //1:��E2:中 3:遠 
		float x;   //x 座標 
		float y;	//y 座標
		int org_x;
		int org_y;
		int skin;
		Value HP;  //生命力 
		Value MV;  //移動力 
		Value ACT; //行動值 
		Value ATT; //攻擊力 
		Value EV;  //閃避力
		Value HIT; //命中力
		Value CT;
		int LV;  //等級
		int EXP;		//friend: 現在的經驗值 enemy:打倒所獲得的經驗值 
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
		virtual void stop(bool type);
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
		void enter(Map &map, Friend* friends, Enemy* enemys, int key);//叫出��E璈怷�Ew士兵
		void cancel(Map &map, Friend* friends, Enemy* enemys);
		void display();
		~Cursor();
	private:
		float x;
		float y;
		int select;//��EF誰 
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
		void drawstage(Enemy *enemys, Friend *friends);		//畫戰鬥場景

		bool draw_action(Enemy *enemys, Friend *friends);		//畫動作
		void draw_background(GLuint bg);
		void draw_sel_man(Enemy *enemys, Friend *friends);
		void draw_all_member(Enemy *enemys, Friend *friends);
		void control(int key, Friend *friends);					//按鍵控��E
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
		GLuint friend_skin;						//人物頭像1
		GLuint friend_skin2;					//人物頭像2
		GLuint fight_bg;						//場景
		GLuint f_bg1[4];						//背景1
		GLuint f_bg2;							//背景2
		GLuint motion_pic[4];					//人物動作
		GLuint sel_frame1;						//��E飫�1
		GLuint sel_frame2;						//��E飫�2
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
		int state;								//��E靰牯A
		int select;								//��E僊�EH
		int enemy_select;
		int fdrawstate;
		int edrawstate;
		int feffectstate;
		int eeffectstate;
		bool e_special;
		bool special;
		bool fighting;							//戰鬥中
		int friend_list[8];						//戰場中的friend列��E
		int enemy_list[8];						//戰場中的enemy列��E
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
		static int get(int choice);//包括WINDOW的長寬 
		void set(int choice, int n);
		void start();
		static void init(int tostate);//��E靰牯A用
		void goon();
		void input(int key);//接��E雿L輸入 
		void display();//開始畫面  SAVE/LOAD 升級畫面 
		bool load(int choice);
		void save(int choice);
		void enter();
		void jump(bool next);
		void selectchoice(int key);
		bool startfight();		//檢查是否有戰鬥發生
		static void changeview(bool type);//true: 3D, false: 2D
		template<typename returntype> returntype* getclass(int choice);
		static void drawtext(char *str, int posx, int posy);
		bool showfps;
		bool canfight;
		static bool check(float x, float y, float z, float r);
		~Game();
	private:
		static int gametype;
		static int prevstate;
		static int state;//game狀態  開始畫面  SAVE/LOAD MAP移動畫面 攻擊舞台 升級畫面 換狀態畫面  ��E�E
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
		GLuint friend_skin;						//人物頭像
		GLuint mapstate;
		Cursor cursor;
		FightTurn fightturn;
		Enemy* enemys;
		Friend* friends;
		Map map;
		AI Ai;
};
