## 普里姆(Prim)算法

普里姆(Prim)算法的基本思想是：假设连通网络为N＝(V，E)；TE为N的最小生成树上边的集合，开始时TE=∅，U为算法在构造最小生成树过程中已得到的顶点集，开始时U={u0}（u0 ∈ V）。算法从N中的某一顶点u0出发，选择与u0关联的具有最小权值的边(u0、vi)，将顶点vi加入到生成树的顶点集合U中，(u0，vi)加入到集合TE中，以后每一步从一个顶点在U中，而另一个顶点在V-U中的各条边当中选择权值最小的边(u、v)（u ∈ U，v ∈ V-U），把顶点v加入到集合U中，边(v、u)加入到集合TE中。如此重复，直到网络中的所有顶点都加入到生成树顶点集合U（U=V）中为止。此时，TE中刚好有n-1条边，则T=（V，TE）为N的最小生成树。

对于前图（a）所示的连通网络，下图(a)~(f)给出了按普里姆算法从顶点A开始构造最小生成树的过程。 

![](img/prim.png)

在利用普里姆算法构造最小生成树过程中，需要设置了一个辅助数组closearc[ ]，以记录从V-U中顶点到U中顶点具有最小权值的边。对每一个顶点v ∈ V-U，在辅助数组中有一个分量closearc[v]，它包括两个域：lowweight和nearvertex。其中，lowweight中存放顶点v到U中的各顶点的边上的当前最小权值(lowweight=0表示v ∈ U)；nearvertex记录顶点v到U中具有最小权值的那条边的另一个邻接顶点u（nearvertex=-1表示该顶点v为开始顶点）。在下面的普里姆算法描述中，连通网络采用邻接矩阵作为存储结构，并假设普里姆算法从顶点A（设顶点A的序号为0）出发（即u0=0）。 

普里姆算法步骤如下：

（1）初始化辅助数组closearc[]；

（2）重复下列步骤（3）和（4）n-1次

（3）在closearc[]中选择lowweight≠0 && lowweight最小的顶点v，即选中的权值最小的边为(closearc[v].nearvertex,i)。

（4）将closearc[v].lowweight改为0，表示顶点i已加入顶点集U中。并将边(closearc[v]. nearvertex、v)加入生成树T的边集合。

（5）对V-U中的每一个顶点j，如果依附于顶点j和刚加入U集合的新顶点v的边的权值Arcs\[v]\[j]小于原来依附于j和生成树顶点集合中顶点的边的最短距离closearc\[j].lowweight，则修改closearc\[j]，使其lowweight = Arcs\[v][j]}，nearvertex = v。

下图给出了对于前图（a）所示的连通网络，按普里姆算法构造最小生成树时辅助数组closearc[ ]的变化过程。

![](img/prim2.png)