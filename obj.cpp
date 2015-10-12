#include"func.h"
#include"obj.h"
#include"def.h"
#include<iostream>
#include<fstream>
#include<vector>
#include<iomanip>
#include<cmath>
#define MAXLEN 100
#define PI 3.14159265358979323846
using namespace std;

Game::Game(){
	gametype = NORMAL;
	scale = -5;
	zRot = 0;
	friendnum = 0;
	enemynum = 0;
	enemys = NULL;
	friends = NULL;
	view = ORTHO2D;
	fps = 0;
}

Game::~Game(){
	if(enemys != NULL)
		delete[] enemys;
	if(friends != NULL)
		delete[] friends;
}

void Game::display(){
	switch(state){
		case STARTMENU :	startmenuscene();break;
		case LOADING :		glColor3ub(255,255,255);
							drawtext("Loading...", glutGet(GLUT_WINDOW_WIDTH) - 100, glutGet(GLUT_WINDOW_HEIGHT)- 10) ;
							break;
		case OURTURN :
		case ENEMYTURN :	mapscene();break;
		case TOOURTURN :	break;
		case TOENEMYTURN :	break;
		case GAMEMENU :		gamemenuscene();break;
		case FIGHTSTAGE :	fightscene();break;
		case LVUP :			LV_UP(' ');break;
		case SAVE :			savescene();break;
		case LOAD :			loadscene();break;
		case MAPTOSTAGE :	break;
		case FIGHTEND :		break;
		case SHOWMAP:		map.showmap();break;
		case GAMEOVER:
		case ALLCLEAR:		gameoverscene();break;
	}
	fps++;
}

void Game::gameoverscene(){
	int w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
	if(state == GAMEOVER)
		glBindTexture(GL_TEXTURE_2D, gameoverpic);
	else
		glBindTexture(GL_TEXTURE_2D, allclearpic);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(0,1);glVertex2f( 0 , 0 );
		glTexCoord2f(1,1);glVertex2f( w , 0 );
		glTexCoord2f(1,0);glVertex2f( w , h );
		glTexCoord2f(0,0);glVertex2f( 0 , h );
	glEnd();
}

void Game::fightscene(){
	int i = 0;
	if( !(fightturn.get<int>(FIGHTING)) ){							//¨Oß_æ‘∞´§§
		fightturn.init();										//™ÅEl§∆æ‘∞´≥ı¥∫∏ÅE∆
		fightturn.set<bool>(FIGHTING, startfight() );				//¨Oß_¶≥µo•Õæ‘∞´

		if( !(fightturn.get<bool>(FIGHTING)) ){						//≠YµLæ‘∞´µo•Õ  ´h±Nstate≈‹¥´
			for(i = 0;i < friendnum;i++)
				if( friends[i].get<int>(DEAD) == false) friends[i].set(STATE,WAITING);
			for(i = 0;i < enemynum;i++)
				if( enemys[i].get<int>(DEAD) == false) enemys[i].set(STATE,WAITING);
			(prevstate == OURTURN)? init(ENEMYTURN): init(LVUP);
			checkmap();
			return;
		}
		else
			fightturn.prev_special_effect(enemys, friends);
	}
	fightturn.drawstage(enemys, friends);
}

void Game::startmenuscene(){

	int w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);

	glBindTexture(GL_TEXTURE_2D, startmenupic);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(0,1);glVertex2f( 0 , 0 );
		glTexCoord2f(1,1);glVertex2f( w , 0 );
		glTexCoord2f(1,0);glVertex2f( w , h );
		glTexCoord2f(0,0);glVertex2f( 0 , h );
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textpic);

	for(int i = 0; i < 3; ++i){
		(select == i)? glColor3f(1, 0, 0): glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
			glTexCoord2f(0,1 - i * 0.125);glVertex2f( w / 3 , h / 2 + i * 50);
			glTexCoord2f(0.5,1 - i * 0.125);glVertex2f( 2 * w / 3, h / 2 + i * 50);
			glTexCoord2f(0.5,0.875 - i * 0.125);glVertex2f( 2 * w / 3, h / 2 + (i + 1) * 50);
			glTexCoord2f(0,0.875 - i * 0.125);glVertex2f( w / 3 , h / 2 + (i + 1) * 50);
		glEnd();
	}
	(gametype == NORMAL)? glColor3f(1, 0, 0): glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0.625);glVertex2f( 2 * w / 3 , h / 2 + 25);
		glTexCoord2f(0.5,0.625);glVertex2f( w, h / 2 + 25);
		glTexCoord2f(0.5,0.5);glVertex2f( w, h / 2 + 75);
		glTexCoord2f(0,0.5);glVertex2f( 2 * w / 3 , h / 2 + 75);
	glEnd();
	(gametype == HARD)? glColor3f(1, 0, 0): glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0.5);glVertex2f( 2 * w / 3 , h / 2 + 75);
		glTexCoord2f(0.5,0.5);glVertex2f( w, h / 2 + 75);
		glTexCoord2f(0.5,0.375);glVertex2f( w, h / 2 + 125);
		glTexCoord2f(0,0.375);glVertex2f( 2 * w / 3 , h / 2 + 125);
	glEnd();
}

void Game::drawtext(char *str, int posx, int posy){
	int len = (int)strlen(str);
	glRasterPos2i(posx,posy);	//µe¶r¶ÅEm
	for(int i = 0; i < len ; i++)//µe¶r
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
}

void Game::mapscene(){
	static int nowfps;
	int i = 0;

	glPushMatrix();
	glTranslatef(0,0,scale);
	
	
	glRotatef(-35, 1.0f, 0.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	
	cursor.display();
	glTranslatef(cursor.get<float>(POSX) - 0.001,cursor.get<float>(POSY) - 0.001,0);
	compute();
	map.display(cursor.get<float>(POSX) - 0.001,cursor.get<float>(POSY) - 0.001);
	if(friends != NULL)
		for(i = 0; i < friendnum; ++i)
			friends[i].display(-(cursor.get<float>(POSX) - 0.001), -(cursor.get<float>(POSY) - 0.001));

	if(enemys != NULL)
		for(i = 0; i < enemynum; ++i)
			enemys[i].display(-(cursor.get<float>(POSX) - 0.001), -(cursor.get<float>(POSY) - 0.001));

	glPopMatrix();
	
	char temp[MAXLEN] = "";
	int w = glutGet(GLUT_WINDOW_WIDTH);
	Rect nowpos = map.getdata(-(cursor.get<float>(POSX) - 0.001), -(cursor.get<float>(POSY) - 0.001));
	changeview(false);
	glColor4ub(255, 255, 255, 150);
	glRectf(6 * w / 7 + 10, 10, w - 10, 105);
	glColor4ub(0, 0, 0, 255);
	sprintf(temp, "%d%%", nowpos.HIT_rate);
	drawtext(temp, w - 60, 60);
	sprintf(temp, "%d%%", nowpos.EV_rate);
	drawtext(temp, w - 60, 90);
	sprintf(temp, "%d", nowfps);
	drawtext(temp, w - 60, 30);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mapstate);
	glColor4ub(255, 255, 255, 255);
	glBegin(GL_QUADS);
		glTexCoord2f(0,1); glVertex2f( 6 * w / 7, 0);
		glTexCoord2f(1,1); glVertex2f( w, 0);
		glTexCoord2f(1,0); glVertex2f( w, w / 7);
		glTexCoord2f(0,0); glVertex2f( 6 * w / 7, w / 7);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	int tmpselect = cursor.get<int>(SELECT);
	if(nowpos.who_on_it != 0)
		tmpselect = nowpos.who_on_it;
	if(tmpselect != 0)
		(tmpselect > 0)? friends[tmpselect - 1].showstate(): enemys[-1 - tmpselect].showstate();
	

	if(showfps){
		showfps = false;
		nowfps = fps;
		fps = 0;
	}

	changeview(true);
}

void Game::gamemenuscene(){
	int w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
	glBindTexture(GL_TEXTURE_2D, textpic);
	for(int i = 0; i < 5; ++i){
		(select == i)? glColor3f(1, 0, 0): glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
			glTexCoord2f(0.5,1 - i * 0.125);	glVertex2f( w / 3 , h / 3 + i * 50);
			glTexCoord2f(1,1 - i * 0.125);		glVertex2f( 2 * w / 3, h / 3 + i * 50);
			glTexCoord2f(1,0.875 - i * 0.125);	glVertex2f( 2 * w / 3, h / 3 + (i + 1) * 50);
			glTexCoord2f(0.5,0.875 - i * 0.125);glVertex2f( w / 3 , h / 3 + (i + 1) * 50);
		glEnd();
	}
}

void Game::loadscene(){
	int w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
	ifstream file[10];
	char temp[MAXLEN] = "";
	int friendnum, enemynum, len;
	bool PorE = true;

	glDisable(GL_TEXTURE_2D);
	for(int i = 0; i < 10; ++i){
		(select == i)? glColor3f(1,0,0): glColor3f(1,1,1);

		sprintf(temp, "./data/%d.sav", i);
		file[i].open(temp);
		if(file[i]){

			friendnum = 0;
			enemynum = 0;
			file[i].getline(temp, MAXLEN);
			do{
				file[i].getline(temp, MAXLEN);
				len = (int)strlen(temp);
				if(temp[0] == 'P' || temp[0] == 'E')
					(temp[0] == 'P')? PorE = true: PorE = false;
				else if(len > 1){
					(PorE)? ++friendnum: ++enemynum;
				}
			}while(len);

			sprintf(temp, "%d. friends: %d enemys: %d.", i, friendnum, enemynum);
			drawtext(temp, w / 3, 50 + i * 50);
			
			file[i].close();
		}else{
			sprintf(temp, "%d. Empty.", i);
			drawtext(temp, w / 3, 50 + i * 50);
		}
	}
}

void  Game::savescene(){
	loadscene();
}

void Game::LV_UP(char key){
	static int lvup_table[8][2] = {0};
	static int count = 0;
	int temp = 0,temp2 = 0;
	int w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
	Value v;

	if(key == 'z'){
		for(int i = 0;i < 8;i++){
			lvup_table[i][0] = 0;
			lvup_table[i][1] = 0;
		}
		init(OURTURN);
		count = 0;
		return;
	}


	for(int i = 0;i < friendnum;i++){
		if( friends[i].get<bool>(DEAD) == false &&  friends[i].get<int>(EXPvalue) >= 1000*friends[i].get<int>(LVvalue)){
			temp2 = friends[i].get<int>(LVvalue);
			temp = 1+(friends[i].get<int>(EXPvalue))/1000;
			friends[i].set<int>(LVvalue, temp);
			temp = temp - temp2;

			v = friends[i].getvalue(ATTvalue);
			friends[i].setvalue(ATTvalue, v.org+5*temp, v.now+5*temp);
			v = friends[i].getvalue(HITvalue);
			friends[i].setvalue(HITvalue, v.org+2*temp, v.now+2*temp );
			v = friends[i].getvalue(CTvalue);
			friends[i].setvalue(CTvalue, v.org+2*temp, v.now+2*temp);
			v = friends[i].getvalue(EVvalue);
			friends[i].setvalue(EVvalue, v.org+2*temp, v.now+2*temp);

			if(count < 8){
				lvup_table[count][0] = i+1;
				lvup_table[count++][1] = temp;
			}
		}
	}
	int a = 0;
	for(a;a<8;a++)
		if(lvup_table[a][0]!=0)
			break;
	if(a==8){
		init(OURTURN);
		count = 0;
		return;
	}

	glBindTexture(GL_TEXTURE_2D, levelup);
	glColor3ub(255, 255, 255);
	glBegin(GL_QUADS);
		glTexCoord2f(0,1);glVertex2f( 0 , 0 );
		glTexCoord2f(1,1);glVertex2f( w , 0 );
		glTexCoord2f(1,0);glVertex2f( w , h );
		glTexCoord2f(0,0);glVertex2f( 0 , h );
	glEnd();



	for(int i = 0;i < 8;i++){
		glPushMatrix();			
		if(i < 4)
			glTranslatef(100,120+100*i,0);
		else
			glTranslatef(400,120+100*i,0);
		if( lvup_table[i][0] != 0){
				glBindTexture(GL_TEXTURE_2D, friend_skin);
				temp2 = lvup_table[i][0]-1;
				temp = friends[temp2].get<int>(TYPE);
				glBegin(GL_QUADS);
					glTexCoord2f( 0.25*(float)temp	   , 1);glVertex2f( 0, 0 );
					glTexCoord2f( 0.25*(float)temp+0.25, 1);glVertex2f(75, 0 );
					glTexCoord2f( 0.25*(float)temp+0.25, 0);glVertex2f(75,75 );
					glTexCoord2f( 0.25*(float)temp	   , 0);glVertex2f( 0,75 );
				glEnd();

				glTranslatef(100,0.0,0.0);
				glBindTexture(GL_TEXTURE_2D, levelup_info);
				glColor3ub(255, 255, 255);
				glBegin(GL_QUADS);
					glTexCoord2f(0,1);glVertex2f( 0 , 0);
					glTexCoord2f(1,1);glVertex2f( 128, 0);
					glTexCoord2f(1,0);glVertex2f( 128,64);
					glTexCoord2f(0,0);glVertex2f( 0 ,64);
				glEnd();

				glPushMatrix();
				glTranslatef(30,6,0);
				temp = friends[i].get<int>(LVvalue) - lvup_table[i][1];
				glBindTexture(GL_TEXTURE_2D, Num_Table);
				for(int j=0;j < 2;j++){
					temp2 = temp%10;
					glBegin(GL_QUADS);
						glTexCoord2f( 0.0625*(float)temp2	    , 1);glVertex2f( 0,22 );
						glTexCoord2f( 0.0625*(float)temp2+0.0625, 1);glVertex2f( 10,22 );
						glTexCoord2f( 0.0625*(float)temp2+0.0625, 0);glVertex2f( 10, 0 );
						glTexCoord2f( 0.0625*(float)temp2	    , 0);glVertex2f( 0, 0 );
					glEnd();
					temp = temp/10;
					glTranslatef(-10, 0.0, 0.0);
				}
				glPopMatrix();


				glPushMatrix();
				glTranslatef(100,6,0);
				temp = friends[i].get<int>(LVvalue);
				glBindTexture(GL_TEXTURE_2D, Num_Table);
				for(int j=0;j < 2;j++){
					temp2 = temp%10;
					glBegin(GL_QUADS);
						glTexCoord2f( 0.0625*(float)temp2	    , 1);glVertex2f( 0,22 );
						glTexCoord2f( 0.0625*(float)temp2+0.0625, 1);glVertex2f( 10,22 );
						glTexCoord2f( 0.0625*(float)temp2+0.0625, 0);glVertex2f( 10, 0 );
						glTexCoord2f( 0.0625*(float)temp2	    , 0);glVertex2f( 0, 0 );
					glEnd();
					temp = temp/10;
					glTranslatef(-10, 0.0, 0.0);
				}
				glPopMatrix();


				glPushMatrix();
				glTranslatef(53,30,0);
				temp = lvup_table[i][1]*5;
				glBindTexture(GL_TEXTURE_2D, Num_Table);
				for(int j=0;j < 2;j++){
					temp2 = temp%10;
					glBegin(GL_QUADS);
						glTexCoord2f( 0.0625*(float)temp2	    , 1);glVertex2f( 0,18 );
						glTexCoord2f( 0.0625*(float)temp2+0.0625, 1);glVertex2f( 8,18 );
						glTexCoord2f( 0.0625*(float)temp2+0.0625, 0);glVertex2f( 8, 0 );
						glTexCoord2f( 0.0625*(float)temp2	    , 0);glVertex2f( 0, 0 );
					glEnd();
					temp = temp/10;
					glTranslatef(-8, 0.0, 0.0);
				}
				glPopMatrix();

				glPushMatrix();
				glTranslatef(53,48,0);
				temp = lvup_table[i][1]*2;
				glBindTexture(GL_TEXTURE_2D, Num_Table);
				for(int j=0;j < 2;j++){
					temp2 = temp%10;
					glBegin(GL_QUADS);
						glTexCoord2f( 0.0625*(float)temp2	    , 1);glVertex2f( 0,18 );
						glTexCoord2f( 0.0625*(float)temp2+0.0625, 1);glVertex2f( 8,18 );
						glTexCoord2f( 0.0625*(float)temp2+0.0625, 0);glVertex2f( 8, 0 );
						glTexCoord2f( 0.0625*(float)temp2	    , 0);glVertex2f( 0, 0 );
					glEnd();
					temp = temp/10;
					glTranslatef(-8, 0.0, 0.0);
				}
				glPopMatrix();

				glPushMatrix();
				glTranslatef(110,30,0);
				temp = lvup_table[i][1]*2;
				glBindTexture(GL_TEXTURE_2D, Num_Table);
				for(int j=0;j < 2;j++){
					temp2 = temp%10;
					glBegin(GL_QUADS);
						glTexCoord2f( 0.0625*(float)temp2	    , 1);glVertex2f( 0,18 );
						glTexCoord2f( 0.0625*(float)temp2+0.0625, 1);glVertex2f( 8,18 );
						glTexCoord2f( 0.0625*(float)temp2+0.0625, 0);glVertex2f( 8, 0 );
						glTexCoord2f( 0.0625*(float)temp2	    , 0);glVertex2f( 0, 0 );
					glEnd();
					temp = temp/10;
					glTranslatef(-8, 0.0, 0.0);
				}
				glPopMatrix();

				glPushMatrix();
				glTranslatef(110,48,0);
				temp = lvup_table[i][1]*2;
				glBindTexture(GL_TEXTURE_2D, Num_Table);
				for(int j=0;j < 2;j++){
					temp2 = temp%10;
					glBegin(GL_QUADS);
						glTexCoord2f( 0.0625*(float)temp2	    , 1);glVertex2f( 0,18 );
						glTexCoord2f( 0.0625*(float)temp2+0.0625, 1);glVertex2f( 8,18 );
						glTexCoord2f( 0.0625*(float)temp2+0.0625, 0);glVertex2f( 8, 0 );
						glTexCoord2f( 0.0625*(float)temp2	    , 0);glVertex2f( 0, 0 );
					glEnd();
					temp = temp/10;
					glTranslatef(-8, 0.0, 0.0);
				}
				glPopMatrix();

		}
		glPopMatrix();
	}
}

void Game::input(int key){
	switch(key){
		case Z :enter();
				break;
		case X :switch(state){
				case STARTMENU:	break;
				case OURTURN:	cursor.cancel(map, friends, enemys); break;
				case ENEMYTURN:	cursor.cancel(map, friends, enemys); break;	
				case FIGHTSTAGE:fightturn.control(X, friends);break;
				case LVUP:		break;
				case SHOWMAP:
				case SAVE:
				case LOAD:
				case GAMEMENU : init(prevstate);break;
				}
				break;
		case C :switch(state){
				case OURTURN:	cursor.enter(map, friends, enemys, C); break;
				//case ENEMYTURN:	cursor.enter(map, friends, enemys, C); break;
				}
				break;
		case A :if(state != OURTURN) return;
				zRot -= 5;
				if(zRot < 0)zRot += 360;
				cout<<"z: "<<zRot<<endl;
				break;
		case D :if(state != OURTURN) return;
				zRot += 5;
				if(zRot > 360)zRot -= 360;
				cout<<"z: "<<zRot<<endl;
				break;
		case W :if(state != OURTURN && state != ENEMYTURN) return;
				scale--;
				break;
		case S :if(state != OURTURN && state != ENEMYTURN) return;
				scale++;
				break;
		case Q :if(state != OURTURN) return;
				jump(true);
				break;
		case E :if(state != OURTURN) return;
				jump(false);
				break;
		case UP :	
		case DOWN :	
		case LEFT :	
		case RIGHT :selectchoice(key);break;
	}
}

void Game::jump(bool next){
	if(cursor.get<int>(SELECT) != 0)
		return;
	float tmpx, tmpy;
	if(state == OURTURN){
		friends[select].set<bool>(CURSORON, false);
		select = (next)? (++select) % friendnum: (select + friendnum - 1) % friendnum;
		while(friends[select].get<int>(DEAD))
			select = (next)? (++select) % friendnum: (select + friendnum - 1) % friendnum;
		tmpx = -friends[select].get<float>(POSX);
		tmpy = -friends[select].get<float>(POSY);
		cursor.set<float>(POSX, tmpx);
		cursor.set<float>(POSY, tmpy);
		friends[select].set<bool>(CURSORON, true);
	}
	if(state == ENEMYTURN){
		enemys[select].set<bool>(CURSORON, false);
		select = (next)? (++select) % enemynum: (select + enemynum - 1) % enemynum;
		while(enemys[select].get<int>(DEAD))
			select = (next)? (++select) % enemynum: (select + enemynum - 1) % enemynum;
		tmpx = -enemys[select].get<float>(POSX);
		tmpy = -enemys[select].get<float>(POSY);
		cursor.set<float>(POSX, tmpx);
		cursor.set<float>(POSY, tmpy);
		enemys[select].set<bool>(CURSORON, true);
	}
}

void Game::enter(){
	switch(state){
	case STARTMENU:	switch(select){
					case 0:loadfile = -1;init(LOADING);break;
					case 1:gametype = (++gametype) % 2;break;
					case 2:init(LOAD);break;
					}break;
	case GAMEMENU:	switch(select){
					case 0:mapover();break;
					case 1:init(SHOWMAP);break;
					case 2:init(LOAD);break;
					case 3:init(SAVE);break;
					case 4:init(STARTMENU);break;
					}break;
	case LOAD:		loadfile = select;
					init(LOADING);
					break;
	case SAVE:		save(select);
					break;
	case FIGHTSTAGE:fightturn.control(Z, friends);break;
	case OURTURN:	cursor.enter(map, friends, enemys, Z); break;
	case ENEMYTURN:	cursor.enter(map, friends, enemys, Z); break;
	case LVUP:		LV_UP('z'); break;
	case GAMEOVER:
	case ALLCLEAR:  init(STARTMENU);break;
	}
}

void Game::selectchoice(int key){
	switch(state){
	case STARTMENU:	if(key == UP)
						select = (select + 2) % 3;
					if(key == DOWN)
					   select = (++select) % 3;
					break;
	case GAMEMENU:	if(key == UP)
					   select = (select + 4) % 5;
					if(key == DOWN)
					   select = (++select) % 5;
					break;
	case LOAD:
	case SAVE:		if(key == UP)
					   select = (select + 9) % 10;
					if(key == DOWN)
					   select = (++select) % 10;
					break;
	case OURTURN:	
	case ENEMYTURN:	cursor.move(key, friends, enemys, map);break;
	case FIGHTSTAGE: fightturn.control(key, friends);break;
	}
}

int Game::get(int choice){
	switch(choice){
		case STATE : return state;
		case VIEWANGLE : return zRot;
		case GAMETYPE: return gametype;
		case VIEW: return view;
		default: return 0;
	}
}

int Game::state = STARTMENU;
int Game::prevstate = STARTMENU;
int Game::view = ORTHO2D;
int Game::select = 0;
int Game::scale = -5;
int Game::zRot = 0;
int Game::gametype = NORMAL;
float Game::planeabcd[6][4];

bool Game::check(float x, float y, float z, float r){
	//cout<<"checkxyz: "<<x<<" "<<y<<endl;
	for(int i = 0; i < 6; ++i)
		if(planeabcd[i][0] * x + planeabcd[i][1] * y + planeabcd[i][2] * z + planeabcd[i][3] <= -r)
			return false;
	return true;
}

void Game::compute(){
	float p[16];
	float v[16];
	float m[16];

	glGetFloatv(GL_PROJECTION_MATRIX, p);
	glGetFloatv(GL_MODELVIEW_MATRIX, v);

	m[ 0] = v[ 0] * p[0] + v[ 1] * p[4] + v[ 2] * p[ 8] + v[ 3] * p[12];
	m[ 1] = v[ 0] * p[1] + v[ 1] * p[5] + v[ 2] * p[ 9] + v[ 3] * p[13];
	m[ 2] = v[ 0] * p[2] + v[ 1] * p[6] + v[ 2] * p[10] + v[ 3] * p[14];
	m[ 3] = v[ 0] * p[3] + v[ 1] * p[7] + v[ 2] * p[11] + v[ 3] * p[15];
	m[ 4] = v[ 4] * p[0] + v[ 5] * p[4] + v[ 6] * p[ 8] + v[ 7] * p[12];
	m[ 5] = v[ 4] * p[1] + v[ 5] * p[5] + v[ 6] * p[ 9] + v[ 7] * p[13];
	m[ 6] = v[ 4] * p[2] + v[ 5] * p[6] + v[ 6] * p[10] + v[ 7] * p[14];
	m[ 7] = v[ 4] * p[3] + v[ 5] * p[7] + v[ 6] * p[11] + v[ 7] * p[15];
	m[ 8] = v[ 8] * p[0] + v[ 9] * p[4] + v[10] * p[ 8] + v[11] * p[12];
	m[ 9] = v[ 8] * p[1] + v[ 9] * p[5] + v[10] * p[ 9] + v[11] * p[13];
	m[10] = v[ 8] * p[2] + v[ 9] * p[6] + v[10] * p[10] + v[11] * p[14];
	m[11] = v[ 8] * p[3] + v[ 9] * p[7] + v[10] * p[11] + v[11] * p[15];
	m[12] = v[12] * p[0] + v[13] * p[4] + v[14] * p[ 8] + v[15] * p[12];
	m[13] = v[12] * p[1] + v[13] * p[5] + v[14] * p[ 9] + v[15] * p[13];
	m[14] = v[12] * p[2] + v[13] * p[6] + v[14] * p[10] + v[15] * p[14];
	m[15] = v[12] * p[3] + v[13] * p[7] + v[14] * p[11] + v[15] * p[15];

	//right
	planeabcd[0][0] = m[ 3] - m[ 0];
	planeabcd[0][1] = m[ 7] - m[ 4];
	planeabcd[0][2] = m[11] - m[ 8];
	planeabcd[0][3] = m[15] - m[12];
	//left
	planeabcd[1][0] = m[ 3] + m[ 0];
	planeabcd[1][1] = m[ 7] + m[ 4];
	planeabcd[1][2] = m[11] + m[ 8];
	planeabcd[1][3] = m[15] + m[12];
	//bottom
	planeabcd[2][0] = m[ 3] + m[ 1];
	planeabcd[2][1] = m[ 7] + m[ 5];
	planeabcd[2][2] = m[11] + m[ 9];
	planeabcd[2][3] = m[15] + m[13];
	//top
	planeabcd[3][0] = m[ 3] - m[ 1];
	planeabcd[3][1] = m[ 7] - m[ 5];
	planeabcd[3][2] = m[11] - m[ 9];
	planeabcd[3][3] = m[15] - m[13];
	//far
	planeabcd[4][0] = m[ 3] - m[ 2];
	planeabcd[4][1] = m[ 7] - m[ 6];
	planeabcd[4][2] = m[11] - m[10];
	planeabcd[4][3] = m[15] - m[14];
	//near
	planeabcd[5][0] = m[ 3] + m[ 2];
	planeabcd[5][1] = m[ 7] + m[ 6];
	planeabcd[5][2] = m[11] + m[10];
	planeabcd[5][3] = m[15] + m[14];
}

