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
int sem_hotel;  //�ź���
int shm_hotel;  //�����ڴ�

int main(int argc, char **argv)
{
	

	int cus_num;
	int cus_id=0;

	int i,j;
	pid_t pid;

	//--------------------------------�����ź����͹����ڴ�;
	sem_hotel=new_sem("./" ,770 ,1 ,1);
	shm_hotel=new_shm("./",111, sizeof(class hotel));
	//-------------------------------���ļ���ȡhotel��customer��Ϣ;
	FILE *fp;
	if((fp=fopen(argv[1],"r"))==NULL)
	{
		printf("Can not open file [%s]\n",argv[1]);
		exit(0);
	}

	myhotel=(class hotel *)my_shmat( shm_hotel ); //��hotel��Ϣӳ�䵽�����ڴ�ռ�
	read_hotel_msg(fp,myhotel);

	my_shmdt((const void *)myhotel);

	read_cust_msg(fp,cus_num, cus_vec);




	//---------------------------------��ʼ����ͻ�����;
	printf("Begin-------------------------------\n");
	for(i=1; i<=cus_num; i++)
		if ((pid=fork())<0) 
		{
			perror("Fork failed");
			exit(0);
		}
		else if(pid==0) //�ӽ���;
		{
			cus_id=i;
			break;
		}

	if(cus_id!=0)//�ӽ���;
	{  //printf("%dopnum:%d\n",cus_id,cus_vec.at(cus_id-1).op_num); 
		for(j=0;j<cus_vec.at(cus_id-1).op_num;j++)
		{
			down(sem_hotel,0);//��ȡ�����ź���
			myhotel=(class hotel *)my_shmat( shm_hotel ); 

			myhotel->do_request (cus_vec.at(cus_id-1).name, cus_vec.at(cus_id-1).cus_op[j] );

			my_shmdt((const void *)myhotel);
			up(sem_hotel,0);//�ͷŲ����ź���
		}
	 
		exit(cus_id);
	}
	else        //������;
	{
		for(j=1; j<=cus_num; j++)  //�ȴ������ӽ��̽���;
			wait(NULL);

		myhotel=(class hotel *)my_shmat( shm_hotel ); 
		myhotel->print();
		my_shmdt((const void *)myhotel);
		//ɾ���ź����͹����ڴ� ;
		clear_sem(sem_hotel); 
		clear_shm(shm_hotel);   
		
		printf("End-------------------------------\n");
	}

	return 0;
}
