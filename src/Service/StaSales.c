//ͳ�����۶�ҵ���߼���

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>

#include "Studio.h"
#include "../Service/StaSales.h"
#include "../Persistence/Studio_Persist.h"
#include "../Common/List.h"
#include "../View/StaSales_UI.h"
#include "../Common/common.h"
#include"../Service/Sale.h"
#include"../Persistence/Sale_Persist.h"
int StaSales_Srv_CompSaleVal(int usrID,ttms_date_t stDate,ttms_date_t enddate){
	int amount=0;
	
    
    sale_list_t saleList;
    
    
    sale_node_t* pSale;/*���� sale_node_t ָ����� pSale*/

    List_Init(saleList,sale_node_t);

	Sale_Perst_SelByID(saleList, usrID); //�����û� usrID��ƱԱ�����ۼ�¼!!!
	 
	 List_ForEach(saleList, pSale);//�������� 
	 {
	if (pSale->data.date.day <= enddate.day && pSale->data.date.day >= stDate.day &&
		pSale->data.date.year <= enddate.year && pSale->data.date.year >= stDate.year &&
		pSale->data.date.month <= enddate.month && pSale->data.date.month >= stDate.month
		) {
		amount += pSale->data.value;
	}
}
    
    
    return amount;
}
