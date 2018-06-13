/* Programa que utiliza el algoritmo de Dijsktra para encontrar el
 * camino más corto o económico entre dos puntos de un grafo
 * no dirigido.
 */
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
using namespace std;
/* Definiendo la estructura etiqueta de nodo. Sus campos incluyen
 * el n'umero de nodo, el coste total del nodo, y su precedente. */
struct label {
	int nro;	/* numero del nodo */
	int prev;	/* nodo precedente (-1 para el nodo inicial )*/
	int peso;	/* peso o coste total de la trayectoria que
				 * conduce al nodo, i.e., el coste total desde
				 * el nodo inicial hasta el actual. Un valor
				 * de -1 denota el infinito */
	int marca;	/* si el nodo ha sido marcado o no */
};
typedef struct label label_t;
 
/* Cola de Prioridad de n'umeros de nodo */
struct node {
	int data;
	struct node * nextPtr;		/* apuntador al siguiente nodo de la cola */
};
typedef struct node node_t;
 
int isEmpty( node_t * );		/* decide si cola es vac'ia */
int insert( node_t **, int, label_t * );	/* agrega un nodo ordenadamente a la cola */
void quit( node_t **, int );	/* retira un nodo espec'ifico de la cola */
int pop( node_t ** );			/* retira el primer nodo de la cola */
void printQueue( node_t * );	/* imprimir los valores almacenados en la cola */
void llenarMatriz(int **m, int cant); //llena la matriz
void leer (int &n); //lee un numero entero, pide otra vez si el dato no es valido
void mostrarMatriz(int **m, int cant); // muestra la matriz
void warshall (int **m, int num); 
/* Funci'on que calcula la distancia m'inima de cualquiera de los 
 * nodos al primero */
void dijkstra( int, int **, int, int );

int main () {
 
	int i, j;
 
	/* cantidad total de nodos */
	cout<<"ingrese la cantidad de nodos: ";
	int numNodos;
	int primerNodo, ultimoNodo;
	leer(numNodos);
	//int numNodos = 8;
	/* Definiendo la matriz de distancia */
	int **A;
	if ( ( A = (int **) malloc( numNodos * sizeof(int *) ) ) == NULL ) return 1;
	for ( i = 0; i < numNodos; i++ )
		if ( ( A[i] = (int *) malloc( numNodos * sizeof(int) ) ) == NULL ) return 1;
 
	for ( i = 0; i < numNodos; i++ )
		for ( j = 0; j < numNodos; j++ )
			A[i][j] = 0;
	cout<<endl;
 	llenarMatriz(A,numNodos);
 	cout<<endl;
	/* Imprimir la matriz de distancia */
	cout<< "Matriz de distancia:\n\n";
	mostrarMatriz(A,numNodos);
	cout<<endl;
 	
 	cout<<"Pasamos la matriz por el algoritmo de warshall"<<endl;
 	warshall(A,numNodos);
 	mostrarMatriz(A,numNodos);
 	cout<<endl<<endl;
 	
 	bool exito = false;
 	cout<<"Ingrese el nodo inicial: ";
 	do{
 		exito = false;
 		leer(primerNodo);
 		if (primerNodo<0 || primerNodo>=numNodos){
 			cout<<"El nodo no existe"<<endl;
 			cout<<"Ingrese nuevamente el nodo inicial: ";
 		} else {
 			exito=true;
		 }
	 }while(!exito);
 	cout<<endl<<"Ingrese el nodo final: ";
 	do{
 		exito = false;
 		leer(ultimoNodo);
 		if (ultimoNodo<0 || ultimoNodo>=numNodos){
 			cout<<"El nodo no existe"<<endl;
 			cout<<"Ingrese nuevamente el nodo final: ";
 		} else if(ultimoNodo == primerNodo) {
 			cout<<"El nodo no puede ser igual al inicial"<<endl;
 			cout<<"Ingrese nuevamente el nodo final: ";
 		} else{
 			exito=true;
		 }
	 }while(!exito);
	 cout<<endl;
	 
	//dijkstra
	dijkstra( numNodos, A, primerNodo, ultimoNodo);
	/* liberar memoria */
	for ( i = 0; i < numNodos; i++ )
		free( A[i] );
	free( A );
 
	return 0;
}
/* Calcula el coste m'inimo de alcanzar un nodo final 'b'
 * grafo no dirigido con N nodos, a partir del nodo inicial 'a',
 * dada su matriz de distancia A */
