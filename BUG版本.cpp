#include<iostream>
#include<queue>
#include<string>
#include<fstream>
#include<iomanip>
using namespace std;
#define INF 10000
const int MaxSize=20;
 
//����prim 
struct shortEdge{
	int edgeIndex;  //�ߵı�� 
	int lowcost;    //��СȨֵ 
};
 
template<class DataType>//���ڶ������ݵ����Ͳ�ȷ�� 
class Graph
{
private:
	int GType;//ͼ������0������ͼ  1������ͼ
	DataType vertex[MaxSize];//�������飬���涥������
	int edge[MaxSize][MaxSize];//��
	int vertexNum, edgeNum;
	int isTrav[MaxSize];
	int visited[MaxSize];
 
public:	
	Graph(DataType a[], int n, int e);//���캯������������n�����㣬e���ߵ�ͼ
	~Graph(){ } //��������Ϊ��
	void resetVisted();
	void DFSTraverse(int v);//Depth��first traverse������ȱ���
	void BFSTraverse(int v);//Breadth��first traverse������ȱ���
	void Djstl(int begin);//���·�� 
	template<class T>
	friend void prim(Graph<T> G);//��Ԫ���������ڷ������е�˽�г�Ա����С��������prim�㷨 
};
 
//���캯�� 
//�ڽӾ����ʼ��Ϊ-1����ʾû��·��
template<class DataType>
Graph<DataType>::Graph(DataType a[], int n, int e)
{
	vertexNum=n;
	edgeNum=e;
	for(int i=0;i<vertexNum;i++)  //�߱���Ϣ 
		vertex[i]=a[i];
	for(int i=0;i<vertexNum;i++)  //�ڽӾ����ʼ��Ϊ-1����ʾû��·���� 
	{
		for(int j=0;j<vertexNum;j++)
		{
				edge[i][j]=-1;
		}
	
	} 
	int i,j,value;
	
	ifstream infile;
	infile.open("GraphData.txt");
	for(int k=0;k<e;k++)   //����ߵ����ӵ����������ż�Ȩֵ 
	{
        //cin>>i>>j>>value;
        infile>>i;infile>>j;infile>>value;
        edge[i][j]=value;
      //  edge[j][i]=value; //��������˵���Ǽ�Ȩ����ͼ���˴����³�Ϊ����ͼ --BUG ղ����  
	}
//	infile.close();//��ȷ���룬���ر��ļ� --BUG ���� 
	
	/*�����õ�������룬Ӱ�����· BUG--ղ���� 
	for(int i=0;i<vertexNum;i++)  //
	{
		for(int j=0;j<vertexNum;j++)
		{
			if(i==j)
				edge[i][j]=0;
		}
	
	} 
	*/
	
	cout<<"�ڽӾ���\n";
	for(int x=0;x<vertexNum;x++)
	{
		for(int y=0;y<vertexNum;y++)
		    cout<<setw(10)<<edge[x][y]; 
        cout<<endl;
	} 
	resetVisted(); //���÷��ʱ� 
}
 
//���÷������飬��ʼ����ÿ�α���ǰ��Ҫ���� 
template<class DataType>
void Graph<DataType>::resetVisted()
{
	//�޸�ѭ�� 
	//for(int i=0;i<vertexNum;i++) ��ȷ�汾 
	for(int i=0;i<vertexNum-1;i++) //BUG --���� 
		visited[i]=0;
}
 
//������ȱ��� 
template<class DataType>
void Graph<DataType>::DFSTraverse(int v)
{
    cout<<vertex[v]<<" ";
    visited[v]=1;
    for(int i=0;i<vertexNum;i++)
    	//if(edge[v][i]!=-1&&visited[i]==0) //��ȷ���� 
    	if(visited[i]==0) //BUG--�Զ��� ������ 
    		DFSTraverse(i);
}
 
