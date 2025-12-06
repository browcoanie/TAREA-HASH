#include <iostream>
#include "contactosTabla.h"

using namespace std;

// Declaración adelantada para que 'insert' reconozca a 'resize' y viceversa
void resize(tableHash &h);

// Insertar datos
void insert(tableHash &h, string key, int value){

    // Chequeo de factor de carga
    // Es importante castear a float, si no la división de enteros da 0
    float chargeFactor = (float)(h.amountData + 1) / h.capacity;

    if (chargeFactor > 0.7)
    {
        resize(h);
    }

    int index = functionHash(key, h.capacity);

    // Crear nuevo nodo
    Node *newNode = new Node();
    newNode -> key = key;
    newNode -> value = value;
    newNode -> next = nullptr;

    // Manejo de colisiones (encadenamiento al inicio)
    if (h.table[index] == nullptr)
    {
        h.table[index] = newNode;
    }
    else{
        // Colision: lo ponemos primero en la lista
        newNode -> next = h.table[index];
        h.table[index] = newNode;
    }

    h.amountData++;
}

// Función para redimensionar
void resize(tableHash &h){
    cout << "\n[AVISO] Carga > 0.7. Redimensionando tabla de " << h.capacity << " a " << h.capacity * 2 <<"...\n";

    int oldCapacity = h.capacity;
    Node **oldTable = h.table; // Guardamos la tabla vieja

    // Nueva configuración
    h.capacity = h.capacity * 2;
    h.table = new Node*[h.capacity]; // Nuevo arreglo más grande
    h.amountData = 0; // Reiniciar contador (se sumará al reinsertar)


    // Inicializar la nueva tabla con nullptr
    for(int i = 0; i < h.capacity; i++) { 
        h.table[i] = nullptr; // Usar '=' y '<'
    }

    // Mover los datos viejos a la nueva tabla
    for (int i = 0; i < oldCapacity; i++) {
        Node* actual = oldTable[i];

        while (actual != nullptr) {
            // Re-insertamos en la nueva tabla (calculará el nuevo hash solito)
            insert(h, actual->key, actual->value);

            // Guardamos el siguiente antes de borrar el nodo viejo
            Node* aux = actual;
            actual = actual->next;
            delete aux; // Liberamos memoria del nodo viejo
        }
    }

    delete[] oldTable; // Borramos el array de punteros viejo
    cout << "Tabla redimensionada exitosamente.\n";
}

// Función para imprimir visualmente
void printTable(tableHash &h) {
    cout << "\n======================================================\n";
    cout << " TABLA HASH (Capacidad: " << h.capacity 
         << " | Elementos: " << h.amountData << ")\n";
    cout << "======================================================\n";
    cout << "Index\t| Contenido (Lista Enlazada)\n";
    cout << "-----\t| --------------------------\n";

    for (int i = 0; i < h.capacity; i++) {
        // 1. Imprimimos el índice seguido de una barra vertical
        cout << "  " << i << "\t| ";

        Node* actual = h.table[i];

        // 2. Si el slot está vacío, lo indicamos claramente
        if (actual == nullptr) {
            cout << "(Vacio)";
        } 
        else {
            // 3. Si hay datos, recorremos la lista
            while (actual != nullptr) {
                // Imprimimos [Clave : Valor]
                cout << "[" << actual->key << ":" << actual->value << "]";

                // Solo dibujamos la flecha si hay OTRO nodo después
                if (actual->next != nullptr) {
                    cout << " -> ";
                }
                
                actual = actual->next;
            }
        }
        cout << endl; // Salto de línea al terminar la fila
    }
    cout << "======================================================\n";
}

int main(){
    tableHash myTable;

    // Tabla de 5 posiciones
    initialize(myTable, 5);

    // Insertamos elementos
    insert(myTable, "Ana", 101);
    insert(myTable, "Luz", 102);
    insert(myTable, "Sol", 103);
    
    printTable(myTable);

    // Insertamos otro elemento para probar el resize
    cout << "\n>> Insertando a 'Mar' (Esto deberia disparar el resize)...";
    insert(myTable,"Mar", 104);
    
    // Al redimensionar, la capacidad pasa a 10
    printTable(myTable);

    cout << "\n>> Insertando a 'Zoe' (Debe colisionar con Ana y Sol en indice 2)...";
    insert(myTable, "Zoe", 999);
    printTable(myTable);

    return 0;
}