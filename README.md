# Juego "Blackjack" o "21" en C

Esta aplicacion es una implementacion en el lenguaje de programacion C del juego de cartas tipico de casino "Blackjack", tambien conocido como "21".

En el Blackjack, nuestro objetivo es ir sacando de una baraja revuelta y sumando sus puntajes para obtener 21 puntos o el mas proximo a este. El rival en nuestra partida sera siempre el Crupier (en este caso la computadora), el comenzara a jugar una vez el jugador termine su turno, que es cuando termina de sacar cartas, el crupier esta sujeto a reglas fijas y comenzara a sacar hasta que su puntaje sea mayor o igual a 17 puntos. El ganador se determina de las siguientes formas, gana instantaneamente quien consiga 21 puntos mientras este sacando cartas, pierde instantaneamente quien tenga mas de 21 puntos mientras este sacando cartas, sino ocurre ninguna de las situaciones anteriormente descritas el ganador sera quien su puntaje este mas cerca del 21 y sea mayor que el de su rival, en caso de tener puntajes iguales termina la partida con un empate.

## Reglas del juego

A continuacion se explicaran las reglas basicas para poder jugar Blackjack:

1. El objetivo del juego es obtener una mano con un valor lo más cercano posible a 21, sin pasarse.
2. Las cartas numéricas (2-10) valen su valor nominal.
3. Las cartas con figuras (J, Q, K) valen 10 puntos cada una.
4. El valor de los Ases puede variar segun la conveniencia de a quien le toque jugar, en el caso del jugador puede escoger que su valor sea entre 1 u 11 segun le convenga, mientras que al Crupier los Ases valdran 11 si a la suma de su puntaje no supera los 21 puntos, en caso contrario valdra 1.
5. Quien supere los 21 puntos en su mano, pierde automaticamente.
6. El usuario puede optar por 'Pedir' una nueva carta o 'Mantenerse' para mantener tu mano actual.
7. El crupier debe seguir ciertas reglas para comenzar a pedir cartas (esperar hasta que el jugador mantenga sus cartas).
8. Si ambos jugadores tienen manos que no pasan de 21, gana el que tenga la mano con el valor más cercano a 21.
9. Si ambos jugadores tienen manos con el mismo puntaje, es un empate.

