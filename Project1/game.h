#include<vector>
#include<graphics.h>
#include"Block.h"
using namespace std;

#ifndef GAME_H
#define GAME_H

class Game
{
public:
	Game(int rows, int cols, int leftdis, int updis, int blocksize);
	void init();//��ʼ��
	void play();//���߼�

private:
	int getDelay();//��ȡ�ӳ�
	void updateWindow();//���»���
	void keyEvent();//��������
	void clearline();//����
	void drop();//����
	bool validblock(const vector<vector<int>>& map);//�жϵ�ǰ���Ƿ�Ϸ�
	void floatmove(int dis);//ˮƽ�ƶ�
	void rotate();//��ת
	void showscore();//չʾ����
	void checkover();//�жϽ���
	void saveScore();//����ɼ�
	void displayOver();//չʾ��������
private:
	bool update;//���±�־
	int delay;//
	int rows;//����
	int cols;//����
	int leftdis;//��߽絽����������
	int updis;//�ϱ߽絽�����ϲ����
	int blocksize;//�����С
	vector<vector<int>>map;//��ͼ
	IMAGE background;//����ͼƬ
	IMAGE loseimg;//ʧ��ͼƬ
	IMAGE winimg;//ʤ��ͼƬ
	Block* curblock;//��ǰ�飬��ָ�룬��ָ�򼸸�ԭʼ���飬���ٿ���
	Block* nextblock;//��һ����
	Block backblock;//���ݿ�
	int score;//�ɼ�
	int level;//�ؿ���
	int linecnt;//����������
	int highestSocre;//��߷�
	bool gameover;//������־λ
	bool win;//ʤ����־λ��1��ʤ����0��ʧ��
};

#endif // !GAME_H


