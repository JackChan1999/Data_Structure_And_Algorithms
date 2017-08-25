### 200. Number of Islands

给定一个二维数组，只含有0和1两个字符。其中1代表陆地，0代表水域。横向和纵向的陆地连接成岛屿，被水域分隔开。问给出的地图中有多少岛屿？

```c++
class Solution {
private:
    int d[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};
    int m,n;
    vector<vector<bool>> visited;

    bool inArea( int x , int y ){
        return x >= 0 && x < m && y >= 0 && y < n;
    }

    // 从grid[x][y]的位置开始,进行floodfill
    // 保证(x,y)合法,且grid[x][y]是没有被访问过的陆地
    void dfs( vector<vector<char>>& grid , int x , int y ){

        //assert( inArea(x,y) );
        visited[x][y] = true;
        for( int i = 0 ; i < 4 ; i ++ ){
            int newx = x + d[i][0];
            int newy = y + d[i][1];
            if( inArea(newx, newy) && !visited[newx][newy] && grid[newx][newy] == '1' )
                dfs( grid , newx , newy );
        }

        return;
    }
public:
    int numIslands(vector<vector<char>>& grid) {

        m = grid.size();
        if( m == 0 )
            return 0;
        n = grid[0].size();
        visited = vector<vector<bool> >(m,vector<bool>(n,false));

        int res = 0;
        for( int i = 0 ; i < m ; i ++ )
            for( int j = 0 ; j < n ; j ++ )
                if( grid[i][j] == '1' && !visited[i][j] ){
                    dfs( grid , i , j );
                    res ++;
                }
        return res;
    }
};
```