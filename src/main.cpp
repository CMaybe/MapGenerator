#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <algorithm>

#define H true
#define W false

#define WIDTH 50
#define HEIGHT 50

#define PRE_WALL	6
#define PRE_ROOM	5
#define LINE		4
#define WALL 		3
#define TERNEL 		2
#define ROOM		1
#define NONE 		0

typedef struct EDGE{
	int x1;
	int y1;
	int x2;
	int y2;
}Edge;

int arr[HEIGHT+2][WIDTH+2];

void init(int tile){
	for(int i = 0;i<HEIGHT+2;i++){
		for(int j = 0;j<WIDTH+2;j++){
			arr[i][j]=WALL;
		}
	}
	
	for(int i = 1;i<=HEIGHT;i++){
		for(int j = 1;j<=WIDTH;j++){
			arr[i][j]=tile;
		}
	}
}

void save(std::string name){
	std::ofstream writeFile;
	std::string output="";
    writeFile.open(name.c_str());   

    
	for(int i = 0;i<HEIGHT+2;i++){
		for(int j = 0;j<WIDTH+2;j++){
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
					output+=" .";
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

Edge BSP(int x1,int y1,int x2, int y2, bool flag, int cnt, int target){
	if(cnt==target){
		x1 += std::rand() % 3 +1;
		if(x1<1) x1=1;
		if(x1>WIDTH) x1=WIDTH;
		y1 += std::rand() % 3 + 1;
		if(y1<1) y1=1;
		if(y1>HEIGHT) y1=HEIGHT;
		x2 -= std::rand() % 3  + 1;
		if(x2<1) x2=1;
		if(x2>WIDTH) x2=WIDTH;
		y2 -= std::rand() % 3 + 1;
		if(y2<1) y2=1;
		if(y2>HEIGHT) y2=HEIGHT;
		for(int i=y1;i<y2;i++){
			for(int j=x1;j<x2;j++){
				arr[i][j]=ROOM;
			}
		}
		return Edge{x1,y1,x2-1,y2-1};
	}
	
	int t_x1 = 0;
	int t_x2 = 0;
	int t_y1 = 0;
	int t_y2 = 0;
	if(flag == H){
		int t = (x2+x1)/2 + (std::rand() % 4 - 2);
		for(int i=y1;i<=y2;i++){
			arr[i][t]=LINE;
		}
		Edge p1 = BSP(x1,y1,t,y2,W,cnt+1, target); // left
		Edge p2 = BSP(t,y1,x2,y2,W,cnt+1, target); // right
		
		t_x1 = p1.x2;
		t_x2 = p2.x1;
		t_y1 = std::max(p1.y1,p2.y1);
		t_y2 = std::min(p1.y2,p2.y2);
		int p = 0;
		do{
			p = std::rand() % (t_y2-t_y1) + t_y1;
			// break;
		}while(arr[p][t_x1]!=ROOM || arr[p][t_x2]!=ROOM);
		for(int i = t_x1;i<=t_x2;i++){
			if(arr[p][i]==TERNEL) break;
			arr[p][i]=TERNEL;
		}
		return Edge{p1.x1,std::max(p1.y1,p2.y1),p2.x2,std::min(p1.y2,p2.y2)};

		
	}
	else if(flag == W){
		int t = (y2+y1)/2 + (std::rand() % 4 - 2);
		for(int i=x1;i<=x2;i++){
			arr[t][i]=LINE;
		}
		Edge p1 = BSP(x1,y1,x2,t,H,cnt+1,target); // up
		Edge p2 = BSP(x1,t,x2,y2,H,cnt+1,target); // down
		
		
		t_x1 = std::max(p1.x1,p2.x1);
		t_x2 = std::min(p1.x2,p2.x2);
		t_y1 = p1.y2;
		t_y2 = p2.y1;
		int p = 0;
		do{
			p = std::rand() % (t_x2-t_x1) + t_x1;
		}while(arr[t_y1][p]!=ROOM || arr[t_y2][p]!=ROOM);
		
		for(int i = t_y1;i<=t_y2;i++){
			if(arr[i][p]==TERNEL) break;
			arr[i][p]=TERNEL;
		}
		return Edge{std::max(p1.x1,p2.x1),p1.y1,std::min(p1.x2,p2.x2),p2.y2};
	}
	return Edge{t_x1,t_y1,t_x2,t_y2};
}

void pre2Real(){
	for(int i = 0;i<HEIGHT+2;i++){
		for(int j = 0;j<WIDTH+2;j++){
			if(arr[i][j]==PRE_WALL) arr[i][j] = WALL;
			if(arr[i][j]==PRE_ROOM) arr[i][j] = ROOM;
		}
	}
}

void Cellular_Automata(const int wall_ratio,const int wall_effected,const int room_effected, int count){
	int wall_cnt = (WIDTH-2)*(HEIGHT-2)*wall_ratio/100;
	int cnt = 0;
	while(cnt++<wall_cnt){
		int x = std::rand() % (WIDTH-1) + 1;
		int y = std::rand() % (WIDTH-1) + 1;
		if(arr[y][x]!=WALL) arr[y][x]=WALL;
		else cnt--;
	}
	while(count--){
		for(int i = 1;i<=HEIGHT;i++){
			for(int j = 1;j<=WIDTH;j++){
				int wc = 0; // wall count;
				if(arr[i+1][j-1]==WALL) wc++;
				if(arr[i][j-1]==WALL) wc++;
				if(arr[i-1][j-1]==WALL) wc++;
				if(arr[i+1][j+1]==WALL) wc++;
				if(arr[i][j+1]==WALL) wc++;
				if(arr[i-1][j+1]==WALL) wc++;
				if(arr[i+1][j]==WALL) wc++;
				if(arr[i-1][j]==WALL) wc++;
				if(wc >= wall_effected){
					for(int k=i-1;k<=i+1;k++){
						for(int w=j-1;w<=j+1;w++){
							arr[k][w]=PRE_WALL;
						}
					}
				}
			}
		}
		for(int i = 2;i<HEIGHT;i++){
			for(int j = 2;j<WIDTH;j++){
				int rc = 0; // room count;
				if(arr[i+1][j-1]==ROOM) rc++;
				if(arr[i][j-1]==ROOM) rc++;
				if(arr[i-1][j-1]==ROOM) rc++;
				if(arr[i+1][j+1]==ROOM) rc++;
				if(arr[i][j+1]==ROOM) rc++;
				if(arr[i-1][j+1]==ROOM) rc++;
				if(arr[i+1][j]==ROOM) rc++;
				if(arr[i-1][j]==ROOM) rc++;
				if(rc >= room_effected){
					for(int k=i-1;k<=i+1;k++){
						for(int w=j-1;w<=j+1;w++){
							arr[k][w]=PRE_ROOM;
						}
					}
				}
			}
		}
		pre2Real();
	}
	
	return;
}
	
int main(int argc, char* argv[]) {
	std::srand(std::time(nullptr));
	
	init(NONE);
	BSP(1,1,WIDTH,HEIGHT,H,1,5);
	save("BSP.txt");

	init(ROOM);
	Cellular_Automata(20,4,7,7);
	save("Cellular_Automata.txt");
	
	
	return 0;
}