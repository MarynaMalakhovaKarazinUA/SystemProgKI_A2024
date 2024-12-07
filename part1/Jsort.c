#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct
{ float timeAsc;
  float timeDesc;
  float timeRand;
  int NCompAsc;
  int NCompDesc;
  int NCompRand;
  int NSwapAsc;
  int NSwapDesc;
  int NSwapRand;
} BUBBLESORTTR_TD;

void fillRandom(int arr[], int size);
void printArray(int arr[], int size);
void JSort(int arr[], int size);
void forwardHeapify(int arr[], int size);
void reverseHeapify(int arr[], int size);
void heapify(int arr[], int size, bool isMinHeap);
void siftDown(int arr[], int size, int index, bool isMinHeap);
void insertionSort(int arr[], int size);

// Function to read an array of integers from a text file
int* ReadIntArrayFromTextFile(const char* filename, int* length)
{ FILE *file;
  int *array;
  int capacity;
  int number;
  //
  file = fopen(filename, "r");
  if (!file) // Checking whether the file was opened successfully
  { perror("Unable to open file");
    return NULL;
  }
  // Variable init
  array = NULL;
  *length = 0;
  capacity = 10; // Start volume array
  // Allocate start memory
  array = (int*)malloc(capacity * sizeof(int));
  if (!array)
  { perror("Memory allocation failed\n");
    fclose(file);
    return NULL;
  }
  // Read numbers from the file and expand the array if necessary
  while (fscanf(file, "%d", &number) == 1)
  { if (*length == capacity) // Checking whether the array size needs to be increased
    { capacity *= 2; // Double the capacity
      int* new_array = (int*)realloc(array, capacity * sizeof(int));
      if (!new_array)
      { perror("Memory reallocation failed\n");
        free(array);
        fclose(file);
        return NULL;
      }
      array = new_array;
    }

    // Adding a number to the array
    array[*length] = number;
    (*length)++;
  }

  fclose(file);

  // We compress the array to a size corresponding to the number of numbers read
  int* new_array = (int*)realloc(array, (*length) * sizeof(int));
  if (!new_array)
  { perror("Memory reallocation failed.\n");
    array = new_array;
    return NULL;
  }
  return array;
}
//
void GenerateTestArray(int *aAsc, int *aDesc, int *aRand, int S)
{ int i;
  for (i=0; i<S; i++)
  { aAsc[i] = i+1;
    aDesc[i] = S-i;
    aRand [i] = rand() % 100 + 1;
  }
}
//
void GenerateAsc(int *aAsc, int S)
{ int i;
  for (i=0; i<S; i++)
  { aAsc[i] = i+1;}
}
//
void GenerateDesc(int *aDesc, int S)
{ int i;
  for (i=0; i<S; i++)
  { aDesc[i] = S-i;}
}
//
void GenerateRand(int *aRand, int S)
{ int i;
  for (i=0; i<S; i++)
  { aRand [i] = rand() % 100 + 1;}
}
// Function to read an array from a binary file
int readArrayFromFile(const char* filename, int* array, size_t arraySize)
{ // Open the binary file for reading
  FILE* file = fopen(filename, "rb");
  if (file == NULL)   // Check if the file was opened successfully
  { return 0;} // File open error}
  size_t elementsRead = fread(array, sizeof(int), arraySize, file);
  fclose(file);
  if (elementsRead != arraySize)
  { return -1; // File read error
  }
  return 1; // Successful read

}
// Function to write an array to a binary file
int writeArrayToFile(const char* filename, const int* array, size_t arraySize)
{ // Open the binary file for writing
  FILE* file = fopen(filename, "wb");
  if (file == NULL) // Check if the file was opened successfully
  { return 0; // File open error
  }
  size_t elementsWritten = fwrite(array, sizeof(int), arraySize, file);
  fclose(file);
  if (elementsWritten != arraySize)
  { return -1; // File write error
  }
  return 1; // Successful write
}

// Builds a forward min-heap from the given array
void forwardHeapify(int arr[], int size)
{ heapify(arr, size, true);
}

// Builds a reverse max-heap from the given array
void reverseHeapify(int arr[], int size)
{
  for (int i = 0; i < size / 2; i++)
  { int temp = arr[i];
    arr[i] = arr[size - i - 1];
    arr[size - i - 1] = temp;
  }
  heapify(arr, size, false);
  for (int i = 0; i < size / 2; i++)
  { int temp = arr[i];
    arr[i] = arr[size - i - 1];
    arr[size - i - 1] = temp;
  }
}