void Game::set(int choice, int n){
	switch(choice){
		case STATE : state = n;break;
	}
}

void Game::init(int tostate){
	if(state != GAMEMENU)
		prevstate = state;
	state = tostate;
	select = 0;
	zRot = 0;
	switch(state){
	case STARTMENU :	glEnable(GL_TEXTURE_2D);
						stopallmusic();
						SoundFunc(MUSIC_START, PLAY);
						break;
	case LOADING :		glDisable(GL_TEXTURE_2D);
						break;
	case OURTURN :		stopallmusic();
						SoundFunc(MUSIC_OUR, PLAY);
						if(view != PERSPECTIVE)
							changeview(true);
						break;
	case ENEMYTURN :	stopallmusic();
						SoundFunc(MUSIC_ENEMY, PLAY);
						if(view != PERSPECTIVE)
							changeview(true);
						break;
	case TOOURTURN :	break;
	case TOENEMYTURN :	break;
	case GAMEMENU :		if(view != ORTHO2D)
							changeview(false);
						glEnable(GL_TEXTURE_2D);
						break;
	case FIGHTSTAGE :	if(view != ORTHO2D)
							changeview(false);
						glEnable(GL_TEXTURE_2D);
						stopallmusic();
						break;
	case LVUP :			stopallmusic();
						SoundFunc(MUSIC_LVUP, PLAY);
						if(view != ORTHO2D)
							changeview(false);
						glEnable(GL_TEXTURE_2D);
						break;
	case SAVE :			if(view != ORTHO2D)
							changeview(false);
						break;
	case LOAD :			if(view != ORTHO2D)
							changeview(false);
						break;
	case MAPTOSTAGE :	break;
	case FIGHTEND :		break;
	case SHOWMAP:		if(view != ORTHO2D)
							changeview(false);
						glDisable(GL_TEXTURE_2D);
						break;
	case GAMEOVER:		stopallmusic();
						SoundFunc(MUSIC_GAMEOVER, PLAY);
						if(view != ORTHO2D)
							changeview(false);
						glEnable(GL_TEXTURE_2D);
						break;
	case ALLCLEAR:		stopallmusic();
						SoundFunc(MUSIC_ALLCLEAR, PLAY);
						if(view != ORTHO2D)
							changeview(false);
						glEnable(GL_TEXTURE_2D);
						break;
	}
	cout<<"prevstate: "<<prevstate<<" state: "<<state<<"view: "<<view<<endl;
}

void Game::start(){
	LoadGLTextures(&startmenupic, "./data/t.bmp", false);
	LoadGLTextures(&gameoverpic, "./data/gameover.bmp", false);
	LoadGLTextures(&allclearpic, "./data/allclear.bmp", false);
	LoadGLTextures(&friend_skin, "./data/friend_skin.bmp", false);
	LoadGLTextures(&textpic, "./data/text.bmp", true);
	LoadGLTextures(&levelup, "./data/level_up.bmp", false);
	LoadGLTextures(&friend_skin, "./data/friend_skin.bmp", false);
	LoadGLTextures(&mapstate, "./data/mapstate.bmp", true);
	LoadGLTextures(&levelup_info, "./data/level_up_info.bmp",false);
	LoadTGATextures(&Num_Table, "./data/Num_Table3.tga");
	LoadWave();
	init(STARTMENU);
}
 
void Game::goon(){
	switch(state){
	case LOADING:	map.init();
					fightturn.load(&map);
					Soldier::load();
					Ai.Init();
					if(load(loadfile)){
						init(OURTURN);
						checkmap();
					}
					else
						init(prevstate);
					
					break;
	case ENEMYTURN: ai();
					//mapover();
					break;
	}
}

void Game::checkmap(){
	bool gameover = true;
	for(int i = 0; i < friendnum; ++i){
		if(friends[i].get<int>(DEAD) != 1){
			gameover = false;
			break;
		}
		gameover = true;
	}
	if(gameover){
		init(GAMEOVER);
		return;
	}
	gameover = true;
	for(int i = 0; i < enemynum; ++i){
		if(enemys[i].get<int>(DEAD) != 1){
			gameover = false;
			break;
		}
		gameover = true;
	}
	if(gameover){
		init(ALLCLEAR);
		return;
	}
	int mapw = map.get(WIDTH), maph = map.get(HEIGHT);
	Value v;
	for(int i = 0; i < maph; ++i)
		for(int j = 0; j < mapw; ++j)
			map.setdata(j, i, 0);
	for(int i = 0; i < friendnum; ++i){
		v = friends[i].getvalue(HPvalue);
		if(v.now == 0 && friends[i].get<int>(DEAD) != 1)
			friends[i].set<int>(DEAD, 1);
		if(friends[i].get<int>(DEAD) != 1)
			map.setdata(friends[i].get<int>(ORGX), friends[i].get<int>(ORGY), i + 1);
	}

	for(int i = 0; i < enemynum; ++i){
		v = enemys[i].getvalue(HPvalue);
		if(v.now == 0 && enemys[i].get<int>(DEAD) != 1)
			enemys[i].set<int>(DEAD, 1);
		if(enemys[i].get<int>(DEAD) != 1)
			map.setdata(enemys[i].get<int>(ORGX), enemys[i].get<int>(ORGY), -i - 1);
	}

	int aftx = (int)(-(cursor.get<float>(POSX)-0.001)), afty = (int)(-(cursor.get<float>(POSY)-0.001));
	Rect temp = map.getdata(aftx, afty);
	if(temp.who_on_it != 0){
		(temp.who_on_it > 0)? friends[temp.who_on_it -1].set<bool>(CURSORON, true)
							: enemys[-1 - temp.who_on_it].set<bool>(CURSORON, true);
	}
}

bool Game::startfight(){
	int stage_x = 0, stage_y = 0;					//æ‘≥ıÆyº–
	int i = 0, j = 0, k = 0, count1 = 0, count2 = 0;
	bool boss = false;

	for(i = 0;i < friendnum;i++){
		if( friends[i].get<int>(DEAD) == false &&  friends[i].get<int>(STATE) != FIGHT){
			stage_x = friends[i].get<int>(POSX)/3;
			stage_y = friends[i].get<int>(POSY)/3;

			for(j = 0;j < enemynum;j++)
				if(enemys[j].get<int>(POSX)/3 == stage_x && enemys[j].get<int>(POSY)/3 == stage_y && enemys[j].get<int>(DEAD) == false){
					fightturn.addenemy(j,count1++);
					enemys[j].set<int>( STATE, FIGHT);
					if(enemys[j].get<int>(TYPE) == 3)
						boss = true;
					if(count1 == 8)
						break;
				}
			if(count1 != 0){
				for(k = 0;k < friendnum;k++)
					if(friends[k].get<int>(POSX)/3 == stage_x && friends[k].get<int>(POSY)/3 == stage_y && friends[k].get<int>(DEAD) == false){
						fightturn.addfriend(k, count2++);
						friends[k].set<int>(STATE, FIGHT);
						if(count2 == 8)
							break;
					}
			}
		}

		if(count1 != 0){
			if(count1 >= count2)
				fightturn.set<int>(ROUND,count1);
			else
				fightturn.set<int>(ROUND,count2);

			if(boss)
				SoundFunc(MUSIC_BOSS,PLAY);
			else
				SoundFunc(MUSIC_BATTLE,PLAY);
			return true;
		}
	}
	return false;
}

bool Game::load(int choice){
	char filename[MAXLEN] = "./data/game.dat";
	char temp[MAXLEN] = "";
	char *ptr = NULL;
	int len = 0, i = 0, j = 0;
	bool PorE = true;
	if(choice != -1){
		sprintf(temp, "./data/%d.sav", choice);
		strcpy(filename, temp);
	}

	ifstream gamedata;
	vector<int> f;
	vector<int> e;
	gamedata.open(filename);

	if(!gamedata){
		cout<<"load fail"<<endl;
		return false;
	}
	gamedata.getline(temp, MAXLEN);

	int n = 17;
	friendnum = 0;
	enemynum = 0;
	do{
		gamedata.getline(temp, MAXLEN);
		len = (int)strlen(temp);
		if(temp[0] == 'P' || temp[0] == 'E')
			(temp[0] == 'P')? PorE = true: PorE = false;
		else if(len > 1){
			ptr = strtok(temp, "\t ");
			for(i = 0; i < n; ++i){
				if(PorE)
					(ptr != NULL)? f.push_back(atoi(ptr)): f.push_back(0);
				else
					(ptr != NULL)? e.push_back(atoi(ptr)): e.push_back(0);
				ptr = strtok(NULL, "\t ");
			}
			(PorE)? ++friendnum: ++enemynum;
	   }
	}while(len);
	
	gamedata.close();
	
	if(enemys != NULL)
		delete[] enemys;
	if(friends != NULL)
		delete[] friends;
	
	friends = new Friend[friendnum];
	enemys = new Enemy[enemynum];

	int *value = new int[n];
	for(i = 0; i < friendnum; ++i){
		for(j = 0; j < n; ++j)
			value[j] = f[i * n + j];
		friends[i].init(value);
		map.setdata(value[0], value[1], i + 1);
	}
	for(i = 0; i < enemynum; ++i){
		for(j = 0; j < n; ++j)
			value[j] = e[i * n + j];
		enemys[i].init(value);
		map.setdata(value[0], value[1], -i - 1);
	}

	delete []value;
	f.clear();
	e.clear();
	cout<<"load: "<<filename<<" f: "<<friendnum<<" e: "<<enemynum<<endl;
	return true;
}

void Game::save(int choice){
	ofstream file;
	char temp[MAXLEN] = "";
	Value tmpvalue;

	sprintf(temp, "./data/%d.sav", choice);
	file.open(temp);
	file<<"X\tY\torgHP\tnowHP\tMV\torgACT\tnowACT\tATT\tEV\tHIT\tCT\tLV\tEXP\tTYPE\tSPECIAL\tMOVED\tSKIN\n";
	file<<"P\n";
	for(int i = 0; i < friendnum; ++i){
		file<<friends[i].get<int>(ORGX)<<"\t"<<friends[i].get<int>(ORGY)<<"\t";
		tmpvalue = friends[i].getvalue(HPvalue);
		file<<tmpvalue.org<<"\t"<<tmpvalue.now<<"\t";
		tmpvalue = friends[i].getvalue(MVvalue);
		file<<tmpvalue.org<<"\t";
		tmpvalue = friends[i].getvalue(ACTvalue);
		file<<tmpvalue.org<<"\t"<<tmpvalue.now<<"\t";
		tmpvalue = friends[i].getvalue(ATTvalue);
		file<<tmpvalue.org<<"\t";
		tmpvalue = friends[i].getvalue(EVvalue);
		file<<tmpvalue.org<<"\t";
		tmpvalue = friends[i].getvalue(HITvalue);
		file<<tmpvalue.org<<"\t";
		tmpvalue = friends[i].getvalue(CTvalue);
		file<<tmpvalue.org<<"\t";
		file<<friends[i].get<int>(LVvalue)<<"\t"<<friends[i].get<int>(EXPvalue)<<"\t";
		file<<friends[i].get<int>(TYPE)<<"\t"<<friends[i].get<int>(SPECIAL)<<"\t";
		file<<friends[i].get<int>(ISMOVE)<<"\t"<<friends[i].get<int>(SKIN)<<"\n";
	}

	file<<"E\n";
	for(int i = 0; i < enemynum; ++i){
		file<<enemys[i].get<int>(ORGX)<<"\t"<<enemys[i].get<int>(ORGY)<<"\t";
		tmpvalue = enemys[i].getvalue(HPvalue);
		file<<tmpvalue.org<<"\t"<<tmpvalue.now<<"\t";
		tmpvalue = enemys[i].getvalue(MVvalue);
		file<<tmpvalue.org<<"\t";
		tmpvalue = enemys[i].getvalue(ACTvalue);
		file<<tmpvalue.org<<"\t"<<tmpvalue.now<<"\t";
		tmpvalue = enemys[i].getvalue(ATTvalue);
		file<<tmpvalue.org<<"\t";
		tmpvalue = enemys[i].getvalue(EVvalue);
		file<<tmpvalue.org<<"\t";
		tmpvalue = enemys[i].getvalue(HITvalue);
		file<<tmpvalue.org<<"\t";
		tmpvalue = enemys[i].getvalue(CTvalue);
		file<<tmpvalue.org<<"\t";
		file<<enemys[i].get<int>(LVvalue)<<"\t"<<enemys[i].get<int>(EXPvalue)<<"\t";
		file<<enemys[i].get<int>(TYPE)<<"\t"<<enemys[i].get<int>(SPECIAL)<<"\t";
		file<<enemys[i].get<int>(ISMOVE)<<"\t"<<enemys[i].get<int>(SKIN)<<"\n";
	}
	file.close();
}

void Game::changeview(bool type){
	GLsizei w = glutGet(GLUT_WINDOW_WIDTH),  h = glutGet(GLUT_WINDOW_HEIGHT);
	glViewport(0, 0, w, h);
	if(type){
		GLfloat fAspect;
		if(h == 0)h = 1;		
		fAspect = (GLfloat)w/(GLfloat)h;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0f, fAspect, 1.0, 600.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		view = PERSPECTIVE;
		glEnable(GL_DEPTH_TEST);
	}else{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, (GLfloat)w, (GLfloat)h, 0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity ();
		view = ORTHO2D;
		glDisable(GL_DEPTH_TEST);
	}
}

void Game::mapover(){
	int i = 0;
	Value temp;
	if(prevstate == OURTURN){
		for(i = 0; i < friendnum; ++i){
			temp = friends[i].getvalue(MVvalue);
			friends[i].setvalue(MVvalue, temp.org, temp.org);
			
			if (friends[i].get<bool>(ISMOVE) == false)
			{
				Value& v = friends[i].getvalue(ACTvalue);
				if (v.now != v.org)
				{
					friends[i].stop(true);
				}
				else
				{
					friends[i].stop(false);
				}
			}
			friends[i].set<bool>(ISMOVE, false);
		}
	}else if(prevstate == ENEMYTURN || state == ENEMYTURN){
		for(i = 0; i < enemynum; ++i){
			temp = enemys[i].getvalue(MVvalue);
			enemys[i].setvalue(MVvalue, temp.org, temp.org);
			enemys[i].set<bool>(ISMOVE, false);
		}
	}//else
	//	init(STARTMENU);
	init(FIGHTSTAGE);
}

void Game::ai(){
	Ai.construct(&map, &cursor, friends, enemys, friendnum, enemynum, gametype);
	Ai.action();
	if(Ai.isFinish())
		mapover();
}

Cursor::Cursor(){
	x = 0;
	y = 0;
	select = 0;
}

Cursor::~Cursor(){

}

template<typename returntype> returntype Cursor::get(int choice){
	switch(choice){
		case POSX: return (returntype)x;
		case POSY: return (returntype)y;
		case SELECT: return (returntype)select;
		default: return 0;
	}
}

template<typename returntype> void Cursor::set(int choice, returntype n){
	switch(choice){
		case POSX: x = n;break;
		case POSY: y = n;break;
	}
}

