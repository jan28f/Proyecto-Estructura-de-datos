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
void tomarCarta(Stack *baraja, ManoCartas *mano, int turno)
{
  // Saca la carta de la baraja y la agrega a la mano del jugador
  Carta *carta = stack_pop(baraja);
  list_pushBack(mano->cartas, carta);

  // Suma el puntaje de la carta a la mano del jugador
  if (!strcmp(carta->valor, "J") || !strcmp(carta->valor, "Q") || !strcmp(carta->valor, "K"))
  {
        mano->sumaValor += 10;
  }
  if (!strcmp(carta->valor, "A"))
  {
    if (turno) // Si es el turno del jugador
    {
      printf("¿Quieres que tu as valga 1 o 11?\n");
      int valorAs;
      scanf("%d", &valorAs);
      while (valorAs != 1 && valorAs != 11)
      {
        printf("Valor inválido, ingresa 1 o 11\n");
        scanf("%d", &valorAs);
      }
      mano->sumaValor += valorAs;
    }
    else // Si es el turno del crupier
    {
      if (mano->sumaValor + 11 > 21)
        mano->sumaValor += 1;
      else
        mano->sumaValor += 11;
    }
  }
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
  //printf("Baraja revuelta con %d cartas:\n", tamañoBaraja);
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

void verificarGanador(ManoCartas *jugador, ManoCartas *crupier)
{
  // Mostrar puntajes finales
  printf("\nPuntaje final del Jugador: %d\n", jugador->sumaValor);
  printf("Puntaje final del Crupier: %d\n", crupier->sumaValor);

  // Mostrar cartas finales del jugador
  printf("\nTus cartas: \n");
  Carta *carta = (Carta *)list_first(jugador->cartas);
  while (carta != NULL)
  {
    printf("  - %s de %s\n", carta->valor, carta->palo);
    carta = (Carta *)list_next(jugador->cartas);
  }

  // Mostrar cartas finales del crupier
  printf("\nCartas del Crupier: \n");
  carta = (Carta *)list_first(crupier->cartas);
  while (carta != NULL)
  {
    printf("  - %s de %s\n", carta->valor, carta->palo);
    carta = (Carta *)list_next(crupier->cartas);
  }

  // Verificar resultados
  if (jugador->sumaValor > 21)
  {
    printf("\nTe has pasado de 21. El Crupier gana.\n");
  }
  else if (crupier->sumaValor > 21)
  {
    printf("\nEl Crupier se ha pasado de 21. ¡Tú ganas!\n");
  }
  else if (jugador->sumaValor == 21) 
  {
     printf("\n¡Blackjack! Has ganado.\n");
  }    
  else if (crupier->sumaValor == 21) 
  {
    printf("\nEl crupier ha conseguido un Blackjack. Has perdido.\n");
  } 
  else if (jugador->sumaValor > crupier->sumaValor)
  {
    printf("\n¡Tú ganas con %d contra %d del Crupier!\n", jugador->sumaValor, crupier->sumaValor);
  }
  else if (jugador->sumaValor < crupier->sumaValor)
  {
    printf("\nEl Crupier gana con %d contra %d tuyos.\n", crupier->sumaValor, jugador->sumaValor);
  }
  else
  {
    printf("\nEs un empate con %d puntos.\n", jugador->sumaValor);
  }
}

/* Funcion encargada de liberar toda la memoria dinamica reservada en las variables
   barajas, manoJugador y manoCrupier*/
void liberarRecursos(Stack *baraja, ManoCartas *manoJugador, ManoCartas *manoCrupier)
{
  // Liberar la memoria de las cartas en la baraja
  stack_clean(baraja);
  free(baraja);
  // Liberar la memoria de las cartas en la mano del jugador
  list_clean(manoJugador->cartas);
  free(manoJugador->cartas);
  free(manoJugador);
  // Liberar la memoria de las cartas en la mano del crupier
  list_clean(manoCrupier->cartas);
  free(manoCrupier->cartas);
  free(manoCrupier);
}

void iniciarPartida()
{
  // Inicializa la baraja de cartas inglesas y la revuelve
  Stack *baraja = iniciarBaraja();
  revolverBaraja(baraja);

  // Inicializa las manos del jugador y del crupier
  ManoCartas *manoJugador = crearMano();
  ManoCartas *manoCrupier = crearMano();

  // Reparte las cartas iniciales al jugador y al crupier
  for (int i = 0; i < 2; i++)
  {
    // Saca una carta de la baraja y la almacena en la mano del jugador
    tomarCarta(baraja, manoJugador, 1);
    // Saca una carta de la baraja y la almacena en la mano del crupier
    tomarCarta(baraja, manoCrupier, 0);
    infoPartida(manoJugador, manoCrupier, 0);
  }

  // Verifica si las dos primeras cartas del jugador suman 21 (BlackJack)
  if (manoJugador->sumaValor == 21)
  {
    infoPartida(manoJugador, manoCrupier, 0);
    printf("\n¡BlackJack! ¡Ganaste con 21!\n");
    // Se libera la memoria dinamica reservada durante la partida
    liberarRecursos(baraja, manoJugador, manoCrupier);
    return;
  }

  char opcion;
  do
  {
    infoPartida(manoJugador, manoCrupier, 0);
    puts("Opciones del jugador:");

    puts("1) Tomar carta");
    puts("2) Mantener");
    puts("3) Retirarse");

    printf("Ingrese su opción: ");
    getchar(); // Consume el '\n' del buffer de entrada
    scanf(" %c", &opcion);

    switch (opcion)
    {
      case '1':
        tomarCarta(baraja, manoJugador, 1);
        if (manoJugador->sumaValor >= 21)
        {
          verificarGanador(manoJugador, manoCrupier);
          // Se libera la memoria dinamica reservada durante la partida
          liberarRecursos(baraja, manoJugador, manoCrupier);
          return;
        }
        break;
      case '2':
        printf("\nHas mantenido tu mano. Comienza el turno del crupier.\n");
        while (manoCrupier->sumaValor < 17)
        {
          tomarCarta(baraja, manoCrupier, 0);
        }
        verificarGanador(manoJugador, manoCrupier);
        // Se libera la memoria dinamica reservada durante la partida
        liberarRecursos(baraja, manoJugador, manoCrupier);
        return;
      case '3':
        printf("\nTe has retirado de la partida. Pierdes.\n");
        return;
      default:
        printf("\nOpción inválida. Inténtalo de nuevo.\n");
    }
  } while (opcion != '2');
}

// Funcion encargada de leer y mostrar el archivo de texto con las instrucciones del juego
void mostrarTutorial()
{
  limpiarPantalla();
  // Se abre el archivo de texto con las instrucciones del juego
  FILE *archivo = fopen("Instrucciones.txt", "r");
  if (archivo == NULL) // Si no se encuentra el archivo de texto
  {
    printf("Error al abrir el archivo del tutorial.\n");
    return; // Se finaliza la funcion
  }

  // Variable para almacenar el contenido de una linea del archivo de texto
  char linea[256];
  /* Se consigue una linea del archivo de texto y se imprime en pantalla,
     hasta que no haya mas lineas*/
  while (fgets(linea, sizeof(linea), archivo) != NULL) {
    printf("%s", linea);
  }
  printf("\n");
  
  fclose(archivo); // Se cierra el archivo de texto
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