// Build an in-place heap from the given data
void heapify(int arr[], int size, bool isMinHeap)
{ // Iterate backward from the last parent (non-leaf) node
  for (int i = (size - 2) / 2; i >= 0; --i)
    // Restore balance to the Heap
    siftDown(arr, size, i, isMinHeap);
}

// Sifts down the element at the given index
void siftDown(int arr[], int size, int index, bool isMinHeap)
{ int parent = index;

  // While the root has at least one child
  while (parent * 2 + 1 < size)
  { int swap = parent; // To keep track of which of the three relevant nodes (parent, left child, right child) is largest
    int leftChild = parent * 2 + 1; // The root's left child

    // Find the largest of the three relevant nodes
    if ((arr[leftChild] < arr[swap]) == isMinHeap) // Left child
      swap = leftChild;
    // Right child (with existence check, because the tree may have a node with only one child)
    if (leftChild + 1 < size && (arr[leftChild + 1] < arr[swap]) == isMinHeap)
      swap = leftChild + 1;

    // If the parent is the largest of the three, this part of the heap is valid, so we do nothing
    if (swap == parent)
      return;

    // Otherwise, swap the parent with the greatest child and start over
    int temp = arr[parent];
    arr[parent] = arr[swap];
    arr[swap] = temp;
    parent = swap;
  }
}

// Fast insertion sort
void insertionSort(int arr[], int size)
{ for (int i = 1; i < size; ++i)
  { int temp = arr[i];
    int j = i;
    while (j > 0 && arr[j - 1] > temp)
    { arr[j] = arr[j - 1];
      --j;
    }
    arr[j] = temp;
  }
}
// Sorts a list using the JSort algorithm
void JSort(int arr[], int size)
{ forwardHeapify(arr, size);
  reverseHeapify(arr, size);
  insertionSort(arr, size);
}

void merge(int arr[], int p, int q, int r)
{
  int n1 = q - p + 1;
  int n2 = r - q;

  int L[n1], M[n2];

  for (int i = 0; i < n1; i++)
    L[i] = arr[p + i];
  for (int j = 0; j < n2; j++)
    M[j] = arr[q + 1 + j];

  // Maintain current index of sub-arrays and main array
  int i, j, k;
  i = 0;
  j = 0;
  k = p;

  // Until we reach either end of either L or M, pick larger among
  // elements L and M and place them in the correct position at A[p..r]
  while (i < n1 && j < n2)
   {
    if (L[i] <= M[j])
    {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = M[j];
      j++;
    }
    k++;
  }

  // When we run out of elements in either L or M,
  // pick up the remaining elements and put in A[p..r]
  while (i < n1)
  {
    arr[k] = L[i];
    i++;
    k++;
  }

  while (j < n2)
  {
    arr[k] = M[j];
    j++;
    k++;
  }
}

// Divide the array into two subarrays, sort them and merge them
void mergeSort(int arr[], int l, int r)
{
  if (l < r)
  { // m is the point where the array is divided into two subarrays
    int m = l + (r - l) / 2;

    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);

    // Merge the sorted subarrays
    merge(arr, l, m, r);
  }
}

