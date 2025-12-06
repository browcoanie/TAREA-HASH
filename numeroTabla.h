#include <iostream>
#include <cmath> // Necesario para abs()

using namespace std;

struct Node
{
    int key;    
    int value;
    Node *next;
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
        h.table[i] = nullptr;
    }
}

// Funcion hash para NUMEROS
int functionHash(int key, int capacity){
    // Usamos valor absoluto (abs) para evitar indices negativos
    return abs(key) % capacity;
}

// Declaracion adelantada
void insert(tableHash &h, int key, int value);