#include "Schedule_Persist.h"
#include "../Service/Schedule.h"
#include "EntityKey_Persist.h"	 
#include "../common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include "unistd.h"
#include <assert.h>
static const char SCHEDULE_DATA_FILE[] = "Schedule.dat"; //演出计划文件名常量 
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat"; //演出计划临时文件名常量 
static const char SCHEDULE_KEY_NAME[] = "Schedule"; //演出计划常量 
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id)
{
	schedule_node_t* newNode;
	int recCount = 0;
	schedule_t data;
	FILE* pf = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == pf) { //文件不存在
		return 0;
	}
	assert(NULL != list);

	List_Free(list, schedule_node_t);
	if (NULL == pf)
	{
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return recCount;
	}
	while (!feof(pf))
	{
		if (fread(&data, sizeof(schedule_t), 1, pf)) {
			if (data.play_id == play_id)
			{
				newNode = (schedule_node_t*)malloc(sizeof(schedule_node_t));
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
	}
	fclose(pf);
	return recCount;
}
int Schedule_Perst_Insert(schedule_t* data)
{
	int rtn = 0;
	FILE* fp = fopen(SCHEDULE_DATA_FILE, "ab");
	if (NULL == fp)
	{
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return rtn;
	}
	long key = EntKey_Perst_GetNewKeys(SCHEDULE_KEY_NAME, 1); //为新演出计划分配获取
	if (key <= 0)			//主键分配失败，直接返回
		return 0;
	data->id = key;		//赋给新对象带回到UI层
	rtn = fwrite(data, sizeof(schedule_t), 1, fp);
	fclose(fp);
	return rtn;


}
int Schedule_Perst_Update(const schedule_t* data)
{
	int found = 0;

	FILE* fp = fopen(SCHEDULE_DATA_FILE, "rb+");
	if (NULL == fp)
	{
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return found;
	}
	schedule_t	buf;
	while (!feof(fp))
	{
		if (fread(&buf, sizeof(schedule_t), 1, fp))
		{
			if (buf.id == data->id)
			{
				fseek(fp, -((long)sizeof(schedule_t)), SEEK_CUR);
				fwrite(data, sizeof(schedule_t), 1, fp);
				found = 1;
				break;
			}
		}
	}
	fclose(fp);
	return found;
}
int Schedule_Perst_RemByID(int id)
{
	int found = 0;
	if (rename(SCHEDULE_DATA_FILE, SCHEDULE_DATA_TEMP_FILE) < 0) {
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}
	FILE* from = fopen(SCHEDULE_DATA_TEMP_FILE,"rb");
	FILE* obj = fopen(SCHEDULE_DATA_FILE, "wb");
	schedule_t buf;
	if (from == NULL || obj == NULL)
	{
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}
	while (!feof(from))
	{
		if (fread(&buf, sizeof(schedule_t), 1, from))
		{
			if (id == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(schedule_t), 1, obj);
		}
	}
	fclose(from);
	fclose(obj);
	remove(SCHEDULE_DATA_TEMP_FILE);
	return found;
}
int Schedule_Perst_SelectByID(int id, schedule_t* buf) {
	assert(NULL != buf);

	FILE* fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	schedule_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
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
int Schedule_Perst_RemByPlayID(int play_id)
{
	int found = 0;
	if (rename(SCHEDULE_DATA_FILE, SCHEDULE_DATA_TEMP_FILE) < 0) {
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}
	FILE* from = fopen(SCHEDULE_DATA_TEMP_FILE, "rb");
	FILE* obj = fopen(SCHEDULE_DATA_FILE, "wb");
	schedule_t buf;
	if (from == NULL || obj == NULL)
	{
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}
	while (!feof(from))
	{
		if (fread(&buf, sizeof(schedule_t), 1, from))
		{
			if (play_id == buf.play_id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(schedule_t), 1, obj);
		}
	}
	fclose(from);
	fclose(obj);
	remove(SCHEDULE_DATA_TEMP_FILE);
	return found;
}
int Schedule_Perst_SelectAll(schedule_list_t list) {
	schedule_node_t* newNode;
	int recCount = 0;
	schedule_t data;
	FILE* fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}
	assert(NULL != list);

	List_Free(list, schedule_node_t);
	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
			newNode = (schedule_node_t*)malloc(sizeof(schedule_node_t));
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
int Schedule_Perst_SelectByID_ticket(int id, schedule_t* buf)//根据票传下来的id找演出
{
	assert(NULL != buf);

	int found = 0;
	schedule_t data;
	FILE* fp = fopen(SCHEDULE_DATA_FILE, "rb");


	if (NULL == fp)
	{
		printf("the file cannot open!\n");
		return 0;
	}
	while (!feof(fp))
	{
		if (fread(&data, sizeof(schedule_t), 1, fp))
		{
			if (data.id == id)
			{
				*buf = data;
				found = 1;
				break;
			}
		}
	}

	fclose(fp);

	return found;
}


