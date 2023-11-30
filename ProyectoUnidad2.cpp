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
    if(guardian.nivelPoder < root->guardian.nivelPoder){
        root->left = insertar(root->left, guardian);
    }else if(guardian.nivelPoder > root->guardian.nivelPoder){
        root->right = insertar(root->right, guardian);
    }else{
        root->right = insertar(root->right, guardian);
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

CiudadGrafo* buscarCiudad(vector<CiudadGrafo>& ciudades, string& nombreCiudad){
    for(auto& ciudad: ciudades){
        if(ciudad.nombre == nombreCiudad){
            return &ciudad;
        }
    }
    return nullptr;
}

bool existeConexion(CiudadGrafo& ciudad, string& otraCiudad){
    for(string& conexion : ciudad.coneccionCiudad){
        if(conexion == otraCiudad){
            return true;
        }
    }
    return false;
}

void agregarConexion(vector<CiudadGrafo>& ciudades, string& ciudadA, string& ciudadB){

    CiudadGrafo* ciudad_A = buscarCiudad(ciudades,ciudadA);
    CiudadGrafo* ciudad_B = buscarCiudad(ciudades,ciudadB);

    if(ciudad_A && ciudad_B){
        bool conexionExiste = false;
        for(auto& conexion : ciudad_A->coneccionCiudad){
            if(conexion == ciudadB){
                conexionExiste = true;
                break;
            }
        }

        if(!conexionExiste){
            ciudad_A->coneccionCiudad.push_back(ciudadB);
        }else{
            cout << "Ya existe conexion entre estas ciudades." << endl;
        }
    }else{
        cout << "Una de las ciudades no existe o no se encontro, no se pudo agregar conexion." << endl;
    }
}

void eliminarConexion(vector<CiudadGrafo>& ciudades, string& ciudadA, string& ciudadB){
    CiudadGrafo* ciudad_A = buscarCiudad(ciudades,ciudadA);
    CiudadGrafo* ciudad_B = buscarCiudad(ciudades,ciudadB);
    if(ciudad_A && ciudad_B){
        bool conexionEncontrada = false;
        for(int i = 0; i< ciudad_A->coneccionCiudad.size(); i++){
            if(ciudad_A->coneccionCiudad[i] == ciudadB){
                for(int j = i; j < ciudad_A->coneccionCiudad.size() - 1; j++){
                    ciudad_A->coneccionCiudad[j]  =ciudad_A->coneccionCiudad[j + 1];
                }
                ciudad_A->coneccionCiudad.pop_back();
                conexionEncontrada = true;
                cout << "Conexion Eliminada entre " << ciudadA << " y " << ciudadB <<endl;
                break;
            }
        }
        if(!conexionEncontrada){
            cout << "No existe conexion entre " << ciudadA << " y " << ciudadB << endl;
        }
    }else{
        cout << "Una de las ciudades no existe o no se encontro, no se pudo agregar conexion." << endl;
    }
}
                                    //funcion cargar archivos ciudades y guardianes(de manera jerarquica).

vector<CiudadGrafo> cargarCiudades(char archivoCiudades[]) {
    vector<CiudadGrafo> ciudades;
    ifstream file(archivoCiudades);
    char line[256];

    while (file.getline(line,sizeof(line))) {
        CiudadGrafo ciudad;
        char*  nombre_ciudad = strtok(line, ",");
        strcpy(ciudad.nombre,nombre_ciudad);
        bool ciudadExistente = false;
        bool ciudadConexionExiste = false;
        for(auto& c : ciudades){
            if(strcmp(c.nombre, ciudad.nombre) == 0){
                ciudadExistente = true;
                break;
            }
        }
        if(!ciudadExistente){
            ciudades.push_back(ciudad);
        }
        char* ciudad_coneccion = strtok(nullptr, ",");
        while (ciudad_coneccion != nullptr) {
            if(ciudadExistente){
                for(auto& c : ciudades){
                    if(strcmp(c.nombre, ciudad.nombre) == 0){
                            //ahora busqueda vecinos vector string ciudad.
                        for(auto& v : c.coneccionCiudad){
                            string vnombre(v);
                            if(vnombre == ciudad_coneccion){
                                ciudadConexionExiste = true;
                        break;
                            }
                        }
                    }
                }
            }

                //si no existe la ciudad, se agrega.
            if(!ciudadConexionExiste){
                ciudades.back().coneccionCiudad.push_back(ciudad_coneccion);
            }else{
                ciudades.back().coneccionCiudad.push_back(ciudad_coneccion);
            }
            //actualizamos el nombre de la ciudad.
            ciudad_coneccion = strtok(nullptr,",");
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

void imprimirGrafoCiudades(vector<CiudadGrafo> &ciudades) {
    int contador = 1;
    cout << "Grafo de ciudades:" << endl;

    for (auto& ciudad : ciudades) {
        cout << contador << ". Ciudad: " << ciudad.nombre << " Conexiones:";
        for (auto& conexion : ciudad.coneccionCiudad) {
            cout << conexion << " ";
        }
        cout << endl;
        contador++;
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
NodeRanking* buscarGuardianRanking(NodeRanking* root, Guardian& guardian){
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
    if(esCandidato){
        cout << contador <<". Guardian Para Batallar Como candidato:" << root->guardian.nombre << endl;

        contador++;
    }
    inOrderRanking(root->right,contador);
}

void ver_lista_candidatos(NodeRanking*& rankingRoot, vector<string>& guardianesActuales){

    cout << "Lista de Candidatos:" << endl;
    inorder(rankingRoot,guardianesActuales);
}

void buscarGuardianesEnCiudad(NodoJerarquico& nodo, char* nombreGuardian, char* ciudadGuardian){
    if(strcmp(nodo.guardian.ciudad,ciudadGuardian) == 0){
        cout << "Guardian en la ciudad:" << nombreGuardian;
        return;
    }
    for(auto& aprendiz : nodo.aprendices){
        buscarGuardianesEnCiudad(aprendiz,nombreGuardian,ciudadGuardian);

    }
}

Guardian* buscarGuardianNumero(NodeRanking* root, int& contador, int& eleccion){

    if (root == nullptr) {
        return nullptr;
    }
    Guardian* resultado = nullptr;
    resultado  = buscarGuardianNumero(root->left,contador,eleccion);

    if(resultado != nullptr){
        return resultado;
    }
    contador ++;
    if(contador == eleccion){
        return &root->guardian;
    }
    return buscarGuardianNumero(root->right,contador,eleccion);
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


int main() {

    char ciudadestexto[50] = "cities.conf";
    char guardianestexto[50] = "guardians.conf";
    NodeRanking* rankingRoot = inicialRanking();

    int contador = 1;
    int contador2 = 0;
    int eleccion;
    int eleccionReinos;
    Guardian* guardianElegido  = nullptr;
    Guardian* eleccionVerDatos = nullptr;

    int op2 , op3 ,op4 , op5;
        //carga.
    vector<CiudadGrafo> ciudades = cargarCiudades(ciudadestexto);
    vector<NodoJerarquico> guardianes = cargarGuardianes(guardianestexto);
    vector<string> guardianesActuales = {"Forseti","Magni","Skadi"};
        //ranking.
    cargarGuardianesArboles(guardianes,rankingRoot);

    int opcion;
    do{
        cout << "Menu De The Guardians Battle" << endl;
        cout << "1. Ver la lista de candidatos." << endl;
        cout << "2. Ver al guardian." << endl;
        cout << "3. Conocer el reino." << endl;
        cout << "4. Presenciar una Batalla." << endl;
        cout << "5. Imprimir lista de todos los guardianes en jerarquia de maestros." << endl;
        cout << "6. Salir" << endl;
        cout << "Eliga una opcion:" << endl;
        cin >> opcion;
        switch(opcion){
            case 1:
                    cout << "Guardianes elegidos como posibles candidatos a Guardianes del Reino:" << endl;
                        ver_lista_candidatos(rankingRoot,guardianesActuales);
                break;
            case 2:
                    contador = 1;
                    contador2 = 0;
                    cout << "Seleccione un guardian para la ver los datos:" << endl;
                        inOrderRanking(rankingRoot,contador);
                    cout << "Ingrese el numero del guardian a seleccionar:" << endl;
                        cin >> eleccion;
                    eleccionVerDatos = buscarGuardianNumero(rankingRoot,contador2,eleccion);

                    if(eleccionVerDatos != nullptr){
                        cout << "Elegiste al guardian: " << eleccionVerDatos->nombre << endl;
                        cout << "Ciudad del Guardian: " << eleccionVerDatos->ciudad <<endl;
                        cout << "Maestro del Guardian: " << eleccionVerDatos->maestro << endl;
                        cout << "Nivel de Poder del Guardian: " <<eleccionVerDatos->nivelPoder << endl;

                    }else{
                        cout << "Seleccion Invalida, reintente." << endl;
                    }

                break;
            case 3:
                    do{
                        cout << "Menu de Conocer el Reino:" << endl;
                        cout << "1. Ver el reino"<<endl;
                        cout << "2. Agregar camino entre ciudades" << endl;
                        cout << "3. Eliminar camino entre ciudades" << endl;
                        cout << "4. Salir." <<endl;
                        cout << "Seleccione una opcion:" << endl;
                        cin >> eleccionReinos;
                        switch(eleccionReinos){
                            case 1:
                                cout << "Reino Completo, aqui estan todas las ciudades y sus conexiones:" << endl;
                                imprimirGrafoCiudades(ciudades);
                                break;
                            case 2:
                                cout << "Elija la ciudad que quiere agregarle una conexion con otra ciudad:" << endl;
                                cout << "Primero elija la ciudad de origen:" << endl;
                                imprimirGrafoCiudades(ciudades);
                                cout << "Eleccion:" << endl;
                                cin >> op2;

                                cout << "Ahora, elija la ciudad de destino:" << endl;
                                imprimirGrafoCiudades(ciudades);
                                cout << "Eleccion:" << endl;
                                cin >> op3;

                                if(op2 >= 1 && op2 <=ciudades.size() && op3 >= 1 && op3 <=ciudades.size()){
                                    string ciudadA = ciudades[op2 - 1].nombre;
                                    string ciudadB = ciudades[op3 - 1].nombre;
                                        cout << "Ahora realizaremos la conexion entre ambos ciudades:" <<endl;
                                        agregarConexion(ciudades,ciudadA,ciudadB);
                                }else{
                                    cout << "Seleccion Invalida. Reintente." << endl;
                                }
                                break;
                            case 3:
                                cout << "Elija la ciudad que quiere eliminarle una conexion con otra ciudad:" << endl;
                                cout << "Primero elija la ciudad de origen:" << endl;
                                imprimirGrafoCiudades(ciudades);
                                cout << "Eleccion:" << endl;
                                cin >> op4;
                                cout << "Ahora, elija la ciudad de destino:" << endl;
                                imprimirGrafoCiudades(ciudades);
                                cout << "Eleccion:" << endl;
                                cin >> op5;

                                if(op4 >= 1 && op4 <=ciudades.size() && op5 >= 1 && op5 <=ciudades.size()){
                                    string ciudadA = ciudades[op4 - 1].nombre;
                                    string ciudadB = ciudades[op5 - 1].nombre;
                                    cout << "Ahora realizaremos la eliminacion de conexion entre ambos ciudades:" <<endl;
                                    eliminarConexion(ciudades,ciudadA,ciudadB);
                                }else{
                                    cout << "Seleccion Invalida. Reintente." << endl;
                                }
                                break;
                            case 4:
                                cout << "Volviendo Al Menu Principal" <<endl;
                                break;
                            default:
                                cout << "Opcion Invalida, reintente." << endl;
                                break;
                        }

                    }while(eleccionReinos != 4);


                break;
            case 4:
                    contador = 1;
                    contador2 = 0;
                    cout << "Seleccione un guardian para la batalla:" << endl;
                        inOrderRanking(rankingRoot,contador);
                    cout << "Ingrese el numero del guardian a seleccionar:" << endl;
                        cin >> eleccion;
                    guardianElegido = buscarGuardianNumero(rankingRoot,contador2,eleccion);

                    if(guardianElegido != nullptr){
                        cout << "Elegiste al guardian: " << guardianElegido->nombre <<" Para la Batalla." << endl;
                        cout << "De la ciudad: " << guardianElegido->ciudad <<endl;
                        //falta seguir impleamentando logica para batalla.

                    }else{
                        cout << "Seleccion Invalida, reintente." << endl;
                    }

                break;
            case 5:
                for(auto& guardian: guardianes){
                    imprimirJerarquia(guardian);
                    cout << endl;
                }
                break;
            case 6:
                cout << "Termino de Funcion." << endl;
                break;
            default:
                cout << "Opcion Invalida, reintente." << endl;
                break;
        }
    }while(opcion !=6 );

    return 0;
}

