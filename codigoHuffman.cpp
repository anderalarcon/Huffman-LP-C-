#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <iomanip>
#include <vector>
#define MAX 100
using namespace std;
int i;

/*
ACHATA ALARCON, ANDERSON ADRIAN      ---> 20172991
MURGA MACAYO, CESAR ALEJANDRO        ---> 20173642
VALDEZ PORTOCARRERO, BORIS CHRISTIAN ---> 20173978
*/

// Me devolver� la longitud de una palabra.
constexpr int longChar(const char* cadena){
    int rpta{0};
    while (*cadena){
        ++cadena;
        ++rpta;
    }
    return rpta;
}

// Se usar� para acceder al valor frecuencia del diccionario a trav�s de su key (letra).
typedef pair<char, int> frecuencia;

// Estructura encargada de soportar los valores del nodo en la letra a ingresar, el valor de la
// frecuencia con la que se repite y los punteros izquierda y derecha del nodo.
struct Nodo {
    char dato;
    int freq;
    struct Nodo *izq, *der;
};

// Estructura encargada de soportar los valores del arbol de Huffman en el tama�o actual de �rbol,
// su capacidad y el arreglo en donde ir�n los nodos acumulados de la estructura Nodo.
struct arbolHuffman {
    int tam;
    int capacidad;
    struct Nodo** arregloNodos;
};

// Esta funci�n de la estructura Nodo se encargar� de generar un nuevo nodo, estableciendo cada
// valor compuesto del tipo Nodo que son sus ramas izquierda y derecha, la letra en dato; y el valor
// de frecuencias de dicha letra. Por �ltimo se retornar� el nodo creado.
struct Nodo* nuevoNodo(char dato, int freq) {
    struct Nodo* nodo = (struct Nodo*)malloc (sizeof(struct Nodo));
    nodo->izq = nodo->der = NULL;
    nodo->dato = dato;
    nodo->freq = freq;
    return nodo;
}

// Esta funci�n pertenciente a la estructura arbolHuffman se encargar� de establecer la capacidad
// en memoria para crear el �rbol de Huffman en base al arreglo que contendr� a todos los nodos.
// Luego se retornar� arreglo de nodos capaz de soportar la data a a�adir a �rbol.
struct arbolHuffman* crearArbol(int capacidad) {
    struct arbolHuffman* nodos = (struct arbolHuffman*)malloc(sizeof(struct arbolHuffman));
    nodos->tam = 0;
    nodos->capacidad = capacidad;
    nodos->arregloNodos = (struct Nodo**)malloc(nodos-> capacidad * sizeof(struct Nodo*));
    return nodos;
}

// Esta funci�n intercambiar� el lugar de memoria al que apuntan ambos nodos.
void intercambioNodos(struct Nodo** a, struct Nodo** b) {
    struct Nodo* aux = *a;
    *a = *b;
    *b = aux;
}

// Esta funci�n se encargar� de ordenar los nodos dentro del arbol direccionando correctamente sus
// ramas. Al ordenar un arreglo por mont�culos (heap) siempre se cumple que la posici�n de la izquierda
// es dos veces la posici�n y la derecha dos veces la posici�n m�s 1. Se har�n comparaciones entre
// los valores de posiciones para izquierda, adem�s de comparar los valores de frecuencia entre las
// posciones izquierda y menor del arreglo. Si se cumple se establecer� como valor de menor la izquierda.
// El mismo procedimiento se cumplir� para la rama derecha.
// Luego de ello se verificar� si menor no es igual a posici�n para ver si ya se lleg� a recorrer todo
// el arreglo al ordenar. Si no es asi se intercambian las punteros de memoria entre menor y posici�n en
// el arreglo. y se volver� a ordenar.
void ordenar(struct arbolHuffman* nodos, int pos) {
    int menor = pos;
    int izq = 2 * pos;
    int der = 2 * pos + 1;
    if (izq < nodos->tam && nodos->arregloNodos[izq]-> freq < nodos->arregloNodos[menor]->freq)
        menor = izq;

    if (der < nodos->tam && nodos->arregloNodos[der]-> freq < nodos->arregloNodos[menor]->freq)
        menor = der;

    if (menor != pos) {
        intercambioNodos(&nodos->arregloNodos[menor], &nodos->arregloNodos[pos]);
        ordenar(nodos, menor);
    }
}

