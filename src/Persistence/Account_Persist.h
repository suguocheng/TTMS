#ifndef ACCOUNT_PERSIST_H_
#define ACCOUNT_PERSIST_H_

#include "../Service/Account.h"

//判断系统用户文件是否存在（TTMS_SCU_Account_Perst_CheckAccFile）1
int Account_Perst_CheckAccFile();

//存储新系统用户（TTMS_SCU_Account_Perst_Insert） 2
int Account_Perst_Insert(account_t *data);

//更新系统用户（TTMS_SCU_Account_Perst_Update）3
int Account_Perst_Update(account_t *data);

//根据ID去除系统用户（TTMS_SCU_Account_Perst_RemByID）4
int Account_Perst_RemByID(int id);

//载入全部系统用户 （TTMS_SCU_Account_Perst_SelectAll）5
int Account_Perst_SelectAll(account_list_t list);
int Account_Perst_SelectByID(int id, account_t* buf);




int Account_Perst_RemByUsrName(char uNAME[]);

#endif /* USERACCOUNTPERSIST_H_ */
