
#pragma once

#include <windows.h>

class CpuUsage
{
public:
	CpuUsage() :
		m_nCpuUsage(-1),
		m_dwLastRun(0),
		m_lRunCount(0)
	{
		ZeroMemory(&m_ftPrevSysKernel, sizeof(FILETIME));
		ZeroMemory(&m_ftPrevSysUser, sizeof(FILETIME));

		ZeroMemory(&m_ftPrevProcKernel, sizeof(FILETIME));
		ZeroMemory(&m_ftPrevProcUser, sizeof(FILETIME));
	}

	/*
	Returns the percent of the CPU that this process has used since the last time the method was called.
	If there is not enough information, -1 is returned.
	If the method is recalled to quickly, the previous value is returned.
	*/
	short GetUsage()
	{
		//create a local copy to protect against race conditions in setting the member variable
		short nCpuCopy = m_nCpuUsage;
		if (::InterlockedIncrement(&m_lRunCount) == 1)
		{
			//If this is called too often, the measurement itself will greatly affect the results.

			if (!EnoughTimePassed())
			{
				::InterlockedDecrement(&m_lRunCount);
				return nCpuCopy;
			}

			FILETIME ftSysIdle, ftSysKernel, ftSysUser;
			FILETIME ftProcCreation, ftProcExit, ftProcKernel, ftProcUser;

			if (!GetSystemTimes(&ftSysIdle, &ftSysKernel, &ftSysUser) ||
				!GetProcessTimes(GetCurrentProcess(), &ftProcCreation, &ftProcExit, &ftProcKernel, &ftProcUser))
			{
				::InterlockedDecrement(&m_lRunCount);
				return nCpuCopy;
			}

			if (!IsFirstRun())
			{
				/*
				CPU usage is calculated by getting
				the total amount of time the system has operated since the last measurement (made up of kernel + user)
				and the total amount of time the process has run (kernel + user).
				*/
				unsigned long long int ftSysKernelDiff = SubtractTimes(ftSysKernel, m_ftPrevSysKernel);
				unsigned long long int ftSysUserDiff = SubtractTimes(ftSysUser, m_ftPrevSysUser);

				unsigned long long int ftProcKernelDiff = SubtractTimes(ftProcKernel, m_ftPrevProcKernel);
				unsigned long long int ftProcUserDiff = SubtractTimes(ftProcUser, m_ftPrevProcUser);

				unsigned long long int nTotalSys = ftSysKernelDiff + ftSysUserDiff;
				unsigned long long int nTotalProc = ftProcKernelDiff + ftProcUserDiff;

				if (nTotalSys > 0) m_nCpuUsage = (short)((100.0 * nTotalProc) / nTotalSys);
			}

			m_ftPrevSysKernel = ftSysKernel;
			m_ftPrevSysUser = ftSysUser;
			m_ftPrevProcKernel = ftProcKernel;
			m_ftPrevProcUser = ftProcUser;

			m_dwLastRun = GetTickCount64();

			nCpuCopy = m_nCpuUsage;
		}

		::InterlockedDecrement(&m_lRunCount);

		return nCpuCopy;
	}

private:
	unsigned long long int SubtractTimes(const FILETIME& ftA, const FILETIME& ftB)
	{
		LARGE_INTEGER a, b;
		a.LowPart = ftA.dwLowDateTime;
		a.HighPart = ftA.dwHighDateTime;

		b.LowPart = ftB.dwLowDateTime;
		b.HighPart = ftB.dwHighDateTime;

		return a.QuadPart - b.QuadPart;
	}

	bool EnoughTimePassed()
	{
		const int minElapsedMS = 250;//milliseconds

		unsigned long long int dwCurrentTickCount = GetTickCount64();
		return (dwCurrentTickCount - m_dwLastRun) > minElapsedMS;
	}

	inline bool IsFirstRun() const
	{
		return (m_dwLastRun == 0);
	}

	//system total times
	FILETIME m_ftPrevSysKernel, m_ftPrevSysUser;
	//process times
	FILETIME m_ftPrevProcKernel, m_ftPrevProcUser;

	short m_nCpuUsage;
	unsigned long long int m_dwLastRun;

	volatile long int m_lRunCount;
};

#include <iostream>
CpuUsage usage;

unsigned long int WINAPI WatchItThreadProc(void* lpParam)
{
	while (1)
	{
		printf("Thread id %d: %d%% cpu usage\n", ::GetCurrentThreadId(), usage.GetUsage());
		Sleep(1000);
	}
}

unsigned long int WINAPI EatItThreadProc(void* lpParam)
{
	unsigned __int64 accum = 0;
	while (1) accum++;
	printf("%64d\n", accum);
}


int main(int argc, char** argv)
{
	//start threads to eat the processor
	CreateThread(0, 0, EatItThreadProc, 0, 0, 0);

	//start threads to watch the processor (to test thread-safety)
	CreateThread(0, 0, WatchItThreadProc, 0, 0, 0);

	while (1)
	{
		Sleep(1000);
	}

	return 0;
}
