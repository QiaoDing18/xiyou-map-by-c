#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>

#define INFINITY 32768
#define NUM 30 
#define FALSE 0
#define TURE 1

void OutputMap();     							//校园地图
void OutputPlace();   							//输出景点概览表
void OutputMenu();     							//根据文件内景点信息的变化动态输出景点概览表 
void ReadInfo();     							//从文件中读取信息并创建图
void SearchPlace();     						//查询景点信息
void SearchPath();     							//查询最短路径  
void ShortPath(int i);     						//计算最短路径(权值最小)
void OutputShortpath(int sight1, int sight2);	//输出函数
void MiniPath();     							//查询最短简单路径
void DFS1();     								//深度优先遍历搜索所有的简单路径并将路径存于文件 
void Allpath();     							//查询两个顶点之间的所有简单路径
void DFS();     								//深度优先遍历搜索所有简单路径并输出 
void MiniSpanTree();     						//最佳布网方案,即求图的最小生成树
void Prim(int start);     						//Prim算法求出图的最小生成树
void AddPlace();     							//增加景点并添加与此景点有关的路线 
void DeletePlace();     						//删除景点 
void AddPath();     							//增加新路线
void DeletePath();     							//撤销旧路线
void ManageSystem();     						//校园导游管理系统 
void password();     							//管理员登陆
void ChangePassword();     						//管理员修改登陆密码并保存于文件中


typedef struct ArcNode
{
	int length;          						//路径长度,即路径的权值
}ArcNode, *ArcLink;     						//边结点的定义

typedef struct VertexNode
{
	int number;     							//景点的编号
	char name[100];     						//景点的名称
	char info[1000];     						//景点的简介
}VertexNode;     								//顶点结点的定义

typedef struct Graph
{
	VertexNode vertex[NUM];
	ArcNode arc[NUM][NUM];
	int vexnum;     							//图的顶点数
	int arcnum;     							//图的边数  
}Graph;    										//图的定义 

Graph G;
int path1[NUM][NUM];
int path2[NUM] = {0}; 
int Dist[NUM];     								//存储最短路径的权值(公里数)
int D[NUM];
int total = 0, total1 = 0;     					//两次深度优先遍历用来记录搜索出的所有简单路径的数量 
int visited[NUM];
int visit[NUM];
FILE *fp = NULL;

//****             各类信息的输出函数                  ****// 
//*********************************************************//
void OutputMap1()   					 			//将文件 Map.txt输到显示屏中 
{
	char ch;
	fp = fopen("Map.txt","r");
	if(fp == NULL)
		printf("打开文件 Map.txt 失败.\n");
	else
	{ 
		ch = fgetc(fp);
		while(ch != '.')
		{
			putchar(ch);
			ch = fgetc(fp);
		}
	}
	fclose(fp); 
}

void OutputMap()
{
	initgraph(946, 884);
	IMAGE img;
	loadimage(&img,"西邮地图.jpg");
	putimage(0, 0, &img);
	getch();
	closegraph();
}

void OutputPlace()     							//输出校园景点名称
{
	printf("\n");
	printf("\t\t|-------------------------------------------------------|\n");
	printf("\t\t|                    1.正门                             |\n");
	printf("\t\t|                    2.行政楼                           |\n");
	printf("\t\t|                    3.教学楼                           |\n");
	printf("\t\t|                    4.水煮鸽子                         |\n");
	printf("\t\t|                    5.实验楼                           |\n");
	printf("\t\t|                    6.大学生活动中心                   |\n");
	printf("\t\t|                    7.图书馆                           |\n");
	printf("\t\t|                    8.西区澡堂                         |\n");
	printf("\t\t|                    9.体育馆                           |\n");
	printf("\t\t|                    10.体育场                          |\n");
	printf("\t\t|                    11.小操场                          |\n");
	printf("\t\t|                    12.篮球场                          |\n");
	printf("\t\t|                    13.美食广场                        |\n");
	printf("\t\t|                    14.旭日苑                          |\n");
	printf("\t\t|-------------------------------------------------------|\n");

}

