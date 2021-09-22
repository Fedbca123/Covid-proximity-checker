#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*COP 3502C Programming Assignment3 
*This program is written by:
*Omar Shalaby
*
*/

//Functions needed to implement: ReadData(), merge(), mergeSort(), InsertionSort(), sortDecider(), and compareTo().

typedef struct point
{
  int x;
  int y;
} point;

point myLocation;

point *readData(FILE *fp, int n, int s, point *people);
void sortDecider(point *list, int n, int threshold);
void insertionSort(point *list, int n);
int compareTo(point Ptr1, point Ptr2);
void mergeSort(point *list, int l, int r);
void merge(point *list, int l, int m, int r);
int binSearch(point *values, int low, int high, point searchval);

int main(void)
{
  FILE *fp, *fout;
  fp = fopen("assignment3input.txt", "r");
  fout = fopen("out.txt", "w");

  fscanf(fp, "%d", &myLocation.x);
  fscanf(fp, "%d", &myLocation.y);

  int numInfected, numToSearch, threshold;

  fscanf(fp, "%d", &numInfected);
  fscanf(fp, "%d", &numToSearch);
  fscanf(fp, "%d", &threshold);

  point *infectedPeople;

  infectedPeople = readData(fp, numInfected, numToSearch, infectedPeople);

  sortDecider(infectedPeople, numInfected, threshold);

  printf("printing out infected people coordinates:\n");
  fprintf(fout, "printing out infected people coordinates:\n");
  for (int i = 0; i < numInfected; i++)
  {
    printf("%d %d\n", infectedPeople[i].x, infectedPeople[i].y);
    fprintf(fout, "%d %d\n", infectedPeople[i].x, infectedPeople[i].y);
  }

  printf("printing out coordinates to search: \n");
  fprintf(fout, "printing out coordinates to search: \n");
  for (int i = numInfected; i < (numToSearch + numInfected); i++)
  {
    int found = binSearch(infectedPeople, 0, numInfected, infectedPeople[(numInfected + i)]);
    if (found == -1)
    {
      printf("point:%d %d not found\n", infectedPeople[(i + numInfected)].x,
             infectedPeople[(i + numInfected)].y);
      fprintf(fout, "point:%d %d not found\n", infectedPeople[(i + numInfected)].x,
              infectedPeople[(i + numInfected)].y);
    }
    else
    {
      printf("point: %d %d at Rank %d\n", infectedPeople[i].x, infectedPeople[i].y, found);
      fprintf(fout, "point: %d %d at Rank %d\n", infectedPeople[i].x, infectedPeople[i].y, found);
    }
  }

  free(infectedPeople);

  fclose(fp);
  fclose(fout);
}

point *readData(FILE *fp, int n, int s, point *people)
{

  people = malloc(sizeof(point) * (n + s));

  for (int i = 0; i < (n + s); i++)
  {
    fscanf(fp, "%d", &people[i].x);
    fscanf(fp, "%d", &people[i].y);
  }

  return people;
}

void sortDecider(point *list, int n, int threshold)
{
  //decides which sort to use

  if (n <= threshold)
  {
    insertionSort(list, n);
  }
  else
  {
    mergeSort(list, 0, n - 1);
  }
}

void insertionSort(point *list, int n)
{
  int i, j;
  point item;
  for (i = 1; i < n; i++)
  {
    item = list[i];
    for (j = i - 1; j >= 0; j--)
    {
      if (compareTo(list[j], item) == -1) // if item is closer to my location than list[j], item gets inserted.
        list[j + 1] = list[j];
      else
        break;
    }
    list[j + 1] = item;
  }
}

int compareTo(point Ptr1, point Ptr2)
{
  double dist1, dist2;

  dist1 = sqrt((pow((Ptr1.x - myLocation.x), 2) + pow((Ptr1.y - myLocation.y), 2)));

  dist2 = sqrt((pow((Ptr2.x - myLocation.x), 2) + pow((Ptr2.y - myLocation.y), 2)));

  if (dist1 < dist2)
  {
    return 1;
  }
  else if (dist1 > dist2)
  {
    return -1;
  }
  else if (dist1 == dist2)
  {
    if (Ptr1.x < Ptr2.x)
      return 1;
    else if (Ptr1.x < Ptr2.x)
      return -1;
    else if (Ptr1.y < Ptr2.y)
      return 1;
    else if (Ptr1.y > Ptr2.y)
      return -1;
  }
  return -1;
}

void mergeSort(point *list, int l, int r)
{
  if (l < r)
  {                      // get the mid point
    int m = (l + r) / 2; // Sort first and second halves
    mergeSort(list, l, m);
    mergeSort(list, m + 1, r);
    //printf("Testing l=%d r=%d m=%d\n", l, r, m);
    merge(list, l, m, r);
  }
}

void merge(point *list, int l, int m, int r)
{
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;
  point *L = malloc(n1 * sizeof(int));
  point *R = malloc(n2 * sizeof(int)); /* Copy data to temp arrays L[] and R[] */
  for (i = 0; i < n1; i++)
    L[i] = list[l + i];
  for (j = 0; j < n2; j++)
    R[j] = list[m + 1 + j]; /* Merge the temp arrays back into list[l..r]*/

  i = 0;
  j = 0;
  k = l;
  while (i < n1 && j < n2)
  {
    if (compareTo(L[i], R[j]) == 1)
    {
      list[k] = L[i];
      i++;
    }
    else
    {
      list[k] = R[j];
      j++;
    }

    k++;
  }

  while (i < n1)
  {
    list[k] = L[i];
    i++;
    k++;
  }

  while (j < n2)
  {
    list[k] = R[j];
    j++;
    k++;
  }

  //free(L);
  //free(R);
}

int binSearch(point *values, int low, int high, point searchval)
{
  int mid;
  if (low <= high)
  {
    mid = (low + high) / 2;
    if (compareTo(searchval, values[mid]) == -1)
      return binSearch(values, low, mid - 1, searchval);
    else if (compareTo(searchval, values[mid]) == 1)
      return binSearch(values, mid + 1, high, searchval);
    else
      return -1;
  }
  return 0;
}
