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

## Opciones no funcionales

1. El juego no cuenta con opciones de guardado y continuado de partido, por lo tanto, en caso de finalizar la ejecucion de la aplicacion durante la partida, el progreso durante esa partida se abra perdido.
2. La aplicacion no cuenta con una interfaz grafica, por lo tanto todo su contenido es mostrado en consola.
3. Las interacciones con la aplicacion solo se pueden realizar a travez de entradas del teclado.

**Aplicacion desarrollada por Hans Silva Gonzalez, Benjamin Leiva Caceres, Martin Basulto Nazir, Mario Osvaldo Rojas**