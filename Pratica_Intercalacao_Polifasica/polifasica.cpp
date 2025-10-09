//Faça a Prática Intercalação de Vários Caminhos ser agora uma Intercalação Polifásica, ou seja,
//use agora um heap para poder fazer a seleção por substituição e depois intercalar os blocos.
//Podem usar std::priority_queue, std::ifstream e ofstream (para simulação de fita).
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

struct ElementoHeap {
    int valor;
    bool corridaAtual; //se e da corrida agora ou da depois
    bool operator>(const ElementoHeap& outro) const { //pra comparar dois ElementoHeap
        return valor > outro.valor;
    }
};
void imprimirVetor(const string& frase, const vector<int>& v) {
    cout << frase;
    for (int i : v) { //imprime todos os valores do vetor
        cout << i << " ";
    }
    cout << endl;
}
vector<int> lerFita(const string& fita) { //recebe o nome do arquivo
    vector<int> resultado; //vetor vazio
    ifstream arquivo(fita); //cria objeto input file stream chamado arquivo
    int valor; //variavel que guarda temporariamente cada valor do arquivo
    while (arquivo >> valor) { //ve cada numero do arquivo e adiciona no vetor valor
        resultado.push_back(valor);
    }
    arquivo.close(); //fecha o arquivo pra liberar recurso
    return resultado;
}
void escreverFita(const string& fita, const vector<int>& dados) { //recebe o nome do arquivo e vetor pra gravar
    ofstream arquivo(fita); //cria o objeto
    for (int i : dados) {
        arquivo << i << " "; //escreve o valor no arquivo separado por um espaco
    }
    arquivo.close(); //fecha o arquivo
}
vector<string> gerarCorridasComHeap(const vector<int>& vetor, int tamMemoria) { //vetor com os valores pra ordenar e o tamanho da memoria
    cout << "geracao de sorridas com selecao por substituicao" << endl;
    vector<string> arquivosCorridas; //guarda os nomes das fitas criadas
    int i = 0;
    int numCorrida = 0;
    priority_queue<ElementoHeap, vector<ElementoHeap>, greater<ElementoHeap>> heapAtual; //mantem os elementos pra continuar na corrida atual,grater faz funcionar como min
    priority_queue<ElementoHeap, vector<ElementoHeap>, greater<ElementoHeap>> heapProxima; //mantem os elementos para proxima corrida
    while (i < vetor.size() || !heapAtual.empty() || !heapProxima.empty()) { //continua enquanto algum heap nao estiver vazio
        cout << "\nCorrida " << (numCorrida + 1) << endl;
        vector<int> corridaAtual;
        int ultimoValorEscrito = -1;
        if (heapAtual.empty()) { //se o heap atual ja acabou
            if (!heapProxima.empty()) { //se ja tem no da proxima corrida vira o novo atual
                cout << "Usando elementos da memoria (proxima corrida): " << heapProxima.size() << endl;
                heapAtual = heapProxima;
                heapProxima = priority_queue<ElementoHeap, vector<ElementoHeap>, greater<ElementoHeap>>();
            } else { //carrega tamMemoria elementos do vetor de entrada pro heapatual
                cout << "Carregando memoria inicial:" << endl;
                for (int j = 0; j < tamMemoria && i < vetor.size(); j++) {
                    heapAtual.push({vetor[i], true});
                    cout << "  " << vetor[i];
                    i++;
                }
                cout << endl;
            }
        }
        while (!heapAtual.empty() || !heapProxima.empty()) { //enquanto tiver elementos
            if (heapAtual.empty()) { //para se ficar vazio
                break;
            }
            ElementoHeap menor = heapAtual.top(); //coloca o menor na corrida atual
            heapAtual.pop();
            corridaAtual.push_back(menor.valor);
            ultimoValorEscrito = menor.valor; //atualiza o ultimo valor pra controlar a ordenacao
            cout << "Escrevendo: " << menor.valor << endl;
            if (i < vetor.size()) { //le o proximo valor de entrada
                int novoValor = vetor[i++];
                cout << "Lendo novo valor: " << novoValor;
                if (novoValor >= ultimoValorEscrito) { //se mantem a ordem vai pro heapAtual
                    heapAtual.push({novoValor, true});
                    cout << " -> heap atual" << endl;
                } else { //se quebra a ordem vai pro proxima
                    heapProxima.push({novoValor, false});
                    cout << " -> proxima corrida" << endl;
                }
            }
        }
        string nomeArquivo = "fita_" + to_string(numCorrida); //cria o nome do arquivo pra corrida
        escreverFita(nomeArquivo, corridaAtual); //escreve a corrida no arquivo
        arquivosCorridas.push_back(nomeArquivo); //adiciona o nome na lista de corridas
        imprimirVetor("Fita " + to_string(numCorrida + 1) + " criada: ", corridaAtual); //imprime o vetor
        numCorrida++;
    }
    return arquivosCorridas;
}
string intercalarComHeap(vector<string> arquivosCorridas) { //recebe o vetor com os nomes nas corridas
    cout << "\nIntercalacao polifasica com heap" << endl;
    int passada = 1;
    while (arquivosCorridas.size() > 1) { //continua enquanto tiver mais de uma corrida
        cout << "\nPassada " << passada << endl;
        vector<string> novosArquivos;
        for (int i = 0; i < arquivosCorridas.size(); i += 2) {
            if (i + 1 < arquivosCorridas.size()) {
                cout << "\nIntercalando " << arquivosCorridas[i] << " e " << arquivosCorridas[i+1] << endl;
                vector<int> fita1 = lerFita(arquivosCorridas[i]); //le as fitas pra intercalar
                vector<int> fita2 = lerFita(arquivosCorridas[i+1]);
                imprimirVetor("  Fita 1: ", fita1); //imprime o vetor
                imprimirVetor("  Fita 2: ", fita2);
                priority_queue<ElementoHeap, vector<ElementoHeap>, greater<ElementoHeap>> heap; //cria o heap minimo pra extrair o menor valor
                vector<int> resultado;
                int idx1 = 0, idx2 = 0; //controlam o proximo elemento de cada fila
                if (idx1 < fita1.size()) { //colocam o primeiro elemento de cada fita no heap
                    heap.push({fita1[idx1++], true});
                }
                if (idx2 < fita2.size()) {
                    heap.push({fita2[idx2++], false});
                }
                while (!heap.empty()) { //loop de intercalacao com heap
                    ElementoHeap menor = heap.top(); //tira o menor valor do heap
                    heap.pop();
                    resultado.push_back(menor.valor); //e coloca no vetro resultado
                    if (menor.corridaAtual && idx1 < fita1.size()) { //le o proximo elemento da mesma fita do menor e coloca no heap
                        heap.push({fita1[idx1++], true});
                    } else if (!menor.corridaAtual && idx2 < fita2.size()) {
                        heap.push({fita2[idx2++], false});
                    }
                }
                string novoArquivo = "fita_p" + to_string(passada) + "_" + to_string(novosArquivos.size()); //novo arquivo para corrida intercalada dessa passada
                escreverFita(novoArquivo, resultado); //escreve na fita
                novosArquivos.push_back(novoArquivo); //adiciona a lista de corridas
                imprimirVetor("  Resultado: ", resultado); //e imprime ela
            } else {
                cout << "\nCopiando " << arquivosCorridas[i] << endl;
                novosArquivos.push_back(arquivosCorridas[i]);
            }
        }
        arquivosCorridas = novosArquivos; //atualiza com os arquivos gerados
        passada++;
    }
    return arquivosCorridas[0];
}
int main() {
    vector<int> vetor = {42, 7, 15, 3, 20, 11, 8, 1, 5, 25, 18, 30, 2, 9};
    int tamMemoria = 4;
    cout << "Vetor de entrada: ";
    imprimirVetor("", vetor);
    cout << "Tamanho da memoria: " << tamMemoria << endl;
    cout << "Total de elementos: " << vetor.size() << endl;
    vector<string> arquivosCorridas = gerarCorridasComHeap(vetor, tamMemoria);
    cout << "\nResumo primeira fase" << endl;
    cout << "Total de corridas: " << arquivosCorridas.size() << endl;
    int totalElementos = 0;
    for (int i = 0; i < arquivosCorridas.size(); i++) { //quantas corridas criadas e quantos elementos tem em cada
        vector<int> corrida = lerFita(arquivosCorridas[i]);
        cout << "Corrida " << (i+1) << ": " << corrida.size() << " elementos" << endl;
        totalElementos += corrida.size();
    }
    cout << "Total de elementos em todas corridas: " << totalElementos << endl;
    string FitaFinal = intercalarComHeap(arquivosCorridas); //chama a fase 2
    vector<int> resultadoFinal = lerFita(FitaFinal); //le o resultado na memoria
    cout << "\nResultado final" << endl;
    imprimirVetor("Fita final: ", resultadoFinal); //mostra o vetor final ordenado
    cout << "Total de elementos no resultado: " << resultadoFinal.size() << endl;
    bool ordenado = true;
    for (int i = 1; i < resultadoFinal.size(); i++) { //verifica se realmente ta ordenado
        if (resultadoFinal[i] < resultadoFinal[i-1]) {
            ordenado = false;
            break;
        }
    }
    cout << "Resultado " << (ordenado ? "ORDENADO corretamente!" : "NAO ordenado!") << endl; //fala se deu certo ou nao
    return 0;
}
