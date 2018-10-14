// monitorSistema.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <mutex>


#pragma comment(lib, "iphlpapi.lib")
std::mutex candado;


void usoDelCPU();
void usoDeRam();
void datagramas();


int main()
{

	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 51);
	//printf("Hola %d\n",51);
	int i;
	std::thread hilos[3];

	for (i = 0; i < 4 ; i++)
	{
		printf("\n\nPasada %d: \n\n",i);
		hilos[0] = std::thread(usoDelCPU);
		hilos[1] = std::thread(usoDeRam);
		hilos[2] = std::thread(datagramas);

		//Sleep(2000);

		hilos[0].join();
		hilos[1].join();
		hilos[2].join();
	}
	getchar();
    return 0;
}

void usoDelCPU()
{
	FILETIME idle, kernel, user;
	//SYSTEMTIME sIdle, sKernel, sUser;
	ULARGE_INTEGER aIdle, nIdle, aKernel, nKernel, aUser, nUser, tIdle, tKernel, tUser;
	unsigned long long int usoCPU;
	int i=0;

	//------

	GetSystemTimes(&idle, &kernel, &user);

	//FileTimeToSystemTime(&idle, &sIdle);
	//FileTimeToSystemTime(&kernel, &sKernel);
	//FileTimeToSystemTime(&user, &sUser);

	aIdle.LowPart = idle.dwLowDateTime;
	aIdle.HighPart = idle.dwHighDateTime;
	aKernel.LowPart = kernel.dwLowDateTime;
	aKernel.HighPart = kernel.dwHighDateTime;
	aUser.LowPart = user.dwLowDateTime;
	aUser.HighPart = user.dwHighDateTime;

	//printf("Tiempo inicio: \n\n");
	//printf("Tiempo en Idle: %d horas %d min %d seg %d ms\n", sIdle.wHour, sIdle.wMinute, sIdle.wSecond, sIdle.wMilliseconds);
	//printf("Tiempo en Kernel: %d horas %d min %d seg %d ms\n", sKernel.wHour, sKernel.wMinute, sKernel.wSecond, sKernel.wMilliseconds);
	//printf("Tiempo en User: %d horas %d min %d seg %d ms\n\n", sUser.wHour, sUser.wMinute, sUser.wSecond, sUser.wMilliseconds);

	Sleep(1000); //Espera para volver a medir los tiempos del sistema

	//------

	//for (i = 0; i < 100; i++)
	//{
		GetSystemTimes(&idle, &kernel, &user); //vuelve a medirlos

		//system("cls");

		nIdle.LowPart = idle.dwLowDateTime;
		nIdle.HighPart = idle.dwHighDateTime;
		nKernel.LowPart = kernel.dwLowDateTime;
		nKernel.HighPart = kernel.dwHighDateTime;
		nUser.LowPart = user.dwLowDateTime;
		nUser.HighPart = user.dwHighDateTime;

		tIdle.QuadPart = nIdle.QuadPart - aIdle.QuadPart; //Calculamos los tiempos totales
		tUser.QuadPart = nUser.QuadPart - aUser.QuadPart;
		tKernel.QuadPart = nKernel.QuadPart - aKernel.QuadPart;

		//calculamos el uso del CPU
		usoCPU = ((tKernel.QuadPart + tUser.QuadPart - tIdle.QuadPart) * 100 / (tKernel.QuadPart + tUser.QuadPart));

		//idle.dwHighDateTime = tIdle.HighPart;
		//idle.dwLowDateTime = tIdle.LowPart;
		//kernel.dwHighDateTime = tKernel.HighPart;
		//kernel.dwLowDateTime = tKernel.LowPart;
		//user.dwHighDateTime = tUser.HighPart;
		//user.dwLowDateTime = tUser.LowPart;

		//FileTimeToSystemTime(&idle, &sIdle);
		//FileTimeToSystemTime(&kernel, &sKernel);
		//FileTimeToSystemTime(&user, &sUser);


		//printf("Tiempo %i: \n\n", i);
		//printf("Tiempo en Idle: %d horas %d min %d seg %d ms\n", sIdle.wHour, sIdle.wMinute, sIdle.wSecond, sIdle.wMilliseconds);
		//printf("Tiempo en Kernel: %d horas %d min %d seg %d ms\n", sKernel.wHour, sKernel.wMinute, sKernel.wSecond, sKernel.wMilliseconds);
		//printf("Tiempo en User: %d horas %d min %d seg %d ms\n", sUser.wHour, sUser.wMinute, sUser.wSecond, sUser.wMilliseconds);
		//candado.lock();
		printf("Uso del CPU: %llu\n\n", usoCPU);
		//candado.unlock();

		aIdle.QuadPart = nIdle.QuadPart;
		aKernel.QuadPart = nKernel.QuadPart;
		aUser.QuadPart = nUser.QuadPart;

		Sleep(1000);
	//}

}

void usoDeRam()
{
	MEMORYSTATUSEX memoria;
	int i;
	unsigned long long int usada;
	memoria.dwLength = sizeof(memoria);

	//for (i = 0; i < 20; i++)
	//{
		GlobalMemoryStatusEx(&memoria);
		usada = memoria.ullTotalPhys - memoria.ullAvailPhys;
		//candado.lock();
		printf("Hay %llu memoria RAM  con %llu disponible\n", memoria.ullTotalPhys, memoria.ullAvailPhys);
		printf("Usada %llu\t %u %% \n\n", usada, memoria.dwMemoryLoad);
		//candado.unlock();
		Sleep(1000);
	//}

}

void datagramas()
{
	//int i;
	unsigned int rAnt, rNvo, eAnt, eNvo;
	float dTot, dEot;
	MIB_IPSTATS *pStats;
	pStats = (MIB_IPSTATS *)malloc(sizeof(MIB_IPSTATS));

	GetIpStatistics(pStats);
	rAnt = pStats->dwInReceives;
	eAnt = pStats->dwOutRequests;//- pStats->dwOutDiscards;
	Sleep(1000);

	//for (i = 0; i < 100; i++)
	//{
		GetIpStatistics(pStats);
		rNvo = pStats->dwInReceives;
		eNvo = pStats->dwOutRequests;//- pStats->dwOutDiscards;
									 //rTot = (rNvo - rAnt);
		dTot = (float)(rNvo - rAnt)*(1.5f);
		dEot = (float)(eNvo - eAnt)*(1.5f);
		//candado.lock();
		if (dTot > 999)
		{
			dTot = dTot / 1000;
			printf("Recibo: %g Mbps\t|\t", dTot);
		}
		else
		{
			printf("Recibo: %g kbps\t|\t", dTot);
		}
		if (dEot > 999)
		{
			dTot = dTot / 1000;
			printf("Envio: %g Mbps\n", dEot);
		}
		else
		{
			printf("Envio: %g kbps\n", dEot);
		}
		//candado.unlock();

		rAnt = rNvo;
		eAnt = eNvo;
		Sleep(1000);
	//}

}