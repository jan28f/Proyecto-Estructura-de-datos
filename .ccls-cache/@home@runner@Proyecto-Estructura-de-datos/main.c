#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/stack.h"
#include "tdas/extra.h"
#include "tdas/map.h"
#include "tdas/treemap.h"
#include <string.h>
#include <time.h>

// Estructura de una carta
typedef struct
{
  char palo[12];
  char valor[3];
} Carta;

// Estructura de una mano de un jugador
typedef struct
{
  List *cartas;
  int sumaValor;
  int numAses;
} ManoCartas;

// Funcion para comparar el valor de 2 claves
int lower_than(void* key1, void* key2)
{
    int k1 = *((int*) (key1));
    int k2 = *((int*) (key2));
    return k1 < k2;
}

/* Funcion para mostrar en consola el estado de la partida al jugador,
   mostrando sus cartas y puntaje y la informacion disponible del crupier*/
void infoPartida(ManoCartas *jugador, ManoCartas *crupier, int turnoCrupier)
{
  limpiarPantalla();
  printf("===============================\n");
  printf("========== BLACKJACK ==========\n");
  printf("===============================\n\n");
  
  // Se muestra el puntaje del jugador
  
  printf("Tu puntaje: %i\n", jugador->sumaValor);
  printf("Tus cartas: \n\n");
  
  // Se muestran las cartas del jugador
  Carta *carta = (Carta *)list_first(jugador->cartas);
  while (carta != NULL)
  {
    printf("  - %s de %s\n", carta->valor, carta->palo);
    carta = (Carta *)list_next(jugador->cartas);
  }
  printf("\n");
  
  printf("Puntaje del Crupier: ");
  if (turnoCrupier) 
    // Si es el turno del crupier
    printf("%i\n", crupier->sumaValor);
  else 
    // Si no es el turno del crupier
    printf("?\n");
  
  // Se muestran las cartas del crupier
  printf("Cartas del Crupier: \n\n");
  carta = (Carta *)list_first(crupier->cartas);
  
  if (turnoCrupier) // Si es el turno del crupier
  {
    // Se muestran todas sus cartas
    while (carta != NULL)
    {
      printf("  - %s de %s\n", carta->valor, carta->palo);
      carta = (Carta *)list_next(crupier->cartas);
    }
  }
    
  else // Si no es el turno del crupier
  {
    // Se muestra una carta y se oculta la ultima
    printf("  - %s de %s\n", carta->valor, carta->palo);
    carta = (Carta *)list_next(crupier->cartas);
    while (carta != NULL)
    {
      printf("  - ???\n");
      carta = (Carta *)list_next(crupier->cartas);
    }
  }
  printf("\n");
}

// Funcion encargada de sacar una carta de la baraja y agregarla a una mano
void tomarCarta(Stack *baraja, ManoCartas *mano)
{
  // Saca la carta de la baraja y la agrega a la mano del jugador
  Carta *carta = stack_pop(baraja);
  list_pushBack(mano->cartas, carta);

  // Suma el puntaje de la carta a la mano del jugador
  if (!strcmp(carta->valor, "J") || !strcmp(carta->valor, "Q") || !strcmp(carta->valor, "K"))
    mano->sumaValor += 10;
  else if (!strcmp(carta->valor, "A"))
  {
    mano->sumaValor += 11;
    mano->numAses +=1; // Incrementa el número de Ases
  }
  else
    mano->sumaValor += atoi(carta->valor);

  // Ajusta el valor de los Ases si la suma es mayor que 21
  while (mano->sumaValor > 21 && mano->numAses > 0) 
  {
    mano->sumaValor -= 10;
    mano->numAses -= 1;
  }
}

// Funcion encargada de crear una nueva mano de cartas
ManoCartas *crearMano()
{
  // Reserva memeoria para la mano de cartas
  ManoCartas *mano = malloc(sizeof(ManoCartas));
  // Inicializa los valores de la mano de cartas
  mano->cartas = list_create();
  mano->sumaValor = 0;
  mano->numAses = 0;

  return mano; // Retorna la mano de cartas
}

// Función para comprobar si una clave ya está en el arreglo, osea, que ya está en el mapa
int claveExistente(int *arreglo, int tamaño, int num) {
  for (int i = 0; i < tamaño; i++) {
    if (arreglo[i] == num) {
      return 1;
    }
  }
  return 0;
}

