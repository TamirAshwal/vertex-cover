#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

bool isVertexCover(int graph[MAX_VERTICES][MAX_VERTICES], int n, int cover[], int k) {
    bool edges_covered[MAX_VERTICES * MAX_VERTICES] = {false};
    
    for (int i = 0; i < k; i++) {
        int v = cover[i];
        for (int j = 0; j < n; j++) {
            if (graph[v][j]) {
                edges_covered[v * n + j] = edges_covered[j * n + v] = true;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (graph[i][j] && !edges_covered[i * n + j]) {
                return false;
            }
        }
    }
    return true;
}

void generateCombinations(int n, int k, int graph[MAX_VERTICES][MAX_VERTICES], int* minCover, int* minSize) {
    int cover[MAX_VERTICES];
    for (int i = 0; i < k; i++) cover[i] = i;
    
    while (1) {
        if (isVertexCover(graph, n, cover, k)) {
            *minSize = k;
            // Copy cover to minCover without memcpy
            for (int i = 0; i < k; i++) {
                minCover[i] = cover[i];
            }
            return;
        }
        
        // Generate next combination
        int i;
        for (i = k - 1; i >= 0 && cover[i] == n - k + i; i--);
        if (i < 0) break;
        cover[i]++;
        for (int j = i + 1; j < k; j++) cover[j] = cover[j-1] + 1;
    }
}

void findMinVertexCover(int graph[MAX_VERTICES][MAX_VERTICES], int n) {
    int* minCover = (int*)malloc(n * sizeof(int));
    int* cover = (int*)malloc(n * sizeof(int));
    int minSize = n + 1;
    int lastPowerOf2 = 1;
    // find the first power of 2 that we get a valid answer

    for (int k = 1; k <= n && minSize > k; k*=2) {
        generateCombinations(n, k, graph, minCover, &minSize);
        lastPowerOf2 = k;

    }
    // check all the number between 2^(i-1) and 2^i
    for (int k = lastPowerOf2; k <= lastPowerOf2 * 2 && minSize > k; k++) {
        generateCombinations(n, k,graph, minCover, &minSize);
    }

    printf("Minimum Vertex Cover: ");
    for (int i = 0; i < minSize; i++) {
        printf("%d ", minCover[i]);
    }
    printf("\nSize: %d\n", minSize);

    free(minCover);
    free(cover);
}