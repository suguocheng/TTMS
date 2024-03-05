
#ifndef PLAY_H_
#define PLAY_H_

typedef enum{
	PLAY_TYPE_FILM=1,
	PLAT_TYPE_OPERA=2,
	PLAY_TYPE_CONCERT=3
}play_type_t; //ӰƬ���� 
typedef enum{
	PLAY_RATE_CHILD=1,
	PLAT_RATE_TEENAGE=2,
	PLAY_RATE_ADULT=3
}play_rating_t; //ӰƬ�ּ� 

typedef struct{
	int year;
	int month;
	int day;
}ttms_date_t;//���ڶ��� 

typedef struct {
	int id;			//����ID
	char name[30];//��������
	play_type_t type;//���� 
	char area[9];//��ӳ���� 
	play_rating_t rating;//�ּ� 
	int duration;//ӰƬʱ�� min
	ttms_date_t start_date;//��ʼ��ӳ���� 
	ttms_date_t end_date;//������ӳ����
	int price; 
} play_t;

//˫������
typedef struct play_node {
	play_t data;			//ʵ������
	struct play_node *next;	//����ָ��
	struct play_node *prev;	//ǰ��ָ��
} play_node_t, *play_list_t;


int Play_Srv_FetchAll(play_list_t list);/*list������Ϊʵ�ε���persis����ȫ����Ŀ��Ϣ 
 ����persistent������ȫ����Ŀ��Ϣ�����ķ���ֵ*/  
int Play_Srv_Add(play_t *data);//����persis�洢�¾�Ŀ����������persistent�ķ���ֵ

int Play_Srv_FetchByID(int id,play_t *buf); //����һ��buf��Ϊָ���Ŀ��ָ�벢��ײ㴫��id 
 
int Play_Srv_Modify(const play_t *data);
 
int Play_Srv_DeleteByID(int id);
int Play_Srv_FetchByName(play_list_t, char cound[]);
void Play_Srv_Print(play_list_t list, char s[]);

 #endif //PLAY_H_
