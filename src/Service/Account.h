#ifndef ACCOUNT_H_
#define ACCOUNT_H_


/*
#ifndef SEAT_H_
#define SEAT_H_

//��λ״̬ 
typedef enum{
	SEAT_NONE=0,			//��λ
	SEAT_GOOD=1,			//����λ
	SEAT_BROKEN=9			//�𻵵���λ
}seat_status_t;

//��λ���ݽṹ 
typedef struct {
	int id;					//��λid
	int roomID;				//�����ݳ���id
	int row;           		//��λ�к�
    int column;        		//��λ�к�
    seat_status_t status;	//��λ�ڸ��е�״̬��0��ʾû����λ��1��ʾ����λ����չ2�ɱ�ʾ��λ����
} seat_t;


//˫������
typedef struct seat_node {
	seat_t data;
	struct seat_node *next, *prev;
} seat_node_t, *seat_list_t;
*/ 



//�û����ͣ�ö�����ͣ� 
typedef enum{
	USR_ANOMY=0, 	                    //�������ͣ���ʼ���˺�ʱʹ��
	USR_CLERK=1,		            	//��ƱԱ 
	USR_MANG=2,                         //��Ժ���� 
	USR_ADMIN=9                         //ϵͳ����Ա
}account_type_t;


//ϵͳ�û����ݽṹ 
typedef struct {
	int id;                             //�û�ID 
	account_type_t type;                //�û����ͣ�0Ϊϵͳ����Ա��1Ϊ��Ժ����2Ϊ��ƱԱ 
	char username[30];		   	        //�û��� 
	char password[30];		            //�û����� 
} account_t;




account_t gl_CurUser;


//˫������
typedef struct account_node {
	account_t data;                     //ʵ������ 
	struct account_node *next, *prev;   //ǰ����ָ�� 
} account_node_t, *account_list_t;//account_list_t list?????







//��������ԱAdmin����ϵͳ�û���TTMS_SCU_Account_ Srv_InitSys) 1
void Account_Srv_InitSys();
 
//��TTMS_SCU_Account_Srv_Verify)2 
int Account_Srv_Verify(char usrName[],char pwd[]); 

//�����ϵͳ�û���TTMS_SCU_Account_Srv_Add��3
int Account_Srv_Add(const account_t *data);

//�޸�ϵͳ�û���TTMS_SCU_Account_Srv_Mod�� 4
int Account_Srv_Modify(const account_t *data);

//ɾ��ϵͳ�û���TTMS_SCU_Account_Srv_DelByID�� 5
int Account_Srv_DeleteByID(int usrID);

//��ȡ����ϵͳ�û���TTMS_SCU_Account_Srv_FetchAll��6
int Account_Srv_FetchAll(account_list_t list); 

//�����û�����ȡϵͳ�û�ָ�루TTMS_SCU_Account_Srv_FindByUsrName��7 
account_node_t * Account_Srv_FindByUsrName(account_list_t list,char usrName[]); 

account_node_t * Account_Srv_FetchByName(char usrName[],account_list_t list);

char Account_Srv_FetchNameByName(char usrName[], account_list_t list);

int Account_Srv_FetchIDByName(char usrName[], account_list_t list);
int Account_Srv_FetchByID(int usrID, account_t* buf);

#endif //USERACCOUNT_H_
