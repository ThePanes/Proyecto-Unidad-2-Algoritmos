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
    NodeRanking* left;
    NodeRanking* right;
    int puntos;
};

NodeRanking* inicialRanking(){
    return nullptr;
}

struct TempGuardian{
    Guardian guardian;
    bool maestroEncontrado;
};

            //lista de candidatos a guardianes del reino / ranking de candidatos en base a arbol.

NodeRanking* createNode(Guardian& guardian) {
    NodeRanking* newNode = new NodeRanking;
    newNode->guardian = guardian;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->puntos = guardian.nivelPoder;
    return newNode;
}
NodeRanking* insertar(NodeRanking* root, Guardian& guardian) {
    if (root == nullptr) {
        return createNode(guardian);
    }
    if (strcmp(guardian.nombre, root->guardian.nombre) < 0) {
        root->left = insertar(root->left, guardian);
    }else if(strcmp(guardian.nombre, root->guardian.nombre) > 0){
        root->right = insertar(root->right, guardian);
    }else {
        root->puntos = guardian.nivelPoder;
    }
    return root;
}

void cargarGuardianesRecursivo(NodeRanking*& rankingRoot, NodoJerarquico& nodo){
    rankingRoot = insertar(rankingRoot, nodo.guardian);
    for(auto& aprendiz : nodo.aprendices){
        cargarGuardianesRecursivo(rankingRoot,aprendiz);
    }
}

void cargarGuardianesArboles(vector<NodoJerarquico>&jerarquia, NodeRanking*& rankingRoot){
    for(auto& nodo : jerarquia){
        cargarGuardianesRecursivo(rankingRoot,nodo);
    }
}
                                    //temino funcion.
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



/*NodeRanking* buscarGuardianRanking(NodeRanking* root, Guardian& guardian){
    if(root == nullptr || strcmp(root->guardian.nombre, guardian.nombre) == 0){
        return root;
    }
    cout << "Buscando en el nodo " << root->guardian.nombre << endl;
    if(strcmp(guardian.nombre, root->guardian.nombre) < 0){
        return buscarGuardianRanking(root->left,guardian);
    }else{
        return buscarGuardianRanking(root->right,guardian);
    }
}
*/
            //imprimiendo arbol de ranking con el metodo inorder, filtrando solo por aquellos guardianes que cumplan como candidatos
void inorder(NodeRanking* root) {
    if (root == nullptr) {
        return;
    }
    inorder(root->left);
    if(root->guardian.nivelPoder >= 90 && root->guardian.nivelPoder <= 99){
        cout << "Guardian seleccionado como candidato: "<< root->guardian.nombre  << " Nivel de Poder: "<<  root->guardian.nivelPoder << " " << endl;
    }
    inorder(root->right);
}

/*void ver_lista_candidatos(vector<NodoJerarquico>& guardianes, NodeRanking*& rankingRoot){

    cout << "Lista de candidatos:" << endl;

    inorder(rankingRoot);

    for(auto& nodo: guardianes){
        if(nodo.guardian.nivelPoder >= 90 && nodo.guardian.nivelPoder <= 99){
            cout << "Seleccionado: " << nodo.guardian.nombre << " Nivel de Poder: " << nodo.guardian.nivelPoder;

            NodeRanking* puntajeGuardian = buscarGuardianRanking(rankingRoot, nodo.guardian);

            if(puntajeGuardian){
                cout << "Puntos: " << puntajeGuardian->puntos;
            }
            cout << endl;
        }
    }
    cout << "Termino la funcion Lista Candidatos" << endl;
}
*/



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
    if (key < root->puntos) {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->puntos) {
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
        root->puntos = temp->puntos;
        root->right = deleteNode(root->right, temp->puntos);
    }
    return root;
}

int main() {

    char ciudadestexto[50] = "cities.conf";
    char guardianestexto[50] = "guardians.conf";
    NodeRanking* rankingRoot = inicialRanking();

    vector<CiudadGrafo> ciudades = cargarCiudades(ciudadestexto);
    vector<NodoJerarquico> guardianes = cargarGuardianes(guardianestexto);

    cargarGuardianesArboles(guardianes,rankingRoot);
    int opcion;

    do{
        cout << "Menu De The Guardians Battle" << endl;
        cout << "1. Ver la lista de candidatos." << endl;
        cout << "2. Ver al guardian." << endl;
        cout << "3. Conocer el reino." << endl;
        cout << "4. Presenciar una Batalla." << endl;
        cout << "5. Imprimir grafo de todas las ciudades." << endl;
        cout << "6. Imprimir lista de todos los guardianes en jerarquia de maestros." << endl;
        cout << "7. Salir" << endl;
        cout << "Eliga una opcion:" << endl;
        cin >> opcion;
        switch(opcion){
            case 1:
                    cout << "Guardianes elegidos como posibles candidatos a Guardianes del Reino:" << endl;
                    inorder(rankingRoot);
                    //ver_lista_candidatos(guardianes,rankingRoot);
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                imprimirGrafoCiudades(ciudades);
                break;
            case 6:
                for(auto& guardian: guardianes){
                    imprimirJerarquia(guardian);
                    cout << endl;
                }
                break;
            case 7:
                cout << "Termino de Funcion." << endl;
                break;
            default:
                cout << "Opcion Invalida, reintente." << endl;
                break;
        }
    }while(opcion !=7 );

    return 0;
}

