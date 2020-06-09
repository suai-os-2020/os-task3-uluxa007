#include "lab3.h"
#include <windows.h>

//Threads
#define THREAD_A 0
#define THREAD_B 1
#define THREAD_C 2
#define THREAD_D 3
#define THREAD_E 4
#define THREAD_F 5
#define THREAD_G 6
#define THREAD_H 7
#define THREAD_I 8
#define THREAD_K 9
#define THREAD_M 10

#define NUMBER_OF_THREADS 11

HANDLE semaphores[NUMBER_OF_THREADS];
HANDLE threads[NUMBER_OF_THREADS];

DWORD WINAPI thread_a(LPVOID);
DWORD WINAPI thread_b(LPVOID);
DWORD WINAPI thread_c(LPVOID);
DWORD WINAPI thread_d(LPVOID);
DWORD WINAPI thread_e(LPVOID);
DWORD WINAPI thread_f(LPVOID);
DWORD WINAPI thread_g(LPVOID);
DWORD WINAPI thread_h(LPVOID);
DWORD WINAPI thread_i(LPVOID);
DWORD WINAPI thread_k(LPVOID);
DWORD WINAPI thread_m(LPVOID);

LPTHREAD_START_ROUTINE WINAPI thread_func[NUMBER_OF_THREADS] = { thread_a,thread_b ,thread_c ,thread_d ,thread_e ,thread_f ,thread_g,thread_h,thread_i,thread_k,thread_m };


unsigned int lab3_thread_graph_id() 
{
    return 17;
}

const char* lab3_unsynchronized_threads()
{
    return "bcd";
}

const char* lab3_sequential_threads()
{
    return "fgh";
}

