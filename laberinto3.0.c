#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // para la función sleep()

#define tamaño 13 // tamaño del laberinto
#define pared '0' // paredes del laberinto
#define camino ' ' // camino para recorrer el laberinto
#define inicio 'A' // comienzo del laberinto
#define fin 'B' // final del laberinto
#define recorrido '.' // camino recorrido hasta el final

// estructura que define las coordenadas en el laberinto
typedef struct {
    int x, y;
} coordenadas;

// declara la matriz del laberinto
char laberinto[tamaño][tamaño];
int movimientos = 0; // contador de movimientos

// función que crea las paredes del laberinto
void CrearParedes() {
    for (int i = 0; i < tamaño; i++) {
        for (int j = 0; j < tamaño; j++) {
            laberinto[i][j] = pared;
        }
    }
}

// función para presentar el laberinto
void PresentarLaberinto() {
    for (int i = 0; i < tamaño; i++) {
        for (int j = 0; j < tamaño; j++) {
            printf("%c  ", laberinto[i][j]);
        }
        printf("\n");
    }
}

// función que crea los caminos del laberinto
void CrearCaminos(int x, int y) {
    int direccion[] = {0, 1, 2, 3}; // arreglo que representa las direcciones
    for (int i = 0; i < 4; i++) {
        int aleatorio = rand() % 4;
        int temp = direccion[aleatorio];
        direccion[aleatorio] = direccion[i];
        direccion[i] = temp;
    }
    for (int i = 0; i < 4; i++) {
        int act_x = x, act_y = y; // posición actual de la creación de camino en el laberinto
        switch (direccion[i]) {
            case 0: act_x = x + 2; break;
            case 1: act_y = y + 2; break;
            case 2: act_x = x - 2; break;
            case 3: act_y = y - 2; break;
        }
        if (act_x > 0 && act_x < tamaño - 1 && act_y > 0 && act_y < tamaño - 1 && laberinto[act_y][act_x] == pared) {
            laberinto[act_y][act_x] = camino;
            laberinto[act_y - (act_y - y) / 2][act_x - (act_x - x) / 2] = camino;
            CrearCaminos(act_x, act_y);
        }
    }
}

// función que crea los caminos dentro del laberinto
void GenerarLaberinto() {
    laberinto[1][1] = camino;
    CrearCaminos(1, 1);
    laberinto[0][1] = inicio;
    laberinto[tamaño - 1][tamaño - 2] = fin;
}

// función para resolver automáticamente el laberinto
int Resolver(int x, int y) {
    // si las coordenadas se encuentran fuera de lugar, devuelve un valor de 0
    if (x < 0 || y < 0 || x >= tamaño || y >= tamaño) {
        return 0;
    }
    // si se encuentra el final en las coordenadas, devuelve un valor de 1
    if (laberinto[y][x] == fin) {
        return 1;
    }
    // si la coordenada no es ni el inicio ni un camino, devuelve un valor de 0
    if (laberinto[y][x] != camino && laberinto[y][x] != inicio) {
        return 0;
    }
    // marca la posición actual como parte de la solución
    laberinto[y][x] = recorrido;
    movimientos++; // incrementa el contador de movimientos
    // presenta el laberinto después de cada movimiento
    system("clear"); // limpia la pantalla
    PresentarLaberinto();
    sleep(5); // espera 5 segundos

    // intenta moverse hacia las 4 direcciones y si alguna devuelve un valor de 1, la condición también lo hará
    if (Resolver(x + 1, y) || Resolver(x, y + 1) || Resolver(x - 1, y) || Resolver(x, y - 1)) {
        return 1;
    }
    // si ninguna condición se cumple, se borrará el camino realizado y se devolverá un valor de 0
    laberinto[y][x] = camino;
    movimientos--; // decrementa el contador de movimientos si retrocede
    // presenta el laberinto después de cada retroceso
    system("clear"); // limpia la pantalla
    PresentarLaberinto();
    sleep(1); // espera 5 segundos
    return 0;
}

// función principal
int main() {
    srand(time(NULL)); // genera un número aleatorio
    CrearParedes(); // llama a la función para crear las paredes del laberinto
    GenerarLaberinto(); // llama a la función para generar un laberinto aleatorio
    printf("LABERINTO GENERADO ALEATORIAMENTE:\n\n"); // genera un mensaje para presentar el laberinto
    PresentarLaberinto(); // llama a la función para presentar el laberinto generado
    // la condición llama a la función para tratar de resolver el laberinto
    if (Resolver(1, 1)) { // si la función devuelve el valor de 1:
        printf("\n\nLABERINTO RESUELTO\n\n"); // genera un mensaje que presenta el laberinto resuelto
        PresentarLaberinto(); // llama a la función para presentar el laberinto ya resuelto
        printf("Movimientos realizados: %d\n", movimientos); // muestra el número de movimientos realizados
    } else { // de otro modo:
        printf("\n\nNO SE PUEDE RESOLVER EL LABERINTO\n"); // genera un mensaje avisando que no se puede resolver el laberinto
    }
    return 0;
}

