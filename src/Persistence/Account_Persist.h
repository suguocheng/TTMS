#ifndef ACCOUNT_PERSIST_H_
#define ACCOUNT_PERSIST_H_

#include "../Service/Account.h"

//�ж�ϵͳ�û��ļ��Ƿ���ڣ�TTMS_SCU_Account_Perst_CheckAccFile��1
int Account_Perst_CheckAccFile();

//�洢��ϵͳ�û���TTMS_SCU_Account_Perst_Insert�� 2
int Account_Perst_Insert(account_t *data);

//����ϵͳ�û���TTMS_SCU_Account_Perst_Update��3
int Account_Perst_Update(account_t *data);

//����IDȥ��ϵͳ�û���TTMS_SCU_Account_Perst_RemByID��4
int Account_Perst_RemByID(int id);

//����ȫ��ϵͳ�û� ��TTMS_SCU_Account_Perst_SelectAll��5
int Account_Perst_SelectAll(account_list_t list);
int Account_Perst_SelectByID(int id, account_t* buf);




int Account_Perst_RemByUsrName(char uNAME[]);

#endif /* USERACCOUNTPERSIST_H_ */