void Cursor::display(){
	static int angle = 0;
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	glTranslatef(0, 0, 2);

	
	glRotatef(180, 0.0f, 1.0f, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	
	glColor3ub(200,200,200);
	glutWireCone(0.1, 0.5, 10, 10);
	glPopMatrix();

	angle += 2;
	if(angle == 360)
		angle = 0;
}

bool Cursor::correct(int d, Map&map, bool change){
	int w = map.get(WIDTH), h = map.get(HEIGHT);

	/*int intx = x - 0.001, inty = y - 0.001;
	if(x - (float)intx < 0.001){
		x = intx;
		cout<<"corroct."<<endl;
	}
	if(y - (float)inty < 0.001){
		y = inty;
		cout<<"corroct."<<endl;
	}*/
	float tmpx = -x, tmpy = -y;
	float angle = Game::get(VIEWANGLE);
	switch(d){
	case UP :	tmpy += CURSORSPEED * cos(PI / 180 * angle); tmpx += CURSORSPEED * sin(PI / 180 * angle);break;
	case DOWN :	tmpy += CURSORSPEED * cos(PI / 180 * (angle + 180)); tmpx += CURSORSPEED * sin(PI / 180 * (angle + 180));break;
	case LEFT :	tmpy += CURSORSPEED * cos(PI / 180 * (angle + 270)); tmpx += CURSORSPEED * sin(PI / 180 * (angle + 270));break;
	case RIGHT :tmpy += CURSORSPEED * cos(PI / 180 * (angle + 90)); tmpx += CURSORSPEED * sin(PI / 180 * (angle + 90));break;
	}
	if(tmpx >= 0 && tmpx <= w && tmpy >=0 && tmpy <= h){
		if(change){
			x = -tmpx;
			y = -tmpy;
		}
		return true;
	}else
		return false;
}

void Cursor::move(int d, Friend* friends, Enemy* enemys, Map &map){
	int w = map.get(WIDTH), h = map.get(HEIGHT);
	
	int prex = (int)(-(x-0.001)), prey = (int)(-(y-0.001));
	Rect temp = map.getdata(prex, prey);
	if(temp.who_on_it != 0 && select == 0){
		(temp.who_on_it > 0)? friends[temp.who_on_it -1].set<bool>(CURSORON, false)
							: enemys[-1 - temp.who_on_it].set<bool>(CURSORON, false);
		cout<<"cursoroff: "<<temp.who_on_it<<endl;
	}
	bool canmove = false;

	if(correct(d, map, false) ){
		if(select == 0)
			correct(d, map, true); 
		else{
			canmove = (select > 0)? friends[select - 1].move(d, map): enemys[-1 - select].move(d, map);
			(canmove) ? correct(d, map, true) : 1;
		}
	}
	int aftx = (int)(-(x-0.001)), afty = (int)(-(y-0.001));
	cout<<"aftxy: "<<aftx<<" "<<afty<<endl;
	temp = map.getdata(aftx, afty);
	if(temp.who_on_it != 0 && select == 0){
		(temp.who_on_it > 0)? friends[temp.who_on_it -1].set<bool>(CURSORON, true)
							: enemys[-1 - temp.who_on_it].set<bool>(CURSORON, true);
		cout<<"cursoron: "<<temp.who_on_it<<endl;
	}
	cout<<"X: "<<x<<" Y: "<<y<<endl;
}

void Cursor::enter(Map &map, Friend* friends, Enemy* enemys, int key){
	int tmpx = (int)(-(x-0.001)), tmpy = (int)(-(y-0.001));
	Rect tmprect;
	if(select != 0){
		switch(key){
		case Z:(select > 0)? friends[select - 1].stop(true): enemys[ -1 - select ].stop(true);break;
		case C:(select > 0)? friends[select - 1].stop(false): enemys[ -1 - select ].stop(false);break;
		}

		map.setdata(tmpx, tmpy, select);
		(select > 0)? friends[select - 1].set<int>(STATE, WAITING): enemys[ -1 - select ].set<int>(STATE, WAITING);
		select = 0;
	}else{
		tmprect = map.getdata(tmpx, tmpy);
		if(Game::get(STATE) == OURTURN && tmprect.who_on_it > 0){			
			select = tmprect.who_on_it;
			friends[select - 1].set<int>(STATE, ACTIVE);
			map.setdata(tmpx, tmpy, 0);
			x = -0.5 - friends[select - 1].get<float>(ORGX);
			y = -0.5 - friends[select - 1].get<float>(ORGY);
		}
		if(Game::get(STATE) == ENEMYTURN && tmprect.who_on_it < 0){
			select = tmprect.who_on_it;
			enemys[ -1 - select ].set<int>(STATE, ACTIVE);
			map.setdata(tmpx, tmpy, 0);
			x = -0.5 - enemys[ -1 - select ].get<float>(ORGX);
			y =	-0.5 - enemys[ -1 - select ].get<float>(ORGY);
		}
		if (tmprect.who_on_it == 0)
		{
			int tempstate = Game::get(STATE);
			Game::init(GAMEMENU);
			if (tempstate == ENEMYTURN)
			{
				Game::init(ENEMYTURN);
			}
		}
	}

	tmprect = map.getdata(tmpx, tmpy);
	cout<<"enter: "<<select<<" "<<tmprect.who_on_it<<endl;
}

void Cursor::cancel(Map &map, Friend* friends, Enemy* enemys){
	if(select == 0) return;
	Value v;
	if(select > 0){
		v = friends[select - 1].getvalue(MVvalue);
		if(friends[select - 1].get<float>(ORGY) == friends[select - 1].get<float>(POSY) &&
		   friends[select - 1].get<float>(ORGX) == friends[select - 1].get<float>(POSX)){

			friends[select - 1].set<int>(STATE, WAITING);
			map.setdata(friends[select - 1].get<int>(ORGX), friends[select - 1].get<int>(ORGY), select);
			select = 0;
		}else{
			friends[select - 1].set<float>(POSX, friends[select - 1].get<float>(ORGX));
			friends[select - 1].set<float>(POSY, friends[select - 1].get<float>(ORGY));
			friends[select - 1].setvalue(MVvalue, v.org, v.org);
			x = -0.5 - friends[select - 1].get<int>(ORGX);
			y = -0.5 - friends[select - 1].get<int>(ORGY);
		}
	}else{
		v = enemys[ -1 - select ].getvalue(MVvalue);
		if(enemys[ -1 - select ].get<float>(ORGY) == enemys[ -1 - select ].get<float>(POSY) &&
		   enemys[ -1 - select ].get<float>(ORGX) == enemys[ -1 - select ].get<float>(POSX)){

			enemys[ -1 - select ].set<int>(STATE, WAITING);
			map.setdata(enemys[ -1 - select ].get<int>(ORGX), enemys[ -1 - select ].get<int>(ORGY), select);
			select = 0;
		}else{
			enemys[ -1 - select ].set<float>(POSX, enemys[ -1 - select ].get<float>(ORGX));
			enemys[ -1 - select ].set<float>(POSY, enemys[ -1 - select ].get<float>(ORGY));
			enemys[ -1 - select ].setvalue(MVvalue, v.org, v.org);
			x = -0.5 - enemys[ -1 - select ].get<int>(ORGX);
			y = -0.5 - enemys[ -1 - select ].get<int>(ORGY);
		}
	}
	cout<<"select: "<<select<<" "<<endl;
}

Map::Map(){
	rect = NULL;
	width = 0;
	height = 0;
}

Map::~Map(){
	if(rect != NULL){
		for(int i = 0; i < height; ++i)
			delete[] rect[i];
		delete []rect;
	}
}

void Map::init(){
	int i = 0, j = 0;
	if(rect != NULL){
		for(i = 0; i < height; i++)
		for(j = 0; j < width; j++)
			rect[i][j].who_on_it = 0;
		return;
	}

	ifstream mapdata;

	char temp[MAXLEN] = "";
	
	vector<char> v;
	mapdata.open("./data/map.dat");
	if(!mapdata){
		cout<<"Open mapdata fail."<<endl;
		system("pause");
		exit(0);
	}

	mapdata.getline(temp, MAXLEN);
	width = (int)strlen(temp);

	do{
		++height;
		for(i = 0 ; i < width; ++i)
			v.push_back(temp[i]);
		mapdata.getline(temp, MAXLEN);
	}while(strlen(temp));

	rect = new Rect*[height];
	for(i = 0; i < height; i++){
		rect[i] = new Rect[width];
		for(j = 0; j < width; j++){
			rect[i][j].type = v[i * width + j];
			switch(rect[i][j].type){
			case '0':rect[i][j].HIT_rate = 0;rect[i][j].EV_rate = 0;break;
			case '1':rect[i][j].HIT_rate = -30;rect[i][j].EV_rate = 30;break;
			case '2':rect[i][j].HIT_rate = 20;rect[i][j].EV_rate = 20;break;
			case '3':rect[i][j].HIT_rate = 0;rect[i][j].EV_rate = -20;break;
			case '4':rect[i][j].HIT_rate = 0;rect[i][j].EV_rate = -10;break;
			case '5':rect[i][j].HIT_rate = 0;rect[i][j].EV_rate = -10;break;
			case '6':rect[i][j].HIT_rate = -20;rect[i][j].EV_rate = 20;break;
			case '7':rect[i][j].HIT_rate = 10;rect[i][j].EV_rate = 10;break;
			default: rect[i][j].HIT_rate = 0;rect[i][j].EV_rate = 0;break;
			}
			rect[i][j].who_on_it = 0;
			cout<<rect[i][j].type;
		}
		cout<<endl;
	}

	mapdata.close();
	v.clear();

	LoadGLTextures(&mappic, "./data/background.bmp", false);
}

void Map::display(float x, float y){

	glPushMatrix();
	//glTranslatef(x, y, 0);

	glEnable(GL_TEXTURE_2D);
	glColor3ub(255,255,255);
	glBindTexture(GL_TEXTURE_2D, mappic);
	glBegin(GL_QUADS);
		glTexCoord2f(1,1);	glVertex3f( 0, height, 0);
		glTexCoord2f(1,0);	glVertex3f( width, height, 0);
		glTexCoord2f(0,0);	glVertex3f( width, 0, 0);
		glTexCoord2f(0,1);	glVertex3f( 0, 0, 0);
	glEnd();

	
	int cx = (int)x, cy = (int)y;
	glDisable(GL_TEXTURE_2D);
	glColor4ub(255,0,0, 50);
	glBegin(GL_QUADS);
				glVertex3f( 1 - cx, -cy , 0.01);
				glVertex3f( -cx, -cy , 0.01);
				glVertex3f( -cx, 1 - cy, 0.01);
				glVertex3f( 1 - cx, 1 - cy , 0.01);
	glEnd();
	
	glColor4ub(0,0,0, 255);
	for(int i = 0; i < height; i += 3)
		for(int j = 0; j < width; j += 3){
			glPushMatrix();
			glTranslatef(1.5 + 3 * (j / 3),1.5 +  3 * (i / 3), -1.4);
			glutWireCube(3);
			glPopMatrix();
		}

	glPopMatrix();
	
}

void Map::showmap(){
	GLsizei w = glutGet(GLUT_WINDOW_WIDTH),  h = glutGet(GLUT_WINDOW_HEIGHT);
	int tmp = (height > width)? height: width;
	float n = (w < h)? (float)w / (float)tmp : (float)h / (float)tmp;
	
	float startx = (float)w / 2 - (float)width / 2 * n;
	float starty = (float)h / 2 + (float)height / 2 * n;

	for(int i = 0; i < height; ++i)
		for(int j = 0; j < width; ++j){
			if(rect[i][j].who_on_it != 0)
				(rect[i][j].who_on_it > 0)? glColor3f(0,0,1): glColor3f(1,0,0);
			else{
				switch(rect[i][j].type){
				case '0':glColor3f(0,1,0);break;
				case '1':glColor3ub(0,166,81);break;
				case '2':glColor3ub(96,57,19);break;
				case '3':glColor3f(0,1,1);break;
				case '4':glColor3f(1,1,0);break;
				case '5':glColor3ub(50,66,69);break;
				case '6':glColor3ub(80,200,0);break;
				case '7':glColor3f(0.7,0.7,0.7);break;
				default:glColor3ub(200,200,200);break;
				}
			}
			glRectf(startx + j * n, starty - i * n, startx + (j + 1) * n, starty - (i + 1) * n);
			glColor3f(0,0,0);
			glBegin(GL_LINES);
				glVertex2f( 0 , starty - i * n );
				glVertex2f( w , starty - i * n);
			glEnd();
			glBegin(GL_LINES);
				glVertex2f( startx + j * n, 0 );
				glVertex2f( startx + j * n , h);
			glEnd();
		}
	glEnable(GL_TEXTURE_2D);
	glColor4ub(255,255,255, 120);
	glBindTexture(GL_TEXTURE_2D, mappic);
	glBegin(GL_QUADS);
		glTexCoord2f(1,1);	glVertex3f( startx, starty - height * n, 0);
		glTexCoord2f(1,0);	glVertex3f( startx + width * n, starty - height * n, 0);
		glTexCoord2f(0,0);	glVertex3f( startx + width * n, starty, 0);
		glTexCoord2f(0,1);	glVertex3f( startx, starty, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void Map::setdata(int x, int y, int who_on_it){
	rect[y][x].who_on_it = who_on_it;
	//cout<<"who_on_it: "<<x<<" "<<y<<" "<<rect[y][x].who_on_it<<endl;
}

Rect Map::getdata(int x, int y){
	return rect[y][x];
}

int Map::get(int choice){
	switch(choice){
	case WIDTH:	return width;
	case HEIGHT:return height;
	}
	return 0;
}

Soldier::Soldier(){
	faced = 0;
}

Soldier::~Soldier(){

}

void Soldier::load(){
	static bool isload = false;
	if(isload)
		return;
	modelPtr[0] = glmReadOBJ("./data/pship.obj");	
	glmFacetNormals(modelPtr[0]);
	glmVertexNormals(modelPtr[0], 90.0);
	glmUnitize(modelPtr[0]);
	glmScale(modelPtr[0], 0.5);
	modelPtr[1] = glmReadOBJ("./data/eship.obj");	
	glmFacetNormals(modelPtr[0]);
	glmVertexNormals(modelPtr[0], 90.0);
	glmUnitize(modelPtr[1]);
	glmScale(modelPtr[1], 0.5);

	modelPtr[2] = glmReadOBJ("./data/pflyship.obj");	
	glmFacetNormals(modelPtr[2]);
	glmVertexNormals(modelPtr[2], 90.0);
	glmUnitize(modelPtr[2]);
	glmScale(modelPtr[2], 0.5);
	modelPtr[3] = glmReadOBJ("./data/eflyship.obj");	
	glmFacetNormals(modelPtr[3]);
	glmVertexNormals(modelPtr[3], 90.0);
	glmUnitize(modelPtr[3]);
	glmScale(modelPtr[3], 0.5);

	modelPtr[4] = glmReadOBJ("./data/pcar.obj");	
	glmFacetNormals(modelPtr[4]);
	glmVertexNormals(modelPtr[4], 90.0);
	glmUnitize(modelPtr[4]);
	glmScale(modelPtr[4], 0.5);
	modelPtr[5] = glmReadOBJ("./data/ecar.obj");	
	glmFacetNormals(modelPtr[5]);
	glmVertexNormals(modelPtr[5], 90.0);
	glmUnitize(modelPtr[5]);
	glmScale(modelPtr[5], 0.5);

	modelPtr[6] = glmReadOBJ("./data/f16.obj");	
	glmFacetNormals(modelPtr[6]);
	glmVertexNormals(modelPtr[6], 90.0);
	glmUnitize(modelPtr[6]);
	glmScale(modelPtr[6], 0.5);

	LoadGLTextures(&soldierstate, "./data/soldierstate.bmp", true);

	isload = true;
}

GLMmodel* Soldier::modelPtr[7];
GLuint Soldier::soldierstate;

void Soldier::init(int value[]){
	org_x = value[0];
	org_y = value[1];
	x = org_x;
	y = org_y;
	HP.org = value[2];
	HP.now = value[3];
	MV.org = value[4];
	MV.now = MV.org;
	ACT.org = value[5];
	ACT.now = value[6];
	ATT.org = value[7];
	ATT.now = ATT.org;
	EV.org = value[8];
	EV.now = EV.org;
	HIT.org = value[9];
	HIT.now = HIT.org;
	CT.org = value[10];
	CT.now = CT.org;
	LV = value[11];
	EXP = value[12];
	type = value[13];
	special = value[14];
	ismove = (value[15])? true: false;
	skin = value[16];
	state = WAITING;
	dead = false;
	cursor_on = false;

	cout<<"init: "<<x<<" "<<y<<" "<<ismove<<" "<<skin<<endl;
}

void Soldier::display(float cursorx, float cursory, bool who){
	if(dead) return;
	
	glEnable(GL_LIGHTING);
	glPushMatrix();
	//glTranslatef(cursor.get<float>(POSX) , cursor.get<float>(POSY)  , 0);
	glTranslatef(x + 0.5, y + 0.5, 0.5);

	glPushMatrix();
	switch(skin){
	case 0:	if(Game::check(x+0.5,y+0.5,0.2,0.6)){
				glTranslatef(0, 0, -0.2);
				if(ismove){
					faced = 0;
					glRotatef(270, 0,0,1);
					glColor3f(0.5, 0.5, 0.5);
					glmDraw(modelPtr[4], GLM_SMOOTH);
				}
				else{
					glRotatef(faced + 270, 0,0,1);
					(who)? glmDraw(modelPtr[4], GLM_SMOOTH | GLM_MATERIAL) 
						 : glmDraw(modelPtr[5], GLM_SMOOTH | GLM_MATERIAL);
				}
			}
			break;
	case 1:	if(Game::check(x+0.5,y+0.5,1,0.6)){
				glTranslatef(0, 0, 0.5);
				if(ismove){
					faced = 0;
					glRotatef(270, 0,0,1);
					glColor3f(0.5, 0.5, 0.5);
					glmDraw(modelPtr[2], GLM_SMOOTH);
				}
				else{
					glRotatef(faced + 270, 0,0,1);
					(who)? glmDraw(modelPtr[2], GLM_SMOOTH | GLM_MATERIAL) : glmDraw(modelPtr[3], GLM_SMOOTH | GLM_MATERIAL);
				}
			}
			break;
	case 2:	if(Game::check(x+0.5,y+0.5,0.3,0.6)){
				if(ismove){
					faced = 0;
					glColor3f(0.5, 0.5, 0.5);
					glmDraw(modelPtr[0], GLM_SMOOTH);
				}
				else{
					glRotatef(faced, 0,0,1);
					(who)? glmDraw(modelPtr[0], GLM_SMOOTH | GLM_MATERIAL) : glmDraw(modelPtr[1], GLM_SMOOTH | GLM_MATERIAL);
				}
			}
			break;
	case 3:	if(Game::check(x+0.5,y+0.5,1,0.6)){
				glTranslatef(0, 0, 0.5);
				if(ismove){
					faced = 0;
					glRotatef(90, 0,0,1);
					glColor3f(0.5, 0.5, 0.5);
					glmDraw(modelPtr[6], GLM_SMOOTH);
				}
				else{
					glRotatef(faced + 90, 0,0,1);
					glmDraw(modelPtr[6], GLM_SMOOTH | GLM_MATERIAL);
				}
			}
			break;
	}
	glPopMatrix();

	glDisable(GL_LIGHTING);
	glTranslatef(0, 0, -0.48);
	glRotatef(180, 0.0f, 1.0f, 0.0f);
	
	glColor4f(0,0,0, 0.4);
	glutSolidCone(0.2, 0, 30, 10);

	
	glPopMatrix();
}

template<typename returntype> void  Soldier::set(int choice, returntype n){
	switch(choice){
	case STATE: state = (returntype)n;break;
	case DEAD: (n == 0)? dead = false: dead = true;break;
	case POSX: x = (returntype)n;break;
	case POSY: y = (returntype)n;break;
	case ISMOVE: (n == 0)? ismove = false: ismove = true;break;
	case CURSORON:(n == 0)? cursor_on = false: cursor_on = true;break;
	case EXPvalue: EXP = (returntype)n;break;
	case LVvalue: LV = (returntype)n;break;
	}
}

template<typename returntype> returntype Soldier::get(int choice){
	switch(choice){
		case POSX: return (returntype)x;
		case POSY: return (returntype)y;
		case ORGX: return (returntype)org_x;
		case ORGY: return (returntype)org_y;
		case LVvalue: return (returntype)LV;
		case TYPE: return (returntype)type;
		case SPECIAL: return (returntype)special;
		case ISMOVE: return (returntype)ismove;
		case SKIN: return (returntype)skin;
		case EXPvalue: return (returntype)EXP;
		case DEAD: return (returntype)dead;
		case STATE: return (returntype)state;
		default: return 0;
	}
}

void Soldier::setvalue(int choice, int n1, int n2){
	switch(choice){
		case MVvalue: MV.org = n1; MV.now = n2;break;
		case HPvalue: HP.org = n1; HP.now = n2;break;
		case ATTvalue: ATT.org = n1; ATT.now = n2;break;
		case ACTvalue: ACT.org = n1; ACT.now = n2;break;
		case HITvalue: HIT.org = n1; HIT.now = n2;break;
		case EVvalue: EV.org = n1; EV.now = n2;break;
		case CTvalue: CT.org = n1; CT.now = n2;break;
	}
	//cout<<"MV: "<<MV.now<<" "<<MV.org<<endl;
}

Value Soldier::getvalue(int choice){
	switch(choice){
		case HPvalue: return HP;
		case MVvalue: return MV;
		case ACTvalue: return ACT;
		case ATTvalue: return ATT;
		case EVvalue: return EV;
		case HITvalue: return HIT;
		case CTvalue: return CT;
		default: return HP;
	}
}

void Soldier::correct(int d){
	float angle = Game::get(VIEWANGLE);

	/*int intx = x + 0.001, inty = y + 0.001;
	if((float)intx - x < 0.001){
		x = intx;
		cout<<"scorroct."<<endl;
	}
	if((float)inty - y < 0.001){
		y = inty;
		cout<<"scorroct."<<endl;
	}*/
	switch(d){
	case UP :	y += CURSORSPEED * cos(PI / 180 * angle); x += CURSORSPEED * sin(PI / 180 * angle);faced = 90 - angle;break;
	case DOWN :	y += CURSORSPEED * cos(PI / 180 * (angle + 180)); x += CURSORSPEED * sin(PI / 180 * (angle + 180));faced = 270 - angle;break;
	case LEFT :	y += CURSORSPEED * cos(PI / 180 * (angle + 270)); x += CURSORSPEED * sin(PI / 180 * (angle + 270));faced = 180 - angle;break;
	case RIGHT :y += CURSORSPEED * cos(PI / 180 * (angle + 90)); x += CURSORSPEED * sin(PI / 180 * (angle + 90));faced = - angle;break;
	}
}

bool Soldier::move(int d, Map &map){
	if(ismove || state != ACTIVE) return false;
	int w = map.get(WIDTH), h = map.get(HEIGHT);

	Rect tmp;
	float tmpx = x, tmpy = y;
	correct(d);
	int cpux = x + 0.5001, cpuy = y + 0.5001;
	
	tmp = map.getdata(cpux, cpuy);
	if(canmove(tmp.type, tmp.who_on_it))
		return MVempty(tmpx, tmpy);
	else{
		x = tmpx;
		y = tmpy;
	}
	cout<<"SX: "<<x<<" SY: "<<y<<endl;
	return false;
}

bool Soldier::canmove(char maptype, int who_on_it){
	if(who_on_it != 0)
		return false;
	cout<<"map: "<<maptype<<" "<<x<<" "<<y + CURSORSPEED<<endl;
	switch(skin){
	case 0:	if(maptype == '3')
				return false;
			else
				return true;
	case 1:	if(maptype == '2')
				return false;
			else
				return true;
	case 2:	if(maptype == '3' || maptype == '4')
				return true;
			else
				return false;
	case 3:	return true;
	}
	return false;
}

bool Soldier::MVempty(float tmpx, float tmpy){
	int cpux, cpuy;
	int tmpMV = MV.now;

	cpux = (int)(x + 0.5001); cpuy = (int)(y + 0.5001);
	(cpux > org_x)? MV.now = MV.org - cpux + org_x: MV.now = MV.org + cpux - org_x;
	(cpuy > org_y)? MV.now = MV.now - cpuy + org_y: MV.now = MV.now + cpuy - org_y;
	cout<<"nowMV: "<<MV.now<<endl;
	if(MV.now >= 0){
		cout<<"SX: "<<x<<" SY: "<<y<<endl;
		return true;
	}
	else{
		cout<<"nowxy: "<<x<<" "<<y<<endl;
		x = tmpx;
		y = tmpy;
		cout<<"tmpxy: "<<x<<" "<<y<<endl;
		MV.now = tmpMV;
		return false;
	}
}

void Soldier::stop(bool type){
	cout << "stop: " << ismove << " " << ACT.now << endl;
	if(ismove) return;
	ismove = true;
	org_x = (int)(x + 0.5001);
	org_y = (int)(y + 0.5001);
	x = org_x;
	y = org_y;
	float rate = 1;
	if (ACT.org != 0)
		rate = (float)ACT.now / (float)ACT.org;
	(type) ? ACT.now += 2 : HP.now += 20 + (int)(rate * (HP.org / 2));
	if (ACT.now > ACT.org)
		ACT.now = ACT.org;
	if(HP.now > HP.org)
		HP.now = HP.org;
}

void Soldier::showstate(){
	if(!cursor_on)
		return;
	
	GLsizei w = glutGet(GLUT_WINDOW_WIDTH),  h = glutGet(GLUT_WINDOW_HEIGHT);
	glColor4ub(255, 255, 255, 150);
	glRectf(30, 30, 240, 420);
	glRectf(w - 80, h - 80, w, h - 40);

	char temp[MAXLEN] = "";
	glColor4ub(0, 0, 0, 255);
	sprintf(temp, "%d", LV);
	Game::drawtext(temp, 100, 60);
	sprintf(temp, "%d", EXP);
	Game::drawtext(temp, 100, 100);
	sprintf(temp, "%d/%d", HP.now, HP.org);
	Game::drawtext(temp, 100, 140);
	sprintf(temp, "%d/%d", ACT.now, ACT.org);
	Game::drawtext(temp, 100, 175);
	sprintf(temp, "%d/%d", ATT.now, ATT.org);
	Game::drawtext(temp, 100, 215);
	sprintf(temp, "%d/%d", HIT.now, HIT.org);
	Game::drawtext(temp, 100, 255);
	sprintf(temp, "%d/%d", EV.now, EV.org);
	Game::drawtext(temp, 100, 290);
	sprintf(temp, "%d/%d", CT.now, CT.org);
	Game::drawtext(temp, 100,330);
	switch(type){
	case 0:Game::drawtext("Near", 100, 370);break;
	case 1:Game::drawtext("Middle", 100, 370);break;
	case 2:Game::drawtext("Far", 100, 370);break;
	case 3:Game::drawtext("Boss", 100, 370);break;
	}
	switch(special){
	case 0:Game::drawtext("None", 140, 410);break;
	case 1:Game::drawtext("Shadow", 140, 410);break;
	case 2:Game::drawtext("Wall", 140, 410);break;
	case 3:Game::drawtext("Service", 140, 410);break;
	case 4:Game::drawtext("Night", 140, 410);break;
	case 5:Game::drawtext("Battle", 140, 410);break;
	case 6:Game::drawtext("Beam",140, 410);break;
	case 7:Game::drawtext("Recover", 140, 410);break;
	case 8:Game::drawtext("HitUp", 140, 410);break;
	}
	
	
	float rate = 0;

	glColor4ub(255, 0, 0, 150);
	glRectf(30, h - 80, w - 90, h - 40);
	glColor4ub(255, 255, 0, 255);
	if(MV.org != 0)
		rate = (float)MV.now / (float)MV.org;
	glRectf(30, h - 80, 30 + (w - 120) * rate, h - 40);
	sprintf(temp, "%d/%d", MV.now, MV.org);
	glColor4ub(0, 0, 0, 255);
	Game::drawtext(temp, w - 50, h - 60);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, soldierstate);
	glColor4ub(255, 255, 255, 255);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0.25);	glVertex2f( 0, 0);
		glTexCoord2f(0,1);		glVertex2f( w / 3, 0);
		glTexCoord2f(1,1);		glVertex2f( w / 3, 3 * h / 4);
		glTexCoord2f(1,0.25);	glVertex2f( 0, 3 * h / 4);
	glEnd();
	glBegin(GL_QUADS);
		glTexCoord2f(0,0.25);	glVertex2f( 0, 4 * h / 5);
		glTexCoord2f(1,0.25);	glVertex2f( w, 4 * h / 5);
		glTexCoord2f(1,0);		glVertex2f( w, h);
		glTexCoord2f(0,0);		glVertex2f( 0, h);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

Friend::Friend(){

}

Friend::~Friend(){

}

void Friend::display(float cursorx, float cursory){
	glColor4ub(0, 0, 255, 255);
	Soldier::display(cursorx, cursory, true);
}

Enemy::Enemy(){

}

Enemy::~Enemy(){

}

void Enemy::display(float cursorx, float cursory){
	glColor4ub(255, 0, 0, 255);
	Soldier::display(cursorx, cursory, false);
}

bool Enemy::move(int d, Map &map)
{
	return Soldier::move(d, map);
}

bool Enemy::canmove(char maptype, int who_on_it)
{
	return Soldier::canmove(maptype, who_on_it);
}

FightTurn::FightTurn(){
	int i = 0;
	state = SEL_MEMBER;
	isload = false;
	fighting = false;
	for(i = 0;i < 8;i++){
		friend_list[i] = 0;
		enemy_list[i] = 0;
	}
}

void FightTurn::load(Map *m){
	if(isload)
		return;
	isload = true;
	LoadGLTextures(&friend_skin, "./data/friend_skin.bmp", false);
	LoadGLTextures(&f_bg1[0], "./data/f_bg0.bmp", false);
	LoadGLTextures(&f_bg1[1], "./data/f_bg1.bmp", false);
	LoadGLTextures(&f_bg1[2], "./data/f_bg2.bmp", false);
	LoadGLTextures(&f_bg1[3], "./data/f_bg3.bmp", false);
	LoadTGATextures(&sel_frame1, "./data/sel_frame1.tga");
	LoadTGATextures(&friend_skin2, "./data/friend_skin2.tga");
	LoadTGATextures(&f_bg2, "./data/f_bg2.tga");

	LoadTGATextures(&fight_bg, "./data/fight_bg.tga");
	LoadTGATextures(&motion_pic[0], "./data/p_active0.tga");
	LoadTGATextures(&motion_pic[1], "./data/p_active1.tga");
	LoadTGATextures(&motion_pic[2], "./data/p_active2.tga");
	LoadTGATextures(&motion_pic[3], "./data/p_active3.tga");

	LoadTGATextures(&att_type, "./data/attack_type.tga");
	LoadTGATextures(&sel_frame2, "./data/sel_frame2.tga");
	LoadGLTextures(&special_att, "./data/special_effect.bmp", true);
	LoadTGATextures(&att_range, "./data/attack_range.tga");
	LoadTGATextures(&Num_Table2, "./data/Num_Table2.tga");
	LoadTGATextures(&Num_Table, "./data/Num_Table.tga");
	LoadTGATextures(&Num_Table3, "./data/Num_Table3.tga");
	LoadTGATextures(&battle_over, "./data/battle_over.tga");
	LoadTGATextures(&special_detail, "./data/special_detail.tga");
	map = m;
}

void FightTurn::init(){
	int i = 0;
	round = -1;
	state = SEL_MEMBER;
	select = 0;
	enemy_select = -1;
	e_special = false;
	special = false;
	fighting = false;


	for(i = 0;i < 6;i++)
		result_table[i] = -1;
	for(i = 0;i < 8;i++){
		friend_list[i] = -1;
		enemy_list[i] = -1;
	}
}

FightTurn::~FightTurn(){

}

template<typename returntype> returntype FightTurn::get(int choice){
	switch(choice){
		case STATE: return (returntype)state;
		case FIGHTING: return (returntype)fighting;
		default: return 0;
	}
}

template<typename returntype> void FightTurn::set(int choice,returntype value){
	switch(choice){
		case STATE: state = value;break;
		case FIGHTING: fighting = value;break;
		case ROUND: round = value;break;
	}
}

void FightTurn::addfriend(int i,int num){
	friend_list[num] = i;
}
void FightTurn::addenemy(int i, int num){
	enemy_list[num] = i;
}

int FightTurn::sel_bg(Enemy *enemys, Friend *friends){
	int member = 0;
	Rect r;

	member = friend_list[0];
	r = map->getdata( friends[member].get<int>(POSX), friends[member].get<int>(POSY) );
	switch(r.type){
		case '0':return 0;
		case '1':return 1;
		case '2':return 2;
		case '3':return 2;
	}
	return 0;
}
void FightTurn::drawstage(Enemy *enemys, Friend *friends){
	static int pre_sel = 0;
	static int bg = -1;
	int w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
	int i = 0, member = 0;
	Value v;

	if(state == SEL_MEMBER)
		check_dead(enemys, friends);
	glColor3ub(255,255,255);


	//µeæ‘∞´≠I¥∫
	if(bg == -1)
		bg = sel_bg(enemys, friends);
	draw_background(bg);


	//∂}©lß¿ª µe•X§H™´∞ ß@
	if(state == CHK_ACT){
		member = friend_list[select];
		v = friends[member].getvalue(ACTvalue);
		ai(enemys, friends);				//AI
		if(special){
			if( special_effect(enemys, friends) ){	//ßP¬_MV¨Oß_®¨∞ÅE
				state = CAL_RESULT;
				result_table[4] = 1;
			}
			else
				state = SEL_ATT;
		}else
			if(v.now == 0){	//ßP¬_MV¨Oß_®¨∞ÅE
				result_table[0] = 0;
				result_table[1] = 0;
				result_table[2] = 0;
				result_table[4] = 0;
				state = CAL_RESULT;
			}
			else{
				i = v.now-1;
				friends[member].setvalue(ACTvalue, v.org, i);
				result_table[4] = 1;
				state = CAL_RESULT;
			}

		member = enemy_list[enemy_select];
		v = enemys[member].getvalue(ACTvalue);
		if(!e_special)
			if(v.now > 0){
				enemys[member].setvalue(ACTvalue, v.org, v.now - 1);
				result_table[5] = 1;
			}
			else{
				result_table[0] = 1;
				result_table[1] = 1;
				result_table[3] = 0;				
				result_table[5] = 0;				
			}
		else
			result_table[5] = 1;
	}


	if(state == CAL_RESULT){
		cal_result(enemys, friends);		//≠p∫‚æ‘∞´µ≤™G
		state = DRAW_ACTION;
	}

	if(state == DRAW_ACTION){
		draw_sel_man(enemys, friends);
		if( draw_action(enemys, friends) )
			round_init(enemys, friends);
	}

	if(state == BATTLE_OVER){
		over(enemys, friends);
		bg = -1;
	}

	//≠I¥∫0
	glBindTexture(GL_TEXTURE_2D, fight_bg);
	glBegin(GL_QUADS);
		glTexCoord2f(0,1);glVertex2f( 0 , h );
		glTexCoord2f(1,1);glVertex2f( w , h );
		glTexCoord2f(1,0);glVertex2f( w , 0 );
		glTexCoord2f(0,0);glVertex2f( 0 , 0 );
	glEnd();


	//µe•XøÅE‹™∫§H™´∏ÅE∆
	draw_sel_man(enemys, friends);


	//µe•X©“¶≥§H™∫∏ÅE∆
	draw_all_member(enemys, friends);
	//µe•XøÅE‹Æÿ
	glPushMatrix();
	if(select >= 0){
		if(pre_sel < select*75)
			glTranslatef( 0.0, pre_sel+=5 , 0.0);
		else 
			if(pre_sel > select*75)
				glTranslatef( 0.0, pre_sel-=5 , 0.0);
			else
				glTranslatef( 0.0, select*75 , 0.0);
		glBindTexture(GL_TEXTURE_2D, sel_frame1);
		glBegin(GL_QUADS);
			glTexCoord2f( 0, 1);glVertex2f(  0, 0 );
			glTexCoord2f( 1, 1);glVertex2f(162, 0 ); 
			glTexCoord2f( 1, 0);glVertex2f(162,76 );
			glTexCoord2f( 0, 0);glVertex2f(  0,76 );
		glEnd();
	}
	else{
		glTranslatef(638,0,0);
		if(pre_sel < (select+1)*-1*75 )
			glTranslatef(0, pre_sel+=5, 0.0);
		else 
			if(pre_sel > (select+1)*-1*75)
				glTranslatef( 0.0, pre_sel-=5 , 0.0);
			else
				glTranslatef( 0.0, (select+1)*-1*75 , 0.0);
		glBindTexture(GL_TEXTURE_2D, sel_frame1);
		glBegin(GL_QUADS);
			glTexCoord2f( 0, 1);glVertex2f(  0, 0 );
			glTexCoord2f( 1, 1);glVertex2f(162, 0 );
			glTexCoord2f( 1, 0);glVertex2f(162,77 );
			glTexCoord2f( 0, 0);glVertex2f(  0,77 );
		glEnd();
	}
	glPopMatrix();
	
	glPushMatrix();
	if(state == DRAW_ACTION){
		glTranslatef( 639.0, enemy_select*75 , 0.0);
		glBindTexture(GL_TEXTURE_2D, sel_frame1);
		glBegin(GL_QUADS);
			glTexCoord2f( 0, 1);glVertex2f(  0, 0 );
			glTexCoord2f( 1, 1);glVertex2f(162, 0 ); 
			glTexCoord2f( 1, 0);glVertex2f(162,76 );
			glTexCoord2f( 0, 0);glVertex2f(  0,76 );
		glEnd();
	}
	glPopMatrix();

	//¥∂≥qß¿ª(state = SEL_MEMBER)
	glPushMatrix();
	glTranslatef(165,550,0);
	glBindTexture(GL_TEXTURE_2D, att_type);
	glBegin(GL_QUADS);
		glTexCoord2f( 0.5,0.5);glVertex2f(  0,50);
		glTexCoord2f(   1,0.5);glVertex2f(100,50);
		glTexCoord2f(	1,  0);glVertex2f(100, 0);
		glTexCoord2f( 0.5,  0);glVertex2f(  0, 0);
	glEnd();
	//ØSÆ˙√¿ª(state = SEL_MEMBER)
	glTranslatef(125,0,0);
	glBindTexture(GL_TEXTURE_2D, att_type);
	glBegin(GL_QUADS);
		glTexCoord2f(   0,  0.5);glVertex2f(  0,50);
		glTexCoord2f( 0.5,  0.5);glVertex2f(100,50);
		glTexCoord2f( 0.5,    0);glVertex2f(100, 0);
		glTexCoord2f(   0,    0);glVertex2f(  0, 0);
	glEnd();
	glPopMatrix();


	if(state == SEL_ATT){
		static int frame_count = 0;
			glPushMatrix();
		if(!special){//¥∂≥qß¿ª(state = SEL_ATT)
			glTranslatef(168,548,0);
			glBindTexture(GL_TEXTURE_2D, att_type);
			glBegin(GL_QUADS);
				glTexCoord2f( 0.5,   1);glVertex2f(  0,50);
				glTexCoord2f(   1,   1);glVertex2f(100,50);
				glTexCoord2f(	1, 0.5);glVertex2f(100, 0);
				glTexCoord2f( 0.5, 0.5);glVertex2f(  0, 0);
			glEnd();
		}

		
		else{		//ØSÆ˙√¿ª(state = SEL_ATT)
			glTranslatef(293,548,0);
			glBindTexture(GL_TEXTURE_2D, att_type);
			glBegin(GL_QUADS);
				glTexCoord2f(   0,   1);glVertex2f(  0,50);
				glTexCoord2f( 0.5,   1);glVertex2f(100,50);
				glTexCoord2f( 0.5, 0.5);glVertex2f(100, 0);
				glTexCoord2f(   0, 0.5);glVertex2f(  0, 0);
			glEnd();
		}

		//øÅE‹Æÿ
		if(special)
			glTranslatef(-10,-3,0);
		else
			glTranslatef(-5,-3,0);
		glBindTexture(GL_TEXTURE_2D, sel_frame2);
		glBegin(GL_QUADS);
			glTexCoord2f( (frame_count%2)*0.5,    (frame_count/2)*0.5+0.5);glVertex2f(  0,55);
			glTexCoord2f( (frame_count%2)*0.5+0.5,(frame_count/2)*0.5+0.5);glVertex2f(115,55);
			glTexCoord2f( (frame_count%2)*0.5+0.5,(frame_count/2)*0.5)	  ;glVertex2f(115, 0);
			glTexCoord2f( (frame_count%2)*0.5,    (frame_count/2)*0.5)	  ;glVertex2f(  0, 0);
		glEnd();
		frame_count = (++frame_count)%4;

		glPopMatrix();
	}
}

void FightTurn::check_dead(Enemy *enemys, Friend *friends){
	int live_count = 0;
	int member = 0;
	for(int i = 0;i < 8 ;i++){
		if(friend_list[i] == -1)
			break;
		member = friend_list[i];
		if( friends[member].getvalue(HPvalue).now > 0 ){
			live_count++;
			break;
		}
	}

	if(live_count == 0){
		state = BATTLE_OVER;
		return;
	}
	live_count = 0;
	for(int i = 0;i < 8 ;i++){
		if(enemy_list[i] == -1)
			break;
		member = enemy_list[i];
		if( enemys[member].getvalue(HPvalue).now > 0){
			live_count++;
			break;
		}
	}
	if(live_count == 0){
		state = BATTLE_OVER;
		return;
	}

}
void FightTurn::round_init(Enemy *enemys, Friend *friends){
	value_cal(enemys, friends);
	special = false;
	e_special = false;
	round--;
	enemy_select = -1;
	select = 0;

	for(int i = 0;i < 4;i++)
		result_table[i] = -1;

	if(round == 0)
		state = BATTLE_OVER;
	else
		state = SEL_MEMBER;

	if(state == SEL_MEMBER)
		check_dead(enemys, friends);

}

void FightTurn::over(Enemy *enemys, Friend *friends){
	static int timer = 150;

	glBindTexture(GL_TEXTURE_2D, battle_over);
	glPushMatrix();
	glTranslatef( 200, 200, 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f( 0, 1);glVertex2f(  0,100);
		glTexCoord2f( 1, 1);glVertex2f(400,100);
		glTexCoord2f( 1, 0);glVertex2f(400,  0);
		glTexCoord2f( 0, 0);glVertex2f(  0,  0);
	glEnd();
	glPopMatrix();
	timer--;
	if(timer == 0){
		fighting = false;
		timer = 150 ;
		value_reset(enemys, friends);
	}
}

void FightTurn::value_reset(Enemy *enemys, Friend *friends){
	int member = 0;
	Value v;

	for(int i = 0; i< 8;i++){
		if(friend_list[i] == -1)
			break;
		member = friend_list[i];
		v = friends[member].getvalue(ATTvalue);
		friends[member].setvalue(ATTvalue, v.org, v.org);
		v = friends[member].getvalue(EVvalue);
		friends[member].setvalue(EVvalue, v.org, v.org);
		v = friends[member].getvalue(HITvalue);
		friends[member].setvalue(HITvalue, v.org, v.org);
		v = friends[member].getvalue(CTvalue);
		friends[member].setvalue(CTvalue, v.org, v.org);
	}

	for(int i = 0; i< 8;i++){
		if(enemy_list[i] == -1)
			break;
		member = enemy_list[i];
		v = enemys[member].getvalue(ATTvalue);
		enemys[member].setvalue(ATTvalue, v.org, v.org);
		v = enemys[member].getvalue(EVvalue);
		enemys[member].setvalue(EVvalue, v.org, v.org);
		v = enemys[member].getvalue(HITvalue);
		enemys[member].setvalue(HITvalue, v.org, v.org);
		v = enemys[member].getvalue(CTvalue);
		enemys[member].setvalue(CTvalue, v.org, v.org);
	}

	for(int i = 0; i< 8;i++){
		if(enemy_list[i] == -1)
			break;
		member = enemy_list[i];
		if( enemys[member].get<int>(SPECIAL) == RECOVER){
			v = enemys[member].getvalue(HPvalue);
			v.now += 30;
			if(v.now > v.org)
				v.now = v.org;
			enemys[member].setvalue(EVvalue, v.org, v.now);
		}
	}
}
float FightTurn::cal_probability(int ev, int hit){
	float total_ev = 0.0;

	if(hit == 0)
		total_ev = 1.0;
	else
		total_ev = (float)ev / (float)hit;

	return total_ev;
}

bool FightTurn::draw_action(Enemy *enemys, Friend *friends){
	float posx[4] = {0}, posy[4] = {0};
	float pos[4] = {0};
	int width, height;

	//if(enemys[enemy_list[enemy_select]].get<int>(TYPE) == 3)
	//	e_special = true;

	bool actionover = actioncontrol(posx, posy, enemys, friends);

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, motion_pic[enemys[enemy_list[enemy_select]].get<int>(TYPE)]);
	cpudrawpos(pos, edrawstate, width, height, false);
	glTranslatef( posx[0], posy[0], 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f( pos[2], pos[0]);glVertex2f(    0,height);
		glTexCoord2f( pos[3], pos[0]);glVertex2f(width,height);
		glTexCoord2f( pos[3], pos[1]);glVertex2f(width,     0);
		glTexCoord2f( pos[2], pos[1]);glVertex2f(    0,     0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, motion_pic[friends[friend_list[select]].get<int>(TYPE)]);
	cpudrawpos(pos, fdrawstate, width, height, false);
	glTranslatef( posx[2], posy[2], 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f( pos[2], pos[0]);glVertex2f(width,height);
		glTexCoord2f( pos[3], pos[0]);glVertex2f(    0,height);
		glTexCoord2f( pos[3], pos[1]);glVertex2f(    0,     0);
		glTexCoord2f( pos[2], pos[1]);glVertex2f(width,     0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, special_att);
	cpudrawpos(pos, eeffectstate, width, height, false);
	glTranslatef( posx[1], posy[1], 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f( pos[2], pos[0]);glVertex2f(    0,     0);
		glTexCoord2f( pos[3], pos[0]);glVertex2f(width,     0);
		glTexCoord2f( pos[3], pos[1]);glVertex2f(width,height);
		glTexCoord2f( pos[2], pos[1]);glVertex2f(    0,height);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, special_att);
	cpudrawpos(pos, feffectstate, width, height, actionover);
	glTranslatef( posx[3], posy[3], 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f( pos[2], pos[0]);glVertex2f(width,     0);
		glTexCoord2f( pos[3], pos[0]);glVertex2f(    0,     0);
		glTexCoord2f( pos[3], pos[1]);glVertex2f(    0,height);
		glTexCoord2f( pos[2], pos[1]);glVertex2f(width,height);
	glEnd();
	glPopMatrix();

	return actionover;
}

//posx 0: edrawstate   1: eeffectstate   2: fdrawstate   3: feffectstate
bool FightTurn::actioncontrol(float posx[], float posy[], Enemy *enemys, Friend *friends){
	static int tmpposx[4] = {500, 500, 150, 200}, tmpposy[4] = {260, 260, 260, 260};
	static int time = 0;
	static int starttime[4] = {0};
	int ftype = friends[friend_list[select]].get<int>(TYPE), etype = enemys[enemy_list[enemy_select]].get<int>(TYPE);
	int fspecialtype = friends[friend_list[select]].get<int>(SPECIAL), especialtype = enemys[enemy_list[enemy_select]].get<int>(SPECIAL);
	int attackdistance;

	if(time == 0){
		tmpposx[0] = 440;
		tmpposx[2] = 170;
		if(e_special || special){
			eeffectstate = (e_special)? FLASH: NOTSHOW;
			starttime[1] = time;
			feffectstate = (special)? FLASH: NOTSHOW;
			starttime[3] = time;
			
			edrawstate = IDLE;starttime[0] = time;
			fdrawstate = IDLE;starttime[2] = time;
			//return true;
		}else{
			switch(ftype){
			case 0:
			case 1:fdrawstate = FORWARD;starttime[2] = time;break;
			case 2:
			case 3:fdrawstate = ATTACK;starttime[2] = time;break;
			}

			switch(etype){
			case 0:
			case 1:edrawstate = FORWARD;starttime[0] = time;break;
			case 2:
			case 3:edrawstate = ATTACK;starttime[0] = time;break;
			}
			eeffectstate = NOTSHOW;
			starttime[1] = time;
			feffectstate = NOTSHOW;
			starttime[3] = time;
		}
		if(result_table[4] == 0){
			fdrawstate = IDLE;starttime[2] = time;
			feffectstate = NOTSHOW;
		}
		if(result_table[5] == 0){
			edrawstate = IDLE;starttime[0] = time;
			eeffectstate = NOTSHOW;
		}
	}

	if(edrawstate == FORWARD)tmpposx[0] -= 2;
	if(fdrawstate == FORWARD)tmpposx[2] += 2;
	if(eeffectstate == FARATTACK || eeffectstate == FARSPECIAL)tmpposx[1] -= 3;
	if(feffectstate == FARATTACK || feffectstate == FARSPECIAL)tmpposx[3] += 3;
	
	for(int i = 0; i < 4; ++i){
		posx[i] = tmpposx[i];
		posy[i] = tmpposy[i];
	}

	switch(fdrawstate){
	case IDLE:			if(posx[0] - posx[2] < 120 && special && fspecialtype == WALL && edrawstate == ATTACK){
							fdrawstate = SPECIALATTACK;
							starttime[2] = time;
							feffectstate = (ftype == 0)? NEARSPECIAL: NOTSHOW;
							starttime[3] = time;
						}
						if(time - starttime[2] == 500){
							time = 0;
							return true;
						}break;
	case FORWARD:		switch(ftype){
						case 0:attackdistance = -70;break;
						case 1:attackdistance = 120;break;
						}
						if(posx[0] - posx[2] < attackdistance){
							fdrawstate = (special)? SPECIALATTACK: ATTACK;
							starttime[2] = time;
							if(ftype == 1 && etype == 0){
								edrawstate = IDLE;
								starttime[0] = time;
							}
							feffectstate = (special && ftype == 0)? NEARSPECIAL: NOTSHOW;
							starttime[3] = time;
						}break;
	case ATTACK:		if(time - starttime[2] == 40)
							switch(ftype){
							case 1:	feffectstate = MIDDLEATTACK;starttime[3] = time;break;
							case 2:
							case 3:	feffectstate = FARATTACK;starttime[3] = time;
									tmpposx[3] = tmpposx[2] + 30;
									posx[3] = tmpposx[3];
									posx[2] = tmpposx[2];
									break;
							}
						if(time - starttime[2] == 80){
							if(ftype == 0){
								if(etype == 0 && edrawstate == ATTACK){
									fdrawstate = (result_table[1])? DODGE: HURT;
									starttime[2] = time;
								}else{
									fdrawstate = IDLE;
									starttime[2] = time;
								}
								edrawstate = (result_table[0])? HURT: DODGE;
								starttime[0] = time;
							}else{
								fdrawstate = IDLE;
								starttime[2] = time;
							}
						}break;
	case SPECIALATTACK:	if(fspecialtype == BEAM && time - starttime[2] == 1){
							feffectstate = LOCK;
							starttime[3] = time;
						}
						if(time - starttime[2] == 40)
							switch(fspecialtype){
							case SERVICE:if(etype >= 2){
											feffectstate = FARSPECIAL;starttime[3] = time;
											tmpposx[3] = tmpposx[2] + 100;
											posx[3] = tmpposx[3];
											posx[2] = tmpposx[2];
										}
							case WALL:  if(etype == 1){
										feffectstate = MIDDLESPECIAL;
										starttime[3] = time;
										}break;
							case SHADOW:
							case NIGHT:	feffectstate = FARSPECIAL;starttime[3] = time;
										tmpposx[3] = tmpposx[2] + 100;
										posx[3] = tmpposx[3];
										posx[2] = tmpposx[2];
										break;
							case BEAM:	feffectstate = BEAMSPECIAL;starttime[3] = time;
										break;
							}
						if(time - starttime[2] == 78 && fspecialtype == WALL){	
							if(etype == 0 && edrawstate == ATTACK){
								fdrawstate = (result_table[1])? DODGE: HURT;
								starttime[2] = time;
							}else{
								fdrawstate = IDLE;
								starttime[2] = time;
							}
							edrawstate = (result_table[0])? HURT: DODGE;
							starttime[0] = time;
							feffectstate = NOTSHOW;
						}
						if(time - starttime[2] == 80){
							if(fspecialtype == SERVICE && etype == 0){
								edrawstate = HURT;
								starttime[0] = time;
							}								
							fdrawstate = IDLE;
							starttime[2] = time;
						}
						break;
	case DODGE:			if(time - starttime[2] == 80)
							if(e_special || special){
								if(e_special)
									switch(especialtype){
									case BEAM:		
									case SHADOW:	if(result_table[4]){
														fdrawstate = (ftype <= 1) ? FORWARD : ATTACK;
														starttime[2] = time;
													}
													if(special){
														if(fspecialtype == SERVICE)
															fdrawstate = SPECIALATTACK;
														else{
															time = 0;return true;
														}
													}
													break;
									case WALL:		time = 0;return true;
									case SERVICE:	fdrawstate = SPECIALATTACK;starttime[2] = time;break;
									//case NIGHT:		
									}
								else{
									if(fspecialtype == SERVICE){
										fdrawstate = SPECIALATTACK;starttime[2] = time;
									}else{
										time = 0;return true;
									}
								}
							}else{
								if(ftype < etype && ftype <= 1 && result_table[4]){
									fdrawstate = FORWARD;
									starttime[2] = time;
								}else{
									time = 0;
									return true;
								}
							}break;
	case HURT:			if(time - starttime[2] == 80)
							if(e_special || special){
								if(e_special)
									switch(especialtype){
									case BEAM:		if(result_table[4] && result_table[2] == 1){
														fdrawstate = (ftype <= 1) ? FORWARD : ATTACK;
														starttime[2] = time;
													}else{
														time = 0;return true;
													}
													if(special){
														time = 0;return true;
													}
													break;
									case SHADOW:
									case SERVICE:
									case NIGHT:
									case WALL:		time = 0;return true;	
									}
								else{
									time = 0;return true;
								}
							}else{
								if(ftype < etype && ftype <= 1 && result_table[2] == 1 && result_table[4]){
									fdrawstate = FORWARD;
									starttime[2] = time;
								}else{
									time = 0;
									return true;
								}
							}
							break;
	}
	
	switch(edrawstate){
	case IDLE:			if(posx[0] - posx[2] < 120 && e_special && especialtype == WALL && fdrawstate == ATTACK){
							edrawstate = SPECIALATTACK;
							starttime[0] = time;
							eeffectstate = (etype == 0)? NEARSPECIAL: NOTSHOW;
							starttime[1] = time;
						}
						if(time - starttime[0] == 500){
							time = 0;
							return true;
						}break;
	case FORWARD:		switch(etype){
						case 0:attackdistance = -70;break;
						case 1:attackdistance = 120;break;
						}
						if(posx[0] - posx[2] < attackdistance){
							edrawstate = (e_special)? SPECIALATTACK: ATTACK;
							starttime[0] = time;
							if(etype == 1 && ftype == 0){
								fdrawstate = IDLE;
								starttime[2] = time;
							}
							eeffectstate =(e_special && etype == 0)? NEARSPECIAL: NOTSHOW;
							starttime[1] = time;
						}break;
	case ATTACK:		if(time - starttime[0] == 40)
							switch(etype){
							case 1:	eeffectstate = MIDDLEATTACK;starttime[1] = time;break;
							case 2:
							case 3:	eeffectstate = FARATTACK;starttime[1] = time;
									tmpposx[1] = tmpposx[0] + 70;
									posx[1] = tmpposx[1];
									posx[0] = tmpposx[0];
									break;
							}
						if(time - starttime[0] == 80){
							if(etype == 0){
								if(ftype == 0 && fdrawstate == ATTACK){
									edrawstate = (result_table[0])? HURT: DODGE;
									starttime[0] = time;
								}else{
									edrawstate = IDLE;
									starttime[0] = time;
								}
								fdrawstate = (result_table[1])? DODGE: HURT;
								starttime[2] = time;
							}else{
								edrawstate = IDLE;
								starttime[0] = time;
							}
						}break;
	case SPECIALATTACK:	if(especialtype == BEAM && time - starttime[0] == 1){
							eeffectstate = LOCK;
							starttime[1] = time;
						}
						if(time - starttime[0] == 40)
							switch(especialtype){
							case SERVICE:if(ftype >= 2){
											eeffectstate = FARSPECIAL;starttime[1] = time;
											tmpposx[1] = tmpposx[0] + 70;
											posx[1] = tmpposx[1];
											posx[0] = tmpposx[0];
										}
							case WALL:  if(ftype == 1){
											eeffectstate = MIDDLESPECIAL;
											starttime[1] = time;
										}
										break;
							case SHADOW:
							case NIGHT:	eeffectstate = FARSPECIAL;starttime[1] = time;
										tmpposx[1] = tmpposx[0] + 70;
										posx[1] = tmpposx[1];
										posx[0] = tmpposx[0];
										break;
							case BEAM:	eeffectstate = BEAMSPECIAL;starttime[1] = time;
										break;
							}
						if(time - starttime[0] == 78 && especialtype == WALL){	
							if(ftype == 0 && fdrawstate == ATTACK){
								edrawstate = (result_table[0])? HURT: DODGE;
								starttime[0] = time;
							}else{
								edrawstate = IDLE;
								starttime[0] = time;
							}
							fdrawstate = (result_table[1])? DODGE: HURT;
							starttime[2] = time;
							eeffectstate = NOTSHOW;
						}
						if(time - starttime[0] == 80){
							if(especialtype == SERVICE && ftype == 0){
								fdrawstate = HURT;
								starttime[2] = time;
							}								
							edrawstate = IDLE;
							starttime[0] = time;
						}break;
	case DODGE:			if(time - starttime[0] == 80)
							if(e_special || special){
								if(special)
									switch(fspecialtype){
									case BEAM:		
									case SHADOW:	if(result_table[5]){
														edrawstate = (etype <= 1) ? FORWARD : ATTACK;
														starttime[0] = time;
													}
													if(e_special){
														if(especialtype == SERVICE)
															edrawstate = SPECIALATTACK;
														else{
															time = 0;return true;
														}
													}
													break;
									case WALL:		time = 0;return true;
									case SERVICE:	edrawstate = SPECIALATTACK;starttime[0] = time;break;
									//case NIGHT:		
									}
								else{
									if(especialtype == SERVICE){
										edrawstate = SPECIALATTACK;starttime[0] = time;
									}else{
										time = 0;return true;
									}
								}
							}else{
								if(etype < ftype && etype <= 1 && result_table[5]){
									edrawstate = FORWARD;
									starttime[0] = time;
								}else{
									time = 0;
									return true;
								}
							}break;
	case HURT:			if(time - starttime[0] == 80)
							if(e_special || special){
								if(special)
									switch(fspecialtype){
									case BEAM:		if(result_table[5] && result_table[3] == 1){
														edrawstate = (etype <= 1) ? FORWARD : ATTACK;
														starttime[0] = time;
													}else{
														time = 0;return true;
													}
													if(e_special){
														time = 0;return true;
													}
													break;
									case SHADOW:
									case SERVICE:
									case NIGHT:
									case WALL:		time = 0;return true;	
									}
								else{
									time = 0;return true;
								}
							}else{
								if(etype < ftype && etype <= 1 && result_table[3] == 1 && result_table[5]){
									edrawstate = FORWARD;
									starttime[0] = time;
								}else{
									time = 0;
									return true;
								}
							}break;
	}

	switch(feffectstate){
	case FLASH: posx[3] = posx[2] - 65; posy[3] = posy[2];
				if(time - starttime[3] == 70){
					switch(fspecialtype){
					case WALL:case SERVICE:
							fdrawstate = IDLE;
							starttime[2] = time;
							if(!e_special && result_table[5]){
								switch(etype){
								case 0:
								case 1:edrawstate = FORWARD;starttime[0] = time;break;
								case 2:
								case 3:edrawstate = ATTACK;starttime[0] = time;break;
								}
							}
							feffectstate = NOTSHOW;
							break;
					case SHADOW:case NIGHT:
							fdrawstate = SPECIALATTACK;
							starttime[2] = time;
							if(ftype == 0){
								feffectstate = NEARSPECIAL;
								starttime[3] = time;
							}else
								feffectstate = NOTSHOW;
							break;
					case BEAM:	
							if(e_special && (especialtype == SHADOW || especialtype == NIGHT) && result_table[5]){
								fdrawstate = IDLE;
								starttime[2] = time;
							}else{
								fdrawstate = SPECIALATTACK;
								starttime[2] = time;
							}
							feffectstate = NOTSHOW;
							break;
					default:time = 0;return true;
					}
				}
				break;
	case NEARSPECIAL: posx[3] = posx[2]; posy[3] = posy[2] - 100; break;
	case MIDDLESPECIAL: posx[3] = posx[2] + 100; posy[3] = posy[2]; 
						if(time - starttime[3] == 70){
							feffectstate = NOTSHOW;
							edrawstate = (result_table[0])? HURT: DODGE;
							starttime[0] = time;
						}break;
	case MIDDLEATTACK:	posx[3] = posx[2] + 100; posy[3] = posy[2];
						if(time - starttime[3] == 70){
							feffectstate = NOTSHOW;
							edrawstate = (result_table[0])? HURT: DODGE;
							starttime[0] = time;
						}break;
	case FARATTACK: if(posx[3] < posx[2] + 30) posx[3] = posx[2] + 30;
					if(posx[0] - posx[3]  < -30 && posx[0] - posx[3]  > -130)
						if(result_table[0] == 1){
							feffectstate = NOTSHOW;
							edrawstate = HURT;
							starttime[0] = time;
						}else{
							edrawstate = DODGE;
							starttime[0] = time;
						}
					posy[3] = posy[2] + 50; 
					if(posx[3] > 560) feffectstate = NOTSHOW;break;
	case FARSPECIAL:if(posx[3] < posx[2] + 100) posx[3] = posx[2] + 100;
					if(posx[0] - posx[3]  < -30 && posx[0] - posx[3]  > -200)
						if(result_table[0] == 1){
							feffectstate = NOTSHOW;
							edrawstate = HURT;
							starttime[0] = time;
						}else{
							edrawstate = DODGE;
							starttime[0] = time;
						}
					posy[3] = posy[2] + 20;  
					if(posx[3] > 650) feffectstate = NOTSHOW;break;
	case BEAMSPECIAL: posx[3] = 160; posy[3] = 280;
					  if(time - starttime[3] == 70){
						(result_table[0] == 1) ? edrawstate = HURT : edrawstate = DODGE;
							feffectstate = NOTSHOW;
							starttime[0] = time;
					  }
					  break;
	case LOCK:	posx[3] = posx[0] + 100; posy[3] = posy[0]; break;
	}

	switch(eeffectstate){
	case FLASH: posx[1] = posx[0] + 115; posy[1] = posy[0];
				if(time - starttime[1] == 70){
					switch(especialtype){
					case WALL:case SERVICE:	
							edrawstate = IDLE;
							starttime[0] = time;
							if(!special && result_table[4]){
								switch(ftype){
								case 0:
								case 1:fdrawstate = FORWARD;starttime[2] = time;break;
								case 2:
								case 3:fdrawstate = ATTACK;starttime[2] = time;break;
								}
							}
							eeffectstate = NOTSHOW;
							break;
					case SHADOW:case NIGHT:	
							edrawstate = SPECIALATTACK;
							starttime[0] = time;
							if(etype == 0){
								eeffectstate = NEARSPECIAL;
								starttime[1] = time;
							}else
								eeffectstate = NOTSHOW;
							break;
					case BEAM:	
							if(special && (fspecialtype == SHADOW || fspecialtype == NIGHT) && result_table[4]){
								edrawstate = IDLE;
								starttime[0] = time;
							}else{
								edrawstate = SPECIALATTACK;
								starttime[0] = time;
							}
							eeffectstate = NOTSHOW;
							break;
					default:time = 0;return true;
					}
				}
				break;
	case NEARSPECIAL: posx[1] = posx[0]; posy[1] = posy[0] - 100; break;
	case MIDDLESPECIAL: posx[1] = posx[0] - 100; posy[1] = posy[0];
						if(time - starttime[1] == 70){
							eeffectstate = NOTSHOW;
							fdrawstate = (result_table[1])? DODGE: HURT;
							starttime[2] = time;
						}break;
	case MIDDLEATTACK:	posx[1] = posx[0] - 100; posy[1] = posy[0];
						if(time - starttime[1] == 70){
							eeffectstate = NOTSHOW;
							fdrawstate = (result_table[1])? DODGE: HURT;
							starttime[2] = time;
						}break;
	case FARATTACK: if(posx[1] > posx[0] + 70) posx[1] = posx[0] + 70;
					if(posx[1] - posx[2]  < 70 && posx[1] - posx[2]  > -30)
						if(result_table[1] == 0){
							eeffectstate = NOTSHOW;
							fdrawstate = HURT;
							starttime[2] = time;
						}else{
							fdrawstate = DODGE;
							starttime[2] = time;
						}
					posy[1] = posy[0] + 50; 
					if(posx[1] < 120) eeffectstate = NOTSHOW;break;
	case FARSPECIAL:if(posx[1] > posx[0]) posx[1] = posx[0];
					if(posx[1] - posx[2]  < 70 && posx[1] - posx[2]  > -100)
						if(result_table[1] == 0){
							eeffectstate = NOTSHOW;
							fdrawstate = HURT;
							starttime[2] = time;
						}else{
							fdrawstate = DODGE;
							starttime[2] = time;
						}
					posy[1] = posy[0] + 20; 
					if(posx[1] < 50) eeffectstate = NOTSHOW;break;
	case BEAMSPECIAL: posx[1] = 160; posy[1] = 280; 
					  if(time - starttime[1] == 70){
						(result_table[1] == 0) ? fdrawstate = HURT : fdrawstate = DODGE;
						eeffectstate = NOTSHOW;
						starttime[2] = time;	
					  }
					  break;
	case LOCK:	posx[1] = posx[2]; posy[1] = posy[2]; break;
	}

	++time;
	return false;
}

void FightTurn::cpudrawpos(float pos[], int nowstate, int &width, int &height, bool clear){
	static int slow[4] = {0};
	static int prestate[4] = {0};
	static int count[4] = {0};
	static int n = 0;
	static int time[4] = {1, 1, 1, 1};

	if(prestate[n] != nowstate){
		prestate[n] = nowstate;
		count[n] = 0;
		switch(nowstate){
		case NOTSHOW:
		case LOCK:
		case IDLE:			
		case DODGE:			
		case HURT:			time[n] = 1;break;
		case FORWARD:		time[n] = 6;break;
		case ATTACK:		
		case SPECIALATTACK:	
		case FLASH:			
		case NEARSPECIAL:	
		case MIDDLEATTACK:	
		case MIDDLESPECIAL:	
		case BEAMSPECIAL:	time[n] = 8;break;
		case FARATTACK:		
		case FARSPECIAL:	time[n] = 4;break;
		}
	}

	width = 200;
	height = 100;
	switch(nowstate){
	//Soldier
	case IDLE:			pos[0] = 0.125;pos[1] = 0;pos[2] = 0.25;pos[3] = 0.5;break;
	case DODGE:			pos[0] = 0.25;pos[1] = 0.125;pos[2] = 0.25;pos[3] = 0.5;break;
	case HURT:			pos[0] = 0.375;pos[1] = 0.25;pos[2] = 0.25;pos[3] = 0.5;break;
	case FORWARD:		pos[0] = 0.125 + 0.125 * count[n];pos[1] = 0.125 * count[n];pos[2] = 0;pos[3] = 0.25;break;
	case ATTACK:		pos[0] = 0.125 + 0.125 * count[n];pos[1] = 0.125 * count[n];pos[2] = 0.5;pos[3] = 0.75;break;
	case SPECIALATTACK:	pos[0] = 0.125 + 0.125 * count[n];pos[1] = 0.125 * count[n];pos[2] = 0.75;pos[3] = 1;break;
	//EFFECT
	case FLASH:			width = 150;height = 150;
						pos[0] = 1 - 0.125 * count[n];pos[1] = 0.875 - 0.125 * count[n];pos[2] = 0;pos[3] = 0.125;break;
	case NEARSPECIAL:	width = 200;height = 200;
						pos[0] = 1 - 0.125 * count[n];pos[1] = 0.875 - 0.125 * count[n];pos[2] = 0.125;pos[3] = 0.25;break;
	case MIDDLEATTACK:	width = 200;height = 100;
						pos[0] = 1 - 0.0625 * count[n];pos[1] = 0.9375 - 0.0625 * count[n];pos[2] = 0.25;pos[3] = 0.375;break;
	case MIDDLESPECIAL:	width = 200;height = 100;
						pos[0] = 0.5 - 0.0625 * count[n];pos[1] = 0.4375 - 0.0625 * count[n];pos[2] = 0.25;pos[3] = 0.375;break;
	case BEAMSPECIAL:	width = 480;height = 60;
						pos[0] = 1 - 0.0625 * count[n];pos[1] = 0.9375 - 0.0625 * count[n];pos[2] = 0.5;pos[3] = 1;break;
	case FARATTACK:		width = 100;height = 100;
						pos[0] = 0.75 - 0.0625 * count[n];pos[1] = 0.6875 - 0.0625 * count[n];pos[2] = 0.375;pos[3] = 0.4375;break;
	case FARSPECIAL:	width = 100;height = 100;
						pos[0] = 1 - 0.0625 * count[n];pos[1] = 0.9375 - 0.0625 * count[n];pos[2] = 0.375;pos[3] = 0.4375;break;
	case LOCK:			width = 100;height = 100;
						pos[0] = 1;pos[1] = 0.9375;pos[2] = 0.4375;pos[3] = 0.5;break;
	case NOTSHOW:		width = 0;height = 0;pos[0] = 0;pos[1] = 0;pos[2] = 0;pos[3] = 0;break;
	}
	if(slow[n] == 0)
		count[n] = (++count[n]) % time[n];
	slow[n] = (++slow[n]) % 10;
	n = (++n) % 4;
	
	if(clear){
		for(int i = 0; i < 4; ++i){
			slow[i] = 0;
			prestate[i] = 0;
			count[i] = 0;
			time[i] = 1;
		}
		n = 0;
	}
}

void FightTurn::draw_sel_man(Enemy *enemys, Friend *friends){
	int member = 0, member_data = 0;
	float pic_temp = 0.04883;
	int v_temp = 0;
	int v_org = 0;
	int sel_temp = 0;
	Rect r;

	if(select >= 0 && friend_list[select] != -1 ){
		member = friend_list[select];
		r = map->getdata( friends[member].get<int>(POSX), friends[member].get<int>(POSY) );

		//©R§§≤v
		glPushMatrix();
		glTranslatef( 165.0, 0.0, 0.0);
		v_temp = friends[member].getvalue(HITvalue).now;
		v_org = friends[member].getvalue(HITvalue).org;
	
		glBindTexture(GL_TEXTURE_2D, Num_Table2);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.6348, 1);glVertex2f( 0,40 );
			glTexCoord2f( 0.6973, 1);glVertex2f(40,40 );
			glTexCoord2f( 0.6973, 0);glVertex2f(40, 0 );
			glTexCoord2f( 0.6348, 0);glVertex2f( 0, 0 );
		glEnd();

		if(v_temp > v_org )
			glColor3ub(255,0,0);
		else if(v_temp < v_org )
			glColor3ub(0,255,0);
		glTranslatef( 85.0, 0.0, 0.0);
		for(int j=0;j < 3;j++){
			member_data = v_temp%10;
			glBegin(GL_QUADS);
				glTexCoord2f( pic_temp*member_data	   , 1);glVertex2f( 0,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 1);glVertex2f(25,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 0);glVertex2f(25, 0 );
				glTexCoord2f( pic_temp*member_data	   , 0);glVertex2f( 0, 0 );
			glEnd();
			v_temp = v_temp/10;
			glTranslatef(-20, 0.0, 0.0);
		}
		glColor3ub(255,255,255);
		//draw '%'
		glTranslatef(83, 0.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.4882, 1);glVertex2f( 0,40 );
			glTexCoord2f( 0.5371, 1);glVertex2f(25,40 );
			glTexCoord2f( 0.5371, 0);glVertex2f(25, 0 );
			glTexCoord2f( 0.4882, 0);glVertex2f( 0, 0 );
		glEnd();

		glTranslatef(20, 0.0, 0.0);
		if(r.HIT_rate >= 0){		//draw '+'
			glBegin(GL_QUADS);
				glTexCoord2f( 0.5371, 1);glVertex2f( 0,40 );
				glTexCoord2f( 0.5859, 1);glVertex2f(25,40 );
				glTexCoord2f( 0.5859, 0);glVertex2f(25, 0 );
				glTexCoord2f( 0.5371, 0);glVertex2f( 0, 0 );
			glEnd();
		}
		else{						//draw '-'
			glBegin(GL_QUADS);
				glTexCoord2f( 0.5859, 1);glVertex2f( 0,40 );
				glTexCoord2f( 0.6347, 1);glVertex2f(25,40 );
				glTexCoord2f( 0.6347, 0);glVertex2f(25, 0 );
				glTexCoord2f( 0.5859, 0);glVertex2f( 0, 0 );
			glEnd();
		}

		glTranslatef(40, 0.0, 0.0);
		v_temp = r.HIT_rate;
		for(int j=0;j < 2;j++){
			member_data = v_temp%10;
			if(member_data < 0)
				member_data = member_data*-1;
			glBegin(GL_QUADS);
				glTexCoord2f( pic_temp*member_data		   , 1);glVertex2f( 0,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 1);glVertex2f(25,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 0);glVertex2f(25, 0 );
				glTexCoord2f( pic_temp*member_data	       , 0);glVertex2f( 0, 0 );
			glEnd();
			v_temp = v_temp/10;
			glTranslatef(-20, 0.0, 0.0);
		}
		//draw '%'
		glTranslatef(60, 0.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.4882, 1);glVertex2f( 0,40 );
			glTexCoord2f( 0.5371, 1);glVertex2f(25,40 );
			glTexCoord2f( 0.5371, 0);glVertex2f(25, 0 );
			glTexCoord2f( 0.4882, 0);glVertex2f( 0, 0 );
		glEnd();
		glPopMatrix();
		//∞{¡◊≤v
		glPushMatrix();
		v_temp = friends[member].getvalue(EVvalue).now;
		v_org = friends[member].getvalue(EVvalue).org;
		glTranslatef( 165, 40.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.6973, 1);glVertex2f( 0,40 );
			glTexCoord2f( 0.7598, 1);glVertex2f(40,40 );
			glTexCoord2f( 0.7598, 0);glVertex2f(40, 0 );
			glTexCoord2f( 0.6973, 0);glVertex2f( 0, 0 );
		glEnd();

		if(v_temp > v_org )
			glColor3ub(255,0,0);
		else if(v_temp < v_org)
			glColor3ub(0,255,0);
		glTranslatef( 85, 0.0, 0.0);
		for(int j=0;j < 3;j++){
			member_data = v_temp%10;
			glBegin(GL_QUADS);
				glTexCoord2f( pic_temp*member_data	   , 1);glVertex2f( 0,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 1);glVertex2f(25,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 0);glVertex2f(25, 0 );
				glTexCoord2f( pic_temp*member_data	   , 0);glVertex2f( 0, 0 );
			glEnd();
			v_temp = v_temp/10;
			glTranslatef(-20, 0.0, 0.0);
		}
		glColor3ub(255,255,255);
		//draw '%'
		glTranslatef(83, 0.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.4882, 1);glVertex2f( 0,40 );
			glTexCoord2f( 0.5371, 1);glVertex2f(25,40 );
			glTexCoord2f( 0.5371, 0);glVertex2f(25, 0 );
			glTexCoord2f( 0.4882, 0);glVertex2f( 0, 0 );
		glEnd();

		glTranslatef(20, 0.0, 0.0);
		if(r.EV_rate >= 0){		//draw '+'
			glBegin(GL_QUADS);
				glTexCoord2f( 0.5371, 1);glVertex2f( 0,40 );
				glTexCoord2f( 0.5859, 1);glVertex2f(25,40 );
				glTexCoord2f( 0.5859, 0);glVertex2f(25, 0 );
				glTexCoord2f( 0.5371, 0);glVertex2f( 0, 0 );
			glEnd();
		}
		else{						//draw '-'
			glBegin(GL_QUADS);
				glTexCoord2f( 0.5859, 1);glVertex2f( 0,40 );
				glTexCoord2f( 0.6347, 1);glVertex2f(25,40 );
				glTexCoord2f( 0.6347, 0);glVertex2f(25, 0 );
				glTexCoord2f( 0.5859, 0);glVertex2f( 0, 0 );
			glEnd();
		}

		glTranslatef(40, 0.0, 0.0);
		v_temp = r.EV_rate;
		for(int j=0;j < 2;j++){
			member_data = v_temp%10;
			glBegin(GL_QUADS);
				glTexCoord2f( pic_temp*member_data	   , 1);glVertex2f( 0,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 1);glVertex2f(25,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 0);glVertex2f(25, 0 );
				glTexCoord2f( pic_temp*member_data	   , 0);glVertex2f( 0, 0 );
			glEnd();
			v_temp = v_temp/10;
			glTranslatef(-20, 0.0, 0.0);
		}
		//draw '%'
		glTranslatef(60, 0.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.4882, 1);glVertex2f( 0,40 );
			glTexCoord2f( 0.5371, 1);glVertex2f(25,40 );
			glTexCoord2f( 0.5371, 0);glVertex2f(25, 0 );
			glTexCoord2f( 0.4882, 0);glVertex2f( 0, 0 );
		glEnd();
		glPopMatrix();
		//§œ¿ª≤v
		glPushMatrix();
		v_temp = friends[member].getvalue(CTvalue).now;
		v_org = friends[member].getvalue(CTvalue).org;
		glTranslatef( 165, 80.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.7598, 1);glVertex2f( 0,40 );
			glTexCoord2f( 0.8223, 1);glVertex2f(40,40 );
			glTexCoord2f( 0.8223, 0);glVertex2f(40, 0 );
			glTexCoord2f( 0.7598, 0);glVertex2f( 0, 0 );
		glEnd();

		if(v_temp > v_org )
			glColor3ub(255,0,0);
		else if(v_temp < v_org )
			glColor3ub(0,255,0);
		else 
			glColor3ub(255,255,255);
		glTranslatef( 85, 0.0, 0.0);
		for(int j=0;j < 3;j++){
			member_data = v_temp%10;
			glBegin(GL_QUADS);
				glTexCoord2f( pic_temp*member_data	   , 1);glVertex2f( 0,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 1);glVertex2f(25,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 0);glVertex2f(25, 0 );
				glTexCoord2f( pic_temp*member_data	   , 0);glVertex2f( 0, 0 );
			glEnd();
			v_temp = v_temp/10;
			glTranslatef(-20, 0.0, 0.0);
		}
		glColor3ub(255,255,255);
		//draw '%'
		glTranslatef(83, 0.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.4882, 1);glVertex2f( 0,40 );
			glTexCoord2f( 0.5371, 1);glVertex2f(25,40 );
			glTexCoord2f( 0.5371, 0);glVertex2f(25, 0 );
			glTexCoord2f( 0.4882, 0);glVertex2f( 0, 0 );
		glEnd();

		glPopMatrix();

		//draw skin
		glPushMatrix();
		glTranslatef( 120, 370, 0.0);
		member_data = (friends[member].get<int>(TYPE));
		glBindTexture(GL_TEXTURE_2D, friend_skin2);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.25*(float)member_data	  , 1);glVertex2f(  0,180 );
			glTexCoord2f( 0.25*(float)member_data+0.25, 1);glVertex2f(180,180 );
			glTexCoord2f( 0.25*(float)member_data+0.25, 0);glVertex2f(180,  0 );
			glTexCoord2f( 0.25*(float)member_data	  , 0);glVertex2f(  0,  0 );
		glEnd();

		//DRAW SPECIAL
		glTranslatef( 160, 5.0, 0.0);
		member_data = friends[member].get<int>(SPECIAL)-1;
		glBindTexture(GL_TEXTURE_2D, special_detail);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.125*(float)member_data		, 1);glVertex2f(  0,170 );
			glTexCoord2f( 0.125*(float)member_data+0.125, 1);glVertex2f(120,170 );
			glTexCoord2f( 0.125*(float)member_data+0.125, 0);glVertex2f(120,  0 );
			glTexCoord2f( 0.125*(float)member_data		, 0);glVertex2f(  0,  0 );
		glEnd();
		
		glPopMatrix();
		
	}
	if(select < 0 || state==4 ){
		if(state == SEL_MEMBER)
			sel_temp = (select+1)*-1;
		else
			sel_temp = enemy_select;

		member = enemy_list[ sel_temp ];
		if(member == -1)
			return;
		r = map->getdata( enemys[member].get<int>(POSX), enemys[member].get<int>(POSY) );
		
		//©R§§≤v
		glPushMatrix();
		glTranslatef( 410.0, 0.0, 0.0);
		v_temp = enemys[member].getvalue(HITvalue).now;
		v_org = enemys[member].getvalue(HITvalue).org;
		glBindTexture(GL_TEXTURE_2D, Num_Table2);
		
		glBegin(GL_QUADS);
			glTexCoord2f( 0.6348, 1);glVertex2f( 0,40 );
			glTexCoord2f( 0.6973, 1);glVertex2f(40,40 );
			glTexCoord2f( 0.6973, 0);glVertex2f(40, 0 );
			glTexCoord2f( 0.6348, 0);glVertex2f( 0, 0 );
		glEnd();

		if(v_temp > v_org )
			glColor3ub(255,0,0);
		else if(v_temp < v_org )
			glColor3ub(0,255,0);
		glTranslatef( 85.0, 0.0, 0.0);
		for(int j=0;j < 3;j++){
			member_data = v_temp%10;
			glBegin(GL_QUADS);
				glTexCoord2f( pic_temp*member_data	   , 1);glVertex2f( 0,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 1);glVertex2f(25,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 0);glVertex2f(25, 0 );
				glTexCoord2f( pic_temp*member_data	   , 0);glVertex2f( 0, 0 );
			glEnd();
			v_temp = v_temp/10;
			glTranslatef(-20, 0.0, 0.0);
		}
		glColor3ub(255,255,255);
		//draw '%'
		glTranslatef(83, 0.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.4882, 1);glVertex2f( 0,40 );
			glTexCoord2f( 0.5371, 1);glVertex2f(25,40 );
			glTexCoord2f( 0.5371, 0);glVertex2f(25, 0 );
			glTexCoord2f( 0.4882, 0);glVertex2f( 0, 0 );
		glEnd();

		glTranslatef(20, 0.0, 0.0);
		if(r.HIT_rate >= 0){		//draw '+'
			glBegin(GL_QUADS);
				glTexCoord2f( 0.5371, 1);glVertex2f( 0,40 );
				glTexCoord2f( 0.5859, 1);glVertex2f(25,40 );
				glTexCoord2f( 0.5859, 0);glVertex2f(25, 0 );
				glTexCoord2f( 0.5371, 0);glVertex2f( 0, 0 );
			glEnd();
		}
		else{						//draw '-'
			glBegin(GL_QUADS);
				glTexCoord2f( 0.5859, 1);glVertex2f( 0,40 );
				glTexCoord2f( 0.6347, 1);glVertex2f(25,40 );
				glTexCoord2f( 0.6347, 0);glVertex2f(25, 0 );
				glTexCoord2f( 0.5859, 0);glVertex2f( 0, 0 );
			glEnd();
		}

		glTranslatef(40, 0.0, 0.0);
		v_temp = r.HIT_rate;
		for(int j=0;j < 2;j++){
			member_data = v_temp%10;
			glBegin(GL_QUADS);
				glTexCoord2f( pic_temp*member_data	   , 1);glVertex2f( 0,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 1);glVertex2f(25,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 0);glVertex2f(25, 0 );
				glTexCoord2f( pic_temp*member_data	   , 0);glVertex2f( 0, 0 );
			glEnd();
			v_temp = v_temp/10;
			glTranslatef(-20, 0.0, 0.0);
		}
		//draw '%'
		glTranslatef(60, 0.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.4882, 1);glVertex2f( 0,40 );
			glTexCoord2f( 0.5371, 1);glVertex2f(25,40 );
			glTexCoord2f( 0.5371, 0);glVertex2f(25, 0 );
			glTexCoord2f( 0.4882, 0);glVertex2f( 0, 0 );
		glEnd();
		glPopMatrix();
		//∞{¡◊≤v
		glPushMatrix();
		v_temp = enemys[member].getvalue(EVvalue).now;
		v_org = enemys[member].getvalue(EVvalue).org;
		glTranslatef( 410.0, 40.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.6973, 1);glVertex2f( 0,40 );
			glTexCoord2f( 0.7598, 1);glVertex2f(40,40 );
			glTexCoord2f( 0.7598, 0);glVertex2f(40, 0 );
			glTexCoord2f( 0.6973, 0);glVertex2f( 0, 0 );
		glEnd();


		if(v_temp > v_org )
			glColor3ub(255,0,0);
		else if(v_temp < v_org )
			glColor3ub(0,255,0);
		glTranslatef( 85.0, 0.0, 0.0);
		for(int j=0;j < 3;j++){
			member_data = v_temp%10;
			glBegin(GL_QUADS);
				glTexCoord2f( pic_temp*member_data	   , 1);glVertex2f( 0,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 1);glVertex2f(25,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 0);glVertex2f(25, 0 );
				glTexCoord2f( pic_temp*member_data	   , 0);glVertex2f( 0, 0 );
			glEnd();
			v_temp = v_temp/10;
			glTranslatef(-20, 0.0, 0.0);
		}
		glColor3ub(255,255,255);
		//draw '%'
		glTranslatef(83, 0.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.4882, 1);glVertex2f( 0,40 );
			glTexCoord2f( 0.5371, 1);glVertex2f(25,40 );
			glTexCoord2f( 0.5371, 0);glVertex2f(25, 0 );
			glTexCoord2f( 0.4882, 0);glVertex2f( 0, 0 );
		glEnd();

		glTranslatef(20, 0.0, 0.0);
		if(r.EV_rate >= 0){		//draw '+'
			glBegin(GL_QUADS);
				glTexCoord2f( 0.5371, 1);glVertex2f( 0,40 );
				glTexCoord2f( 0.5859, 1);glVertex2f(25,40 );
				glTexCoord2f( 0.5859, 0);glVertex2f(25, 0 );
				glTexCoord2f( 0.5371, 0);glVertex2f( 0, 0 );
			glEnd();
		}
		else{						//draw '-'
			glBegin(GL_QUADS);
				glTexCoord2f( 0.5859, 1);glVertex2f( 0,40 );
				glTexCoord2f( 0.6347, 1);glVertex2f(25,40 );
				glTexCoord2f( 0.6347, 0);glVertex2f(25, 0 );
				glTexCoord2f( 0.5859, 0);glVertex2f( 0, 0 );
			glEnd();
		}

		glTranslatef(40, 0.0, 0.0);
		v_temp = r.EV_rate;
		for(int j=0;j < 2;j++){
			member_data = v_temp%10;
			glBegin(GL_QUADS);
				glTexCoord2f( pic_temp*member_data	   , 1);glVertex2f( 0,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 1);glVertex2f(25,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 0);glVertex2f(25, 0 );
				glTexCoord2f( pic_temp*member_data	   , 0);glVertex2f( 0, 0 );
			glEnd();
			v_temp = v_temp/10;
			glTranslatef(-20, 0.0, 0.0);
		}
		//draw '%'
		glTranslatef(60, 0.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.4882, 1);glVertex2f( 0,40 );
			glTexCoord2f( 0.5371, 1);glVertex2f(25,40 );
			glTexCoord2f( 0.5371, 0);glVertex2f(25, 0 );
			glTexCoord2f( 0.4882, 0);glVertex2f( 0, 0 );
		glEnd();
		glPopMatrix();
		//§œ¿ª≤v
		glPushMatrix();
		v_temp = enemys[member].getvalue(CTvalue).now;
		v_org = enemys[member].getvalue(CTvalue).org;

		glTranslatef( 410.0, 80.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.7598, 1);glVertex2f( 0,40 );
			glTexCoord2f( 0.8223, 1);glVertex2f(40,40 );
			glTexCoord2f( 0.8223, 0);glVertex2f(40, 0 );
			glTexCoord2f( 0.7598, 0);glVertex2f( 0, 0 );
		glEnd();

		if(v_temp > v_org )
			glColor3ub(255,0,0);
		else if(v_temp < v_org )
			glColor3ub(0,255,0);
		glTranslatef( 85.0, 0.0, 0.0);
		for(int j=0;j < 3;j++){
			member_data = v_temp%10;
			glBegin(GL_QUADS);
				glTexCoord2f( pic_temp*member_data	   , 1);glVertex2f( 0,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 1);glVertex2f(25,40 );
				glTexCoord2f( pic_temp*member_data+pic_temp, 0);glVertex2f(25, 0 );
				glTexCoord2f( pic_temp*member_data	   , 0);glVertex2f( 0, 0 );
			glEnd();
			v_temp = v_temp/10;
			glTranslatef(-20, 0.0, 0.0);
		}
		glColor3ub(255,255,255);
		//draw '%'
		glTranslatef(83, 0.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.4882, 1);glVertex2f( 0,40 );
			glTexCoord2f( 0.5371, 1);glVertex2f(25,40 );
			glTexCoord2f( 0.5371, 0);glVertex2f(25, 0 );
			glTexCoord2f( 0.4882, 0);glVertex2f( 0, 0 );
		glEnd();

		glPopMatrix();
		//draw skin
		glPushMatrix();
		glTranslatef( 470, 370, 0.0);

		member_data = (enemys[member].get<int>(TYPE));
		glBindTexture(GL_TEXTURE_2D, friend_skin2);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.25*(float)member_data	  , 1);glVertex2f(  0,180 );
			glTexCoord2f( 0.25*(float)member_data+0.25, 1);glVertex2f(180,180 );
			glTexCoord2f( 0.25*(float)member_data+0.25, 0);glVertex2f(180,  0 );
			glTexCoord2f( 0.25*(float)member_data	  , 0);glVertex2f(  0,  0 );
		glEnd();

		//DRAW SPECIAL
		glTranslatef( -70, 5.0, 0.0);
		member_data = enemys[member].get<int>(SPECIAL)-1;
		if(member_data != -1){
			glBindTexture(GL_TEXTURE_2D, special_detail);
			glBegin(GL_QUADS);
				glTexCoord2f( 0.125*(float)member_data		, 1);glVertex2f(  0,170 );
				glTexCoord2f( 0.125*(float)member_data+0.125, 1);glVertex2f(120,170 );
				glTexCoord2f( 0.125*(float)member_data+0.125, 0);glVertex2f(120,  0 );
				glTexCoord2f( 0.125*(float)member_data		, 0);glVertex2f(  0,  0 );
			glEnd();
		}
		glPopMatrix();
	}
}


void FightTurn::draw_background(GLuint bg){
	static float bg_frame1 = 0.0, bg_frame2 = 0.0;
	//≠I¥∫1
	glPushMatrix();
	glTranslatef( 162, 120, 0.0);
	glBindTexture(GL_TEXTURE_2D, f_bg1[bg]);
	glBegin(GL_QUADS);
		glTexCoord2f( bg_frame1	   , 1);glVertex2f(  0,  0);
		glTexCoord2f( bg_frame1+0.5, 1);glVertex2f(476,  0);
		glTexCoord2f( bg_frame1+0.5, 0);glVertex2f(476,200);
		glTexCoord2f( bg_frame1	   , 0);glVertex2f(  0,200);
	glEnd();
	bg_frame1 +=0.002;
	if(bg_frame1 == 0.5)
		bg_frame1 = 0;

	//≠I¥∫2
	glTranslatef( 0, 170, 0.0);
	glBindTexture(GL_TEXTURE_2D, f_bg2);
	glBegin(GL_QUADS);
		glTexCoord2f( bg_frame2	   , 1);glVertex2f(  0,85);
		glTexCoord2f( bg_frame2+0.5, 1);glVertex2f(476,85);
		glTexCoord2f( bg_frame2+0.5, 0);glVertex2f(476, 0);
		glTexCoord2f( bg_frame2	   , 0);glVertex2f(  0, 0);
	glEnd();
	bg_frame2 +=0.006;
	if(bg_frame2 == 0.5)
		bg_frame2 = 0;
	glPopMatrix();

}

void FightTurn::draw_all_member(Enemy *enemys, Friend *friends){
	int i = 0;
	int member = 0, member_data = 0;
	Value v,v_temp;

	//¶C•Xfriend list
	for(i = 0;i < 8 ;i++){
		if(friend_list[i] == -1)
			break;
		member = friend_list[i];


		//µe§H™´skin
		glPushMatrix();
		glTranslatef( 0.0, 75*i, 0.0);
		
		member_data = (friends[member].get<int>(TYPE));
		
		v_temp = friends[member].getvalue(HPvalue);
		if(v_temp.now == 0)
			glColor3ub(100,100,100);

		glBindTexture(GL_TEXTURE_2D, friend_skin);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.25*(float)member_data	  , 1);glVertex2f( 0, 0 );
			glTexCoord2f( 0.25*(float)member_data+0.25, 1);glVertex2f(75, 0 );
			glTexCoord2f( 0.25*(float)member_data+0.25, 0);glVertex2f(75,75 );
			glTexCoord2f( 0.25*(float)member_data	  , 0);glVertex2f( 0,75 );
		glEnd();

		glColor3ub(255,255,255);

		glTranslatef(50, 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, att_range);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.25*(float)member_data	  , 1);glVertex2f( 0,30 );
			glTexCoord2f( 0.25*(float)member_data+0.25, 1);glVertex2f(30,30 );
			glTexCoord2f( 0.25*(float)member_data+0.25, 0);glVertex2f(30, 0 );
			glTexCoord2f( 0.25*(float)member_data	  , 0);glVertex2f( 0, 0 );
		glEnd();
		glPopMatrix();

		//¶C•XACT≠»
		glPushMatrix();
		glTranslatef(80, 76*i, 0.0);
		glBindTexture(GL_TEXTURE_2D, Num_Table);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.3517, 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.4414, 1);glVertex2f(24,18 );
			glTexCoord2f( 0.4414, 0);glVertex2f(24, 0 );
			glTexCoord2f( 0.3517, 0);glVertex2f( 0, 0 );
		glEnd();
		glTranslatef(45, 0, 0.0);

		v = friends[member].getvalue(ACTvalue);

		glBegin(GL_QUADS);
			glTexCoord2f( 0.03125*(v.now)	   , 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.03125*(v.now)+0.03125, 1);glVertex2f(10,18 );
			glTexCoord2f( 0.03125*(v.now)+0.03125, 0);glVertex2f(10, 0 );
			glTexCoord2f( 0.03125*(v.now)	   , 0);glVertex2f( 0, 0 );
		glEnd();
		glTranslatef(10,0.0,0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.03125*10	   , 1);glVertex2f( 0,20 );
			glTexCoord2f( 0.03125*10+0.03125, 1);glVertex2f(10,20 );
			glTexCoord2f( 0.03125*10+0.03125, 0);glVertex2f(10, 0 );
			glTexCoord2f( 0.03125*10	   , 0);glVertex2f( 0, 0 );
		glEnd();
		glTranslatef(10,0.0,0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.03125*(v.org)	   , 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.03125*(v.org)+0.03125, 1);glVertex2f(10,18 );
			glTexCoord2f( 0.03125*(v.org)+0.03125, 0);glVertex2f(10, 0 );
			glTexCoord2f( 0.03125*(v.org)	   , 0);glVertex2f( 0, 0 );
		glEnd();

		//¶C•XLV≠»
		glTranslatef(-65, 19, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.4453, 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.5078, 1);glVertex2f(16,18 );
			glTexCoord2f( 0.5078, 0);glVertex2f(16, 0 );
			glTexCoord2f( 0.4453, 0);glVertex2f( 0, 0 );
		glEnd();
		glTranslatef(55,  0, 0.0);

		member_data = friends[member].get<int>(LVvalue);
		if(member_data/10 > 0){
		glBegin(GL_QUADS);
			glTexCoord2f( 0.03125*(member_data/10)		, 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.03125*(member_data/10)+0.03125, 1);glVertex2f(10,18 );
			glTexCoord2f( 0.03125*(member_data/10)+0.03125, 0);glVertex2f(10, 0 );
			glTexCoord2f( 0.03125*(member_data/10)	    , 0);glVertex2f( 0, 0 );
		glEnd();
		}
		glTranslatef(10, 0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.03125*(member_data%10)		, 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.03125*(member_data%10)+0.03125, 1);glVertex2f(10,18 );
			glTexCoord2f( 0.03125*(member_data%10)+0.03125, 0);glVertex2f(10, 0 );
			glTexCoord2f( 0.03125*(member_data%10)	    , 0);glVertex2f( 0, 0 );
		glEnd();




		//¶C•XHP≠»
		v = friends[member].getvalue(HPvalue);
		glTranslatef(-65, 19, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.5117, 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.5742, 1);glVertex2f(16,18 );
			glTexCoord2f( 0.5742, 0);glVertex2f(16, 0 );
			glTexCoord2f( 0.5117, 0);glVertex2f( 0, 0 );
		glEnd();
		glTranslatef(65, 0, 0.0);

		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, Num_Table);
		for(int j=0;j < 3;j++){
			member_data = v.org%10;
			glBegin(GL_QUADS);
				glTexCoord2f( 0.03125*member_data		 , 1);glVertex2f( 0,18 );
				glTexCoord2f( 0.03125*member_data+0.03125, 1);glVertex2f(8,18 );
				glTexCoord2f( 0.03125*member_data+0.03125, 0);glVertex2f(8, 0 );
				glTexCoord2f( 0.03125*member_data	     , 0);glVertex2f( 0, 0 );
			glEnd();
			v.org = v.org/10;
			glTranslatef(-8, 0.0, 0.0);
		}
		glTranslatef(5, 0.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.03125*10	   , 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.03125*10+0.03125, 1);glVertex2f(5,18 );
			glTexCoord2f( 0.03125*10+0.03125, 0);glVertex2f(5, 0 );
			glTexCoord2f( 0.03125*10	   , 0);glVertex2f( 0, 0 );
		glEnd();
		glTranslatef(-10,0.0,0.0);
		for(int j=0;j < 3;j++){
			member_data = v.now%10;
			glBegin(GL_QUADS);
				glTexCoord2f( 0.03125*member_data		 , 1);glVertex2f( 0,18 );
				glTexCoord2f( 0.03125*member_data+0.03125, 1);glVertex2f(8,18 );
				glTexCoord2f( 0.03125*member_data+0.03125, 0);glVertex2f(8, 0 );
				glTexCoord2f( 0.03125*member_data	     , 0);glVertex2f( 0, 0 );
			glEnd();
			v.now = v.now/10;
			glTranslatef(-8, 0.0, 0.0);
		}
		glPopMatrix();
		

		//¶C•XATT≠»
		v = friends[member].getvalue(ATTvalue);
		glTranslatef(-65, 18, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.5781, 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.6719, 1);glVertex2f(24,18 );
			glTexCoord2f( 0.6719, 0);glVertex2f(24, 0 );
			glTexCoord2f( 0.5781, 0);glVertex2f( 0, 0 );
		glEnd();
		glTranslatef(65, 1, 0.0);

		glPushMatrix();

		if(v.now > v.org)
				glColor3ub(255,0,0);
		else if(v.now < v.org)
			glColor3ub(0,150,0);
		else
			glColor3ub(0,0,0);
		glBindTexture(GL_TEXTURE_2D, Num_Table3);
		for(int j=0;j < 3;j++){
			member_data = v.now%10;
			glBegin(GL_QUADS);
				glTexCoord2f( 0.0625*member_data	   , 1);glVertex2f( 0,18 );
				glTexCoord2f( 0.0625*member_data+0.0625, 1);glVertex2f( 8,18 );
				glTexCoord2f( 0.0625*member_data+0.0625, 0);glVertex2f( 8, 0 );
				glTexCoord2f( 0.0625*member_data	   , 0);glVertex2f( 0, 0 );
			glEnd();
			v.now = v.now/10;
			glTranslatef(-8, 0.0, 0.0);
		}
		glColor3ub(255,255,255);
		glPopMatrix();

		glPopMatrix();
	}
	//¶C•Xenemy list
	for(i = 0;i < 8 ;i++){
		if(enemy_list[i] == -1)
			break;
		member = enemy_list[i];
		
		//µe§H™´skin
		glPushMatrix();
		glTranslatef( 639, 75*i, 0.0);

		v_temp = enemys[member].getvalue(HPvalue);
		if(v_temp.now == 0)
			glColor3ub(100,100,100);

		member_data = (enemys[member].get<int>(TYPE));
		glBindTexture(GL_TEXTURE_2D, friend_skin);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.25*(float)member_data	  , 1);glVertex2f( 0, 0 );
			glTexCoord2f( 0.25*(float)member_data+0.25, 1);glVertex2f(77, 0 );
			glTexCoord2f( 0.25*(float)member_data+0.25, 0);glVertex2f(77,75 );
			glTexCoord2f( 0.25*(float)member_data	  , 0);glVertex2f( 0,75 );
		glEnd();
		glColor3ub(255,255,255);
		glTranslatef(51, -2.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, att_range);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.25*(float)member_data	   , 1);glVertex2f( 0,30 );
			glTexCoord2f( 0.25*(float)member_data+0.25, 1);glVertex2f(30,30 );
			glTexCoord2f( 0.25*(float)member_data+0.25, 0);glVertex2f(30, 0 );
			glTexCoord2f( 0.25*(float)member_data	   , 0);glVertex2f( 0, 0 );
		glEnd();
		glPopMatrix();


		//¶C•XACT≠»
		glPushMatrix();
		v = enemys[member].getvalue(ACTvalue);
		glTranslatef(720, 76*i, 0.0);
		glBindTexture(GL_TEXTURE_2D, Num_Table);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.3517, 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.4414, 1);glVertex2f(24,18 );
			glTexCoord2f( 0.4414, 0);glVertex2f(24, 0 );
			glTexCoord2f( 0.3517, 0);glVertex2f( 0, 0 );
		glEnd();
		glTranslatef(45, 0, 0.0);
		glBindTexture(GL_TEXTURE_2D, Num_Table);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.03125*(v.now)	   , 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.03125*(v.now)+0.03125, 1);glVertex2f(10,18 );
			glTexCoord2f( 0.03125*(v.now)+0.03125, 0);glVertex2f(10, 0 );
			glTexCoord2f( 0.03125*(v.now)	   , 0);glVertex2f( 0, 0 );
		glEnd();
		glTranslatef(10,0.0,0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.03125*10	   , 1);glVertex2f( 0,20 );
			glTexCoord2f( 0.03125*10+0.03125, 1);glVertex2f(10,20 );
			glTexCoord2f( 0.03125*10+0.03125, 0);glVertex2f(10, 0 );
			glTexCoord2f( 0.03125*10	   , 0);glVertex2f( 0, 0 );
		glEnd();
		glTranslatef(10,0.0,0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.03125*(v.org)	   , 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.03125*(v.org)+0.03125, 1);glVertex2f(10,18 );
			glTexCoord2f( 0.03125*(v.org)+0.03125, 0);glVertex2f(10, 0 );
			glTexCoord2f( 0.03125*(v.org)	   , 0);glVertex2f( 0, 0 );
		glEnd();

		//¶C•XLV≠»
		glTranslatef(-65, 19, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.4453, 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.5078, 1);glVertex2f(16,18 );
			glTexCoord2f( 0.5078, 0);glVertex2f(16, 0 );
			glTexCoord2f( 0.4453, 0);glVertex2f( 0, 0 );
		glEnd();
		glTranslatef(55,  0, 0.0);

		member_data = enemys[member].get<int>(LVvalue);
		if(member_data/10 > 0){
		glBegin(GL_QUADS);
			glTexCoord2f( 0.03125*(member_data/10)		 , 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.03125*(member_data/10)+0.03125, 1);glVertex2f(10,18 );
			glTexCoord2f( 0.03125*(member_data/10)+0.03125, 0);glVertex2f(10, 0 );
			glTexCoord2f( 0.03125*(member_data/10)	     , 0);glVertex2f( 0, 0 );
		glEnd();
		}
		glTranslatef(10, 0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.03125*(member_data%10)		 , 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.03125*(member_data%10)+0.03125, 1);glVertex2f(10,18 );
			glTexCoord2f( 0.03125*(member_data%10)+0.03125, 0);glVertex2f(10, 0 );
			glTexCoord2f( 0.03125*(member_data%10)	     , 0);glVertex2f( 0, 0 );
		glEnd();

		//¶C•XHP≠»
		v = enemys[member].getvalue(HPvalue);
		glTranslatef(-65, 19, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.5117, 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.5742, 1);glVertex2f(16,18 );
			glTexCoord2f( 0.5742, 0);glVertex2f(16, 0 );
			glTexCoord2f( 0.5117, 0);glVertex2f( 0, 0 );
		glEnd();
		glTranslatef(65, 0, 0.0);

		glPushMatrix();
		for(int j=0;j < 3;j++){
			member_data = v.org%10;
			glBegin(GL_QUADS);
				glTexCoord2f( 0.03125*member_data		 , 1);glVertex2f( 0,18 );
				glTexCoord2f( 0.03125*member_data+0.03125, 1);glVertex2f(8,18 );
				glTexCoord2f( 0.03125*member_data+0.03125, 0);glVertex2f(8, 0 );
				glTexCoord2f( 0.03125*member_data	     , 0);glVertex2f( 0, 0 );
			glEnd();
			v.org = v.org/10;
			glTranslatef(-8, 0.0, 0.0);
		}
		glTranslatef(5, 0.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.03125*10	   , 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.03125*10+0.03125, 1);glVertex2f(5,18 );
			glTexCoord2f( 0.03125*10+0.03125, 0);glVertex2f(5, 0 );
			glTexCoord2f( 0.03125*10	   , 0);glVertex2f( 0, 0 );
		glEnd();
		glTranslatef(-10,0.0,0.0);
		for(int j=0;j < 3;j++){
			member_data = v.now%10;
			glBegin(GL_QUADS);
				glTexCoord2f( 0.03125*member_data		 , 1);glVertex2f( 0,18 );
				glTexCoord2f( 0.03125*member_data+0.03125, 1);glVertex2f(8,18 );
				glTexCoord2f( 0.03125*member_data+0.03125, 0);glVertex2f(8, 0 );
				glTexCoord2f( 0.03125*member_data	     , 0);glVertex2f( 0, 0 );
			glEnd();
			v.now = v.now/10;
			glTranslatef(-8, 0.0, 0.0);
		}
		glPopMatrix();
		

		//¶C•XATT≠»
		v = enemys[member].getvalue(ATTvalue);
		glTranslatef(-65, 18, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.5781, 1);glVertex2f( 0,18 );
			glTexCoord2f( 0.6719, 1);glVertex2f(24,18 );
			glTexCoord2f( 0.6719, 0);glVertex2f(24, 0 );
			glTexCoord2f( 0.5781, 0);glVertex2f( 0, 0 );
		glEnd();
		glTranslatef(65, 1, 0.0);
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, Num_Table3);
		if(v.now > v.org)
				glColor3ub(255,0,0);
		else if(v.now < v.org)
			glColor3ub(0,150,0);
		else
			glColor3ub(0,0,0);
		for(int j=0;j < 3;j++){
			member_data = v.now%10;
			glBegin(GL_QUADS);
				glTexCoord2f( 0.0625*member_data	   , 1);glVertex2f( 0,18 );
				glTexCoord2f( 0.0625*member_data+0.0625, 1);glVertex2f( 8,18 );
				glTexCoord2f( 0.0625*member_data+0.0625, 0);glVertex2f( 8, 0 );
				glTexCoord2f( 0.0625*member_data	   , 0);glVertex2f( 0, 0 );
			glEnd();
			v.now = v.now/10;
			glTranslatef(-10, 0.0, 0.0);
		}
		glColor3ub(255,255,255);
		glPopMatrix();

		glPopMatrix();
	}
}

