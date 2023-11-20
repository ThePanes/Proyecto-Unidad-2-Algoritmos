#include <iostream>
#include <stdio.h>
#include <queue>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <time.h>

using namespace std;

struct CiudadGrafo {
    string nombre;
    vector <string> coneccionCiudad;
};

struct Guardian {
    char nombre[50];
    int nivelPoder;
    char maestro[50];
    char ciudad[50];
};

struct NodeRanking {
    Guardian guardian;
    int data;
    NodeRanking* left;
    NodeRanking* right;
};

                    //arbol general para jerarquia de guardianes.

struct NodoJerarquico {
    Guardian guardian;
    vector<NodoJerarquico> aprendices;
};
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

/*NodeRanking* DFS(NodeRanking* root, int value) {

    if (root == NULL || root->data == value) {
        return root;
    }

    NodeRanking* left_result = DFS(root->left, value);
    if (left_result != NULL) {
        return left_result;
    }
    NodeRanking* right_result = DFS(root->right, value);
    if (right_result != NULL) {
        return right_result;
    }
    return NULL;
}

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

    /*
    int numeros[] = {21,18,6,9,10,7,19,15,12,5};
    int n = sizeof(numeros) / sizeof(numeros[0]);

    NodeRanking* root = NULL;

    for (int i = 0; i < n; i++) {
        root = insertar(root, numeros[i]);
    }

    cout << "In Order" << endl;
    inorder(root);
    cout << endl;

    int numerobuscadoDFS = 18;

    NodeRanking* resultanteDFS = DFS(root, numerobuscadoDFS);

    cout << "Busqueda por profundidad." << endl;

    if (resultanteDFS != NULL) {
        cout << "Numero encontrado:" << resultanteDFS->data << endl;
    }
    else {
        cout << "No encontrado" << endl;
    }

    int numerobuscadoBFS = 7;
    NodeRanking* resultanteBFS = BFS(root, numerobuscadoBFS);
    cout << "Busqueda por Amplitud." << endl;
    if (resultanteBFS != NULL) {
        cout << "Numero encontrado:" << resultanteBFS->data << endl;
    }
    else {
        cout << "No encontrado" << endl;
    }
    */

    return 0;
}

