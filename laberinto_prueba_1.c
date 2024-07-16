#include <stdio.h>  // Incluye la biblioteca estándar de entrada y salida
#include <stdlib.h> // Incluye la biblioteca estándar de utilidades
#include <time.h>   // Incluye la biblioteca estándar para manipulación de tiempo
#include <unistd.h> // Incluye la biblioteca estándar para manejo de pausas

// Definir el tamaño del laberinto
#define Ancho 13
#define Alto 13

// Definir los posibles movimientos: arriba, derecha, abajo, izquierda
int dx[] = {0, 1, 0, -1};
int dy[] = {-1, 0, 1, 0};

// Estructura para representar una coordenada
typedef struct
{
    int x; // Coordenada x
    int y; // Coordenada y
} Coordenada;

// Matriz para registrar los movimientos y contar los pasos
int visitado[Alto][Ancho];

// Variable para representar la posición del jugador
Coordenada jugador = {1, 0}; // Inicializa al jugador en la posición (1, 0)

// Función para inicializar el laberinto con paredes
void inicializar_laberinto(int laberinto[Alto][Ancho])
{
    // Itera sobre cada celda del laberinto
    for (int y = 0; y < Alto; y++)
    {
        for (int x = 0; x < Ancho; x++)
        {
            laberinto[y][x] = 1; // 1 representa una pared
            visitado[y][x] = 0;  // Inicializar la matriz de visitados con 0
        }
    }
}

// Función para imprimir el laberinto
void imprimir_laberinto(int laberinto[Alto][Ancho])
{
    // Limpiar la consola antes de imprimir
    printf("\033[H\033[J");

    // Itera sobre cada celda del laberinto
    for (int y = 0; y < Alto; y++)
    {
        for (int x = 0; x < Ancho; x++)
        {
            if (jugador.x == x && jugador.y == y) // Si la posición del jugador coincide con las coordenadas actuales
            {
                printf("* "); // Imprimir la posición del jugador
            }
            else if (visitado[y][x])
            {
                printf(". "); // Imprimir el camino recorrido
            }
            else
            {
                printf(laberinto[y][x] ? "# " : "  "); // Imprimir pared (#) o camino (espacio)
            }
        }
        printf("\n"); // Nueva línea al final de cada fila
    }
}

// Función para verificar si una celda es válida para moverse
int es_valido(int x, int y, int laberinto[Alto][Ancho])
{
    // La celda es válida si está dentro de los límites y es una pared
    return x > 0 && y > 0 && x < Ancho - 1 && y < Alto - 1 && laberinto[y][x] == 1;
}

// Función para generar el laberinto usando DFS
void generar_laberinto(int laberinto[Alto][Ancho], int x, int y, int pasos)
{
    laberinto[y][x] = 0;    // Marcar la celda actual como camino
    visitado[y][x] = pasos; // Registrar el número de pasos

    // Mezclar los movimientos para hacer el laberinto aleatorio
    for (int i = 0; i < 4; i++)
    {
        int r = rand() % 4; // Generar un índice aleatorio
        int temp = dx[i];   // Intercambiar valores en dx
        dx[i] = dx[r];
        dx[r] = temp;
        temp = dy[i]; // Intercambiar valores en dy
        dy[i] = dy[r];
        dy[r] = temp;
    }

    // Intentar moverse en las direcciones aleatorias
    for (int i = 0; i < 4; i++)
    {
        int nx = x + dx[i] * 2; // Calcular nueva coordenada x
        int ny = y + dy[i] * 2; // Calcular nueva coordenada y

        if (es_valido(nx, ny, laberinto)) // Si la nueva celda es válida
        {
            laberinto[y + dy[i]][x + dx[i]] = 0;             // Marcar el camino intermedio como camino
            generar_laberinto(laberinto, nx, ny, pasos + 1); // Recursivamente generar el laberinto desde la nueva celda
        }
    }
}

// Función para agregar ciclos repetitivos al laberinto
void agregar_ciclos(int laberinto[Alto][Ancho])
{
    // Itera sobre celdas alternas del laberinto
    for (int y = 1; y < Alto - 1; y += 2)
    {
        for (int x = 1; x < Ancho - 1; x += 2)
        {
            if (rand() % 4 == 0) // Con probabilidad de 1/4
            {
                int dir = rand() % 4;             // Selecciona una dirección aleatoria
                int nx = x + dx[dir];             // Calcula nueva coordenada x
                int ny = y + dy[dir];             // Calcula nueva coordenada y
                if (es_valido(nx, ny, laberinto)) // Si la nueva celda es válida
                {
                    laberinto[ny][nx] = 0; // Crear un ciclo al convertir una pared en camino
                }
            }
        }
    }
}

// Función para verificar si el movimiento del personaje es válido
int puede_moverse(int x, int y, int laberinto[Alto][Ancho])
{
    // El movimiento es válido si está dentro de los límites y es un camino
    return x >= 0 && y >= 0 && x < Ancho && y < Alto && laberinto[y][x] == 0;
}

// Función para mover al personaje automáticamente usando DFS
int mover_jugador_auto(int laberinto[Alto][Ancho], int x, int y)
{
    if (x == Ancho - 1 && y == Alto - 2) // Si alcanza la salida
    {
        jugador.x = x;
        jugador.y = y;
        return 1; // Indicar que se ha encontrado la salida
    }

    if (!puede_moverse(x, y, laberinto) || visitado[y][x]) // Si el movimiento no es válido o ya se visitó la celda
    {
        return 0; // No se puede continuar por este camino
    }

    visitado[y][x] = 1; // Marcar la celda como visitada
    jugador.x = x;
    jugador.y = y;
    imprimir_laberinto(laberinto); // Imprimir el laberinto después de cada movimiento
    usleep(100000);                // Pausar por 100 milisegundos para que el movimiento sea visible

    // Intentar moverse en las cuatro direcciones
    for (int i = 0; i < 4; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (mover_jugador_auto(laberinto, nx, ny)) // Si se encuentra la salida en alguna dirección
        {
            return 1; // Indicar que se ha encontrado la salida
        }
    }

    visitado[y][x] = 0; // Desmarcar la celda si no se encontró la salida
    return 0;           // No se encontró la salida en este camino
}

int main()
{
    srand(time(NULL)); // Inicializar la semilla para la generación de números aleatorios

    int laberinto[Alto][Ancho];
    inicializar_laberinto(laberinto); // Inicializar el laberinto con paredes

    // Generar el laberinto usando DFS
    generar_laberinto(laberinto, 1, 1, 0);

    // Agregar ciclos repetitivos al laberinto
    agregar_ciclos(laberinto);

    // Establecer la entrada y salida del laberinto
    laberinto[1][0] = 0;                // Entrada
    laberinto[1][1] = 0;                // Asegurarse de que la entrada esté conectada al camino
    laberinto[Alto - 2][Ancho - 2] = 0; // Salida
    laberinto[Alto - 2][Ancho - 1] = 0; // Asegurarse de que la salida esté conectada al camino

    // Imprimir el laberinto inicial
    imprimir_laberinto(laberinto);

    // Mover al jugador automáticamente
    mover_jugador_auto(laberinto, jugador.x, jugador.y);

    // Imprimir el número total de movimientos desde la entrada hasta la salida
    printf("\nNumero total de movimientos desde la entrada hasta la salida: %d\n", visitado[Alto - 2][Ancho - 2]);

    return 0; // Finaliza el programa
}