void FightTurn::control(int key, Friend *friends){
	switch(state){
	case 0:
		switch(key){
		case UP: if(select > 0) select--;
				 else if(select < -1) select++;
				 break;
		case DOWN: if(select < 7 && select >= 0) select++;
				   else if(select > -8 && select <= -1)select--; 
				   break;
		case LEFT: if(select < 0) select = -1*select-1;break;
		case RIGHT: if(select >= 0) select = -1*select-1;break;
		case Z: if(friend_list[select] != -1 && select >= 0 && (friends[friend_list[select]].getvalue(HPvalue)).now != 0) state = SEL_ATT;break;
		case C: fighting = false;break;
		}
		break;
	case 1:
		switch(key){
		case RIGHT: special = true; break;
		case LEFT:  special = false;break;
		case Z: state = CHK_ACT;
				break;
		case X: state = SEL_MEMBER;
				special = false;
				break;
		}
		break;
	}

}

void FightTurn::value_cal(Enemy *enemys, Friend *friends){
	int member1 = 0, member2 = 0;
	Value f_att, f_hp;
	Value e_att, e_hp;
	Value temp;
	int exp;
	int f_type,e_type;

	member1 = friend_list[select];
	member2 = enemy_list[enemy_select];

	f_att = friends[member1].getvalue(ATTvalue);
	f_hp  = friends[member1].getvalue(HPvalue);
	e_att = enemys[member2].getvalue(ATTvalue);
	e_hp  = enemys[member2].getvalue(HPvalue);

	f_type = friends[member1].get<int>(TYPE);
	e_type = enemys[member2].get<int>(TYPE);
	if(e_type == 3)
		e_type = 2;

	if( f_type > e_type ){		//ª∑ vs ™ÅE
		if(result_table[0] == 1 && ( (friends[member1].get<int>(SPECIAL) != NIGHT || friends[member1].get<int>(SPECIAL) != SERVICE) || special != true) && result_table[4] == 1)
			if(friends[member1].get<int>(SPECIAL) != WALL || !special)
				e_hp.now = e_hp.now - f_att.now;

		if(result_table[1] == 0 && e_hp.now > 0 && result_table[5] == 1)
			if(friends[member1].get<int>(SPECIAL) != SHADOW || result_table[0] == 0 || (friends[member1].get<int>(SPECIAL) == SHADOW && special == false))
				if(friends[member1].get<int>(SPECIAL) == WALL && special){
					f_hp.now = f_hp.now - e_att.now*0.5;
					e_hp.now = e_hp.now - e_att.now*1.5;
				}
				else
					f_hp.now = f_hp.now - e_att.now;
	}
	
	if( f_type == e_type ){	//∂Z¬˜¨€¶P
		if(friends[member1].get<int>(SPECIAL) == SHADOW && special == true && result_table[0] == 1)
			e_hp.now = e_hp.now - f_att.now;

		if(e_special && result_table[1] == 0 && e_hp.now > 0){
			for(int i=0;i<8;i++){
				if(friend_list[i] == -1)
					break;
				if(friend_list[i] != member1){
					temp = friends[ friend_list[i] ].getvalue(HPvalue);
					friends[ friend_list[i] ].setvalue(HPvalue, temp.org , temp.now*0.5);
					if(temp.now == 1)
						friends[ friend_list[i] ].set<bool>(DEAD,true);
				}
			}
			f_hp.now = f_hp.now*0.5;
		}

		if(result_table[0] == 1 && (friends[member1].get<int>(SPECIAL) != SHADOW || special != true) && f_hp.now > 0 && 
				( (friends[member1].get<int>(SPECIAL) != NIGHT || friends[member1].get<int>(SPECIAL) != SERVICE) || special != true) && result_table[4] == 1)
			if(friends[member1].get<int>(SPECIAL) != WALL || !special)
				e_hp.now = e_hp.now - f_att.now;

		if(result_table[1] == 0 &&  e_special == false && result_table[5] == 1)
			if(friends[member1].get<int>(SPECIAL) == WALL && special){
				f_hp.now = f_hp.now - e_att.now*0.5;
				e_hp.now = e_hp.now - e_att.now*1.5;
			}
			else
				f_hp.now = f_hp.now - e_att.now;
	}

	if( f_type < e_type ){		//™ÅEvs ª∑
		if(result_table[0] == 1 && friends[member1].get<int>(SPECIAL) == SHADOW && special)
			e_hp.now = e_hp.now - f_att.now;

		if(result_table[1] == 0 && result_table[5] == 1)
			if(e_special){						//beam
				for(int i=0;i<8;i++){
					if(friend_list[i] == -1)
						break;
					if(friend_list[i] != member1){
						temp = friends[ friend_list[i] ].getvalue(HPvalue);
						friends[ friend_list[i] ].setvalue(HPvalue, temp.org , temp.now*0.5);
						if(temp.now == 1)
							friends[ friend_list[i] ].set<bool>(DEAD,true);
					}
				}
				f_hp.now = f_hp.now*0.5;
			}
			else
				if(friends[member1].get<int>(SPECIAL) == WALL && special && e_type < 2){
					f_hp.now = f_hp.now - e_att.now*0.5;
					e_hp.now = e_hp.now - e_att.now*1.5;
				}
				else
					f_hp.now = f_hp.now - e_att.now;
		
		if(result_table[0] == 1 && f_hp.now > 0 && (friends[member1].get<int>(SPECIAL) != SHADOW || !special) &&
			( (friends[member1].get<int>(SPECIAL) != NIGHT || friends[member1].get<int>(SPECIAL) != SERVICE) || special != true) && result_table[4] == 1)
			if(friends[member1].get<int>(SPECIAL) != WALL || !special)
				e_hp.now = e_hp.now - f_att.now;
	}

	if(e_hp.now <= 0){
		e_hp.now = 0;
		enemys[member2].set<bool>(DEAD, true);
		exp = friends[member1].get<int>(EXPvalue) + 1000;
		friends[member1].set<int>(EXPvalue , exp);
	}

	if(f_hp.now <= 0){
		f_hp.now = 0;
		friends[member1].set<bool>(DEAD, true);
	}
	enemys[member2].setvalue(HPvalue, e_hp.org , e_hp.now);
	friends[member1].setvalue(HPvalue, f_hp.org , f_hp.now);
}