// Funcion encargada de revolver una baraja de cartas
void revolverBaraja(Stack *baraja)
{
  //Se establece una semilla para el generador de numeros aleatorios, en este caso se usara la hora local
  srand(time(NULL));
  int tamañoBaraja = 0;

  // Inicializa un mapa ordenado para almacenar las cartas
  Map *barajaDesordenada = sorted_map_create(lower_than);
  int *clavesUsadas = malloc(sizeof(int) * 52);
  int contadorClaves = 0;

  // Mientras haya cartas en la baraja
  while (stack_top(baraja) != NULL)
  {
    Carta *carta = (Carta *)stack_pop(baraja);

    int *clave = (int *)malloc(sizeof(int));
    do {
      *clave = rand() % 1000;
    } while (claveExistente(clavesUsadas, contadorClaves, *clave)); // Asegura que la clave sea unica

    clavesUsadas[contadorClaves++] = *clave;

    map_insert(barajaDesordenada, clave, carta);
  }

  // Mientras haya cartas en el mapa ordenado
  while (map_first(barajaDesordenada) != NULL)
  {
    // Consigue la primera carta de la baraja desordenada
    Carta *carta = (Carta *)map_first(barajaDesordenada)->value;
    // Elimina la primera carta de la baraja desordenada
    map_remove(barajaDesordenada, map_first(barajaDesordenada)->key);
    // La agrega a la baraja
    stack_push(baraja, carta);
    tamañoBaraja++;
  }
  map_clean(barajaDesordenada);
  free(barajaDesordenada);
  free(clavesUsadas);
  printf("Baraja revuelta con %d cartas:\n", tamañoBaraja);
}

// Funcion encargada de retornar un stack con todas las cartas de una baraja inglesa
Stack *iniciarBaraja()
{
  Stack *stack = stack_create(stack); // Correct stack creation
  char *palos[4] = {"Corazon", "Diamante", "Trebol", "Pica"};
  char *valor[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

  for (int i = 0 ; i < 4 ; i++) // Recorre los palos
    for (int k = 0 ; k < 13 ; k++) // Recorre los valores
    {
      // Crear una carta y la almacena en el stack
      Carta *carta = (Carta *)malloc(sizeof(Carta));
      strcpy(carta->palo, palos[i]);
      strcpy(carta->valor, valor[k]);
      stack_push(stack, carta);
    }

  return stack; // Retorna el stack con todas las cartas de la baraja
}

void iniciarPartida()
{
  limpiarPantalla();

  // Inicializa la baraja de cartas inglesas y la revuelve
  Stack *baraja = iniciarBaraja();
  revolverBaraja(baraja);
  
  int i = 1;
  while (stack_top(baraja) != NULL)
  {
    Carta *carta = (Carta *)stack_pop(baraja);
    printf("%s %s %i\n", carta->palo, carta->valor, i);
    i++;
  }

  // Inicializa las manos del jugador y de la maquina
  ManoCartas *manoJugador = crearMano();
  ManoCartas *manoMaquina = crearMano();

  // Reparte las cartas iniciales al jugador y a la maquina
  for (int i = 0 ; i < 2 ; i++)
  {
    // Saca una carta de la baraja y la almacena en la mano del jugador
    tomarCarta(baraja, manoJugador);
    // Saca una carta de la baraja y la almacena en la mano de la maquina
    tomarCarta(baraja, manoMaquina);
  }

  printf("jugador: %i\n", manoJugador->sumaValor);
  printf("maquina: %i\n", manoMaquina->sumaValor);
  //Muestra baraja revuelta
  /*printf("Baraja revuelta:\n");
  int i = 1;
  Carta *carta = (Carta *)stack_top(baraja);
  while (carta != NULL)
  {
    //Carta *carta = (Carta *)stack_top(baraja);
    printf("%s %s %i\n", carta->palo, carta->valor, i);
    //stack_pop(baraja);
    carta = (Carta *)list_next(baraja);
    i++;
  }*/

  /*
  char opcion;
  do
  {
    infoPartida(manoJugador, manoMaquina, 0);
    puts("Opciones del jugador:");

    puts("1) Tomar carta");
    puts("2) Mantener");
    puts("3) Retirarse");
    
    printf("Ingrese su opción: ");
    getchar(); // Consume el '\n' del buffer de entrada
    scanf("%c", &opcion);

    switch (opcion)
    {
      case '1':
        tomarCarta(baraja, manoJugador);
        printf("Se ha agregado una carta a tu mano\n");
        break;
      case '2':
        printf("Has mantenido tu mano\n");
        printf("Comienza el turno del crupier\n");
        break;
      case '3':
        printf("Has retirado de la partida, perdiste\n");
        return;
    }
  } while (opcion != '2'); */
  // Muestra las cartas iniciales del jugador y de la maquina
}

void mostrarTutorial() {
  FILE *archivo = fopen("tutorial.txt", "r");
  if (archivo == NULL) {
    printf("Error al abrir el archivo de tutorial.\n");
    return;
  }
  char linea[256];
  while (fgets(linea, sizeof(linea), archivo) != NULL) {
    printf("%s", linea);
  }
  printf("\n");
  fclose(archivo);
}

int main()
{
  char opcion;
    do
    {
      puts("========================================");
      puts("     BlackJack - Menu de opciones       ");
      puts("========================================");

      puts("1) Iniciar partida");
      puts("2) Como jugar");
      puts("3) Salir");

      printf("\nIngrese su opción: ");
      scanf(" %c", &opcion);

      switch (opcion)
      {
        case '1':
          iniciarPartida();
          break;
        case '2':
          mostrarTutorial();
          break;
        case '3':
          printf("Saliendo del programa...\n");
          break;
        default:
          printf("Ingrese una opción válida\n");
      }
      presioneTeclaParaContinuar();
      limpiarPantalla();

  } while (opcion != '3');

  return 0;
}
