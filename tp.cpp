#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

class Aresta;

class Vertice{
    public:
        int id;

        std::vector<Aresta*> lista_arestas;

        Vertice(){
            this->id = 0;
        }

        Vertice(int _id){
            this->id = _id;
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

    return 0;
}