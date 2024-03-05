#ifndef SCHEDULE_PERSIST_H_
#define SCHEDULE_PERSIST_H_
#include "../Service/Schedule.h"
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id);
int Schedule_Perst_Insert(schedule_t* data);
int Schedule_Perst_Update(const schedule_t* data);
int Schedule_Perst_RemByID(int id);
int Schedule_Perst_SelectByID(int id, schedule_t* buf);
int Schedule_Perst_RemByPlayID(int play_id);
int Schedule_Perst_SelectAll(schedule_list_t list);
int Schedule_Perst_SelectByID_ticket(int id, schedule_t* buf);
#endif