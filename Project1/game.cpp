#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include "Game.h"
#include"Block.h"
#include<stdio.h>
#include<mmsystem.h>
#include<fstream>
#pragma comment(lib, "winmm.lib")
#define RECORD_FILE "record.txt"

const int maxlevel = 5;//���ؿ���
const int SPEED[maxlevel+1] = { 0,500,300,150,100,80 };//���ص������ٶ�

Game::Game(int rows, int cols, int leftdis, int updis, int blocksize)
{
	//�Ѳ���������Ա
	this->blocksize = blocksize;
	this->rows = rows;
	this->cols = cols;
	this->updis = updis;
	this->leftdis = leftdis;

	//�̵�ͼ����ʼ��Ϊ-1��ʾ��
	for (int i = 0; i <= rows; i++) {
		vector<int>rowmap;
		for (int j = 0; j <= cols; j++)
			rowmap.push_back(-1);
		map.push_back(rowmap);
	}
}

void Game::init()//��ʼ��
{
	mciSendString("play res/bg.mp3 repeat", 0, 0, 0);//��������ѭ������

	gameover = 0;
	win = 0;//��ʼĬ�Ͻ�����ʽ��ʧ��
	score = 0;
	level = 1;
	linecnt = 0;
	delay = SPEED[level];//���ó�ʼ�ٶ�
	srand(time(NULL));

	//������Ҫ��ͼƬ
	loadimage(&background, "res/bg2.png",480,480,1);
	loadimage(&winimg, "res/win.png", 150, 100, 1);
	loadimage(&loseimg, "res/over.png", 150, 100, 1);

	initgraph(480, 480);//������

	//��ʼ�����������ؿ�����˰ѵ�ͼ���
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			map[i][j] = -1;

	//��ȡ��ʷ��߷�
	ifstream file(RECORD_FILE);
	file >> highestSocre;
	file.close();
}

void Game::play()
{
	init();

	//���ɵ�ǰ�����һ����
	nextblock = new Block;
	curblock = nextblock;
	nextblock = new Block;

	int timer = 0;
	//��ѭ��
	while (1) {
		keyEvent();//��ȡ��������

		timer += getDelay();//������ʱ

		//��ʱ����ʱ�޾�ˢ��
		if (timer > delay){
			timer = 0;
			update = 1;//����
			drop();//����һ��
		}

		//���±�־λΪ1ʱ����
		if (update) {
			update = 0;//���±�־λ��ת
			clearline();//����
			updateWindow();//������Ļ
		}
		//������־λΪ1ʱ����
		if (gameover) {
			saveScore();//����÷�
			displayOver();//չʾ��������
			system("pause");//��ͣ
			init();//�������������ͣ���ؿ�һ��
		}
	}
}

int Game::getDelay()//��ȡ�ӳ�
{
	static unsigned long long lasttime = 0;//��̬������������ϴε��õ����ڵ��ӳ�
	unsigned long long curtime = GetTickCount();//��ȡ��ǰʱ��
	//��һ�ε���
	if (lasttime == 0) {
		lasttime = curtime;
		return 0;
	}
	else {
		int ret = curtime - lasttime;//�����ӳ�
		lasttime = curtime;//���浱ǰʱ��
		return ret;
	}
}

void Game::updateWindow()//���»���
{
	putimage(0, 0, &background);//��ʾ��������֪��ΪɶҪ������ÿ�ζ�ˢ��

	BeginBatchDraw();
	IMAGE** imgs = Block::getimgs();//��ȡ�������ӵ�ԭʼͼƬ
	for(int i=0;i<rows;i++)
		for (int j = 0; j < cols; j++) {
			if (map[i][j] == -1)continue;//�ո���ʾ
			int x = j * blocksize + leftdis;
			int y = i * blocksize + updis;//��������
			putimage(x, y, imgs[map[i][j]]);//������ɫ��������ʾ
		}

	curblock->paint(leftdis, updis);//��ʾ��ǰ���еĸ���
	nextblock->paint(400, 150);//��ʾ��һ������

	showscore();//չʾ�ɼ�
	EndBatchDraw();

}

void Game::keyEvent()
{
	bool rotateflag = 0;//��ת��־λ����
	int dx=0;//ˮƽλ������
	unsigned char t;
	if (_kbhit()) {
		t = _getch();//��һ���ַ�
		//�����224��˵�������������ĸ���
		if (t == 224) {
			t = _getch();//�ٶ�һ�����жϵ������ĸ�
			if (t == 72)rotateflag = 1;//����ת
			else if (t == 80)delay = SPEED[5];//�¼���
			else if (t == 75)dx = -1;//������һ��
			else if (t == 77)dx = 1;//������һ��
		}
	}
	//��ת
	if (rotateflag) {
		rotate();
		update = 1;//��ת����������
	}
	//λ��
	if (dx != 0) {
		floatmove(dx);
		update = 1;//��λ��Ҳ��������
	}
}

