/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_02
* File name: Seat.h
* Description : 设置座位用例持久化层
* Author:   XUPT
* Version:  v.1
* Date: 	2015年4月22日
*/

#include "EntityKey_Persist.h"
#include "Seat_Persist.h"
#include "../Service/Seat.h"
#include "../Common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>



static const char SEAT_DATA_FILE[] = "Seat.dat";
static const char SEAT_DATA_TEMP_FILE[] = "SeatTmp.dat";

//添加对象主键标识名称
static const char SEAT_KEY_NAME[] = "Seat";

/*
函数功能：用于向文件中添加一个新座位数据。
参数说明：data为seat_t类型指针，表示需要添加的座位数据结点。
返 回 值：整型，表示是否成功添加了座位的标志。
*/


int Seat_Perst_Insert(seat_t* data) {
	assert(data != NULL);
	long key = EntKey_Perst_GetNewKeys(SEAT_KEY_NAME, 1);
	if (key <= 0)
	{
		return 0;
	}
	else
	{
		data->id = key;
	}
	int rtn = 0;
	FILE* fp = fopen(SEAT_DATA_FILE, "ab");
	if (fp != NULL)
	{
		fwrite(data, sizeof(seat_t), 1, fp);
		rtn = 1;
	}
	else
	{
		return 0;
	}
	fclose(fp);
	return rtn;
}

/*
标识符：TTMS_SCU_Seat_Perst_InsertBatch
函数功能：用于向文件中添加一批座位数据。
参数说明：list为seat_list_t类型，表示需要添加的一批座位的链表头指针。
返 回 值：整型，表示成功添加一批座位的个数。
*/
int Seat_Perst_InsertBatch(seat_list_t list) {
	assert(list != NULL);
	int len = 0;
	seat_node_t* p;
	int rtn = 0;
	List_ForEach(list, p)
	{
		len++;
	}
	int key = EntKey_Perst_GetNewKeys(SEAT_KEY_NAME, len);
	if (key <= 0)
	{
		return 0;
	}
	else
	{
		FILE* fp = fopen(SEAT_DATA_FILE, "ab");
		if (fp != NULL)
		{
			List_ForEach(list, p)
			{
				p->data.id = key++;
				fwrite(&p->data, sizeof(seat_t), 1, fp);
				rtn++;
			}
		}
		else
		{
			printf("打开文件失败\n");
			return 0;
		}
		fclose(fp);
	}
	return rtn;
}

/*
标识符：TTMS_SCU_Seat_Perst_Update
函数功能：用于在文件中更新一个座位数据。
参数说明：data为seat_t类型指针，表示需要更新的座位数据结点。
返 回 值：整型，表示是否成功更新了座位的标志。
*/
int Seat_Perst_Update(const seat_t* seatdata) {
	assert(seatdata != NULL);
	int found = 0;
	seat_t buf;
	FILE* fp = fopen(SEAT_DATA_FILE, "rb+");
	if (fp != NULL)
	{
		while (feof(fp) == 0)
		{
			fread(&buf, sizeof(seat_t), 1, fp);
			if (buf.id == seatdata->id)
			{
				fseek(fp, -((int)sizeof(seat_t)), SEEK_CUR);
				fwrite(seatdata, sizeof(seat_t), 1, fp);
				found = 1;
				break;
			}
		}
	}
	else
	{
		printf("打开文件失败\n");
		return 0;
	}
	fclose(fp);
	return found;
}

