#ifndef _HOTEL_H
#define _HOTEL_H

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include <vector>
#include <list>
#include <stdbool.h>
#include <string.h>
using namespace std;
#define MAX_OP 20 //最多操作个数
#define MAX_ROOM 9 //房间最大数目

#define RESERVE 10
#define CANCEL 11
#define RESERVEBLOCK 12
#define CANCELBLOCK 13
#define RESERVEANY 14
#define CANCELANY 15
#define CHECK 16
class date//日期类
{
public:
	int year;
	int month;
	int day;

	date(){}

	date(int y,int m,int d)
	{
		year=y;
		month=m;
		day=d;
	}

	void add(int n)//计算当前日期加上n天后的日期
	{
	   day=day+n-1;
	   while(1){	
		if(month==2){
			if(day>28){
				day=day-28;
				month++;
			}
			else
				break;
		}
		else if(month==4||month==6||month==9||month==11){
			if(day>30){
				day=day-30;
				month++;
			}
			else
				break;
		}
		else if(month==12){
			if(day>30){
				day=day-30;
				year++;
				month=1;
			}
		}
		else{
			if(day>31){
				day=day-31;
				month++;
			}
			else
				break;
		}
	   }//while
	}

	int compareTo(class date D)//日期比较
	{
		if(year>D.year)
			return 1;
		else if(year<D.year)
			return -1;
		else{
			if(month>D.month)
				return 1;
			else if(month<D.month)
				return -1;
			else
			{
				if(day>D.day)
					return 1;
				else if(day<D.day)
					return -1;
				else
					return 0;
			}
		}	
	}
};

struct op//客户操作结构体
{
	char op_type[20];  //操作类型 
	int op_time;
	int room_NO; //房间号或者第一个房间号
	int room_num;//房间数目
	class date start_date;//预约起始日期
	int days;     //天数
};

struct customer//客户类
{
	char name[50];
	int op_num;
	struct op cus_op[MAX_OP];//操作集合
};

struct reserved_info//房间预订信息结构体
{
	class date start_date ;	        //预约的起始时间
	class date end_date ;	        //预约的结束时间
	int days ;		//预约天数
    char  name[10] ;		//预约者 姓名
};

void copy_res(struct reserved_info &a,struct reserved_info b); //复制订房信息


class room//房间类
{
public:
	int room_NO ;	//房间号	
	int res_num;//预约信息数量
	struct reserved_info res_info[3]; //房间被预约信息数组
	room() ;
	bool can_reserved(class date start_date,int days,int &loca); //检查房间在请求时间内是否被预订
	bool addreserve(class date start_date,int days,char name[50],int loca);//新增预订信息
	bool delreserve(int loca); //删除预订信息 
	bool search(class date start_date,int days,char name[50],int &loca);//查找订房信息
	bool search_by_name(char name[50]);//按名字查询订房信息并显示
} ;

class hotel//宾馆类
{
public:
	class room rooms[MAX_ROOM] ;
	int roomnum ;
	hotel() ;
	bool init_hotel(vector<int>&vec) ;//初始化宾馆
	int location_room ( int room_NO ) ; //依据房间号找出房间在数组中的位置，没有返回-1
	bool do_request (char name[50], struct op cusop ) ;//宾馆处理客户请求
	void print();//显示宾馆房间的分配信息
} ;

bool check_data(int month,int day);// 检查日期的合法性

void read_hotel_msg(FILE *fp,class hotel *myhotel);//从文件读取hotel信息

void read_cust_msg(FILE *fp,int &cust_num, vector<struct customer> &cus_vec);//从文件读取customer信息
#endif