// Esta funci�n me ayudar� a saber si el tama�o del arbol ya es uno del arreglo de nodos.
int tam_uno(struct arbolHuffman* nodos) {
    return (nodos->tam == 1);
}

// Esta funci�n de la estructura Nodo se escargar� de extraer el nodo m�nimo del arreglo de nodos.
// Es extraer� la posici�n 0 del arreglo y ser� retornada. Luego se moveran cada nodo una pocisi�n
// adelante por el nodo extra�do y se disminuir� de tama�o el tama�o del arreglo.
// Se volver� a ordenar el arreglo.
struct Nodo* extraerNodoMin(struct arbolHuffman* nodos) {
    struct Nodo* aux = nodos->arregloNodos[0];
    nodos->arregloNodos[0] = nodos->arregloNodos[nodos->tam - 1];
    --nodos->tam;
    ordenar(nodos, 0);
    return aux;
}

// Esta funci�n se encargar� de insertar un nodo dentro del arreglo de nodos. Primero se aumentar�
// la variable tama�o en 1 de nodos y se comenzar� a iterar hasta encontrar la pocisi�n sea
// de frecuencia no sea menor a la posic�n del nodo iterado de pocisi�n con tama�o del arreglo menos
// uno entre dos, esto se hace para bajar niveles del arbol dentro del arreglo. Una vez encontrada
// se le asignar� a esa posici�n el nuevo nodo.
void insertarNodo(struct arbolHuffman* nodos, struct Nodo* nodo) {
    ++nodos->tam;
    int i = nodos->tam - 1;
    while (i && nodo->freq < nodos->arregloNodos[(i - 1) / 2]->freq) {
        nodos->arregloNodos[i] = nodos->arregloNodos[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    nodos->arregloNodos[i] = nodo;
}

// Esta funci�n se encargar� de ordenar los nodos ingresado iterando la cantidad de veces por
// por tama�o inversamente dentro del arreglo de nodos.
void ordenarNodos(struct arbolHuffman* nodos) {
    int n = nodos->tam - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        ordenar(nodos, i);
}

// Imprime en pantalla el c�digo binario ingresado y su car�cter respectivo.
void decodificacion(int arreglo[], int n, char ch) {
    int i;
    for (i = 0; i < n; ++i)
        cout << arreglo[i];
    cout <<  ": " << ch << endl;
}

// Imprime en pantalla el car�cter y sus c�digo binario que se encuentra en el arreglo ingresado.
void codificacion(int arreglo[], int n, char ch) {
    int i;
    cout << ch << ": ";
    for (i = 0; i < n; ++i)
        cout << arreglo[i];
    cout << endl;
}

// Reconoce si la raiz ingresada es hoja o no, reconociendo que no tenga ramas adyacentes.
int esHoja(struct Nodo* raiz) {
    return !(raiz->izq) && !(raiz->der);
}

// Esta funci�n se encargar� de generar los nodos con los datos, frecuencias y tama�o proporcionados
// por el diccioario inicial. Esto generar� un arreglo de nodo donde se ir�n almacenando cada uno.
// Para ello se iterar� el tama�o del diccionario y por cada dato y frecuencia proporcionado se
// generar� un nodo. Una vez terminada la iteraci�n se le asignar� al tama�o de la varible nodo el
// tama�o del diccionario. Luego se emplear� la funci�n ordenarNodos para ordenar el arreglo y se
// retornar� los nodos generados.
struct arbolHuffman* crearNodos(char dato[], int freq[], int tam) {
    struct arbolHuffman* nodos = crearArbol(tam);
    for (int i = 0; i < tam; ++i)
        nodos->arregloNodos[i] = nuevoNodo(dato[i], freq[i]);
    nodos->tam = tam;
    ordenarNodos(nodos);
    return nodos;
}

// La funci�n crearArbolHuffman del struct Nodo se encargar� de generar el �rbol con la datos
// obtenidos del diccionario. Se incializar�n varios punteros de ramas y raiz. Adem�s, se inicializar�
// la variable nodos que contendr� los nodos para generar el arbol. A esta variable se le
// asignar� la funci�n crearNodos para crear dichos nodos. Luego se empezar� la generaci�n del
// �rbol de Huffman, interando los nodos generados, extrayendo los nodos con menor frecuencia
// y generando un nodo top de la suma de las frecuencias de ellos y poniendo los punteros izquierda
// y derecha de top. Al obtener top se insertar� al conjunto de nodos restantes del arbol Huffman.
// Por �ltimo, se retornar� el menor nodo del conjunto que es el c�digo Huffman de la frase comprimida.
struct Nodo* crearArbolHuffman(char dato[], int freq[], int tam) {
    struct Nodo *izq, *der, *top;
    struct arbolHuffman* nodos = crearNodos(dato, freq, tam);
    while (!tam_uno(nodos)) {
        izq = extraerNodoMin(nodos);
        der = extraerNodoMin(nodos);
        top = nuevoNodo('*', izq->freq + der->freq);
        top->izq = izq;
        top->der = der;
        insertarNodo(nodos, top);
    }
    return extraerNodoMin(nodos);
}

// Esta funci�n se encargar� generar los c�digos binarios de cada letra. Esto ser� posible
// recorriendo cada rama del arbol generado. Si recorre la rama izquierda se le asignar�
// un bit 0 al arreglo ingresado, de manera contraria ocurre con la rama derecha a la cual se
// asignar�a bit 1. Esta generaci�n de bits se har� de manera recursiva para verificar si la
// raiz actual tiene m�s ramas izquierdas o derechas. Si la instancia de una raiz no detecta rama
// izquierda o deracha y se comprueba que es una hoja, se ejecutar� la impresi�n de los valores
// de esa raiz nodo.
void imprimirCodigos(struct Nodo* raiz, int arreglo[], int top) {
    if (raiz->izq) {
        arreglo[top] = 0;
        imprimirCodigos(raiz->izq, arreglo, top + 1);
    }
    if (raiz->der) {
        arreglo[top] = 1;
        imprimirCodigos(raiz->der, arreglo, top + 1);
    }
    if (esHoja(raiz)) {
        codificacion(arreglo, top, raiz -> dato);
    }
}

// Se usa la misma l�gica para imprimir en pantalla los c�digo binarios y mostrar su letra
// correspondiente.
void imprimirChar(struct Nodo* raiz, int arreglo[], int top) {
    if (raiz->izq) {
        arreglo[top] = 0;
        imprimirChar(raiz->izq, arreglo, top + 1);
    }
    if (raiz->der) {
        arreglo[top] = 1;
        imprimirChar(raiz->der, arreglo, top + 1);
    }
    if (esHoja(raiz)) {
        decodificacion(arreglo, top, raiz -> dato);
    }
}

// Esta es la funci�n principal del programa y que tendr� como par�metros el diccionario
// generado y su tama�o. Se comenzar� generando un par de arreglos que contendr�n los
// datos de cada key del diccionario ingresado y sus respectivas frecuencias, respectivamente.
// Luego se inicializar� la variable raiz que ser� el resultado del c�digo Huffman. A esta
// variable se le asignar� la funci�n crearArbolHuffman y que adem�s se le ingresar�n los
// arreglos generados y el tama�o del diccionario. Una vez obtenido la raiz se pasar� a imprimir
// sus datos de c�digos con las funciones de imprimirCodigos e imprimirChar.
void codigosHuffman(map<char,int> diccionario, int tam) {
    map<char,int>::iterator p = diccionario.begin();
    char dato[MAX];
    int freq[MAX];
    int i=0;
    while (p != diccionario.end()){
        dato[i] = p->first;
        freq[i] = p->second;
        i++;
        p++;
    }
    cout << endl;
    struct Nodo* raiz = crearArbolHuffman(dato, freq, tam);
    int arreglo[MAX], top = 0;
    cout << "Resultado del codigo Huffman ->  ";
    cout << raiz -> freq << ":" << raiz -> dato << endl;
    cout << endl;
    cout << "********* Codificacion *********" << endl;
    imprimirCodigos(raiz, arreglo, top);
    cout << endl;
    cout << "********* Decodificacion *********" << endl;
    imprimirChar(raiz, arreglo, top);
}

// Esta funci�n se encargar� de generar el diccionario de frecuencias por cada letra encontrada
// en el texto ingresado. Se recorrer� iterando la longitud del texto ingresado y se identificar�
// si en el diccionario existe o no la letra iterada. Si no la encuentra, el programa reconocer�
// que es la primera vez que se encuentra la letra y le asignar� a su valor frecuencia el valor de 1.
// Si la letra ya exist�a, simplemente se le sumar� 1 al valor de frecuencia se esa letra.
// Una vez terminado el recorrido del texto se devolver� el diccionario generado.
map<char,int> frecuencia_diccionario(char texto[MAX]){
    map<char,int> diccionario;
    int tam_diccionario = 0;
    for (i=0;i<longChar(texto);i++){
        map<char,int>::iterator p = diccionario.begin();
        bool h = false;
        for (int j=0;j<tam_diccionario;j++){
            if (texto[i] == (p->first)){
                diccionario.insert(frecuencia(texto[i],++(p->second)));
                h = true;
                break;
            }
            p++;
        }

        if (h == false){
            diccionario.insert(frecuencia(texto[i],(p->second = 1)));
            tam_diccionario++;
        }
    }
    return diccionario;
}

// Esta funci�n se encargar� de devolver la longitud del diccionario recorriendolo y acumulando
// la cantidad de iteraciones del diccionario.
int long_diccionario(map<char, int> diccionario){
    map<char,int>::iterator p = diccionario.begin();
    int cant = 0;
    while (p != diccionario.end()){
        cant++;
        p++;
    }
    return cant;
}

// Esta funci�n se encargar� de imprimir el diccionario recorriendolo por cada char(key) mostrando
// su respectivo valor.
void imprimir_diccionario(map<char, int> diccionario){
    map<char,int>::iterator p = diccionario.begin();
    cout << endl << "Frecuencias: {";
    int h = 1;
    int cant = long_diccionario(diccionario);
    while (p != diccionario.end() ){
        cout << p->first <<":"<< p->second;
        if (h != cant){
            cout << ", ";
        }
        h++;
        p++;
    }
    cout << "}" << endl;
}

// Aqu� se iniciar� el c�digo pidiendo al usuario ingresar una palabra, la cual se mostrar�.
// Luego se incializar� la variable diccionario de tipo map(contiene valores char e int), y se le
// asignar� el resultado retornado por la funci�n frecuencia_diccionario.
// Se mostrar� en pantalla el diccionario generado.
// Luego se ejecutar� la funci�n principal llamada codigosHuffman, al cual se le enviar�n como
// par�metros el diccionario generado y su longitud.
int main(){
    //Texto probado: AABBBBACCDAA
    //cout << endl << "Ingrese texto: ";
    char texto[MAX] = "VIVAELPERU";
    //cin >> texto;
    cout << endl;
    cout << "Texto ingresado -> " << texto << endl;
    map<char, int> diccionario = frecuencia_diccionario(texto);
    imprimir_diccionario(diccionario);
    codigosHuffman(diccionario,long_diccionario(diccionario));
    return 0;
}
