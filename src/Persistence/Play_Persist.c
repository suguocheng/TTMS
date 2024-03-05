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

static const char PLAY_DATA_FILE[] = "Play.dat"; //�ݳ����ļ������� 
static const char PLAY_DATA_TEMP_FILE[] = "PlayTmp.dat"; //�ݳ�����ʱ�ļ������� 
static const char PLAY_KEY_NAME[] = "Play"; //�ݳ��������� 
/*
��ʶ����TTMS_SCU_Play_Perst_SelAll 
�������ܣ����ڴ��ļ������������ݳ����ݡ�
����˵����list��play_list_t����ָ�룬ָ���ݳ������ͷָ�롣
�� �� ֵ�����ͣ���ʾ�ɹ��������ݳ��ĸ�����
*/
int Play_Perst_SelectAll(play_list_t list) {
	play_node_t *newNode;
	play_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, play_node_t);

	FILE *fp = fopen(PLAY_DATA_FILE, "rb");
	if (NULL == fp) { //�ļ�������
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
	
	long key = EntKey_Perst_GetNewKeys(PLAY_KEY_NAME, 1); //Ϊ���ݳ������ȡ
	if(key<=0)			//��������ʧ�ܣ�ֱ�ӷ���
		return 0;
	data->id = key;		//�����¶�����ص�UI��
	
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
				fseek(fp, -((int)sizeof(play_t)), SEEK_CUR);//ע�⸺��ǰ�Ʊ�ʾ�ı䱾�ڵ� 
				fwrite(data, sizeof(play_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;//�ҵ���1��û�ҵ�����0 
}

int Play_Perst_DeleteByID(int ID) {

	//��ԭʼ�ļ���������Ȼ���ȡ��������д�뵽�����ļ��У�����Ҫɾ����ʵ����˵���

	//��ԭʼ�����ļ�������
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

	//ɾ����ʱ�ļ�
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
//�������������Ŀ 

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













