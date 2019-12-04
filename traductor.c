#include "ansinjson.c"

FILE *archivoXML;

void jsonTrad();
void elementTrad();
void objectTrad();
void objTrad();
void arrayTrad();
void arrTrad();
void attributeListTrad();
void attrListTrad();
void elementListTrad();
void elmListTrad();
void attributeTrad();
void attNameTrad();
void attValueTrad();
int tabulacion=0;

void print_tab(){
    int i=0;
    for (i=1;i<=tabulacion;i++){
        printf(" ");
		fprintf(archivoXML," "); 
    }
}


void jsonTrad(){
	elementTrad();
}

void elementTrad(){
	
	if(t.compLex == L_LLAVE){
		objectTrad();
	}
	else if(t.compLex == L_CORCHETE){
		arrayTrad();
	}
}

void objectTrad(){
	if(t.compLex == L_LLAVE){
		match(L_LLAVE);
		objTrad();
	}
}

void objTrad(){
	if(t.compLex == STRING){
        attributeListTrad();
        match(R_LLAVE);

    }
    else if(t.compLex == R_LLAVE){
        match(R_LLAVE);
    }
}

void arrayTrad(){
	if(t.compLex == L_CORCHETE){   
        match(L_CORCHETE);
        arrTrad();
		print_tab();
    }	
}

void arrTrad(){
	if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
        printf("\n");
		fprintf(archivoXML,"\n"); 
        print_tab();
        printf("<item>\n");
		fprintf(archivoXML,"<item>\n"); 
        tabulacion+=4;
        elementListTrad();
        print_tab();
        printf("</item>\n");
		fprintf(archivoXML,"</item>\n"); 
		tabulacion-=4; 
    }
    else if(t.compLex == R_CORCHETE){
        tabulacion-=4;
        match(R_CORCHETE);
    }
}

void attributeListTrad() {
	if(t.compLex == STRING){
        attributeTrad();
        attrListTrad();
    }
}

void attrListTrad(){
	if (t.compLex == R_LLAVE){
		tabulacion-=4;
    }
  
    if(t.compLex == COMA){
        match(COMA);
        attributeTrad();
        attrListTrad();
    }
}

void elementListTrad(){
	if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
        elementTrad();
        elmListTrad();   
    }
}

void elmListTrad(){
	if(t.compLex == R_CORCHETE){ 
        match(R_CORCHETE);
    }
	if(t.compLex == COMA){  
        match(COMA);
        print_tab();
        printf("</item>\n");
		fprintf(archivoXML,"</item>\n"); 
        print_tab();
        printf("<item>\n");
		fprintf(archivoXML,"<item>\n"); 
        tabulacion+=4;
        elementTrad();
        elmListTrad();
    }
}

void attributeTrad(){
	if(t.compLex == STRING){
        print_tab();
        char lexema[TAMLEX];
        strcpy(lexema,t.lexema);
        printf("<");
		fprintf(archivoXML,"<");
        attNameTrad();
        printf(">");
		fprintf(archivoXML,">"); 
        match(DOS_PUNTOS);
        attValueTrad();
		printf("</");
		fprintf(archivoXML,"</"); 
		printf("%s", lexema);
        fprintf(archivoXML,"%s",lexema); 
        printf(">\n"); 
		fprintf(archivoXML,">\n");   
    }
}

void attNameTrad(){
	if(t.compLex == STRING){    
        char lexema[TAMLEX];
        strcpy(lexema,t.lexema);
		printf("%s", lexema);
        fprintf(archivoXML,"%s",lexema); 
        match(STRING);
    }
}

void attValueTrad(){
	if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
        tabulacion +=4;
        elementTrad();
    }
    else if(t.compLex == STRING){
        printf("%s",t.lexema);
		fprintf(archivoXML," %s",t.lexema);
        match(STRING);
    }
    else if(t.compLex == NUMBER){
        printf("%s",t.lexema);
		fprintf(archivoXML," %s",t.lexema);
        match(NUMBER);
    }
    else if(t.compLex == PR_TRUE){
        printf("%s",t.lexema);
		fprintf(archivoXML," %s",t.lexema);
        match(PR_TRUE);
    }
    else if(t.compLex == PR_FALSE){
        printf("%s",t.lexema);
		fprintf(archivoXML," %s",t.lexema);
        match(PR_FALSE);
    }
    else if(t.compLex == PR_NULL){
        printf("%s",t.lexema);
		fprintf(archivoXML," %s",t.lexema);
        match(PR_NULL);
    }
}

int main (int argc,char* args[]){
    if(argc > 1)
    {
        if (!(archivo=fopen(args[1],"rt")))
        {
            printf("Archivo no encontrado.\n");
            exit(1);
        }
        lex_siglex();
        json();
		archivoXML=fopen("output.xml","a");
        if(aceptar){
            fclose(archivo);
            archivo=fopen(args[1],"rt");
	    
            lex_siglex();
            jsonTrad();
	    
        }
	else{	
		fprintf(archivoXML,"\nEl fuente tiene errores sintacticos.\n");
		
	}
	fclose(archivoXML);
    }else{
        printf("Debe pasar como parametro el path al archivo fuente.\n");
        exit(1);
    }

    return 0;
}
