#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/stack.h"
#include "tdas/extra.h"
#include "tdas/map.h"
#include "tdas/treemap.h"
#include <string.h>
#include <time.h>

// asdjlfleiwjfeil

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
void infoPartida(ManoCartas *jugador, ManoCartas *maquina, int turnoCrupier)
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
  if (turnoCrupier) // Si es el turno del crupier
    printf("%i\n", maquina->sumaValor);
  else // Si no es el turno del crupier
    printf("?\n");
  // Se muestran las cartas del crupier
  printf("Cartas del Crupier: \n\n");
  carta = (Carta *)list_first(maquina->cartas);
  if (turnoCrupier) // Si es el turno del crupier
  {
    // Se muestran todas sus cartas
    while (carta != NULL)
    {
      printf("  - %s de %s\n", carta->valor, carta->palo);
      carta = (Carta *)list_next(maquina->cartas);
    }
  }
  else // Si no es el turno del crupier
  {
    // Se muestra una carta y se oculta la ultima
    printf("  - %s de %s\n", carta->valor, carta->palo);
    carta = (Carta *)list_next(maquina->cartas);
    while (carta != NULL)
    {
      printf("  - ???\n");
      carta = (Carta *)list_next(maquina->cartas);
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
    mano->sumaValor += 11;
  else
    mano->sumaValor += atoi(carta->valor);
}

// Funcion encargada de crear una nueva mano de cartas
ManoCartas *crearMano()
{
  // Reserva memeoria para la mano de cartas
  ManoCartas *mano = malloc(sizeof(ManoCartas));
  // Inicializa los valores de la mano de cartas
  mano->cartas = list_create();
  mano->sumaValor = 0;

  return mano; // Retorna la mano de cartas
}

// Funcion encargada de revolver una baraja de cartas
void revolverBaraja(Stack *baraja)
{
  /* Se establece una semilla para el generador de numeros aleatorios,
     en este caso se usara la hora local*/
  srand(time(NULL));

  // Inicializa un mapa ordenado para almacenar las cartas
  Map *barajaDesordenada = sorted_map_create(lower_than);

  // Mientras haya cartas en la baraja
  while (stack_top(baraja) != NULL)
  {
    Carta *carta = (Carta *)stack_top(baraja);
    stack_pop(baraja); // Saca la carta de la baraja

    int *num = (int *)malloc(sizeof(int));
    *num = rand() % 1000; // Genera un numero aleatorio entre 1 y 1000

    // Almacena la carta en el mapa ordenado
    map_insert(barajaDesordenada, num, carta);
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
  }
  map_clean(barajaDesordenada); // Limpia el mapa
  free(barajaDesordenada); // Libera la memoria usada por el mapa
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
  printf("Baraja revuelta:\n");
  int i = 1;
  while (stack_top(baraja) != NULL)
  {
    Carta *carta = (Carta *)stack_top(baraja);
    printf("%s %s %i\n", carta->palo, carta->valor, i);
    stack_pop(baraja);
    i++;
  }

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
      scanf("%c", &opcion);

      switch (opcion)
      {
        case '1':
          iniciarPartida();
          break;
        case '2':
          //tutorial();
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