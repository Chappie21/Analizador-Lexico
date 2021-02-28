///////////////////////////////////////////////////////////////////
/*
    ALUMNOS: 
        -ANDRÉS CHAPARRO CI: 27552207
        -CARLOS DUBUC CI: 28451238
    
    PROYECTO #1 DISEÑO DE COMPILADORES 2021 A (ANALIZADOR LEXICO)
*/
///////////////////////////////////////////////////////////////////
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

///////////////////////////////////
/* NODOS U ESTADOS DEL AUTOMATA */
#define A 0
#define B 1
#define C 2
#define D 4
#define E 5
#define F 6
#define G 7
#define H 8
#define I 9
////////////////////////////////////

char Alfabeto[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                   'i', 'j', 'k', 'm', 'n', 'l', 'o', 'p',
                   'q', 't', 's', 'r', 'v', 'w', 'x', 'y', 
                   'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                   'H', 'I', 'J', 'K', 'M', 'N', 'L', 'O',
                   'P', 'Q', 'T', 'S', 'R', 'V', 'W', 'X',
                   'Y', 'Z', '0', '1', '2', '3', '4', '5',
                   '6', '7', '8', '9', '>', '<', '=', '!',' '};

char entrada[100]; // CADENA DE ENTRADA
char cadenaComp[30]; // ALMACENAR COMPONENTE LEXICO

bool Analizar_Entrada(char[]); // ANALIZAR ENTRADA DE FORMA RECURSIVA
bool Parte_Alf(char); // COMPROBAR SI ES PARTE DEL ALFABETO
bool Comprobar_Orden(int[]); // COMPROBAR ORDEN DE SINTAXIS

int main(){

    cout<<".....::::: ANALIZADOR LEXICO :::::....."<<endl<<endl;
    system("pause");
    system("cls");

    while(true){

        cout<<">> ";
        cin.getline(entrada,50);
        cout<<"\n\n";

        bool exito = Analizar_Entrada(entrada);// ANALIZAR ENTRADA

        if(!exito){ 
            cout<<"\t\t\t*CADENA NO VALIDA*"<<endl;
        }else{
             cout<<"\t\t\t*CADENA VALIDA*"<<endl;; 
        }
    }
    return 0;
}

