//TODO: Celem projektu będzie implementacja algorytmu Huffmana w postaci kompresora/dekompresora plików (binarnych).

//Algorytm Huffmana to statystyczny algorytm kompresji bezstratnej, który przypisuje każdemu bajtowi z ciągu wejściowego //kod o zmiennej długości (najkrótszy kod ma długość  bitu). Kod bajtu jest krótszy, jeśli bajt występuje wielokrotnie //w kompresowanym ciągu, a dłuższy, jeśli występuje rzadko.

//Algorytm składa się z trzech podstawowych faz:

//wyznaczenie częstości wystąpień poszczególnych bajtów,
//budowa tzw. drzewa Huffmana i wyznaczenie kodów na jego podstawie,
//zasadnicza kompresja: ponowne czytanie ciągu wejściowego i zastępowanie bajtów przez odpowiadające im kody.

#include <stdio.h>
#include <stdlib.h>
#include "huff.h"

#define MAX1 256 //dla 8 bitowych
#define MAX2 1096 //dla 12 bitowych
#define MAX3 65536 //dla 16 bitowych


char * zapisz_w_buforze(NewCode *noweKody){ // !!!Jeśli dopiero zapełni 8 bitów, wtedy dodajemy do buffora, inaczej sie nie da
	
	FILE *plik=fopen("witam.bin","rb");
	char *buffor = calloc(1,sizeof(char));
	char x;
 	int count=1,i=0,k=0,sum=0,sumTmp=0;
	

	if (plik==NULL){ 
   	 	printf("Unable to open file!");     
		return NULL;
 	}
 	while(!feof(plik)){ 
  		
		fread(&x,1,1,plik);   
		while(x!=noweKody[k].old){
			k++;
		}
		while(k=0){
			char temp = noweKody[k].new;
			sumTmp=sum;
			sum += noweKody[k].depth;
		
			if(sum<=8){
				temp <<= (8-sum);
				buffor[i] |= temp;
			}
			else if(sum>8 ){
				int ka = 8-sumTmp;
				int gleb = noweKody[k].depth;
				int reszta= gleb - ka;
				temp >>=reszta;
				buffor[i] |= temp;
				i++;
				count++;
				buffor = realloc(buffor,count*sizeof(char));
				
				sum =0;
				sum+=reszta;
				temp = noweKody[k].new;
				temp <<=(8-reszta);
				buffor[i] |=temp;
				k=0;
			}

		}
 	}  
	fclose(plik);
	return buffor;

}

Huffman_node *find_parent_node(Huffman_node *root, Huffman_node *node) {
    if (root == NULL) {
        return NULL;
    }
    if (root->left == node || root->right == node) {
        return root;
    }
    Huffman_node *left_parent = find_parent_node(root->left, node);
    if (left_parent != NULL) {
        return left_parent;
    }
    return find_parent_node(root->right, node);
}

NewCode *przejdz_i_nowe_kody(Huffman_node *tree,int i){	// działamy na bitach np 00100110 przez operatory << lub >> i & |, code zapisujemy w charze,
	
	NewCode * coding = malloc(i*sizeof(NewCode));
	int j=0;
			// napewno dobry sposób, przemyślany, na kartce rozpisany ez
	char buf = 0;

	Huffman_node * przejdz = tree;			//!!!!!!!PROBLEMEM JEST TO ZE # JEST ZNAKIEM, i nie tylko
	while(j!=i){
		if(przejdz->right==NULL && przejdz->left==NULL){
			if(przejdz->bit!=254){
				printf("zesraj mi sie na klate");
				coding[j].old=przejdz->bit;
				coding[j].new = buf;
				j++;
				buf=0;
			
			}
			
			Huffman_node *parent = find_parent_node(tree, przejdz);
        	if (parent != NULL) {
            	if (parent->left == przejdz) {
                	parent->left = NULL;
            	}
            	else if (parent->right == przejdz) {
                	parent->right = NULL;
            	}
				if(parent->left == NULL && parent->right == NULL){
					Huffman_node *parent2 = find_parent_node(tree, parent);
					if (parent2->left == parent) {
                		parent2->left = NULL;
            		}
            		else if (parent2->right == parent) {
                		parent2->right = NULL;
            		}
				}	
        	}

			przejdz = tree;
		}
		else if(przejdz->left !=NULL){
			coding[j].depth++;
			przejdz = przejdz->left;
			buf |=0;
			if(przejdz->left!=NULL||przejdz->right!=NULL)
				buf <<=1;
		}
		else if(przejdz ->right !=NULL && przejdz->left ==NULL){
			coding[j].depth++;
			przejdz = przejdz->right;
			buf |=1;
			if(przejdz->right!=NULL)
				buf <<=1;
		}
		else
			continue;
		


	}

	return coding;
	
	
}