void Game::clearline()//����
{
	int k = rows - 1;
	int lines = 0;//���������ԭΪ0
	for (int i = rows - 1; i >= 0; i--) {
		int cnt = 0;
		for (int j = 0; j < cols; j++) {
			if (map[i][j] != -1)cnt++;//ͳ�Ƶ�ǰ�з�����
			map[k][j] = map[i][j];//�õ�ǰ�и��ǽ��ָ������һ��
		}
		if (cnt < cols)k--;//�����һ�в����Ǹ��ӣ����ָ������һλ����������һ�У��൱�ڲ���
		else lines++;//�����Ǹ��ӣ��ͽ��ָ�벻����������������һ�У�ͬʱ��¼�������������
	}
	//��������ˣ����·���
	if (lines) {
		linecnt += lines;//������������
		int reward[4] = { 10,30,50,80 };//������ͬ�����÷ֲ�ͬ

		score += reward[lines - 1];
		if (score > highestSocre)highestSocre = score;//������߷�

		level = score / 100 + 1;//���ݷ����жϹؿ�
		if (level > maxlevel)gameover = 1,win=1;//����������һ�أ�����

		update = 1;//������Ļ

		mciSendString("play res/xiaochu2.mp3", 0, 0, 0);//����������Ч
	}
}

void Game::drop()//����
{
	backblock = *curblock;//����
	curblock->drop();//����
	
	//������䲻�Ϸ�
	if (!validblock(map)) {
		delay = SPEED[level];//�����ٶ�
		backblock.solidarity(map);//�̻���ǰ��
		delete curblock;//ɾ����ǰ��
		curblock = nextblock;//����һ���鸳����ǰ��
		nextblock = new Block;//������һ����
		checkover();//����µĵ�ǰ���Ƿ����Ϸ����
	}
}

bool Game::validblock(const vector<vector<int>>& map)//��鵱ǰ���Ƿ�Ϸ��������ͼ 
{
	point* loc = curblock->getloc();//��ȡ��ǰ�������
	for (int i = 0; i < 4; i++) {
		if (loc[i].col < 0 || loc[i].row < 0 || 
			loc[i].row >= rows || loc[i].col >= cols)//����
			return 0;
		if (map[loc[i].row][loc[i].col] != -1)return 0;//�ص������Ϸ�
	}
	return 1;
}

void Game::floatmove(int dis)//ˮƽ�ƶ������뷽��
{
	backblock = *curblock;//����ͬ���䣬�ȱ����ٲ���
	curblock->floatmove(dis);
	if (!validblock(map)) {
		*curblock = backblock;//����µ�λ�ò��Ϸ�����ԭ
	}
}

void Game::rotate()//��ת
{
	if (curblock->getblocktype() == 6)return;//�����β�����ת
	backblock = *curblock;//����
	curblock->rotate();//����
	if (!validblock(map)) {
		*curblock = backblock;//�����󲻺Ϸ�����ԭ
	}
}

void Game::showscore()//չʾ����
{
	char scoretxt[32];
	sprintf_s(scoretxt, sizeof(scoretxt), "%d", score);//����ɼ�

	setcolor(RGB(180, 180, 180));//��ɫ

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;//�ִ�С
	f.lfWidth = 15;
	f.lfQuality = ANTIALIASED_QUALITY;
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));//����
	settextstyle(&f);

	outtextxy(340, 390, scoretxt);//����ɼ�

	sprintf_s(scoretxt, sizeof(scoretxt), "%d", level);
	outtextxy(70, 395, scoretxt);//����ؿ���

	sprintf_s(scoretxt, sizeof(scoretxt), "%d", linecnt);
	outtextxy(70, 440, scoretxt);//��������������

	sprintf_s(scoretxt, sizeof(scoretxt), "%d", highestSocre);
	outtextxy(340, 440, scoretxt);//�����߷�
}

void Game::checkover()//�����ж�
{
	gameover = !validblock(map);//��ǰ��û���в����Ͳ��Ϸ���Ҳ����ûλ���ˣ�����
}

void Game::saveScore()//�������
{
	ofstream file(RECORD_FILE);//������߷ֵ��ļ���
	file << highestSocre;
	file.close();
}

void Game::displayOver()
{
	//�����ͨ����
	if (win) {
		putimage(150, 150, &winimg);//��ʾʤ��ͼƬ
		mciSendString("play res/win.mp3", 0, 0, 0);//����ʤ����Ч
	}
	//�����ʧ����
	else {
		putimage(150, 150, &loseimg);
		mciSendString("play res/over.mp3", 0, 0, 0);
	}
}
