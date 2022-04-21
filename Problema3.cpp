#include <stdio.h>

typedef struct{
    int nrMat;
    int is;
    int grupa;
    int notaPractica;
    int notaExamen;
    int puncteTeme[10];
} Student;

int nrArticole(FILE *f)
{
    fseek(f, 0, 2);
	return ftell(f) / sizeof(Student);
}

void ExmatriculeazaStudent(char s1[], int nrMat) {
    Student s;
    FILE* f;

    f=fopen(s1, "r+b");

    if (f==NULL) {
        printf("Fisierul nu a putut fi gasit");
        return;
    }

    fseek(f, nrMat * sizeof(Student), 0);
    fread(&s,sizeof(s), 1, f);

    if(s.is==0) {
        printf("\nNu exista niciun student cu acest numar matricol");
    } else {
        fseek(f, ftell(f)-sizeof(Student), 0);
        s.is=0;
        fwrite(&s, sizeof(Student), 1, f);
        fseek(f, 0, 1);
        printf("\nStudentul cu nr matricol %d a fost exmatriculat", nrMat);
    }

    fclose(f);

}

void conversie(char s1[]){
    FILE* f, * g;
    Student s;
    int i;
    char s2[20] = {0};

    f=fopen(s1, "rb");

    if (f==NULL) {
        printf("Fisierul nu a putut fi gasit");
        return;
    }

    printf("\nFisier text: ");
    scanf("%s", s2);
    g=fopen(s2,"w+");

    if(g==NULL) {
        printf("Eroare la crearea fisierului");
        return;
    }

    fprintf(g, "IS \t|Nr matricol \t|Grupa \t|Puncte proba practica \t|Puncte examen \t");
    for (i=0; i<10; i++) {
        fprintf(g,"|Puncte tema %d \t", i+1);
    }


    fread(&s, sizeof(s), 1, f);
    while (!feof(f)) {
        if (s.is == 1) {
            fprintf(g, "\n%d \t|%d \t\t|%d \t|%d \t\t\t|%d \t\t", s.is, s.nrMat, s.grupa, s.notaPractica, s.notaExamen);
            for (i=0;i<10;i++) {
                fprintf(g,"|%d \t\t", s.puncteTeme[i]);
            }
        }
        fread(&s, sizeof(s), 1, f);
    }

    fclose(f);
    fclose(g);
}

int main()
{
    FILE *f;
    char nume[20], textFinal[1000], textLucru[100];
    Student s;
    int i, nrMatricol, total, nrArt, varLucru;

    printf("\nFisier: ");
    gets(nume);
    f=fopen(nume,"wb+");

    if(f==NULL) {
        printf("Eroare la deschiderea/crearea fisierului");
        return 0;
    }

    printf("Nr articole pt preformare: ");
    scanf("%d", &total);
    s.is = 0;
    for (i = 0; i <= total; i++) {
        fwrite(&s, sizeof(s), 1, f);
    }

    printf("\nNumar matricol: ");
    scanf("%d",&nrMatricol);
    while(nrMatricol < 1) {
        printf("Numarul matricol nu poate fi mai mic decat 1. Nr matricol: ");
        scanf("%d",&nrMatricol);
    }

    while(!feof(stdin)){

        if(nrMatricol>nrArticole(f)) {
            s.is=0;
            fseek(f,0,2);
            for(i=nrArticole(f);i<=nrMatricol;i++) {
                fwrite(&s,sizeof(s), 1, f);
            }
        }

        fseek(f, nrMatricol * sizeof(Student), 0);
        fread(&s,sizeof(s), 1, f);

        if(s.is==1) {
            printf("\nExista deja un student cu acest numar matricol");
        }
        else {
            s.is=1;
            s.nrMat = nrMatricol;

            printf("Grupa: ");
            scanf("%d",&varLucru);
            while (varLucru<1) {
                printf("Grupa trebuie sa fie mai mare decat 0. Introduceti grupa: ");
                scanf("%d", &varLucru);
            }
            s.grupa = varLucru;

            printf("Nota Practica: ");
            scanf("%d",&varLucru);
            while (varLucru<1 || varLucru>10) {
                printf("Nota trebuie sa fie intre 1 si 10. Introduceti nota din nou: ");
                scanf("%d", &varLucru);
            }
            s.notaPractica = varLucru;

            printf("Nota Examen: ");
            scanf("%d",&varLucru);
            while (varLucru<1 || varLucru>10) {
                printf("Nota trebuie sa fie intre 1 si 10. Introduceti nota din nou: ");
                scanf("%d", &varLucru);
            }
            s.notaExamen = varLucru;

            for(i=0;i<10;i++){
                printf("Nota tema %d: ",i+1);
                scanf("%d",&varLucru);
                while (varLucru<1 || varLucru>10) {
                    printf("Nota trebuie sa fie intre 1 si 10. Introduceti nota din nou: ");
                    scanf("%d", &varLucru);
                }
                s.puncteTeme[i]=varLucru;
            }

            fseek(f, nrMatricol * sizeof(Student), 0);
            fwrite(&s, sizeof(s), 1, f);
        }
        printf("\nNr matricol nou: ");
        scanf("%d",&nrMatricol);
        while(nrMatricol < 1) {
            printf("Numarul matricol nu poate fi mai mic decat 1. Nr matricol: ");
            scanf("%d",&nrMatricol);
        }
    }

    fclose(f);

    conversie(nume);

    printf("Nr matricol pt exmatriculare sau -1: ");
    scanf("%d", &nrMatricol);

    while(nrMatricol!=-1){
        ExmatriculeazaStudent(nume, nrMatricol);
        printf("\nUn alt nr matricol pt exmatriculare sau -1: ");
        scanf("%d", &nrMatricol);
    }

    conversie(nume);

    return 0;
}
