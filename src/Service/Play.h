
#ifndef PLAY_H_
#define PLAY_H_

typedef enum{
	PLAY_TYPE_FILM=1,
	PLAT_TYPE_OPERA=2,
	PLAY_TYPE_CONCERT=3
}play_type_t; //影片类型 
typedef enum{
	PLAY_RATE_CHILD=1,
	PLAT_RATE_TEENAGE=2,
	PLAY_RATE_ADULT=3
}play_rating_t; //影片分级 

typedef struct{
	int year;
	int month;
	int day;
}ttms_date_t;//日期定义 

typedef struct {
	int id;			//表演ID
	char name[30];//表演名称
	play_type_t type;//类型 
	char area[9];//上映地区 
	play_rating_t rating;//分级 
	int duration;//影片时长 min
	ttms_date_t start_date;//开始放映日期 
	ttms_date_t end_date;//结束放映日期
	int price; 
} play_t;

//双向链表
typedef struct play_node {
	play_t data;			//实体数据
	struct play_node *next;	//后向指针
	struct play_node *prev;	//前向指针
} play_node_t, *play_list_t;


int Play_Srv_FetchAll(play_list_t list);/*list参数作为实参调用persis载入全部剧目信息 
 返回persistent层载入全部剧目信息函数的返回值*/  
int Play_Srv_Add(play_t *data);//调用persis存储新剧目函数；返回persistent的返回值

int Play_Srv_FetchByID(int id,play_t *buf); //传递一个buf作为指向剧目的指针并向底层传递id 
 
int Play_Srv_Modify(const play_t *data);
 
int Play_Srv_DeleteByID(int id);
int Play_Srv_FetchByName(play_list_t, char cound[]);
void Play_Srv_Print(play_list_t list, char s[]);

 #endif //PLAY_H_
