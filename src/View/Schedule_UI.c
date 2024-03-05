#include "../View/Schedule_UI.h"
#include "../Common/List.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include"../Persistence/Schedule_Persist.h"
#include"Ticket_UI.h"
#include <stdlib.h>
#include <stdio.h>
#include "../Service/Studio.h"
static const int SCHEDULE_PAGE_SIZE = 5;
void Schedule_UI_MgtEntry(int play_id)
{
	int i, id;
	char choice;
	schedule_list_t head;
	schedule_node_t* pos;
	Pagination_t paging;
	List_Init(head, schedule_node_t);
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;
	paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);
	Paging_Locate_FirstPage(head, paging);
	do {
		system("cls");
		printf(
			"\n==================================================================\n");
		printf(
			"-------------------- Projection Schedule List --------------------\n\n");
		printf("%5s  %5s  %5s  %10s  %10s %10s\n", "ID", "playID", "studioID",
			"start date", "start time","seat count");
		printf(
			"------------------------------------------------------------------\n");
		//显示数据
		Paging_ViewPage_ForEach(head, paging, schedule_node_t, pos, i){
			printf(" %2d  %2d  %3d  %8d%2d%2d  %5d%2d%2d %10\n", 
				pos->data.id, pos->data.play_id, pos->data.studio_id,
				pos->data.date.year, pos->data.date.month, pos->data.date.day
				, pos->data.time.hour, pos->data.time.minute
				, pos->data.time.second
				, pos->data.seat_count);
		}

		printf(
			"------- Total Records:%2d ----------------------- Page %2d/%2d ----\n\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		printf(
			"------------------------------------------------------------------\n");
		printf(
			"[P]revPage | [N]extPage | [A]dd | [D]elete | [U]pdate | [R]eturn | [T]icket");
		printf(
			"\n==================================================================\n");
		printf("Your Choice:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
		switch (choice) {
		case 'a':
		case 'A':
			if (Schedule_UI_Add(play_id)) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Schedule_Srv_FetchByPlay(head,play_id);
				;
				Paging_Locate_LastPage(head, paging, schedule_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Schedule_UI_Delete(id)) {	//从新载入数据
				paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);
				List_Paging(head, paging, schedule_node_t);
			}
			break;

		case 'u':
		case 'U':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Schedule_UI_Modify(id)) {	//从新载入数据
				paging.totalRecords = Schedule_Srv_FetchByPlay(head,play_id);
				List_Paging(head, paging, schedule_node_t);
			}
			break;
		case 't':
		case 'T':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Schedule_Srv_FetchByID(id, &(pos->data))) {	//从新载入数据
				Ticket_UI_MgtEntry(id);
			}
			else {
				printf("cannot find this schedule");
			}
			break;

		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, schedule_node_t);
}
int Schedule_UI_Add(int play_id)
{
	schedule_t rec;
	int newRecCount = 0;
	char choice;
	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("------------------  Add New Projection Schedule  -----------------\n");
		printf("------------------------------------------------------------------\n");
		
		printf("Room ID:");
		fflush(stdin);
		scanf("%d",&(rec.studio_id));
		printf("Play ID:");
		scanf("%d", &(rec.play_id));

		printf("start date:");
		fflush(stdin);
		printf("year:");
		scanf("%d", &(rec.date.year));
		printf("month:");
		fflush(stdin);
		scanf("%d", &(rec.date.month));
		printf("day:");
		fflush(stdin);
		scanf("%d", &(rec.date.day));
		printf("start time:");
		fflush(stdin);
		printf("hour:");
		scanf("%d", &(rec.time.hour));
		printf("minute:");
		fflush(stdin);
		scanf("%d", &(rec.time.minute));
		printf("second:");
		fflush(stdin);
		scanf("%d", &(rec.time.second));
		studio_t buf;
		Studio_Srv_FetchByID(rec.studio_id, &buf);
		rec.seat_count = buf.seatsCount;
		printf("=======================================================\n");
		if (Schedule_Srv_Add(&rec)) {
			newRecCount++;
			printf("The new schedule added successfully!\n");
		}
		else
			printf("The new schedule added failed!\n");
		printf("-------------------------------------------------------\n");
		printf("[A]dd more, [R]eturn:");
		fflush(stdin);
		getchar();
		choice = getchar();
		fflush(stdin);

	}while(choice != 'r' && choice != 'R');
	return newRecCount;
}
int Schedule_UI_Modify(int id)
{
	int rtn = 0;
	schedule_t rec;
	int newRecCount = 0;
	char choice;
	schedule_list_t list;

	/*Load record*/
	if (!Schedule_Srv_FetchByID(id, &rec)) {
		printf("The Schedule does not exist!\nPress [Enter] key to return!\n");
		getchar();
		return 0;
	}
	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("-------------------  Update Projection Schedule  -----------------\n");
		printf("------------------------------------------------------------------\n");
		printf("Schedule ID:%d\n", rec.id);
		printf("Play ID[%d]:", rec.play_id);
		fflush(stdin);
		getchar();
		scanf("%d", &(rec.play_id));
		List_Init(list, schedule_node_t);
		printf("start date:");
		fflush(stdin);
		printf("year:");
		scanf("%d", &(rec.date.year));
		printf("month:");
		fflush(stdin);
		scanf("%d", &(rec.date.month));
		printf("day:");
		fflush(stdin);
		scanf("%d", &(rec.date.day));
		printf("start time:");
		fflush(stdin);
		printf("hour:");
		scanf("%d", &(rec.time.hour));
		printf("minute:");
		fflush(stdin);
		scanf("%d", &(rec.time.minute));
		printf("second:");
		fflush(stdin);
		scanf("%d", &(rec.time.second));
		printf("seat count:");
		scanf("%d", &(rec.seat_count));
		printf("=======================================================\n");
		if (Schedule_Srv_Modify(&rec)) {
			newRecCount++;
			printf("The new schedule updated successfully!\n");
		}
		else
			printf("The new schedule updated failed!\n");
		printf("-------------------------------------------------------\n");
		printf("press enter to continue, [R]eturn:");
		fflush(stdin);
		getchar();
		scanf("%c", &choice);

	} while (choice != 'r' && choice != 'R');
	return newRecCount;

}
int Schedule_UI_Delete(int id)
{


	int rtn = 0;

	if (Schedule_Srv_DeleteByID(id)) {
		printf(
			"The Schedule deleted successfully!\nPress [Enter] key to return!\n");
		rtn = 1;
	}
	else {
		printf("The Schedule does not exist!\nPress [Enter] key to return!\n");
	}

	getchar();
	return rtn;
}




