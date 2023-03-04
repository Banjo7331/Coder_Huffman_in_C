typedef struct huffmanAlg {
    char bit;
    int znak;
    int frequency;
    struct huffmanAlg *left;
    struct huffmanAlg *right;
}  Huffman_node ;

typedef struct toSort{
    char ch;
    int fre;
} ArraySort;
typedef struct encoding{
    char old;
    char new;
} NewCode;