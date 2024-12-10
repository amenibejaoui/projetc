#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 5 
#define M 3 
#define MAX 100
#define U 6
#define max_film 50
#define max_queue 100
#define prix_standard 10
#define prix_premuim 13
#define prix_vip 17
#define max_utilisateurs 100
#define maxpoints 50
#define REMISE 0.10 

typedef struct {
    char nom_prenom[50];
    char email[100];
    char mdp[100];
    int age;
    int points;
} Compte;
typedef struct {
    char nom[50];
    char genre[20];
    char duree[10];
    int placedisponible;
    int salle;
} Film;

typedef struct {
    int filmN;
    int placedemande;
    char typeplace[20];
    char langue[20];
    char lieu[10];
} Reservation;

typedef struct {
    Reservation reservations[max_queue];
    int front;
    int rear;
    int size;
} Queue;
typedef struct boisson {
    char nom[50];
    int prix;
    int qte;
} boisson;

typedef struct listch {
    boisson b;
    struct listch *suivant;
} listch;

char *languedisponible[N] = {"Anglais", "Francais", "Espagnol", "Allemand", "Arabe"};
char *typeplace_disponible[M] = {"VIP", "Standard", "Premium"};
char *lieu_disponible[U]= {"tunis","ariana","manouba","nabeul","sousse","kef"};
char *t1[6] = {"oh", "viva", "tropico", "jannette", "diva", "rawaa"};
int t2[6] = {1200, 2000, 1500, 1000, 1000, 2500};
int t3[6] = {15, 20, 15, 10, 9, 22};

Compte *comptes = NULL;
int nbr = 0;


int compte_existe(const char *email) {
    for (int i = 0; i < nbr; i++) {
        if (strcmp(comptes[i].email, email) == 0) {
            return 1;
        }
    }
    return 0;
}

void creer_compte() {
    char email[100], mdp[100];
    int age, points = 0;

    printf("Entrez votre email : ");
    scanf("%s", email);

    if (compte_existe(email)) {
        printf("Un compte avec cet email existe deja. Veuillez vous connecter.\n");
        return;
    }

    printf("Entrez votre mot de passe : ");
    scanf("%s", mdp);

    printf("Entrez votre age : ");
    scanf("%d", &age);
    if (age<18){
        printf("tu ne peux pas faire une reservation \n ");
        return;
    }

    comptes = realloc(comptes, (nbr + 1) * sizeof(Compte));
    if (comptes == NULL) {
        printf("Erreur de memoire. Impossible de creer le compte.\n");
        exit(1);
    }

    strcpy(comptes[nbr].email, email);
    strcpy(comptes[nbr].mdp, mdp);
    comptes[nbr].age = age;
    comptes[nbr].points = points; 
    nbr++;

    printf("Compte cre avec succes !\n");
}

void se_connecter() {
    char email[100], mdp[100];

    printf("Entrez votre email : ");
    scanf("%s", email);

    printf("Entrez votre mot de passe : ");
    scanf("%s", mdp);

    for (int i = 0; i < nbr; i++) {
        if (strcmp(comptes[i].email, email) == 0 && strcmp(comptes[i].mdp, mdp) == 0) {
            printf("Connexion reussie !\n");
            printf("Bonjour, %s ! Vous avez %d points.\n", email, comptes[i].points);
            return;
        }
    }

    printf("Email ou mot de passe incorrect.\n");
}

void initQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->size = 0;
}

int isQueueEmpty(Queue *q) {
    return q->size == 0;
}

void enfiler(Queue *q, int filmN, int placedemande,  char *typeplace,  char *langue,char *lieu) {
    if (q->size == max_queue) {
        printf("La file est pleine");
        return;
    }
    q->rear = (q->rear + 1) % max_queue;
    q->reservations[q->rear].filmN = filmN;
    q->reservations[q->rear].placedemande = placedemande;
    strcpy(q->reservations[q->rear].typeplace, typeplace);
    strcpy(q->reservations[q->rear].langue, langue);
    strcpy(q->reservations[q->rear].lieu, lieu);

    q->size++;
}
Reservation defiler(Queue *q) {
    Reservation r;
    if (isQueueEmpty(q)) {
        printf("La file est vide");
        return r;
    }
    r = q->reservations[q->front];
    q->front = (q->front + 1) % max_queue;
    q->size--;
    return r;
}
void afficherFilms(Film films[], int nbFilms) {
    printf("Films disponibles\n ");
    for (int i = 0; i < nbFilms; i++) {
        printf("%d, Nom: %s , Genre : %s , Duree : %s , Salle : %d , Places disponibles : %d\n",
               i + 1, films[i].nom, films[i].genre, films[i].duree, films[i].salle, films[i].placedisponible);
    }
}
void afficherLangues() {
    printf("Langues disponibles\n ");
    for (int i = 0; i < N; i++) {
        printf("%d. %s\n", i + 1, languedisponible[i]);
    }
}

