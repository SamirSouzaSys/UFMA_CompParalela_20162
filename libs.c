#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "libs.h"

#define CH 10
#define tamanho 200

/*
    L� o arquivo e coloca nas estruturas
*/
void DadosEntrada(int *****Aula, int ****Pref, int ***cargaHoraria, int *nProfs, int *nTurmas, int *nDias, int *nHorarios){
    int nProf = 0, nTurma = 0, nDia = 0, nHorario = 0;
    char buf[tamanho], cH[tamanho], *pCH, pref[tamanho], *pPref;
    FILE *file;

    file = fopen("entrada.txt","r");
    if (file == NULL){
        printf("\nErro ao abrir o arquivo.\n");
        exit(0);
    }
    while (!feof(file)){
        fscanf(file, "%[A-Z a-z :\n]s", buf);
        if(strcmp(buf, "Carga Horaria:\n") != 0){
            fseek (file, -(strlen(buf)+1), SEEK_CUR);
            break;
        }
        fgets(cH, tamanho, file);
        nProf++;
        nTurma = 0;
        pCH = strtok(cH," ");
        while (pCH != NULL){
            pCH = strtok(NULL, " ");
            nTurma++;
        }
    }

    while (!feof(file)){
        fscanf(file, "%[A-Z a-z :\n]s", buf);
        if(strcmp(buf, "Preferencia:\n") != 0){
            fseek (file, -strlen(buf)+1, SEEK_CUR);
            break;
        }
        fgets(pref, tamanho, file);
        nHorario++;
        nDia = 0;
        pPref = strtok(pref, " ");
        while (pPref != NULL){
            pPref = strtok(NULL, " ");
            nDia++;
        }
    }

    *nProfs = nProf;
    *nTurmas = nTurma;
    *nDias = nDia;
    *nHorarios = nHorario;

    //Aloca��es aqui
    (*cargaHoraria) = criarCH(*cargaHoraria, nProf, nTurma);

    (*Pref) = criarPref(*Pref, nProf, nDia, nHorario);

    (*Aula) = criarAula(*Aula, nProf, nTurma, nDia, nHorario);

    rewind(file);
    nProf = nTurma = 0;
    while (!feof(file)){
        fscanf(file, "%[A-Z a-z :\n]s", buf);
        if(strcmp(buf, "Carga Horaria:\n") != 0){
            fseek (file, -(strlen(buf)+1), SEEK_CUR);
            break;
        }
        fgets(cH, tamanho, file);
        nProf++;
        nTurma = 0;
        pCH = strtok(cH," ");
        while (pCH != NULL){
            nTurma++;
            (*cargaHoraria)[nProf-1][nTurma-1] = atoi(pCH);
            pCH = strtok (NULL, " ");
        }
    }

    nProf = 0;
    nDia = 0;
    nHorario = 0;
    fscanf(file, "%[A-Z a-z :\n]s", buf);
    if(strcmp(buf, "Preferencia:\n") != 0){
        fseek (file, -(strlen(buf)), SEEK_CUR);
        //break;
    }

    while (!feof(file)){
        fgets(pref, tamanho, file);
        if(strcmp(pref, "\n") == 0){
            fgets(pref, tamanho, file);
            nHorario = 0;
            nProf++;
        }

        nHorario++;
        nDia = 0;
        pPref = strtok(pref, " ");
        while (pPref != NULL){
            nDia++;
            (*Pref)[nProf][nDia-1][nHorario-1] = atoi(pPref);
            pPref = strtok (NULL, " ");
        }
    }

    fclose(file);
}

/*
    Cria estrutura Carga Hor�ria
*/
int **criarCH(int **cargaHoraria, int nProf, int nTurma){
    int i;
    cargaHoraria = (int **)malloc(nProf * sizeof(int *));
    if(cargaHoraria != NULL){
        for(i = 0; i < nProf; i++){
            cargaHoraria[i] = (int *)malloc(nTurma * sizeof(int));
            if(cargaHoraria[i] == NULL)
                return NULL;
        }
        return cargaHoraria;
    }
    return NULL;
}