void FightTurn::cal_result(Enemy *enemys, Friend *friends){
	int member1 = 0, member2 = 0;
	int v_temp1 = 0, v_temp2 = 0;
	float v_temp = 0;
	int temp = 0;
	Rect r1, r2;

	member1 = friend_list[select];
	member2 = enemy_list[enemy_select];
	r1 = map->getdata( friends[member1].get<int>(POSX), friends[member1].get<int>(POSY) );
	r2 = map->getdata( enemys[member2].get<int>(POSX), enemys[member2].get<int>(POSY) );

	if(result_table[0] == -1){
		//©R§§≤v
		v_temp1 = friends[member1].getvalue(HITvalue).now;
		v_temp1 += r1.HIT_rate;			//™±Æa©R§§≤v
		v_temp2 = enemys[member2].getvalue(EVvalue).now;
		v_temp2 += r2.EV_rate;			//πq∏£∞{¡◊≤v

		v_temp = 1 - cal_probability(v_temp2, v_temp1);

		temp = rand()%100;
		if(temp < v_temp*100)
			result_table[0] = 1;
		else
			result_table[0] = 0;
	}
	if(result_table[1] == -1){
		//∞{¡◊≤v
		v_temp1 = friends[member1].getvalue(EVvalue).now;
		v_temp1 += r1.EV_rate;				//™±Æa∞{¡◊≤v
		v_temp2 = enemys[member2].getvalue(HITvalue).now;
		v_temp2 += r2.HIT_rate;				//πq∏£©R§§≤v

		v_temp = cal_probability(v_temp1, v_temp2);

		temp = rand()%100;
		if(temp < v_temp*100)
			result_table[1] = 1;
		else
			result_table[1] = 0;
	}
	if(result_table[2] == -1){
		//™±Æa§œ¿ª
		v_temp1 = friends[member1].getvalue(HPvalue).now;
		v_temp2 = enemys[member2].getvalue(ATTvalue).now;
		if(v_temp1 <= v_temp2)
			result_table[2] = 0;
		else{
			v_temp1 = friends[member1].getvalue(CTvalue).now;
			temp = rand()%100;
			if(temp < v_temp1)
				result_table[2] = 1;
			else
				result_table[2] = 0;
		}
	}
	//πq∏£§œ¿ª
	if(result_table[3] == -1){
		v_temp1 = friends[member1].getvalue(ATTvalue).now;
		v_temp2 = enemys[member2].getvalue(HPvalue).now;
		if(v_temp2 <= v_temp1)
			result_table[3] = 0;
		else{
			v_temp2 = enemys[member2].getvalue(CTvalue).now;
			temp = rand()%100;
			if(temp < v_temp2)
				result_table[3] = 1;
			else
				result_table[3] = 0;
		}
	}
	cout<<result_table[0]<<result_table[1]<<result_table[2]<<result_table[3]<<endl;
}

