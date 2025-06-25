#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // Para atoi()
using namespace std;

// Estructura del nodo del árbol genealógico
struct Nodo {
    int id;
    string nombre;
    string fechaNacimiento;
    string parentesco;
    Nodo* izquierda;
    Nodo* derecha;
};

// Función para crear un nuevo nodo
Nodo* crearNodo(int id, string nombre, string fechaNacimiento, string parentesco) {
    Nodo* nuevo = new Nodo;
    nuevo->id = id;
    nuevo->nombre = nombre;
    nuevo->fechaNacimiento = fechaNacimiento;
    nuevo->parentesco = parentesco;
    nuevo->izquierda = NULL;
    nuevo->derecha = NULL;
    return nuevo;
}

// Función para insertar un nodo en el árbol binario
Nodo* insertar(Nodo* raiz, Nodo* nuevo) {
    if (raiz == NULL) return nuevo; // Árbol vacío
    if (nuevo->id < raiz->id)
        raiz->izquierda = insertar(raiz->izquierda, nuevo);
    else if (nuevo->id > raiz->id)
        raiz->derecha = insertar(raiz->derecha, nuevo);
    return raiz;
}

// Buscar un miembro por su ID
Nodo* buscarPorID(Nodo* raiz, int id) {
    if (raiz == NULL || raiz->id == id) return raiz;
    if (id < raiz->id) return buscarPorID(raiz->izquierda, id);
    else return buscarPorID(raiz->derecha, id);
}

// Buscar un miembro por su nombre (puede haber más de uno)
void buscarPorNombre(Nodo* raiz, string nombre) {
    if (raiz == NULL) return;
    buscarPorNombre(raiz->izquierda, nombre);
    if (raiz->nombre == nombre)
        cout << "ID: " << raiz->id << " | Nombre: " << raiz->nombre
             << " | Fecha: " << raiz->fechaNacimiento
             << " | Parentesco: " << raiz->parentesco << endl;
    buscarPorNombre(raiz->derecha, nombre);
}

// Recorrido inorden (muestra nodos en orden de ID)
void inorden(Nodo* raiz) {
    if (raiz == NULL) return;
    inorden(raiz->izquierda);
    cout << raiz->id << " - " << raiz->nombre << " (" << raiz->parentesco << ")" << endl;
    inorden(raiz->derecha);
}

// Recorrido preorden (útil para guardar estructura del árbol)
void preorden(Nodo* raiz) {
    if (raiz == NULL) return;
    cout << raiz->id << " - " << raiz->nombre << endl;
    preorden(raiz->izquierda);
    preorden(raiz->derecha);
}

// Recorrido postorden (útil para eliminar nodos)
void postorden(Nodo* raiz) {
    if (raiz == NULL) return;
    postorden(raiz->izquierda);
    postorden(raiz->derecha);
    cout << raiz->id << " - " << raiz->nombre << endl;
}

// Encontrar el nodo con el valor mínimo (para eliminación)
Nodo* encontrarMinimo(Nodo* raiz) {
    while (raiz && raiz->izquierda != NULL)
        raiz = raiz->izquierda;
    return raiz;
}

// Eliminar un miembro por ID
Nodo* eliminar(Nodo* raiz, int id) {
    if (raiz == NULL) return raiz;

    if (id < raiz->id)
        raiz->izquierda = eliminar(raiz->izquierda, id);
    else if (id > raiz->id)
        raiz->derecha = eliminar(raiz->derecha, id);
    else {
        // Caso 1: Nodo sin hijos
        if (raiz->izquierda == NULL && raiz->derecha == NULL) {
            delete raiz;
            return NULL;
        }
        // Caso 2: Nodo con un hijo
        else if (raiz->izquierda == NULL || raiz->derecha == NULL) {
            Nodo* temp = (raiz->izquierda) ? raiz->izquierda : raiz->derecha;
            delete raiz;
            return temp;
        }
        // Caso 3: Nodo con dos hijos
        Nodo* temp = encontrarMinimo(raiz->derecha); // Sucesor
        raiz->id = temp->id;
        raiz->nombre = temp->nombre;
        raiz->fechaNacimiento = temp->fechaNacimiento;
        raiz->parentesco = temp->parentesco;
        raiz->derecha = eliminar(raiz->derecha, temp->id); // Eliminar sucesor
    }
    return raiz;
}