//������ȱ��� 
template<class DataType>
void Graph<DataType>::BFSTraverse(int v)
{
	queue<int> Q;//���У����ڹ�ȱ���
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
                //����Ȫ�Ķ���ɾ�������� 
                //visited[i]=1; //��ȷ���� 
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
    int flag[MaxSize];      // flag[i]=1��ʾ"����vs"��"����i"�����·���ѳɹ���ȡ��
 	int dist[MaxSize];
 	int prev[MaxSize];
    // ��ʼ��
    for (i = 0; i <vertexNum; i++)
    {
        flag[i] = 0;              // ����i�����·����û��ȡ����
        prev[i] = -1;              // ����i��ǰ������Ϊ0��
        /*��ȷ��ʼ�� 
		if(edge[begin][i]!=-1)
       {
       	dist[i]=edge[begin][i];
	   }
	   else
	   {
	   	dist[i]=INF;
	   }*/
        dist[i] = edge[begin][i];//�������-������ 
    }
 
    // ��"����vs"������г�ʼ��
    flag[begin] = 1;
    dist[begin] = 0;
 
    // ����G.vexnum-1�Σ�ÿ���ҳ�һ����������·����
    for (i = 0; i<vertexNum; i++)
    {
        // Ѱ�ҵ�ǰ��С��·����
        // ������δ��ȡ���·���Ķ����У��ҵ���vs����Ķ���(k)��
        min = INF;
        for (j = 0; j <vertexNum; j++)
        {
            //if (flag[j]==0 && dist[j]<min)//��ȷ����
			if(dist[j]<min) //BUG--ղ���� 
            {
                min = dist[j];
                k = j;
            }
        }
        // ���"����k"Ϊ�Ѿ���ȡ�����·��
        flag[k] = 1;
 
        // ������ǰ���·����ǰ������
        // �������Ѿ�"����k�����·��"֮�󣬸���"δ��ȡ���·���Ķ�������·����ǰ������"��
        for (j = 0; j <vertexNum; j++)
        {
            tmp = (edge[k][j]==-1 ? INF : (min + edge[k][j])); // ��ֹ���
           // if (flag[j] == 0 && (tmp  < dist[j]) )//��ȷ���� 
           if (flag[j] == 0 && (tmp  >= dist[j]) )//BUG-ղ����  
            {
                dist[j] = tmp;
                prev[j] = k;
            }
        }
    }
 
    // ��ӡdijkstra���·���Ľ��
    cout<<"dijkstra��"<<vertex[begin]<<"����"<<endl; 
    //printf("dijkstra��(%c)����: \n",vertex[begin]);
    for (i = 0; i < vertexNum; i++)
        //printf("  shortest(%s, %s)=%d\n",vertex[begin], vertex[i], dist[i]);
        cout<<"shortest("<<vertex[begin]<<", "<<vertex[i]<<")="<<dist[i]<<endl;

}
 
 
template<class T>
void prim(Graph<T> G)
{
	cout<<"\n��С������Prim:\n"; 
	//1.��ʼ��shortEdge����,����ӵ�0�����㿪ʼ 
	shortEdge shortedge[G.vertexNum];
	for(int i=1;i<G.vertexNum;i++)
	{
		shortedge[i].edgeIndex=0;
		shortedge[i].lowcost=G.edge[0][i];
    } 
    //2.������0���뼯��U 
    shortedge[0].edgeIndex=0;
    shortedge[0].lowcost=0;
    //3.
    int min,x;
    for(int i=1;i<G.vertexNum;i++)
	{
		//�ҳ���̱ߵ��ڽӵ� x
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
		//������k���뼯��U 
		shortedge[x].lowcost=0;
		//����shortedge
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
	cout<<" \n������ȱ���:";
	myGraph.DFSTraverse(0);
	
	myGraph.resetVisted();
	
	cout<<" \n������ȱ�����";
	myGraph.BFSTraverse(0);
	
	cout<<" \n���·����";
	
	myGraph.Djstl(0); 
	
	prim(myGraph);
	return 0;
}
