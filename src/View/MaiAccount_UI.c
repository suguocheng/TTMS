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


//ά���������Ͻ��棨TTMS_SCU_MaiAccount_UI_MgtEnt) 
void MaiAccount_UI_MgtEntry()
{
	account_t cdata = gl_CurUser;//��ŵ�ǰ�û���Ϣ 
//	cdata=gl_CurUser;
	
	// ����һ���ַ�������cpassword��Ϊ���� 
	char cpassword[30];
	
	printf("������������:");
	
    // ʹ��fgets�ӱ�׼�����ȡһ�����ı���Ϊ���� 
	scanf("%s", cpassword);
	getchar();
	//=;
	strcpy(cdata.password, cpassword);
	Account_Srv_Modify(&cdata); 
	return; 
}