void FightTurn::prev_special_effect(Enemy *enemys, Friend *friends){
	int member = 0;
	Value v;

	for(int i = 0;i<8;i++){
		if(friend_list[i] == -1)
			break;
		member = friend_list[i];
		if(friends[i].get<int>(SPECIAL) == BATTLECONTROL)
			for(int j = 0;j < 8;j++){											//ß⁄§Ë•˛≠ÅE
				if(friend_list[j] == -1)
					break;
				member = friend_list[j];
				v = friends[member].getvalue(EVvalue);
				v.now += 10;
				friends[member].setvalue(EVvalue, v.org, v.now );		//EV+10
		
				v = friends[member].getvalue(HITvalue);
				v.now += 10;
				friends[member].setvalue(HITvalue, v.org, v.now );	//HIT+10

				v = friends[member].getvalue(CTvalue);
				v.now += 10;
				friends[member].setvalue(CTvalue, v.org, v.now);		//CT+10
			}
	}
	
	for(int i = 0;i<8;i++){
		if(enemy_list[i] == -1)
			break;
		member = enemy_list[i];
		if(enemys[i].get<int>(SPECIAL) == HITUP)
			for(int j = 0;j < 8;j++){											//ß⁄§Ë•˛≠ÅE
				if(enemy_list[j] == -1)
					break;
				member = enemy_list[j];
				v = enemys[member].getvalue(HITvalue);
				v.now += 10;
				enemys[member].setvalue(HITvalue, v.org, v.now );	//HIT+10
			}
	}
}