void OutputMenu()     //当增删景点后用于输出文件中所有景点以便用户直观浏览 
{
	int i;
	ReadInfo();	
	printf("\t\t|---------------------------------------|\n");
	printf("\t\t|            欢迎光临西安邮电           |\n");
	printf("\t\t|---------------------------------------|\n");
	for(i = 1; i <= G.vexnum; i++)
	{
		printf("\t\t|\t  %d. %s\t\t\t|\n", G.vertex[i].number, G.vertex[i].name);
	}
	printf("\t\t|---------------------------------------|\n");
}

void ReadInfo()     //将存有学校各景点的文件读取进程序
{
	int i,j,num;
	for(i = 0; i < NUM; ++i)     //初始化邻接矩阵
		for(j = 0; j < NUM; ++j)
			G.arc[i][j].length = INFINITY;
	fp = fopen("information.txt","r");
	if(fp == NULL)
		printf("\n\t\t\t打开文件 information.txt失败.\n");
	else
	{
		fscanf(fp, "%d\n", &G.vexnum);
		for(j = 0; j < G.vexnum; j++)
		{	fscanf(fp, "%d ", &num);
			i = num;
			G.vertex[i].number = i;
			fscanf(fp, "%s ", G.vertex[i].name); 
			fscanf(fp, "%s\n", G.vertex[i].info);
		}
	}
	fclose(fp);
	fp = fopen("road.txt","r");
	if(fp == NULL)
		printf("\n\t\t\t打开文件 road.txt失败.\n");
	else
	{
		for(i = 1; i <= G.vexnum; i++)
			for(j = 1; j <= G.vexnum; j++)
				fscanf(fp, "%d", &G.arc[i][j].length);
	}
	fclose(fp);
}

//*********************************************************//
//****                查询景点信息                     ****//
//*********************************************************//
void SearchPlace()									//查询景点信息  
{
	int i,num;
	char c = 'y';
	while(c == 'y')
	{
		system("cls");
		OutputMenu();
		printf("\n\n\t\t\t请输入您要查找的景点编号：");
		scanf("%d", &num);
		getchar();
		system("cls");
		if(num > 0 && num <= G.vexnum)   			//判定信息输入正确 
		{
			for(i = 1; i <= G.vexnum; i++)
				if(num == G.vertex[i].number)
				{
					printf("\n\t\t景点编号: %d\n", i);
					printf("\n\t\t景点名称: %s\n", G.vertex[i].name);
					printf("\n\t\t景点简介: %s\n", G.vertex[i].info);
				}
		}
		else
			printf("\n\t\t信息输入有误!\n");
		num = 0;
		printf("\n\t\t是否继续查询景点信息(y/n): ");
		c = getchar();
		getchar();
	}
	system("cls");  
}

//*********************************************************//
//****    在所有简单路径中找出中转次数最少的路径       ****//
//********************************************************* //
int R[3000] = {0};    								 //记录每条简单路径的景点个数

void DFS1(int start, int end, int k)     			 //求解两点间中转次数最少的简单路径
{
	int s;
	int t = 0; 
 	fp = fopen("path.txt", "a+");     				//用深度优先遍历将两个顶点的所有简单路径找出并存储为 path.txt文件中 
	if(D[k] == end && k <= G.vexnum )
	{
		for(s = 1; s < k; s++)     					//将简单路径存入文件中 
		{
			t++;
			fprintf(fp, "%s ", G.vertex[D[s]].name);
		}
		fprintf(fp, "%s\n", G.vertex[D[s]].name);
		t++;     									//每存入一个景点,t++ 
		total1++;     								//记录简单路径的条数
		R[total1] = t;     							//每条路径的景点个数
	}
	else
	{
		s = 1;
		while(s <= G.vexnum)
		{
			if((G.arc [D[k]][s].length != INFINITY) && visited[s] == 0)
			{
				visited[s] = 1;
				D[k+1] = s;
				DFS1(start, end, k+1);
				visited[s] = 0;
			}
			s++;
		}
	}
	fclose(fp);
}


