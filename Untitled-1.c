int rechercher(Personne *t, int taille, char nom[]) {
    for (int i = 0; i < taille; i++) {
        if (strcmp(t[i].nom, nom) == 0) {
            return i;
        }
    }
    return -1;
}


void ajouter(Personne **t, int *taille) {
    (*taille)++;

    Personne *temp = realloc(*t, (*taille) * sizeof(Personne));
    if (temp == NULL) {
        printf("Erreur realloc\n");
        return;
    }
    *t = temp;

    printf("Nom : ");
    scanf("%s", (*t)[*taille - 1].nom);

    printf("Age : ");
    scanf("%d", &(*t)[*taille - 1].age);

    printf("CIN : ");
    scanf("%s", (*t)[*taille - 1].cin);
}

void supprimer(Personne **t, int *taille) {
    char nom[50];
    printf("Nom à supprimer : ");
    scanf("%s…