/*
* Copyright(C), 2007-2008, XUPT Univ.	
* ������ţ�TTMS_UC_02 
* File name: Seat.c		  
* Description : ������λ����ҵ���߼���	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015��4��22��	
*/

#include <stdlib.h>
#include "../Common/List.h"
#include "Seat.h"
#include"../Persistence/Seat_Persist.h"
/*
�������ܣ��������һ������λ���ݡ�
����˵����dataΪseat_t����ָ�룬��ʾ��Ҫ��ӵ���λ���ݽ�㡣
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ��������λ�ı�־��
*/
int Seat_Srv_Add(const seat_t *data){
	return Seat_Perst_Insert(data);
}

/*
�������ܣ����������λ���ݡ�
����˵����listΪseat_list_t����ָ�룬��ʾ��Ҫ��ӵ�������λ�����γɵ�����
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ������������λ�ı�־��
*/
int Seat_Srv_AddBatch(seat_list_t list){
	// �벹������
       return 0;
}

/*
�������ܣ������޸�һ����λ���ݡ�
����˵����dataΪseat_t����ָ�룬��ʾ��Ҫ�޸ĵ���λ���ݽ�㡣
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ��޸�����λ�ı�־��
*/
int Seat_Srv_Modify(const seat_t *data){
	return Seat_Perst_Update(data);
}

/*
�������ܣ�������λIDɾ��һ����λ��
����˵����IDΪ���ͣ���ʾ��Ҫɾ������λ���ݽ�㡣
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ�ɾ������λ�ı�־��
*/
int Seat_Srv_DeleteByID(int ID){
	return Seat_Perst_DeleteByID(ID);
}

/*
�������ܣ�������λID��ȡ��λ���ݡ�
����˵������һ������IDΪ���ͣ���ʾ��λID���ڶ�������bufΪseat_tָ�룬ָ�����ȡ����λ���ݽ�㡣
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ���ȡ����λ�ı�־��
*/
int Seat_Srv_FetchByID(int ID, seat_t *buf){
	return Seat_Perst_SelectByID(ID,buf);
}

/*
�������ܣ������ݳ���IDɾ��������λ��
����˵����roomIDΪ���ͣ���ʾ��Ҫɾ��������λ���ݳ���ID��
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ�ɾ�����ݳ���������λ�ı�־��
*/
int Seat_Srv_DeleteAllByRoomID(int roomID){
	return Seat_Perst_DeleteAllByRoomID(roomID);
}

/*
�������ܣ������ݳ���ID��ȡ������λ��������λ���� �� 
����˵������һ������listΪseat_list_t���ͣ���ʾ��ȡ������Ϊ����ͷָ�룬�ڶ�������roomIDΪ���ͣ���ʾ��ȡ������λ���ݳ���ID�� 
�� �� ֵ�����ͣ��ݳ�������λ������ 
*/
int Seat_Srv_FetchByRoomID(seat_list_t list, int roomID){
	Seat_Perst_SelectByRoomID(list,roomID);
	int SeatCount=0;
	seat_list_t curPos;
	List_ForEach(list, curPos)
    {
    	SeatCount++;
	}
	Seat_Srv_SortSeatList(list);
	return SeatCount;
}

/*
�������ܣ������ݳ���ID��ø��ݳ�������Ч��λ��
����˵������һ������listΪseat_list_t���ͣ���ʾ��ȡ������Ч��λ����ͷָ�룬�ڶ�������roomIDΪ���ͣ���ʾ��Ҫ��ȡ��Ч��λ���ݳ���ID��
�� �� ֵ�����ͣ���ʾ�ݳ�������Ч��λ������
*/
int Seat_Srv_FetchValidByRoomID(seat_list_t list, int roomID)
{
	int SeatCount = 0;
	SeatCount = Seat_Perst_SelectByRoomID(list, roomID);
	seat_list_t temp = list;
	while (list->next != temp) {
		list = list->next;
		if (list->data.status != SEAT_GOOD) {
			SeatCount--;
		}
	}
	Seat_Srv_SortSeatList(list);
	return SeatCount;
}

