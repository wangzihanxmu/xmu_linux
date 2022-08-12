#include "hotel.h"
#include "my_ipc.h"
#include <stdlib.h>

#include<sys/types.h>

#include<sys/ipc.h>

#include<sys/sem.h>

#include<sys/shm.h>

#include<sys/time.h>

#include<sys/wait.h>

#include<unistd.h>

#include<fcntl.h>

#include<stdbool.h>
#include <vector>
using namespace std;

vector<struct customer> cus_vec;
class hotel *myhotel;
int sem_hotel;  //信号量
int shm_hotel;  //共享内存

int main(int argc, char **argv)
{
	

	int cus_num;
	int cus_id=0;

	int i,j;
	pid_t pid;

	//--------------------------------创建信号量和共享内存;
	sem_hotel=new_sem("./" ,770 ,1 ,1);
	shm_hotel=new_shm("./",111, sizeof(class hotel));
	//-------------------------------从文件读取hotel和customer信息;
	FILE *fp;
	if((fp=fopen(argv[1],"r"))==NULL)
	{
		printf("Can not open file [%s]\n",argv[1]);
		exit(0);
	}

	myhotel=(class hotel *)my_shmat( shm_hotel ); //将hotel信息映射到共享内存空间
	read_hotel_msg(fp,myhotel);

	my_shmdt((const void *)myhotel);

	read_cust_msg(fp,cus_num, cus_vec);




	//---------------------------------开始处理客户请求;
	printf("Begin-------------------------------\n");
	for(i=1; i<=cus_num; i++)
		if ((pid=fork())<0) 
		{
			perror("Fork failed");
			exit(0);
		}
		else if(pid==0) //子进程;
		{
			cus_id=i;
			break;
		}

	if(cus_id!=0)//子进程;
	{  //printf("%dopnum:%d\n",cus_id,cus_vec.at(cus_id-1).op_num); 
		for(j=0;j<cus_vec.at(cus_id-1).op_num;j++)
		{
			down(sem_hotel,0);//获取操作信号量
			myhotel=(class hotel *)my_shmat( shm_hotel ); 

			myhotel->do_request (cus_vec.at(cus_id-1).name, cus_vec.at(cus_id-1).cus_op[j] );

			my_shmdt((const void *)myhotel);
			up(sem_hotel,0);//释放操作信号量
		}
	 
		exit(cus_id);
	}
	else        //父进程;
	{
		for(j=1; j<=cus_num; j++)  //等待所有子进程结束;
			wait(NULL);

		myhotel=(class hotel *)my_shmat( shm_hotel ); 
		myhotel->print();
		my_shmdt((const void *)myhotel);
		//删除信号量和共享内存 ;
		clear_sem(sem_hotel); 
		clear_shm(shm_hotel);   
		
		printf("End-------------------------------\n");
	}

	return 0;
}
