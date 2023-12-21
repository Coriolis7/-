#include<graphics.h>
#include "Block.h"
#include<stdlib.h>
#include<vector>
using namespace std;

IMAGE* Block::imgs[7] = { NULL };//ԭʼ���飬��ʼΪ��
int Block::size = 19;//���÷����С


Block::Block()
{
	//���ԭʼ���ǿյģ��ȵ���ԭʼ��
	if (imgs[0] == NULL) {
		IMAGE tmp;
		loadimage(&tmp, "res/tiles.png",152,19,1);//���߸�ԭʼ���ͼƬ����

		SetWorkingImage(&tmp);
		for (int i = 0; i < 7; i++) {
			imgs[i] = new IMAGE;//����ռ�
			getimage(imgs[i], i * size, 0, size, size);//�и�ͼƬ�Ž�ԭʼ������
		}
		SetWorkingImage();
	}
	int blocks[7][4] = {
		1,3,5,7,
		2,4,5,7,
		3,5,4,6,
		3,5,4,7,
		2,3,5,7,
		3,5,7,6,
		2,3,4,5
	};//���ø��ֿ����״

	blocktype = rand() % 7;//���ѡһ��

	for (int i = 0; i < 4; i++) {
		int v = blocks[blocktype][i];//�÷�����״������ڵ�ͼ������
		loc[i].col = v % 2;
		loc[i].row = v / 2;
	}

	img = imgs[blocktype];//����������ͷ����ͼƬ
}

void Block::drop()//����
{
	for (int i = 0; i < 4; i++)
		loc[i].row++;//������һ
}

void Block::floatmove(int dis)//ˮƽλ��
{
	for (int i = 0; i < 4; i++)
		loc[i].col += dis;//��������ˮƽλ����
}

void Block::rotate()//��ת
{
	point center = loc[1];//ȡ��ת����
	for (int i = 0; i < 4; i++) {
		point tmp = loc[i];//����ԭʼ����
		loc[i].col = center.col - tmp.row + center.row;//��ת
		loc[i].row = center.row + tmp.col - center.col;
	}
}

void Block::paint(int leftdis, int updis)//��������
{
	for (int i = 0; i < 4; i++) {
		int x = leftdis + loc[i].col * size;//�õ�ͼ�����������߽�λ�ã������Ļ������
		int y = updis + loc[i].row * size;
		putimage(x, y, img);//��������
	}
}

IMAGE** Block::getimgs()//����ԭʼ������
{
	return imgs;
}

point* Block::getloc()//������������
{
	return loc;
}

void Block::solidarity(vector<vector<int>>& map)//�̻�
{
	for (int i = 0; i < 4; i++) {
		map[loc[i].row][loc[i].col] = blocktype;//�ڵ�ͼ�ϱ�Ƿ�������
	}
}

Block& Block::operator=(const Block& other)
{
	if (this == &other)return *this;//��ͬ�ͷ����Լ�

	this->blocktype = other.blocktype;//��������

	for (int i = 0; i < 4; i++) {
		this->loc[i] = other.loc[i];//��������
	}
	return *this;
}

int Block::getblocktype()//��ȡ����
{
	return blocktype;
}


