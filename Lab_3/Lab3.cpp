#include <omp.h>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
using namespace std;

void func()
{
  for (int i = 0; i < 500000; i++)
    rand();
}
int main()
{
  setlocale(LC_ALL, "rus");

#pragma omp parallel
  {
#pragma omp sections nowait
    {
#pragma omp section 
      for (int i = 0; i < 10; i++)
      {
        cout << 1; func();
      }
#pragma omp section
      for (int i = 0; i < 15; i++)
      {
        cout << 2; func();
      } 
#pragma omp section
      for (int i = 0; i < 15; i++)
      {
        cout << 3; func();
      }
#pragma omp section
      for (int i = 0; i < 15; i++)
      {
        cout << 4; func();
      }
#pragma omp section
      for (int i = 0; i < 15; i++)
      {
        cout << 5; func();
      }
#pragma omp section
      for (int i = 0; i < 15; i++)
      {
        cout << 6; func();
      }
#pragma omp section
      for (int i = 0; i < 15; i++)
      {
        cout << 7; func();
      }
#pragma omp section
      for (int i = 0; i < 15; i++)
      {
        cout << 8; func();
      }
#pragma omp section
      for (int i = 9; i < 15; i++)
      {
        cout << 9; func();
      }
#pragma omp section
      for (int i = 10; i < 15; i++)
      {
        cout << 11; func();
      }
#pragma omp section
      for (int i = 0; i < 15; i++)
      {
        cout << 12; func();
      }
#pragma omp section 
      cout << endl <<  "Количество потоков: " << omp_get_num_threads() << endl;
    }
#pragma omp barrier
    for (int i = 0; i < 5; i++)
    {
      cout << 3;
      func();
    }
  }
  cout << endl;
  int k = 0;
#pragma omp parallel 
  {
#pragma omp atomic
    k++;
#pragma omp atomic
    k++;
#pragma omp critical   
    {
      cout << k << std::endl;
    }
  }
  omp_set_num_threads(4);
  omp_lock_t lock;
  int n;
  omp_init_lock(&lock);
#pragma omp parallel private (n)
  {
    n = omp_get_thread_num();
    while (!omp_test_lock(&lock))
    {
      cout << endl << "Waiting...Number of threads: " << n ;
      Sleep(3);
    }
    cout << endl << "Start work. Threads number is: " << n ;
    Sleep(5); // Work...
    cout << endl << "End work. Threads number is: " << n ;
    omp_unset_lock(&lock);
  }
  omp_destroy_lock(&lock);

}