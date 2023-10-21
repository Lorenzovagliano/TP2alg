#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>

class Aresta;

class Vertice{
    public:
        int id;
        int dist;
        bool visitado;
        Vertice* pai;

        std::vector<Aresta*> lista_arestas;

        Vertice(){
            this->id = 0;
            this->visitado = false;
            this->dist = NULL;
            this->pai = nullptr;
        }

        Vertice(int _id){
            this->id = _id;
            this->dist = NULL;
            this->pai = nullptr;
            this->visitado = false;
        }

};

class Aresta{
    public:
        Vertice* v1;
        Vertice* v2;
        bool visitada;

        int ano;
        int tempo;
        int custo;

        Aresta(){
            this->v1 = nullptr;
            this->v2 = nullptr;
            this->ano = 0;
            this->tempo = 0;
            this->custo = 0;
            this->visitada = false;
        }

        Aresta(Vertice* _v1, Vertice* _v2, int _ano, int _tempo, int _custo){
            this->v1 = _v1;
            this->v2 = _v2;
            this->ano = _ano;
            this->tempo = _tempo;
            this->custo = _custo;
            this->visitada = false;
        }
};

class Fila{
    public:
        std::vector<Vertice*> vertices;

        Fila(){}

        void push(Vertice* novo){
            vertices.push_back(novo);
        }

        Vertice* popMenor(){
            if(vertices.size() < 2){
                std::cout << "0 ou 1 VÉRTICES\n";
                return nullptr;
            }

            for(int i = 0; i < vertices.size() - 2; i++){
                if(vertices[i]->dist < vertices[i + 1]->dist){
                    Vertice* temp = vertices[i + 1];
                    vertices[i] = vertices[i+1];
                    vertices[i + 1] = temp;
                }
            }

            Vertice* retorno = vertices[vertices.size()];
            vertices.pop_back();

            return retorno;
        }

};

struct CompararDistancia{
    bool operator()(Vertice* const& v1, Vertice* const& v2){
        return v1->dist > v2->dist;
    }
};

void dijkstra(std::vector<Vertice*> vertices){
    for(int i = 0; i < vertices.size(); i++){
        vertices[i]->dist = 99999999;
    }
    
    std::priority_queue<Vertice*, std::vector<Vertice*>, CompararDistancia> fila;

    fila.push(vertices[0]);
    vertices[0]->dist = 0;

    while(fila.size() != 0){
        Vertice* atual = fila.top();
        std::cout << "ID: " << atual->id + 1 << '\n';
        fila.pop();

        if(atual->visitado == true){
            continue;
        }
        atual->visitado = true;

        for(int i = 0; i < atual->lista_arestas.size(); i++){
            unsigned long long int tentativeDistance = atual->dist + atual->lista_arestas[i]->tempo;

            if(tentativeDistance < atual->lista_arestas[i]->v2->dist){
                atual->lista_arestas[i]->v2->dist = tentativeDistance;
                atual->lista_arestas[i]->v2->pai = atual;
                std::cout << atual->lista_arestas[i]->v2->id + 1 << ", pai: " << atual->lista_arestas[i]->v2->pai->id + 1 << '\n';
                fila.push(atual->lista_arestas[i]->v2);
            }
        }
    }
}

std::vector<Vertice*> restaurar(Vertice* vertice, Vertice* raiz){
    std::vector<Vertice*> caminho;

    for(Vertice* u = vertice; u != raiz; u = vertice->pai){
        caminho.insert(caminho.begin(), u);
    }

    return caminho;
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

    for(int i = 0; i < arestas.size(); i++){
        std::cout << arestas[i]->v1->id + 1 << ", " << arestas[i]->v2->id + 1 << '\n';
    }

    dijkstra(vertices);

    /*for(int i = 0; i < vertices.size(); i++){
        if(vertices[i]->visitado != true){
            std::cout << "aaaazn\n";
            continue;
        }

        std::cout << "v = " << vertices[i]->id + 1 << ", Pai = " << vertices[i]->pai-> id + 1 << '\n';
    }*/

    /*for(int i = 0; i < vertices.size(); i++){
        if(vertices[i]->visitado != true){
            std::cout << "aaaazn\n";
            continue;
        }
        std::vector<Vertice*> caminho = restaurar(vertices[i], vertices[0]);
        for(int j = 0; j < caminho.size(); j++){
            std::cout << "CAMINHO: " << caminho[j]->id + 1 << ", ";
        }
    }*/

    return 0;
}