//*********************************************************//
//****         在所有路径中找出最佳访问路径            ****//
//*********************************************************//
void SearchPath()     						//查询最短路径
{
	int i, j;
	char c = 'y';
	while(c == 'y')
	{
		system("cls");
		OutputMenu();
		printf("\n\n\t\t\t初始景点编号: ");
		scanf("%d", &i);
		printf("\n\t\t\t最终景点编号: ");
		scanf("%d", &j);
		getchar();
		if(i > G.vexnum || i <= 0 || j > G.vexnum || j < 0 || i == j)
			printf("\n\t\t\t输入信息错误!\n\n");
		else
		{
			ShortPath(i);     				//用Dijkstra算法求出最短路径
			OutputShortpath(i, j);   		//将最短路径上的景点依次输出
		}
		printf("\n\t\t\t是否继续查询最短路径(y/n): ");
		c = getchar();
		getchar();
	}
	system("cls");
}

void ShortPath(int num)    					//Dijkstra算法最短路径
{
	int v, w, i, t;
	int final[NUM];
	int min;     
	for(v = 0; v < NUM; v++)
	{
		final[v] = 0;     
		Dist[v] = G.arc[num][v].length;   
		for(w = 0; w < NUM; w++)
		path1[v][w] = 0;     
		if(Dist[v] < 20000)     
		{
			path1[v][num] = 1;
			path1[v][v] = 1;
		}
	}
	Dist[num] = 0;
	final[num]=1;
	
	for(i = 0; i < NUM; ++i)     
	{
		min = INFINITY;
		for(w = 0; w < NUM; ++w)
		if(!final[w])     
		{
			if(Dist[w]<min)     
			{
				v = w;
				min = Dist[w];
			}
			final[v] = 1;    
		}
		for(w = 0; w < NUM; ++w)     
			if(!final[w] && ((min + G.arc[v][w].length) < Dist[w]))
			{
				Dist[w] = min+G.arc[v][w].length;     
				for(t = 0; t < NUM; t++)
					path1[w][t] = path1[v][t];
				path1[w][w] = 1;
			}
	}
}

void OutputShortpath(int sight1,int sight2)    // 输出函数
{
	int a, b, c, d, q = 0;
	a = sight2;
	if(a != sight1)
	{
		printf("\n\t\t\t从 %s 到 %s 的最短路径是:\n\n\t\t\t", G.vertex[sight1].name, G.vertex[sight2].name);//输出提示信息
																											//输出sight1到sight2的最短路径长度，存放在Dist[]数组中
		printf("%s", G.vertex[sight1].name);  																//输出景点一的名称
		d = sight1;      																					//将景点一的编号赋值给d
		for(c = 0; c < NUM; ++c)
		{
			gate:;
			path1[a][sight1] = 0;
			for(b = 0; b < NUM; b++)
			{
				if(G.arc[d][b].length < INFINITY && path1[a][b]) 											//如果景点一和它的一个临界点之间存在路径且最短路径
				{
					printf("-->%s", G.vertex[b].name);  													//输出此节点的名称
					q = q + 1;    																			//计数变量加一，满8控制输出时的换行
					path1[a][b] = 0;
					d = b;     																				//将b作为出发点进行下一次循环输出 
					if(q % 14 == 0)
						printf("\n");
					goto gate;
				}
			}
	 	}
		printf("\n\n\t\t\t最短距离为 %dm.\n\n\t", Dist[a]);
	}
}

//*********************************************************//
//****            两个顶点间的所有简单路径             ****//
//*********************************************************//
void DFS(int start, int end, int k)      								//深度优先遍历找出两个顶点的所有简单路径
{
	int s;
	if(D[k] == end && k <= G.vexnum)
	{
		for(s = 0; s < k; s++)
			printf("%s-->", G.vertex[D[s]].name);
		printf("%s\n\n", G.vertex[D[s]].name);
		total++;
	}
	else
	{
		s = 0;
		while(s < G.vexnum)
		{
			if((G.arc[D[k]][s].length < INFINITY) && visited[s] ==0)
			{
				visited[s] = 1;
				D[k+1] = s;
				DFS(start, end, k+1);
				visited[s] = 0;
			}
			s++;
		}
	}
}