void afficherTypePlace() {
    printf("Types de places disponibles \n");
    for (int i = 0; i < M; i++) {
        printf("%d. %s\n", i + 1, typeplace_disponible[i]);
    }
}
void afficherlieu() {
    printf("\nlieux disponibles :\n");
    for (int i = 0; i < U; i++) {
        printf("%d. %s\n", i + 1, lieu_disponible[i]);
    }
}
float remise(float montantTotal, char *email) {
    for (int i = 0; i < nbr; i++) {
        if (strcmp(comptes[i].email, email) == 0) {
           if (comptes[i].points >= maxpoints) {
                printf("Vous avez %d points, vous obtenez une remise de %.0f%%.\n", comptes[i].points, REMISE * 100);
                montantTotal -= montantTotal * REMISE; 
                printf("Montant apres remise : %.2f\n", montantTotal);
            } else {
                printf("Vous avez %d points. Pas de remise disponible.\n", comptes[i].points);
            }
            return montantTotal;
        }
    }
}

void paiement(int placedemande, char *typePlace, char *email) {
    float montantTotal = 0;
    char modePaiement[20];

    if (strcmp(typePlace, "Standard") == 0) {
        montantTotal = placedemande * prix_standard;
    } else if (strcmp(typePlace, "Premium") == 0) {
        montantTotal = placedemande * prix_premuim;
    } else if (strcmp(typePlace, "VIP") == 0) {
        montantTotal = placedemande * prix_vip;
    } else {
        printf("Type de place invalide\n");
        return;
    }
    montantTotal = remise(montantTotal, email);
    printf(" Paiement \n");
    printf("Type de place : %s\n", typePlace);
    printf("Nombre de places : %d\n", placedemande);
    printf("Montant total a payer : %f\n", montantTotal);
    printf("Montant total apres remise : %f\n", montantTotal);

    printf("Choisissez un mode de paiement (carte, especes) ");
    scanf("%s", modePaiement);

    if (strcmp(modePaiement, "carte") == 0 || strcmp(modePaiement, "especes") == 0) {
        printf("Paiement effectue avec succes via %s. Merci pour votre achat\n", modePaiement);
    } else {
        printf("Mode de paiement invalide\n");
    }
}
void ajouterPoints( char *email, int gagne) {
    for (int i = 0; i < nbr; i++) {
        if (strcmp(comptes[i].email, email) == 0) {
            comptes[i].points += gagne;
            printf("Points ajoutes à votre compte Vous avez maintenant %d points\n", comptes[i].points);
            return ;
        }
    }
    printf("Utilisateur non trouve.\n");

}
void reservation(){
    FILE *fichier = fopen("film.txt", "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
    }

    Film films[max_film];
    int nbFilms = 0;

    while (fscanf(fichier, "%s %d %d %s %s",
                  films[nbFilms].nom,
                  &films[nbFilms].placedisponible,
                  &films[nbFilms].salle,
                  films[nbFilms].duree,
                  films[nbFilms].genre) != EOF) {
        nbFilms++;
        if (nbFilms >= max_film){
            printf("choix impossible");
        }
                  }
    fclose(fichier);

    Queue q;
    initQueue(&q);
    int choixFilm, choixplace, choixlangue, placedemande,choixlieu;
    char typeplacechoisie[20], languechoisie[20],lieuchoisi[10];
    int reservationsnb = 0;
    int maxReservations = 50;  
    char email[100];

    
    while (reservationsnb < maxReservations) {

       
        afficherFilms(films, nbFilms);
        printf("Entrez le numero du film que vous souhaitez reserver (ou tapez 0 pour quitter) : ");
        scanf("%d", &choixFilm);
           

        if (choixFilm == 0) {
            printf("Merci d'avoir utilise notre service de reservation.\n");
            return;
        }

        if (choixFilm < 1 || choixFilm > nbFilms) {
            printf("Choix invalide.\n");
            continue;
        }

        Film *filmChoisi = &films[choixFilm - 1];

        printf("Combien de places souhaitez-vous reserver ? ");
        scanf("%d", &placedemande);
            
        if (placedemande > filmChoisi->placedisponible) {
            printf("Pas assez de places disponibles.\n");
            continue;
        }

        afficherTypePlace();
        printf("Choisissez un type de place : ");
        scanf("%d",&choixplace);
        if( choixplace < 1 || choixplace > M) {
            printf("Type de place invalide, 'Standard' sera utilise.\n");
            strcpy(typeplacechoisie, "Standard");
        } else {
            strcpy(typeplacechoisie, typeplace_disponible[choixplace - 1]);
        }
        printf("Bonjour s'il vous plait choisit le lieu du cinema pour projection du film:");
        afficherlieu();
        printf("ecrire votre choix");
        scanf("%d",&choixlieu);
        if ( choixlieu < 1 || choixlieu > U) {
             printf("lieu indisponible, 'tunis' sera note.\n");
            strcpy(lieuchoisi, "tunis");
        } else {
            strcpy(lieuchoisi, lieu_disponible[choixlieu - 1]);
                    
            }


        afficherLangues();
        printf("Choisissez une langue : ");
        scanf("%d",&choixlangue);
        if ( choixlangue < 1 || choixlangue > N) {
            printf("Langue invalide,'Anglais' sera utiliee.\n");
            strcpy(languechoisie, "Anglais");
        } else {
            strcpy(languechoisie, languedisponible[choixlangue - 1]);
        }
        

        printf("Entrez votre email : ");
        scanf("%s", email);

        enfiler(&q, choixFilm - 1, placedemande, typeplacechoisie, languechoisie, lieuchoisi);
        printf("Reservation ajoutee à la file.\n");

    
        
        while (!isQueueEmpty(&q)) {
            Reservation r = defiler(&q);
            Film *f = &films[r.filmN];
            if (r.placedemande <= f->placedisponible) {
                f->placedisponible -= r.placedemande;
                printf("Reservation confirme : %d places pour le film '%s'dans la cinema situe a %s (%s, %s).\n",
                       r.placedemande, f->nom, r.lieu, r.typeplace, r.langue);
                paiement(r.placedemande, r.typeplace, email);
                ajouterPoints(email, r.placedemande); 
            } else {
                printf("Reservation echouee pour le film '%s' : pas assez de places.", f->nom);
            }
        }

        reservationsnb++;  
    }

    if (reservationsnb >= maxReservations) {
        printf("Vous avez atteint le nombre maximum de reservations \n");
    }
}
boisson cree(char *n, int p, int q) {
    boisson b;
    strcpy(b.nom, n);
    b.prix = p;
    b.qte = q;
    return b;
}

