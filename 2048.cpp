//#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
#include<conio.h>
#include<windows.h>
using namespace std;
int mapp[6][6],score,maxscore;
bool mappp[6][6];//判断这个点是否已经合成过
int c;
const char WALL[]="---------------------";

bool over();
void GetMaxScore();
void UpdateMaxScore();
void OutPut();
void Set();
void mov(int,int);
void MoveNum(int,int,int,int,bool &);
void Save();
bool Read();

int main(){
	HWND hwnd=GetForegroundWindow();
	HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hOut, &cursorInfo);
	cursorInfo.bVisible=0;
	SetConsoleCursorInfo(hOut, &cursorInfo);
	//设置光标不可见
	SetConsoleTitle("2048");
	//设置控制台标题
	COORD size={24,13};
	SetConsoleScreenBufferSize(hOut, size);
	//设置控制台缓冲区大小
	SMALL_RECT rect={0,0,26,13};
	SetConsoleWindowInfo(hOut,1,&rect);
	//设置控制台大小
	srand(time(NULL));
	GetMaxScore();
	bool flag=0;
	mapp[0][0]=mapp[5][5]=-1;
	for(int i=1;i<=4;i++)mapp[i][0]=mapp[0][i]=mapp[i][5]=mapp[5][i]=-1;
	while(1){
		if(!flag) printf("Press any key to start");
			else printf("Press any key to restart");
		getch();
		if(!flag&&Read()){
			printf("\nLast game's record detected\nContinue with it?\n1.Yes 0.No");
			if(getch()=='1')goto Start;
		}
		for(int i=1;i<=4;i++)
			for(int j=1;j<=4;j++)mapp[i][j]=0;
		Set();
Start:
		score=0;
		int x=0,y=0;
		while(1){
			flag=1;
			OutPut();
			if(over()){
				printf("GAME OVER\n");
				UpdateMaxScore();
				printf("\n\n");
				break;
			}
			c=getch();
			if(c=='0'){
				printf("Are you sure to exit?\n1.Yes 0.No");
				if(getch()=='1'){
					Save();
					system("cls");
					break;
				}
				else{
					OutPut();
					c=getch();
				}
			}
			switch(c){
				case 224:switch(getch()){
					case 75:goto LEFT;
					case 80:goto DOWN;
					case 77:goto RIGHT;
					case 72:goto UP;
				}
				case 'a':case 'A':LEFT:x=0,y=1;mov(0,-1);break;
				case 's':case 'S':DOWN:x=-1,y=0;mov(1,0);break;
				case 'd':case 'D':RIGHT:x=0,y=-1;mov(0,1);break;
				case 'w':case 'W':UP:x=1,y=0;mov(-1,0);break;
				case '\b':ShowWindow(hwnd,SW_MINIMIZE);break;
				case 27:Save();return 0;
				default:printf("INVALID COMMAND"); Sleep(500);
			} 
			memset(mappp,0,sizeof(mappp));
		}
	}
	return 0;
}

void MoveNum(int i,int j,int x,int y,bool &flag){
	if(mapp[i][j]&&!mapp[i+x][j+y]){
		mapp[i+x][j+y]=mapp[i][j];
		mapp[i][j]=0;
		flag=1;
		MoveNum(i+x,j+y,x,y,flag);
	}
	else if(mapp[i][j]==mapp[i+x][j+y]&&mapp[i][j]&&!mappp[i][j]){
		mapp[i+x][j+y]+=mapp[i][j];
		score+=mapp[i][j];
		mapp[i][j]=0;
		flag=1;
		mappp[i][j]=1;
		return;
	}
	return;
}
void mov(int x,int y){
	bool flag=0;//判断移动是否有效
	if(x==-1||y==-1){
		for(int i=1;i<=4;i++)
			for(int j=1;j<=4;j++)
				MoveNum(i,j,x,y,flag);
	}
	else{
		for(int i=4;i>0;i--)
			for(int j=4;j>0;j--)
				MoveNum(i,j,x,y,flag);
	}
	//同样好tm简陋的移动方式
	if(flag) Set();
		else{
			printf("INVALID COMMAND");
			Sleep(500);
		}
} 
bool over(){
	for(int i=1;i<=4;i++)
	    for(int j=1;j<=4;j++)
	    	if(!mapp[i][j]||mapp[i+1][j]==mapp[i][j]||mapp[i-1][j]==mapp[i][j]||mapp[i][j+1]==mapp[i][j]||mapp[i][j-1]==mapp[i][j])
	      		return 0;
	return 1;
}
void GetMaxScore(){
	ifstream fin("maxscore");
	fin>>maxscore;
	fin.close();
}
void UpdateMaxScore(){
	if(score>maxscore){
		ofstream fout("maxscore");
		maxscore=score;
		fout<<maxscore;
		fout.close();
		cout<<"NEW SCORE!";
	}
	else cout<<"Max score:"<<maxscore;
}
void OutPut(){
	system("cls");
	for(int i=1;i<=4;i++){
		cout<<WALL<<endl;
		for(int j=1;j<=4;j++)
			if(!mapp[i][j])printf("|    ");
		    else if(mapp[i][j]<10)printf("| %d  ",mapp[i][j]);
	        else if(mapp[i][j]<100)printf("| %d ",mapp[i][j]);
	        else if(mapp[i][j]<1000)printf("|%d ",mapp[i][j]);
	        else printf("|%d",mapp[i][j]);
	    cout<<'|'<<endl;
	}//输出方法好tm简陋 
	cout<<WALL<<endl;
	cout<<"score:"<<score<<endl;
}
void Set(){
	int x=0,y=0;
	while(mapp[x][y])x=rand()%4+1,y=rand()%4+1;
	int k=rand()%5;
	mapp[x][y]=k<1?4:2;
	//20%概率生成4，80%概率生成2
}
void Save(){
	ofstream fout("lastgame");
	for(int i=1;i<=4;i++)
		for(int j=1;j<=4;j++)
			fout<<mapp[i][j]<<' ';
	fout.close();
}
bool Read(){
	ifstream fin("lastgame");
	if(!fin.is_open())return 0;
	for(int i=1;i<=4;i++)
		for(int j=1;j<=4;j++)
			fin>>mapp[i][j];
	fin.close();
	remove("lastgame");
	return 1;
}