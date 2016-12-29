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

void OutputMap();     							//У԰��ͼ
void OutputPlace();   							//������������
void OutputMenu();     							//�����ļ��ھ�����Ϣ�ı仯��̬������������ 
void ReadInfo();     							//���ļ��ж�ȡ��Ϣ������ͼ
void SearchPlace();     						//��ѯ������Ϣ
void SearchPath();     							//��ѯ���·��  
void ShortPath(int i);     						//�������·��(Ȩֵ��С)
void OutputShortpath(int sight1, int sight2);	//�������
void MiniPath();     							//��ѯ��̼�·��
void DFS1();     								//������ȱ����������еļ�·������·�������ļ� 
void Allpath();     							//��ѯ��������֮������м�·��
void DFS();     								//������ȱ����������м�·������� 
void MiniSpanTree();     						//��Ѳ�������,����ͼ����С������
void Prim(int start);     						//Prim�㷨���ͼ����С������
void AddPlace();     							//���Ӿ��㲢�����˾����йص�·�� 
void DeletePlace();     						//ɾ������ 
void AddPath();     							//������·��
void DeletePath();     							//������·��
void ManageSystem();     						//У԰���ι���ϵͳ 
void password();     							//����Ա��½
void ChangePassword();     						//����Ա�޸ĵ�½���벢�������ļ���


typedef struct ArcNode
{
	int length;          						//·������,��·����Ȩֵ
}ArcNode, *ArcLink;     						//�߽��Ķ���

typedef struct VertexNode
{
	int number;     							//����ı��
	char name[100];     						//���������
	char info[1000];     						//����ļ��
}VertexNode;     								//������Ķ���

typedef struct Graph
{
	VertexNode vertex[NUM];
	ArcNode arc[NUM][NUM];
	int vexnum;     							//ͼ�Ķ�����
	int arcnum;     							//ͼ�ı���  
}Graph;    										//ͼ�Ķ��� 

Graph G;
int path1[NUM][NUM];
int path2[NUM] = {0}; 
int Dist[NUM];     								//�洢���·����Ȩֵ(������)
int D[NUM];
int total = 0, total1 = 0;     					//����������ȱ���������¼�����������м�·�������� 
int visited[NUM];
int visit[NUM];
FILE *fp = NULL;

//****             ������Ϣ���������                  ****// 
//*********************************************************//
void OutputMap1()   					 			//���ļ� Map.txt�䵽��ʾ���� 
{
	char ch;
	fp = fopen("Map.txt","r");
	if(fp == NULL)
		printf("���ļ� Map.txt ʧ��.\n");
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
	loadimage(&img,"���ʵ�ͼ.jpg");
	putimage(0, 0, &img);
	getch();
	closegraph();
}

void OutputPlace()     							//���У԰��������
{
	printf("\n");
	printf("\t\t|-------------------------------------------------------|\n");
	printf("\t\t|                    1.����                             |\n");
	printf("\t\t|                    2.����¥                           |\n");
	printf("\t\t|                    3.��ѧ¥                           |\n");
	printf("\t\t|                    4.ˮ�����                         |\n");
	printf("\t\t|                    5.ʵ��¥                           |\n");
	printf("\t\t|                    6.��ѧ�������                   |\n");
	printf("\t\t|                    7.ͼ���                           |\n");
	printf("\t\t|                    8.��������                         |\n");
	printf("\t\t|                    9.������                           |\n");
	printf("\t\t|                    10.������                          |\n");
	printf("\t\t|                    11.С�ٳ�                          |\n");
	printf("\t\t|                    12.����                          |\n");
	printf("\t\t|                    13.��ʳ�㳡                        |\n");
	printf("\t\t|                    14.����Է                          |\n");
	printf("\t\t|-------------------------------------------------------|\n");

}

void OutputMenu()     //����ɾ�������������ļ������о����Ա��û�ֱ����� 
{
	int i;
	ReadInfo();	
	printf("\t\t|---------------------------------------|\n");
	printf("\t\t|            ��ӭ���������ʵ�           |\n");
	printf("\t\t|---------------------------------------|\n");
	for(i = 1; i <= G.vexnum; i++)
	{
		printf("\t\t|\t  %d. %s\t\t\t|\n", G.vertex[i].number, G.vertex[i].name);
	}
	printf("\t\t|---------------------------------------|\n");
}