## Como compilar y ejecutar la aplicacion
Para poder ejecutar la aplicacion, primero, tendremos que iniciar la consola y en esta dirigirnos a la carpeta de la aplicacion (o carpeta raíz de la aplicacion). Luego compilaremos el archivo main.c con el siguiente comando:
````
gcc tdas/*.c main.c -Wno-unused-result -o main
````

Compilada la aplicacion, la ejecutaremos ingresando el siguiente comando en consola:
````
./main
````

## Instrucciones  de uso

Al ejecutar la aplicacion, se presentara el menu con las siguientes opciones para que el usuario escoger:

1. Iniciar partida: Comienza una nueva partida de Blackjack.
2. Como jugar: Se explicaran las reglas y como jugar el Blackjack.
3. Salir: Se finalizara la ejecucion de la aplicacion.

Si el jugador escogio la opcion `Iniciar partida`, al jugador durante la partida se le presentara las siguientes opciones:

1. Tomar carta: Añade una nueva carta a su mano y se suma su puntaje.
2. Mantener: El jugador termina su turno y comienza a jugar el crupier.
3. Retirarse: El jugador se retira de la partida perdiendo.

## Implementacion

La aplicacion fue implementada con las siguientes funciones claves para su funcionamiento:

### mostrarTutorial()

Cuando se necesite leer y mostrar las reglas del juego, contenidas en el archivo `instrucciones.txt`, el usuario seleccionara la opcion `Como jugar` la cual llamara a la funcion `mostrarTutorial()`. Esta funcion no recibe nada como argumento:

1. La funcion abre el archivo de texto `instrucciones.txt` y lo almacena en una variable.
2. Si no se encontro el archivo, se muestra un mensaje de error y se finaliza la funcion.
3. Si se encuentra el archivo, la funcion en un bucle consigue una linea del archivo y la imprime, hasta haber leido y imprimido todas las lineas del archivo.
4. Se cierra el archivo una vez terminado el bucle.

Ejemplo:
```c
mostrarTutorial();
```

### iniciarPartida()

La funcion encargada de iniciar una partida de Blackjack y mantener su flujo es la funcion `iniciarPartida()`, la cual se inicia cuando el usuario selecciona la opcion `Iniciar partida`. Esta funcion no recibe nada como argumento:

1. La funcion inicializa una baraja con la funcion `iniciarBaraja` y la almacena en una variable.
2. La funcion revuelve la baraja con la funcion `revolverBaraja`.
3. En 2 variables, una para el jugador y otra para el crupier, se crea e inicializan sus manos de cartas con la funcion `crearMano`.
4. En un bucle con 2 iteraciones, se llama 2 veces a la funcion `tomarCarta` para agregar una carta a la mano del jugador primero, y despues agregar una carta al crupier.
5. Se verifica si tras el reparto de las 2 cartas iniciales alguien consiguio 21 puntos, en caso de alguien haberlo conseguido, se muestra el ganador y se finaliza la partida.
6. En caso contrario, se despliega un menu con las siguientes 3 opciones para el usuario, **Tomar carta**, **Mantenerse** y **Retirarse**.
7. Si el jugador escoge la opcion **Retirarse**, se muestra un mensaje de derrota y se finaliza el programa.
8. Si el jugador escoge la opcion **Tomar carta**, se llama a la funcion `tomarCarta` y posteriormente se verifica si supero los 21 puntos, en caso de superarlos se muestra un mensaje de derrota y se finaliza la partida.
9. Si el jugador escoge la opcion **Mantenerse**, en un bucle se comienza a llamar a la funcion `tomarCarta` para la mano del crupier, este bucle finalizara cuando el crupier tenga 17 o mas puntos, se llamara a la funcion `verificarGanador` y se finalizara la partida.

Ejemplo:
```c
iniciarPartida();
```

### iniciarBaraja()

Esta funcion se encarga de crear y retorna una baraja de cartas inglesas sin comodines. No recibe ningun argumento, y retorna un puntero de tipo Stack:

1. La funcion inicializa un stack.
2. La funcion inicializa un arreglo de tipo char que contendra los palos de carta de una baraja inglesa.
3. La funcion inicializa un arreglo de tipo char que contendra los valores de carta de una baraja inglesa.
4. Se inicializa un bucle anidado en donde se recorre, primero, el arreglo de palos, y segundo el arrelgo de valores.
5. En cada iteracion del bucle anidado se reserva memoria para una variable de tipo Carta, y se le asigna el palo y valor.
6. Se apila la carta en el stack.
7. Finalizado el bucle, se retorna el stack.

Ejemplo:
```c
Stack baraja = iniciarBaraja();
```

### revolverBaraja(Stack *baraja)

Esta funcion desorneda el contenido de una baraja. Recibe como argumento una variable de tipo Stack, y no retorna nada:

1. La funcion establece como semilla de generacion de numeros aleatorios la hora local, usando la funcion `srand()`, y pasando de argumento la hora local con la funcion `time(NULL)`, `NULL` siendo el argumento para que la funcion `time()` nos retorne la hora local.
2. Se inicializa un mapa ordenado, un arreglo y contador de tipo int.
4. Se inicia un bucle un bucle el cual funcionara mientras se encuentre un elemento en el stack.
5. Se desapila y almacena un elemento en el stack y se genera un numero aleatorio.
6. Se verifica si el numero aleatorio esta en el arreglo, si lo esta se genera un nuevo numero aleatorio y se repite el proceso de verificacion.
7. Si no se encuentra el numero aleatorio en arreglo, se inserta el elemento en el mapa ordenado usando de clave el numero aleatorio.
8. Finalizado el bucle, se inicia un bucle que funcionara mientras se encuentre elementos en el mapa ordenado.
9. Se consigue el primer elemento del mapa ordenado y se agrega a la baraja.
10. Finalizado el bucle se libera la memoria ocupada por el mapa ordenado.

Ejemplo:
```c
Stack baraja = iniciarBaraja();
revolverBaraja(baraja);
```

### crearMano()

Esta funcion se encarga de crear, inicializar y retornar una mano de cartas para registrar las cartas y puntajed de los jugadores en una partida de Blackjack. No recibe nada como argumento, y retorna un puntero de tipo ManoCartas:

1. Se reserva memoria memoria para una variable de tipo ManoCartas y se almacena en una variable.
2. Se inicializa la lista de cartas de la mano.
3. Se inicializa la variable sumaValor en 0 de la mano.
4. Retorna la mano ya inicializada.

Ejemplo:
```c
ManoCartas mano = crearMano();
```

### tomarCarta(Stack *baraja, ManoCartas *mano, int turno)

Esta funcion se encarga de agregar una carta de una baraja a la mano correspondiente. Esta funcion recibe 3 argumentos la baraja de cartas, la mano a que se agregara la carta y un numero 0 o 1, siendo 1 el turno del jugador y 0 si es el turno del crupier. Esta funcion no retorna nada:

1. Se desapila una carta de la baraja y se almacena en una variable.
2. Si el valor de la carta es "J", "Q" o "K", se agrega la carta a la lista de cartas de la mano y se le suman 10 puntos. En caso contrario se pasa a la sigiuiente codicional.
3. Si el valor de la carta es "A", si turno es 1 se preguntara si el valor de la carta quiere que sea 1 o 11, agregando al carta a lista de cartas de la mano y sumando el puntaje escogido.
4. En caso de turno ser 0, se agregara a la carta a lista de cartas de la mano y se sumara 11 puntos si el puntaje final mas 11 no supera los 21 puntos, en caso contrario se sumara 1.
5. Si el valor de la carta es un numero entre 2 y 10, se agregara la carta a lista de cartas de la mano y al puntaje se sumara el valor indicado en la carta.

Ejemplo:
```c
Stack baraja = iniciarBaraja();
revolverBaraja(baraja);

ManoCartas mano = crearMano();

tomarCarta(baraja, mano, 1);
```

### verificarGanador(ManoCartas *jugador, ManoCartas *crupier)

Esta funcion se encarga de de comparar los puntajes del jugador y del crupier y determinar quien gano la partida. Esta funcion recibe como argumento 2 variables de tipo ManoCartas, primero la mano del jugador, segundo la mano del crupier. Esta funcion no retorna nada:

1. La funcion imprime el puntaje final del jugador.
2. La funcion imprime la lista de cartas de la mano del jugador.
3. La funcion imprime le puntaje final del crupier.
4. La funcion imprime la lista de cartas del crupier.
5. Se compara si la mano del jugador es mayor a 21, si lo es, el jugador perdio la partida. En caso contrario se pasa a la siguiente condicional.
6. Se compara si la mano del crupier es mayor a 21, si lo es, el jugador gano la partida. En caso contrario se pasa a la sigiuente condicional.
7. Se compara si la mano del jugador es igual a 21, si lo es, el jugador gano la partida. En caso contrario se pasa a la siguiente condicional.
8. Se compara si la mano del crupier es igual a 21, si los es, el jugador perdio la partida. En caso contrario se pasa a la siguiente condicional.
9. Se compara si la mano del jugador es mayor a la del crupier, si lo es, el jugador gana la partida. En caso contrario se pasa a la siguiente condicional.
10. Se compara si la mano del crupier es mayor a la del jugador, si lo es, el jugador perdio la partida. En caso contrario se pasa a la siugiente condicional.
11. Si no se cumplio ninguna de las condiciones anteriores, es un empate.

Ejemplo:
```c
Stack baraja = iniciarBaraja();
revolverBaraja(baraja);

ManoCartas manoJugador = crearMano();
ManoCartas manoCrupier = crearMano();

tomarCarta(baraja, manoJugador, 1);
tomarCarta(baraja, manoJugador, 0);

verificarGanador(manoJugador, manoCrupier);
```

## Opciones no funcionales

1. El juego no cuenta con opciones de guardado y continuado de partido, por lo tanto, en caso de finalizar la ejecucion de la aplicacion durante la partida, el progreso durante esa partida se abra perdido.
2. La aplicacion no cuenta con una interfaz grafica, por lo tanto todo su contenido es mostrado en consola.
3. Las interacciones con la aplicacion solo se pueden realizar a travez de entradas del teclado.

## Consideraciones

1. Para el funcionamiento correcto de la opcion `Como jugar`, el programa requiere del archivo de texto `instrucciones.txt`.
2. Para la correcta compilacion y ejecucion del programa se debe revisar que este presente el directorio `./tdas`.

## Contribuciones

### Hans

1. Ideacion de las estructuras **Carta** y **ManoCartas** junto con Martin y implementacion junto con Mario.
2. Ideacion de las funciones junto con Benjamin.
3. Implementacion de las funciones del juego.
4. Ideacion y implementacion de los menus de juego junto con Benjamin.
5. Redaccion del archivo README.

### Martin

1. Ideacion de las estructuras **Carta** y **ManoCartas** junto con Hans.
2. Ideacion y implementacion de la funcion mostrarTutorial y la lectura del archivo de texto.
3. Implementacion de las funciones del juego.

### Benjamin

1. Implementacion de las funciones principales junto con Hans.
2. Ideacion de las funciones junto con Hans.
3. Ideacion y implementacion de los menus de juego junto a Hans.
4. Implementacion de las funciones del juego.

### Mario

1. Implementacion de las estructuras de datos **Carta** y **ManoCartas** junto con Hans.
2. Encargado de encontrar los errores durante las implementaciones y solucionarlos.
3. Implementacion de las funciones del juego.

**Aplicacion desarrollada por Hans Silva Gonzalez, Benjamin Leiva Caceres, Martin Basulto Nazir, Mario Osvaldo Rojas**