/*
    Cria estrutura Prefer�ncia
*/
int ***criarPref(int ***Pref, int nProf, int nDia, int nHorario){
    int i, j;
    Pref = (int ***)malloc(nProf * sizeof(int **));
    if(Pref != NULL){
        for(i = 0; i < nProf; i++){
            Pref[i] = (int **)malloc(nDia * sizeof(int *));
            if(Pref[i] != NULL){
                for(j = 0; j < nDia; j++){
                    Pref[i][j] = (int *)malloc(nHorario * sizeof(int));
                    if(Pref[i][j] == NULL)
                        return NULL;
                }
            }else
                return NULL;
        }
        return Pref;
    }
    return NULL;
}

/*
    Cria estrutura Aula
*/
int ****criarAula(int ****Aula, int nProf, int nTurma, int nDia, int nHorario){
    int i, j, k;
    Aula = (int ****)malloc(nProf * sizeof(int ***));
    if(Aula != NULL){
        for(i = 0; i < nProf; i++){
            Aula[i] = (int ***)malloc(nTurma * sizeof(int **));
            if(Aula[i] != NULL){
                for(j = 0; j < nTurma; j++){
                    Aula[i][j] = (int **)malloc(nDia * sizeof(int *));
                    if(Aula[i][j] != NULL){
                        for(k = 0; k < nDia; k++){
                            Aula[i][j][k] = (int *)calloc(nHorario, sizeof(int));
                            if(Aula[i][j][k] == NULL)
                                return NULL;
                        }
                    }else
                        return NULL;
                }
            }else
                return NULL;
        }
        return Aula;
    }
    return NULL;
}

/*
    Desaloca Estruturas
*/
void desalocar(int *****Aula, int ****Pref, int ***cargaHoraria, int nProfs, int nTurmas, int nDias){
    int i, j, k;
    for(i = 0; i < nProfs; i++)
        free((*cargaHoraria)[i]);
    free(*cargaHoraria);
    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nDias; j++)
            free((*Pref)[i][j]);
        free((*Pref)[i]);
    }
    free(*Pref);
    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nTurmas; j++){
            for(k = 0; k < nDias; k++)
                free((*Aula)[i][j][k]);
            free((*Aula)[i][j]);
        }
        free((*Aula)[i]);
    }
    free(*Aula);
}

/*
    Imprime a Solu��o Atual
*/
void ImprimeSol(int ****Aula, int nProfs, int nTurmas, int nDias, int nHorarios){
    int i,j,k,l;
    for(i = 0; i < nProfs; i++){
        printf("\nProfessor %d:\n",i);
        printf("   S T Q Q S\n");
        for(j = 0; j < nTurmas; j++){
            printf("T%d:\n",j);
            for(l = 0; l < nHorarios; l++){
                printf("%d: ",l);
                for(k = 0; k < nDias; k++)
                    printf("%d ", Aula[i][j][k][l]);
                printf("\n");
            }
        }
    }
}

