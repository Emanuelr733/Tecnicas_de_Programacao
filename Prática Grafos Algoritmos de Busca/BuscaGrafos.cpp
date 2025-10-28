#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

enum Cor { WHITE, GRAY, BLACK };

class Grafo {
public:
    int V;
    vector<vector<int>> adj; // lista de adjacencia
    int tempo; //inicializa como atributo como recomendado pelo slide
    Grafo(int vertices) {
        V = vertices;
        adj.resize(V);
        tempo = 0;
    }
    void adicionarAresta(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // pq nao e direcionado
    }
    void BFS(int s) {
        vector<Cor> color(V);
        vector<int> d(V);
        vector<int> pi(V);
        for (int u = 0; u < V; u++) {  //Todos os vertices,
            if (u != s) {              //exceto a origem s,
                color[u] = WHITE;      //ficam brancos,
                d[u] = INT_MAX;        //a distancia e infinita e
                pi[u] = -1;            //o predecessor e nulo.
            }
        }
        color[s] = GRAY;               //Inicialização dos dados da origem, a cor e cinza (vamos visita-lo para percorrer seus vizinhos),
        d[s] = 0;                      //a distancia e zero e
        pi[s] = -1;                    //o predecessor e nulo (nao ha arestas para chegar nesse vertice, ja que vamos começar por ele).
        queue<int> Q;                  //Inicializamos uma fila vazia,
        Q.push(s);                     //a fila recebe primeiro a origem
        cout << "\nBusca em largura" << endl;
        cout << "Ordem de visita: ";
        while (!Q.empty()) {           //e o algoritmo executara enquanto a fila nao estiver vazia.
            int u = Q.front();         //Removemos o vertice que esta na frente da fila,
            Q.pop();                   //que chamamos de u.
            cout << u << " ";
            for (int v : adj[u]) {     //Depois inicia-se um laco que vai analisar cada um dos vertices vizinhos de u.
                if (color[v] == WHITE) {
                    color[v] = GRAY;   //A cada passagem do laco,
                    d[v] = d[u] + 1;   //vamos aumentando o raio da distancia a partir de s em 1, ou seja,
                    pi[v] = u;         //primeiro os alcancaveis de 1 aresta ate n, ate processar todo o grafo.
                    Q.push(v);
                }
            }
            color[u] = BLACK;
        }
        cout << "\n\nDistancias (d):" << endl;
        for (int i = 0; i < V; i++)
            cout << "Vertice " << i << ": " << (d[i] == INT_MAX ? -1 : d[i]) << endl;
        cout << "\nPredecessores (pi):" << endl;
        for (int i = 0; i < V; i++)
            cout << "Vertice " << i << ": " << (pi[i] == -1 ? -1 : pi[i]) << endl;
    }
    void DFS_VISIT(int u, vector<Cor>& color, vector<int>& pi,vector<int>& d, vector<int>& f) {
        tempo++;                       //Em cada chamada de DFS-VISIT, o vertice u e branco no comeco.
        d[u] = tempo;                  //Incremente o tempo e atribui esse tempo de descobrimento ao u e depois
        color[u] = GRAY;               //o marca com o cinza.
        cout << u << " ";              //mostra os vértice descoberto.
        for (int v : adj[u]) {         //sai procurando os que ainda estao brancos pra visitar, processar em cinza e depois preto.
            if (color[v] == WHITE) {
                pi[v] = u;
                DFS_VISIT(v, color, pi, d, f);
            }
        }
        color[u] = BLACK;              //terminou de processar u e deixa ele preto.
        tempo++;
        f[u] = tempo;
    }
    void DFS() {
        vector<Cor> color(V);
        vector<int> pi(V);
        vector<int> d(V);
        vector<int> f(V);
        for (int u = 0; u < V; u++) {
            color[u] = WHITE;          //Inicializacao de todos os vertices para a cor branca e
            pi[u] = -1;                //a indicacao que o predecessor e nulo.
        }
        tempo = 0;
        cout << "\nBusca em profundidade" << endl;
        cout << "Ordem de visita: ";
        for (int u = 0; u < V; u++) {
            if (color[u] == WHITE)
                DFS_VISIT(u, color, pi, d, f);
        }
    }
};
int main() {
    int V, E;
    cout << "Digite o numero de vertices: ";
    cin >> V;
    cout << "Digite o numero de arestas: ";
    cin >> E;
    Grafo g(V);
    cout << "Digite as arestas (u v):" << endl;
    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        g.adicionarAresta(u, v);
    }
    int opcao = -1;
    while (opcao != 0) {
        cout << "\n1 pra executar a BFS" << endl;
        cout << "2 pra executar a DFS" << endl;
        cout << "0 pra sair" << endl;
        cin >> opcao;
        if (opcao == 1) {
            int inicio;
            cout << "Digite o vertice de inicio: ";
            cin >> inicio;
            g.BFS(inicio);
        }
        else if (opcao == 2) {
            g.DFS();
        }
    }
    return 0;
}
