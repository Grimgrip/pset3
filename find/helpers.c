/*helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

//Returns true if value is in array of n values, else false.
bool search(int value, int values[], int n)
{
    int needle = value;
    int min = 0;
    int max = n-1; 
    
    while(max >= min)
    {
        //int mid = values[n/2];
        int mid = (min + max)/2;
        
        if(values[mid] == needle)
            return true;
        else if(values[mid] > needle)
            max = mid - 1;
        else if(values[mid] < needle)
            min = mid + 1;
    }
    
    return false;
    
    /*for(int i = 0; i < n; i++)
    {
        if(values[i] == value)
            return true;
    }
    return false;*/
}

//Sorts array of n values.
void sort(int values[], int n)
{   // TODO: implement an O(n^2) sorting algorithm
    
    int temp;
    
    for(int i = 0; i < n; i++)
    {
        int tiny = i;
        for(int j = i + 1; j < n; j++)
        {
            if(values[tiny] > values[j])
                tiny = j;
        }
        
        temp = values[tiny];
        values[tiny] = values[i];
        values[i] = temp;
    }
}

/*int main (int argc, string argv[])
{
    return 0;
}*/