void Allpath()     									//两个顶点间的所有简单路径
{
	int i;
	int num1, num2;
	int start, end;
	char c = 'y';
	while(c == 'y')
	{
		OutputMenu();
		printf("\n\t\t\t初始景点编号: ");
		scanf("%d", &num1);
		if(num1 > 0 && num1 <= G.vexnum)  			 //判定信息输入正确
		{
			for(i = 1; i <= G.vexnum; i++)
				if(num1 == G.vertex[i].number)
					start = num1;     				//找到编号与输入相同的景点,将编号赋予start
		}
		printf("\n\t\t\t最终景点编号: ");
		scanf("%d" ,&num2);
		if(num2 > 0 && num2 <= G.vexnum)   			//判定信息输入正确
		{
			for(i = 1; i <= G.vexnum; i++)
				if(num2 == G.vertex[i].number)
					end = num2;     				//找到编号与输入相同的景点,将编号赋予end
		}
		if(num1 > G.vexnum || num1 <= 0 || num2 > G.vexnum || num2 < 0 || num1 == num2)
			printf("\n\t\t\t输入信息错误!\n\n");
		else
		{
			D[0] = start;     					//存储初始顶点
			for(i = 0; i < G.vexnum; i++)
				visited[i] = 0;     			//初始化数组
			visited[start] = 1;     			//起始顶点已访问
			DFS(start, end, 0);
		}
		printf("从%s到%s一共有%d条简单路径\n", G.vertex[start].name, G.vertex[end].name, total);
		total = 0;
		printf("\n\t\t\t是否继续增加路线(y/n): ");
		getchar();
		c = getchar();
		system("cls");
	}
}

//*********************************************************//
//****                 增删景点和路线                  ****//
//*********************************************************//
void AddPlace()     //增加新的景点,在文件中对之前的文件进行全面覆盖
{
	int x, i, j, length;
	int sight;
	char c = 'y', ch = 'y';
	while(c == 'y')
	{
		OutputMenu();
		printf("\n\t\t\t请输入新增景点编号: ");
		scanf("%d", &x);
		G.vertex[G.vexnum+1].number = x;
		printf("\n\t\t\t请输入新增景点名称: ");
		scanf("%s", G.vertex[x].name);
		printf("\n\t\t\t请输入新增景点简介: ");
		scanf("%s", G.vertex[x].info);
		G.vexnum++;
		fp = fopen("information.txt", "wt");
		if(fp == NULL)
			printf("\n\t\t\t打开文件 information.txt失败.\n");
		else
		{
			fprintf(fp, "%d\n", G.vexnum);
			for(i = 1; i <= G.vexnum; i++)
			{
				fprintf(fp, "%d %s %s\n", G.vertex[i].number, G.vertex[i].name, G.vertex[i].info);
			}
			fclose(fp);
			while(ch == 'y')
			{
				printf("\n\t\t\t请输入与新增景点相连的景点: ");
				scanf("%d", &sight);
				printf("\n\t\t\t请输入路径的长度: ");
				scanf("%d", &length);
				G.arc[x][sight].length = G.arc[sight][x].length = length;
				fp = fopen("road.txt", "wt");
				for(i = 1; i <= G.vexnum; i++)
				{
					for(j = 1; j < G.vexnum; j++)
					{
						fprintf(fp, "%d ", G.arc[i][j].length);
					}
					fprintf(fp, "%d", G.arc[i][j].length);
					fprintf(fp, "\n");
				}
				fclose(fp);
				printf("\n\t\t\t是否继续添加路线(y/n): ");
				getchar();
				ch = getchar();
				printf("\n\t\t\t是否继续增加景点(y/n): ");
				getchar();
				c = getchar();
			}
		}
	}
	system("cls");
	ReadInfo();
}

