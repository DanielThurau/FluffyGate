#include<stdio.h>
#include<stdlib.h>
#include<string.h>
 
/* Following function is used by the library qsort() function
  to sort an array of chars */
int compare (const void * a, const void * b);
 
/* The main function that recursively prints all repeated 
   permutations of  the given string. It uses data[] to store all
   permutations one by one */
void allLexicographicRecur (char **str, char** data, int last, int index)
{
    int i, len = strlen(str);
 
    // One by one fix all characters at the given index and recur for 
    // the/ subsequent indexes
    for ( i=0; i<len; i++ )
    {
        // Fix the ith character at index and if this is not the last 
        // index then recursively call for higher indexes
        data[index] = str[i] ;
 
        // If this is the last index then print the string stored in
        // data[]
        if (index == last)
            printf("%s\n", data);
        else // Recur for higher indexes
            allLexicographicRecur (str, data, last, index+1);
    }
}
 
/* This function sorts input string, allocate memory for data (needed 
   for allLexicographicRecur()) and calls allLexicographicRecur() for
   printing all  permutations */
void allLexicographic(char **str, int n)
{
    int len = n ;
 
    // Create a temp array that will be used by allLexicographicRecur()
    char **data = (char **) malloc (sizeof(char*) * (len + 1)) ;

    for(int i = 0; i < n; i++){
        data[i] = (char*) malloc(sizeof(char) * 3);
        data[i][2] = '\0';
    }
 
    // Sort the input string so that we get all output strings in
    // lexicographically sorted order
    // qsort(str, len, sizeof(char), compare);
    // for(int i = 0; i < n; i++){
    //     printf("%s\n",str[i]);
    // }
    // Now print all permutaions
    allLexicographicRecur (str, data, len-1, 0);
 
    // // Free data to avoid memory leak
    // free(data);
}
 
// Needed for library function qsort()
int compare (const void * a, const void * b)
{
    return ( *(char *)a - *(char *)b );
}
 
// Driver program to test above functions
int main()
{
    char* str[] = {"03","01","02"};
    // printf("All permutations with repetition of %s are: \n",
    //         str);
    allLexicographic(str, 3);
    return 0;
}