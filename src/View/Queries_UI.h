#ifndef QUERIES_UI_H_
#define QUERIES_UI_H_
//分页数据结构体，简称分页器类型

//查询演出界面
void Schdule_UI_ListAll(void);
//根据剧目名称获取演出计划
int Schedule_UI_Query(char* play_name);
void QTicket_UI_Query(int schedule_id);

#endif /* QUERIES_UI_H_ */