Huffman_node * huffmanAlg(ArraySort *tab,int i){
										
	Huffman_node *root= malloc(sizeof(Huffman_node));
	Huffman_node  **array=malloc(i*sizeof(Huffman_node)); 
	
	int size = i-1;

	for(int j=0;j<i;j++){
		array[j] = malloc(sizeof(Huffman_node));
		array[j]->bit=tab[j].ch;
		array[j]->frequency= tab[j].fre;
		array[j]->znak = 0;
		array[j]->left = NULL;
		array[j]->right = NULL;
		printf("%d\n",array[j]->frequency);
		
	}
		
	//r = make_right();
	//l = make_Left();
	while(size!=0){   
		
		Huffman_node  *newnode=malloc(sizeof(Huffman_node)); 


		Huffman_node * r;
		Huffman_node * l;
		r=array[size];
		r->znak =1;
		
		l=array[--size];
		l->znak =0;
	
		newnode->right=r;
		newnode->left=l;
		newnode->bit = 254;
		newnode->znak =0;
		newnode->frequency = l->frequency+(r->frequency);

		
		
		array[size]=newnode;
		printf("przed zmiana!! + size = %d\n",size);
		for(int a = size;a>=0;a--){
			printf("%d\n",array[a]->frequency);
		}
		
		for(int a = size;a>0;a--){
			if(array[a]->frequency > array[a-1]->frequency){
				Huffman_node *temp = array[a];
				
				array[a] = array[a-1];
				array[a-1]=temp;
			}
			else	
				break;
		}
		printf("po!!\n");
		for(int a = size;a>=0;a--){
			printf("%d\n",array[a]->frequency);
		}
		if(size==0)
			root=array[size];
	}		
	//printf("\n\n%d",root->left->left->frequency);

	return root;
}

int sortuj(const void *a, const void *b){
	const ArraySort *aSort = (const ArraySort *) a;
    const ArraySort *bSort = (const ArraySort *) b;
    return (-(aSort->fre - bSort->fre));
}

void first_priority_sort(ArraySort *tab,int i){
	qsort(tab,i,sizeof(ArraySort),sortuj);      //sortuje dla porządku
}

int * frequency(char * tab,int cont){

	int i;
	int *vec = calloc(MAX1,sizeof(int));
	for(i=0;i<cont;i++){
		
		vec[tab[i]]++;
	}
	return vec;
}

int main(int argc,char**argv){ 
	
	unsigned char x;
 	int count=1,i=0;
	char *td = malloc(sizeof(char));
 	FILE *ptr = fopen("witam.bin","rb");

	if (ptr==NULL){ 
   	 	printf("Unable to open file!");     
		return 1;
 	}
 	while(!feof(ptr)){ 
  		fread(&x,1,1,ptr);   
		td[i]=x;
		td = realloc(td,count*sizeof(char)+1);
		i++;
		count++;
 	}  
	fclose(ptr);
	printf("%s\n",td);
	
	int *v = frequency(td,i); //liczba wystąpień
	
	ArraySort *przk = malloc(8);
	
	for(int s=0;s<256;s++)
		printf("%c -- %d\n",s,v[s]);
	
	int fa=0,fe=1;
	for(int s=0;s<256;s++)
		if(v[s]>0){
			fe++;
			przk[fa].ch=(char)s;
			printf("%c\n",przk[s].ch);
			przk[fa].fre=v[s];
			przk = realloc(przk,fe*8);
			fa++;
			
		}
	//for(int s = 0; s<fa;s++){
		//printf("%c -- %d\n",przk[s].ch,przk[s].fre);
	//}
	//printf("\n\n%d\n\n",fa);
 	first_priority_sort(przk,fa); 
	//printf("\n\n%d\n\n",fa);  
	
	//printf("\n\n\n\n");
	for(int s = 0; s<fa;s++){
		printf("%c -- %d\n",przk[s].ch,przk[s].fre);
	}
	
	Huffman_node *root = huffmanAlg(przk,fa);  //Algorytm Huffmana

	//printf("\n\n%c\n",root->right->left->right->bit);

	/*poczytac bardziej o operacjach bitowych*/
	NewCode * newkodes = przejdz_i_nowe_kody(root,fa);  //dodać do strukturt, na jaką głebokosc drzewa, wchodzi dany znak
		printf("\n\n%c",newkodes[0].old);// !!!!!!!!do naprawienia głebokosc!!!!!!!!

	//FILE *compresed;
	char *buffor=zapisz_w_buforze(newkodes);//!!!!UWaga, dla pliku pdf, nie działa huffmanAlg, (jest segmentation fault)!!!!
	printf("\n%d",buffor[0]);
	return 0;

}