void ReadInfo()     //������ѧУ��������ļ���ȡ������
{
	int i,j,num;
	for(i = 0; i < NUM; ++i)     //��ʼ���ڽӾ���
		for(j = 0; j < NUM; ++j)
			G.arc[i][j].length = INFINITY;
	fp = fopen("information.txt","r");
	if(fp == NULL)
		printf("\n\t\t\t���ļ� information.txtʧ��.\n");
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
		printf("\n\t\t\t���ļ� road.txtʧ��.\n");
	else
	{
		for(i = 1; i <= G.vexnum; i++)
			for(j = 1; j <= G.vexnum; j++)
				fscanf(fp, "%d", &G.arc[i][j].length);
	}
	fclose(fp);
}

//*********************************************************//
//****                ��ѯ������Ϣ                     ****//
//*********************************************************//
void SearchPlace()									//��ѯ������Ϣ  
{
	int i,num;
	char c = 'y';
	while(c == 'y')
	{
		system("cls");
		OutputMenu();
		printf("\n\n\t\t\t��������Ҫ���ҵľ����ţ�");
		scanf("%d", &num);
		getchar();
		system("cls");
		if(num > 0 && num <= G.vexnum)   			//�ж���Ϣ������ȷ 
		{
			for(i = 1; i <= G.vexnum; i++)
				if(num == G.vertex[i].number)
				{
					printf("\n\t\t������: %d\n", i);
					printf("\n\t\t��������: %s\n", G.vertex[i].name);
					printf("\n\t\t������: %s\n", G.vertex[i].info);
				}
		}
		else
			printf("\n\t\t��Ϣ��������!\n");
		num = 0;
		printf("\n\t\t�Ƿ������ѯ������Ϣ(y/n): ");
		c = getchar();
		getchar();
	}
	system("cls");  
}

//*********************************************************//
//****    �����м�·�����ҳ���ת�������ٵ�·��       ****//
//********************************************************* //
int R[3000] = {0};    								 //��¼ÿ����·���ľ������

void DFS1(int start, int end, int k)     			 //����������ת�������ٵļ�·��
{
	int s;
	int t = 0; 
 	fp = fopen("path.txt", "a+");     				//��������ȱ�����������������м�·���ҳ����洢Ϊ path.txt�ļ��� 
	if(D[k] == end && k <= G.vexnum )
	{
		for(s = 1; s < k; s++)     					//����·�������ļ��� 
		{
			t++;
			fprintf(fp, "%s ", G.vertex[D[s]].name);
		}
		fprintf(fp, "%s\n", G.vertex[D[s]].name);
		t++;     									//ÿ����һ������,t++ 
		total1++;     								//��¼��·��������
		R[total1] = t;     							//ÿ��·���ľ������
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
//****         ������·�����ҳ���ѷ���·��            ****//
//*********************************************************//
void SearchPath()     						//��ѯ���·��
{
	int i, j;
	char c = 'y';
	while(c == 'y')
	{
		system("cls");
		OutputMenu();
		printf("\n\n\t\t\t��ʼ������: ");
		scanf("%d", &i);
		printf("\n\t\t\t���վ�����: ");
		scanf("%d", &j);
		getchar();
		if(i > G.vexnum || i <= 0 || j > G.vexnum || j < 0 || i == j)
			printf("\n\t\t\t������Ϣ����!\n\n");
		else
		{
			ShortPath(i);     				//��Dijkstra�㷨������·��
			OutputShortpath(i, j);   		//�����·���ϵľ����������
		}
		printf("\n\t\t\t�Ƿ������ѯ���·��(y/n): ");
		c = getchar();
		getchar();
	}
	system("cls");
}

void ShortPath(int num)    					//Dijkstra�㷨���·��
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

void OutputShortpath(int sight1,int sight2)    // �������
{
	int a, b, c, d, q = 0;
	a = sight2;
	if(a != sight1)
	{
		printf("\n\t\t\t�� %s �� %s �����·����:\n\n\t\t\t", G.vertex[sight1].name, G.vertex[sight2].name);//�����ʾ��Ϣ
																											//���sight1��sight2�����·�����ȣ������Dist[]������
		printf("%s", G.vertex[sight1].name);  																//�������һ������
		d = sight1;      																					//������һ�ı�Ÿ�ֵ��d
		for(c = 0; c < NUM; ++c)
		{
			gate:;
			path1[a][sight1] = 0;
			for(b = 0; b < NUM; b++)
			{
				if(G.arc[d][b].length < INFINITY && path1[a][b]) 											//�������һ������һ���ٽ��֮�����·�������·��
				{
					printf("-->%s", G.vertex[b].name);  													//����˽ڵ������
					q = q + 1;    																			//����������һ����8�������ʱ�Ļ���
					path1[a][b] = 0;
					d = b;     																				//��b��Ϊ�����������һ��ѭ����� 
					if(q % 14 == 0)
						printf("\n");
					goto gate;
				}
			}
	 	}
		printf("\n\n\t\t\t��̾���Ϊ %dm.\n\n\t", Dist[a]);
	}
}

//*********************************************************//
//****            �������������м�·��             ****//
//*********************************************************//
void DFS(int start, int end, int k)      								//������ȱ����ҳ�������������м�·��
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

void Allpath()     									//�������������м�·��
{
	int i;
	int num1, num2;
	int start, end;
	char c = 'y';
	while(c == 'y')
	{
		OutputMenu();
		printf("\n\t\t\t��ʼ������: ");
		scanf("%d", &num1);
		if(num1 > 0 && num1 <= G.vexnum)  			 //�ж���Ϣ������ȷ
		{
			for(i = 1; i <= G.vexnum; i++)
				if(num1 == G.vertex[i].number)
					start = num1;     				//�ҵ������������ͬ�ľ���,����Ÿ���start
		}
		printf("\n\t\t\t���վ�����: ");
		scanf("%d" ,&num2);
		if(num2 > 0 && num2 <= G.vexnum)   			//�ж���Ϣ������ȷ
		{
			for(i = 1; i <= G.vexnum; i++)
				if(num2 == G.vertex[i].number)
					end = num2;     				//�ҵ������������ͬ�ľ���,����Ÿ���end
		}
		if(num1 > G.vexnum || num1 <= 0 || num2 > G.vexnum || num2 < 0 || num1 == num2)
			printf("\n\t\t\t������Ϣ����!\n\n");
		else
		{
			D[0] = start;     					//�洢��ʼ����
			for(i = 0; i < G.vexnum; i++)
				visited[i] = 0;     			//��ʼ������
			visited[start] = 1;     			//��ʼ�����ѷ���
			DFS(start, end, 0);
		}
		printf("��%s��%sһ����%d����·��\n", G.vertex[start].name, G.vertex[end].name, total);
		total = 0;
		printf("\n\t\t\t�Ƿ��������·��(y/n): ");
		getchar();
		c = getchar();
		system("cls");
	}
}

//*********************************************************//
//****                 ��ɾ�����·��                  ****//
//*********************************************************//
void AddPlace()     //�����µľ���,���ļ��ж�֮ǰ���ļ�����ȫ�渲��
{
	int x, i, j, length;
	int sight;
	char c = 'y', ch = 'y';
	while(c == 'y')
	{
		OutputMenu();
		printf("\n\t\t\t����������������: ");
		scanf("%d", &x);
		G.vertex[G.vexnum+1].number = x;
		printf("\n\t\t\t������������������: ");
		scanf("%s", G.vertex[x].name);
		printf("\n\t\t\t����������������: ");
		scanf("%s", G.vertex[x].info);
		G.vexnum++;
		fp = fopen("information.txt", "wt");
		if(fp == NULL)
			printf("\n\t\t\t���ļ� information.txtʧ��.\n");
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
				printf("\n\t\t\t���������������������ľ���: ");
				scanf("%d", &sight);
				printf("\n\t\t\t������·���ĳ���: ");
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
				printf("\n\t\t\t�Ƿ�������·��(y/n): ");
				getchar();
				ch = getchar();
				printf("\n\t\t\t�Ƿ�������Ӿ���(y/n): ");
				getchar();
				c = getchar();
			}
		}
	}
	system("cls");
	ReadInfo();
}

void DeletePlace()     //ɾ���ɵľ���,���ļ��ж�֮ǰ���ļ�����ȫ�渲��
{
	int x, i, j, k = 0;
	char c = 'y';
	
	while(c == 'y')
	{
		OutputMenu();
		printf("\n\t\t\t������ɾ������ı��: ");
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
		printf("\n\t\t\t�Ƿ����ɾ������(y/n): ");
		getchar();
		c = getchar();
	}
	system("cls");
	ReadInfo();
}

