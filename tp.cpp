#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>
#include <limits>

#define INT_MAX std::numeric_limits<int>::max();

class Aresta;

class Vertice {
public:
    int id;
    int dist;
    bool visitado;
    Vertice* pai;
    std::vector<Aresta*> lista_arestas;

    Vertice() {
        this->id = 0;
        this->visitado = false;
        this->dist = INT_MAX;
        this->pai = nullptr;
    }

    Vertice(int _id) {
        this->id = _id;
        this->dist = INT_MAX;
        this->pai = nullptr;
        this->visitado = false;
    }

    struct Compare {
        bool operator()(const Vertice* v1, const Vertice* v2) {
            return v1->dist > v2->dist;
        }
    };
};

class Aresta {
public:
    Vertice* v1;
    Vertice* v2;
    bool visitada;
    int ano;
    int tempo;
    int custo;

    Aresta() {
        this->v1 = nullptr;
        this->v2 = nullptr;
        this->ano = 0;
        this->tempo = 0;
        this->custo = 0;
        this->visitada = false;
    }

    Aresta(Vertice* _v1, Vertice* _v2, int _ano, int _tempo, int _custo) {
        this->v1 = _v1;
        this->v2 = _v2;
        this->ano = _ano;
        this->tempo = _tempo;
        this->custo = _custo;
        this->visitada = false;
    }

    struct Compare {
        bool operator()(const Aresta* a1, const Aresta* a2) {
            return a1->custo > a2->custo;
        }
    };
};

void dijkstra(std::vector<Vertice*>& vertices, Vertice* fonte) {
    fonte->dist = 0;
    std::priority_queue<Vertice*, std::vector<Vertice*>, Vertice::Compare> fila_prioridade;
    fila_prioridade.push(fonte);

    while (!fila_prioridade.empty()) {
        Vertice* u = fila_prioridade.top();
        fila_prioridade.pop();
        u->visitado = true;

        for (Aresta* aresta : u->lista_arestas) {
            Vertice* v = (aresta->v1 == u) ? aresta->v2 : aresta->v1;
            if (!v->visitado && u->dist + aresta->tempo < v->dist) {
                v->dist = u->dist + aresta->tempo;
                v->pai = u;
                fila_prioridade.push(v);
            }
        }
    }
}

void dijkstraAno(std::vector<Vertice*>& vertices, Vertice* fonte, Vertice* &ultimo) {
    fonte->dist = 0;
    std::priority_queue<Vertice*, std::vector<Vertice*>, Vertice::Compare> fila_prioridade;
    fila_prioridade.push(fonte);

    while (!fila_prioridade.empty()) {
        Vertice* u = fila_prioridade.top();
        ultimo = u;
        fila_prioridade.pop();
        u->visitado = true;

        for (Aresta* aresta : u->lista_arestas) {
            Vertice* v = (aresta->v1 == u) ? aresta->v2 : aresta->v1;
            if (!v->visitado && u->dist + aresta->ano < v->dist) {
                v->dist = u->dist + aresta->ano;
                v->pai = u;
                fila_prioridade.push(v);
            }
        }
    }
}

void prim(std::vector<Vertice*>& vertices, std::vector<Aresta*>& arvore_geradora_minima) {
    std::priority_queue<Aresta*, std::vector<Aresta*>, Aresta::Compare> fila_prioridade;
    vertices[0]->visitado = true;

    for (Aresta* aresta : vertices[0]->lista_arestas) {
        fila_prioridade.push(aresta);
    }

    while (!fila_prioridade.empty()) {
        Aresta* aresta = fila_prioridade.top();
        fila_prioridade.pop();

        if (aresta->v1->visitado && aresta->v2->visitado) {
            continue;
        }

        aresta->visitada = true;
        arvore_geradora_minima.push_back(aresta);

        Vertice* proximo_vertice = (aresta->v1->visitado) ? aresta->v2 : aresta->v1;
        proximo_vertice->visitado = true;

        for (Aresta* adjacente : proximo_vertice->lista_arestas) {
            if (!adjacente->visitada) {
                fila_prioridade.push(adjacente);
            }
        }
    }
}

int main(){

    std::vector<Vertice*> vertices;
    std::vector<Aresta*> arestas;

    int n, m;
    scanf("%d%d", &n, &m);
    //std::cin >> n >> m;

    for(int i = 0; i < n; i++){
        vertices.push_back(new Vertice(i));
    }

    for(int i = 0; i < m; i++){
        int id1, id2;
        scanf("%d%d", &id1, &id2);
        //std::cin >> id1 >> id2;

        int ano, tempo, custo;
        scanf("%d%d%d", &ano, &tempo, &custo);
        //std::cin >> ano >> tempo >> custo;

        Aresta* aresta = new Aresta(vertices[id1 - 1], vertices[id2 - 1], ano, tempo, custo);
        Aresta* arestaInversa = new Aresta(vertices[id2 - 1], vertices[id1 - 1], ano, tempo, custo);

        vertices[id1 - 1]->lista_arestas.push_back(aresta);
        vertices[id2 - 1]->lista_arestas.push_back(arestaInversa);

        arestas.push_back(aresta);
        arestas.push_back(arestaInversa);
    }

    //1)
    dijkstra(vertices, vertices[0]);

    for(int i = 0; i < vertices.size(); i++){
        std::cout << vertices[i]->id + 1 << ": " << vertices[i]->dist << '\n';
    }

    int maiorAno = 0;
    for(Aresta* aresta : arestas){
        maiorAno = std::max(maiorAno, aresta->ano);
    }
    std::cout << "Maior ano: " << maiorAno << '\n';

    //2)
    for(int i = 0; i < vertices.size(); i++){
        vertices[i]->dist = INT_MAX;
        vertices[i]->visitado = false;
    }

    Vertice* ultimo;
    dijkstraAno(vertices, vertices[0], ultimo);

    Vertice* u;
    std::vector<Vertice*> caminho;
    std::vector<Aresta*> caminhoArestas;
    for(u = ultimo; u != vertices[0]; u = u->pai){
        caminho.push_back(u);
        for(int i = 0; i < u->pai->lista_arestas.size(); i++){
            if(u->pai->lista_arestas[i]->v2 == u){
                caminhoArestas.push_back(u->pai->lista_arestas[i]);
            }
        }
    }

    int menorAno = 0;
    for(Aresta* aresta : caminhoArestas){
        menorAno = std::max(menorAno, aresta->ano);
    }
    std::cout << "Menor ano: " << menorAno << '\n';

    //3)
    for(int i = 0; i < vertices.size(); i++){
        vertices[i]->dist = INT_MAX;
        vertices[i]->visitado = false;
    }

    std::vector<Aresta*> arvore_geradora_minima;
    prim(vertices, arvore_geradora_minima);

    int menorCusto = 0;
    for(int i = 0; i < arvore_geradora_minima.size(); i++){
        menorCusto += arvore_geradora_minima[i]->custo;
    }
    std::cout << "Menor custo: " << menorCusto << '\n';



    return 0;
}