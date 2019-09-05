/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"
#define ALPHABET 27

// structure for trie nodes
typedef struct node
{
    bool isWord;
    struct node *children[ALPHABET];
} node;

node *root;
void (free_nodes(node *current));


int counter = 0;


// function to convert a character to a number 0-26
int findIndex(char c)
{
    if (c == '\'')
    {
        return 26;
    }
    else
    {
        return tolower(c) % 'a';
    }
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // sets current node to root
    node *current = root;
    
    // increments through the length of the word
    for(int c = 0; c < strlen(word); c++)
    {
        // converts character to index number
        int i = findIndex(word[c]);
        
        // if NULL found, word is not in the dictionary
        if (current->children[i] == NULL)
        {
            return false;
        }
    // increments current node     
        current = current->children[i];    
    }
    // if no NULL found, we have found the word, set bool to true
    return current->isWord;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // opens specified dictionary.
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        printf("Failed to open dictionary");
        return false;
    }
    
    // initialize root node, sets current node to root node.
    root = calloc(sizeof(node), 1);
    node *current = root;
    
    // increments through file until EOF marker found    
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        // if newline character found, full word has been found, reset current to root
        if (c == '\n')
        {
            current->isWord = true;
            current = root; 
            counter++;
        }
        else 
        {
            // gets index number for current character
            int i = findIndex(c);
            
            // if no node found, creates new one
            if (current->children[i] == NULL)
            {
                current->children[i] = calloc(sizeof(node), 1);
            }
            // increments current node 
            current = current->children[i];
        }
    }
    fclose(fp);
    return true; 
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (&load)
    {
        return counter;
    }
    else
    {
        return 0;
    }
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    free_nodes(root);
    return true;
}
// recurive function to free allocated memory.
void free_nodes(node *current)
{
    
    for(int i = 0; i < ALPHABET; i++)
    {
        if (current->children[i] != NULL)
        {
            free_nodes(current->children[i]);
        }
    }
    free(current);
}
