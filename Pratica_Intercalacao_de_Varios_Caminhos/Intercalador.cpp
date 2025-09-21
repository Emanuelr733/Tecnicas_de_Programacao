//Faça uma simulação de ordenação externa. A entrada deve ser um vetor grande de registros.
//No código você devera mostrar a divisão do vetor em blocos que cabem na memória (por exemplo de tamanho 3)
//Ordene na memória, ou seja use um algoritmo de ordenação interna como o MergeSort.
//Grave cada corrida em uma fita. Repetir até restar apenas uma corrida final.
//E por último, fazer a intercalação sempre comparando cabeçalhos e escrevendo nas fitas.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

void imprimirFita(const string& frase, const vector<int>& fita) {
    cout << frase;
    for (int x : fita) {
        cout << x << " ";
    }
    cout << endl;
}
vector<vector<int>> gerarCorridas(const vector<int>& vetor, int tamMemoria) {
    cout << "Fase 1, gerando as corridas" << endl;
    vector<vector<int>> corridas;
    for (int i = 0; i < vetor.size(); i += tamMemoria) {
        vector<int> bloco(vetor.begin() + i,vetor.begin() + min((int)vetor.size(), i + tamMemoria));
        imprimirFita("Bloco lido: ", bloco);
        sort(bloco.begin(), bloco.end());
        imprimirFita("Fita " + to_string(corridas.size() + 1) + " criada: ", bloco);
        cout << endl;
        corridas.push_back(bloco);
    }
    return corridas;
}
vector<int> intercala(vector<vector<int>> corridas) {
    if (corridas.empty()) {return {};}
    cout << "Fase 2, intercalacao de corridas";
    int passadaNum = 1;
    while (corridas.size() > 1) {
        cout << "\nPassada " << passadaNum << endl;
        vector<vector<int>> novasCorridas;
        for (int i = 0; i < corridas.size(); i += 2) {
            if (i + 1 < corridas.size()) {
                imprimirFita("Lendo a fita " + to_string(i + 1) + ": ", corridas[i]);
                imprimirFita("Lendo a fita " + to_string(i + 2) + ": ", corridas[i+1]);
                int tamanhoTotal = corridas[i].size() + corridas[i+1].size();
                vector<int> resultado(tamanhoTotal);
                merge(corridas[i].begin(), corridas[i].end(),corridas[i+1].begin(), corridas[i+1].end(),resultado.begin());
                imprimirFita("Nova fita gerada: ", resultado);
                novasCorridas.push_back(resultado);
            } else {
                imprimirFita("Copiando a fita " + to_string(i + 1) + ": ", corridas[i]);
                novasCorridas.push_back(corridas[i]);
            }
        }
        corridas = move(novasCorridas);
        passadaNum++;
    }
    return corridas[0];
}
int main() {
    vector<int> vetor = {42, 7, 15, 3, 20, 11, 8, 1, 5};
    int tamMemoria = 3;
    for (int x : vetor) {cout << x << " ";}
    cout << endl;
    auto corridas = gerarCorridas(vetor, tamMemoria);
    vector<int> resultado = intercala(corridas);
    cout << endl;
    imprimirFita("Fita final: ", resultado);
    return 0;
}