bool FightTurn::special_effect(Enemy *enemys, Friend *friends){
	int member1 = 0, member2 = 0;
	int sp = 0, e_sp = 0;
	Value v1, v2;
	int n1 = 0, n2 = 0;

	member1 = friend_list[select];
	member2 = enemy_list[enemy_select];

	sp = friends[member1].get<int>(SPECIAL);
	e_sp = enemys[member2].get<int>(SPECIAL);

	switch(sp){ 
	case NONE:return false;
	case SHADOW:
			v1 = friends[member1].getvalue(ACTvalue);
			if(v1.now < 2)
				return false;
			friends[member1].setvalue(ACTvalue, v1.org, v1.now-2 );		
			v1 = friends[member1].getvalue(ATTvalue);
			friends[member1].setvalue(ATTvalue, v1.org, v1.now*1.5 );		//ß¿ª§O*1.5
			result_table[3] = 0;		//ºƒ§ËµL™k§œ¿ª
			break;
	case WALL:
			v1 = friends[member1].getvalue(ACTvalue);
			if(v1.now < 2)
				return false;
			friends[member1].setvalue(ACTvalue, v1.org, v1.now-2 );
			result_table[2] = 0;
			break;
	case SERVICE:
			v1 = friends[member1].getvalue(ACTvalue);
			if(v1.now < 5)
				return false;
			friends[member1].setvalue(ACTvalue, v1.org, v1.now-5 );
			for(int i = 0;i < 8;i++){											//ß⁄§Ë•˛≠ÅE
				if(friend_list[i] == -1)
					break;
				member1 = friend_list[i];
				v1 = friends[member1].getvalue(ATTvalue);
				friends[member1].setvalue(ATTvalue, v1.org, v1.now+30 );	//ß¿ª§O+30
				v1 = friends[member1].getvalue(EVvalue);
				friends[member1].setvalue(EVvalue, v1.org, v1.now+30 );		//EV+30
				v1 = friends[member1].getvalue(HITvalue);
				friends[member1].setvalue(HITvalue, v1.org, v1.now+30 );	//HIT+30
				v1 = friends[member1].getvalue(CTvalue);
				friends[member1].setvalue(CTvalue, v1.org, v1.now+30 );		//CT+30
			}
			result_table[0] = 1;											//µ¥πÅER§§
			result_table[1] = 1;											//µ¥πÅE{¡◊
			break;
	case NIGHT:
			v1 = friends[member1].getvalue(ACTvalue);
			if(v1.now < 2)
				return false;
			friends[member1].setvalue(ACTvalue, v1.org, v1.now-2 );
			for(int i = 0;i < 8;i++){											//ºƒ§Ë•˛≠ÅE
				if(enemy_list[i] == -1)
					break;
				member2 = enemy_list[i];
				v2 = enemys[member2].getvalue(ACTvalue);
				enemys[member2].setvalue(ATTvalue, v2.org, v2.now-1 );			//ACT -1
			}
			result_table[0] = 1;												//µ¥πÅER§§
			result_table[3] = 0;												//æ‘∞´∞j¡◊
			break;
	case BATTLECONTROL:return false;
	}
	if(e_special)
		switch(e_sp){
		case BEAM:
				v2 = enemys[member2].getvalue(ACTvalue);
				enemys[member2].setvalue(ACTvalue, v2.org, v2.now-7 );
				break;
		}
	return true;
}

void FightTurn::ai(Enemy *enemys, Friend *friends){
	int num = 0;
	int i;
	Value v;
	static int roundtimes = 0;
	if(Game::get(GAMETYPE) == NORMAL){
		for(i = 0; i < 8; ++i)
		if(enemy_list[i] != -1)
			num++;
		enemy_select = roundtimes % num;
		e_special = false;
		i = 0;
		v = enemys[enemy_list[enemy_select]].getvalue(ACTvalue);
		while(enemys[enemy_list[enemy_select]].get<int>(DEAD) == 1 || v.now <= 0){
			enemy_select = (++enemy_select) % num;
			v = enemys[enemy_list[enemy_select]].getvalue(ACTvalue);
			i++;
			if(i >= num)
				break;
		}
	}else{
		smartai(enemys, friends);
	}
	++roundtimes;
	if(round == 0)
		roundtimes = 0;
}

void FightTurn::smartai(Enemy *enemys, Friend *friends){
	int i = 0, j = 0, fnumber = 0, enumber = 0;
	int tmpHP[2][8] = {{0, 0}};
	int tmpACT[2][8] = {{0, 0}};
	int tmpCT[2][8] = {{0, 0}};
	int tmptype[2][8] = {{0, 0}};
	int tmpATT[2][8] = {{0, 0}};
	int tmpHIT[2][8] = {{0, 0}};
	int tmpEV[2][8] = {{0, 0}};
	float posHIT[8][8] = {{0, 0}};
	float posEV[8][8] = {{0, 0}};
	Value v;
	for(i = 0; i < 8; i++){
		if(enemy_list[i] != -1){
			enumber++;
			v = enemys[enemy_list[i]].getvalue(HPvalue);
			tmpHP[0][i] = v.now;
			v = enemys[enemy_list[i]].getvalue(ACTvalue);
			tmpACT[0][i] = v.now;
			v = enemys[enemy_list[i]].getvalue(CTvalue);
			tmpCT[0][i] = v.now;
			v = enemys[enemy_list[i]].getvalue(ATTvalue);
			tmpATT[0][i] = v.now;
			v = enemys[enemy_list[i]].getvalue(HITvalue);
			tmpHIT[0][i] = v.now;
			v = enemys[enemy_list[i]].getvalue(EVvalue);
			tmpEV[0][i] = v.now;
			tmptype[0][i] = enemys[enemy_list[i]].get<int>(TYPE);
		}
		if(friend_list[i] != -1){
			fnumber++;
			v = friends[friend_list[i]].getvalue(HPvalue);
			tmpHP[1][i] = v.now;
			v = friends[friend_list[i]].getvalue(ACTvalue);
			tmpACT[1][i] = v.now;
			v = friends[friend_list[i]].getvalue(CTvalue);
			tmpCT[1][i] = v.now;
			v = friends[friend_list[i]].getvalue(ATTvalue);
			tmpATT[1][i] = v.now;
			v = friends[friend_list[i]].getvalue(HITvalue);
			tmpHIT[1][i] = v.now;
			v = friends[friend_list[i]].getvalue(EVvalue);
			tmpEV[1][i] = v.now;
			tmptype[1][i] = friends[friend_list[i]].get<int>(TYPE);
		}
	}
	for(i = 0; i < enumber; ++i)
		if(tmpHP[0][i] != 0)
			for(j = 0; j < fnumber; ++j)
				if(tmpHP[1][j] != 0){
					if(tmpEV[1][j] != 0)
						posHIT[i][j] = 1.0 - cal_probability(tmpEV[1][j], tmpHIT[0][i]);
					else
						posHIT[i][j] = 1;
					if(tmpEV[0][i] != 0)
						posEV[i][j] = cal_probability(tmpEV[0][i], tmpHIT[1][j]);
					else
						posEV[i][j] = 0;
				}
	int tmpavg = 0;
	int avgHP = 0;
	int avgATT = 0;
	int avgHIT = 0;
	int avgEV = 0;

	for(i = 0; i < enumber; ++i)
		if(tmpHP[0][i] > 0){
			tmpavg++;
			avgHP += tmpHP[0][i];
			avgATT += tmpATT[0][i];
			avgHIT += tmpHIT[0][i];
			avgEV += tmpEV[0][i];
		}
	avgHP /= tmpavg;
	avgATT /= tmpavg;
	avgHIT /= tmpavg;
	avgEV /= tmpavg;


	int pos_fselect[5] = {0};
	int eselect[5] = {-1, -1, -1, -1, -1};
	
	int bestACT = 0;						//ß‰¶Ê∞ ≠»≥Ã§j  •B±j™Ã
	while(tmpHP[1][bestACT] <= 0)
		++bestACT;
	for(j = bestACT + 1; j < fnumber; ++j)
		if(tmpHP[1][j] > 0)
			if(tmpATT[1][j] > avgHP && cal_probability(avgEV, tmpHIT[1][j]) < 0.5 && tmpHP[1][j] > avgATT)
				if(tmpACT[1][j] > tmpACT[1][bestACT])
					bestACT = j;
	pos_fselect[0] = bestACT;

	int besttype = 0;					//ß‰ª∑∂Z¬˜´¨™∫  •B©R§§≤v∞™ ¶≥¶Ê∞ ≠»
	while(tmpHP[1][besttype] <= 0)
		++besttype;
	for(j = besttype + 1; j < fnumber; ++j)
		if(tmpHP[1][j] > 0)
			if(cal_probability(avgEV, tmpHIT[1][j]) < 0.5 && tmpACT[1][j] > 0)
				if(tmptype[1][j] > tmptype[1][besttype] || tmpACT[1][besttype] == 0)
					besttype = j;
	pos_fselect[1] = besttype;

	int bestCT = 0;					//ß‰§œ¿ª≤v∞™ •B¶Â¶h ¶≥¶Ê∞ ≠»
	while(tmpHP[1][bestCT] <= 0)
		++bestCT;
	for(j = bestCT + 1; j < fnumber; ++j)
		if(tmpHP[1][j] > 0)
			if(tmpHP[1][j] > avgATT && tmpACT[1][j] > 0)
				if(tmpCT[1][j] > tmpCT[1][bestCT] || tmpACT[1][besttype] == 0)
					bestCT = j;
	pos_fselect[2] = bestCT;

	int bestEV = 0;					//ß‰∞{¡◊≤v∞™ •B¶≥¶Ê∞ ≠»
	while(tmpHP[1][bestEV] <= 0)
		++bestEV;
	for(j = bestEV + 1; j < fnumber; ++j)
		if(tmpHP[1][j] > 0)
			if(tmpACT[1][j] > 0)
				if(tmpEV[1][j] > tmpEV[1][bestEV] || tmpACT[1][besttype] == 0)
					bestEV = j;
	pos_fselect[3] = bestEV;

	int bestATT = 0;					//ß‰ß¿ª§O∞™ •B¶Â¶h ¶≥¶Ê∞ ≠»
	while(tmpHP[1][bestATT] <= 0)
		++bestATT;
	for(j = bestATT + 1; j < fnumber; ++j)
		if(tmpHP[1][j] > 0)
			if(tmpHP[1][j] > avgATT && tmpACT[1][j] > 0 && cal_probability(avgEV, tmpHIT[1][j]) < 0.5)
				if(tmpATT[1][j] > tmpATT[1][bestATT] || tmpACT[1][besttype] == 0)
					bestATT = j;
	pos_fselect[4] = bestATT;

	int weight[5] = {0};
	weight[0] = 25;
	weight[1] = 40;
	weight[2] = 10;
	weight[3] = 10;
	weight[4] = 15;
	if(tmptype[1][besttype] == 0){
		weight[1] = 10;
		weight[2] = 20;
		weight[3] = 20;
		weight[4] = 20;
	}
	
	int rate = rand() % 100;
	int expectfselect = 0;
	for(i = 0; i < 5; ++i){
		rate -= weight[i];
		if(rate < 0){
			expectfselect = i;
			break;
		}
	}
	cout<<"expectfselect: "<<pos_fselect[expectfselect]<<endl;


	besttype = 0;					//ß‰type§Ò™±Æa¶n™∫  •B©R§§≤v∞™ ¶≥¶Ê∞ ≠»
	i = 0;
	while(tmpHP[0][besttype] <= 0 || tmptype[0][besttype] <= tmptype[1][pos_fselect[expectfselect]]){
		besttype = (++besttype) % enumber;
		if(i++ >= enumber)
			break;
	}
	for(j = besttype + 1; j < enumber; ++j)
		if(tmpHP[0][j] > 0)
			if(tmptype[0][j] > tmptype[1][pos_fselect[expectfselect]] && tmpACT[0][j] > 0 && posHIT[j][pos_fselect[expectfselect]] > 0.5)
				if(tmptype[0][j] < tmptype[0][besttype] || tmpACT[0][besttype] <= 0)
					besttype = j;
	pos_fselect[0] = besttype;

	bestCT = 0;					//ß‰§œ¿ª≤v∞™ •B¶≥¶Ê∞ ≠» ¶≥¶ÅE
	while(tmpHP[0][bestCT] <= 0)
		++bestCT;
	for(j = bestCT + 1; j < enumber; ++j)
		if(tmpHP[0][j] > tmpATT[1][pos_fselect[expectfselect]])
			if(tmpACT[0][j] > 0)
				if(tmpCT[0][j] > tmpCT[0][bestCT] || tmpACT[0][besttype] <= 0)
					bestCT = j;
	pos_fselect[1] = bestCT;

	bestEV = 0;					//ß‰∞{¡◊≤v∞™ •B¶≥¶Ê∞ ≠»
	while(tmpHP[0][bestEV] <= 0)
		++bestEV;
	for(j = bestEV + 1; j < enumber; ++j)
		if(tmpHP[0][j] > 0)
			if(tmpACT[0][j] > 0)
				if(tmpEV[0][j] > tmpEV[0][bestEV] || tmpACT[0][bestEV] <= 0)
					bestEV = j;
	pos_fselect[2] = bestEV;

	int weakest = 0;					//ß‰≥ÃÆz
	while(tmpHP[0][weakest] <= 0)
		++weakest;
	for(j = weakest + 1; j < enumber; ++j)
		if(tmpHP[0][j] < tmpATT[1][pos_fselect[expectfselect]] && tmpHP[0][j] > 0)
			if(tmptype[0][j] < tmptype[0][weakest] && tmpATT[0][j] < tmpATT[0][weakest])
				weakest = j;
	pos_fselect[3] = weakest;

	bestATT = 0;					//ß‰ß¿ª§O∞™ •B¶Â¶h ¶≥¶Ê∞ ≠»
	while(tmpHP[0][bestATT] <= 0)
		++bestATT;
	for(j = bestATT + 1; j < enumber; ++j)
		if(tmpHP[0][j] > tmpATT[1][pos_fselect[expectfselect]])
			if(tmpACT[0][j] > 0)
				if(tmpATT[0][j] > tmpATT[0][bestATT] || tmpACT[0][bestEV] <= 0)
					bestATT = j;
	pos_fselect[4] = bestATT;

	weight[0] = 40;
	weight[1] = 10;
	weight[2] = 10;
	weight[3] = 10;
	weight[4] = 30;
	if(tmptype[0][besttype] <= tmptype[1][pos_fselect[expectfselect]]){
		weight[0] = 10;
		weight[1] = 20;
		weight[2] = 20;
		weight[3] = 20;
	}
	
	enemy_select = 0;
	rate = rand() % 100;
	for(i = 0; i < 5; ++i){
		rate -= weight[i];
		if(rate < 0){
			enemy_select = pos_fselect[i];
			break;
		}
	}
	e_special = false;
	cout<<"enemy_select: "<<enemy_select<<endl;

	if(enemys[enemy_list[enemy_select]].get<int>(SPECIAL) == BEAM && tmpACT[0][enemy_select] >= 7){
		rate = rand() % 100;
		if(rate < 50)
			e_special = true;
	}
}

AI::AI():isConstructed(false){
};
void AI::Init(){
	if(isConstructed){
		for(int i=0;i<mapHeight;i++){
			delete mapTable[i];
			delete mapAttack[i];
		}
		delete mapTable;
		delete mapAttack;
		delete enemyPoint;
		delete friendPoint;
		delete groupsAction;
		delete enemysGroups;
		isConstructed = false;
		isInit = false;
	}
}
void AI::construct(Map * m, Cursor * c, Friend *f, Enemy *e, int fn, int en, int s){
	if(!isConstructed){
		current=0;
		enemyPoint = new IntPoint [en];
		friendPoint = new IntPoint [fn];
		map = m;
		cursor = c;
		enemys = e;
		friends = f;
		isConstructed=true;
		isGrouped=false;
		friendsNum = fn;
		enemysNum = en;
		state = s;
		mapHeight = map->get(HEIGHT);
		mapWidth = map->get(WIDTH);
		enemysGroups = new int [enemysNum];
		groupsAction = new IntPoint [enemysNum];
		mapTable = new int * [mapHeight];
		for(int i=0;i<mapHeight;i++){
			mapTable[i]=new int [mapWidth];
		}
		mapAttack = new int * [mapHeight];
		for(int i=0;i<mapHeight;i++){
			mapAttack[i]=new int [mapWidth];
		}
		for(int i=0;i<mapHeight;i++){
			for(int j=0;j<mapWidth;j++){
				mapTable[i][j]=0;
				mapAttack[i][j]=-1;
			}
		}
	}
	isFin=false;
}


bool AI::isFinish(){
	return isFin;
}

void AI::action(){
	if(isInit){
		if (isStarted){
			//if (enemys[current].getvalue(HPvalue).now > 0)
			{
				cursor->set<float>(POSX, -(float)enemyPoint[current].x - 0.5);
				cursor->set<float>(POSY, -(float)enemyPoint[current].y - 0.5);
				Rect r = map->getdata(enemyPoint[current].x, enemyPoint[current].y);
				if (r.who_on_it == -current - 1)
					cursor->enter(*map, friends, enemys, Z);
				else{
					if (enemyPoint[current].x > 0){
						r = map->getdata(enemyPoint[current].x + 1, enemyPoint[current].y);
						if (r.who_on_it == -current - 1){
							cursor->set<float>(POSX, -((float)enemyPoint[current].x + 1.0 + 0.5));
							cursor->set<float>(POSY, -((float)enemyPoint[current].y) - 0.5);
						}
					}
					if (enemyPoint[current].x<mapWidth){
						r = map->getdata(enemyPoint[current].x - 1, enemyPoint[current].y);
						if (r.who_on_it == -current - 1){
							cursor->set<float>(POSX, -((float)enemyPoint[current].x - 1.0 + 0.5));
							cursor->set<float>(POSY, -((float)enemyPoint[current].y) - 0.5);
						}
					}
					if (enemyPoint[current].y>0){
						r = map->getdata(enemyPoint[current].x, enemyPoint[current].y + 1);
						if (r.who_on_it == -current - 1){
							cursor->set<float>(POSX, -((float)enemyPoint[current].x) - 0.5);
							cursor->set<float>(POSY, -((float)enemyPoint[current].y + 1.0 + 0.5));
						}
					}
					if (enemyPoint[current].y < mapHeight){
						r = map->getdata(enemyPoint[current].x, enemyPoint[current].y - 1);
						if (r.who_on_it == -current - 1){
							cursor->set<float>(POSX, -((float)enemyPoint[current].x) - 0.5);
							cursor->set<float>(POSY, -((float)enemyPoint[current].y - 1.0 + 0.5));
						}
					}

					if (r.who_on_it != -current - 1)
					{
						cursor->set<float>(POSX, -((float)enemyPoint[current].x + 0.1));
						cursor->set<float>(POSY, -((float)enemyPoint[current].y + 0.1));
						map->setdata(enemyPoint[current].x, enemyPoint[current].y, -current - 1);
					}

					cursor->enter(*map, friends, enemys, Z);
				}
				if (enemys[current].get<int>(STATE) != ACTIVE){
					cout << "break";
				};
				isStarted = false;
			}
// 			else
// 			{
// 				if (!pathAttack.empty())
// 					pathAttack.pop();
// 			}
			
		}

		if(pathAttack.empty()){
			int prev=current;
			if(current == enemysNum - 1){
				current = 0;
				isFin = true;
				isInit = false;
				isStarted=false;
				isGrouped=false;
				Value v=enemys[prev].getvalue(HPvalue);
				if(v.now<0.5*v.org)
                    cursor->enter(*map, friends, enemys, Z);
				else
					cursor->enter(*map, friends, enemys, C);
			}
			else{
				current++;
				while (current < enemysNum && enemys[current].getvalue(HPvalue).now <= 0)
				{
					current++;
				}
				if (current == enemysNum)
				{
					isFin = true;
					isInit = false;
					isStarted = false;
					isGrouped = false;
					return;
				}

				isInit=false;
				isStarted=false;
				Value v=enemys[prev].getvalue(HPvalue);
				if(v.now<0.5*v.org)
                    cursor->enter(*map, friends, enemys, Z);
				else
					cursor->enter(*map, friends, enemys, C);
			}
			return;
		}

		if(!pathAttack.empty()){
			IntPoint temps= pathAttack.top();
			float fx = enemys[current].get<float>(POSX);
			float fy = enemys[current].get<float>(POSY);

			if (abs(temps.x - fx) > 0.2)
			{
				if (temps.x < fx){
					cursor->move(LEFT, friends, enemys, *map);
					return;
				}
				else if (temps.x > fx){
					cursor->move(RIGHT, friends, enemys, *map);
					return;
				}
			}
			
			if (abs(temps.y - fy) > 0.2)
			{
				if (temps.y < fy){
					cursor->move(DOWN, friends, enemys, *map);
					return;
				}
				else if (temps.y > fy){
					cursor->move(UP, friends, enemys, *map);
					return;
				}
			}
			
			pathAttack.pop();
			Value v=enemys[current].getvalue(MVvalue);
			if(v.now==0)
				while(!pathAttack.empty())
					pathAttack.pop();
		}

	}else{
		if(state==NORMAL)
			InitNormal();
		else
			InitHard();
	}
}

