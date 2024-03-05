#include "../Common/List.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include <stdio.h>
#include <stdlib.h>
#include"../Service/Ticket.h"
const int SCHEDULE_PAGE_SIZE = 5;
/*
��ʶ����TTMS_SCU_Schdule_Perst_SelAll
�������ܣ���ѯ�ݳ�����
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
		//��ʾ����
		
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
			"[P/p]��һҳ    |   [N/n]��һҳ  |  [q/Q] ��������ɸѡ�ݳ��ƻ�\n[r/R]�����ϲ�  |");
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
			printf("������Ҫ��ѯ�ľ�Ŀ����:");
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
��ʶ���� TTMS_SCU_Schedule_UI_Qry
���ܣ����ݾ�Ŀ���ƻ�ȡ�ݳ��ƻ�
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
	//��ӡ�ݳ��ƻ�
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
		"********************** �ݳ��ƻ���Ϣ **********************\n");
	printf("%8s %8s  %8s  %8s  %8s %8s\n", "�ݳ��ƻ�ID", "��ӳ��ĿID", "�ݳ���ID",
		"��ӳ����", "��ӳʱ��", "��λ��");
	printf(
		"------------------------------------------------------------------\n");

	Paging_ViewPage_ForEach(list_s, paging, schedule_node_t, pos, i) {
		printf("%8d %8d  %8d  %d/%d/%d  %d/%d/%d %8d \n", pos->data.id,
			pos->data.play_id, pos->data.studio_id, pos->data.date.year,
			pos->data.date.month, pos->data.date.day, pos->data.time.hour,
			pos->data.time.minute, pos->data.time.second, pos->data.seat_count);
	}

	printf(
		"------- ��:%2dҳ ----------------------- ҳ�� :%2d/%2d ----\n",
		paging.totalRecords, Pageing_CurPage(paging),
		Pageing_TotalPages(paging));
	printf(
		"******************************************************************\n");
	printf("����R������һ�㣬����T����Ʊ���ѯ");

	setbuf(stdin, NULL);
	scanf("%c", &choice);
	getchar();
	printf("����Ҫ��ѯ�ļƻ�id");
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
	List_Init(ticket_list, ticket_node_t);//��ʼ���ݳ�����
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
			"*********************** �ݳ�Ʊ����Ϣ **********************\n");
		printf("%5s  %5s  %5s %5s %5s", "Ʊid", "�ݳ��ƻ�id", "��λid", "�۸�", "״̬");
		Paging_ViewPage_ForEach(ticket_list, paging, ticket_node_t, pos, i) {

			printf("%5d  %5d  %5d  %5d  %5d\n", pos->data.id, pos->data.schedule_id, pos->data.seat_id, pos->data.price, pos->data.status);
		}
		printf(
			"\n-------------------------------------------------------\n");
		printf("\n\n\n\n��ʾ������'R'���ء�");
		setbuf(stdin, NULL);
		scanf("%c", &choice);
		getchar();
		break;

	} while (choice!='r'&&choice!='R');

	List_Destroy(ticket_list, ticket_node_t);
	return 0;
}