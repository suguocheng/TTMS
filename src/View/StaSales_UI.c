//统计销售额界面层 

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
   }/*判断用户类型--店员统计个人销售界面*/
   else if(gl_CurUser.type==2){//2 
   	StaSales_UI_Clerk();
   }/*判断用户类型--经理统计售票员销售界面*/
   else{
   	printf("无权限，按回车退出");
    getchar();
   }
}

void StaSales_UI_Self(){/*判断用户类型--统计个人销售界面*/
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
    printf("查看当日票额输入'd'|'D'\n");
    printf("当月票额输入'm'|'M'\n");
    int a;
    int b;
    scanf("%c",&choice);
    switch (choice) {
    case 'd':
    case 'D'://当日
    	
    	a= SalesAnalysis_Srv_CompSaleVal(id,curdate,curdate);/*当日售票额*/
    	printf("当日售票额；%d\n",a);
    	break;
	case 'm':
    case 'M'://当月
		
		b= SalesAnalysis_Srv_CompSaleVal(id,startdate,enddate);/*当月售票额*/
		printf("当月售票额;%d\n",b);
		break;
	} 
}


void StaSales_UI_Clerk(){//统计售票员销售额界面
    account_t tem;
    int id;

    ttms_date_t startdate, enddate;

    char Usrname[20];

    
     printf("请输入用户名: ");
    scanf("%s",&Usrname);;
    if(Account_Srv_FetchByName(Usrname,&tem)){//获取系统用户 ，新函数 
        id = tem.id;
    	printf("请输入起始日期（年 月 日）：");
    	scanf("%d %d %d",&startdate.year,&startdate.month,&startdate.day);
    	printf("请输入结束日期（年 月 日）：");
    	scanf("%d %d %d",&enddate.year,&enddate.month,&enddate.day);
    	getchar();
    	int a;
    	a= SalesAnalysis_Srv_CompSaleVal(id,startdate,enddate);
    	printf("销售额；%d\n",a);
	}else{
		printf("用户不存在\n");
		getchar();
		return;
	}
}
