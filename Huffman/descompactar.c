#include "descompactar.h"

#define MAX_FILENAME_SIZE 100
#define BYTE_SIZE 8 // Tamanho de um byte
#define HUFF_GARBAGE 3 // Tamanho do lixo 
#define HUFF_TREE 13 // Tamanho da arvore


void descompactar()
{
    char nomedoarquivo[MAX_FILENAME_SIZE];
    printf("Entre com o nome do arquivo que deseja descompactar: ");
    scanf("%s", nomedoarquivo);

    FILE *compactado = fopen(nomedoarquivo, "rb"); // Apenas leitura do arquivo em binário.
    if(compactado == NULL)
    {
        printf("\nErro no arquivo.\nArquivo Vazio ou inexistente\n\n");
        return;
    }

    descompressao(compactado);
}


int descompressao(FILE *compactado)
{
	int tamanho_arquivo = 0;
	short tamanho_lixo, tamanho_arvore;
    short mask_sizeArvore = 8191; // 8191 = 00011111 11111111
	char nome_saida[MAX_FILENAME_SIZE]; 
    unsigned char f_byte, s_byte;
    NO *raiz = NULL;

    getchar();
    printf("Digite o nome final do arquivo: ");
    scanf("%[^\n]s", nome_saida);

    printf("\nIniciando processo de descompactação...\n\n");

    FILE *descompactado = fopen(nome_saida, "wb"); 
    
    // Verifica o tamanho do arquivo compactado
    fseek(compactado, 0, SEEK_END); // chegou ao EOF
    tamanho_arquivo = ftell(compactado); // Retorna a posição de EOF
	rewind(compactado);

    f_byte = fgetc(compactado); // f_byte recebe o primeiro byte do arquivo
    s_byte = fgetc(compactado); // s_byte recebe o segundo byte do arquivo

    printf("\nProcesso em andamento...\n\n");
	
    tamanho_lixo = f_byte >> 5; // tamanho_lixo recebe os 3 bits do primeiro byte

    // tamanho_arvore recebe os 5 ultimos bits do primeiro byte e os 8 bits do segundo byte
    // desloca o f_byte 8 bits para a esquerda e faz um OR com o s_byte
    tamanho_arvore = ((f_byte << 8) | s_byte) & mask_sizeArvore; 

    raiz = montagem_arvore(compactado); // Monta a arvore de acordo com o arquivo compactado
    
	fseek(compactado, tamanho_arvore + 2, SEEK_SET); // Escreve depois da header no novo arquivo

	escrever_arquivo(compactado, descompactado, raiz, tamanho_lixo, 
        tamanho_arquivo); // Escreve tudo no arquivo

	fclose(compactado);
    fclose(descompactado);

    printf("\nArquivo descompactado com sucesso!\n\n");
	return 0;

}

NO *montagem_arvore(FILE *compactado) 
{
	unsigned char atual;
    NO *novo_no;
    atual = fgetc(compactado);
    // Se for um nó interno, cria um nó com o caractere nulo
    if (atual == '*')
    {
    	novo_no = criar_no(atual, 0);
        novo_no->esq = montagem_arvore(compactado);
        novo_no->dir = montagem_arvore(compactado);
    }
    else if (atual == '\\') // se encontrar um \ (escape), pega o proximo byte
    {
        atual = fgetc(compactado); // pega o proximo byte que é o item que queremos
        novo_no = criar_no(atual, 0);
    }
    else
    {
        novo_no = criar_no(atual, 0);
    }
    return novo_no;
}



void escrever_arquivo(FILE* compactado, FILE* descompactado, NO *raiz,
                        short tamanho_lixo, int tamanho_arquivo_compactado) 
{
	unsigned char buffer;
	NO *atual = raiz;
    int posicao_atual_arquivo;      

	while(!feof(compactado))
	{
        //printf("##%d##\n", tamanho_lixo);
        posicao_atual_arquivo = ftell(compactado);
        //printf("#%d# ,,,,,,,, #%d#\n", posicao_atual_arquivo, tamanho_arquivo_compactado);
        int k;
		buffer = fgetc(compactado);
		if (posicao_atual_arquivo == tamanho_arquivo_compactado - 1) // Ultimo byte
        {
			k = 8 - tamanho_lixo; 
            //printf("$$%d$$\n", k);
        }
		else
        {
			k = 8; // 1 byte
        }

		for (int j = 0; j < k; j++)
		{
            // 1 = direita e 0 = esquerda   
			if (bit_esta_setado(buffer, 7 - j)) 
            {
				atual = atual->dir;
            }
			else
            {
				atual = atual->esq;
            }

			if (atual->esq == NULL && atual->dir == NULL) //folha
            {
                // Escreve o caractere no arquivo
                fwrite(&atual->item, sizeof(char), 1, descompactado);
                atual = raiz;
            }
		}
	}
}