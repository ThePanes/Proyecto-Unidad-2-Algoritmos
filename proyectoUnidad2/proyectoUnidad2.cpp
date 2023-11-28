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

                        //estructuras a utilizar
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
    vector<Guardian> empates;
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
    return newNode;
}
NodeRanking* insertar(NodeRanking* root, Guardian& guardian) {
    if (root == nullptr) {
        return createNode(guardian);
    }
    /*
    if (strcmp(guardian.nombre, root->guardian.nombre) < 0) {
        root->left = insertar(root->left, guardian);
    }else if(strcmp(guardian.nombre, root->guardian.nombre) > 0){
        root->right = insertar(root->right, guardian);
    }else {
        root->puntos = guardian.nivelPoder;
    }*/
    if(guardian.nivelPoder < root->guardian.nivelPoder){
        root->left = insertar(root->left, guardian);
    }else if(guardian.nivelPoder > root->guardian.nivelPoder){
        root->right = insertar(root->right, guardian);
    }else{
        root->empates.push_back(guardian);
    }
    return root;
}
                        //funciones para crear el ranking de los guardianes.
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
                                    //temino funciones.
                                    //funcion cargar archivos ciudades y guardianes(de manera jerarquica).

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
*/


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
            //imprimiendo arbol de ranking con el metodo inorder, filtrando solo por aquellos guardianes que cumplan como candidatos(guardianes con nivel > 90)
void inorder(NodeRanking* root, vector<string>& guardianesActuales) {
    if (root == nullptr) {
        return;
    }
    inorder(root->left,guardianesActuales);
    bool esCandidato = root->guardian.nivelPoder >= 90 && root->guardian.nivelPoder <= 99;
    for(auto& guardianActual : guardianesActuales){
        if(root->guardian.nombre == guardianActual){
            cout << "Guardianes Actuales del reino(No candidatos, ya son guardianes):" << root->guardian.nombre << endl;
            esCandidato = false;
            break;
        }
    }
    if(esCandidato){
        cout << "Guardian seleccionado como candidato: "<< root->guardian.nombre  << " Nivel de Poder: "<<  root->guardian.nivelPoder << " " << endl;
    }
    inorder(root->right, guardianesActuales);
}
                //otra funcion inorder para ver que guardianes pueden batallar.
void inOrderRanking(NodeRanking* root, int& contador){

    if (root == nullptr) {
        return;
    }
    inOrderRanking(root->left,contador);
    bool esCandidato = root->guardian.nivelPoder < 90;
    bool CandidatoEmpates;
    for(Guardian& empateGuardian : root->empates){
      CandidatoEmpates   = empateGuardian.nivelPoder < 90;
    }
    if(esCandidato || CandidatoEmpates){
        cout << contador <<". Guardian Para Batallar Como candidato:" << root->guardian.nombre << " Nivel de Poder: " << root->guardian.nivelPoder<<  endl;

        contador++;

        for(Guardian& empateGuardian : root->empates){
          cout << contador << ". Guardian Para Batallar Como candidato:" << empateGuardian.nombre << " Nivel de Poder: " << empateGuardian.nivelPoder<<  endl;
            contador++;
        }
    }
    inOrderRanking(root->right,contador);
}

void ver_lista_candidatos(NodeRanking*& rankingRoot, vector<string>& guardianesActuales){

    cout << "Lista de Candidatos:" << endl;
    inorder(rankingRoot,guardianesActuales);
}

void buscarCiudadGuardian(NodoJerarquico& nodo, char* nombreGuardian, CiudadGrafo& ciudadGuardian){
    if(strcmp(nodo.guardian.nombre,nombreGuardian) == 0){
        strcpy(ciudadGuardian.nombre, nodo.guardian.ciudad);
        return;
    }
    for(auto& aprendiz : nodo.aprendices){
        buscarCiudadGuardian(aprendiz,nombreGuardian,ciudadGuardian);
        if(ciudadGuardian.nombre[0] != '\0'){
            return;
        }
    }
}
bool buscarGuardianJerarquia(){


}
int pelea(Guardian& elegido, Guardian& contrincante){
    if(elegido.nivelPoder < contrincante.nivelPoder){
        elegido.nivelPoder -=1;
        return 1;
    }else if(elegido.nivelPoder > contrincante.nivelPoder){
        elegido.nivelPoder += 1;
        return 2;
    }
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

/*NodeRanking* deleteNode(NodeRanking* root, int key) {
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
}*/

int main() {

    char ciudadestexto[50] = "cities.conf";
    char guardianestexto[50] = "guardians.conf";
    NodeRanking* rankingRoot = inicialRanking();

    int contador = 1;
    int eleccion;

    vector<CiudadGrafo> ciudades = cargarCiudades(ciudadestexto);
    vector<NodoJerarquico> guardianes = cargarGuardianes(guardianestexto);
    vector<string> guardianesActuales = {"Forseti","Magni","Skadi"};


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
                        ver_lista_candidatos(rankingRoot,guardianesActuales);
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                cout << "Seleccione un guardian para la batalla:" << endl;
                    inOrderRanking(rankingRoot,contador);

                cout << "Ingrese el numero del guardian a seleccionar:" << endl;
                    cin >> eleccion;

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

