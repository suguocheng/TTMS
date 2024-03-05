/*
* Copyright(C), 2007-2008, XUPT Univ.	 
* File name: Main_Menu.c			 	 
* Description : TTMS 系统主菜单	
* Author:   XUPT  		 
* Version:  v.1 	 
* Da
*/

#include "../View/Main_Menu.h"
#include"Account_UI.h"
#include <stdio.h>
#include"../View/Play_UI.h"
#include "../View/Studio_UI.h"
#include"SalesAnalysis_UI.h"
#include"StaSales_UI.h"
#include<Windows.h>
//系统主菜单 
void Main_Menu(void) {
	char choice;
	SysLogin();
	do { 

		
		system("cls");
		printf("\n==================================================================\n");
		printf("---------------- Theater Ticket Management System ----------------\n\n");
		printf("                     [S]tudio Management.\n");
		printf("                     [P]lay Management.\n");
		printf("                     [T]icket Sale.\n");
		printf("                     [R]eturn Ticket.\n");
		printf("                     [Q]ueries\n");
		printf("                     Ra[n]king and Statistics.\n");
		printf("                     [A]ccount Management.\n");
		printf("                     [E]xit.\n");
		printf("\n==================================================================\n");
		printf("Please input your choice:");
		fflush(stdin);
		choice = getchar();
		switch (choice) {
		case 'S':
		case 's':
			Studio_UI_MgtEntry();
			break;
		case 'P':
		case 'p':
			Play_UI_MgtEntry();
			break;
		case 'Q':
		case 'q':
			Schdule_UI_ListAll();
			break;
		case 'T':
		case 't':
			Sale_UI_MgtEntry();
			break;
		case 'R':
		case 'r':
			Sale_UI_ReturnTicket();
			break;
		case 'N':
		case 'n':
			SalesAnalysis_UI_MgtEntry();
			 break;
		case 'A':
		case 'a':
			Account_UI_MgtEntry();
			break;
		}
	} while ('E' != choice && 'e' != choice);
}

