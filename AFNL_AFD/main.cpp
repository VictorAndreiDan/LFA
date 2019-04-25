#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

//AFN lambda -> AFD
//transformarea

ifstream    f("AFN_Lambda.in");
int         nr_noduri;
int         nr_ramuri;

class   nod_p
{
public:
    vector  <int> *continut;
    long int        alias;
    nod_p(){continut = NULL;}
    nod_p(int sz){continut = new vector<int>[sz];}
    void set_size(int sz){continut = new vector<int>[sz];}
    ~nod_p(){delete[] continut;}
};


void    read_nod(nod_p *nod, ifstream *f, int nr_noduri, int nr_ramuri)
{
    int aux;
    *f>>aux;
    for(int i = 1; i <= nr_noduri; i++)
    {
        for(int j = 0; j <= nr_ramuri; j++)
        {
            while(aux != 42)
            {
                nod[i].continut[j].push_back(aux + 1);//incep de la 1 numaratoarea
                *f>>aux;                               //am avut o idee si nu am mai modificat
            }
            if(aux == 42 && *f)
                *f>>aux;
        }
    }
}


void    print_nod(nod_p *nod, int nr_ramuri, int nr_noduri)
{
    for(int i = 1; i <= nr_noduri; i++)
    {
        cout<<"Nodul "<<i<<": ";
        for(int j = 0; j <= nr_ramuri; j++)
        {
            cout<<"Ramura "<<(char)('a' + j - 1)<<": ";
            for(unsigned int y = 0; y < nod[i].continut[j].size(); y++)
            {
                cout<<"q"<<nod[i].continut[j][y]<<" ";
            }
            cout<<": ";
        }
        cout<<endl;
    }
}


int     vect_cmp(vector<int> v1, vector<int> v2)
{
    if(v1.size() != v2.size())
        return 0;
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    for(int i = 0; i < v1.size(); i++)
    {
        if(v1[i] != v2[i])
            return 0;
    }
    //cout<<"Elemente egale";
    return 1;
}

int     in_v(int x, vector <int> v)
{
    for(int i = 0; i < v.size(); i++)
    {
        if(x == v[i])
            return 1;
    }
    return 0;
}

vector <int>*   gen_l(int loc, nod_p *nod) //loc = nodul de pe care se gen
{
    vector <int> *v;

    v = new vector<int>;
    for(int i = 0; i < nod[loc].continut[0].size(); i++)
    {
        //adaugare lambda initiala
        v->push_back(nod[loc].continut[0][i]);
    }
    //cout<<v->at(0);
    for(int k = 0; k < v->size(); k++)
    {
        //cout<<"1";
        if(nod[v->at(k)].continut[0].size() != 0)
        {
            //cout<<"2";
            for(int i = 0; i < nod[v->at(k)].continut[0].size(); i++)
            {
                //cout<<"3";
                //verificare daca e deja in v
                if(!in_v(nod[v->at(k)].continut[0][i], *v))
                {
                    v->push_back(nod[v->at(k)].continut[0][i]);
                }
            }
        }
    }
    if(!in_v(loc, *v))
        v->push_back(loc);
    return v;
}


void    reuniune(vector <int> *first, vector <int> *second)
{
    for(int i = 0; i < first->size(); i++)
    {
        if(!in_v(first->at(i), *second))
        {
            second->push_back(first->at(i));
        }
    }
}

void    transformare_afnl_afd(nod_p *nod, int nr_noduri, char litera, int nr_ramuri)
{
    vector < vector <int> >   alias;
    vector <int>               *aux;
    vector <int>               aux2;
    vector <int>             noul_nod;
    int                         ok;

    //adaug toate starile
    for(int i = 1; i <= nr_noduri; i++)
    {
        noul_nod.push_back(i);
        alias.push_back(noul_nod);
        noul_nod.pop_back();
    }
    //tabel
    for(int i = 0; i < alias.size(); i++) //pentru fiecare alias din v
    {
        //lal lbl de fiecare nod care este si care apare
        //adaug toate nodurile intr-un vector, cand apare unul nou(alis), il adaug
        //l
        cout<<"Stare ";
        for(int k = 0; k < alias[i].size(); k++)//print
            cout<<alias[i][k] - 1;
        for(int p = 0; p < nr_ramuri; p++)
        {
            //start l-a-l
            if(alias[i].size() > 1 && (char)(litera+p) == 'a')
                cout<<"\t";
            else
                cout<<"\t\t";
            cout<<"L-"<<(char)(litera+p)<<"-L: ";
            for(int j = 0; j < alias[i].size(); j++)//generezi lambda in noul nod
            {
                aux = gen_l(alias[i][j], nod);
                reuniune(aux, &noul_nod);
            }
            cout<<" ";
            sort(noul_nod.begin(), noul_nod.end());
            for(int i = 0; i < noul_nod.size(); i++)//print
            {
                cout<<noul_nod[i] - 1;
            }
            //a
            for(int i = 0; i < noul_nod.size(); i++)//pentru fiecare lambda aplic litera
            {
                reuniune(&nod[noul_nod[i]].continut[98 - (int)litera + p], &aux2);
            }
            cout<<" ";
            sort(aux2.begin(), aux2.end());
            for(int j = 0; j < aux2.size(); j++)//print
            {
                cout<<aux2[j] - 1;
            }
            //l
            cout<<" ";
            noul_nod.clear();//pregatesc noul nod
            aux->clear();
            for(int j = 0; j < aux2.size(); j++)//generezi lambda in noul nod
            {
                aux = gen_l(aux2[j], nod);
                reuniune(aux, &noul_nod);
            }
            sort(noul_nod.begin(), noul_nod.end());
            for(int i = 0; i < noul_nod.size(); i++)//print
            {
                cout<<noul_nod[i] - 1;
            }
            //cout<<" ";
            //verificare daca noul nod apartine deja tabelului
            ok = 1;
            for(int m = 0; m < alias.size(); m++)
            {
                if(vect_cmp(alias[m], noul_nod))
                {
                    ok = 0;
                }
            }
            if(ok == 1 && noul_nod.size() != 0)
                alias.push_back(noul_nod);
            //clear for next
            aux2.clear();
            noul_nod.clear();
            aux->clear();
            cout<<endl;

        //end l-a-l
        }
    }

}

int     main()
{
    f>>nr_noduri>>nr_ramuri;
    nod_p   nod[nr_noduri + 1];

    //alocare
    for(int i = 1; i <= nr_noduri; i++)
    {
        nod[i].set_size(nr_ramuri + 1);
    }

    //citire noduri
    read_nod(nod, &f, nr_noduri, nr_ramuri);

    //algoritm
    transformare_afnl_afd(nod, nr_noduri, 'a', nr_ramuri);
    return 0;
}
