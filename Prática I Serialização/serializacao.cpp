#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Pessoa {
private:
    string* nome;   // ponteiro
    int idade;
public:
    Pessoa(const string& n = "", int i = 0) {
        nome = new string(n);
        idade = i;
    }
    Pessoa(const Pessoa& other) {
        nome = new string(*other.nome);
        idade = other.idade;
    }
    ~Pessoa() {
        delete nome;
    }
    // getters pra imprimir
    string getNome() const { return *nome; }
    int getIdade() const { return idade; }
    //salva em texto
    void salvarTexto(const string& nomeArq) const {
        ofstream arq(nomeArq);
        if (!arq) {
            cerr << "Erro ao abrir " << nomeArq << endl;
            return;
        }

        arq << *nome << "\n" << idade << "\n";
    }
    //le do texto
    static Pessoa lerTexto(const string& nomeArq) {
        ifstream arq(nomeArq);
        if (!arq) {
            cerr << "Erro ao abrir " << nomeArq << endl;
            return Pessoa();
        }

        string n;
        int i;

        getline(arq, n);
        arq >> i;

        return Pessoa(n, i);
    }
    //salva em binario
    void salvarBinario(const string& nomeArquivo) const {
        ofstream arq(nomeArquivo, ios::binary);
        if (!arq) {
            cerr << "Erro ao abrir " << nomeArquivo << endl;
            return;
        }

        size_t tamanhoNome = nome->size();
        arq.write((char*)&tamanhoNome, sizeof(tamanhoNome));
        arq.write(nome->c_str(), tamanhoNome);

        arq.write((char*)&idade, sizeof(idade));

        cout << "Arquivo binário criado: " << nomeArquivo << endl;
    }
    //le em binario
    static Pessoa lerBinario(const string& nomeArq) {
        ifstream arq(nomeArq, ios::binary);
        if (!arq) {
            cerr << "Erro ao abrir " << nomeArq << endl;
            return Pessoa();
        }

        size_t tam;
        arq.read((char*)&tam, sizeof(tam));

        string n;
        n.resize(tam);
        arq.read(&n[0], tam);

        int i;
        arq.read((char*)&i, sizeof(i));

        return Pessoa(n, i);
    }
};
int main() {
    //cria o objeto
    Pessoa original("Joao", 34);
    //salva o objeto em texto e em binario
    original.salvarTexto("arquivoTeste.txt");
    original.salvarBinario("arquivoTeste.bin");
    //salva oque leu dos arquivos nos objetos texto e binario
    Pessoa texto = Pessoa::lerTexto("arquivoTeste.txt");
    Pessoa binario = Pessoa::lerBinario("arquivoTeste.bin");
    //le eles usando os getts
    cout << "Conteúdo lido do TXT: "
         << texto.getNome() << " | " << texto.getIdade() << endl;
    cout << "Conteúdo lido do BIN: "
         << binario.getNome() << " | " << binario.getIdade() << endl;
    return 0;
}