/*
    Cria o arquivo simplex.pl a ser lido e resolvido pelo Simplex
    A sa�da fica no arquivo sol.txt
*/
void criaSimplex(int ****Aula, int ***Pref, int **cargaHoraria,int nProfs, int nTurmas, int nDias, int nHorarios){
    FILE *fileO;
    int i, j, k, l;

    fileO = fopen("simplex.pl","w");

    if (fileO == NULL){
        printf("\nErro ao abrir o arquivo.\n");
        exit(0);
    }

    fprintf(fileO, "Maximize\naula: ");
    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nTurmas; j++){
            for(k = 0; k < nDias; k++){
                for(l = 0; l < nHorarios; l++){
                    fprintf(fileO, "+ %d x%d.%d.%d.%d ", Pref[i][k][l], i, j, k, l);
                }
                //fprintf(fileO, "\n");
            }
        }
    }

    fprintf(fileO, "\n\n");
    fprintf(fileO, "Subject To\n");

    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nDias; j++){
            for(k = 0; k < nHorarios; k++){
                //fprintf(fileO, "rT%d%d%d: (", i, j, k);
                for(l = 0; l < nTurmas-1; l++){
                    fprintf(fileO, "x%d.%d.%d.%d + ", i, l, j, k);
                }
                fprintf(fileO, "x%d.%d.%d.%d", i, l, j, k);
                fprintf(fileO, " <= 1\n");
            }
        }
    }

    fprintf(fileO, "\n");

    for(i = 0; i < nTurmas; i++){
        for(j = 0; j < nDias; j++){
            for(k = 0; k < nHorarios; k++){
                fprintf(fileO, "rP%d.%d.%d: ", i, j, k);
                for(l = 0; l < nProfs-1; l++){
                    fprintf(fileO, "x%d.%d.%d.%d + ", l, i, j, k);
                }
                fprintf(fileO, "x%d.%d.%d.%d", l, i, j, k);
                fprintf(fileO, " <= 1\n");
            }
        }
    }

    fprintf(fileO, "\n");

    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nTurmas; j++){
            fprintf(fileO, "rCH%d.%d: ", i, j);
            for(k = 0; k < nDias; k++){
                for(l = 0; l < nHorarios; l++){
                    if((k == nDias-1) &&(l == nHorarios-1))
                        break;
                    fprintf(fileO, "xx%d.%d.%d.%d + ", i, j, k, l);
                }
            }
            fprintf(fileO, "xx%d.%d.%d.%d ", i, j, k, l);
            fprintf(fileO, "<= ");
            fprintf(fileO, "%d\n", cargaHoraria[i][j]);
        }
    }

    fprintf(fileO, "\n");

    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nTurmas; j++){
            for(k = 0; k < nDias; k++){
                fprintf(fileO, "rHD%d.%d.%d: ", i, j, k);
                for(l = 0; l < nHorarios-1; l++){
                    fprintf(fileO, "x%d.%d.%d.%d + ", i, j, k, l);
                }
                fprintf(fileO, "x%d.%d.%d.%d", i, j, k, l);
                fprintf(fileO, " <= 4\n");
            }
        }
    }

    fprintf(fileO, "\n");
    fprintf(fileO, "Binary\n");

    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nTurmas; j++){
            for(k = 0; k < nDias; k++){
                for(l = 0; l < nHorarios; l++){
                    fprintf(fileO, "x%d.%d.%d.%d\n", i, j, k, l);
                }
            }
        }
    }

    fprintf(fileO, "End");
    fclose(fileO);

    // Coloque aqui o diret�rio do seu GLPX
    system("\"C:\\Users\\Lau Filho\\Documents\\UFMA\\2016.1\\PO\\winglpk-4.60\\w64\\glpsol.exe\" --cpxlp simplex.pl -o sol.txt");
}

/*
    Cria uma nova inst�ncia
*/
void criarInstancia(void){
    FILE *file;
    int i, j, k;
    int nProfs, nTurmas, nDias, nHorarios;

    file = fopen("entrada.txt","w");
    if (file == NULL){
        printf("\nErro ao abrir o arquivo.\n");
        exit(0);
    }

    printf("Voce deseja criar uma instancia com quantos:\n");

    printf("Professores?\n");
    scanf("%d", &nProfs);

    printf("Turmas?\n");
    scanf("%d", &nTurmas);

    printf("Dias?\n");
    scanf("%d", &nDias);

    printf("Horarios?\n");
    scanf("%d", &nHorarios);

    fprintf(file, "Carga Horaria:\n");
    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nTurmas-1; j++){
            fprintf(file, "%d ", CH);
        }
        fprintf(file, "%d", CH);
        fprintf(file, "\n");
    }

    fprintf(file, "Preferencia:\n");
    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nHorarios; j++){
            for(k = 0; k < nDias-1; k++){
                fprintf(file, "%d ", rand()%5);
            }
            fprintf(file, "%d", rand()%5);
            if((i != nProfs-1) || (j != nHorarios-1))
                fprintf(file, "\n");
        }
        if(i != nProfs-1)
            fprintf(file, "\n");
    }

    fclose(file);
    return;
}
