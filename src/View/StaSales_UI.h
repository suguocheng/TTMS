
#ifndef STASALES_UI_H_
#define STASALES_UI_H_

#include "../Service/Account.h"
#include "../Service/SalesAnalysis.h"
#include "../Common/list.h"
#include "../Service/Sale.h"

static const int STASALES_PAGE_SIZE=6;

//统计销售额界面 
void StaSales_UI_MgtEntry();/*函数声明*/

//统计个人销售额界面
void StaSales_UI_Self();

//统计售票员销售额界面
void StaSales_UI_Clerk() ;

#endif/* STASALES_UI_H_ */
