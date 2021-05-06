#include<iostream>
#include<queue>
#include<string>
#include<fstream>
#include<iomanip>
using namespace std;
#define INF 10000
const int MaxSize=20;
 
//辅助prim 
struct shortEdge{
	int edgeIndex;  //边的编号 
	int lowcost;    //最小权值 
};
 
template<class DataType>//由于顶点数据的类型不确定 
class Graph
{
private:
	int GType;//图的类型0：无向图  1：有向图
	DataType vertex[MaxSize];//定义数组，保存顶点数据
	int edge[MaxSize][MaxSize];//边
	int vertexNum, edgeNum;
	int isTrav[MaxSize];
	int visited[MaxSize];
 
public:	
	Graph(DataType a[], int n, int e);//构造函数，简历具有n个顶点，e条边的图
	~Graph(){ } //析构函数为空
	void resetVisted();
	void DFSTraverse(int v);//Depth—first traverse深度优先遍历
	void BFSTraverse(int v);//Breadth—first traverse广度优先遍历
	void Djstl(int begin);//最短路径 
	template<class T>
	friend void prim(Graph<T> G);//友元函数，用于访问类中的私有成员。最小生成树的prim算法 
};
 
//构造函数 
//邻接矩阵初始化为-1，表示没有路径
template<class DataType>
Graph<DataType>::Graph(DataType a[], int n, int e)
{
	vertexNum=n;
	edgeNum=e;
	for(int i=0;i<vertexNum;i++)  //边表信息 
		vertex[i]=a[i];
	for(int i=0;i<vertexNum;i++)  //邻接矩阵初始化为-1，表示没有路径， 
	{
		for(int j=0;j<vertexNum;j++)
		{
				edge[i][j]=-1;
		}
	
	} 
	int i,j,value;
	
	ifstream infile;
	infile.open("GraphData.txt");
	for(int k=0;k<e;k++)   //输入边的连接的两个顶点编号及权值 
	{
        //cin>>i>>j>>value;
        infile>>i;infile>>j;infile>>value;
        edge[i][j]=value;
      //  edge[j][i]=value; //需求书里说明是加权无向图，此处导致成为有向图 --BUG 詹泽生  
	}
//	infile.close();//正确代码，不关闭文件 --BUG 龚星 
	
	/*不设置到自身距离，影响最短路 BUG--詹泽生 
	for(int i=0;i<vertexNum;i++)  //
	{
		for(int j=0;j<vertexNum;j++)
		{
			if(i==j)
				edge[i][j]=0;
		}
	
	} 
	*/
	
	cout<<"邻接矩阵：\n";
	for(int x=0;x<vertexNum;x++)
	{
		for(int y=0;y<vertexNum;y++)
		    cout<<setw(10)<<edge[x][y]; 
        cout<<endl;
	} 
	resetVisted(); //重置访问表 
}
 
//重置访问数组，初始化及每次遍历前需要重置 
template<class DataType>
void Graph<DataType>::resetVisted()
{
	//修改循环 
	//for(int i=0;i<vertexNum;i++) 正确版本 
	for(int i=0;i<vertexNum-1;i++) //BUG --龚星 
		visited[i]=0;
}
 
//深度优先遍历 
template<class DataType>
void Graph<DataType>::DFSTraverse(int v)
{
    cout<<vertex[v]<<" ";
    visited[v]=1;
    for(int i=0;i<vertexNum;i++)
    	//if(edge[v][i]!=-1&&visited[i]==0) //正确代码 
    	if(visited[i]==0) //BUG--赵东旭 赵梓函 
    		DFSTraverse(i);
}
 
//广度优先遍历 
template<class DataType>
void Graph<DataType>::BFSTraverse(int v)
{
	queue<int> Q;//队列，用于广度遍历
    cout<<vertex[v]<<" ";
    visited[v]=1;
    Q.push(v);
    int tmpv;
    while(!Q.empty())
    {
        tmpv=Q.front();
        Q.pop();
        for(int i=0;i<vertexNum;i++)
            if(edge[tmpv][i]!=-1&&visited[i]==0)
            {
            	cout<<vertex[i]<<" ";
                Q.push(i);
                //张文泉改动，删除标记语句 
                //visited[i]=1; //正确代码 
            }
    }
    cout<<"\n";
}
 
