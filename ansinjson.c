/*
 *	Analizador sintactico descendente recursivo
 *   JSON -> element EOF
 *   element -> object | array
 *   array -> [ a'
 *   a' -> element-list ] | ]
 *   element-list -> element el'
 *   el' ->  ,element el' | ε
 *   object -> { o'
 *   o' -> attributes-list} | }
 *   attributes-list -> attributes al'
 *   al' -> ,attributes al' | ε
 *   attribute -> att-name : att-value
 *   att-name -> string
 *   att-value -> element | string | number | true | false | null
*/

#include "anlexjson.c"

void element(int synchset[], int size);
void object(int synchset[], int size);
void obj(int synchset[], int size);
void array(int synchset[], int size);
void arr(int synchset[], int size);
void attributeList(int synchset[], int size);
void attrList(int synchset[], int size);
void elementList(int synchset[], int size);
void elmList(int synchset[], int size);
void attribute(int synchset[], int size);
void attName(int synchset[], int size);
void attValue(int synchset[], int size);
int aceptar = 1;

void errorSint(){
    aceptar = 0;
    printf("\nLin %d: Error Sintactico. No se esperaba %s. ", numLinea, t.comp);
}

void match (int expToken) {
    if (t.compLex = expToken) {
    	lex_siglex();
    }
}

int in(int vector[], int size, int comp){
    int i = 0;
    while (i < size) {
        if (vector[i] == comp) {
            return 1;
        }
        i++;
    }
    return 0;
}

void scanTo(int synchset[], int size) {
    while (in(synchset, size, t.compLex) == 0 && t.compLex != EOF) {
    	lex_siglex();
    }
    lex_siglex();
}

void checkInput(int primero[], int siguiente[], int size1, int size2) {
    int vunion[size1+size2];
    int i = 0, j = 0;
    while (i < size1) {
        if (t.compLex == primero[i]) { return;}
        i++;
    }
    i = 0;
    while (i < size1) {
        vunion[i] = primero[i];
        i++;
    }
    while (j < size2) {
        vunion[i] = siguiente[j];
        i++;
        j++;
    }
    errorSint();
    scanTo(vunion, size1+size2);
}

// json -> element
void json(){
    int primero[2] = {L_CORCHETE, L_LLAVE};
    int siguiente[1] = {EOF};
    checkInput(primero, siguiente, 2, 1);
    element(siguiente, 1);
}

//element -> object | array
void element(int synchset[], int size){
    int primero[2] = {L_CORCHETE, L_LLAVE};
    int siguiente[4] = {COMA, R_CORCHETE, R_LLAVE, EOF};
    int i = 0, is = 0;
    checkInput(primero, synchset, 2, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == L_LLAVE) {
            object(siguiente, 4);
        }
        else if (t.compLex == L_CORCHETE) {
            array(siguiente, 4);
        }
    }
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 4, 2);
}

//object -> { obj
void object(int synchset[], int size){
    int primero[1] = {L_LLAVE};
    int siguiente[4] = {COMA, R_CORCHETE, R_LLAVE, EOF};
    int i = 0, is = 0;
    checkInput(primero, synchset, 1, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == L_LLAVE) {
            match(L_LLAVE);
            obj(siguiente, 4);
        }
    }
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 4, 1);
}

//obj -> attributeList} | }
void obj(int synchset[], int size){
    int primero[2] = {R_LLAVE, STRING};
    int siguiente[4] = {COMA, R_CORCHETE, R_LLAVE, EOF};
    int i = 0, is = 0;
    checkInput(primero, synchset, 2, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == R_LLAVE) {
            match(R_LLAVE);
        }
        else if (t.compLex == STRING) {
            attributeList(siguiente, 4);
            match(R_LLAVE);
        }
    }
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 4, 2);
}

//array -> [ arr 
void array(int synchset[], int size){
    int primero[1] = {L_CORCHETE};
    int siguiente[4] = {COMA, R_CORCHETE, R_LLAVE, EOF};
    int i = 0, is = 0;
    checkInput(primero, synchset, 1, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == L_CORCHETE) {
            match(L_CORCHETE);
            arr(siguiente, 4);
        }
    }
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 4, 1);
}