void AddPath()     //������·��
{
	int i, j;
	int num1, num2;
	int start, end, length;
	char c = 'y';
	while(c == 'y')
	{
		OutputMenu();
		printf("\n\t\t\t����������·�ߵĳ�ʼ������: ");
		scanf("%d", &num1);
		if(num1 > 0 && num1 <= G.vexnum)   //�ж���Ϣ������ȷ
		{
			for(i = 1; i <= G.vexnum; i++)
				if(num1 == G.vertex[i].number)
					start = num1;     //�ҵ������������ͬ�ľ���,����Ÿ���start
		}
		printf("\n\t\t\t����������·�ߵ����վ�����: ");
		scanf("%d", &num2);
		if(num2 > 0 && num2 <= G.vexnum)   //�ж���Ϣ������ȷ
		{
			for(i = 1; i <= G.vexnum; i++)
				if(num2 == G.vertex[i].number)
					end = num2;     //�ҵ������������ͬ�ľ���,����Ÿ���end
		}
		if(num1 > G.vexnum || num1 <= 0 || num2 > G.vexnum || num2 < 0 || num1 == num2)
			printf("\n\t\t\t������Ϣ����!\n\n");
		else
		{
			printf("\n\t\t\t��������������֮��ľ���(������): ");
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
		printf("\n\t\t\t�Ƿ��������·��(y/n): ");
		getchar();
		c = getchar();
	}
	system("cls");
	ReadInfo();
}

void DeletePath()    										 //������·��
{
	int i, j;
	int num1, num2;
	int start, end;
	char c = 'y';
	while(c == 'y')
	{
		OutputMenu();
		printf("\n\t\t\t�����볷��·�ߵĳ�ʼ������: ");
		scanf("%d", &num1);
		if(num1 > 0 && num1 <= G.vexnum)  					 //�ж���Ϣ������ȷ
		{
			for(i = 1; i <= G.vexnum; i++)
				if(num1 == G.vertex[i].number)
					start = num1;     						//�ҵ������������ͬ�ľ���,����Ÿ���start
		}
		printf("\n\t\t\t�����볷��·�ߵ����վ�����: ");
		scanf("%d", &num2);
		if(num2 > 0 && num2 <= G.vexnum)   					//�ж���Ϣ������ȷ
		{
			for(i = 1; i <= G.vexnum; i++)
				if(num2 == G.vertex[i].number)
					end = num2;     						//�ҵ������������ͬ�ľ���,����Ÿ���end
		}
		if(num1 > G.vexnum || num1 <= 0 || num2 > G.vexnum || num2 < 0 || num1 == num2)
			printf("\n\t\t\t������Ϣ����!\n\n");
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
		printf("\n\t\t\t�Ƿ��������·��(y/n): ");
		getchar();
		c = getchar();
	}
	system("cls");
	ReadInfo();
}


//*************�ṩͼ�����еص����Ѳ�������******************//       

void Prim(int start)
{
	struct
	{
		int adjvex;
		int lowcost;
	}closedge[NUM];
	int i, e, k, m, min;
	int sum = 0;     					//����·���ܳ���
	closedge[start].lowcost = 0;     	//��־����u�Ѿ����뵽U-����������

	for(i = 1; i <= G.vexnum; i++)
	{
		if(i != start)
		{
			closedge[i].adjvex = start;
			closedge[i].lowcost = G.arc[start][i].length;
		}
	}
	printf("\n%s", G.vertex[start].name);
	for(e = 1; e<= G.vexnum - 1; e++)     //����ѡ�е�n-1�����ϵı�
	{
										  //ѡ����СȨֵ�ı�
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
		closedge[m].lowcost = 0;     //��־����v0���뵽U-����������
		
		for(i = 1; i <= G.vexnum; i++)
			if(i != m && G.arc[m][i].length < closedge[i].lowcost)
			{
				closedge[i].lowcost = G.arc[m][i].length;
				closedge[i].adjvex = m;
			}
	}
	printf("\n\n\t\t\t��%s�������Ƽ�·���ܳ���:%dm\n", G.vertex[start].name, sum);
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
		printf("\n\n\t\t\t�������������: ");
		scanf("%d", &num);
		if(num > 0 && num <= G.vexnum)   //�ж���Ϣ������ȷ
		{
			for(i = 1; i <= G.vexnum; i++)
				if(num == G.vertex[i].number)
					start = num;     //�ҵ������������ͬ�ľ���,����Ÿ���start
			Prim(start);
		}
		else
			printf("\n\t\t\t��Ϣ��������!\n");
		num = 0; 
		printf("\n\t\t\t�Ƿ������ѯ�����Ƽ�·��(y/n): ");
	 	getchar();
		c = getchar();
	}
	system("cls");
}


