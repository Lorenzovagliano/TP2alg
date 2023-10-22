#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>
#include <limits>
#include <unordered_map>
#include <map>
#include <stack>

#define INT_MAX std::numeric_limits<long long>::max();

class Aresta;

class Vertice {
public:
    long long id;
    long long dist;
    long long visitado;
    Vertice* pai;
    std::vector<Aresta*> lista_arestas;

    Vertice() {
        this->id = 0;
        this->visitado = false;
        this->dist = INT_MAX;
        this->pai = nullptr;
    }

    Vertice(long long _id) {
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
    long long ano;
    long long tempo;
    long long custo;

    Aresta() {
        this->v1 = nullptr;
        this->v2 = nullptr;
        this->ano = 0;
        this->tempo = 0;
        this->custo = 0;
        this->visitada = false;
    }

    Aresta(Vertice* _v1, Vertice* _v2, long long _ano, long long _tempo, long long _custo) {
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

    struct CompareAno {
        bool operator()(const Aresta* a1, const Aresta* a2) {
            return a1->ano > a2->ano;
        }
    };

};

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};

std::unordered_map<std::pair<Vertice*, Vertice*>, Aresta*, pair_hash> map_arestas;

void adicionarAresta(Aresta* aresta) {
    // ... código para adicionar a aresta à lista de arestas do vértice ...

    map_arestas[std::make_pair(aresta->v1, aresta->v2)] = aresta;
    map_arestas[std::make_pair(aresta->v2, aresta->v1)] = aresta;
}

Aresta* encontrarArestaComVertices(Vertice* u, Vertice* v) {
    auto it = map_arestas.find(std::make_pair(u, v));
    if (it != map_arestas.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

void dijkstra(std::vector<Vertice*>& vertices, Vertice* fonte) {
    fonte->dist = 0;
    std::priority_queue<Vertice*, std::vector<Vertice*>, Vertice::Compare> fila_prioridade;
    fila_prioridade.push(fonte);

    while (!fila_prioridade.empty()) {
        Vertice* u = fila_prioridade.top();
        fila_prioridade.pop();
        u->visitado = true;

        for (Aresta* aresta : u->lista_arestas) {
            Vertice* v = aresta->v2;
            if (u->dist + aresta->tempo < v->dist) {
                v->dist = u->dist + aresta->tempo;
                v->pai = u;
                fila_prioridade.push(v);
            }
        }
    }
}

void dijkstraAno(std::vector<Vertice*>& vertices, Vertice* fonte) {
    fonte->dist = 0;
    std::priority_queue<Vertice*, std::vector<Vertice*>, Vertice::Compare> fila_prioridade;
    fila_prioridade.push(fonte);

    while (!fila_prioridade.empty()) {
        Vertice* u = fila_prioridade.top();
        fila_prioridade.pop();
        u->visitado = true;

        for (Aresta* aresta : u->lista_arestas) {
            Vertice* v = aresta->v2;
            if (u->dist + aresta->ano < v->dist) {
                v->dist = u->dist + aresta->ano;
                v->pai = u;
                fila_prioridade.push(v);
            }
        }
    }
}

void DFS(std::vector<Vertice*>& vertices, Vertice* fonte){
    std::vector<Vertice*> pilha;
    pilha.push_back(fonte);

    while(!pilha.empty()){
        Vertice* atual = pilha[0];
        pilha.erase(pilha.begin());

        if(atual->visitado != true){
            atual->visitado = true;
            std::sort(atual->lista_arestas.begin(), atual->lista_arestas.end(), [](Aresta* a, Aresta* b) {
                return a->ano > b->ano; 
            });
            
            for(int i = 0; i < atual->lista_arestas.size(); i++){
                pilha.push_back(atual->lista_arestas[i]->v2);
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

void primAno(std::vector<Vertice*> vertices, std::vector<Aresta*>& arvore_geradora_minima) {
    std::priority_queue<Aresta*, std::vector<Aresta*>, Aresta::CompareAno> fila_prioridade;
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

        Vertice* proximo_vertice = aresta->v2;
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

    long long n, m;
    scanf("%lld%lld", &n, &m);

    for(int i = 0; i < n; i++){
        vertices.push_back(new Vertice(i));
    }

    for(int i = 0; i < m; i++){
        long long id1, id2;
        scanf("%lld%lld", &id1, &id2);
        //std::cin >> id1 >> id2;

        long long ano, tempo, custo;
        scanf("%lld%lld%lld", &ano, &tempo, &custo);
        //std::cin >> ano >> tempo >> custo;

        Aresta* aresta = new Aresta(vertices[id1 - 1], vertices[id2 - 1], ano, tempo, custo);
        Aresta* arestaInversa = new Aresta(vertices[id2 - 1], vertices[id1 - 1], ano, tempo, custo);

        vertices[id1 - 1]->lista_arestas.push_back(aresta);
        vertices[id2 - 1]->lista_arestas.push_back(arestaInversa);
        adicionarAresta(aresta);
        adicionarAresta(arestaInversa);

        arestas.push_back(aresta);
        arestas.push_back(arestaInversa);
    }

    //1)
    dijkstra(vertices, vertices[0]);

    for(int i = 0; i < vertices.size(); i++){
        std::cout << vertices[i]->dist << '\n';
    }

    Vertice* v;
    std::vector<Aresta*> caminhoArestas1;
    for(int i = 0; i < vertices.size(); i++){
        for(v = vertices[i]; v != vertices[0]; v = v->pai){
            for(int i = 0; i < v->pai->lista_arestas.size(); i++){
                if(v->pai->lista_arestas[i]->v2 == v){
                    caminhoArestas1.push_back(v->pai->lista_arestas[i]);
                }
            }
        }
    }

    long long maiorAno = 0;
    for(Aresta* aresta : caminhoArestas1){
        maiorAno = std::max(maiorAno, aresta->ano);
    }
    std::cout << maiorAno << '\n';

    //2)
    for(int i = 0; i < vertices.size(); i++){
        vertices[i]->dist = INT_MAX;
        vertices[i]->pai = nullptr;
        vertices[i]->visitado = false;
        arestas[i]->visitada =false;
    }

    std::vector<Aresta*> arvore_geradora_minima1;
    primAno(vertices, arvore_geradora_minima1);

    long long menorAno = 0;
    for(Aresta* aresta : arvore_geradora_minima1){
        menorAno = std::max(menorAno, aresta->ano);
    }
    std::cout << menorAno << '\n';


    //3)
    for(int i = 0; i < vertices.size(); i++){
        vertices[i]->dist = INT_MAX;
        vertices[i]->pai = nullptr;
        vertices[i]->visitado = false;
    }
    for(int i = 0; i < arestas.size(); i++){
        arestas[i]->visitada = false;
    }

    std::vector<Aresta*> arvore_geradora_minima;
    prim(vertices, arvore_geradora_minima);

    long long menorCusto = 0;
    for(int i = 0; i < arvore_geradora_minima.size(); i++){
        menorCusto += arvore_geradora_minima[i]->custo;
    }
    std::cout << menorCusto << '\n';



    return 0;
}