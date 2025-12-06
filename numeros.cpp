#include <iostream>
#include "numeroTabla.h"

using namespace std;

// Declaración adelantada
void resize(tableHash &h);

// Insertar datos (Ahora recibe int key)
void insert(tableHash &h, int key, int value){

    // 1. Chequeo de factor de carga (anticipado +1)
    float chargeFactor = (float)(h.amountData + 1) / h.capacity;

    if (chargeFactor > 0.7)
    {
        resize(h);
    }

    // 2. Calcular índice (simple modulo)
    int index = functionHash(key, h.capacity);

    Node *newNode = new Node();
    newNode -> key = key;
    newNode -> value = value;
    newNode -> next = nullptr;

    // 3. Manejo de colisiones
    if (h.table[index] == nullptr)
    {
        h.table[index] = newNode;
    }
    else{
        // Colision: insertar al inicio de la lista
        newNode -> next = h.table[index];
        h.table[index] = newNode;
    }

    h.amountData++;   
}

// Función Resize adaptada a int
void resize(tableHash &h){
    cout << "\n[AVISO] Carga critica. Redimensionando de " << h.capacity << " a " << h.capacity * 2 <<"...\n";

    int oldCapacity = h.capacity;
    Node **oldTable = h.table; 

    h.capacity = h.capacity * 2;
    h.table = new Node*[h.capacity]; 
    h.amountData = 0; 

    // Limpieza correcta
    for(int i = 0; i < h.capacity; i++) h.table[i] = nullptr;

    // Rehashing
    for (int i = 0; i < oldCapacity; i++) {
        Node* actual = oldTable[i];

        while (actual != nullptr) {
            insert(h, actual->key, actual->value); // Re-insertamos el int
            
            Node* aux = actual;
            actual = actual->next;
            delete aux;
        }
    }
    delete[] oldTable;
    cout << "Tabla redimensionada exitosamente.\n";
}

// Imprimir tabla bonita
void printTable(tableHash &h) {
    cout << "\n======================================================\n";
    cout << " TABLA HASH (Cap: " << h.capacity << " | Datos: " << h.amountData << ")\n";
    cout << "======================================================\n";
    cout << "Index\t| Contenido\n";
    cout << "-----\t| ---------\n";

    for (int i = 0; i < h.capacity; i++) {
        cout << "  " << i << "\t| ";
        Node* actual = h.table[i];

        if (actual == nullptr) {
            cout << "(Vacio)";
        } 
        else {
            while (actual != nullptr) {
                // Mostramos [Clave : Valor]
                cout << "[" << actual->key << ":" << actual->value << "]";
                if (actual->next != nullptr) cout << " -> ";
                actual = actual->next;
            }
        }
        cout << endl;
    }
    cout << "======================================================\n";
}

int main(){
    tableHash myTable;
    // Tabla inicial pequeña (5)
    initialize(myTable, 5);

    // --- CASO DE PRUEBA: COLISIONES NUMERICAS ---
    // Tamaño 5. El hash es (key % 5).
    
    // 10 % 5 = 0
    insert(myTable, 10, 100); 
    
    // 20 % 5 = 0 (¡Colision con el 10!)
    insert(myTable, 20, 200); 
    
    // 33 % 5 = 3
    insert(myTable, 33, 300);
    
    cout << ">> Estado inicial (con colision en indice 0):";
    printTable(myTable);

    // --- DISPARAR RESIZE ---
    // Insertamos el 4to dato. (3+1)/5 = 0.8 > 0.7
    cout << "\n>> Insertando 42 (Dispara Resize a 10)...";
    insert(myTable, 42, 400); 

    // Ahora la tabla es de 10.
    // 10 % 10 = 0
    // 20 % 10 = 0 (Siguen juntos, es normal por ser múltiplos)
    // Pero si hubieramos puesto 15 y 25 (terminan en 5), al crecer la tabla se separarian.
    
    printTable(myTable);

    // Prueba de colisión triple en la nueva tabla
    cout << "\n>> Insertando 50 (Colisionara con 10 y 20 en indice 0)...";
    insert(myTable, 50, 500);

    printTable(myTable);

    return 0;
}