#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

bool isVertexCover(int graph[MAX_VERTICES][MAX_VERTICES], int n, int cover[], int k) {
    bool edges_covered[MAX_VERTICES][MAX_VERTICES] = {false};
    
    for (int i = 0; i < k; i++) {
        // convert indexing to use pointers
        int v = *cover++;
        for (int j = 0; j < n; j++) {
            if (graph[v][j]) {
                edges_covered[v][j] = edges_covered[j][v] = true;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (graph[i][j] && !edges_covered[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void generateCombinations(int n, int k, int cover[], int start, int currentSize,
                          int graph[MAX_VERTICES][MAX_VERTICES], int* minCover, int* minSize) {
    if (currentSize == k) {
        if (isVertexCover(graph, n, cover, k)) {
            // we found a solution check if it is better then the minSize
            if (k < *minSize) {
                // if so update minSize and minCover
                *minSize = k;
                // use loop unrolling
                int i;
                // convert indexing to use pointers
                for (i = 0; i < k - 3; i+=4) {
                    *minCover++ = *cover++;
                    *minCover++ = *cover++;
                    *minCover++ = *cover++;
                    *minCover++ = *cover++;
                }
                for (; i < k; i++) {
                    *minCover++ = *cover++;
                }

            }
        }
        return;
    }

    for (int i = start; i < n; i++) {
        cover[currentSize] = i;
        generateCombinations(n, k, cover, i + 1, currentSize + 1, graph, minCover, minSize);
        if (*minSize <= k) return; // Early termination if we've found a solution of size k or smaller
    }
}

void findMinVertexCover(int graph[MAX_VERTICES][MAX_VERTICES], int n) {
    int* minCover = (int*)malloc(n * sizeof(int));
    int* cover = (int*)malloc(n * sizeof(int));
    int minSize = n + 1;
    int lastPowerOf2 = 1;
    // find the first power of 2 that we get a valid answer

    for (int k = 1; k <= n && minSize > k; k*=2) {
        generateCombinations(n, k, cover, 0, 0, graph, minCover, &minSize);
        lastPowerOf2 = k;

    }
    // check all the number between 2^(i-1) and 2^i
    for (int k = lastPowerOf2; k <= lastPowerOf2 * 2 && minSize > k; k++) {
        generateCombinations(n, k, cover, 0, 0, graph, minCover, &minSize);
    }

    printf("Minimum Vertex Cover: ");
    for (int i = 0; i < minSize; i++) {
        printf("%d ", minCover[i]);
    }
    printf("\nSize: %d\n", minSize);

    free(minCover);
    free(cover);
}
