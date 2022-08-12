#include "my_ipc.h"

/*new_sem---------------------------------------
 *创建新信号量，并且初始化
 *path,id:用于生成IPC键
 *sem_num:指明信号灯集合中包含的信号灯数
 *val:    信号灯初值
 *返回值：信号灯标识符
 */
int new_sem(char * path ,int id ,int sem_num ,int val)
{
	key_t ipc_key;
    int sem_id,i;
    union semun options;

	ipc_key = ftok(path,id);
    if( ipc_key == -1)
    {
        perror("ftok error");
        exit(0);
    }
    
    sem_id = semget(ipc_key,sem_num,IPC_CREAT|0666);
    if(sem_id ==-1)
    {
        perror("semget_IPC_CREAT error");
        exit(0);
    }

    options.val = val;

    for(i = 0 ; i < sem_num ; i++)
    {
        if(semctl(sem_id,i,SETVAL,options) == -1)
        {
            perror("semctl_setval error");
            exit(0);
        }
    }
    return sem_id;
}

/*down------------------------------------------
 *获取信号量，成功获取后信号量计数值减1，获取不成功进程进入睡眠状态
 *sem_id:信号灯标识符
 *sem_NO:信号灯号
 *空返回值
 */ 
void down(int sem_id,int sem_NO)
{
   struct sembuf op;
   op.sem_num = sem_NO;
   op.sem_op = -1;
   op.sem_flg = 0;
   if(semop(sem_id, &op, 1) == -1)
   {
   	 perror("semop_down error");
   	 exit(0);
   }
}

/*up--------------------------------------------
 *释放信号量，信号量计数值加1，此时在此信号量上等待的进程可以获得它
 *sem_id:信号灯标识符
 *sem_NO:信号灯号
 *空返回值
 */
void up(int sem_id,int sem_NO)
{
	 struct sembuf op;
   op.sem_num = sem_NO;
   op.sem_op = 1;
   op.sem_flg = 0;
   if(semop(sem_id, &op, 1) == -1)
   {
   	 perror("semop_up error");
   	 exit(0);
   }
}

/*clear_sem-------------------------------------
 *清除信号量
 *sem_id:信号灯标识符
 *空返回值
 */
void clear_sem(int sem_id)
{
	
	if(semctl(sem_id,0,IPC_RMID) == -1)
         perror("semctl_IPC_RMID error");
	
}

/*new_shm---------------------------------------
 *创建共享内存区
 *path,id:用于生成IPC键
 *shmsize：共享内存区的大小，以字节为单位
 *返回值：内存区标识符
 */
int new_shm(char *path,int id,size_t shmsize)
{
	key_t ipc_key;
    int shm_id ,i;
	char *p;
   
    ipc_key = ftok(path,id);
    if( ipc_key == -1)
    {
        perror("ftok error");
        exit(0);
    }

	shm_id = shmget(ipc_key,shmsize,IPC_CREAT|0666);
    if(shm_id == -1)
    {
        perror("shmget_IPC_CREAT error");
        exit(0);
    }
	//内存初始化
	p=(char *)my_shmat(shm_id);
	p[0]='\0';
	my_shmdt((const void *)p);
    
    return shm_id;
}

/*my_shmat--------------------------------------
 *将一个打开的共享内存区附接到调用进程的地址空间
 *shm_id:内存区标识符
 *返回值:该内存映射区起始地址
 */
void* my_shmat(int shm_id)
{
	void *p;
	char *q;
	p=shmat(shm_id,NULL,0);
	q=(char *)p;
	if( ((long)q) == -1 )
	{
		perror("shmat error");
		exit(0);		
	}
	return p;
}

/*my_shmdt--------------------------------------
 *切断共享内存区与本进程地址空间的联系
 *addr:内存映射区起始地址
 *空返回值
 */
void my_shmdt(const void *addr)
{
	if(shmdt(addr) == -1)
	{
		perror("shmdt error");
		exit(0);
	}
}

/*clear_shm-------------------------------------
 *清除共享内存
 *shmid:内存区标识符
 *空返回值
 */
void clear_shm(int shm_id)
{
	if(shmctl(shm_id,IPC_RMID,NULL) == -1)
	{
		perror("shmctl_IPC_RMID error");
		exit(0);
	}
}