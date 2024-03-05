#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Common/List.h"

#include "Account_UI.h"

#include "../Service/Account.h"
#include "../Service/Seat.h"
#include "../Service/Studio.h"

//定义全局变量gl_CurUser存储系统用户信息 
static const int ACCOUNT_PAGE_SIZE = 6;

//系统登录（TTMS_SCU_Login）1
int SysLogin()//系统登录，创建账号，接收输入的用户名和密码，验证用户名密码正确性 
{
	char UNAME[30],PWD[30];
	Account_Srv_InitSys;//
	int i,flag;
	for( i = 0 ;i < 3; i++ )
	{
		system("cls");
		printf("\033[1;45;33m  ------------TTMS_system------------  \033[0m\n" "\033[1;45;33m   -------------Welcome!------------   \033[0m \n");
		printf("请输入用户名： ");
        //scanf("%s", data_admin->username);
		scanf("%s", UNAME);
        /*fgets(UNAME, sizeof(UNAME), stdin);*/
        fflush;
		getchar();
        printf("请输入密码： ");
        /*fgets(PWD, sizeof(PWD), stdin);*/
		scanf("%s", PWD);
        //scanf("%s", data_admin->password);
		fflush;
		
		flag = Account_Srv_Verify(UNAME,PWD);//char usrName[],char pwd[]//验证用户名和密码 成功返回1 
		if( flag== 1) return 1;	
	}
	exit(0);
	return 0;
}



////////////////////////////////////////////////////


int Account_Srv_Verify(char usrName[],char pwd[])
{
	account_t usr;
	if(Account_Perst_SelByName(usrName,&usr))/////////////////////////检查返回值 ///////////////////////////////
	{
		if(!strcmp(pwd,usr.password)) {
		gl_CurUser=usr;
		return 1;
	    }
		else return 0; 
	}
	else return 0;	
}//MTG


////////////////////////////////////////////////////[strlen(pwd)]