//****               У԰���ι���ϵͳ                  ****//
//*********************************************************//
void ChangePassword()     //�޸ĵ�½����
{
	int password1, password2, newpass;
	char c = 'y'; 
	fp = fopen("password.txt", "wt");
	if(fp == NULL)
		printf("\n\t\t\t���ļ� password.txtʧ��.\n");
	else
	{
		while(c == 'y')
		{
			printf("\n\t\t\t������������: ");
			scanf("%d", &password1);
			printf("\n\t\t\t���ٴ�����������: ");
			scanf("%d", &password2);
			if(password1 == password2)
			{
				newpass = password1;
				fprintf(fp, "%d", newpass);
				printf("\n\t\t\t�޸�����ɹ�.\n");
				break;
			}
			else
			{
				printf("\n\t\t\t�����������벻��ͬ,�޸�����ʧ��.\n");
				printf("\n\t\t\t�Ƿ�����޸�����(y/n): ");
	 			getchar();
				c = getchar();
			}
		}
		fclose(fp);
		printf("\n\t\t\t����������ع���˵�\n");
		getch();
		system("cls"); 
		ManageSystem();
	}
}

void ManageSystem()     //����ԱȨ��
{
	int x;
	printf("\n\n\t\t   \n");
	printf("\n\n");
	printf("\t\t\t1. �����¾���       \n");
	printf("\t\t\t2. ɾ���ɾ���       \n");
	printf("\t\t\t3. ������·��       \n");
	printf("\t\t\t4. ������·��       \n");
	printf("\t\t\t5. �޸ĵ�½����     \n");
	printf("\t\t\t0. �˳�����ϵͳ     \n");
	printf("\n\t\t\t��ѡ������Ҫ�Ĳ���(0-5): ");
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
			 printf("\t\t\t������Ϣ����,����������!\n");
			 break;
	}
}

void password()    //����Ա�����֤
{
	int t;
	int password, pass;
	fp = fopen("password.txt","r");
	if(fp == NULL)
		printf("\n\t\t\t���ļ� password.txtʧ��.\n");
	else
	{
		fscanf(fp, "%d", &pass);
		printf("\n\t\t\t����������: ");
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
				printf("\n\t\t\t����������ٴ�����: ");
				scanf("%d", &password);
			}
		}
	}
	fclose(fp);
	if(password != pass)
	{
		printf("\n\t\t\t�Բ���,���޷���½����ϵͳ.\n\n\t\t\t�밴���������������...\n");
		getch();
		system("cls");
	}
}

//****                   ������                        ****//
//*********************************************************//
void main()
{
	int x;
	system("mode con: cols=140 lines=130");
	system("COLOR 2f");
	ReadInfo();     //���ļ��ж�ȡ��Ӧ����Ϣ������ͼ
	system("cls"); 

	printf("\n\n\t\t    ��ӭʹ�������ʵ��ѧУ԰����ϵͳ\n");
	while(1)
	{
		printf("\n\n");
		printf("\t\t\t1. У԰ȫ�����     \n");
		printf("\t\t\t2. ������Ϣ��ѯ     \n");
		printf("\t\t\t3. ���·����ѯ     \n");
	//	printf("\t\t\t4. �Ƽ��ι�·��     \n");
		printf("\t\t\t4. ���м�·��     \n");
	    printf("\t\t\t5. ��ɾ�����·��   \n");
		printf("\t\t\t0. �˳�ϵͳ         \n");
		printf("\n\t\t\t��ѡ������Ҫ�Ĳ���(0-7): ");
		scanf("%d", &x);
		switch(x)
		{
			case 1: system("cls");
					printf("\n\n\t\t�ص���: \n");
					OutputMap();
					OutputPlace(); 
				    printf("�밴������������˵�...");
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
					 printf("\n\t\t\t������Ϣ����,����������!\n");
					 break;
		}
    }
}
