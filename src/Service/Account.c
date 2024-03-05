#include <stdlib.h>
#include <stdio.h>

#include "Account.h"
#include"../View/Account_UI.h"
#include"../View/MaiAccount_UI.h"
#include "../Persistence/Account_Persist.h"
#include <assert.h>


account_t gl_CurUser = { (int)0,(account_type_t)0,"admin","1" };//匿名类型，初始化账号时使用

////创建管理员Admin匿名系统用户（TTMS_SCU_Account_ Srv_InitSys) 1
//void Account_Srv_InitSys()
//{
//	
//	if(Account_Perst_CheckAccFile()) return;
//	else{
//		account_t data_admin;
//		Account_Srv_Add(&data_admin);
//		return;
//	}
//}//MTG
// 
// 
////验证系统用户的用户名和密码（TTMS_SCU_Account_Srv_Verify)2 
//int Account_Srv_Verify(char usrName[], unsigned char pwd[]) {
//	account_t usr;
//
//	if (Account_Perst_SelByName(usrName, &usr)) {
//		int x = 1;
//		for (int i = 0; i < 16; i++) {
//
//			if (pwd[i] != usr.password[i])
//				x = 0;
//		}
//		if (x) {
//			gl_CurUser = usr;
//			return 1;
//		}
//		else {
//			return 0;
//		}
//	}
//	return 0;
//}
//


//添加新系统用户（TTMS_SCU_Account_Srv_Add）3
int Account_Srv_Add(const account_t *data)
{
	return Account_Perst_Insert(data);
}//MTG


//修改系统用户（TTMS_SCU_Account_Srv_Mod） 4
int Account_Srv_Modify(const account_t *data)
{
	return Account_Perst_Update(data);
}//MTG


//删除系统用户（TTMS_SCU_Account_Srv_DelByID） 5
int Account_Srv_DeleteByID(int usrID)
{
	return Account_Perst_DeleteByID(usrID);
}//MTG


//获取所有系统用户（TTMS_SCU_Account_Srv_FetchAll）6
int Account_Srv_FetchAll(account_list_t list)
{
	return Account_Perst_SelectAll(list);
}//MTG

//根据用户名获取系统用户指针（TTMS_SCU_Account_Srv_FindByUsrName）7 
account_node_t * Account_Srv_FindByUsrName(account_list_t list,char usrName[])
{
	assert(NULL!=list);
	account_node_t *pos=list->next;
	while(pos!=list){
		if (!strcmp(pos->data.username, usrName))//!strcmp(pwd[strlen(pwd)],usr.password
			return pos;
		else
			pos=pos->next;
	}
	return NULL;
}//MTG





account_node_t * Account_Srv_FetchByName(char usrName[],account_list_t list)
{
	assert(NULL!=list);
	account_node_t *pos=list->next;
	while(pos!=list){
		if (!strcmp(pos->data.username, usrName))//[strlen(usrName)]
			return pos;
		else
			pos=pos->next;
	}
	return NULL;
}//MTG

char Account_Srv_FetchNameByName(char usrName[], account_list_t list)
{
	assert(NULL != list);
	account_node_t* pos = list->next;
	while (pos != list) {
		if (!strcmp(pos->data.username, usrName))//[strlen(usrName)]
			return pos->data.username;
		else
			pos = pos->next;
	}
	return NULL;
}//MTG

int Account_Srv_FetchIDByName(char usrName[], account_list_t list)
{
	assert(NULL != list);
	account_node_t* pos = list->next;
	while (pos != list) {
		if (!strcmp(pos->data.username, usrName))//[strlen(usrName)]
			return pos->data.id;
		else
			pos = pos->next;
	}
	return NULL;
}//MTG
 int Account_Srv_FetchByID(int usrID, account_t* buf) {
	return Account_Perst_SelectByID(usrID, buf);

}
