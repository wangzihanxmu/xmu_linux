#ifndef _MY_SV_IPC_H
#define _MY_SV_IPC_H

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>

union semun
{   int val;				      //������SETVAL���� 
	struct semid_ds * buf;	//����IPC_SET������ 
	ushort * array;			    //����SETALL������
};


/*new_sem---------------------------------------
 *�������ź��������ҳ�ʼ��
 *path,id:��������IPC��
 *sem_num:ָ���źŵƼ����а������źŵ���
 *val:    �źŵƳ�ֵ
 *����ֵ���źŵƱ�ʶ��
 */
int new_sem(char * path ,int id ,int sem_num ,int val);

/*down------------------------------------------
 *��ȡ�ź������ɹ���ȡ���ź�������ֵ��1����ȡ���ɹ����̽���˯��״̬
 *sem_id:�źŵƱ�ʶ��
 *sem_NO:�źŵƺ�
 *�շ���ֵ
 */ 
void down(int sem_id,int sem_NO);

/*up--------------------------------------------
 *�ͷ��ź������ź�������ֵ��1����ʱ�ڴ��ź����ϵȴ��Ľ��̿��Ի����
 *sem_id:�źŵƱ�ʶ��
 *sem_NO:�źŵƺ�
 *�շ���ֵ
 */
void up(int sem_id,int sem_NO);

/*clear_sem-------------------------------------
 *����ź���
 *sem_id:�źŵƱ�ʶ��
 *�շ���ֵ
 */
void clear_sem(int sem_id);

/*new_shm---------------------------------------
 *���������ڴ���
 *path,id:��������IPC��
 *shmsize�������ڴ����Ĵ�С�����ֽ�Ϊ��λ
 *����ֵ���ڴ�����ʶ��
 */
int new_shm(char *path,int id,size_t shmsize);

/*my_shmat--------------------------------------
 *��һ���򿪵Ĺ����ڴ������ӵ����ý��̵ĵ�ַ�ռ�
 *shmid:�ڴ�����ʶ��
 *����ֵ:���ڴ�ӳ������ʼ��ַ
 */
void * my_shmat(int shmid);

/*my_shmdt--------------------------------------
 *�жϹ����ڴ����뱾���̵�ַ�ռ����ϵ
 *addr:�ڴ�ӳ������ʼ��ַ
 *�շ���ֵ
 */
void my_shmdt(const void *addr);

/*clear_shm-------------------------------------
 *��������ڴ�
 *shmid:�ڴ�����ʶ��
 *�շ���ֵ
 */
void clear_shm(int shmid);

#endif