int main()
{ char *filename = "SizesTestedArrays.txt";
  char *fnout = "JSortTestResult.csv";
  int maxSizeArray;
  int i;
  int *arrS;                // array with size
  int NTest;                // Number of test read from file
  int *arrAsc;              // pointers to ascending array
  int *arrDesc;             // pointers to descending array
  int *arrRand;             // pointer to random array
  BUBBLESORTTR_TD *TR;      // structure for store test result
  char sNF[256];
  struct timespec start, end;   // Start the timer
  float te;         // time for current sorting
  FILE *fout;
  //
  printf("...Sort speed test...\n");
  //
  printf("Reading size for tested arrays...");
  arrS = ReadIntArrayFromTextFile(filename,&NTest);
  if (arrS != NULL)
  { printf("Number of test is: %d\n", NTest);
  }
  else
  { perror("Error reading test conditions.\n");
    return 100;
  }
//
  maxSizeArray = 0;
  for (i=0; i<NTest; i++)
  { if (NTest<arrS[i]) maxSizeArray = arrS[i];
  }
  // Allocate memory for test array
  arrAsc = (int*)malloc(maxSizeArray * sizeof(int));
  arrDesc = (int*)malloc(maxSizeArray * sizeof(int));
  arrRand = (int*)malloc(maxSizeArray * sizeof(int));
  TR = (BUBBLESORTTR_TD*)malloc(maxSizeArray * sizeof(BUBBLESORTTR_TD));
  if ((arrAsc == NULL)||(arrDesc == NULL)||(arrRand == NULL)||(TR == NULL))
  { perror("Memory allocation failed\n");
    return 100;
  }
  for (i=0; i<NTest; i++)
  { TR[i].NCompAsc = 0;
    TR[i].NCompDesc = 0;
    TR[i].NCompRand = 0;
    TR[i].NSwapAsc = 0;
    TR[i].NSwapDesc = 0;
    TR[i].NSwapRand = 0;
    TR[i].timeAsc = 0;
    TR[i].timeDesc = 0;
    TR[i].timeRand = 0;
  }
  //
  // MAIN CYCLETest
  //
    // Jutput result to csv file
  fout = fopen(fnout, "w");
  if (!fout) // Checking whether the file was opened successfully
  { perror("Unable to open csv file");
    return 100;
  }
  //
  srand(time(NULL));
  for (i=0; i<NTest; i++)
  { // read test array for one test speed
    GenerateAsc(arrAsc,arrS[i]);
    GenerateDesc(arrDesc,arrS[i]);
    sprintf(sNF,"./data/%drand.bin",arrS[i]);
    if (readArrayFromFile(sNF, arrRand,arrS[i]) != 1)
    { GenerateRand(arrRand,arrS[i]);
      writeArrayToFile(sNF, arrRand,arrS[i]);
    }
    //
    // JSORT
    //
    clock_gettime(CLOCK_MONOTONIC, &start);
    JSort(arrAsc,arrS[i]);
    clock_gettime(CLOCK_MONOTONIC, &end);
    te = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    TR[i].timeAsc = te;
    //
    clock_gettime(CLOCK_MONOTONIC, &start);
    JSort(arrDesc,arrS[i]);
    clock_gettime(CLOCK_MONOTONIC, &end);
    te = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    TR[i].timeDesc = te;
    //
    clock_gettime(CLOCK_MONOTONIC, &start);
    JSort(arrRand,arrS[i]);
    clock_gettime(CLOCK_MONOTONIC, &end);
    te = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    TR[i].timeRand = te;
    //
    printf("%10d %10.4f %10.4f %10.4f",arrS[i],TR[i].timeAsc,TR[i].timeDesc,TR[i].timeRand);
    fprintf(fout,"%10d, %10.4f, %10.4f, %10.4f,",arrS[i],TR[i].timeAsc,TR[i].timeDesc,TR[i].timeRand);
    //
    //
    sprintf(sNF,"./data/%drandsortshel.bin",arrS[i]);
    writeArrayToFile(sNF, arrRand,arrS[i]);
    //
    // read test array for one test speed
    GenerateAsc(arrAsc,arrS[i]);
    GenerateDesc(arrDesc,arrS[i]);
    sprintf(sNF,"./data/%drand.bin",arrS[i]);
    if (readArrayFromFile(sNF, arrRand,arrS[i]) != 1)
    { GenerateRand(arrRand,arrS[i]);
      writeArrayToFile(sNF, arrRand,arrS[i]);
    }
    //
    // MERGE SORT
    //
    clock_gettime(CLOCK_MONOTONIC, &start);
    mergeSort(arrAsc,0,arrS[i]-1);
    clock_gettime(CLOCK_MONOTONIC, &end);
    te = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    TR[i].timeAsc = te;
    //
    clock_gettime(CLOCK_MONOTONIC, &start);
    mergeSort(arrDesc,0,arrS[i]-1);
    clock_gettime(CLOCK_MONOTONIC, &end);
    te = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    TR[i].timeDesc = te;
    //
    clock_gettime(CLOCK_MONOTONIC, &start);
    mergeSort(arrRand,0,arrS[i]-1);
    clock_gettime(CLOCK_MONOTONIC, &end);
    te = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    TR[i].timeRand = te;
    //
    printf("%10d %10.4f %10.4f %10.4f",arrS[i],TR[i].timeAsc,TR[i].timeDesc,TR[i].timeRand);
    fprintf(fout,"%10d, %10.4f, %10.4f, %10.4f\n",arrS[i],TR[i].timeAsc,TR[i].timeDesc,TR[i].timeRand);
    //
    printf("\n");
    //
    sprintf(sNF,"./data/%drandsortciura.bin",arrS[i]);
    writeArrayToFile(sNF, arrRand,arrS[i]);
  }
  //
  fclose(fout);
}
