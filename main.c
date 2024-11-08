#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

typedef struct node {
    int n;
    struct node* next;
} node;

int** createG(int n) {
    int** G = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; ++i) {
        G[i] = (int*)malloc(n * sizeof(int));
    }
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            G[i][j] = rand() % 2;
            G[j][i] = (i == j) ? 0 : G[i][j];
        }
    }
    return G;
}

void printG(int** G, int n) {
    printf("  ");
    for (int i = 0; i < n; ++i) printf("%d ", i);
    printf("\n");
    for (int i = 0; i < n; ++i) {
        printf("%d ", i);
        for (int j = 0; j < n; ++j) {
            printf("%d ", G[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

node** createAdj(int** G, int n) {
    node** A = (node**)malloc(n * sizeof(node*));
    for (int i = 0; i < n; ++i) {
        A[i] = NULL;
        node* tail = NULL; 
        for (int j = 0; j < n; ++j) {
            if (G[i][j] == 1) {
                node* newNode = (node*)malloc(sizeof(node));
                newNode->n = j;
                newNode->next = NULL;
                if (tail == NULL) {
                    A[i] = newNode;
                }
                else {
                    tail->next = newNode; 
                }
                tail = newNode; 
            }
        }
    }
    return A;
}

void printAdj(node** A, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d: ", i);
        node* current = A[i];
        while (current != NULL) {
            printf("%d ", current->n);
            current = current->next;
        }
        printf("\n");
    }
    printf("\n");
}

int** delG(int** G, int size, int v) {
	int** G1 = createG(size - 1);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (j < v && i < v)	G1[i][j] = G[i][j];
			if (j > v && i < v)	G1[i][j - 1] = G[i][j];
			if (j<v && i>v)	G1[i - 1][j] = G[i][j];
			if (j > v && i > v)	G1[i - 1][j - 1] = G[i][j];
		}
	}

	for (int i = 0; i < size; i++) {
		free(G[i]);
	}
	free(G);
	G = NULL;

	return G1;
}

int** unionV(int** G, int size) {
	int v1 = 0, v2 = 0;
	printf("ver1 for union =");
	scanf("%d", &v1);
	printf("ver2 for union =");
	scanf("%d", &v2);
	for (int i = 0; i < size; i++) {
		if (G[i][v2] == 1) {
			G[i][v1] = G[i][v2];
			G[v1][i] = G[v2][i];
		}
	}
	// printG(G, size);
	if (G[v1][v2] == 1) G[v1][v1] = 1;
	G = delG(G, size, v2);
	return G;
}

int** contrV(int** G, int size, int v1, int v2) {
	G[v1][v2] = 0;
	G[v2][v1] = 0;
	for (int i = 0; i < size; i++) {
		if (G[i][v2] == 1) {
			G[i][v1] = G[i][v2];
			G[v1][i] = G[v2][i];
		}
	}

	G = delG(G, size, v2);
	G[v1][v1] = 0;
	return G;
}

int** splitV(int** G, int size, int v) {
    int** Gtemp = createG(size + 1);

    for (int i = 0; i < size; ++i) {
        Gtemp[i][size] = G[i][v];
        Gtemp[size][i] = G[v][i];
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            Gtemp[i][j] = G[i][j];
        }
    }

    Gtemp[size][size] = 0;
    for (int i = 0; i < size; ++i) {
        free(G[i]);
    }
    free(G);
    G = NULL;
    return Gtemp;
}

int** unionG(int **G1, int **G2, int size1, int size2) {
    int sizemax = (size1 > size2) ? size1 : size2;
    int sizemin = (size1 < size2) ? size1 : size2;
    int** Gmax = (size1 > size2) ? G1 : G2;
    int** Gmin = (size1 < size2) ? G1 : G2;
    int** Gtemp = createG(sizemax);

    for (int i = 0; i < sizemin; ++i) {
        for (int j = 0; j < sizemin; ++j) {
            Gtemp[i][j] = Gmin[i][j] | Gmax[i][j];
        }
    }
    for (int i = 0; i < sizemax; ++i) {
        for (int j = sizemin; j < sizemax; ++j) {
            Gtemp[i][j] = Gmax[i][j];
            Gtemp[j][i] = Gtemp[i][j];
        }
    }
    return Gtemp;
}

int** intersectG(int** G1, int** G2, int size1, int size2) {
    int sizemin = (size1 < size2) ? size1 : size2;
    int** Gtemp = createG(sizemin);

    for (int i = 0; i < sizemin; ++i) {
        for (int j = 0; j < sizemin; ++j) {
            Gtemp[i][j] = G1[i][j]&G2[i][j];
        }
    }
    return Gtemp;
}

int** xorG(int** G1, int** G2, int size1, int size2) {

    int sizemax = (size1 > size2) ? size1 : size2;
    int sizemin = (size1 < size2) ? size1 : size2;
    int** Gmax = (size1 > size2) ? G1 : G2;
    int** Gmin = (size1 < size2) ? G1 : G2;
    int** Gtemp = createG(sizemax - sizemin);

    for (int i = sizemin; i < sizemin; ++i) {
        for (int j = sizemin; j < sizemax; ++j) {
            Gtemp[i - sizemin][j - sizemin] = Gmax[i][j];
        }
    }
    return Gtemp;
}

int main() {
    setlocale(LC_ALL, "Rus");
    int n = 4;
    int** M1 = NULL, ** M2 = NULL;
    node** A1 = NULL, ** A2 = NULL;
    printf("Введите количество вершин: ");
    srand(time(NULL));

    //scanf("%d", &n);
    printf("\n");

    M1 = createG(n);
    printf("Граф G1\n");
    printG(M1, n);
    M2 = createG(n);
    printf("Граф G2\n");
    printG(M2, n);

    A1 = createAdj(M1, n);
    printf("список смежности Граф G1\n");
    printAdj(A1, n);

    A2 = createAdj(M2, n);
    printf("список смежности Граф G2\n");
    printAdj(A2, n);

    free(A1);
    free(A2);
    free(M1);
    free(M2);

    return 0;
    //srand(time(NULL));
    //int nG1, nG2, v1, v2;
    //printf("nG1 =");
    //scanf("%d", &nG1);
    //printf("nG2 =");
    //scanf("%d", &nG2);

    //int** G1 = createG(nG1);
    //int** G2 = createG(nG2);

    //printG(G1, nG1);
    //printG(G2, nG2);

    //G1 = unionV(G1, nG1);
    //nG1--;
    //printf("\n");
    //printG(G1, nG1);

    //printf("ver1 for contr =");
    //scanf("%d", &v1);
    //printf("ver2 for contr =");
    //scanf("%d", &v2);

    //if (G2[v1][v2] != 1) {
    //    printf("No rebra");
    //}
    //else {
    //    G2 = contrV(G2, nG2, v1, v2);
    //    nG2--;
    //}

    //printf("\n");
    //printG(G2, nG2);
    //G1 = splitV(G1, nG1, 2);
    //nG1++;
    //printG(G1, nG1);
    //G1 = unionG(G1, G2, nG1, nG2);
    //nG1++;
    //printG(G1, nG1);
    //G1 = xorG(G1, G2, nG1, nG2);
    //printG(G1, nG1 - nG2);
    getchar();
    getchar();
    return 0;

}
