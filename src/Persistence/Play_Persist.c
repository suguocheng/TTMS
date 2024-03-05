#include "Play_Persist.h"
#include "../Service/Play.h"
#include "EntityKey_Persist.h"
#include "../common/List.h"
#include <stdlib.h>
#include <stdio.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif
#include<string.h>
#include <assert.h>

static const char PLAY_DATA_FILE[] = "Play.dat"; //演出厅文件名常量 
static const char PLAY_DATA_TEMP_FILE[] = "PlayTmp.dat"; //演出厅临时文件名常量 
static const char PLAY_KEY_NAME[] = "Play"; //演出厅名常量 
/*
标识符：TTMS_SCU_Play_Perst_SelAll 
函数功能：用于从文件中载入所有演出数据。
参数说明：list是play_list_t类型指针，指向演出链表的头指针。
返 回 值：整型，表示成功载入了演出的个数。
*/
int Play_Perst_SelectAll(play_list_t list) {
	play_node_t *newNode;
	play_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, play_node_t);

	FILE *fp = fopen(PLAY_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(play_t), 1, fp)) {
			newNode = (play_node_t*) malloc(sizeof(play_node_t));
			if (!newNode) {
				printf(
						"Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;
}


int Play_Perst_Insert(play_t *data){
	int rtn=0;
	
	long key = EntKey_Perst_GetNewKeys(PLAY_KEY_NAME, 1); //为新演出分配获取
	if(key<=0)			//主键分配失败，直接返回
		return 0;
	data->id = key;		//赋给新对象带回到UI层
	
	FILE *fp = fopen(PLAY_DATA_FILE, "ab");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", PLAY_DATA_FILE);
		return 0;
	}
	rtn = fwrite(data, sizeof(play_t), 1, fp);
	fclose(fp);
	return rtn;
}
int Play_Perst_Update(const play_t *data){
	int found=0;
	assert(NULL!=data);

	FILE *fp = fopen(PLAY_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	play_t buf;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(play_t), 1, fp)) {
			if (buf.id == data->id) {
				fseek(fp, -((int)sizeof(play_t)), SEEK_CUR);//注意负号前移表示改变本节点 
				fwrite(data, sizeof(play_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;//找到改1，没找到还是0 
}

int Play_Perst_DeleteByID(int ID) {

	//将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。

	//对原始数据文件重命名
	if (rename(PLAY_DATA_FILE, PLAY_DATA_TEMP_FILE) < 0) {
		printf("Cannot open file %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	FILE* fpSour, * fpTarg;
	fpSour = fopen(PLAY_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour) {
		printf("Cannot open file %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(PLAY_DATA_FILE, "wb");
	if (NULL == fpTarg) {
		printf("Cannot open file %s!\n", PLAY_DATA_TEMP_FILE);
		return 0;
	}


	play_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(play_t), 1, fpSour)) {
			if (ID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(play_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(PLAY_DATA_TEMP_FILE);
	return found;
}
int Play_Perst_SelectByID(int id,play_t *buf){
	assert(NULL!=buf);

	FILE *fp = fopen(PLAY_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	play_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(play_t), 1, fp)) {
			if (id == data.id) {
				*buf = data;
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;

}
//根据名称载入剧目 

int Play_Perst_SelectByName(char name[], play_t* buf) {
	assert(NULL != buf);

	FILE* fp = fopen(PLAY_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	play_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(play_t), 1, fp)) {
			if (strcmp(name, data.name)) {
				*buf = data;
				found = data.id;
				break;
			}

		}
	}
	fclose(fp);

	return found;

}













