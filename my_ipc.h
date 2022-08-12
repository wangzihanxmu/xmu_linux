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
{   int val;				      //仅用于SETVAL命令 
	struct semid_ds * buf;	//用于IPC_SET等命令 
	ushort * array;			    //用于SETALL等命令
};


/*new_sem---------------------------------------
 *创建新信号量，并且初始化
 *path,id:用于生成IPC键
 *sem_num:指明信号灯集合中包含的信号灯数
 *val:    信号灯初值
 *返回值：信号灯标识符
 */
int new_sem(char * path ,int id ,int sem_num ,int val);

/*down------------------------------------------
 *获取信号量，成功获取后信号量计数值减1，获取不成功进程进入睡眠状态
 *sem_id:信号灯标识符
 *sem_NO:信号灯号
 *空返回值
 */ 
void down(int sem_id,int sem_NO);

/*up--------------------------------------------
 *释放信号量，信号量计数值加1，此时在此信号量上等待的进程可以获得它
 *sem_id:信号灯标识符
 *sem_NO:信号灯号
 *空返回值
 */
void up(int sem_id,int sem_NO);

/*clear_sem-------------------------------------
 *清除信号量
 *sem_id:信号灯标识符
 *空返回值
 */
void clear_sem(int sem_id);

/*new_shm---------------------------------------
 *创建共享内存区
 *path,id:用于生成IPC键
 *shmsize：共享内存区的大小，以字节为单位
 *返回值：内存区标识符
 */
int new_shm(char *path,int id,size_t shmsize);

/*my_shmat--------------------------------------
 *将一个打开的共享内存区附接到调用进程的地址空间
 *shmid:内存区标识符
 *返回值:该内存映射区起始地址
 */
void * my_shmat(int shmid);

/*my_shmdt--------------------------------------
 *切断共享内存区与本进程地址空间的联系
 *addr:内存映射区起始地址
 *空返回值
 */
void my_shmdt(const void *addr);

/*clear_shm-------------------------------------
 *清除共享内存
 *shmid:内存区标识符
 *空返回值
 */
void clear_shm(int shmid);

#endif