void DeletePlace()     //删除旧的景点,在文件中对之前的文件进行全面覆盖
{
	int x, i, j, k = 0;
	char c = 'y';
	
	while(c == 'y')
	{
		OutputMenu();
		printf("\n\t\t\t请输入删除景点的编号: ");
		scanf("%d", &x);
		for(i = x; i <= G.vexnum; i++)
		{
			strcpy(G.vertex[i].name, G.vertex[i+1].name);
			strcpy(G.vertex[i].info, G.vertex[i+1].info);
			if(G.vertex[i].number == 0)
				break;
		}
		G.vexnum--;
		for(i = 1; i <= G.vexnum; i++)
		{
			for(j = x; j <= G.vexnum; j++)
			{
				G.arc[i][j].length = G.arc[i][j+1].length;
				G.arc[j][i].length = G.arc[j+1][i].length;
			}
		}
		fp = fopen("information.txt", "wt");
		fprintf(fp, "%d\n", G.vexnum);
		for(i = 1; i <= G.vexnum; i++)
		{
			fprintf(fp, "%d %s %s\n", G.vertex[i].number, G.vertex[i].name, G.vertex[i].info);
		}
		fclose(fp);
		fp = fopen("road.txt", "wt");
		for(i = 1; i <= G.vexnum; i++)
		{
			for(j = 1; j < G.vexnum; j++)
			{
				fprintf(fp, "%d ", G.arc[i][j].length);
			}
			fprintf(fp, "%d", G.arc[i][j].length);
				fprintf(fp, "\n");
		}
		fclose(fp);
		printf("\n\t\t\t是否继续删除景点(y/n): ");
		getchar();
		c = getchar();
	}
	system("cls");
	ReadInfo();
}

void AddPath()     //增加新路线
{
	int i, j;
	int num1, num2;
	int start, end, length;
	char c = 'y';
	while(c == 'y')
	{
		OutputMenu();
		printf("\n\t\t\t请输入增加路线的初始景点编号: ");
		scanf("%d", &num1);
		if(num1 > 0 && num1 <= G.vexnum)   //判定信息输入正确
		{
			for(i = 1; i <= G.vexnum; i++)
				if(num1 == G.vertex[i].number)
					start = num1;     //找到编号与输入相同的景点,将编号赋予start
		}
		printf("\n\t\t\t请输入增加路线的最终景点编号: ");
		scanf("%d", &num2);
		if(num2 > 0 && num2 <= G.vexnum)   //判定信息输入正确
		{
			for(i = 1; i <= G.vexnum; i++)
				if(num2 == G.vertex[i].number)
					end = num2;     //找到编号与输入相同的景点,将编号赋予end
		}
		if(num1 > G.vexnum || num1 <= 0 || num2 > G.vexnum || num2 < 0 || num1 == num2)
			printf("\n\t\t\t输入信息错误!\n\n");
		else
		{
			printf("\n\t\t\t请输入两个景点之间的距离(公里数): ");
			scanf("%d", &length);

			G.arc[start][end].length = G.arc[end][start].length = length;
			fp = fopen("road.txt", "wt");
			for(i = 1; i <= G.vexnum; i++)
			{
				for(j = 1; j < G.vexnum; j++)
				{
					fprintf(fp, "%d ", G.arc[i][j].length);
				}
				fprintf(fp, "%d", G.arc[i][j].length);
				fprintf(fp, "\n");
			}
			fclose(fp);
		}
		printf("\n\t\t\t是否继续增加路线(y/n): ");
		getchar();
		c = getchar();
	}
	system("cls");
	ReadInfo();
}

