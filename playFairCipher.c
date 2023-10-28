#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Function to generate the 5x5 key square
void generateKeyTable(char key[], int ks, char keyT[5][5])
{
    int i, j, k, flag = 0, *dicty;

    // a 26 character hashmap
    // to store count of the alphabet
    dicty = (int *)calloc(26, sizeof(int));
    for (i = 0; i < ks; i++)
    {
        if (key[i] != 'j')
            dicty[key[i] - 97] = 2;
    }

    dicty['j' - 97] = 1;

    i = 0;
    j = 0;

    for (k = 0; k < ks; k++)
    {
        if (dicty[key[k] - 97] == 2)
        {
            dicty[key[k] - 97] -= 1;
            keyT[i][j] = key[k];
            j++;
            if (j == 5)
            {
                i++;
                j = 0;
            }
        }
    }

    for (k = 0; k < 26; k++)
    {
        if (dicty[k] == 0)
        {
            keyT[i][j] = (char)(k + 97);
            j++;
            if (j == 5)
            {
                i++;
                j = 0;
            }
        }
    }
}

// Function to search for the characters of a digraph
// in the key square and return their position
void search(char keyT[5][5], char a, char b, int arr[])
{
    int i, j;

    if (a == 'j')
        a = 'i';
    else if (b == 'j')
        b = 'i';

    for (i = 0; i < 5; i++)
    {

        for (j = 0; j < 5; j++)
        {

            if (keyT[i][j] == a)
            {
                arr[0] = i;
                arr[1] = j;
            }
            else if (keyT[i][j] == b)
            {
                arr[2] = i;
                arr[3] = j;
            }
        }
    }
}

// Function to find the modulus with 5
int mod5(int a)
{
    return (a % 5);
}

// Function to make the plain text length to be even
int prepare(char str[], int ptrs, bool *odd)
{
    if (ptrs % 2 != 0)
    {
        *odd = 1;
        str[ptrs++] = 'z';
        str[ptrs] = '\0';
    }

    return ptrs;
}

// Function for performing the encryption
void encrypt(char str[], char keyT[5][5], int ps)
{
    int i, a[4];

    for (i = 0; i < ps; i += 2)
    {

        search(keyT, str[i], str[i + 1], a);

        if (a[0] == a[2])
        {
            str[i] = keyT[a[0]][mod5(a[1] + 1)];
            str[i + 1] = keyT[a[0]][mod5(a[3] + 1)];
        }
        else if (a[1] == a[3])
        {
            str[i] = keyT[mod5(a[0] + 1)][a[1]];
            str[i + 1] = keyT[mod5(a[2] + 1)][a[1]];
        }
        else
        {
            str[i] = keyT[a[0]][a[3]];
            str[i + 1] = keyT[a[2]][a[1]];
        }
    }
}

// Function to encrypt using Playfair Cipher
bool encryptByPlayfairCipher(char str[], char key[], char *lastchar)
{
    char keyT[5][5];
    int ks, ps;
    bool odd = 0;

    // Key
    ks = strlen(key);
    ks--;
    // ks = removeSpaces(key, ks);
    // toLowerCase(key, ks);

    // Plaintext
    ps = strlen(str);
    ps--; //
    // toLowerCase(str, ps);
    // ps = removeSpaces(str, ps);

    // Prepare
    ps = prepare(str, ps, &odd);

    // tempstr
    char tempstr[ps];
    strcpy(tempstr, str);

    // generate Key Table
    generateKeyTable(key, ks, keyT);

    encrypt(tempstr, keyT, ps);

    // if length of str is odd then we remove the last character
    if (odd == 1)
    {
        for (int i = 0; i < ps - 1; i++)
        {
            str[i] = tempstr[i];
        }

        *lastchar = tempstr[ps - 1];
    }
    else
    {
        for (int i = 0; i < ps; i++)
        {
            str[i] = tempstr[i];
        }
    }

    return odd;
}