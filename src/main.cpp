#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>

#define H true
#define W false

#define WIDTH 50
#define HEIGHT 50

#define LINE	4
#define WALL 	3
#define TERNEL 	2
#define ROOM	1
#define NONE 	0

int target =5;
int arr[HEIGHT][WIDTH];

void init(){
	for(int i = 0;i<HEIGHT;i++){
		for(int j = 0;j<WIDTH;j++){
			arr[i][j]=WALL;
		}
	}
	
	for(int i = 1;i<HEIGHT-1;i++){
		for(int j = 1;j<WIDTH-1;j++){
			arr[i][j]=NONE;
		}
	}
}

void print(){
	std::ofstream writeFile;
	std::string output="";
    writeFile.open("map.txt");   


    
	for(int i = 0;i<HEIGHT;i++){
		for(int j = 0;j<WIDTH;j++){
			switch(arr[i][j]){
				case NONE:
					output+=" *";
					break;
				case ROOM:
					output+="  ";
					break;
				case TERNEL:
					output+="  ";
					break;
				case WALL:
					output+=" ■";
					break;
				case LINE:
					output+=" +";
					break;
			}
		}
		output+='\n';
	}
	
	writeFile.write(output.c_str(), output.size());
    writeFile.close();   
}

void BSP(int x1,int y1,int x2, int y2, bool flag, int cnt){
	if(cnt==target){
		x1 += std::rand() % 3 +1;
		if(x1<1) x1=1;
		if(x1>WIDTH-1) x1=WIDTH-1;
		y1 += std::rand() % 3 + 1;
		if(y1<1) y1=1;
		if(y1>HEIGHT-1) y1=HEIGHT-1;
		x2 -= std::rand() % 3  + 1;
		if(x2<1) x2=1;
		if(x2>WIDTH-1) x2=WIDTH-1;
		y2 -= std::rand() % 3 + 1;
		if(y2<1) y2=1;
		if(y2>HEIGHT-1) y2=HEIGHT-1;
		for(int i=y1;i<y2;i++){
			for(int j=x1;j<x2;j++){
				arr[i][j]=ROOM;
			}
		}
		x1-=1; y1-=1; x2+=1; y2+=1;
		int n= rand()%3;
		if(n==0 || n==2){
			int p = std::rand() % (y2-y1) + y1;
			for(int i = x1;;i--){
				if(arr[p][i]== WALL) break;
				if(arr[p][i]== ROOM){
					for(int j=x1;j>=i;j--){
						arr[p][j]=ROOM;
					}
					break;
				}
			}
			for(int i = x1;;i++){
				if(arr[p][i]== WALL) break;
				if(arr[p][i]== ROOM){
					for(int j=x1;j<=i;j++){
						arr[p][j]=TERNEL;
					}
					break;
				}
			}
		}
		if(n==1 || n==2){
			int p = std::rand() % (x2-x1) + x1;
			for(int i = y1;;i--){
				if(arr[i][p]== WALL) break;
				if(arr[i][p]== ROOM){
					for(int j=y1;j>=i;j--){
						arr[j][p]=TERNEL;
					}
					break;
				}
			}
			for(int i = y1;;i++){
				if(arr[i][p]== WALL) break;
				if(arr[i][p]== ROOM){
					for(int j=y1;j<=i;j++){
						arr[j][p]=TERNEL;
					}
					break;
				}
			}
		}
		
		return;

	}
	if(flag == H){
		int t = (x2+x1)/2 + (std::rand() % 4 - 2);
		for(int i=y1;i<=y2;i++){
			arr[i][t]=LINE;
		}
		BSP(x1,y1,t,y2,W,cnt+1);
		BSP(t,y1,x2,y2,W,cnt+1);
	}
	else if(flag == W){
		int t = (y2+y1)/2 + (std::rand() % 4 - 2);
		for(int i=x1;i<=x2;i++){
			arr[t][i]=LINE;
		}
		BSP(x1,y1,x2,t,H,cnt+1);
		BSP(x1,t,x2,y2,H,cnt+1);
	}
	
	return;
}
	
int main(int argc, char* argv[]) {
	std::srand(std::time(nullptr));
	init();
	
	BSP(1,1,WIDTH-1,HEIGHT-1,H,1);
	
	print();
	
	return 0;
}