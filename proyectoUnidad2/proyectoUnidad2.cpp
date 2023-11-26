#include <iostream>
#include <stdio.h>
#include <queue>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cstring>
#include <fstream>

using namespace std;
struct Guardian {
    char nombre[50];
    int nivelPoder;
    char maestro[50];
    char ciudad[50];
};
struct CiudadGrafo {
    char nombre[100];
    vector <string> coneccionCiudad;
};
struct NodoJerarquico {
    Guardian guardian;
    vector<NodoJerarquico> aprendices;
};

struct NodeRanking {
    Guardian guardian;
    int data;
    NodeRanking* left;
    NodeRanking* right;
};

struct TempGuardian{
    Guardian guardian;
    bool maestroEncontrado;
};
                                    //funcion cargar archivos ciudades y guardianes.

vector<CiudadGrafo> cargarCiudades(char archivoCiudades[]) {
    vector<CiudadGrafo> ciudades;
    ifstream file(archivoCiudades);
    char line[256];

    while (file.getline(line,sizeof(line))) {
        CiudadGrafo ciudad;

        char* nombre_ciudad = strtok(line, ",");
        strcpy(ciudad.nombre,nombre_ciudad);

        bool ciudadExistente = false;

        for(auto& c : ciudades){
            if(strcmp(c.nombre, ciudad.nombre) == 0){
                ciudadExistente = true;
                break;
            }
        }
        char* ciudad_coneccion = strtok(nullptr, ",");
        while (ciudad_coneccion != nullptr) {

            if(!ciudadExistente){
                ciudad.coneccionCiudad.push_back(ciudad_coneccion);
            }else{

                for(auto& c : ciudades){
                    if(strcmp(c.nombre,ciudad.nombre) == 0){
                        c.coneccionCiudad.push_back(ciudad_coneccion);
                        break;
                    }
                }
            }
            ciudad_coneccion = strtok(nullptr,",");
        }
        if(!ciudadExistente){
            ciudades.push_back(ciudad);
        }
    }
    return ciudades;
}
/*
void intercambiarGuardianes(Guardian& a, Guardian& b){

    Guardian temp = a;

    strcpy(a.nombre,b.nombre);
    a.nivelPoder = b.nivelPoder;
    strcpy(a.maestro,b.maestro);
    strcpy(a.ciudad,b.ciudad);

    strcpy(b.nombre,temp.nombre);
    b.nivelPoder = temp.nivelPoder;
    strcpy(b.maestro,temp.maestro);
    strcpy(b.ciudad,temp.ciudad);
}

void fun_quicksort(vector<Guardian> &arr, int low, int high){

    if(low < high){
        Guardian pivote = arr[high];
        int i = low - 1;

        for(int j = low; j <=  high - 1; j++){
            if(strcmp(arr[j].maestro, pivote.maestro) < 0 || (strcmp(arr[j].maestro, pivote.maestro) == 0 && strcmp(arr[j].nombre, pivote.nombre) < 0)){
                i++;
                intercambiarGuardianes(arr[i], arr[j]);
            }
        }
        intercambiarGuardianes(arr[ i + 1], arr[high]);
        int pi = i + 1;

        fun_quicksort(arr,low,pi - 1);
        fun_quicksort(arr,pi + 1 , high);
    }
}
*/
void Buscar_Agregar_Aprendiz(NodoJerarquico& nodo, Guardian& guard, bool& maestroEncontrado){
    if(strcmp(guard.maestro,nodo.guardian.nombre) == 0 || (strcmp(guard.maestro, "") == 0 && strcmp(guard.nombre, nodo.guardian.nombre) == 0 )){
        NodoJerarquico nodoAprendiz;
        nodoAprendiz.guardian = guard;
        nodo.aprendices.push_back(nodoAprendiz);
        maestroEncontrado = true;
    }
    for(auto& aprendiz : nodo.aprendices){
        Buscar_Agregar_Aprendiz(aprendiz,guard,maestroEncontrado);
        if(maestroEncontrado){
            break;
        }
    }

    if(!maestroEncontrado && ((strcmp(guard.maestro, nodo.guardian.nombre) == 0) ||  (strcmp(guard.maestro, "") == 0 &&  strcmp(guard.nombre, nodo.guardian.nombre) == 0))){
        NodoJerarquico nodoAprendiz;
        nodoAprendiz.guardian = guard;
        nodo.aprendices.push_back(nodoAprendiz);
        maestroEncontrado = true;
    }
}
vector<NodoJerarquico> cargarGuardianes(char archivoGuardianes[]) {
    vector<NodoJerarquico> jerarquia;
    vector<Guardian> guardianes;
    ifstream file(archivoGuardianes);
    char line[256];

    vector<TempGuardian> tempGuardianes;

    while (file.getline(line, sizeof(line))) {

        TempGuardian temp_Guardian;
        char* token = strtok(line,",");
        strcpy(temp_Guardian.guardian.nombre, token);

        token = strtok(nullptr,",");
        temp_Guardian.guardian.nivelPoder = atoi(token);

        token = strtok(nullptr, ",");
        strcpy(temp_Guardian.guardian.maestro, token);

        token = strtok(nullptr, ",");
        strcpy(temp_Guardian.guardian.ciudad, token);

        temp_Guardian.maestroEncontrado = false;

        tempGuardianes.push_back(temp_Guardian);
    }

    for(TempGuardian& temp_guardian : tempGuardianes){
        guardianes.push_back(temp_guardian.guardian);
    }

    for(TempGuardian& temp_Guardian : tempGuardianes){
        if(strcmp(temp_Guardian.guardian.maestro, "") != 0){
            bool maestroEncontrado = false;
            for(NodoJerarquico& nodo : jerarquia){
                Buscar_Agregar_Aprendiz(nodo,temp_Guardian.guardian, maestroEncontrado);
                if(maestroEncontrado){
                    break;
                }
            }
            if(!maestroEncontrado){
                cout << "Maestro no encontrado para: " << temp_Guardian.guardian.nombre <<" Se agregara como independiente" << endl;
                NodoJerarquico nuevoNodo;
                nuevoNodo.guardian = temp_Guardian.guardian;
                jerarquia.push_back(nuevoNodo);
            }
        }
    }
    return jerarquia;
}
                    //termino funciones de carga de ciudades y guardianes.

                    //arbol general para jerarquia de guardianes.


