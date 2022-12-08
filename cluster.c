
/**
 * Kostra programu pro 2. projekt IZP 2022/23
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h> // Pridan pro zjednoduseni vstupu za pomoci funkce strcat

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

struct Clusters_t {
    int size;
    struct cluster_t* clus;
};
/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    if (!(cap >= 0)) cap = 0;
    c->capacity = cap;
    c->size = 0;
    c->obj = malloc(sizeof(struct obj_t)*cap);
    return;
    // TODO

}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    c->capacity = 0;
    c->size = 0;
    free(c->obj);
 
    return;
    // TODO
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    if (c->capacity == c->size)
    {
        resize_cluster(c,c->capacity+1);
    }
    c->size++;
    c->obj[c->size-1] = obj;
    // TODO
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);
    int size = c2->size;
    for (int index = 0;index < size;index++)
    {
            append_cluster(c1,c2->obj[index]);
    }
    return;
    // TODO
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t* carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);
    clear_cluster(&carr[idx]);
    if (narr > idx)
    {
        for (int index = idx; index < narr - 1; index++)
        {
            carr[index] = carr[index + 1];
        }
        clear_cluster(&carr[narr]);
        return (narr - 1);
    }

    return (narr);
    // TODO
}
/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    float distance = sqrtf((o1->x-o2->x)*(o1->x-o2->x)+(o1->y-o2->y)*(o1->y-o2->y));
    return distance;
    // TODO
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);
    float cdistance = obj_distance(&(c1->obj[0]), &(c2->obj[0]));
    float cdistancetester;
    for (int index1 = 0; index1 < c1->size; index1++)
    {
        for (int index2 = index1+1; index2 < c2->size; index2++)
        {
            cdistancetester = obj_distance(&(c1->obj[index1]), &(c2->obj[index2]));
            if (cdistance > cdistancetester) cdistance = cdistancetester;
        }
        
    }
    return cdistance;
    // TODO
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    *c1 = 0;
    *c2 = 1;
    float distance = cluster_distance(&carr[0],&carr[1]);
    float distancetester;
    for (int index1 = 0; index1 < narr; index1++)
    {
        for (int index2 = index1+1; index2 < narr; index2++)
        {
            distancetester = cluster_distance(&carr[index1],&carr[index2]);
            if (distance > distancetester)
            {
                *c1 = index1;
                *c2 = index2;
                distance = distancetester; 
            }
        }
        
    }

    return;
    
    // TODO
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct Clusters_t *arr)
{
    assert(arr != NULL);
    FILE* filePointer;
    filePointer = fopen(filename, "r");
    char c = fgetc(filePointer);
    int size = 0;
    struct obj_t object;
    object.id = 0;
    object.x = 0;
    object.y = 0;
    while (c != '=') { c = fgetc(filePointer); }c = fgetc(filePointer);
    while (c != '\n') { size *= 10, size += ((int)(c)-48); c = fgetc(filePointer);}
    arr->size = size;
    arr->clus = malloc(sizeof(struct cluster_t) * size);
    for (int i = 0; i < size; i++)
    {
        c = fgetc(filePointer);
        while (c != ' ') { object.id *= 10, object.id += (((int)(c)) - 48); c = fgetc(filePointer);}; c = fgetc(filePointer);
        while (c != ' ') { object.x *= 10, object.x += (((int)(c)) - 48); c = fgetc(filePointer);}; c = fgetc(filePointer);
        while (c != '\n') { object.y *= 10, object.y += (((int)(c)) - 48); c = fgetc(filePointer);};
        init_cluster(&arr->clus[i], 0);
        append_cluster(&arr->clus[i], object);
        object.id = 0;
        object.x = 0;
        object.y = 0;
    }



    // TODO                                             !!!!! 9
    fclose(filePointer);
    return size;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char* argv[])
{
    struct Clusters_t clusters;
    int number;
    char* name;
    if (argc == 1) { printf("Missing file\n"); return -1; }
    else
        if (argc == 2) { number = 1; name = argv[1]; }
        else {
            number = (atoi(argv[1]));
            if (number == 0)
            {
                number = (atoi(argv[2])); name = argv[1];
            }
            else name = argv[2];
        }
    if (number == 0) { printf("Wrong input.\n"); return -1; }
    strcat(name, ".txt");
    int size = load_clusters(name, &clusters);
    int a, b;
    if (number > size) printf("The number of groups is more than the number of points. I list groups with at least one point.\n");
    while (size > number)
    {
        find_neighbours(clusters.clus, size, &a, &b);
        merge_clusters(&clusters.clus[a], &clusters.clus[b]);
        size = remove_cluster(clusters.clus, size, b);
        clusters.clus = realloc(clusters.clus, sizeof(struct cluster_t) * size);
        clusters.size = size;
    }
    for (int i = 0; i < number; i++)
        sort_cluster(&clusters.clus[i]);
    print_clusters(clusters.clus, number);
    for (int i = 0; i <= number; i++)
    {
        remove_cluster(clusters.clus, size, 0);
    }
    free(clusters.clus);
    clusters.size = 0;
    return argc;

    // TODO
}