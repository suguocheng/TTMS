
#ifndef PLAY_UI_H_
#define PLAY_UI_H_
#include "../Common/list.h"
#include "../Service/play.h"
//static const int PLAY_PAGE_SIZE = 5;

#include <stdio.h>

void Play_UI_MgtEntry(void);//���������Ŀ����ں�������ʾ��ǰ�����ݣ����ṩ������ӡ��޸ļ�ɾ�����ܲ��������

int play_UI_Add(void);//����srv��add 

int Play_UI_Query(void);//����srv��FetchById���ز��ҳɹ���� 

int Play_UI_Modify(int id);//�ȵ���fech�������ң��ٵ���modify����

int Play_UI_Del(int id);// �ȵ���fech���ң��ٵ���delɾ�� 

#endif /* PLAY_UI_H_ */

