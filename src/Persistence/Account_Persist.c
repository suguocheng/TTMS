#include "Account_Persist.h"
#include "../Service/Account.h"
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

static const char ACCOUNT_DATA_FILE[] = "Account.dat";
static const char ACCOUNT_DATA_TEMP_FILE[] = "AccountTmp.dat";

//添加对象主键标识名称
static const char ACCOUNT_KEY_NAME[] = "Account";



//判断系统用户文件是否存在（TTMS_SCU_Account_Perst_CheckAccFile）1
int Account_Perst_CheckAccFile()
{
	return access(ACCOUNT_DATA_FILE,0);//待调试 
}//MTG 



//存储新系统用户（TTMS_SCU_Account_Perst_Insert） 2
int Account_Perst_Insert(account_t *data)
{
	assert(NULL!=data);
	
	////以下是新设计方案方案添加的代码
	////以下是新设计方案方案添加的代码
	long key = EntKey_Perst_GetNewKeys(ACCOUNT_KEY_NAME, 1); //为新账户分配获取
	if(key<=0)			//主键分配失败，直接返回
		return 0;
	data->id = key;		//赋给新对象带回到UI层
	////以上是新设计方案方案添加的代码
	////以上是新设计方案方案添加的代码
	
	FILE *fp = fopen(ACCOUNT_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(account_t), 1, fp);

	fclose(fp);
	return rtn;
}//MTG



//更新系统用户（TTMS_SCU_Account_Perst_Update）3
int Account_Perst_Update(account_t *data)
{
	assert(NULL!=data);

	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}
	
	account_t buf;
	int found = 0;

	while (!feof(fp)) 
	{
		if (fread(&buf, sizeof(account_t), 1, fp)) {
			if (!strcmp(buf.username,data->username)) {///
				fseek(fp, -((int)sizeof(account_t)), SEEK_CUR);
				fwrite(data, sizeof(account_t), 1, fp);
				found = 1;
				break;
			}
		}
	}
	fclose(fp);
	return found;
	//MTG
}


/*
标识符：TTMS_SCU_Account_Perst_RemByID
函数功能：用于从文件中根据ID删除一个系统用户的数据。
参数说明：第一个参数ID为整型，表示需要载入数据的用户ID；第二个参数buf为account_t指针，指向载入用户数据的指针。
返 回 值：整型，表示是否成功删除了用户的标志。
*/


//4
int Account_Perst_RemByID(int id)
{
	//将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。

	//对原始数据文件重命名
	if (rename(ACCOUNT_DATA_FILE, ACCOUNT_DATA_TEMP_FILE) < 0) {
		printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	FILE* fpSour, * fpTarg;
	fpSour = fopen(ACCOUNT_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour) {
		printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(ACCOUNT_DATA_FILE, "wb");
	if (NULL == fpTarg) {
		printf("Cannot open file %s!\n", ACCOUNT_DATA_TEMP_FILE);
		return 0;
	}
	account_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(account_t), 1, fpSour)) {
			if (buf.id==id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(account_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(ACCOUNT_DATA_TEMP_FILE);
	return found;
}



int Account_Perst_RemByUsrName(char uNAME[])
	{
		//将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。

		//对原始数据文件重命名
		if (rename(ACCOUNT_DATA_FILE, ACCOUNT_DATA_TEMP_FILE) < 0) {
			printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
			return 0;
		}

		FILE* fpSour, * fpTarg;
		fpSour = fopen(ACCOUNT_DATA_TEMP_FILE, "rb");
		if (NULL == fpSour) {
			printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
			return 0;
		}

		fpTarg = fopen(ACCOUNT_DATA_FILE, "wb");
		if (NULL == fpTarg) {
			printf("Cannot open file %s!\n", ACCOUNT_DATA_TEMP_FILE);
			return 0;
		}

	account_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(account_t), 1, fpSour)) {
			if (!strcmp(buf.username,uNAME)) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(account_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(ACCOUNT_DATA_TEMP_FILE);
	return found;
}//MTG


//载入全部系统用户 （TTMS_SCU_Account_Perst_SelectAll）5
int Account_Perst_SelectAll(account_list_t list)
{
	account_node_t *newNode;
	account_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, account_node_t);

	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(account_t), 1, fp)) {
			newNode = (account_node_t*)malloc(sizeof(account_node_t));
			if (!newNode) {
				printf(
						"Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;////////////////////	
}
//MTG


int Account_Perst_SelByName(char usrName[], account_t* buf) {
	assert(NULL!=buf);//使用 assert 宏来检查传入的指针 buf 是否为空。如果为空，程序会终止，并在调试时输出相关信息

	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb");//打开一个以二进制只读方式打开的文件流，文件名为 ACCOUNT_DATA_FILE。
	if (NULL == fp) {
		return 0;
	}

	account_t data;

	while (!feof(fp)) {
		if (fread(&data, sizeof(account_t), 1, fp)) {
			if (!strcmp(usrName,data.username)){
				*buf = data;
				return 1;
			};

		}
	}
	fclose(fp);

	return 0;
}//MTG


int Account_Perst_DeleteByID(int id)
{
	//将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。

	//对原始数据文件重命名
	if(rename(ACCOUNT_DATA_FILE, ACCOUNT_DATA_TEMP_FILE)<0){
		printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(ACCOUNT_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(ACCOUNT_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("Cannot open file %s!\n", ACCOUNT_DATA_TEMP_FILE);
		return 0;
	}


	account_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(account_t), 1, fpSour)) {
			if (id == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(account_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(ACCOUNT_DATA_TEMP_FILE);
	return found;
}//MTG

//在账号文件中查找与参数id匹配的账号，并通过指针buf传出；匹配成功 return 1；否则 return 0；
int Account_Perst_SelectByID(int id, account_t* buf) {
	assert(NULL != buf);

	FILE* fp = fopen(ACCOUNT_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	account_t data;
	int found = 0;
	while (!feof(fp)) {
		if (fread(&data, sizeof(account_t), 1, fp)) {
			if (id == data.id) {
				*buf = data;
				found = 1;
				break;
			};
		}
	}
	fclose(fp);

	return found;
}








