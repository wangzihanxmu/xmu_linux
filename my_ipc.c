#include "my_ipc.h"

/*new_sem---------------------------------------
 *�������ź��������ҳ�ʼ��
 *path,id:��������IPC��
 *sem_num:ָ���źŵƼ����а������źŵ���
 *val:    �źŵƳ�ֵ
 *����ֵ���źŵƱ�ʶ��
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
 *��ȡ�ź������ɹ���ȡ���ź�������ֵ��1����ȡ���ɹ����̽���˯��״̬
 *sem_id:�źŵƱ�ʶ��
 *sem_NO:�źŵƺ�
 *�շ���ֵ
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
 *�ͷ��ź������ź�������ֵ��1����ʱ�ڴ��ź����ϵȴ��Ľ��̿��Ի����
 *sem_id:�źŵƱ�ʶ��
 *sem_NO:�źŵƺ�
 *�շ���ֵ
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
 *����ź���
 *sem_id:�źŵƱ�ʶ��
 *�շ���ֵ
 */
void clear_sem(int sem_id)
{
	
	if(semctl(sem_id,0,IPC_RMID) == -1)
         perror("semctl_IPC_RMID error");
	
}

/*new_shm---------------------------------------
 *���������ڴ���
 *path,id:��������IPC��
 *shmsize�������ڴ����Ĵ�С�����ֽ�Ϊ��λ
 *����ֵ���ڴ�����ʶ��
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
	//�ڴ��ʼ��
	p=(char *)my_shmat(shm_id);
	p[0]='\0';
	my_shmdt((const void *)p);
    
    return shm_id;
}

/*my_shmat--------------------------------------
 *��һ���򿪵Ĺ����ڴ������ӵ����ý��̵ĵ�ַ�ռ�
 *shm_id:�ڴ�����ʶ��
 *����ֵ:���ڴ�ӳ������ʼ��ַ
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
 *�жϹ����ڴ����뱾���̵�ַ�ռ����ϵ
 *addr:�ڴ�ӳ������ʼ��ַ
 *�շ���ֵ
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
 *��������ڴ�
 *shmid:�ڴ�����ʶ��
 *�շ���ֵ
 */
void clear_shm(int shm_id)
{
	if(shmctl(shm_id,IPC_RMID,NULL) == -1)
	{
		perror("shmctl_IPC_RMID error");
		exit(0);
	}
}