/*
标识符：TTMS_SCU_Seat_Perst_DelByID
函数功能：用于从文件中删除一个座位的数据。
参数说明：参数ID为整型，表示需要删除的座位ID。
返 回 值：整型，表示是否成功删除了座位的标志。
*/
int Seat_Perst_DeleteByID(int ID) {

	//将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。

	//对原始数据文件重命名
	if (rename(SEAT_DATA_FILE, SEAT_DATA_TEMP_FILE) < 0) {
		printf("Cannot open file %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	FILE* fpSour, * fpTarg;
	fpSour = fopen(SEAT_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour) {
		printf("Cannot open file %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(SEAT_DATA_FILE, "wb");
	if (NULL == fpTarg) {
		printf("Cannot open file %s!\n", SEAT_DATA_TEMP_FILE);
		return 0;
	}


	seat_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(seat_t), 1, fpSour)) {
			if (ID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(seat_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(SEAT_DATA_TEMP_FILE);
	return found;

}

/*
标识符：TTMS_SCU_Seat_Perst_DelAllByID
函数功能：根据编号用于从文件中删除座位数据。
参数说明：参数roomID为整型，表示演出厅ID。
返 回 值：整型，表示是否成功删除了座位的标志。
*/
int Seat_Perst_DeleteAllByRoomID(int roomID) {


	//将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。

	//对原始数据文件重命名
	if (rename(SEAT_DATA_FILE, SEAT_DATA_TEMP_FILE) < 0) {
		printf("Cannot open file %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	FILE* fpSour, * fpTarg;
	fpSour = fopen(SEAT_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour) {
		printf("Cannot open file %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(SEAT_DATA_FILE, "wb");
	if (NULL == fpTarg) {
		printf("Cannot open file %s!\n", SEAT_DATA_TEMP_FILE);
		return 0;
	}


	seat_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(seat_t), 1, fpSour)) {
			if (roomID == buf.roomID) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(seat_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(SEAT_DATA_TEMP_FILE);
	return found;
}

/*
标识符：TTMS_SCU_Studio_Perst_SelByID
函数功能：用于从文件中载入一个座位的数据。
参数说明：第一个参数ID为整型，表示需要载入数据的座位ID；第二个参数buf为seat_t指针，指向载入座位数据的指针。
返 回 值：整型，表示是否成功载入了座位的标志。
*/
int Seat_Perst_SelectByID(int ID, seat_t* buf) {
	assert(buf != NULL);
	int found = 0;
	FILE* fp = fopen(SEAT_DATA_FILE, "rb");
	seat_t data;
	if (fp != NULL)
	{
		do
		{
			fread(&data, sizeof(seat_t), 1, fp);
			if (ID == data.id)
			{
				*buf = data;
				found = 1;
				break;
			}
		} while (feof(fp) == 0);
	}
	else
	{
		printf("打开文件失败\n");
		return 0;
	}
	fclose(fp);
	return found;
}

/*
标识符：TTMS_SCU_Seat_Perst_SelAll
函数功能：用于从文件中载入所有座位数据。
参数说明：list为seat_list_t类型，表示将要载入的座位链表头指针。
返 回 值：整型，成功载入座位的个数。
*/
int Seat_Perst_SelectAll(seat_list_t list) {
	assert(list != NULL);
	int recCount = 0;
	List_Free(list, seat_node_t);
	FILE* fp = fopen(SEAT_DATA_FILE, "rb");
	seat_t data;
	seat_node_t* newnode;
	if (fp != NULL)
	{
		do
		{
			fread(&data, sizeof(seat_t), 1, fp);
			newnode = (seat_node_t*)malloc(sizeof(seat_node_t));
			newnode->data = data;
			List_AddTail(list, newnode);
			recCount++;
		} while (feof(fp) != 0);
	}
	else
	{
		printf("打开文件失败\n");
		return 0;
	}
	fclose(fp);
	return recCount;

}

/*
标识符：TTMS_SCU_Seat_Perst_SelByRoomID
函数功能：用于在文件中根据演出厅ID载入所有座位数据。
参数说明：第一个参数list为seat_list_t类型，表示将要载入的座位链表头指针，第二个参数roomID为整型，表示演出厅ID。
返 回 值：整型，表示成功载入了演出厅座位的个数。
*/
int Seat_Perst_SelectByRoomID(seat_list_t list, int roomID) {


	seat_list_t newNode;
	seat_t data;
	int recCount = 0;

	assert(NULL != list);

	List_Free(list, seat_node_t);

	FILE* fp = fopen(SEAT_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		printf("cuo wu da kai %s", SEAT_DATA_FILE);
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(seat_t), 1, fp)) {
			newNode = (seat_node_t*)malloc(sizeof(seat_node_t));
			if (!newNode) {
				printf(
					"Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
				break;
			}
			if (data.roomID == roomID) {
				newNode->data = data;
				List_AddTail(list, newNode);
				recCount++;
			}
		}
	}
	fclose(fp);
	return recCount;
}
