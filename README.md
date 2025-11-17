# Pseudo code
## Graph::asAdjMatrix()
```cpp
int** Graph::asAdjMatrix() {
    int matrix[numV][numV];
    for (int i = 0; i < numV; i++) {
        for (int j = 0; j < numV; j++) {
            if (isEdge(i, j)) {
                matrix[i][j] = vertices[i].adj[j];
            } else {
                matrix[i][j] = INT_MAX;
            }
        }
    }

    return matrix;
}
```

## Graph::BFS()
```cpp
#include <queue>
Graph::BFS(source) {
    for (int i = 0; i < numV; i++) {
        VertexInfo vrt = vertices[i];
        vrt.color = 'w';
        vrt.d = INT_MAX;
        vrt.pre = nullptr;
    }
    VertexInfo src = vertices[source];
    src.color = 'g';
    src.d = 0;
    src.pre = nullptr;

    queue<K> q;
    q.push(src);
    while(!q.empty()) {
        VertexInfo predecessor = q.front();
        q.pop();

        const auto& adj = vertices[current].adj;

        for (const auto& edge: adj) {
            K v = get<0>(edge);
            if (v.color == 'w') {
                v.color = 'g';
                v.d = predecessor.d + 1;
                v.pre = predecessor;
                q.push(v);
            }
        }
        predecessor.color = 'b';
    }
}
```
