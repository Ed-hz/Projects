#include <algorithm>
#include <iostream>
#include <math.h>
#include <queue>
#include <stdio.h>
#include <string.h>

using namespace std;

#define N 50005

#define lson rt << 1
#define rson rt << 1 | 1
#define Pair pair<double, Node>
#define Sqrt2(x) (x) * (x)

int n, k, idx;

struct Node
{
    int feature[5]; //������������
    bool operator<(const Node &u) const
    {
        return feature[idx] < u.feature[idx];
    }
} _data[N]; //_data[]����������������

priority_queue<Pair> Q; //����Q���ڴ����p�����m������

class KDTree
{

public:
    void Build(int, int, int, int);  //����
    void Query(Node, int, int, int); //��ѯ

private:
    Node data[4 * N]; //data[]�������K-D�������нڵ�����
    int flag[4 * N];  //���ڱ��ĳ���ڵ��Ƿ���ڣ�1��ʾ���ڣ�-1��ʾ������
} kd;

//�������裬����dept�����������
void KDTree::Build(int l, int r, int rt, int dept)
{
    if (l > r)
        return;
    flag[rt] = 1;                 //��ʾ���Ϊrt�Ľڵ����
    flag[lson] = flag[rson] = -1; //��ǰ�ڵ�ĺ�����ʱ��ǲ�����
    idx = dept % k;               //���ձ��Ϊidx�����Խ��л���
    int mid = (l + r) >> 1;
    nth_element(_data + l, _data + mid, _data + r + 1); //nth_element()ΪSTL�еĺ���
    data[rt] = _data[mid];
    Build(l, mid - 1, lson, dept + 1); //�ݹ�������
    Build(mid + 1, r, rson, dept + 1); //�ݹ�������
}

//��ѯ������Ѱ����p�����m����������
void KDTree::Query(Node p, int m, int rt, int dept)
{
    if (flag[rt] == -1)
        return;            //�����ڵĽڵ㲻����
    Pair cur(0, data[rt]); //��ȡ��ǰ�ڵ�����ݺ͵�p�ľ���
    for (int i = 0; i < k; i++)
        cur.first += Sqrt2(cur.second.feature[i] - p.feature[i]);
    int dim = dept % k; //������һ���������ܱ�֤��ͬ�ڵ��dimֵ����
    bool fg = 0;        //���ڱ���Ƿ���Ҫ����������
    int x = lson;
    int y = rson;
    if (p.feature[dim] >= data[rt].feature[dim]) //����p�ĵ�dim������ֵ���ڵ��ڵ�ǰ�����ݣ�����Ҫ����������
        swap(x, y);
    if (~flag[x])
        Query(p, m, x, dept + 1); //����ڵ�x���ڣ������������������

    //�����ǻ��ݹ��̣�ά��һ�����ȶ���
    if (Q.size() < m) //�������û���������������
    {
        Q.push(cur);
        fg = 1;
    }
    else
    {
        if (cur.first < Q.top().first) //����ҵ���С�ľ��룬�������滻����Q�����ľ��������
        {
            Q.pop();
            Q.push(cur);
        }
        if (Sqrt2(p.feature[dim] - data[rt].feature[dim]) < Q.top().first)
        {
            fg = 1;
        }
    }
    if (~flag[y] && fg)
        Query(p, m, y, dept + 1);
}

//������
void Print(Node data)
{
    for (int i = 0; i < k; i++)
        printf("%d%c", data.feature[i], i == k - 1 ? '\n' : ' ');
}

int main()
{
    while (scanf("%d%d", &n, &k) != EOF)
    {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < k; j++)
                scanf("%d", &_data[i].feature[j]);
        kd.Build(0, n - 1, 1, 0);
        int t, m;
        scanf("%d", &t);
        while (t--)
        {
            Node p;
            for (int i = 0; i < k; i++)
                scanf("%d", &p.feature[i]);
            scanf("%d", &m);
            while (!Q.empty())
                Q.pop(); //������Ҫ������ȶ���
            kd.Query(p, m, 1, 0);
            printf("the closest %d points are:\n", m);
            Node tmp[25];
            for (int i = 0; !Q.empty(); i++)
            {
                tmp[i] = Q.top().second;
                Q.pop();
            }
            for (int i = m - 1; i >= 0; i--)
                Print(tmp[i]);
        }
    }
    return 0;
}