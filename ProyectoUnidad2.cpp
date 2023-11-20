#include <iostream>
#include <stdio.h>
#include <queue>
#include <vector>

using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
};
void initFromText(Carta** primeraCarta) {
    //solo funcionara si el texto en la misma carpeta tiene el nombre guardians.txt
    char* archivito = "guardians.txt";
    FILE* file = fopen(archivito, "r");
    if (file == NULL) return;
    char line[MAX_LINE_LENGTH]; //MAX_LINE_LENGTH es un valor global que definen ustedes
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        Carta* nuevaCartita = (Carta*)malloc(sizeof(Carta));
        char* nombreCartitaNueva = strtok(line, ",");
        char* tipoCartitaNueva = strtok(NULL, ",");
        strcpy(nuevaCartita->nombreCarta, nombreCartitaNueva);
        strcpy(nuevaCartita->tipoCarta, tipoCartitaNueva);
        //atoi para valores enteros
        nuevaCartita->puntosVida = atoi(strtok(NULL, ",")); //atof para valores decimales
        nuevaCartita->puntosAtaque = atoi(strtok(NULL, ","));
        nuevaCartita->puntosDefensa = atoi(strtok(NULL, ","));
        nuevaCartita->siguiente = NULL;
        // Se añade el Struct creado a la lista existente.
        add_Cartita_Mazo(primeraCarta, nuevaCartita);
    }
    fclose(file);
}
Node* createNode(int data) {
    Node* newNode = new Node;
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node* insertar(Node* root, int data) {

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

void inorder(Node* root) {
    if (root == NULL) {
        return;
    }
    inorder(root->left);
    cout << root->data << " ";

    inorder(root->right);
}

Node* DFS(Node* root, int value) {

    if (root == NULL || root->data == value) {
        return root;
    }

    Node* left_result = DFS(root->left, value);
    if (left_result != NULL) {
        return left_result;
    }
    Node* right_result = DFS(root->right, value);
    if (right_result != NULL) {
        return right_result;
    }
    return NULL;
}

Node* BFS(Node* root, int value) {
    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* current = q.front();
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

Node* deleteNode(Node* root, int key) {
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
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == NULL) {
            Node* temp = root->left;
            delete root;
            return temp;
        }
        Node* temp = root->right;
        while (temp->left != NULL) {
            temp = temp->left;
        }
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}
int main() {

    int numeros[] = { 21,18,6,9,10,7,19,15,12,5 };
    int n = sizeof(numeros) / sizeof(numeros[0]);

    Node* root = NULL;

    for (int i = 0; i < n; i++) {
        root = insertar(root, numeros[i]);
    }

    cout << "In Order" << endl;
    inorder(root);
    cout << endl;

    int numerobuscadoDFS = 18;

    Node* resultanteDFS = DFS(root, numerobuscadoDFS);

    cout << "Busqueda por profundidad." << endl;

    if (resultanteDFS != NULL) {
        cout << "Numero encontrado:" << resultanteDFS->data << endl;
    }
    else {
        cout << "No encontrado" << endl;
    }

    int numerobuscadoBFS = 7;
    Node* resultanteBFS = BFS(root, numerobuscadoBFS);
    cout << "Busqueda por Amplitud." << endl;
    if (resultanteBFS != NULL) {
        cout << "Numero encontrado:" << resultanteBFS->data << endl;
    }
    else {
        cout << "No encontrado" << endl;
    }

    return 0;
}