//系统用户管理界面（TTMS_SCU_Account_UI_Mgt）2
void Account_UI_MgtEntry()
{
	int i;
	char name[30];
	char choice;
	account_list_t head;
	account_node_t *post;
	Pagination_t paging;
	List_Init(head, account_node_t);//??
	
	paging.offset = 0;
	paging.pageSize = ACCOUNT_PAGE_SIZE;
	
	//载入数据Account_Srv_FetchAll
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	
	do {
		system("cls");
		printf(
				"\n==================================================================\n");
		printf(
				"------------------------- User Account List ----------------------\n");
		//printf("%5s  %3s  %4s  %4s\n", "ID", "Type", "Username","Password");
		printf("%5s  %3s  %4s \n", "ID", "Type", "Username");
		printf(
				"\n------------------------------------------------------------------\n");
		//显示数据
		Paging_ViewPage_ForEach(head, paging, account_node_t, post, i){
		//	printf("%5d  %3d  %6s  %5s\n", post->data.id,	post->data.type, post->data.username, post->data.password);
			printf("%5d  %3d  %6s\n", post->data.id, post->data.type, post->data.username);
		}

		printf(
				"------- Total Records:%2d ----------------------- Page %2d/%2d ----\n\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"------------------------------------------------------------------\n");
		printf(
				" [A]dd | [D]elete | [M]odify | [Q]uery | [R]eturn | [P]revPage | [N]extPage ");
		printf(
				"\n==================================================================\n");
		printf("Your Choice:");
		fflush(stdin);
		
		scanf("%c", &choice);
		
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Account_UI_Add(head)) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, account_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("Input the Username:");
			fflush;
			scanf("%s", &name);
			getchar();
			if (Account_UI_Delete(head,name)) {	//从新载入数据
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 'm':
		case 'M':
			printf("Input the Username:");
			fflush;
			scanf("%s", &name);
			getchar();
			if (Account_UI_Modify(head,name)) {	//从新载入数据
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 'q':
		case 'Q':
			printf("Input the Username:");
			fflush;
			scanf("%s", &name);
			getchar();
//			//Seat_UI_MgtEntry(id);
//			paging.totalRecords = Studio_Srv_FetchAll(head);
//			List_Paging(head, paging, studio_node_t);
//			
			if (Account_UI_Query(head,name)) {	//从新载入数据
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, account_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, account_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, account_node_t);
}//MTG
 //待修改*************************************************************** 
 
 /*
标识符：TTMS_SCU_Studio_UI_Add 
函数功能：用于向系统中添加一个新演出厅数据。
参数说明：无。
返 回 值：整型，成功添加新演出厅的个数。
*/
//int Studio_UI_Add(void) {
//	studio_t rec;
//	int newRecCount = 0;
//	char choice;
//
//	do {
//		printf("\n=======================================================\n");
//		printf("****************  Add New Projection Room  ****************\n");
//		printf("-------------------------------------------------------\n");
//		printf("Room Name:");
//		fflush(stdin);
//		gets(rec.name);
//		printf("Row Count of Seats:");
//		scanf("%d", &(rec.rowsCount));
//		printf("Column Count of Seats:");
//		scanf("%d", &(rec.colsCount));
//		rec.seatsCount = 0;
//		printf("=======================================================\n");
//
//		if (Studio_Srv_Add(&rec)) {
//			newRecCount += 1;
//			printf("The new room added successfully!\n");
//		} else
//			printf("The new room added failed!\n");
//		printf("-------------------------------------------------------\n");
//		printf("[A]dd more, [R]eturn:");
//		fflush(stdin);
//		scanf("%c", &choice);
//	} while ('a' == choice || 'A' == choice);
//	return newRecCount;
//}
//添加新系统用户界面（TTMS_SCU_Account_UI_Add）3
int Account_UI_Add(account_list_t list)
{
	account_node_t* rec;
	int rtn;
    char choice;

	int ID=0;
	int enumType=0;
    char name[30];
	char pword[30];
	
	account_t newuser;

	do {
		printf("\n=======================================================\n");
		printf("****************  Add New System User  ****************\n");
		printf("-------------------------------------------------------\n");
		printf("User Name:");
		fflush(stdin);
		
		scanf("%s", &name);
		getchar();
		rec = Account_Srv_FindByUsrName(list, name);

		
		if(NULL==rec)//若没找到，则执行添加 
		{
			//fflush(stdin);
			//scanf("%d", &ID);
			//newuser.id = ID;

			printf("未找到该用户。请按下Enter添加\n\n\n");

			getchar();

			printf("用户类型：\n（1--售票员 || 2--剧院经理 || 3--系统管理员 || 0--匿名类型（初始化使用））\n\n");
			fflush(stdin);
			scanf("%d", &enumType);
			newuser.type = enumType;

			getchar();
			printf("用户名：（请确保与原查询用户名相同）\n");
			fflush(stdin);
			scanf("%s",name);
            strcpy(newuser.username, name);

			printf("密码：\n");
			fflush(stdin);
			scanf("%s",pword);
			strcpy(newuser.password,pword);


		    if (Account_Srv_Add(&newuser)) {//若添加成功 
			printf("\n\nThe new user added successfully!\n");
			printf("\n=======================================================\n");
			rtn = 1;
		    } else{//否则 
		        	printf("The new user added failed!\n");
		        	printf("\n=======================================================\n");
					rtn = 0;
				}
		}
		else{
		  	printf("\nThe user has been added!\n");
		  	printf("\n=======================================================\n");
		  	rtn = 0;
		}
		  
//		printf("Row Count of Seats:");
//		scanf("%d", &(rec.rowsCount));
//		printf("Column Count of Seats:");
//		scanf("%d", &(rec.colsCount));
//		rec.seatsCount = 0;
	


//		printf("-------------------------------------------------------\n");
		printf("[A]dd more, [R]eturn:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return rtn;//?
}
//eg.
//int Studio_UI_Modify(int id) {
//	studio_t rec;
//	int rtn = 0;
//	int newrow, newcolumn;
//	seat_list_t list;
//	int seatcount;
//
//	/*Load record*/
//	if (!Studio_Srv_FetchByID(id, &rec)) {
//		printf("The room does not exist!\nPress [Enter] key to return!\n");
//		getchar();
//		return 0;
//	}
//
//	printf("\n=======================================================\n");
//	printf("****************  Update Projection Room  ****************\n");
//	printf("-------------------------------------------------------\n");
//	printf("Room ID:%d\n", rec.id);
//	printf("Room Name[%s]:", rec.name);
//	fflush(stdin);
//	gets(rec.name);
//
//	List_Init(list, seat_node_t);
//	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
//	if (seatcount) {
//		do {			//如果座位文件中已有座位信息，则更新的行列必须比以前大，否则不允许更改
//			printf("Row Count of Seats should >= [%d]:", rec.rowsCount);
//			scanf("%d", &(newrow));
//			printf("Column Count of Seats should >= [%d]:", rec.colsCount);
//			scanf("%d", &(newcolumn));
//		} while (newrow < rec.rowsCount || newcolumn < rec.colsCount);
//		rec.rowsCount = newrow;
//		rec.colsCount = newcolumn;
//		rec.seatsCount = seatcount;
//	} else {
//		printf("Row Count of Seats:");
//		scanf("%d", &rec.rowsCount);
//		printf("Column Count of Seats:");
//		scanf("%d", &rec.colsCount);
//		rec.seatsCount = 0;
//	}
//
//	printf("-------------------------------------------------------\n");
//
//	if (Studio_Srv_Modify(&rec)) {
//		rtn = 1;
//		printf(
//				"The room data updated successfully!\nPress [Enter] key to return!\n");
//	} else
//		printf("The room data updated failed!\nPress [Enter] key to return!\n");
//
//	getchar();
//	return rtn;
//}/////////////////////////eg/////

//修改系统用户界面（TTMS_SCU_Account_UI_Mod） 4         
//即查询用户并修改密码  
int Account_UI_Modify(account_list_t list,char usrName[])       
{
	
	int rtn = 0;
	char oldPassword[30];
    char newPassword[30];
//	int seatcount,uName;/////

	account_t changeuser;
    account_node_t* rec = Account_Srv_FindByUsrName(list, usrName);
	
	
	/*Load record*/
    if (rec==NULL) {
		printf("The user does not exist!\nPress [Enter] key to return!\n");
		getchar();
		return 0;
	}//a
	
	
	printf("User ID:%d\n", rec->data.id);////
	printf("User Type:%d\n", rec->data.type);////
	printf("User Name:%s\n", rec->data.username);////查到了基础信息，但要修改密码还要输入原密码 
    printf("\n=======================================================\n");
	printf("****************  Update Account Password ****************\n");
	printf("\n=======================================================\n");	
	
	
	
	printf("请输入原密码: ");
	scanf("%s", oldPassword);
	getchar();
    if(strcmp(rec->data.password, oldPassword)==0)
    {
		printf("原密码正确！\n请输入您想修改的新密码: ");
		scanf("%s", newPassword);
		getchar();
        strcpy(rec->data.password,newPassword);
		Account_Srv_Modify(rec);
		printf("\nThe User Account updated successfully!\nPress [Enter] key to return!\n");
		printf("-------------------------------------------------------\n");
        rtn = 1;
        
		

	}
    
 //    List_Init(list, account_node_t);
//	newpass = Account_Srv_FindByUsrName(list, rec.password);////


//	if (uName) {
//		do {			//如果用户文件中已有用户信息，则更新的行列？必须比以前大，否则不允许更改
//			printf("Row Count of Seats should >= [%d]:", rec.rowsCount);
//			scanf("%d", &(newrow));
//			printf("Column Count of Seats should >= [%d]:", rec.colsCount);
//			scanf("%d", &(newcolumn));
//		} while (newrow < rec.rowsCount || newcolumn < rec.colsCount);
//		rec.rowsCount = newrow;
//		rec.colsCount = newcolumn;
//		rec.seatsCount = seatcount;
//	} else {
//		printf("Row Count of Seats:");
//		scanf("%d", &rec.rowsCount);
//		printf("Column Count of Seats:");
//		scanf("%d", &rec.colsCount);
//		rec.seatsCount = 0;
//	}

//注释为不明意义代码 

	else printf("The User Account updated failed!\nPress [Enter] key to return!\n");

	getchar();
	return rtn;
	
}//MTG


//删除系统用户界面（TTMS_SCU_Account_UI_Del）5
int Account_UI_Delete(account_list_t list,char usrName[])
{
    int rtn = 0;

	if (Account_Srv_FindByUsrName(list,usrName)!= NULL) {//此处就用到指针作为返回值 
		//在删除用户结点？时，同时根据用户id删除用户文件中的信息
		//可以通过写个小函数删掉内存里的，或者直接断联 


		/*List_FreeNode(Account_Srv_FindByUsrName(list,usrName));*/


		Account_Perst_RemByUsrName(usrName);

		
//		if (Account_Srv_DeleteByID()){	
//		printf("The information of the user deleted successfully!\n");
//		printf("The user deleted successfully!\nPress [Enter] key to return!\n");
//			List_FreeNode(Account_Srv_FindByUsrName(list,usrName));//?
//		}
		printf("The User deleted successfully!\nPress [Enter] key to return!\n\n");
////////////////////对account_list_t链表进行同步删除account_node_t类型结点??????????????????????????????????
		rtn = 1;
	} else {
		printf("The User does not exist!\n\nPress [Enter] key to return!\n");
	}

	getchar();
	return rtn;	
}//MTG
 
//查询系统用户界面（TTMS_SCU_Account_UI_Que） 6
int Account_UI_Query(account_list_t list,char usrName[])
{
	account_node_t* rec = Account_Srv_FindByUsrName(list, usrName);
	/*Load record*/
	if (rec==NULL) {
		printf("The user does not exist!\nPress [Enter] key to return!\n");
		getchar();
		return 0;
	}
	else {

		printf("\nThe User is found successfully!\n\nPress [Enter] key to return!\n");
		printf("User ID: %d\n", rec->data.id);
		printf("User Type: %d\n", rec->data.type);
		printf("User Name: %s\n", rec->data.username);

		getchar();
		return 1;
	}
}//MTG 

