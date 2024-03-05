#ifndef ACCOUNT_UI_H_
#define ACCOUNT_UI_H_

#include "../Common/List.h"
#include "../Service/Account.h"


//系统登录（TTMS_SCU_Login）1
int SysLogin();


//系统用户管理界面（TTMS_SCU_Account_UI_Mgt）2
void Account_UI_MgtEntry();

 
//添加新系统用户界面（TTMS_SCU_Account_UI_Add）3
int Account_UI_Add(account_list_t list);


//修改系统用户界面（TTMS_SCU_Account_UI_Mod） 4
int Account_UI_Modify(account_list_t list,char usrName[]);


//删除系统用户界面（TTMS_SCU_Account_UI_Del）5
int Account_UI_Delete(account_list_t list,char usrName[]);

 
//查询系统用户界面（TTMS_SCU_Account_UI_Que） 6
int Account_UI_Query(account_list_t list,char usrName[]);


#endif