listch* ajouterFin(listch* liste, boisson valeur) {
    listch* nouveau = malloc(sizeof(listch));
    nouveau->b = valeur;
    nouveau->suivant = NULL;
    if (liste == NULL) {
        return nouveau;
    } else {
        listch* tempo = liste;
        while (tempo->suivant != NULL) {
            tempo = tempo->suivant;
        }
        tempo->suivant = nouveau;
        return liste;
    }
}
listch *remplir(listch *liste) {
    for (int i = 0; i < 6; i++) {
        boisson b = cree(t1[i], t2[i], t3[i]);
        liste = ajouterFin(liste, b);
    }
    return liste;
}

void choisir(listch *liste) {
    listch *tmp = liste;
    while (tmp != NULL) {
        printf("Nom: %s, Prix: %d, Quantité disponible: %d\n", tmp->b.nom, tmp->b.prix, tmp->b.qte);
        tmp = tmp->suivant;
    }
}
listch *supprimer(listch *list, boisson b) {
    listch *current = list;
    listch *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->b.nom, b.nom) == 0) {
            if (prev == NULL) {
                list = current->suivant;
            } else {
                prev->suivant = current->suivant;
            }
            free(current);
            return list;
        }
        prev = current;
        current = current->suivant;
    }
    return list;
}
int commander(listch *liste, char *nom, int montant, int quantite) {
    listch *tmp = liste;

    while (tmp != NULL) {
        if (strcmp(tmp->b.nom, nom) == 0) {
            if ((tmp->b.prix * quantite) <= montant) {
                if (tmp->b.qte >= quantite) {
                    tmp->b.qte -= quantite;
                    montant -= (tmp->b.prix * quantite);

                    printf("Commande reussie: %s, Montant restant: %d\n", tmp->b.nom, montant);
                    if (tmp->b.qte == 0) {
                        liste = supprimer(liste, tmp->b);
                    }
                    return 1;
                } else {
                    printf("Quantite insuffisante pour %s.\n", tmp->b.nom);
                    return 0;
                }
            } else {
                printf("Montant insuffisant pour %s.\n", tmp->b.nom);
                return 0;
            }
        }
        tmp = tmp->suivant;
    }

    printf("Boisson non trouvee.\n");
    return 0;
}

int main() {
    int choix;
    char choix1[50];
    char choix2[2];
    int montant, quantite;
    listch *liste = NULL;
    liste = remplir(liste);

    do {
        printf("\n** Menu **\n");
        printf("1. Creer un compte\n");
        printf("2. Se connecter\n");
        printf("3. Reserver un film\n");
        printf("4. Commander une boisson\n");
        printf("5. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                creer_compte();
                break;
            case 2:
                se_connecter();
                break;
            case 3:

                reservation();
                break;
            case 4:
                do {
                    choisir(liste);
                    printf("Entrez le nom de la boisson: ");
                    scanf("%s", choix1);
                    printf("Entrez le montant: ");
                    scanf("%d", &montant);
                    printf("Entrez la quantité: ");
                    scanf("%d", &quantite);
                    commander(liste, choix1, montant, quantite);
                    printf("Voulez-vous continuer? (c pour continuer, s pour arreter): ");
                    scanf("%s", choix2);
                } while (choix2[0] == 'c');
                break;
            case 5:
                printf("Au revoir\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 5);
    free(comptes);
    listch *tmp;
    while (liste != NULL) {
        tmp = liste;
        liste = liste->suivant;
        free(tmp);
    }

    return 0;
}    
  


                