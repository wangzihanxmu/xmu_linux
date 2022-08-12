#include<unistd.h>
#include <stdlib.h>
#include <string.h>
#include "hotel.h"
#define MAX_OP 20 //最多操作个数
#define MAX_ROOM 9 //房间最大数目

bool check_data(int year,int month,int day){
	if(year!=2022 && year!=2023)
		return false;
	else if ( (month >= 1) && (month <= 12) ){
		if ( month == 2 ){
			if ( day>=1 && day<=28 )
				return true;
		}
		else if (month==1||month==3||month==5||month==7||month==8||month==10||month==12){
			if ( day>=1 && day<=31 )
				return true;
		}
		else{
			if ( day>=1 && day<=30 )
				return true;
		}
	}
	return false;
}

void copy_res(struct reserved_info &a,struct reserved_info b){
	a.start_date.year=b.start_date.year;
	a.start_date.month=b.start_date.month;
	a.start_date.day=b.start_date.day;
	a.end_date.year=b.end_date.year;
	a.end_date.month=b.end_date.month;
	a.end_date.day=b.end_date.day;
	a.days=b.days;
	strcpy(a.name,b.name);
}
/////////////////////////////////////////////////////////////////////////////////////////////
room::room(){
		room_NO= -1 ;
		res_num=0;		
}

bool room::can_reserved(class date start_date,int days,int &loca){
	int i,j;
	class date end_date(start_date.year,start_date.month,start_date.day);
	end_date.add(days);
	i=0;j=1;
	if(res_num==0 || res_info[0].start_date.compareTo(end_date)>0){
		loca=0;
		//return true;
	}
	else{
		while(j!=res_num && res_info[i].end_date.compareTo(start_date) < 0){
			if(res_info[j].start_date.compareTo(end_date)>0 ){
				loca=j;
				break;
			}
			else{
				i++;  
				j++;
			}
		}
		if(j==res_num && res_info[i].end_date.compareTo(start_date) < 0){
			loca=j;
			//return true;
		}
		else
			loca=-1;
	}
	if(loca>=0)
		return true;
	else
		return false;
}

bool room::addreserve(class date start_date,int days,char name[50],int loca){
	class date end_date(start_date.year,start_date.month,start_date.day);
	end_date.add(days);
	struct reserved_info resinf;
	resinf.days=days;
	resinf.end_date=end_date;
	resinf.start_date=start_date;
	strcpy(resinf.name,name);
	for(int i=res_num;i>=loca+1;i--){
		copy_res(res_info[i],res_info[i-1]);
	}
	copy_res(res_info[loca],resinf);
	res_num++;
	return true;
}

bool room::delreserve(int loca){
	for(int i=loca;i<res_num;i++){
		copy_res(res_info[i],res_info[i+1]);
	}
	res_num--;
	return true;
}

bool room::search(class date start_date,int days,char name[50],int &loca){
	int i=0;
	while(i<res_num){
		if(strcmp(res_info[i].name,name)==0 && 
			res_info[i].start_date.year==start_date.year &&
			res_info[i].start_date.month==start_date.month && 
			res_info[i].start_date.day==start_date.day && 
			res_info[i].days==days)
		{
			loca=i;
			return true;
		}
		i++;
	}
	return false;
}

bool room::search_by_name(char name[50]){
	int i=0;
	bool flag=false;
	while(i<res_num){
		if(strcmp(res_info[i].name,name)==0){
			flag=true;
			printf("%s check result: reserve NO.%d from %d.%d.%d for %ddays \n",name,room_NO,
				res_info[i].start_date.year,res_info[i].start_date.month,res_info[i].start_date.day,
				res_info[i].days);
		}
		i++;
	}
	return flag;
}
///////////////////////////////////////////////////

bool hotel::init_hotel(vector<int>&vec){
	roomnum = vec.size() ;
	for ( int i = 0 ; i < vec.size() ; i++ ){
		rooms[i].room_NO = vec[i];
	}
	return true ;
}

