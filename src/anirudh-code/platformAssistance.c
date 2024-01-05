/* Needs to be updated, incomplete code
*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int adjacencyMatrix[10][10] = {
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
    {1, 0, 1, 0, 1, 0, 0, 1, 0, 0},
    {0, 1, 0, 1, 0, 0, 1, 0, 0, 1},
    {0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
    {0, 0, 1, 0, 0, 1, 0, 0, 1, 0}};

int weightMatrix[10][10] = {
    {0, 2, 999999, 999999, 999999, 999999, 5, 999999, 999999, 999999},
    {2, 0, 3, 999999, 2, 999999, 999999, 10, 999999, 999999},
    {999999, 3, 0, 5, 999999, 999999, 8, 999999, 999999, 2},
    {999999, 999999, 5, 0, 3, 999999, 999999, 999999, 999999, 999999},
    {999999, 2, 999999, 3, 0, 5, 999999, 7, 999999, 999999},
    {999999, 999999, 999999, 999999, 5, 0, 999999, 999999, 4, 2},
    {5, 999999, 8, 999999, 999999, 999999, 0, 3, 999999, 999999},
    {999999, 10, 999999, 999999, 7, 999999, 3, 0, 6, 999999},
    {999999, 999999, 999999, 999999, 999999, 4, 999999, 6, 0, 2},
    {999999, 999999, 2, 999999, 999999, 2, 999999, 999999, 2, 0}};

void dijkstra(int graph[10][10], int src, int dest, int numPlatforms)
{
    int *dist = (int *)malloc(sizeof(int) * numPlatforms);
    int *visited = (int *)malloc(sizeof(int) * numPlatforms);

    for (int i = 0; i < numPlatforms; i++)
    {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }

    dist[src] = 0;

    for (int count = 0; count < numPlatforms - 1; count++)
    {
        int minDist = INT_MAX, minIndex;

        for (int v = 0; v < numPlatforms; v++)
        {
            if (!visited[v] && dist[v] <= minDist)
            {
                minDist = dist[v];
                minIndex = v;
            }
        }

        visited[minIndex] = 1;

        for (int v = 0; v < numPlatforms; v++)
        {
            if (!visited[v] && graph[minIndex][v] && dist[minIndex] != INT_MAX &&
                dist[minIndex] + graph[minIndex][v] < dist[v])
            {
                dist[v] = dist[minIndex] + graph[minIndex][v];
            }
        }
    }

    printf("Shortest distance between Platform %d and Platform %d is: %d\n", src + 1, dest + 1, dist[dest]);
}

void shortestRouteBwPlatforms()
{
    int initialPlatform, finalPlatform;

    printf("Enter initial platform (1-10): ");
    scanf("%d", &initialPlatform);

    printf("Enter final platform (1-10): ");
    scanf("%d", &finalPlatform);

    if (initialPlatform < 1 || initialPlatform > 10 || finalPlatform < 1 || finalPlatform > 10)
    {
        printf("Invalid platform numbers. Platforms should be between 1 and 10.\n");
        return;
    }

    dijkstra(weightMatrix, initialPlatform - 1, finalPlatform - 1, 10);
}

int main()
{
    shortestRouteBwPlatforms();
    return 0;
}