template<class DataType>
void Graph<DataType>::Djstl(int begin)
{
	int i,j,k;
    int min;
    int tmp;
    int flag[MaxSize];      // flag[i]=1表示"顶点vs"到"顶点i"的最短路径已成功获取。
 	int dist[MaxSize];
 	int prev[MaxSize];
    // 初始化
    for (i = 0; i <vertexNum; i++)
    {
        flag[i] = 0;              // 顶点i的最短路径还没获取到。
        prev[i] = -1;              // 顶点i的前驱顶点为0。
        /*正确初始化 
		if(edge[begin][i]!=-1)
       {
       	dist[i]=edge[begin][i];
	   }
	   else
	   {
	   	dist[i]=INF;
	   }*/
        dist[i] = edge[begin][i];//编译测试-王文宇 
    }
 
    // 对"顶点vs"自身进行初始化
    flag[begin] = 1;
    dist[begin] = 0;
 
    // 遍历G.vexnum-1次；每次找出一个顶点的最短路径。
    for (i = 0; i<vertexNum; i++)
    {
        // 寻找当前最小的路径；
        // 即，在未获取最短路径的顶点中，找到离vs最近的顶点(k)。
        min = INF;
        for (j = 0; j <vertexNum; j++)
        {
            //if (flag[j]==0 && dist[j]<min)//正确代码
			if(dist[j]<min) //BUG--詹泽生 
            {
                min = dist[j];
                k = j;
            }
        }
        // 标记"顶点k"为已经获取到最短路径
        flag[k] = 1;
 
        // 修正当前最短路径和前驱顶点
        // 即，当已经"顶点k的最短路径"之后，更新"未获取最短路径的顶点的最短路径和前驱顶点"。
        for (j = 0; j <vertexNum; j++)
        {
            tmp = (edge[k][j]==-1 ? INF : (min + edge[k][j])); // 防止溢出
           // if (flag[j] == 0 && (tmp  < dist[j]) )//正确代码 
           if (flag[j] == 0 && (tmp  >= dist[j]) )//BUG-詹泽生  
            {
                dist[j] = tmp;
                prev[j] = k;
            }
        }
    }
 
    // 打印dijkstra最短路径的结果
    cout<<"dijkstra从"<<vertex[begin]<<"出发"<<endl; 
    //printf("dijkstra从(%c)出发: \n",vertex[begin]);
    for (i = 0; i < vertexNum; i++)
        //printf("  shortest(%s, %s)=%d\n",vertex[begin], vertex[i], dist[i]);
        cout<<"shortest("<<vertex[begin]<<", "<<vertex[i]<<")="<<dist[i]<<endl;

}
 
 
template<class T>
void prim(Graph<T> G)
{
	cout<<"\n最小生成树Prim:\n"; 
	//1.初始化shortEdge数组,假设从第0个顶点开始 
	shortEdge shortedge[G.vertexNum];
	for(int i=1;i<G.vertexNum;i++)
	{
		shortedge[i].edgeIndex=0;
		shortedge[i].lowcost=G.edge[0][i];
    } 
    //2.将顶点0加入集合U 
    shortedge[0].edgeIndex=0;
    shortedge[0].lowcost=0;
    //3.
    int min,x;
    for(int i=1;i<G.vertexNum;i++)
	{
		//找出最短边的邻接点 x
		min=-1;
		for(int k=1;k<G.vertexNum;k++)
		{
			if((min==-1||shortedge[k].lowcost<min)&&shortedge[k].lowcost>0)
			{
				min=shortedge[k].lowcost;
				x=k;
			}
		}
		cout<<"("<<shortedge[x].edgeIndex<<"->"<<x<<")"<<shortedge[x].lowcost<<endl;
		//将顶点k加入集合U 
		shortedge[x].lowcost=0;
		//更新shortedge
		for(int j=1;j<G.vertexNum;j++)
		{
			if(G.edge[x][j]!=0&&G.edge[x][j]!=-1)
			{
				if(shortedge[j].lowcost==-1||G.edge[x][j]<shortedge[j].lowcost)
				{
					shortedge[j].lowcost=G.edge[x][j];
				    shortedge[j].edgeIndex=x;
				}
			}
        } 
	}
} 
 
int main()
{   
	string v[]={"v0","v1","v2","v3","v4","v5"};
	Graph<string> myGraph(v,6,9);
	cout<<" \n深度优先遍历:";
	myGraph.DFSTraverse(0);
	
	myGraph.resetVisted();
	
	cout<<" \n广度优先遍历：";
	myGraph.BFSTraverse(0);
	
	cout<<" \n最短路径：";
	
	myGraph.Djstl(0); 
	
	prim(myGraph);
	return 0;
}
