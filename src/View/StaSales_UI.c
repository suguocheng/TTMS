//ͳ�����۶����� 

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>

#include "../View/StaSales_UI.h"

#include "../Common/List.h"
#include "../Service/StaSales.h"
#include "../Service/Studio.h"
#include "../Service/Seat.h"
#include "../Service/Account.h"
#include "../View/StaSales_UI.h"
#include "../Service/SalesAnalysis.h"

void StaSales_UI_MgtEntry(){
	
   if(gl_CurUser.type==1){//1 
   	StaSales_UI_Self();
   }/*�ж��û�����--��Աͳ�Ƹ������۽���*/
   else if(gl_CurUser.type==2){//2 
   	StaSales_UI_Clerk();
   }/*�ж��û�����--����ͳ����ƱԱ���۽���*/
   else{
   	printf("��Ȩ�ޣ����س��˳�");
    getchar();
   }
}

void StaSales_UI_Self(){/*�ж��û�����--ͳ�Ƹ������۽���*/
    int id = gl_CurUser.id;
    char choice;

    ttms_date_t curdate, startdate, enddate;
    struct tm* p;
    time_t timep;
    time(&timep);
    p = localtime(&timep);
    printf("[D]ay is One-day sales            |            [M]onth Monthly sales");
    printf("\nyou choice:");
    scanf("%c", &choice);
    getchar();
    setbuf(stdin, NULL);
    curdate.year = p->tm_year + 1900;
    curdate.month = p->tm_mon + 1;
    curdate.day = p->tm_mday;
    startdate.year = p->tm_year + 1900;
    startdate.month = p->tm_mon + 1;
    startdate.day = 1;
    enddate.year = p->tm_year + 1900;
    enddate.month = p->tm_mon + 1;
    enddate.day = 31;
    printf("�鿴����Ʊ������'d'|'D'\n");
    printf("����Ʊ������'m'|'M'\n");
    int a;
    int b;
    scanf("%c",&choice);
    switch (choice) {
    case 'd':
    case 'D'://����
    	
    	a= SalesAnalysis_Srv_CompSaleVal(id,curdate,curdate);/*������Ʊ��*/
    	printf("������Ʊ�%d\n",a);
    	break;
	case 'm':
    case 'M'://����
		
		b= SalesAnalysis_Srv_CompSaleVal(id,startdate,enddate);/*������Ʊ��*/
		printf("������Ʊ��;%d\n",b);
		break;
	} 
}


void StaSales_UI_Clerk(){//ͳ����ƱԱ���۶����
    account_t tem;
    int id;

    ttms_date_t startdate, enddate;

    char Usrname[20];

    
     printf("�������û���: ");
    scanf("%s",&Usrname);;
    if(Account_Srv_FetchByName(Usrname,&tem)){//��ȡϵͳ�û� ���º��� 
        id = tem.id;
    	printf("��������ʼ���ڣ��� �� �գ���");
    	scanf("%d %d %d",&startdate.year,&startdate.month,&startdate.day);
    	printf("������������ڣ��� �� �գ���");
    	scanf("%d %d %d",&enddate.year,&enddate.month,&enddate.day);
    	getchar();
    	int a;
    	a= SalesAnalysis_Srv_CompSaleVal(id,startdate,enddate);
    	printf("���۶%d\n",a);
	}else{
		printf("�û�������\n");
		getchar();
		return;
	}
}