bool Analizar_Entrada(char cadena[]){

    bool flag = true; // VARIABLE AUXILIAR
    int i = 0,j = 0, o = 0; // CONTADORES
    int nodo = A; // ESTADO INICIAL DEL AUTOMATA
    int orden[20]; // ALAMCENA EL ORDEN DE LOS COMPONENTES LEXICOS 

    ofstream archivo("componentes_lexicos.txt"); // GENERAR ARCHIVO

    do{
        // * COMPROBAR QUE EL CARACTER SEA PARTE DEL ALFABETO CORRESPONDIENTE
        if( Parte_Alf(cadena[i]) ){

            /* EN CASO DE SER UN DIGITO */
           if( isdigit(cadena[i]) ){
               
              switch(nodo){

                  case A: nodo = B; cadenaComp[j++] = cadena[i++]; break; // TOMAR DIGITO Y DESPLAZAR A B

                  case B:

                    // TOMA TODOS LOS DIGITOS, Y LUEGO RETORNA AL NODO A
                    while( isdigit(cadena[i]) ){
                        cadenaComp[j++] = cadena[i++];  
                    }
                    archivo<<cadenaComp<<"\t\tEntero"<<endl;
                    j=0;
                    memset(cadenaComp, '\0', 30);
                    orden[o++] = 4; // INDICAR EL ORDEN
                    nodo = A;    

                  break;

                  case D: cadenaComp[j++] = cadena[i++]; break; // TOMA EL CARACTER (DIGITO) Y SE MANTIENE EN D
              }

           }else{

               switch (nodo){
                    case A:

                        if(cadena[i] == '>' || cadena[i] == '<'){

                            /* EN ESTE CASO SE DESPLAZA AL NODO E Y DEVULVE AL NODO A*/
                            cadenaComp[j++] = cadena[i++];
                            archivo<<cadenaComp<<"\t\tOperador Logico Relacional"<<endl;
                            j=0;
                            memset(cadenaComp, '\0', 30);
                            orden[o++] = 5; // INDICAR EL ORDEN
                            nodo = A;  
                        }else if(cadena[i] == '='){
                            /* SI EL SIGUIENTE CARACTER ES "=" DETECTA ==, SI NO ES UNA ASIGNACION*/
                            if(cadena[i+1] == '='){ 

                                cadenaComp[j++] = cadena[i++];
                                cadenaComp[j++] = cadena[i++];

                                archivo<<cadenaComp<<"\t\tOperador Logico Relacional"<<endl;
                                j=0;
                                memset(cadenaComp, '\0', 30);
                                orden[o++] = 5; // INDICAR EL ORDEN
                                nodo = A; 

                            }else{

                                cadenaComp[j++] = cadena[i++];
                                archivo<<cadenaComp<<"\t\tAsignacion"<<endl;
                                j=0;
                                memset(cadenaComp, '\0', 30);
                                orden[o++] = 3; // INDICAR EL ORDEN
                                nodo = A;   
                            }

                        }
                        else if(cadena[i] == '!'){ nodo = H;  cadenaComp[j++] = cadena[i++];} // TOMAR Y DESPLAZAR AL NODO H PARA DETECTAR "!="
                        else if(cadena[i] == ' '){ nodo = A; i++;} // IGNORAR ESPACIOS
                        else{ nodo = D;  cadenaComp[j++] = cadena[i++];}  // TOMAR Y DESPLAZAR AL NODO D

                    break;

                    case B: 
                        // EN CASO DE ESTAR EN EL NODO B Y DETECTAR OTRO CARACTER NO DIGITO, DETECTA UN DIGITO NUMERICO DE UNA CIFRA
                        if(cadena[i] == '!' || cadena[i] == '=' || cadena[i] == '<' || cadena[i] == '>' || cadena[i] == ' '){
                            
                            archivo<<cadenaComp<<"\t\tEntero"<<endl;
                            j=0;
                            memset(cadenaComp, '\0', 30);
                            orden[o++] = 4; // INDICAR EL ORDEN
                            nodo = A;    

                        }else{
                            flag = false; // ! EN CASO DE NO SER CIERTO, INDICARÁ UN ERROR
                        }

                    break;

                    case D:
                        // TOMAR TODO CARACTER DIFERENTE DE ALGUN SIMBOLO, OPERADOR U ESPACIO
                        while(cadena[i] != '=' && cadena[i] != '!' && cadena[i] != '<' && cadena[i] != '>' && cadena[i] != ' ' && i<strlen(cadena)){
                            cadenaComp[j++] = cadena[i++];  
                        }

                        // DETECTAR COMPONENTES DE APERTURA Y CIERRE
                        if(strcmp(cadenaComp, "ABRIR") == 0){

                            archivo<<cadenaComp<<"\t\tEspecial (CLAVE)"<<endl;
                            j=0;
                            memset(cadenaComp, '\0', 30);
                            orden[o++] = 1; // INDICAR EL ORDEN
                            nodo = A;  
                            
                        }else if(strcmp(cadenaComp, "CERRAR") == 0){

                            archivo<<cadenaComp<<"\t\tEspecial (CLAVE)"<<endl;
                            j=0;
                            memset(cadenaComp, '\0', 30);
                            orden[o++] = 6; // INDICAR EL ORDEN
                            nodo = A;  
                            
                        }else{
                            
                            /* EN CASO DE NO SE UN COMPONENTE ESPECIAL, 
                               ES ESPECIFICADO COMO INDENTIFICADOR*/

                            archivo<<cadenaComp<<"\t\tIdenficador"<<endl;
                            j=0;
                            memset(cadenaComp, '\0', 30);
                            orden[o++] = 2; // INDICAR EL ORDEN
                            nodo = A;  
                        }
 
                    break;

                    case H:
                        // EN CASO DE SER EL OPERADOR "!="
                        if(cadena[i] == '='){
                            cadenaComp[j++] = cadena[i++];
                            archivo<<cadenaComp<<"\t\tOperador Logico Relacional"<<endl;
                            j=0;
                            memset(cadenaComp, '\0', 30);
                            orden[o++] = 5;
                            nodo = A;      
                        }else{
                            flag = false; // ! DE NO SERLO, DETECTA ERROR
                        }

                    break;

               }
           }

        }else{  
            // EN CASO DE NO PERTERNECER AL LENGUAJE MUESTRA ERROR E IMPRIME EL CARACTER EN PANTALLA
            flag = false;
            cout<<"No pertenece al lenguaje: "<<cadena[i]<<"\t"<<Parte_Alf(cadena[i])<<endl;
            break;
        }

        if(!flag) break; // ! TERMINAR EL CICLO EN CASO DE DETECTAR ALGÚN ERROR

    }while(i < strlen(cadena) );

    if( !Comprobar_Orden(orden) ) flag = false; // ! EN CASO DE NO CUMPLIR EL ORDEN
    
    return flag; // RETORNA EL ESTADO DE LA BANDERA
}

// VERICIFA QUE EL CARACTER A ANALIZAR PERTENEZCA AL ALFABETO DEFINIDO
bool Parte_Alf(char caracter){

    for(int k = 0; Alfabeto[k]; k++){
        if(caracter == Alfabeto[k]) return true; //* PERTENECE
    }
    return false; // ! NO PERTENECE

}

// COMPROBAR ORDEN DE LA CADENA
bool Comprobar_Orden(int orden[]){

    /* 
        RECTIFICA QUE EL ORDEN DE LOS COMPOENTES LEXICOS SEA EL ADECUADO
        siendo:

        * ABRIR = 1
        * identificador = 2
        * asigancion = 3
        * entero = 4
        * operador = 5
        * CERRAR = 6

        ! DEBIENDO CUMPLIR EL ORDEN "1-2-3-4-5-4-6"
    */

    if(orden[0] == 1 && orden[1] == 2 && orden[2] == 3
        && (orden[3] == 4 && orden[5] == 4) && orden[4] == 5 && orden[6] == 6){
        return true; // * CUMPLE EL ORDEN
    }

    return false; // ! NO CUMPLE EL ORDEN
}