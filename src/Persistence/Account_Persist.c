#include "Account_Persist.h"
#include "../Service/Account.h"
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

static const char ACCOUNT_DATA_FILE[] = "Account.dat";
static const char ACCOUNT_DATA_TEMP_FILE[] = "AccountTmp.dat";

//��Ӷ���������ʶ����
static const char ACCOUNT_KEY_NAME[] = "Account";



//�ж�ϵͳ�û��ļ��Ƿ���ڣ�TTMS_SCU_Account_Perst_CheckAccFile��1
int Account_Perst_CheckAccFile()
{
	return access(ACCOUNT_DATA_FILE,0);//������ 
}//MTG 



//�洢��ϵͳ�û���TTMS_SCU_Account_Perst_Insert�� 2
int Account_Perst_Insert(account_t *data)
{
	assert(NULL!=data);
	
	////����������Ʒ���������ӵĴ���
	////����������Ʒ���������ӵĴ���
	long key = EntKey_Perst_GetNewKeys(ACCOUNT_KEY_NAME, 1); //Ϊ���˻������ȡ
	if(key<=0)			//��������ʧ�ܣ�ֱ�ӷ���
		return 0;
	data->id = key;		//�����¶�����ص�UI��
	////����������Ʒ���������ӵĴ���
	////����������Ʒ���������ӵĴ���
	
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



//����ϵͳ�û���TTMS_SCU_Account_Perst_Update��3
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
��ʶ����TTMS_SCU_Account_Perst_RemByID
�������ܣ����ڴ��ļ��и���IDɾ��һ��ϵͳ�û������ݡ�
����˵������һ������IDΪ���ͣ���ʾ��Ҫ�������ݵ��û�ID���ڶ�������bufΪaccount_tָ�룬ָ�������û����ݵ�ָ�롣
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ�ɾ�����û��ı�־��
*/


//4
int Account_Perst_RemByID(int id)
{
	//��ԭʼ�ļ���������Ȼ���ȡ��������д�뵽�����ļ��У�����Ҫɾ����ʵ����˵���

	//��ԭʼ�����ļ�������
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

	//ɾ����ʱ�ļ�
	remove(ACCOUNT_DATA_TEMP_FILE);
	return found;
}



int Account_Perst_RemByUsrName(char uNAME[])
	{
		//��ԭʼ�ļ���������Ȼ���ȡ��������д�뵽�����ļ��У�����Ҫɾ����ʵ����˵���

		//��ԭʼ�����ļ�������
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

	//ɾ����ʱ�ļ�
	remove(ACCOUNT_DATA_TEMP_FILE);
	return found;
}//MTG


//����ȫ��ϵͳ�û� ��TTMS_SCU_Account_Perst_SelectAll��5
int Account_Perst_SelectAll(account_list_t list)
{
	account_node_t *newNode;
	account_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, account_node_t);

	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb");
	if (NULL == fp) { //�ļ�������
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
	assert(NULL!=buf);//ʹ�� assert ������鴫���ָ�� buf �Ƿ�Ϊ�ա����Ϊ�գ��������ֹ�����ڵ���ʱ��������Ϣ

	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb");//��һ���Զ�����ֻ����ʽ�򿪵��ļ������ļ���Ϊ ACCOUNT_DATA_FILE��
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
	//��ԭʼ�ļ���������Ȼ���ȡ��������д�뵽�����ļ��У�����Ҫɾ����ʵ����˵���

	//��ԭʼ�����ļ�������
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

	//ɾ����ʱ�ļ�
	remove(ACCOUNT_DATA_TEMP_FILE);
	return found;
}//MTG

//���˺��ļ��в��������idƥ����˺ţ���ͨ��ָ��buf������ƥ��ɹ� return 1������ return 0��
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








