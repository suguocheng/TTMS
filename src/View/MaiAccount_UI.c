#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Common/List.h"

#include "MaiAccount_UI.h"
#include "Account_UI.h"

#include "../Service/Account.h"
#include "../Service/Seat.h"
#include "../Service/Studio.h"
#include "../Persistence/Account_Persist.h"


//维护个人资料界面（TTMS_SCU_MaiAccount_UI_MgtEnt) 
void MaiAccount_UI_MgtEntry()
{
	account_t cdata = gl_CurUser;//存放当前用户信息 
//	cdata=gl_CurUser;
	
	// 定义一个字符串变量cpassword作为密码 
	char cpassword[30];
	
	printf("请输入新密码:");
	
    // 使用fgets从标准输入获取一整行文本作为密码 
	scanf("%s", cpassword);
	getchar();
	//=;
	strcpy(cdata.password, cpassword);
	Account_Srv_Modify(&cdata); 
	return; 
}
