#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

bool answer = false;

bool NFA(char *str, int size, char *str1, int x, int y, int n, FILE *file)
{
    printf("%s", str1);
    int len = snprintf(NULL, 0, "%d", (x + (n + 1) * y));
    char *strTemp = (char *)malloc(len + 1);
    snprintf(strTemp, len + 1, "%d", (x + (n + 1) * y));
    strcat(str1, strTemp);
    strcat(str1, " ");

    for (int i = 0; i < size; i++)
    {
        pid_t p = fork();
        if (str[i] == '0')
        {
            if (!p && x)
            {
                x--;
                int len = snprintf(NULL, 0, "%d", (x + (n + 1) * y));
                char *strTemp = (char *)malloc(len + 1);
                snprintf(strTemp, len + 1, "%d", (x + (n + 1) * y));
                strcat(str1, strTemp);
                strcat(str1, " ");
            }
            else
            {
                if (x < n)
                {
                    wait(NULL);
                    x++;
                    int len = snprintf(NULL, 0, "%d", (x + (n + 1) * y));
                    char *strTemp = (char *)malloc(len + 1);
                    snprintf(strTemp, len + 1, "%d", (x + (n + 1) * y));
                    strcat(str1, strTemp);
                    strcat(str1, " ");
                }
                else
                    goto j;
            }
        }
        else
        {
            if (!p && y)
            {
                y--;
                int len = snprintf(NULL, 0, "%d", (x + (n + 1) * y));
                char *strTemp = (char *)malloc(len + 1);
                snprintf(strTemp, len + 1, "%d", (x + (n + 1) * y));
                strcat(str1, strTemp);
                strcat(str1, " ");
            }
            else
            {
                if (y < n)
                {
                    wait(NULL);
                    y++;
                    int len = snprintf(NULL, 0, "%d", (x + (n + 1) * y));
                    char *strTemp = (char *)malloc(len + 1);
                    snprintf(strTemp, len + 1, "%d", (x + (n + 1) * y));
                    strcat(str1, strTemp);
                    strcat(str1, " ");
                }
                else
                    goto j;
            }
        }
        fprintf(file, "[%d:%d] Exploring Path: %s\n", getpid(), getppid(), str1);
    }
    if (x == y && x == n)
    {
        fprintf(file, "Accepted! Followed Path: %s\n", str1);
        answer = true;
        kill(0, SIGKILL);
        exit(0);
    }
    j:;
}

int str_length(char str[])
{
    int count;
    for (count = 0; str[count] != '\0'; count++);
    return count;
}

int main(int argc, char *argv[])
{
    FILE *file, *file1, *file2;
    char ch;
    char str[1001] = {'\0'};
    file = fopen("input.txt", "r");
    file1 = fopen("2020A7PS0034H_t1.txt", "w");
    file2 = fopen("2020A7PS0034H_t2.txt", "w");
    if (file == NULL)
    {
        fprintf(file, "Error! File cannot be opened.");
        exit(1);
    }
    int n;
    fscanf(file, "%d %s", &n, &str);
    char str1[1000] = {'\0'};
    NFA(str, str_length(str), str1, 0, 0, n, file1);
    if (!answer)
        fprintf(file1, "Failed at path: %s\n", str1);

    int t = (n + 1) * (n + 1);
    int i, j;
    int sol[(2 * t) + 1][t];
    for (i = 0; i < (2 * t) + 1; i++)
    {
        for (j = 0; j < t; j++)
            sol[i][j] = 0;
    }
    sol[0][t - 1] = 1;

    for (i = 0; i < n + 1; i++)
    {
        for (j = 0; j < n + 1; j++)
        {
            if (j == 0)
                sol[((n + 1) * i) + 1][(n + 1) * (i) + 1] = 1;
            else if (j == n)
                sol[((n + 1) * i) + n + 1][(n + 1) * (i + 1) - 2] = 1;
            else
            {
                sol[((n + 1) * i) + j + 1][(n + 1) * (i) + j - 1] = 1;
                sol[((n + 1) * i) + j + 1][(n + 1) * (i) + j + 1] = 1;
            }
        }
    }

    for (i = 0; i < n + 1; i++)
    {
        for (j = 0; j < n + 1; j++)
        {
            if (i == 0)
                sol[t + 1 + (n + 1) * (i) + j][n + 1 + j] = 1;
            else if (i == n)
                sol[t + 1 + j + n * (n + 1)][(n * n) + j - 1] = 1;
            else
            {
                sol[t + 1 + j + n + 1 + (n + 1) * (i - 1)][j + (n + 1) * (i - 1)] = 1;
                sol[t + 1 + j + n + 1 + (n + 1) * (i - 1)][j + 2 * (n + 1) + (n + 1) * (i - 1)] = 1;
            }
        }
    }
    for (i = 0; i < (2 * t) + 1; i++)
    {
        for (j = 0; j < t; j++)
            fprintf(file2, "%d ", sol[i][j]);
        fprintf(file2, "\n");
    }

    fclose(file);
    fclose(file1);
    fclose(file2);

    return 0;
}