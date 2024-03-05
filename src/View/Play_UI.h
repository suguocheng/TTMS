
#ifndef PLAY_UI_H_
#define PLAY_UI_H_
#include "../Common/list.h"
#include "../Service/play.h"
//static const int PLAY_PAGE_SIZE = 5;

#include <stdio.h>

void Play_UI_MgtEntry(void);//界面层管理剧目的入口函数，显示当前的数据，并提供数据添加、修改及删除功能操作的入口

int play_UI_Add(void);//调用srv的add 

int Play_UI_Query(void);//调用srv层FetchById返回查找成功与否 

int Play_UI_Modify(int id);//先调用fech函数查找，再调用modify更改

int Play_UI_Del(int id);// 先调用fech查找，再调用del删除 

#endif /* PLAY_UI_H_ */

