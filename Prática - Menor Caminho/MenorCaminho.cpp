#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <limits>
using namespace std;

// Distancia inicial "muito grande"
const int INICIAL_DIST = numeric_limits<int>::max();

// Dijkstra 
void dijkstra(int origem, const vector<vector<pair<int,int>>>& grafo, vector<int>& dist) {
    int n = grafo.size();
    vector<bool> visitado(n, false);
    dist[origem] = 0;

    // Voces podem usar fila de prioridade, usando o priority_queue que tem referencia abaixo
    // priority_queue: lembre-se de colocar greater<> pois e a MENOR distância, c++ faz automatico
    // o maior numero em vez do menor.
    priority_queue<
        pair<int,int>,
        vector<pair<int,int>>,
        greater<pair<int,int>>
    > fila;

    fila.push({0, origem});

    while (!fila.empty()) {
        auto [distAtual, u] = fila.top();
        fila.pop();

        if (visitado[u]) continue;
        visitado[u] = true;

        for (auto [v, peso] : grafo[u]) {
            // Complete o relaxamento
            // if (dist[u] + c(u,v) < d[v]) d[v] = d[u] + c(u,v)
            if (dist[u] != INICIAL_DIST && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                fila.push({dist[v], v});
            }
        }
    }
}

// Bellman-Ford 
bool bellmanFord(int n, const vector<tuple<int,int,int>>& arestas, vector<int>& dist, int origem) {
    dist[origem] = 0;

    for (int i = 1; i < n; i++) {
        for (auto [u, v, w] : arestas) {
            // Relaxamento de cada par de aresta
            if (dist[u] != INICIAL_DIST && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }

    // Precisa-se de uma função para detectar ciclo negativo
    for (auto [u, v, w] : arestas) {
        if (dist[u] != INICIAL_DIST && dist[u] + w < dist[v]) {
            return false;
        }
    }

    return true;
}

int main() {

    int n, e;
    cin >> n >> e;

    vector<vector<pair<int,int>>> grafo(n+1);
    vector<tuple<int,int,int>> arestas;

    bool temNegativo = false;

    for (int i = 0; i < e; i++) {
        int u, v, w;
        cin >> u >> v >> w;

        grafo[u].push_back({v, w});
        arestas.push_back({u, v, w});

        if (w < 0) temNegativo = true;
    }

    int origem;
    cin >> origem;

    vector<int> dist(n+1, INICIAL_DIST);

    if (!temNegativo) {
        // Usa Dijkstra quando não tiver peso negativo
        dijkstra(origem, grafo, dist);
    } else {
        // Usa Bellman-Ford quando houver peso negativo
        bool ok = bellmanFord(n+1, arestas, dist, origem);
        if (!ok) {
            cout << "Ciclo negativo detectado" << endl;
            return 0;
        }
    }

    // Impressão das distâncias
    for (int i = 1; i <= n; i++) {
        if (dist[i] == INICIAL_DIST) cout << "INF";
        else cout << dist[i];

        if (i < n) cout << " ";
    }
    cout << endl;

    return 0;
}
