#include<bits/stdc++.h>
#include<omp.h>
#include<stdio.h>
#include<string.h>
using namespace std;
#define NUMPROCS 8

// Estructura para generar los delimitadores inicializandose con un string
struct word_reader : std::ctype<char> {
    word_reader(std::string const &delims) : std::ctype<char>(get_table(delims)) {}
    static std::ctype_base::mask const* get_table(std::string const &delims) {
        static std::vector<std::ctype_base::mask> rc(table_size, std::ctype_base::mask());

        for (char ch : delims)
            rc[ch] = std::ctype_base::space;
        return &rc[0];
    }
};


int main(int argc,char **argv){
    // Establecer la cantidad de threads que se utilizarán
    if(argc==1)
        omp_set_num_threads(NUMPROCS);
    else
        omp_set_num_threads(atoi(argv[1]));

    // Mantener el número de threads en un int para su uso posterior
    int num=NUMPROCS;
    if(argc>1)
        num=atoi(argv[1]);


    cout << "Punto 1" << endl;
    // Variables para almacenamiento de strings
    string str;
    vector<string> vals;
    ifstream inpstream;
    inpstream.open("prueba.txt");

    // Tokenización de las palabras del archivo prueba.txt
    while(getline(inpstream,str)){
       // Generación de los delimitadores
        /*
       istringstream in(str);
       in.imbue(std::locale(std::locale(), new word_reader(" ,.\r\n")));
        */
        string str2 = str;
        // Tokenización
        string delim = " ";
        size_t pos = 0;
        string word;
        while ((pos = str2.find(delim)) != string::npos) {
            vals.push_back(str2.substr(0, pos));
            str2.erase(0, pos + delim.length());
        }
    }
    int len=vals.size();
    cout << "Punto 2" << endl;
    // generación de un array con los string para su uso
    string *arr=new string[len];


    // Clonación del array
    for(int i=0;i<len;i++){
       arr[i]=vals[i];
    }

    // maps para cada threads string - int
    map<string,int> mapper[num];

    cout << "Punto 3" << endl;
    // paralelización usando OMP
    #pragma omp parallel for
       for(int i=0;i<len;i++)
       {
            // mapear cada palabra del array, en caso no lo encuentre lo inicializa y lo mapea con un 1
            if(mapper[omp_get_thread_num()].find(arr[i])!=mapper[omp_get_thread_num()].end()){
               mapper[omp_get_thread_num()][arr[i]]=1;
            }
            else
            {
            // En caso contrario aumento su valor en una unidad
                mapper[omp_get_thread_num()][arr[i]]+=1;
            }

       }

    // Un nuevo map, para juntar los resultados de los otros maps
    map <string,int> res;
    cout << "Punto 4" << endl;
    // Estableciendo que es una operación crítica
    #pragma omp critical
    {
        // Búsqueda en cada map anterior
        for(int i=0;i<num;i++)
            // Recorrido de cada map
            for (map<string, int>::iterator it = mapper[i].begin(); it != mapper[i].end(); it++) {
                // mapeando los resultados en el nuevo map
                if(res.find(it->first)!=res.end())
                res[it->first] += it->second;
                else
                res[it->first] = it->second;
            }
    }

    cout << "Punto 5" << endl;
    // Escritura de archivo con los resultados finales (wordcount)
    FILE *fptr=fopen("result.txt","w+");
    for (map<string, int>::iterator it = res.begin(); it != res.end(); it++)
        fprintf(fptr,"%s : %d\n",(it->first).c_str(),it->second);


   }
