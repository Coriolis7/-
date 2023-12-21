#include<vector>
using namespace std;
#ifndef BLOCK_H
#define BLOCK_H
struct point {
	int row, col;
};

class Block
{
public:
	Block();
	void drop();//����
	void floatmove(int dis);//ˮƽ�ƶ�
	void rotate();//��ת
	void paint(int leftborder, int upborder);//��ʾ����
	static IMAGE** getimgs();//����ԭʼ��ͼƬ
	point* getloc();//��ȡ���꣬������˽�г�Ա����Ҫ�ӿڸ������ʹ��
	void solidarity(vector<vector<int>>& map);//�̻�������ͼ����
	Block& operator=(const Block& other);//����Block�ĸ���
	int getblocktype();//��ȡ��������
private:
	int blocktype;//��������
	point loc[4];//�ĸ����ӵ�����
	IMAGE* img;//���ĸ����ӵ�ͼƬ
	static IMAGE* imgs[7];//ԭʼͼƬ
	static int size;//�����С
};

#endif // !BLOCK_H