void dijkstra( int N, int **A, int a, int b ) {
 
	label_t *Labels;
	int i, i0, j, peso;
	int *ruta;		/* array de nodos de la ruta minima */
 
	node_t *Cola = NULL;	/* cola de prioridad */
 
	/* Crea din'amicamente el arreglo de etiquetas de nodo */
	if ( ( Labels = (label_t*)malloc( N * sizeof( label_t ) ) ) == NULL ) return;
 
	/* nodo inicial 'a' entre 0 y N - 1 */
	if ( a < 0 || a > N - 1 ) return;
	/* nodo final 'a' entre 0 y N - 1 */
	if ( b < 0 || b > N - 1 ) return;
 
	/* inicializar las etiquetas de nodo */
	for ( i = 0; i < N; i++ ) {
		Labels[i].nro = i;
		if ( i != a ) {
			Labels[i].prev = -1;	/* a'un no se ha definido predecesor */
			Labels[i].peso = -1;	/* infinito */
			Labels[i].marca = 0;
		}
		else {
			Labels[i].prev = -1;	/* a'un no se ha definido predecesor */
			Labels[i].peso = 0;		/* coste del nodo inicial a s'i mismo es cero */
			Labels[i].marca = 0;
		}
	}
 
	/* agregamos el nodo inicial como primer elemento de la cola */
	insert( &Cola, a, Labels );
 
	/* continuamos este ciclo mientras existan nodos en la cola */
	while ( !isEmpty( Cola ) ) {
 
		/* toma como objetivo el primer elemento de la cola de prioridad */
		i0 = pop( &Cola );
 
		printf( "*** Analizando nodo %d ***\n", i0 );
 
		/* actualiza el peso de todos los sucesores no marcados (si los hay) del nodo
		 * encontrado y luego se~nala dicho nodo como marcado */
		for ( i = 0; i < N; i++ ) {
			if ( A[i0][i] > 0 && Labels[i].marca == 0 ) {
				/* si el coste es infinito, actualizar y añadir a la cola */
				if ( Labels[i].peso == -1 ) {
					Labels[i].peso = Labels[i0].peso + A[i0][i];
					insert( &Cola, i, Labels );
				}
				/* si el coste acumulado sumado al coste del enlace del nodo i0 al nodo i
				 * es menor al coste del nodo i, actualizar al valor del costo menor y 
				 * reubicar en la cola */
				else if ( Labels[i0].peso + A[i0][i] < Labels[i].peso ) {
					printf( "   * advertencia: mejorando coste de nodo %d\n", i );
					Labels[i].peso = Labels[i0].peso + A[i0][i];
					quit( &Cola, i );		/* para reubicar, quitamos y luego a~nadimos el nodo */
					insert( &Cola, i, Labels );
				}
				Labels[i].prev = i0;
				printf( "   coste de nodo %d desde nodo %d: %d\n", i, i0, Labels[i].peso );
			}
		}
		Labels[i0].marca = 1;
		printf( "   * nodo %d marcado\n", i0 );
 
		/* para verificar, imprime los costes calculados hasta el momento */
		for ( i = 0; i < N; i++ ) {
			printf( "%d: [", i);
			if ( Labels[i].peso == -1 ) printf( "Inf" ); else printf( "%d", Labels[i].peso);
			printf( ", %d", Labels[i].prev ); 
			if ( Labels[i].marca == 1 ) printf( ", x]\n" );	else printf( "]\n" );
		}
 
		/* imprimir cola prioridad de nodos */
		printf( "cola de nodos en orden de prioridad es:\n" );
		printQueue( Cola );
 
		/* pausa (opcional) */
		//printf( "\npresione ENTER para continuar ..." );
		//getchar();
	}
	printf( "Ya no quedan nodos por analizar.\n" );	/* mensaje final */
 
	/* Ruta desde el nodo 'a' hasta el nodo 'b' */
	int longitud = 2;
	i = b;
	while ( ( i = Labels[i].prev ) != a ) longitud++;	/* primero estimamos la longitud de la ruta */
	if ( ( ruta = (int*)malloc( longitud * sizeof(int) ) ) == NULL ) return;
 
	ruta[longitud - 1] = b;		/* luego rellenamos */
	i = b;
	j = 0;
	for ( j = 1; j < longitud; j++ ) {
		i = Labels[i].prev;
		ruta[longitud - j - 1] = i;
	}
 
	printf( "================================================================\n" );
	printf( "\nRuta mas economica entre nodo %d y nodo %d:\n\n", a, b );
	for ( i = 0; i < longitud; i++ ) {
		printf( "%d", ruta[i]);
		if ( i < longitud - 1 ) printf( " - " );
	}
	printf( "\n\nCosto total: %d\n\n", Labels[b].peso );
 
	free( ruta );
	free( Labels );
}
/* Devuelve 1 si la cola está vacía, 0 si no lo está. */
int isEmpty( node_t * head ) { 
 
	return head == NULL; 
}
 
