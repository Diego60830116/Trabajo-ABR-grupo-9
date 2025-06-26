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

// Guardar los datos del árbol en un archivo de texto
void guardarArchivo(Nodo* raiz, ofstream& archivo) {
    if (raiz == NULL) return;
    archivo << raiz->id << "," << raiz->nombre << "," << raiz->fechaNacimiento << "," << raiz->parentesco << endl;
    guardarArchivo(raiz->izquierda, archivo);
    guardarArchivo(raiz->derecha, archivo);
}

// Cargar los datos desde el archivo al iniciar el programa
void cargarArchivo(Nodo*& raiz) {
    ifstream archivo("miembros.txt");
    if (!archivo) return;

    int id;
    string nombre, fecha, parentesco;
    string linea;

    while (getline(archivo, linea)) {
        size_t pos1 = linea.find(",");
        size_t pos2 = linea.find(",", pos1 + 1);
        size_t pos3 = linea.find(",", pos2 + 1);

        id = atoi(linea.substr(0, pos1).c_str());
        nombre = linea.substr(pos1 + 1, pos2 - pos1 - 1);
        fecha = linea.substr(pos2 + 1, pos3 - pos2 - 1);
        parentesco = linea.substr(pos3 + 1);

        Nodo* nuevo = crearNodo(id, nombre, fecha, parentesco);
        raiz = insertar(raiz, nuevo);
    }
    archivo.close();
}

// Función principal: menú de opciones
int main() {
    Nodo* raiz = NULL;
    cargarArchivo(raiz); // Cargar datos si existen

    int opcion;
    do {
        cout << "\n--- MENU ---\n";
        cout << "1. Insertar miembro\n";
        cout << "2. Buscar por ID\n";
        cout << "3. Buscar por nombre\n";
        cout << "4. Eliminar miembro\n";
        cout << "5. Recorrido Inorden\n";
        cout << "6. Recorrido Preorden\n";
        cout << "7. Recorrido Postorden\n";
        cout << "8. Guardar y salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore();

        int id;
        string nombre, fecha, parentesco;

        switch (opcion) {
            case 1:
                // Insertar nuevo miembro
                cout << "ID: "; cin >> id; cin.ignore();
                cout << "Nombre: "; getline(cin, nombre);
                cout << "Fecha de nacimiento: "; getline(cin, fecha);
                cout << "Parentesco: "; getline(cin, parentesco);
                raiz = insertar(raiz, crearNodo(id, nombre, fecha, parentesco));
                cout << "\nÁrbol actualizado (Recorrido Inorden):\n";
                inorden(raiz); // Mostrar árbol tras inserción
                break;
            case 2:
                // Buscar por ID
                cout << "Ingrese ID: "; cin >> id;
                {
                    Nodo* encontrado = buscarPorID(raiz, id);
                    if (encontrado)
                        cout << "Encontrado: " << encontrado->nombre << " (" << encontrado->parentesco << ")" << endl;
                    else
                        cout << "No encontrado.\n";
                }
                break;
            case 3:
                // Buscar por nombre
                cout << "Ingrese nombre: "; getline(cin, nombre);
                buscarPorNombre(raiz, nombre);
                break;
            case 4:
                // Eliminar miembro
                cout << "Ingrese ID a eliminar: "; cin >> id;
                raiz = eliminar(raiz, id);
                cout << "Miembro eliminado.\n";
                break;
            case 5:
                // Mostrar árbol Inorden
                cout << "Recorrido Inorden:\n";
                inorden(raiz); break;
            case 6:
                // Mostrar árbol Preorden
                cout << "Recorrido Preorden:\n";
                preorden(raiz); break;
            case 7:
                // Mostrar árbol Postorden
                cout << "Recorrido Postorden:\n";
                postorden(raiz); break;
            case 8:
                // Guardar en archivo y salir
                {
                    ofstream archivo("miembros.txt");
                    guardarArchivo(raiz, archivo);
                    archivo.close();
                    cout << "Datos guardados. Saliendo...\n";
                }
                break;
            default:
                cout << "Opción inválida.\n";
        }

    } while (opcion != 8);

    return 0;
}