int hotel::location_room ( int room_NO ){
	for ( int i = 0 ; i <roomnum ; i++ ){
		if ( rooms[i].room_NO == room_NO )
			return i ;
	}
	return -1 ;
}

bool hotel::do_request (char name[50], struct op cusop ){
	sleep(cusop.op_time/1000);//模拟操作时间
	if(strcmp(cusop.op_type,"reserve")==0){	
		if( !check_data(cusop.start_date.year,cusop.start_date.month,cusop.start_date.day) ){
			printf("%s %s from %d.%d.%d for %ddays **Error:date error \n",name,cusop.op_type,
				cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
			return false;
		}
		int loca=location_room(cusop.room_NO);
		if(loca==-1){
			printf("%s reserve NO.%d from %d.%d.%d for %ddays **Error:no this room \n",name,cusop.room_NO,
				cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
			return false;
		}
		int ite;
		if( rooms[loca].can_reserved(cusop.start_date,cusop.days,ite) ){
			rooms[loca].addreserve(cusop.start_date,cusop.days,name,ite);
			printf("%s reserve NO.%d from %d.%d.%d for %ddays **Success \n",name,cusop.room_NO,
				cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
			return true;
		}
		else{
			printf("%s reserve NO.%d from %d.%d.%d for %ddays **Fail:room has reserved \n",name,cusop.room_NO,
				cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
			return false;
		}

	}
	else if(strcmp(cusop.op_type,"cancel")==0){
		if( !check_data(cusop.start_date.year,cusop.start_date.month,cusop.start_date.day) ){
			printf("%s %s from %d.%d.%d for %ddays **Error:date error \n",name,cusop.op_type,
				cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
			return false;
		}
		int loca=location_room(cusop.room_NO);
		int ite;
		if( rooms[loca].search(cusop.start_date,cusop.days,name,ite) ){
			rooms[loca].delreserve(ite);
			printf("%s cancel NO.%d from %d.%d.%d for %ddays **Success \n",name,cusop.room_NO,
				cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
			return true;
		}
		else{
			printf("%s cancel NO.%d from %d.%d.%d for %ddays **Fail:no reserve information \n",name,cusop.room_NO,
				cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
			return false;
		}
	}
	else if(strcmp(cusop.op_type,"reserveblock")==0){
		if( !check_data(cusop.start_date.year,cusop.start_date.month,cusop.start_date.day) ){
			printf("%s %s from %d.%d.%d for %ddays **Error:date error \n",name,cusop.op_type,
				cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
			return false;
		}
		int loca=location_room(cusop.room_NO);
		vector<int> ite_vec;
		if(loca==-1||loca+cusop.room_num-1>MAX_ROOM-1){
			printf("%s reserveblock %drooms(NO.%d) from %d.%d.%d for %ddays **Error:no match rooms \n",name,cusop.room_num,
				cusop.room_NO,cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
			return false;
		}
		int ite;
		for(int i=loca;i<loca+cusop.room_num;i++){
			if( rooms[i].can_reserved(cusop.start_date,cusop.days,ite)){
				ite_vec.push_back(ite);
			}
			else{
				printf("%s reserveblock %drooms(NO.%d) from %d.%d.%d for %ddays **Error:no match rooms \n",name,cusop.room_num,
					cusop.room_NO,cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
				return false;
			}
		}

		vector<int>::iterator iite=ite_vec.begin();
		for(int i=loca;i<loca+cusop.room_num;i++)
		{
			rooms[i].addreserve(cusop.start_date,cusop.days,name,*iite);
			iite++;
		}

		printf("%s reserveblock %drooms(NO.%d) from %d.%d.%d for %ddays **Success \n",name,cusop.room_num,
			cusop.room_NO,cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
		return true;
		
	}
	else if(strcmp(cusop.op_type,"cancelblock")==0)
	{
		if( !check_data(cusop.start_date.year,cusop.start_date.month,cusop.start_date.day) )
		{
			printf("%s %s from %d.%d.%d for %ddays **Error:date error \n",name,cusop.op_type,
				cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
			return false;
		}
		int loca=location_room(cusop.room_NO);
		vector<int> ite_vec;

		int ite;
		for(int i=loca;i<loca+cusop.room_num;i++)
		{
			if( rooms[i].search(cusop.start_date,cusop.days,name,ite) )
			{
				ite_vec.push_back(ite);
			}
			else
			{
				printf("%s cancelblock %drooms(NO.%d) from %d.%d.%d for %ddays **Error:no match reserve imformation \n",name,cusop.room_num,
					cusop.room_NO,cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
				return false;
			}
		}

		vector<int>::iterator iite=ite_vec.begin();
		for(int i=loca;i<loca+cusop.room_num;i++)
		{
			rooms[i].delreserve(*iite);
			iite++;
		}

		printf("%s cancelblock %drooms(NO.%d) from %d.%d.%d for %ddays **Success \n",name,cusop.room_num,
			cusop.room_NO,cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
		return true;
	}
	else if(strcmp(cusop.op_type,"reserveany")==0)
	{
		if( !check_data(cusop.start_date.year,cusop.start_date.month,cusop.start_date.day) )
		{
			printf("%s %s from %d.%d.%d for %ddays **Error:date error \n",name,cusop.op_type,
				cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
			return false;
		}
		vector<int> loca_vec;
		vector<int> ite_vec;
		int j=0;
		for(int i=0;i<roomnum&&j<cusop.room_num;i++)
		{
			int ite;
			if( rooms[i].can_reserved(cusop.start_date,cusop.days,ite)  )
			{
				j++;
				loca_vec.push_back(i);
				ite_vec.push_back(ite);
			}
		}

		if(j==cusop.room_num)
		{
			for(int i=0;i<j;i++)
			{
				rooms[loca_vec.at(i)].addreserve( cusop.start_date,cusop.days,name,ite_vec.at(i) );
			}
			printf("%s reserveany %drooms(NO.%d) from %d.%d.%d for %ddays **Success \n",name,cusop.room_num,
				cusop.room_NO,cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
			return true;
		}
		else
		{
			printf("%s reserveany %drooms(NO.%d) from %d.%d.%d for %ddays **Error:no match rooms \n",name,cusop.room_num,
				cusop.room_NO,cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
			return false;
		}
		
	}
	else if(strcmp(cusop.op_type,"cancelany")==0)
	{
		if( !check_data(cusop.start_date.year,cusop.start_date.month,cusop.start_date.day) )
		{
			printf("%s %s from %d.%d.%d for %ddays **Error:date error \n",name,cusop.op_type,
				cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
			return false;
		}
		vector<int> loca_vec;
		vector<int> ite_vec;
		int j=0;
		for(int i=0;i<roomnum&&j<cusop.room_num;i++)
		{
			int ite;
			if( rooms[i].search(cusop.start_date,cusop.days,name,ite) )
			{
				j++;
				loca_vec.push_back(i);
				ite_vec.push_back(ite);
			}
		}

		if(j<=cusop.room_num)
		{
			for(int i=0;i<j;i++)
			{
				rooms[loca_vec.at(i)].delreserve(ite_vec.at(i));
			}
			printf("%s cancelany %drooms(NO.%d) from %d.%d.%d for %ddays **Success \n",name,cusop.room_num,
				cusop.room_NO,cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
			return true;
		}
		else
		{
			printf("%s cancelany %drooms(NO.%d) from %d.%d.%d for %ddays **Error:no match reserve imformation \n",name,cusop.room_num,
				cusop.room_NO,cusop.start_date.year,cusop.start_date.month,cusop.start_date.day,cusop.days);
			return false;
		}
	}
	else if(strcmp(cusop.op_type,"check")==0)
	{
		bool flag=false;
		for(int i=0;i<roomnum;i++)
		{
			if( rooms[i].search_by_name(name) )
				flag=true;
		}

		if(!flag)
		{
			printf("%s check result: no reserve imformation\n",name);
			return false;
		}
		return true;
	}
	else
	{
		printf("%s [%s] Command Error!\n",name,cusop.op_type);
			return false;
	}
}

///////////////////////////////////////////////////////

void read_hotel_msg(FILE *fp,class hotel *myhotel)
{
	fscanf(fp,"%d",&(myhotel->roomnum));
	for(int i=0;i<myhotel->roomnum;i++)
	{
		fscanf(fp,"%d",&(myhotel->rooms[i].room_NO));
		myhotel->rooms[i].res_num=0;
	}
}

void read_cust_msg(FILE *fp,int &cust_num, vector<struct customer> &cus_vec)
{
	fscanf(fp,"%d",&cust_num);//顾客数量
	char buf[30];
	struct customer cust;
	for(int i=1;i<=cust_num;i++)
	{
		cust.op_num=0;
		fscanf(fp,"%s",buf);//读取customer标识符
		fscanf(fp,"%s",buf);
		while(strstr(buf,"end") == NULL)
		{
			strcpy(cust.cus_op[cust.op_num].op_type,buf);
			if(strcmp(buf,"reserve")==0 || strcmp(buf,"cancel")==0)
			{
				fscanf(fp,"%d %d %d %d %d %s %d",&(cust.cus_op[cust.op_num].room_NO),&(cust.cus_op[cust.op_num].start_date.year),&(cust.cus_op[cust.op_num].start_date.month),
					&(cust.cus_op[cust.op_num].start_date.day),&(cust.cus_op[cust.op_num].days),cust.name,&(cust.cus_op[cust.op_num].op_time));
			}
			else if(strcmp(buf,"reserveblock")==0 || strcmp(buf,"cancelblock")==0)
			{
				fscanf(fp,"%d %d %d %d %d %d %s %d",&cust.cus_op[cust.op_num].room_num,&cust.cus_op[cust.op_num].room_NO,&cust.cus_op[cust.op_num].start_date.year,&cust.cus_op[cust.op_num].start_date.month,
					&cust.cus_op[cust.op_num].start_date.day,&cust.cus_op[cust.op_num].days,cust.name,&cust.cus_op[cust.op_num].op_time);
			}
			else if(strcmp(buf,"reserveany")==0 || strcmp(buf,"cancelany")==0)
			{
				fscanf(fp,"%d %d %d %d %d %s %d",&cust.cus_op[cust.op_num].room_num,&cust.cus_op[cust.op_num].start_date.year,&cust.cus_op[cust.op_num].start_date.month,
					&cust.cus_op[cust.op_num].start_date.day,&cust.cus_op[cust.op_num].days,cust.name,&cust.cus_op[cust.op_num].op_time);
			}
			else if(strcmp(buf,"check")==0)
			{
				fscanf(fp,"%s %d",cust.name,&cust.cus_op[cust.op_num].op_time);
			}
			else
			{
				cust.cus_op[cust.op_num].op_time=100;
				char c;
				fscanf(fp,"%c",&c);
				while(c!='\n')
				{
					fscanf(fp,"%c",&c);
				}
			}
			
			cust.op_num++;
			fscanf(fp,"%s",buf);
		}
		cus_vec.push_back(cust);
	}
}

void hotel::print()
{
	printf("\n------------------hotel information----------------\n");
	for(int i=0;i<roomnum;i++)
	{
		printf("room NO.%d:---\n",rooms[i].room_NO);
		if(rooms[i].res_num==0){printf("free\n"); continue;}
		for(int j=0;j<rooms[i].res_num;j++)
		{
			printf("name:%s  start_date:%d.%d.%d  end_date:%d.%d.%d  days:%d\n",
				rooms[i].res_info[j].name,rooms[i].res_info[j].start_date.year,rooms[i].res_info[j].start_date.month,
				rooms[i].res_info[j].start_date.day,rooms[i].res_info[j].end_date.year,rooms[i].res_info[j].end_date.month,
				rooms[i].res_info[j].end_date.day,rooms[i].res_info[j].days);
		}
	}

}
