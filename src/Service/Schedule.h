#ifndef SCHEDULE_H_
#define SCHEDULE_H_
#include <stdlib.h>
#include "../Common/List.h"
#include "Play.h"
typedef struct {
	int hour;
	int minute;
	int second;
}ttms_time_t;
typedef struct {
	int id;
	int play_id;
	int studio_id;
	ttms_date_t date;
	ttms_time_t time;
	int seat_count;
}schedule_t;

typedef struct schedule_node {
	schedule_t data;
	struct schedule_node* next;
	struct schedule_node* prev;
}schedule_node_t,*schedule_list_t;
int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id);
int Schedule_Srv_Add(schedule_t* data);
int Schedule_Srv_Modify(const schedule_t* data);
int Schedule_Srv_DeleteByID(int id);
int Schedule_Srv_FetchByID(int id, schedule_t* buf);
int Schedule_Srv_DeleteAllByPlayID(int play_id);
int Schedule_Srv_FetchAll(schedule_list_t list);
int Schedule_Srv_FetchByID_ticket(int id, schedule_t* buf);

#endif