void DeletePath()    										 //撤销旧路线
{
	int i, j;
	int num1, num2;
	int start, end;
	char c = 'y';
	while(c == 'y')
	{
		OutputMenu();
		printf("\n\t\t\t请输入撤销路线的初始景点编号: ");
		scanf("%d", &num1);
		if(num1 > 0 && num1 <= G.vexnum)  					 //判定信息输入正确
		{
			for(i = 1; i <= G.vexnum; i++)
				if(num1 == G.vertex[i].number)
					start = num1;     						//找到编号与输入相同的景点,将编号赋予start
		}
		printf("\n\t\t\t请输入撤销路线的最终景点编号: ");
		scanf("%d", &num2);
		if(num2 > 0 && num2 <= G.vexnum)   					//判定信息输入正确
		{
			for(i = 1; i <= G.vexnum; i++)
				if(num2 == G.vertex[i].number)
					end = num2;     						//找到编号与输入相同的景点,将编号赋予end
		}
		if(num1 > G.vexnum || num1 <= 0 || num2 > G.vexnum || num2 < 0 || num1 == num2)
			printf("\n\t\t\t输入信息错误!\n\n");
		else
		{
			G.arc[start][end].length = G.arc[end][start].length = INFINITY;
			fp = fopen("road.txt", "wt");
			for(i = 1; i <= G.vexnum; i++)
			{
				for(j = 1; j < G.vexnum; j++)
				{
					fprintf(fp, "%d ", G.arc[i][j].length);
				}
				fprintf(fp, "%d", G.arc[i][j].length);
				fprintf(fp, "\n");
			}
			fclose(fp);
		}
		printf("\n\t\t\t是否继续撤销路线(y/n): ");
		getchar();
		c = getchar();
	}
	system("cls");
	ReadInfo();
}


//*************提供图中所有地点的最佳布网方案******************//       

void Prim(int start)
{
	struct
	{
		int adjvex;
		int lowcost;
	}closedge[NUM];
	int i, e, k, m, min;
	int sum = 0;     					//计算路径总长度
	closedge[start].lowcost = 0;     	//标志顶点u已经加入到U-生成树集合

	for(i = 1; i <= G.vexnum; i++)
	{
		if(i != start)
		{
			closedge[i].adjvex = start;
			closedge[i].lowcost = G.arc[start][i].length;
		}
	}
	printf("\n%s", G.vertex[start].name);
	for(e = 1; e<= G.vexnum - 1; e++)     //控制选中的n-1条符合的边
	{
										  //选择最小权值的边
		min = INFINITY;
		for(k = 1;k <= G.vexnum; k++)
		{
			if(closedge[k].lowcost != 0 && closedge[k].lowcost < min)
			{
				m = k;
				min = closedge[k].lowcost;
			}
		}
		sum += closedge[m].lowcost;
		printf("-->%s", G.vertex[m].name);
		closedge[m].lowcost = 0;     //标志顶点v0加入到U-生成树集合
		
		for(i = 1; i <= G.vexnum; i++)
			if(i != m && G.arc[m][i].length < closedge[i].lowcost)
			{
				closedge[i].lowcost = G.arc[m][i].length;
				closedge[i].adjvex = m;
			}
	}
	printf("\n\n\t\t\t从%s出发的推荐路线总长是:%dm\n", G.vertex[start].name, sum);
}

void MiniSpanTree()						//////////
{
	int num; 
	int start, i;
	char c = 'y';
	while(c == 'y')
	{
		system("cls");
		OutputMenu();
		printf("\n\n\t\t\t请输入出发景点: ");
		scanf("%d", &num);
		if(num > 0 && num <= G.vexnum)   //判定信息输入正确
		{
			for(i = 1; i <= G.vexnum; i++)
				if(num == G.vertex[i].number)
					start = num;     //找到编号与输入相同的景点,将编号赋予start
			Prim(start);
		}
		else
			printf("\n\t\t\t信息输入有误!\n");
		num = 0; 
		printf("\n\t\t\t是否继续查询其他推荐路线(y/n): ");
	 	getchar();
		c = getchar();
	}
	system("cls");
}


//****               校园导游管理系统                  ****//
//*********************************************************//
void ChangePassword()     //修改登陆密码
{
	int password1, password2, newpass;
	char c = 'y'; 
	fp = fopen("password.txt", "wt");
	if(fp == NULL)
		printf("\n\t\t\t打开文件 password.txt失败.\n");
	else
	{
		while(c == 'y')
		{
			printf("\n\t\t\t请输入新密码: ");
			scanf("%d", &password1);
			printf("\n\t\t\t请再次输入新密码: ");
			scanf("%d", &password2);
			if(password1 == password2)
			{
				newpass = password1;
				fprintf(fp, "%d", newpass);
				printf("\n\t\t\t修改密码成功.\n");
				break;
			}
			else
			{
				printf("\n\t\t\t两次密码输入不相同,修改密码失败.\n");
				printf("\n\t\t\t是否继续修改密码(y/n): ");
	 			getchar();
				c = getchar();
			}
		}
		fclose(fp);
		printf("\n\t\t\t按任意键返回管理菜单\n");
		getch();
		system("cls"); 
		ManageSystem();
	}
}

