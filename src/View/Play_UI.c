#include"Play_UI.h"
#include"Schedule_UI.h"
#include"../Service/Play.h" 
#include"../Service/Schedule.h"
#include"../Service/Account.h"
#include"Account_UI.h"
#include"../Common/List.h"
#include"../View/Schedule_UI.h"
#include"../Persistence/Play_Persist.h"
//#include"Account_UI.h"
#include"../Common/listTestDrv.h"
static const int PLAY_PAGE_SIZE=5;
void Play_UI_MgtEntry(void){//界面层管理剧目的入口函数
	if (gl_CurUser.type == USR_ANOMY || gl_CurUser.type == USR_ADMIN) {
		printf("you can't join in there!please input the [Enter]");
		getchar();
		return 0;
	}
	int i, id;
	char choice;

	play_list_t head;
	play_node_t *pos;
	Pagination_t paging;
	play_t* tmp;
	List_Init(head, play_node_t);
	paging.offset = 0;
	paging.pageSize = PLAY_PAGE_SIZE;
	//载入数据
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	
	do {
		system("cls");
		printf(
				"\n==================================================================\n");
		printf(
				"---------------------- Projection Play List ----------------------\n\n");
		printf("%4s  %5s  %5s  %5s  %6s  %4s  %7s  %7s %7s\n", "ID", "Name", "Type",
				"Area", "Rating","long","stdate","eddate","price");
		printf(
				"------------------------------------------------------------------\n");
		//显示数据
		Paging_ViewPage_ForEach(head, paging, play_node_t, pos, i){
			printf("%3d  %5s  %4d  %7s  %3d  %6d  %5d%2d%2d  %3d%2d%2d %3d\n", pos->data.id,
					pos->data.name, pos->data.type, pos->data.area,
					pos->data.rating,pos->data.duration,pos->data.start_date.year
					,pos->data.start_date.month,pos->data.start_date.day
					,pos->data.end_date.year
					,pos->data.end_date.month, pos->data.end_date.day,pos->data.price);
		}

		printf(
				"------- Total Records:%2d ----------------------- Page %2d/%2d ----\n\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("------------------------------------------------------------------\n");
		printf("[P]revPage | [N]extPage | [A]dd | [D]elete | [U]pdate | [S]chedule | [R]eturn\n");
		printf(
			"==================================================================\n");

		getchar();
		fflush(stdin);
		
		scanf("%c", &choice);
		
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Play_UI_Add()) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Play_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, play_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Play_UI_Delete(id)) {	//从新载入数据
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Play_UI_Modify(id)) {	//从新载入数据
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Play_Srv_FetchByID(id,&(pos->data))) {	//从新载入数据
				Schedule_UI_MgtEntry(id);
			}
			else {
				printf("cannot find this play");
			}
			break;
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, play_node_t);
}


int Play_UI_Add(void) {//添加一个演出 
	play_t rec;
	int newRecCount = 0;
	char choice;

	do {
		printf("\n=======================================================\n");
		printf("****************  Add New Projection Play  ****************\n");
		printf("-------------------------------------------------------\n");
		printf("Play Name:");
		fflush(stdin);
		getchar();
		gets(rec.name);
		do{
			printf("Type 1.film 2.opera 3.concert:");
			fflush(stdin);
			scanf("%d", &(rec.type));
		}while(rec.type>3||rec.type<1);
		
		printf("Area:");
		fflush(stdin);
		scanf("%s", rec.area);
		printf("rating 1.child 2.teenage 3.adult");
		fflush(stdin);
		scanf("%d",&(rec.rating));
		printf("Duration(in minute):");
		fflush(stdin);
		scanf("%d",&(rec.duration));
		printf("start date:");
		fflush(stdin);
		printf("year:");
		scanf("%d",&(rec.start_date.year));
		printf("month:");
		fflush(stdin);
		scanf("%d",&(rec.start_date.month));
		printf("day:");
		fflush(stdin);
		scanf("%d",&(rec.start_date.day));
		printf("end date:");
		fflush(stdin);
		printf("year:");
		scanf("%d",&(rec.end_date.year));
		printf("month:");
		fflush(stdin);
		scanf("%d",&(rec.end_date.month));
		printf("day:");
		fflush(stdin);
		scanf("%d",&(rec.end_date.day));
		printf("price:");
		fflush(stdin);
		scanf("%d", &rec.price);
		printf("=======================================================\n");
		if (Play_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("The new play added successfully!\n");
		} else
			printf("The new play added failed!\n");
		printf("-------------------------------------------------------\n");
		printf("[A]dd more, [R]eturn:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}
int Play_UI_Modify(int id) {
	int rtn = 0;
	play_t rec;
	int newRecCount = 0;
	char choice;
	play_list_t list;

	/*Load record*/
	if (!Play_Srv_FetchByID(id, &rec)) {
		printf("The Play does not exist!\nPress [Enter] key to return!\n");
		getchar();
		return 0;
	}

	printf("\n=======================================================\n");
	printf("****************  Update Projection Play  ****************\n");
	printf("-------------------------------------------------------\n");
	printf("Play ID:%d\n", rec.id);
	printf("Play Name[%s]:", rec.name);
	fflush(stdin);
	getchar();
	gets(rec.name);

	List_Init(list, play_node_t);
	
		do{
			printf("Type 1.film 2.opera 3.concert:");
			fflush(stdin);
			scanf("%d", &(rec.type));
		}while(rec.type>3||rec.type<1);
		
		printf("Area:");
		fflush(stdin);
		scanf("%s", rec.area);
		printf("rating 1.child 2.teenage 3.adult");
		fflush(stdin);
		scanf("%d",&(rec.rating));
		printf("Duration(in minute):");
		fflush(stdin);
		scanf("%d",&(rec.duration));
		printf("start date:");
		fflush(stdin);
		printf("year:");
		scanf("%d",&(rec.start_date.year));
		printf("month:");
		fflush(stdin);
		scanf("%d",&(rec.start_date.month));
		printf("day:");
		fflush(stdin);
		scanf("%d",&(rec.start_date.day));
		printf("end date:");
		fflush(stdin);
		printf("year:");
		scanf("%d",&(rec.end_date.year));
		printf("month:");
		fflush(stdin);
		scanf("%d",&(rec.end_date.month));
		printf("day:");
		fflush(stdin);
		scanf("%d",&(rec.end_date.day));
		printf("price:");
		fflush(stdin);
		scanf("%d", &rec.price);
	printf("-------------------------------------------------------\n");

	if (Play_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"The room data updated successfully!\nPress [Enter] key to return!\n");
	} else
		printf("The room data updated failed!\nPress [Enter] key to return!\n");

	getchar();
	return rtn;
}
int Play_UI_Delete(int id) {

	int rtn = 0;

	if (Play_Srv_DeleteByID(id)) {
		//在删除剧目时，同时根据表演id删除计划文件中的计划 
		if (Schedule_Srv_DeleteAllByPlayID(id))
			printf("The schedule of the Play deleted successfully!\n");
		printf(
				"The play deleted successfully!\nPress [Enter] key to return!\n");
		rtn = 1;
	} else {
		printf("The play does not exist!\nPress [Enter] key to return!\n");
	}

	getchar();
	return rtn;
}