int lab3_init()
{
    	HANDLE lock = CreateMutex(NULL, FALSE, NULL);
	if (lock == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		return 1;
	}


	for (int i = 0; i < NUMBER_OF_THREADS; i++)
	{
		semaphores[i] = CreateSemaphore(NULL, 0, 4, NULL);
		if (NULL == semaphores[i]) {
			printf("Failed to create semaphore. %d\n", GetLastError());
			return 1;
		}
	}


	for (int i = 0; i < NUMBER_OF_THREADS; i++)
	{
		threads[i] = CreateThread(NULL, 0, thread_func[i], lock, 0, NULL);
		if (NULL == threads[i]) {
			printf("Failed to create thread C. %d\n", GetLastError());
			return 1;
		}
	}


	if (!ReleaseSemaphore(semaphores[THREAD_A], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	if (!ReleaseSemaphore(semaphores[THREAD_B], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	if (!ReleaseSemaphore(semaphores[THREAD_D], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }



	WaitForMultipleObjects(NUMBER_OF_THREADS, threads, TRUE, INFINITE);
	CloseHandle(lock);
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
	{
		CloseHandle(threads[i]);
	}
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
	{
		CloseHandle(semaphores[i]);
	}
    
    return 0;
}


DWORD WINAPI thread_a(LPVOID params)												//        ####
{																					//       ##  ##
	WaitForSingleObject(semaphores[THREAD_A],INFINITE);								//       ######
																					//       ##  ##
	CONST HANDLE hMutex = (CONST HANDLE)params;										//       ##  ##
	for (int i = 0; i < 3; ++i) {													
		WaitForSingleObject(hMutex, INFINITE);		
		std::cout << "a" << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	if (!ReleaseSemaphore(semaphores[THREAD_D], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }

	ExitThread(0);
}


DWORD WINAPI thread_b(LPVOID params)												//       #####
{																					//       ##  ##
	WaitForSingleObject(semaphores[THREAD_B], INFINITE);							//       #####
	CONST HANDLE hMutex = (CONST HANDLE)params;										//       ##  ##
	for (int i = 0; i < 3; ++i) {													//       #####
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << "b" << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	WaitForSingleObject(semaphores[THREAD_B], INFINITE);

	////////////////////////////////////////////////////////////////////////////////////////////////////////            T0

	if (!ReleaseSemaphore(semaphores[THREAD_C], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	if (!ReleaseSemaphore(semaphores[THREAD_D], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	for (int i = 0; i < 3; ++i) {
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << "b" << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	WaitForSingleObject(semaphores[THREAD_B], INFINITE);
	if (!ReleaseSemaphore(semaphores[THREAD_D], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }

	ExitThread(0);
}

DWORD WINAPI thread_c(LPVOID params)												 //        ####     
{																					 //       ##  ##     
	WaitForSingleObject(semaphores[THREAD_C], INFINITE);							 //       ##     
	CONST HANDLE hMutex = (CONST HANDLE)params;										 //       ##  ##     
	for (int i = 0; i < 3; ++i) {													 //        ####     
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << "c" << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	if (!ReleaseSemaphore(semaphores[THREAD_B], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }

	ExitThread(0);
}






DWORD WINAPI thread_d(LPVOID params)												  //       #####
{																					  //       ##  ##
	WaitForSingleObject(semaphores[THREAD_D], INFINITE);							  //       ##  ##
	CONST HANDLE hMutex = (CONST HANDLE)params;										  //       ##  ##
	for (int i = 0; i < 3; ++i) {													  //       #####
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << "d" << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	WaitForSingleObject(semaphores[THREAD_D], INFINITE);
	if (!ReleaseSemaphore(semaphores[THREAD_B], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	WaitForSingleObject(semaphores[THREAD_D], INFINITE);

	for (int i = 0; i < 3; ++i) {
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << "d" << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	WaitForSingleObject(semaphores[THREAD_D], INFINITE);

	////////////////////////////////////////////////////////////////////////////////////////////////////////            T1

	if (!ReleaseSemaphore(semaphores[THREAD_E], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	if (!ReleaseSemaphore(semaphores[THREAD_H], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	for (int i = 0; i < 3; ++i) {
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << "d" << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	WaitForSingleObject(semaphores[THREAD_D], INFINITE);
	if (!ReleaseSemaphore(semaphores[THREAD_H], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	ExitThread(0);
}








DWORD WINAPI thread_e(LPVOID params)										            //        #####
{																			            //        ##
	WaitForSingleObject(semaphores[THREAD_E], INFINITE);					            //        ####
	CONST HANDLE hMutex = (CONST HANDLE)params;								            //        ##
	for (int i = 0; i < 3; ++i) {											            //        #####
		WaitForSingleObject(hMutex, INFINITE);		
		std::cout << "e" << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	WaitForSingleObject(semaphores[THREAD_E], INFINITE);
	if (!ReleaseSemaphore(semaphores[THREAD_D], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	ExitThread(0);
}


DWORD WINAPI thread_f(LPVOID params)												   //     ######
{																					   //     ##
																					   //     ####
	CONST HANDLE hMutex = (CONST HANDLE)params;										   //     ##
	for (int i = 0; i < 3; ++i) {													   //     ##
		WaitForSingleObject(semaphores[THREAD_F], INFINITE);
		WaitForSingleObject(hMutex, INFINITE);	
		std::cout << "f" << std::flush;
		ReleaseMutex(hMutex);
		computation();
		if (!ReleaseSemaphore(semaphores[THREAD_H], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	}

	for (int i = 0; i < 3; ++i) {													
		WaitForSingleObject(semaphores[THREAD_F], INFINITE);
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << "f" << std::flush;
		ReleaseMutex(hMutex);
		computation();
		if (!ReleaseSemaphore(semaphores[THREAD_G], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	}
	ExitThread(0);
}


DWORD WINAPI thread_g(LPVOID params)													 //         ####
{																						 //        ##
																						 //        ## ###
	CONST HANDLE hMutex = (CONST HANDLE)params;											 //        ##  ##
	for (int i = 0; i < 3; ++i) {														 //         ####
		WaitForSingleObject(semaphores[THREAD_G], INFINITE);
		WaitForSingleObject(hMutex, INFINITE);	
		std::cout << "g" << std::flush;
		ReleaseMutex(hMutex);
		computation();
		if (!ReleaseSemaphore(semaphores[THREAD_H], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////            T3
	
	if (!ReleaseSemaphore(semaphores[THREAD_H], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	if (!ReleaseSemaphore(semaphores[THREAD_I], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	if (!ReleaseSemaphore(semaphores[THREAD_K], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }

	ExitThread(0);
}


DWORD WINAPI thread_h(LPVOID params)													  //       ##  ##
{																						  //       ##  ##
																						  //       ######
	CONST HANDLE hMutex = (CONST HANDLE)params;											  //       ##  ##
	for (int i = 0; i < 3; ++i) {														  //       ##  ##
		WaitForSingleObject(semaphores[THREAD_H], INFINITE);
		WaitForSingleObject(hMutex, INFINITE);											         
		std::cout << "h" << std::flush;
		ReleaseMutex(hMutex);
		computation();
		if (!ReleaseSemaphore(semaphores[THREAD_F], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	}
	WaitForSingleObject(semaphores[THREAD_H], INFINITE);
	if (!ReleaseSemaphore(semaphores[THREAD_E], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }//
	/////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////            T2
	/////////////////////
	for (int i = 0; i < 3; ++i) {														 
		WaitForSingleObject(semaphores[THREAD_H], INFINITE);
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << "h" << std::flush;
		ReleaseMutex(hMutex);
		computation();
		if (!ReleaseSemaphore(semaphores[THREAD_F], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	}
	WaitForSingleObject(semaphores[THREAD_H], INFINITE);
	for (int i = 0; i < 3; ++i) {
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << "h" << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	if (!ReleaseSemaphore(semaphores[THREAD_I], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	ExitThread(0);
}

DWORD WINAPI thread_i(LPVOID params)													 //       ######
{																						 //         ##
	WaitForSingleObject(semaphores[THREAD_I], INFINITE);								 //         ##
	CONST HANDLE hMutex = (CONST HANDLE)params;											 //         ##
	for (int i = 0; i < 3; ++i) {														 //       ######
		WaitForSingleObject(hMutex, INFINITE);	
		std::cout << "i" << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	WaitForSingleObject(semaphores[THREAD_I], INFINITE);
	if (!ReleaseSemaphore(semaphores[THREAD_K], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	ExitThread(0);
}

DWORD WINAPI thread_k(LPVOID params)													//        ##  ##
{																						//        ## ##
	WaitForSingleObject(semaphores[THREAD_K], INFINITE);								//        ####
	CONST HANDLE hMutex = (CONST HANDLE)params;											//        ## ##
	for (int i = 0; i < 3; ++i) {														//        ##  ##
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << "k" << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	WaitForSingleObject(semaphores[THREAD_K], INFINITE);
	if (!ReleaseSemaphore(semaphores[THREAD_M], 1, NULL)) { printf("ReleaseSemaphore error: %d\n", GetLastError()); }
	for (int i = 0; i < 3; ++i) {
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << "k" << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	ExitThread(0);
}

DWORD WINAPI thread_m(LPVOID params)												  //        ##   ##
{																					  //        ### ###
	WaitForSingleObject(semaphores[THREAD_M], INFINITE);							  //        ## # ##
	CONST HANDLE hMutex = (CONST HANDLE)params;										  //        ##   ##
	for (int i = 0; i < 3; ++i) {													  //        ##   ##
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << "m" << std::flush;
		ReleaseMutex(hMutex);
		computation();
	}
	ExitThread(0);
}
