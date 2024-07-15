#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definir el tamaño del laberinto
#define Ancho 15
#define Alto 15

// Definir los posibles movimientos
int dx[] = {0, 1, 0, -1};
int dy[] = {-1, 0, 1, 0};

// Estructura para representar una coordenada
typedef struct
{
    int x;
    int y;
} Coordinate;

// Matriz para registrar los movimientos y contar los pasos
int visited[Alto][Ancho];

// Variables para representar la posición del personaje
Coordinate player = {1, 0};

// Función para inicializar el laberinto con paredes
void initialize_maze(int maze[Alto][Ancho])
{
    for (int y = 0; y < Alto; y++)
    {
        for (int x = 0; x < Ancho; x++)
        {
            maze[y][x] = 1;    // 1 representa una pared
            visited[y][x] = 0; // Inicializar la matriz de visitados
        }
    }
}

// Función para imprimir el laberinto
void print_maze(int maze[Alto][Ancho])
{
    for (int y = 0; y < Alto; y++)
    {
        for (int x = 0; x < Ancho; x++)
        {
            if (player.x == x && player.y == y)
            {
                printf("o ");
            }
            else
            {
                printf(maze[y][x] ? "# " : "  ");
            }
        }
        printf("\n");
    }
}

// Función para verificar si una celda es válida
int is_valid(int x, int y, int maze[Alto][Ancho])
{
    return x > 0 && y > 0 && x < Ancho - 1 && y < Alto - 1 && maze[y][x] == 1;
}

// Función para generar el laberinto usando DFS
void generate_maze(int maze[Alto][Ancho], int x, int y, int steps)
{
    maze[y][x] = 0;        // 0 representa un camino
    visited[y][x] = steps; // Registrar el número de pasos

    // Mezclar los movimientos
    for (int i = 0; i < 4; i++)
    {
        int r = rand() % 4;
        int temp = dx[i];
        dx[i] = dx[r];
        dx[r] = temp;
        temp = dy[i];
        dy[i] = dy[r];
        dy[r] = temp;
    }

    // Intentar moverse en las direcciones aleatorias
    for (int i = 0; i < 4; i++)
    {
        int nx = x + dx[i] * 2;
        int ny = y + dy[i] * 2;

        if (is_valid(nx, ny, maze))
        {
            maze[y + dy[i]][x + dx[i]] = 0;
            generate_maze(maze, nx, ny, steps + 1);
        }
    }
}

// Función para agregar ciclos repetitivos
void add_cycles(int maze[Alto][Ancho])
{
    for (int y = 1; y < Alto - 1; y += 2)
    {
        for (int x = 1; x < Ancho - 1; x += 2)
        {
            if (rand() % 4 == 0)
            {
                int dir = rand() % 4;
                int nx = x + dx[dir];
                int ny = y + dy[dir];
                if (is_valid(nx, ny, maze))
                {
                    maze[ny][nx] = 0;
                }
            }
        }
    }
}

// Función para verificar si el movimiento del personaje es válido
int can_move(int x, int y, int maze[Alto][Ancho])
{
    return x >= 0 && y >= 0 && x < Ancho && y < Alto && maze[y][x] == 0;
}

// Función para mover el personaje
void move_player(char direction, int maze[Alto][Ancho])
{
    int new_x = player.x;
    int new_y = player.y;

    if (direction == 'w')
    {
        new_y -= 1;
    }
    else if (direction == 's')
    {
        new_y += 1;
    }
    else if (direction == 'a')
    {
        new_x -= 1;
    }
    else if (direction == 'd')
    {
        new_x += 1;
    }
    else
    {
        printf("Movimiento inválido\n");
        return;
    }

    if (can_move(new_x, new_y, maze))
    {
        player.x = new_x;
        player.y = new_y;
    }
    else
    {
        printf("Movimiento no permitido\n");
    }
}

int main()
{
    srand(time(NULL));

    int maze[Alto][Ancho];
    initialize_maze(maze);

    // Generar el laberinto
    generate_maze(maze, 1, 1, 0);

    // Agregar ciclos repetitivos
    add_cycles(maze);

    // Establecer la entrada y salida
    maze[1][0] = 0;                // Entrada
    maze[1][1] = 0;                // Asegurarse de que la entrada esté conectada al camino
    maze[Alto - 2][Ancho - 2] = 0; // Salida
    maze[Alto - 2][Ancho - 1] = 0; // Asegurarse de que la salida esté conectada al camino

    // Imprimir el laberinto
    print_maze(maze);

    // Movimiento del personaje
    char move;
    while (player.x != Ancho - 1 || player.y != Alto - 2)
    {
        printf("Introduce movimiento (w/a/s/d): ");
        scanf(" %c", &move);
        move_player(move, maze);
        print_maze(maze);
    }

    // Imprimir el número total de movimientos
    printf("\nNumero total de movimientos desde la entrada hasta la salida: %d\n", visited[Alto - 2][Ancho - 2]);

    return 0;
 }