/* Insertar ordenadamente en la cola.
 * Devuelve 0 si terminó con éxito, y 1 si ocurrió un error
 * de asignación de memoria. */
int insert( node_t ** frontPtr, int nodeNumber, label_t * Labels ) {
 
	node_t *newPtr, *currentPtr, *prevPtr;
 
	if ( ( newPtr = (node_t*)malloc( sizeof( node_t ) ) ) == NULL ) return 1;
 
	/* busca el último nodo menor al que se quiere insertar */
	currentPtr = *frontPtr;
	prevPtr = NULL;
	while ( currentPtr != NULL && Labels[currentPtr->data].peso < Labels[nodeNumber].peso ) {
		prevPtr = currentPtr;
		currentPtr = currentPtr->nextPtr;
	}
 
	/* inserta el nuevo nodo */
	newPtr->data = nodeNumber;
	if ( currentPtr != NULL ) 
		newPtr->nextPtr = currentPtr;
	else
		newPtr->nextPtr = NULL;
 
	if ( prevPtr != NULL ) 
		prevPtr->nextPtr = newPtr;
	else
		*frontPtr = newPtr;
 
	return 0;
}
 
/* Remover de la cola el nodo con el valor específico, si lo encuentra */
void quit( node_t ** frontPtr, int data ) {
 
	node_t *prevPtr = NULL, *currPtr = *frontPtr;
 
	/* busca el nodo con el valor espec'ifico */
	while ( currPtr != NULL && currPtr->data != data ) {
		prevPtr = currPtr;
		currPtr = currPtr->nextPtr;
	}
	if ( currPtr == NULL ) return;	/* y termina si no encuentra */
 
	/* si encuentra el nodo, lo remueve */
	if ( prevPtr != NULL )	
		prevPtr->nextPtr = currPtr->nextPtr;
	else 
		*frontPtr = currPtr->nextPtr;
 
	free( currPtr );
}
 
/* Retirar el primer elemento de la cola. 
 * Precaución: Devuelve cero si la cola est'a vacía.
 */
int pop( node_t **frontPtr ) {
 
	node_t *auxPtr;
	int data;
 
	if ( *frontPtr != NULL ) {
		auxPtr = *frontPtr;
		*frontPtr = (*frontPtr)->nextPtr;
		data = auxPtr->data;
		free( auxPtr );
		return data;
	}
	else
		return 0;
}
 
void printQueue( node_t *queue ) {
 
	node_t *currPtr = queue;
 
	if ( currPtr == NULL ) {
		printf( "(vacio)\n" );
		return;
	}
 
	while ( currPtr != NULL ) {
		printf( "%d", currPtr->data );
		if ( currPtr->nextPtr != NULL ) printf( " -> " );
		currPtr = currPtr->nextPtr;
	} 
	printf( "\n" );
}

void llenarMatriz(int **m, int cant) {
	int valor;
	cout<<"Ingrese los datos "<<endl;
	for (int i=0; i<cant;i++){
		for(int j=0;j<cant;j++){
			cout<<"m["<<i<<"]["<<j<<"] = ";	
			leer(m[i][j]);
		}
		cout<<endl;
	}
}

void mostrarMatriz(int **m, int cant){
		for (int i=0; i<cant;i++){
			for(int j=0;j<cant;j++)
				cout << setw(2) << m[i][j] << "  ";
		cout<<endl;
		}
}
void leer (int &n){
	cin >>n;
	while (cin.fail()){
		cin.clear();//limpia el buffer, reinicia cin.fail()
		cin.ignore(256, '\n');//misma funcion que el gets(temp)
		cout<<"Debe de ingresar un numero entero"<<endl;
		cout<<"por favor escriba el dato otra vez: ";
		cin>>n;
	}
}

void warshall (int **m, int num){
	for (int k = 0; k<num;k++){
		for (int i = 0; i<num;i++){
			for (int j = 0; j<num;j++){
				if(m[i][j]>m[i][k]+m[k][j]&&m[i][k]!=0&&m[k][j]!=0){
					m[i][j]=m[i][k]+m[k][j];
				} 
			}
		}	
	}
}