void ManageSystem()     //管理员权限
{
	int x;
	printf("\n\n\t\t   \n");
	printf("\n\n");
	printf("\t\t\t1. 增加新景点       \n");
	printf("\t\t\t2. 删除旧景点       \n");
	printf("\t\t\t3. 增加新路线       \n");
	printf("\t\t\t4. 撤销旧路线       \n");
	printf("\t\t\t5. 修改登陆密码     \n");
	printf("\t\t\t0. 退出管理系统     \n");
	printf("\n\t\t\t请选择您需要的操作(0-5): ");
	scanf("%d", &x);
	switch(x)
	{
	case 1: system("cls");
			AddPlace();
			break;
	case 2: system("cls");
			DeletePlace();
			break;
	case 3: system("cls");
			AddPath();
			break;
	case 4: system("cls");
			DeletePath();
			break;
	case 5: ChangePassword();
			system("cls");
			break;
	case 0: printf("\n");
			system("cls");
			break;
	default: system("cls");
			 printf("\t\t\t输入信息错误,请重新输入!\n");
			 break;
	}
}

void password()    //管理员身份认证
{
	int t;
	int password, pass;
	fp = fopen("password.txt","r");
	if(fp == NULL)
		printf("\n\t\t\t打开文件 password.txt失败.\n");
	else
	{
		fscanf(fp, "%d", &pass);
		printf("\n\t\t\t请输入密码: ");
		scanf("%d", &password);
		for(t = 1; t < 3; t++)
		{
			if(password == pass)
			{
				system("cls");
				ManageSystem();
				break;
			}
			else
			{
				printf("\n\t\t\t密码错误，请再次输入: ");
				scanf("%d", &password);
			}
		}
	}
	fclose(fp);
	if(password != pass)
	{
		printf("\n\t\t\t对不起,您无法登陆管理系统.\n\n\t\t\t请按任意键返回主界面...\n");
		getch();
		system("cls");
	}
}

//****                   主函数                        ****//
//*********************************************************//
void main()
{
	int x;
	system("mode con: cols=140 lines=130");
	system("COLOR 2f");
	ReadInfo();     //从文件中读取相应的信息并创建图
	system("cls"); 

	printf("\n\n\t\t    欢迎使用西安邮电大学校园导游系统\n");
	while(1)
	{
		printf("\n\n");
		printf("\t\t\t1. 校园全景浏览     \n");
		printf("\t\t\t2. 景点信息查询     \n");
		printf("\t\t\t3. 最短路径查询     \n");
	//	printf("\t\t\t4. 推荐参观路线     \n");
		printf("\t\t\t4. 所有简单路径     \n");
	    printf("\t\t\t5. 增删景点和路线   \n");
		printf("\t\t\t0. 退出系统         \n");
		printf("\n\t\t\t请选择您需要的操作(0-7): ");
		scanf("%d", &x);
		switch(x)
		{
			case 1: system("cls");
					printf("\n\n\t\t地点编号: \n");
					OutputMap();
					OutputPlace(); 
				    printf("请按任意键返回主菜单...");
					getch();
				    system("cls"); 
					break;
			case 2: system("cls");
					SearchPlace();
					break;
			case 3: system("cls");
					SearchPath();
					break;
		//	case 4: system("cls");
		//			MiniSpanTree();
		//			break;
			case 4: system("cls"); 
					Allpath();
					break;
		    case 5: system("cls");
					password();
					break;
			case 0: printf("\n");
					exit(0);
			default: system("cls");
					 printf("\n\t\t\t输入信息错误,请重新输入!\n");
					 break;
		}
    }
}
