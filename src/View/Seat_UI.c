/*
* Copyright(C), 2007-2008, XUPT Univ.
* ������ţ�TTMS_UC_02
* File name: Seat_UI.c
* Description : ������λ���������
* Author:   XUPT
* Version:  v.1
* Date: 	2015��4��22��
*/

#include "Seat_UI.h"
#include "../Service/Seat.h"
#include "../Service/Account.h"
#include "../Service/Studio.h"
#include "../Common/list.h"
#include <stdio.h>
#include <string.h>
#include<Windows.h>
inline char Seat_UI_Status2Char(seat_status_t status) {
	if (status == 0)
	{
		return ' ';
	}
	else if (status == 1)
	{
		return '#';
	}
	else
	{
		return '?';
	}
}


inline seat_status_t Seat_UI_Char2Status(char statusChar) {
	if (statusChar == ' ')
	{
		return SEAT_NONE;
	}
	else if (statusChar == '#')
	{
		return SEAT_GOOD;
	}
	else
	{
		return SEAT_BROKEN;
	}
}


void Seat_UI_MgtEntry(int roomID) {
	studio_t buf;
	seat_list_t list;
	char param;

	if (!Studio_Srv_FetchByID(roomID, &buf))
	{
		printf("演出厅不存在\n按回车键返回\n");
		getchar();
		return;
	}
	List_Init(list, seat_node_t);
	if (!Seat_Srv_FetchByRoomID(list, roomID))
	{
		buf.seatsCount = Seat_Srv_RoomInit(list, roomID, buf.rowsCount, buf.colsCount);
		Studio_Srv_Modify(&buf);
	}
	do
	{
		Sleep(700);
		system("cls");
		printf("==================================================================\n");
		printf("---------------------- Projection Seat List ----------------------\n\n");
		printf("row:%d                   colum:%d                  seatsnumber:%d\n", buf.rowsCount, buf.colsCount, buf.seatsCount);
		printf("------------------------------------------------------------------\n");
		int x = 0, y = 1;
		int i, j;
		for (i = 0; i <= buf.rowsCount; i++)
		{
			for (j = 0; j <= buf.colsCount; j++)
			{
				if (i == 0) {
					printf("%3d", x++);
				}
				else if (j == 0) {
					printf("%3d", y++);
				}
				else {
					int flag = 0;
					seat_node_t* temp;
					List_ForEach(list, temp)
					{
						if (temp->data.row == i && temp->data.column == j)
						{
							flag = 1;
							printf("%3c", Seat_UI_Status2Char(temp->data.status));
							break;
						}
					}
					if (!flag)
						printf("  x");//此处没有座位
				}
			}
			putchar('\n');
		}
		printf("------------------------------------------------------------------\n");
		printf("[A]dd | [D]elete | [U]pdate | [R]eturn\n");
		printf("==================================================================\n");
		scanf("%c", &param);
		if (param == 'A' || param == 'a')
		{
			int row, column;
			printf("请输入所要添加座位的行数：");
			scanf("%d", &row);
			printf("请输入所要添加座位的列数：");
			scanf("%d", &column);
			getchar();
			if (!Seat_UI_Add(list, roomID, row, column))
			{
				printf("添加失败\n");
				
			}
			else
			{
				buf.seatsCount = buf.seatsCount + 1;
				Studio_Srv_Modify(&buf);
				Seat_Srv_FetchByRoomID(list, roomID);
				printf("添加成功\n");
			}
		}
		else if (param == 'D' || param == 'd')
		{
			int row, column;
			printf("请输入所要删除座位的行数：");
			scanf("%d", &row);
			printf("请输入所要删除座位的列数：");
			scanf("%d", &column);
			getchar();
			if (!Seat_UI_Delete(list, row, column))
			{
				printf("删除失败\n");
			}
			else
			{
				buf.seatsCount = buf.seatsCount - 1;
				Studio_Srv_Modify(&buf);
				Seat_Srv_FetchByRoomID(list, roomID);
				printf("删除成功\n");
			}
		}
		else if (param == 'U' || param == 'u')
		{
			int row, column;
			printf("请输入所要修改座位的行数：");
			scanf("%d", &row);
			printf("请输入所要修改座位的列数：");
			scanf("%d", &column);
			getchar();
			if (!Seat_UI_Modify(list, row, column))
			{
				printf("修改失败\n");
			}
			else
			{
				Seat_Srv_FetchByRoomID(list, roomID);
				printf("修改成功\n");
			}

		}
	} while (param != 'r' && param != 'R');
	List_Destroy(list, seat_node_t);
}



int Seat_UI_Add(seat_list_t list, int roomID, int row, int column) {  //����һ����λ


    seat_list_t tmp = Seat_Srv_FindByRowCol(list, row, column);
    if (tmp == NULL) {
        seat_t temp;

        temp.column = column;
        temp.row = row;
        temp.status = SEAT_GOOD;
        temp.roomID = roomID;

        if (Seat_Srv_Add(&temp)) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}



int Seat_UI_Modify(seat_list_t list, int row, int column) {


    seat_list_t tmp = Seat_Srv_FindByRowCol(list, row, column);

    if (tmp != NULL) {
        seat_t temp;
        Seat_Perst_SelectByID(tmp->data.id, &temp);
        char choice;
        printf(
            "*******************************************************************\n");
        printf(
            "[#]Good | [?]Break | [ ]Empty | the seat's situation is %c", Seat_UI_Status2Char(temp.status));
        printf(
            "\n==================================================================\n");
        printf("Your Choice:");
        scanf("%c", &choice);
        getchar();
        tmp->data.status = Seat_UI_Char2Status(choice);

        if (Seat_Srv_Modify(&tmp->data)) {

            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}



int Seat_UI_Delete(seat_list_t list, int row, int column) {

    seat_list_t tmp = Seat_Srv_FindByRowCol(list, row, column);

    if (tmp != NULL) {
        if (Seat_Srv_DeleteByID(tmp->data.id)) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}
