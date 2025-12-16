#include <iostream>
#include <cmath> // Necesario para abs()

using namespace std;

// En Open Addressing, ya no necesitamos el puntero 'next'
// porque si colisiona, buscamos la siguiente casilla vacía en el array.
struct Node
{
    int key;    
    int value;
};

struct tableHash{
    Node **table; 
    int capacity; 
    int amountData; 
};

void initialize(tableHash &h, int capInicial){
    h.capacity = capInicial;
    h.amountData = 0;
    h.table = new Node*[h.capacity];

    for (int i = 0; i < h.capacity; i++)
    {
        h.table[i] = nullptr; // nullptr indica casilla vacía
    }
}

// Funcion hash para NUMEROS
int functionHash(int key, int capacity){
    return abs(key) % capacity;
}

// Declaracion adelantada
// Agregamos un booleano 'verbose' para decidir si queremos ver los avisos de colisión
// (útil para que no moleste durante el resize interno)
void insert(tableHash &h, int key, int value, bool verbose = true);