//arr -> elementList] | ]
void arr(int synchset[], int size){
    int primero[3] = {R_CORCHETE, L_CORCHETE, L_LLAVE};
    int siguiente[4] = {COMA, R_CORCHETE, R_LLAVE, EOF};
    checkInput(primero, synchset, 3, size);
    if (t.compLex == R_CORCHETE) {
        match(R_CORCHETE);
    }
    else if (t.compLex == L_LLAVE || t.compLex == L_CORCHETE) {
        elementList(siguiente, 4);
        match(R_CORCHETE);
    }
    checkInput(siguiente, primero, 4, 3);
}

//attributeList -> attribute attrList
void attributeList(int synchset[], int size){
    int primero[1] = {STRING};
    int siguiente[1] = {R_LLAVE};
    int i = 0, is = 0;
    checkInput(primero, synchset, 1, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == STRING) {
            attribute(siguiente, 1);
                    attrList(siguiente, 1);
        }
    }
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 1, 1);
}

//attrList -> , attribute attrList | ES (empty string)
void attrList(int synchset[], int size){
    if (t.compLex == R_LLAVE){
        return;
    }

    int primero[1] = {COMA};
    int siguiente[1] = {R_LLAVE};
    int i = 0, is = 0;
    checkInput(primero, synchset, 1, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == COMA) {
            match(COMA);
            attribute(siguiente, 1);
            attrList(siguiente, 1);
        }
    }
    checkInput(siguiente, primero, 1, 1);
}

//elementList -> element elmList
void elementList(int synchset[], int size){
    int primero[2] = {L_LLAVE, L_CORCHETE};
    int siguiente[1] = {R_CORCHETE};
    int i = 0, is = 0;
    checkInput(primero, synchset, 2, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == L_CORCHETE || t.compLex == L_LLAVE) {
            element(siguiente, 1);
            elmList(siguiente, 1);
        }
    }
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 1, 2);
}

//elmList -> , element elmList | ES (empty string)
void elmList(int synchset[], int size){
    if(t.compLex == R_CORCHETE){ 
       return;
    }
    int primero[1] = {COMA};
    int siguiente[1] = {R_CORCHETE};
    checkInput(primero, synchset, 1, size);
    if (t.compLex == COMA) {
        match(COMA);
        element(siguiente, 1);
        elmList(siguiente, 1);
    }
    checkInput(siguiente, primero, 1, 1);
}

//attribute -> attName : attValue
void attribute(int synchset[], int size){
    int primero[1] = {STRING};
    int siguiente[2] = {COMA, R_LLAVE};
    int i = 0, is = 0;
    checkInput(primero, synchset, 1, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == STRING) {
            attName(siguiente, 2);
            match(DOS_PUNTOS);
            attValue(siguiente, 2);
        }
    }
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 2, 1);
}

//attName -> string
void attName(int synchset[], int size){
    int primero[1] = {STRING};
    int siguiente[1] = {DOS_PUNTOS};
    int i = 0, is = 0;
    checkInput(primero, synchset, 1, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
}
    if (is == 0) {
        if (t.compLex == STRING){
            match(STRING);
        }
    }
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 1, 1);
}

//attValue -> element | string | num | true | false | null
void attValue(int synchset[], int size){
    int primero[7] = {L_LLAVE, L_CORCHETE, STRING, NUMBER, PR_TRUE, PR_FALSE, PR_NULL};
    int siguiente[2] = {COMA, R_LLAVE};
    int i = 0, is = 0;
    checkInput(primero, synchset, 7, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == L_LLAVE || t.compLex == L_CORCHETE) {
            element(siguiente, 2);
        }
        else if (t.compLex == STRING) {
            match(STRING);
        }
        else if (t.compLex == NUMBER) {
            match(NUMBER);
        }
        else if (t.compLex == PR_TRUE) {
            match(PR_TRUE);
        }
        else if (t.compLex == PR_FALSE) {
            match(PR_FALSE);
        }
        else if (t.compLex == PR_NULL) {
            match(PR_NULL);
        }
}
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 2, 7);
}

/*
int main(int argc,char* args[]){

    if(argc > 1){
        if (!(archivo=fopen(args[1],"rt"))){
            printf("Archivo no encontrado.\n");
            exit(1);
        }
        lex_siglex();
        json();
        if (aceptar == 1) {
            printf("El fuente es sintacticamente correcto\n");
        }
        fclose(archivo);
    }
    else{
        printf("Debe pasar como parametro el path al archivo fuente.\n");
        exit(1);
    }
    return 0;
}*/

