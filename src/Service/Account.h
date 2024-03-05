#ifndef ACCOUNT_H_
#define ACCOUNT_H_


/*
#ifndef SEAT_H_
#define SEAT_H_

//座位状态 
typedef enum{
	SEAT_NONE=0,			//空位
	SEAT_GOOD=1,			//有座位
	SEAT_BROKEN=9			//损坏的座位
}seat_status_t;

//座位数据结构 
typedef struct {
	int id;					//座位id
	int roomID;				//所在演出厅id
	int row;           		//座位行号
    int column;        		//座位列号
    seat_status_t status;	//座位在该行的状态，0表示没有座位，1表示有座位。扩展2可表示座位坏了
} seat_t;


//双向链表
typedef struct seat_node {
	seat_t data;
	struct seat_node *next, *prev;
} seat_node_t, *seat_list_t;
*/ 



//用户类型（枚举类型） 
typedef enum{
	USR_ANOMY=0, 	                    //匿名类型，初始化账号时使用
	USR_CLERK=1,		            	//售票员 
	USR_MANG=2,                         //剧院经理 
	USR_ADMIN=9                         //系统管理员
}account_type_t;


//系统用户数据结构 
typedef struct {
	int id;                             //用户ID 
	account_type_t type;                //用户类型，0为系统管理员，1为剧院经理，2为售票员 
	char username[30];		   	        //用户名 
	char password[30];		            //用户密码 
} account_t;




account_t gl_CurUser;


//双向链表
typedef struct account_node {
	account_t data;                     //实体数据 
	struct account_node *next, *prev;   //前后向指针 
} account_node_t, *account_list_t;//account_list_t list?????







//创建管理员Admin匿名系统用户（TTMS_SCU_Account_ Srv_InitSys) 1
void Account_Srv_InitSys();
 
//验TTMS_SCU_Account_Srv_Verify)2 
int Account_Srv_Verify(char usrName[],char pwd[]); 

//添加新系统用户（TTMS_SCU_Account_Srv_Add）3
int Account_Srv_Add(const account_t *data);

//修改系统用户（TTMS_SCU_Account_Srv_Mod） 4
int Account_Srv_Modify(const account_t *data);

//删除系统用户（TTMS_SCU_Account_Srv_DelByID） 5
int Account_Srv_DeleteByID(int usrID);

//获取所有系统用户（TTMS_SCU_Account_Srv_FetchAll）6
int Account_Srv_FetchAll(account_list_t list); 

//根据用户名获取系统用户指针（TTMS_SCU_Account_Srv_FindByUsrName）7 
account_node_t * Account_Srv_FindByUsrName(account_list_t list,char usrName[]); 

account_node_t * Account_Srv_FetchByName(char usrName[],account_list_t list);

char Account_Srv_FetchNameByName(char usrName[], account_list_t list);

int Account_Srv_FetchIDByName(char usrName[], account_list_t list);
int Account_Srv_FetchByID(int usrID, account_t* buf);

#endif //USERACCOUNT_H_
