#include <stdlib.h>
#include "../Common/List.h"
#include"../Persistence/Schedule_Persist.h"
#include "../Service/Schedule.h"


int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id)
{
	return Schedule_Perst_SelectByPlay(list, play_id);
}
int Schedule_Srv_Add(schedule_t* data)
{
	return Schedule_Perst_Insert(data);
}
int Schedule_Srv_Modify(const schedule_t* data)
{
	int rtn = Schedule_Perst_Update(data);
	return rtn;
}
int Schedule_Srv_DeleteByID(int id)
{
	return Schedule_Perst_RemByID(id);
}

int Schedule_Srv_FetchByID(int id,schedule_t* buf)
{
	return Schedule_Perst_SelectByID(id,buf);
}
int Schedule_Srv_DeleteAllByPlayID(int play_id)
{
	return Schedule_Perst_RemByPlayID;
}
int Schedule_Srv_FetchAll(schedule_list_t list) {
	return Schedule_Perst_SelectAll(list);
}
int Schedule_Srv_FetchByID_ticket(int id, schedule_t* buf)
{
	return Schedule_Perst_SelectByID_ticket(id, buf);
}