/*
�������ܣ����ݸ����ݳ������С�������ʼ���ݳ�����������λ���ݣ�����ÿ����λ��㰴�в�����λ����
����˵������һ������listΪseat_list_t����ָ�룬ָ����λ����ͷָ�룬�ڶ�������rowsCountΪ���ͣ���ʾ��λ�����кţ�����������colsCountΪ���ͣ���ʾ��λ�����кš�
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ���ʼ�����ݳ�����������λ��
*/
int Seat_Srv_RoomInit(seat_list_t list, int roomID, int rowsCount, int colsCount) {

	seat_list_t tem;
	for (int i = 1; i <= rowsCount; i++) {
		for (int j = 1; j <= colsCount; j++) {

			tem = (seat_list_t)malloc(sizeof(seat_node_t));
			tem->data.column = j;
			tem->data.row = i;
			tem->data.roomID = roomID;
			tem->data.status = SEAT_GOOD;
			List_AddTail(list, tem);
		}
	}
	return Seat_Perst_InsertBatch(list);

}

/*
�������ܣ�����λ����list����λ�кš��кŽ�������
����˵����listΪseat_list_t���ͣ���ʾ��������λ����ͷָ�롣
�� �� ֵ���ޡ�
*/
void Seat_Srv_SortSeatList(seat_list_t list) {
    if(list!=NULL)
	{
		list->prev->next=NULL;
		seat_list_t listLeft=list->next;
		list->next=list->prev=list;
		while(listLeft!=NULL)
		{
			seat_list_t p=listLeft;
			listLeft=listLeft->next;
			Seat_Srv_AddToSoftedList(list,p);
		}
		return;
	}
	else
	{
		return;
	}
}

/*
�������ܣ���һ����λ�����뵽���������λ�����С�
����˵������һ������listΪseat_list_t���ͣ���ʾ�����������λ����ͷָ�룬�ڶ�������nodeΪseat_node_tָ�룬��ʾ��Ҫ�������λ���ݽ�㡣
�� �� ֵ���ޡ�
*/
void Seat_Srv_AddToSoftedList(seat_list_t list, seat_node_t *node) {
    if(list!=NULL)
    {
    	seat_node_t *p=list;
    	while(p!=list&&(p->data.row<node->data.row||(p->data.row==node->data.row&&p->data.column<node->data.column)))
    	{
    		p=p->next;
		}
		List_InsertBefore(p,node)
	}
	else
	{
		List_AddTail(list,node); 
	}
}

/*
�������ܣ�������λ���С��кŻ�ȡ��λ���ݡ�
����˵������һ������listΪseat_list_t���ͣ���ʾ��λ����ͷָ�룬
         �ڶ�������rowΪ���ͣ���ʾ����ȡ��λ���кţ�����������columnΪ���ͣ���ʾ����ȡ��λ���кš�
�� �� ֵ��Ϊseat_node_tָ�룬��ʾ��ȡ������λ���ݡ�
*/
seat_node_t * Seat_Srv_FindByRowCol(seat_list_t list, int row, int column) {
	seat_node_t *curPos;
    List_ForEach(list, curPos)
    {
    	if(curPos->data.row==row&&curPos->data.column==column)
    	{
    		return curPos;
		}
	}
	return NULL;
}

/*
�������ܣ�������λID�������л�ȡ��λ���ݡ�
����˵������һ������listΪseat_list_t���ͣ�ָ����λ���������ڶ�������IDΪ���ͣ���ʾ��λID��
�� �� ֵ��seat_node_t���ͣ���ʾ��ȡ����λ���ݡ�
*/

seat_node_t* Seat_Srv_FindByID(seat_list_t list, int rowID) {
	// �벹������
	assert(NULL != list);
	seat_node_t* p;

	List_ForEach(list, p)
		if (p->data.id == rowID)
			return p;

	return NULL;
}


