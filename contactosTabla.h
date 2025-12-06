#include <string> 

using namespace std;

struct Node
{
    string key;
    int value;
    Node *next;
};

struct tableHash{
    Node **table; // El arreglo de punteros
    int capacity; // Tamaño actual del arreglo
    int amountData; // Cuantos elementos estan guardados
};

void initialize(tableHash &h, int capInicial){
    h.capacity = capInicial;
    h.amountData = 0;

    //Creamos el arreglo dinamico
    h.table = new Node*[h.capacity];

    // Limpiamos 
    for (int i = 0; i < h.capacity; i++)
    {
        h.table[i] = nullptr;
    }
    
}

//Funcion hash
int functionHash(string key, int capacity){
    int sum = 0;
    for(char c: key) sum+=c;
    return sum % capacity;
}

// Declaracion adelantada para usarla dentro de redimensionar
void insert(tableHash &h, string key, int value);


