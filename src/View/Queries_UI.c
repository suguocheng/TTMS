#include "../Common/List.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include <stdio.h>
#include <stdlib.h>
#include"../Service/Ticket.h"
const int SCHEDULE_PAGE_SIZE = 5;
/*
标识符：TTMS_SCU_Schdule_Perst_SelAll
函数功能：查询演出界面
*/
void Schdule_UI_ListAll(void) {
	int i, id;
	char choice;
	char name[30];
	schedule_list_t head;
	schedule_node_t* pos;
	Pagination_t paging;
	List_Init(head, schedule_node_t);
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;
	paging.totalRecords = Schedule_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	do {
		system("cls");
		printf(
			"\n==================================================================\n");
		printf(
			"-------------------- Projection Schedule List --------------------\n\n");
		printf("%5s  %5s  %5s  %10s  %10s %10s\n", "ID", "playID", "studioID",
			"start date", "start time", "seat count");
		printf(
			"------------------------------------------------------------------\n");
		//显示数据
		
			Paging_ViewPage_ForEach(head, paging, schedule_node_t, pos, i) {
				if (pos->data.id != 33 && pos->data.id != 34)
				{
					printf(" %6d  %10d  %5d  %4d%2d%2d  %4d%2d%2d %10\n",
						pos->data.id, pos->data.play_id, pos->data.studio_id,
						pos->data.date.year, pos->data.date.month, pos->data.date.day
						, pos->data.time.hour, pos->data.time.minute
						, pos->data.time.second
						, pos->data.seat_count);
				}
			}
		

		printf(
			"------- Total Records:%2d ----------------------- Page %2d/%2d -----\n\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		printf(
			"------------------------------------------------------------------\n");
		printf(
			"[P/p]上一页    |   [N/n]下一页  |  [q/Q] 根据名字筛选演出计划\n[r/R]返回上层  |");
		printf(
			"\n==================================================================\n");
		printf("Your Choice:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
		switch (choice) {
		case 'q':
		case 'Q':
			system("cls");
			printf("请输入要查询的剧目名字:");
			scanf("%s", &name);
			getchar();
			if (Schedule_UI_Query(name))
			{
				
				List_Init(head, schedule_node_t);
				paging.totalRecords = Schedule_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, schedule_node_t);
			}
			break;

		case 'P':
		case 'p':
			system("cls");
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
			}
			break;
		case 'N':
		case'n':
			system("cls");
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
			}
			break;
		}
	} while (choice != 'r'&& choice != 'R');
	system("cls");
	List_Destroy(head, schedule_node_t);
	return 0;
}

/*
标识符： TTMS_SCU_Schedule_UI_Qry
功能：根据剧目名称获取演出计划
*/
int Schedule_UI_Query(char* play_name) {
	play_list_t list_p;
	schedule_list_t list_s;
	char choice;
	List_Init(list_p, play_node_t);
	List_Init(list_s, schedule_node_t);
	int schid;
	int id=Play_Srv_FetchByName(list_p, play_name);
	play_node_t* p;
	
	Schedule_Srv_FetchByPlay(list_s,id);
	

	int len = 0, i;
	schedule_list_t s;
	List_ForEach(list_s, s)
	{
		len++;
	}
	//打印演出计划
	schedule_node_t* pos;
	Pagination_t paging;


	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;

	paging.totalRecords = len;
	Paging_Locate_FirstPage(list_s, paging);
	system("cls");
	printf(
		"\n==================================================================\n");
	printf(
		"********************** 演出计划信息 **********************\n");
	printf("%8s %8s  %8s  %8s  %8s %8s\n", "演出计划ID", "上映剧目ID", "演出厅ID",
		"放映日期", "放映时间", "座位数");
	printf(
		"------------------------------------------------------------------\n");

	Paging_ViewPage_ForEach(list_s, paging, schedule_node_t, pos, i) {
		printf("%8d %8d  %8d  %d/%d/%d  %d/%d/%d %8d \n", pos->data.id,
			pos->data.play_id, pos->data.studio_id, pos->data.date.year,
			pos->data.date.month, pos->data.date.day, pos->data.time.hour,
			pos->data.time.minute, pos->data.time.second, pos->data.seat_count);
	}

	printf(
		"------- 共:%2d页 ----------------------- 页数 :%2d/%2d ----\n",
		paging.totalRecords, Pageing_CurPage(paging),
		Pageing_TotalPages(paging));
	printf(
		"******************************************************************\n");
	printf("输入R返回上一层，输入T进行票务查询");

	setbuf(stdin, NULL);
	scanf("%c", &choice);
	getchar();
	printf("输入要查询的计划id");
	scanf("%d", &schid);
	switch (choice)
	{
	case't':
	case'T':
		Ticket_UI_Query(schid);
		break;
	}
	
	return 1;

}
void QTicket_UI_Query(int schedule_id) {
	int id, i;
	ticket_list_t ticket_list;
	Pagination_t paging;
	char choice;
	List_Init(ticket_list, ticket_node_t);//初始化演出链表
	ticket_list_t pos = (ticket_list_t)malloc(sizeof(ticket_node_t));
	paging.totalRecords =Ticket_Srv_FetchBySchID_ticket(ticket_list, schedule_id);
	ticket_list_t head;
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;
	Paging_Locate_FirstPage(ticket_list , paging);


	do
	{
		printf("\n=======================================================\n");
		printf(
			"*********************** 演出票的信息 **********************\n");
		printf("%5s  %5s  %5s %5s %5s", "票id", "演出计划id", "座位id", "价格", "状态");
		Paging_ViewPage_ForEach(ticket_list, paging, ticket_node_t, pos, i) {

			printf("%5d  %5d  %5d  %5d  %5d\n", pos->data.id, pos->data.schedule_id, pos->data.seat_id, pos->data.price, pos->data.status);
		}
		printf(
			"\n-------------------------------------------------------\n");
		printf("\n\n\n\n提示：输入'R'返回。");
		setbuf(stdin, NULL);
		scanf("%c", &choice);
		getchar();
		break;

	} while (choice!='r'&&choice!='R');

	List_Destroy(ticket_list, ticket_node_t);
	return 0;
}