NodoJerarquico* agregar_guardian_Jerarquia(NodoJerarquico* root, Guardian &nuevoGuardian) {
    NodoJerarquico* nuevoNodo = new NodoJerarquico;
    nuevoNodo->guardian = nuevoGuardian;

    if (root) {
        for (size_t i = 0; i < root->aprendices.size(); i++)
        {
            NodoJerarquico* aprendiz = &(root->aprendices[i]);
            if (strcmp(aprendiz->guardian.nombre, nuevoGuardian.maestro) == 0) {
                aprendiz->aprendices.push_back(*nuevoNodo);
                return root;
            }
        }
    }
    NodoJerarquico* Nodomaestro = agregar_guardian_Jerarquia(root, nuevoGuardian);
    if (Nodomaestro) {
        Nodomaestro->aprendices.push_back(*nuevoNodo);
        return root;
    }
    else {
        return nuevoNodo;
    }
}
            //termino arbol general jerarquia guardianes.



void imprimirGrafoCiudades(vector<CiudadGrafo> &ciudades) {

    cout << "Grafo de ciudades:" << endl;

    for (auto& ciudad : ciudades) {
        cout << "Ciudad: " << ciudad.nombre << " Conexiones:";
        for (auto& conexion : ciudad.coneccionCiudad) {

            cout << conexion << " ";
        }
        cout << endl;
    }
    cout << endl;

}
                //basado en DFS, imprimimos los guardianes en jerarquia.
void imprimirJerarquia(NodoJerarquico& nodo, int nivel = 0) {
    for(int i = 0; i < nivel; i++){
        cout << " ";
    }

    if(nivel > 0){
        cout << " *Aprendiz: ";
    }

    cout << nodo.guardian.nombre << " Nivel de Poder: " << nodo.guardian.nivelPoder << " Ciudad Origen: " << nodo.guardian.ciudad<< " Maestro: " << nodo.guardian.maestro<<endl;

    for(auto& aprendiz : nodo.aprendices){
        imprimirJerarquia(aprendiz, nivel + 1);
    }
}



NodeRanking* createNode(int data) {
    NodeRanking* newNode = new NodeRanking;
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

NodeRanking* insertar(NodeRanking* root, int data) {

    if (root == NULL) {
        return createNode(data);
    }

    if (data < root->data) {
        root->left = insertar(root->left, data);
    }
    else {
        root->right = insertar(root->right, data);
    }

    return root;
}

void inorder(NodeRanking* root) {
    if (root == NULL) {
        return;
    }
    inorder(root->left);
    cout << root->data << " ";

    inorder(root->right);
}

/*

NodeRanking* BFS(NodeRanking* root, int value) {
    queue<NodeRanking*> q;
    q.push(root);
    while (!q.empty()) {
        NodeRanking* current = q.front();
        q.pop();
        if (current->data == value) {
            return current;
        }
        if (current->left != NULL) {
            q.push(current->left);
        }
        if (current->right != NULL) {
            q.push(current->right);
        }
    }
    return NULL;
}
*/

NodeRanking* deleteNode(NodeRanking* root, int key) {
    if (root == NULL) {
        return root;
    }
    if (key < root->data) {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->data) {
        root->right = deleteNode(root->right, key);
    }
    else {
        if (root->left == NULL) {
            NodeRanking* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == NULL) {
            NodeRanking* temp = root->left;
            delete root;
            return temp;
        }
        NodeRanking* temp = root->right;
        while (temp->left != NULL) {
            temp = temp->left;
        }
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}

int main() {

    char ciudadestexto[50] = "cities.conf";
    char guardianestexto[50] = "guardians.conf";

    vector<CiudadGrafo> ciudades = cargarCiudades(ciudadestexto);
    vector<NodoJerarquico> guardianes = cargarGuardianes(guardianestexto);

    imprimirGrafoCiudades(ciudades);

    cout << "A partir de aqui se imprimiran los guardianes:" << endl;
    for(auto& guardian: guardianes){
        imprimirJerarquia(guardian);
        cout << endl;
    }
    cout << "termino funcion";

    return 0;
}