AI::~AI(){
	for(int i=0;i<mapHeight;i++){
		delete mapTable[i];
		delete mapAttack[i];
	}
	delete[] mapTable;
	delete[] mapAttack;
	delete enemyPoint;
	delete friendPoint;
}

void AI::InitNormal(){
	InitMap();
	for(int i=0;i<friendsNum;i++){
		if(friends[i].get<int>(DEAD)) continue;
		int x = (int)(friends[i].get<float>(POSX)+0.5);
		int y = (int)(friends[i].get<float>(POSY)+0.5);
		mapTable[y][x]=-2;
	}

	MovePathMatrix();

	ConstructStackAttackPath();

	isStarted=true;
	isInit=true;
	isFin=false;

}

void AI::InitHard(){
	InitMap();
	int iWidth = mapWidth/3;
	int iHeight = mapHeight/3;
	int iLength = iWidth * iHeight;
	int * bigMap;
	bigMap = new int [iLength];

	AI02(ENEMYS, bigMap);
	int ** bigMap2 = new int * [iHeight];			// ENEMYS map
	for(int i=0;i<iHeight;i++){
		bigMap2[i]=new int [iWidth];
	}
	for(int i=0;i<iHeight;i++){
		for(int j=0;j<iWidth;j++){
			bigMap2[i][j]=bigMap[i*iHeight+j];
		}
	}
	AI02(FRIENDS, bigMap);
	int ** bigMap3 = new int * [iHeight];			// FRIENDS map
	for(int i=0;i<iHeight;i++){
		bigMap3[i]=new int [iWidth];
	}
	for(int i=0;i<iHeight;i++){
		for(int j=0;j<iWidth;j++){
			bigMap3[i][j]=bigMap[i*iHeight+j];
		}
	}

	div_t d;
	int x;
	int y;

	if(!isGrouped){
		AI04();
		AI05(bigMap3);
	}else{
		x=groupsAction[enemysGroups[current]].x;
		y=groupsAction[enemysGroups[current]].y;
		x=x*3+1;
		y=y*3+1;
		if(x>=0 && y>=0){
			int currentX=(int)(enemys[current].get<float>(POSX)+0.5);
			int currentY=(int)(enemys[current].get<float>(POSY)+0.5);
			if(mapTable[y-1][x]!=-1 && mapTable[y+1][x]!=-1 && mapTable[y][x-1]!=-1 && mapTable[y][x+1]!=-1){
				if(currentX<x && mapTable[y-1][x-1]==-1)
					y-=1;
				if(currentX>x && mapTable[y-1][x+1]==-1)
					y-=1;
				if(currentX<x && mapTable[y+1][x-1]==-1)
					y+=1;
				if(currentX>x && mapTable[y+1][x+1]==-1)
					y+=1;
				if(currentY<y && mapTable[y-1][x-1]==-1)
					x-=1;
				if(currentY>y && mapTable[y+1][x-1]==-1)
					x-=1;
				if(currentY<y && mapTable[y-1][x+1]==-1)
					x+=1;
				if(currentY>y && mapTable[y+1][x+1]==-1)
					x+=1;
			}
			mapTable[y][x]=-2;
			MovePathMatrix();
			ConstructStackAttackPath();
			cout << "Attack!!!!!!!!!!!    ";
			cout << y << ":" << x << endl;
			isStarted=true;
			isInit=true;
			isFin=false;
			isGrouped=true;
			return;
		}
	}

	x=(int)(enemys[current].get<float>(POSX)+0.5);
	y=(int)(enemys[current].get<float>(POSY)+0.5);
	d=div(x,3);
	x=d.quot;
	d=div(y,3);
	y=d.quot;

	IntPoint toBigMap;
	toBigMap.x=-1;
	toBigMap.y=-1;
	if(bigMap2[y][x]<3){
		AI03(bigMap2, bigMap3, iHeight, iWidth, toBigMap);
	}
	if(toBigMap.x!=-1 && toBigMap.y!=-1){
		if( mapTable[toBigMap.y*3][toBigMap.x*3+1]==-1 || 
			mapTable[toBigMap.y*3+1][toBigMap.x*3+2]==-1 ||
			mapTable[toBigMap.y*3+2][toBigMap.x*3+1]==-1 ||
			mapTable[toBigMap.y*3+1][toBigMap.x*3]==-1 ){
			mapTable[toBigMap.y*3+1][toBigMap.x*3+1]=-2;
		}else 
		if((mapTable[toBigMap.y*3][toBigMap.x*3+1]==-3 || 
			mapTable[toBigMap.y*3][toBigMap.x*3+1]==-2 || 
			mapTable[toBigMap.y*3][toBigMap.x*3+1]==-4 )&& 
		   (mapTable[toBigMap.y*3+1][toBigMap.x*3+2]==-3 ||
			mapTable[toBigMap.y*3+1][toBigMap.x*3+2]==-2 ||
			mapTable[toBigMap.y*3+1][toBigMap.x*3+2]==-4 )&&
		   (mapTable[toBigMap.y*3+2][toBigMap.x*3+1]==-3 ||
			mapTable[toBigMap.y*3+2][toBigMap.x*3+1]==-2 ||
			mapTable[toBigMap.y*3+2][toBigMap.x*3+1]==-4 )&&
			mapTable[toBigMap.y*3+1][toBigMap.x*3]==-3 ||
			mapTable[toBigMap.y*3+1][toBigMap.x*3]==-2 ||
			mapTable[toBigMap.y*3+1][toBigMap.x*3]==-4 ){
				if((int)(enemys[current].get<float>(POSX)+0.5)>toBigMap.x)
					mapTable[toBigMap.y*3][toBigMap.x*3+1]=-2;
				else if((int)(enemys[current].get<float>(POSX)+0.5)<toBigMap.x)
					mapTable[toBigMap.y*3][toBigMap.x*3-1]=-2;
				else if((int)(enemys[current].get<float>(POSY)+0.5)>toBigMap.y)
					mapTable[toBigMap.y*3+1][toBigMap.x*3]=-2;
				else if((int)(enemys[current].get<float>(POSY)+0.5+1)<toBigMap.y)
					mapTable[toBigMap.y*3-1][toBigMap.x*3]=-2;
		}
		bool isTargeted = false;
		Value v=enemys[current].getvalue(MVvalue);
		int ** mapTableTemp = new int * [mapHeight];
		for(int i=0;i<mapHeight;i++)
			mapTableTemp[i]=new int [mapWidth];
		for(int i=0;i<mapHeight;i++)
			for(int j=0;j<mapWidth;j++)
				mapTableTemp[i][j]=mapTable[i][j];
		mapTableTemp[(int)(enemys[current].get<float>(POSY)+0.5)][(int)(enemys[current].get<float>(POSX)+0.5)]=0;
		for(int i=0;i<v.now;i++){
			for(int j=1;j<mapHeight-1;j++){
				for(int k=1;k<mapWidth-1;k++){
					if(mapTableTemp[j][k]==i){
						iSteps=mapTableTemp[j][k]+1;
						if(mapTableTemp[j-1][k]==-1) mapTableTemp[j-1][k]=iSteps;
						if(mapTableTemp[j+1][k]==-1) mapTableTemp[j+1][k]=iSteps;
						if(mapTableTemp[j][k-1]==-1) mapTableTemp[j][k-1]=iSteps;
						if(mapTableTemp[j][k+1]==-1) mapTableTemp[j][k+1]=iSteps;
						if(mapTableTemp[j-1][k]==-2) { isTargeted=true; break;}
						if(mapTableTemp[j+1][k]==-2) { isTargeted=true; break;} 
						if(mapTableTemp[j][k-1]==-2) { isTargeted=true; break;}
						if(mapTableTemp[j][k+1]==-2) { isTargeted=true; break;}
					}
				}
				if(isTargeted)break;
			}
			if(isTargeted)break;
		}
		if(!isTargeted)
			for(int i=0;i<iHeight;i++){
				for(int j=0;j<iWidth;j++){
					if(bigMap3[i][j]>0){
						for(int k=0;k<3;k++){
							for(int l=0;l<3;l++){
								mapTable[i*3+k][j*3+l]=-4;
							}
						}
					}
				}
			}
		MovePathMatrix();
		ConstructStackAttackPath();
	}
	//Value v = enemys[current].getvalue(HPvalue);
	//if(v.now<10){

	//}
	isStarted=true;
	isInit=true;
	isFin=false;
	isGrouped=true;

	delete bigMap;
	for(int i=0;i<iHeight;i++){
		delete bigMap2[i];
		delete bigMap3[i];
	}
	delete bigMap2;
	delete bigMap3;
}
void AI::InitMap(){
	Rect r;
	int soldierSkin = enemys[current].get<int>(SKIN);
	for(int i=0;i<enemysNum;i++){
		enemyPoint[i].x=enemys[i].get<int>(ORGX);
		enemyPoint[i].y=enemys[i].get<int>(ORGY);
	}

	for(int i=0;i<mapHeight;i++){
		for(int j=0;j<mapWidth;j++){
			r=map->getdata(j,i);
			if(enemys[current].canmove(r.type, r.who_on_it))
				mapTable[i][j]=-1;
			else
				mapTable[i][j]=-4;
		}
	}

	for(int i=0;i<enemysNum;i++){
		if(enemys[i].get<int>(DEAD)) continue;
		int x = (int)(enemys[i].get<float>(POSX)+0.5);
		int y = (int)(enemys[i].get<float>(POSY)+0.5);
		mapTable[y][x]=-3;
	}
}

void AI::MovePathMatrix(){
	mapTable[enemys[current].get<int>(POSY)][enemys[current].get<int>(POSX)]=0;
	bool isTargeted = false;

	iSteps=0;
	int MaxCount;
	MaxCount = (mapHeight < mapWidth ? mapWidth : mapHeight);
	for(int i=0;i<MaxCount;i++){
		for(int j=1;j<mapHeight-1;j++){
			for(int k=1;k<mapWidth-1;k++){
				if(mapTable[j][k]==i){
					iSteps=mapTable[j][k]+1;
					if(mapTable[j-1][k]==-1) mapTable[j-1][k]=iSteps;
					if(mapTable[j+1][k]==-1) mapTable[j+1][k]=iSteps;
					if(mapTable[j][k-1]==-1) mapTable[j][k-1]=iSteps;
					if(mapTable[j][k+1]==-1) mapTable[j][k+1]=iSteps;
					if(mapTable[j-1][k]==-2) { attackTarget.x=k; attackTarget.y=j-1; isTargeted=true; break;}
					if(mapTable[j+1][k]==-2) { attackTarget.x=k; attackTarget.y=j+1; isTargeted=true; break;} 
					if(mapTable[j][k-1]==-2) { attackTarget.x=k-1; attackTarget.y=j; isTargeted=true; break;}
					if(mapTable[j][k+1]==-2) { attackTarget.x=k+1; attackTarget.y=j; isTargeted=true; break;}
				}
			}
			if(isTargeted)break;
		}
		if(isTargeted)break;
	}

	ofstream fout;
	fout.open("mapAttack.txt", ios_base::app);
	for(int i=0;i<mapHeight;i++){
		for(int j=0;j<mapWidth;j++){
			fout << setw(2) << mapTable[i][j];
		}
		fout<< endl;
	}
	fout << endl;
	fout.close();


}

void AI::ConstructStackAttackPath(){
	int sX, sY;
	sX = attackTarget.x;
	sY = attackTarget.y;
	if(iSteps>0)
		for(int j=iSteps-1;j>0;j--){
			if(sY>=1)
				if(mapTable[sY-1][sX]==j)
					sY--;
			if(sY<=20)
				if(mapTable[sY+1][sX]==j)
					sY++;
			if(sX>=1)
				if(mapTable[sY][sX-1]==j)
					sX--;
			if(sX<=20)
				if(mapTable[sY][sX+1]==j)
					sX++;
			IntPoint temp;
			temp.x=sX;
			temp.y=sY;
			if(!pathAttack.empty()){
				IntPoint temps=pathAttack.top();
				if(temps.x==temp.x && temps.y==temp.y){
					while(!pathAttack.empty())
						pathAttack.pop();
					break;
				}
			}
			ofstream fout;
			fout.open("mapAttack.txt", ios_base::app);
			fout << current << "=[";
			fout << temp.y << ":" << temp.x;
			fout << "]" << endl;
			fout.close();
			pathAttack.push(temp);
		}

}

int AI::AI01(){	// ßP¬_•i≤æ∞ Ωd≥Ú§∫¨Oß_¶≥ß¿ªπÅE≥
	int returnvalue=0;
	int x1,y1;
	int x2=(int)(enemys[current].get<float>(POSX)+0.5);
	int y2=(int)(enemys[current].get<float>(POSY)+0.5);
	int x;
	int y;
	Value v = enemys[current].getvalue(MVvalue);
	int MaxValue=v.now;
	for(int i=0;i<friendsNum;i++){
		if(friends[i].get<int>(DEAD)) continue;
		x1 = (int)(friends[i].get<float>(POSX)+0.5);
		y1 = (int)(friends[i].get<float>(POSY)+0.5);
		x=abs(x1-x2);
		y=abs(y1-y2);
		if(x+y+1 <= MaxValue)
			returnvalue++;
	}
	return returnvalue;
}

void AI::AI02(int soldier, int * bigMap){	//ßP¬_•iß¿ª™∫§j¶aπœ
	int iWidth = mapWidth/3;
	int iHeight = mapHeight/3;
	int iLength = iWidth * iHeight;
	int x1,y1;
	div_t xy1;
	div_t xy2;
	for(int i=0;i<iLength;i++)
			bigMap[i]=-1;
	if(soldier == FRIENDS){
		for(int i=0;i<friendsNum;i++){
			if(friends[i].get<int>(DEAD)) continue;
			x1=(int)(friends[i].get<float>(POSX)+0.5);
			y1=(int)(friends[i].get<float>(POSY)+0.5);
			xy1 = div(x1,3);
			xy2 = div(y1,3);
			if(bigMap[xy2.quot*iWidth+xy1.quot]==-1)
				bigMap[xy2.quot*iWidth+xy1.quot]=0;
			bigMap[xy2.quot*iWidth+xy1.quot]+=1;
		}
	}else{
		int * tempMap = new int [iLength];
		for(int i=0;i<iLength;i++)
			tempMap[i]=-1;
		for(int i=0;i<friendsNum;i++){
			if(friends[i].get<int>(DEAD)) continue;
			x1=(int)(friends[i].get<float>(POSX)+0.5);
			y1=(int)(friends[i].get<float>(POSY)+0.5);
			xy1 = div(x1,3);
			xy2 = div(y1,3);
			if(tempMap[xy2.quot*iWidth+xy1.quot]==-1)
				tempMap[xy2.quot*iWidth+xy1.quot]=0;
			tempMap[xy2.quot*iWidth+xy1.quot]+=1;
		}
		for(int i=0;i<enemysNum;i++){
			if(enemys[i].get<int>(DEAD)) continue;
			x1=(int)(enemys[i].get<float>(POSX)+0.5);
			y1=(int)(enemys[i].get<float>(POSY)+0.5);
			xy1 = div(x1,3);
			xy2 = div(y1,3);
			if(bigMap[xy2.quot*iWidth+xy1.quot]==-1)
				bigMap[xy2.quot*iWidth+xy1.quot]=0;
			bigMap[xy2.quot*iWidth+xy1.quot]+=1;
		}
		x1=(int)(enemys[current].get<float>(POSX)+0.5);
		y1=(int)(enemys[current].get<float>(POSY)+0.5);
		div_t xxy1 = div(x1,3);
		div_t xxy2 = div(y1,3);
		//if(tempMap[xxy2.quot*iWidth+xxy1.quot]==-1)
			for(int i=0;i<enemysNum;i++){
				if(enemys[i].get<int>(DEAD)) continue;
				x1=(int)(enemys[i].get<float>(POSX)+0.5);
				y1=(int)(enemys[i].get<float>(POSY)+0.5);
				xy1 = div(x1,3);
				xy2 = div(y1,3);
				if(bigMap[xy2.quot*iWidth+xy1.quot]==1 && i>current){
					if(tempMap[xxy2.quot*iWidth+xxy1.quot]==-1)
						bigMap[xy2.quot*iWidth+xy1.quot]=0;
				}
			}
		
		delete tempMap;
	}

	for(int i=0;i<iLength;i++){
		xy1=div(i,iWidth);
	}
}


void AI::AI03(int ** bigMap2, int ** bigMap3, int iHeight, int iWidth, IntPoint &toBigMap)				//≠Y∏®≥Ê°A´h¥Mß‰∂Z¬˜≥Ã™Ò§ß•ÅEÅE
{
	div_t d;
	int x=(int)(enemys[current].get<float>(POSX)+0.5);
	int y=(int)(enemys[current].get<float>(POSY)+0.5);
	d=div(x,3);
	x=d.quot;
	d=div(y,3);
	y=d.quot;

	toBigMap.x=-1;
	toBigMap.y=-1;
	bool isTargeted = false;
	int iSteps2=0;
	int MaxCount;
	MaxCount = (iHeight < iWidth ? iWidth : iHeight);
	int **bigMapTemp = new int * [iHeight];
	for(int i=0;i<iHeight;i++)
		bigMapTemp[i] = new int [iWidth];
	for(int i=0;i<iHeight;i++)
		for(int j=0;j<iWidth;j++)
			bigMapTemp[i][j]=bigMap2[i][j];
	for(int i=0;i<iHeight;i++){
		for(int j=0;j<iWidth;j++){
			if(bigMap3[i][j]>0)
				bigMap2[i][j]=-4;
			if(bigMap2[i][j]>0)
				bigMap2[i][j]=-3;
			if(bigMap2[i][j]==0)
				bigMap2[i][j]=-1;
		}
	}
	ofstream fout;
	fout.open("ai03.txt", ios_base::app);
	fout << current << endl;;
	for(int i=0;i<iHeight;i++){
		for(int j=0;j<iWidth;j++){
			fout << setw(3) << bigMap2[i][j];
		}
		fout << endl;
	}
	fout << endl;

	bigMap2[y][x]=0;
	for(int i=0;i<iHeight;i++){
		for(int j=0;j<iWidth;j++){
			fout << setw(3) << bigMap2[i][j];
		}
		fout << endl;
	}
	fout << endl;
	for(int i=0;i<MaxCount;i++){
		for(int j=0;j<iHeight;j++){
			for(int k=0;k<iWidth;k++){
				if(bigMap2[j][k]==i){
					iSteps2=bigMap2[j][k]+1;
					if(j>0)
						if(bigMap2[j-1][k]==-1) bigMap2[j-1][k]=iSteps2;
					if(j<iHeight-1)
						if(bigMap2[j+1][k]==-1) bigMap2[j+1][k]=iSteps2;
					if(k>0)
						if(bigMap2[j][k-1]==-1) bigMap2[j][k-1]=iSteps2;
					if(k<iWidth-1)
						if(bigMap2[j][k+1]==-1) bigMap2[j][k+1]=iSteps2;
					if(j>0)
						if(bigMap2[j-1][k]==-3) {
							toBigMap.x=k; 
							toBigMap.y=j-1; 
							isTargeted=true;
							break;
						}
					if(j<iHeight-1)
						if(bigMap2[j+1][k]==-3) {
							toBigMap.x=k; 
							toBigMap.y=j+1; 
							isTargeted=true;
							break;
						}
					if(k>0)
						if(bigMap2[j][k-1]==-3) {
							toBigMap.x=k-1; 
							toBigMap.y=j; 
							isTargeted=true;
							break;
						}
					if(k<iWidth-1)
						if(bigMap2[j][k+1]==-3) {
							toBigMap.x=k+1; 
							toBigMap.y=j; 
							isTargeted=true;
							break;
						}
				}
			}
			if(isTargeted)break;
		}
		if(isTargeted)break;
	}
	for(int i=0;i<iHeight;i++){
		for(int j=0;j<iWidth;j++){
			fout << setw(3) << bigMap2[i][j];
		}
		fout << endl;
	}
	fout << endl;
	fout.close();

	for(int i=0;i<iHeight;i++)
		delete bigMapTemp[i];
	delete bigMapTemp;


}

void AI::AI04(){		// Ωs∂§
	int x, y;
	div_t d;
	int count=0;
	int ** tempMap = new int * [mapHeight/3];
	for(int i=0;i<mapHeight/3;i++){
		tempMap[i]=new int [mapWidth/3];
	}
	for(int i=0;i<mapHeight/3;i++)
		for(int j=0;j<mapWidth/3;j++)
			tempMap[i][j]=0;
	for(int i=0;i<enemysNum;i++){
		enemysGroups[i]=0;
		x=(int)(enemys[i].get<float>(POSX)+0.5);
		y=(int)(enemys[i].get<float>(POSY)+0.5);
		d=div(x,3);
		x=d.quot;
		d=div(y,3);
		y=d.quot;
		if(tempMap[y][x]==0){
			tempMap[y][x]=++count;
			enemysGroups[i]=count;
		}else
			enemysGroups[i]=tempMap[y][x];
	}
	ofstream fout;
	fout.open("enemysGroups.txt", ios_base::app);
	for(int i=0;i<enemysNum;i++){
		fout << "[" << enemysGroups[i] << "] ";
	}
	fout << endl;
	fout.close();
	for(int i=0;i<mapHeight/3;i++){
		delete tempMap[i];
	}
	delete tempMap;
}

void AI::AI05(int ** bigMap){	//≠Y§w¶®πŒ´hß¿ª™˛™Òºƒ§H
	int iHeight = mapHeight/3;
	int iWidth = mapWidth/3;
	int x,y;
	int * iCounts = new int [enemysNum];
	int * iMapping = new int [enemysNum];
	int ** bigMap2 = new int * [iHeight];
	for(int i=0;i<iHeight;i++)
		bigMap2[i]=new int [iWidth];
	for(int i=0;i<iHeight;i++){
		for(int j=0;j<iWidth;j++){
			if(bigMap[i][j]!=-1)
				bigMap2[i][j]=-2;
			else
				bigMap2[i][j]=-1;
		}
	}

	for(int i=0;i<enemysNum;i++){
		iCounts[i]=0;
		iMapping[i]=-1;
	}
	for(int i=0;i<enemysNum;i++){
		iCounts[enemysGroups[i]]++;
		if(iMapping[i]==-1)
			iMapping[enemysGroups[i]]=i;
	}
	for(int i=0;i<enemysNum;i++){
		if(iCounts[i]>=3){
			x=(int)(enemys[iMapping[i]].get<float>(POSX)+0.5);
			y=(int)(enemys[iMapping[i]].get<float>(POSY)+0.5);
			div_t d;
			d=div(x,3);
			x=d.quot;
			d=div(y,3);
			y=d.quot;
			bigMap2[y][x]=0;

			ofstream fout;
			fout.open("groupsAction.txt", ios_base::app);
			fout << x << ":" << y << endl;
			fout.close();

			bool isTargeted = false;
			int iSteps2=0;
			int MaxCount;
			MaxCount = (iHeight < iWidth ? iWidth : iHeight);
			for(int j=0;j<MaxCount;j++){
				for(int k=1;k<iHeight-1;k++){
					for(int l=1;l<iWidth-1;l++){
						if(bigMap2[k][l]==j){
							iSteps2=bigMap2[k][l]+1;
							if(bigMap2[k-1][l]==-1) bigMap2[k-1][l]=iSteps2;
							if(bigMap2[k+1][l]==-1) bigMap2[k+1][l]=iSteps2;
							if(bigMap2[k][l-1]==-1) bigMap2[k][l-1]=iSteps2;
							if(bigMap2[k][l+1]==-1) bigMap2[k][l+1]=iSteps2;
							if(bigMap2[k-1][l]==-2) { groupsAction[i].x=l; groupsAction[i].y=k-1; isTargeted=true; break;}
							if(bigMap2[k+1][l]==-2) { groupsAction[i].x=l; groupsAction[i].y=k+1; isTargeted=true; break;} 
							if(bigMap2[k][l-1]==-2) { groupsAction[i].x=l-1; groupsAction[i].y=k; isTargeted=true; break;}
							if(bigMap2[k][l+1]==-2) { groupsAction[i].x=l+1; groupsAction[i].y=k; isTargeted=true; break;}
						}
					}
					if(isTargeted)break;
				}
				if(isTargeted)break;
			}

		}else{
			groupsAction[i].x=-1;
			groupsAction[i].y=-1;
		}
	}
	int * min = new int [enemysNum];
	for(int i=0;i<enemysNum;i++)
		min[i]=15;
	Value v;
	for(int i=0;i<enemysNum;i++){
		v=enemys[i].getvalue(MVvalue);
		if(v.now<min[enemysGroups[i]])
			min[enemysGroups[i]]=v.now;
	}
	int ** tempMapTable = new int * [mapHeight];
	for(int i=0;i<mapHeight;i++)
		tempMapTable[i]=new int [mapWidth];
	for(int i=0;i<mapHeight;i++)
		for(int j=0;j<mapWidth;j++)
			tempMapTable[i][j]=mapTable[i][j];

	for(int i=0;i<mapHeight;i++)
		delete tempMapTable[i];
	delete tempMapTable;
	delete min;
	ofstream fout;
	fout.open("groupsAction.txt", ios_base::app);
	for(int i=0;i<enemysNum;i++){
		fout << groupsAction[i].y << ":" << groupsAction[i].x << endl;
	}
	for(int i=0;i<iHeight;i++){
		for(int j=0;j<iWidth;j++){
			fout << setw(3) << bigMap2[i][j] ;
		}
		fout << endl;
	}
	fout << endl;
	fout.close();
}

bool AI::AI06(int ** bigMap2, int ** bigMap3, int iHeight, int iWidth, IntPoint toBigMap){
	div_t d;
	if(bigMap2[toBigMap.y][toBigMap.x]==1){
		for(int i=0;i<current;i++){
			int x=(int)(enemys[i].get<float>(POSX)+0.5);
			int y=(int)(enemys[i].get<float>(POSY)+0.5);
			d=div(x,3);
			x=d.quot;
			d=div(y,3);
			y=d.quot;
			if(x==toBigMap.x && y==toBigMap.y)
				return true;
		}
		return false;
	}
	else if(bigMap2[toBigMap.y][toBigMap.x]>1)
        return	true;
	else
		return	false;
}

