#include <iostream>
#include "numeroTabla.h"

using namespace std;

// Declaración adelantada
void resize(tableHash &h);

// Insertar datos
void insert(tableHash &h, int key, int value, bool verbose){

    // Chequeo de factor de carga
    // En Open Addressing es CRITICO no llenar la tabla al 100% o entra en bucle infinito.
    float chargeFactor = (float)(h.amountData + 1) / h.capacity;

    if (chargeFactor > 0.7)
    {
        resize(h);
    }

    // Calcular índice inicial
    int index = functionHash(key, h.capacity);
    int originalIndex = index; // Para detectar si dimos la vuelta completa 

    // Manejo de colisiones: LINEAR PROBING
    // Mientras la casilla NO sea nullptr está ocupada, avanzamos a la siguiente
    while (h.table[index] != nullptr)
    {
        // 
        if(verbose) {
            cout << "   [!] COLISION detectada en indice " << index 
                 << " (Ocupado por clave: " << h.table[index]->key << ")." 
                 << " Buscando siguiente..." << endl;
        }

        // Avanzamos uno (Circular)
        index = (index + 1) % h.capacity;

        // Seguridad: Si volvemos al inicio, la tabla está 100% llena (no debería pasar por el resize previo)
        if (index == originalIndex) {
            cout << "Error: Tabla completamente llena.\n";
            return;
        }
    }

    //  Insertamos en la casilla vacía encontrada
    Node *newNode = new Node();
    newNode -> key = key;
    newNode -> value = value;
    
    h.table[index] = newNode;
    h.amountData++;   

    if(verbose) cout << "   -> Insertado exitosamente en indice " << index << endl;
}

// Función Resize adaptada a Open Addressing
void resize(tableHash &h){
    cout << "\n[SISTEMA] Factor de carga > 0.7. Redimensionando de " << h.capacity << " a " << h.capacity * 2 <<"...\n";

    int oldCapacity = h.capacity;
    Node **oldTable = h.table; 

    // Actualizamos estructura
    h.capacity = h.capacity * 2;
    h.table = new Node*[h.capacity]; 
    h.amountData = 0; // Se reinicia porque 'insert' lo va a ir sumando de nuevo

    // Inicializamos la nueva tabla vacía
    for(int i = 0; i < h.capacity; i++) h.table[i] = nullptr;

    // Rehashing: Recorremos la tabla vieja linealmente
    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i] != nullptr) {
            // Reinsertamos en la nueva tabla (verbose = false para que no llene la pantalla de avisos)
            insert(h, oldTable[i]->key, oldTable[i]->value, false);
            
            // Liberamos la memoria del nodo viejo (ya creamos uno nuevo en insert)
            delete oldTable[i];
        }
    }
    delete[] oldTable;
    cout << "[SISTEMA] Tabla redimensionada exitosamente.\n";
}

// Imprimir tabla (Formato Open Addressing)
void printTable(tableHash &h) {
    cout << "\n========================================\n";
    cout << " TABLA HASH (Open Addressing | Cap: " << h.capacity << ")\n";
    cout << "========================================\n";
    cout << "Index\t| Contenido\n";
    cout << "-----\t| ---------\n";

    for (int i = 0; i < h.capacity; i++) {
        cout << "  " << i << "\t| ";
        if (h.table[i] == nullptr) {
            cout << "(Vacio)";
        } 
        else {
            // Ya no hay "while" ni "next", solo el dato en la casilla
            cout << "[" << h.table[i]->key << " : " << h.table[i]->value << "]";
        }
        cout << endl;
    }
    cout << "========================================\n";
}

int main(){
    tableHash myTable;
    int tamanoInicial;
    int opcion = 0;

    cout << "--- CONFIGURACION INICIAL ---\n";
    cout << "Ingrese el tamano inicial de la tabla: ";
    cin >> tamanoInicial;

    // Validación básica
    if (tamanoInicial < 1) tamanoInicial = 5;
    
    initialize(myTable, tamanoInicial);

    do {
        cout << "\n--- MENU PRINCIPAL ---\n";
        cout << "1. Insertar numero\n";
        cout << "2. Mostrar tabla\n";
        cout << "3. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1: {
            int clave, valor;
            cout << "Ingrese el numero (clave): ";
            cin >> clave;
            // Usamos la clave como valor por defecto para simplificar, o puedes pedirlo aparte
            // cout << "Ingrese el valor asociado: "; cin >> valor; 
            valor = clave * 10; // Ejemplo: Si clave es 5, valor es 50 (automático)
            
            cout << "Intentando insertar " << clave << "...\n";
            insert(myTable, clave, valor, true);
            break;
        }
        case 2:
            printTable(myTable);
            break;
        case 3:
            cout << "Saliendo...\n";
            break;
        default:
            cout << "Opcion invalida.\n";
            break;
        }

    } while (opcion != 3);

    return 0;
}
