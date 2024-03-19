#include <stdio.h>
#include <string.h>

typedef struct Node {
    struct Node *left, *right;
    int frequency;
    char character;
} *Node;

struct Node pool[256] = {{0}};
Node priorityQueue[255];
Node *queue = priorityQueue - 1;
int nodeCount = 0, queueEnd = 1;
char *huffmanCodes[128] = {0}, buffer[1024];

Node newNode(int frequency, char character, Node left, Node right) {
    Node newNode = pool + nodeCount++;
    if (frequency)
        newNode->character = character, newNode->frequency = frequency;
    else {
        newNode->left = left, newNode->right = right;
        newNode->frequency = left->frequency + right->frequency;
    }
    return newNode;
}

void queueInsert(Node newNode) {
    int j, i = queueEnd++;
    while ((j = i / 2)) {
        if (queue[j]->frequency <= newNode->frequency) break;
        queue[i] = queue[j], i = j;
    }
    queue[i] = newNode;
}

Node queueRemove() {
    int i, l;
    Node node = queue[i = 1];
    if (queueEnd < 2) return 0;
    queueEnd--;
    while ((l = i * 2) < queueEnd) {
        if (l + 1 < queueEnd && queue[l + 1]->frequency < queue[l]->frequency) l++;
        queue[i] = queue[l], i = l;
    }
    queue[i] = queue[queueEnd];
    return node;
}

void buildHuffmanCodes(Node node, char *code, int len) {
    static char *output = buffer;
    if (node->character) {
        code[len] = 0;
        strcpy(output, code);
        huffmanCodes[node->character] = output;
        output += len + 1;
        return;
    }
    code[len] = '0'; buildHuffmanCodes(node->left,  code, len + 1);
    code[len] = '1'; buildHuffmanCodes(node->right, code, len + 1);
}

void initializeHuffman(const char *input) {
    int i, frequencies[128] = {0};
    char code[16];

    while (*input) frequencies[(int)*input++]++;

    for (i = 0; i < 128; i++)
        if (frequencies[i]) queueInsert(newNode(frequencies[i], i, 0, 0));

    while (queueEnd > 2) 
        queueInsert(newNode(0, 0, queueRemove(), queueRemove()));

    buildHuffmanCodes(queue[1], code, 0);
}

void encodeString(const char *input, char *output) {
    while (*input) {
        strcpy(output, huffmanCodes[*input]);
        output += strlen(huffmanCodes[*input++]);
    }
}

void decodeString(const char *input, Node tree) {
    Node node = tree;
    while (*input) {
        if (*input++ == '0') node = node->left;
        else node = node->right;
        if (node->character) putchar(node->character), node = tree;
    }
    putchar('\n');
    if (tree != node) printf("Invalid input\n");
}

int main(void) {
    int i;
    const char *inputString = "this is an example for huffman encoding";
    char outputBuffer[1024];

    initializeHuffman(inputString);
    for (i = 0; i < 128; i++)
        if (huffmanCodes[i]) printf("'%c': %s\n", i, huffmanCodes[i]);

    encodeString(inputString, outputBuffer);
    printf("Encoded: %s\n", outputBuffer);

    printf("Decoded: ");
    decodeString(outputBuffer, queue[1]);

    return 0;
}
