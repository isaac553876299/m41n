/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once
#ifndef _ANALYTICS_HARDWARE_
#define _ANALYTICS_HARDWARE_


#include <Windows.h>
//#include <stdint.h>
#include <chrono>


namespace AnalyticsHardware
{

	class CPUInfo final
	{
	public:
		
		float CalculateCPULoad();

	private:

		float CalculateCPULoad(uint64_t idleTicks, uint64_t totalTicks);
		uint64_t FileTimeToInt64(const FILETIME& ft);

	private:

		uint64_t m_cpuPreviousTotalTicks = 0;
		uint64_t m_cpuPreviousIdleTicks = 0;

	};

	class RAMInfo final
	{
	public:
		
		void Update();

		float GetUsedRAM();
		float GetFreeRAM();
		float GetMaxRAM();

	private:

		MEMORYSTATUSEX m_statex;

	};

	class GPUInfo final
	{
	public:
		
		GPUInfo();

		float CalculateGPULoad();

	private:

		/* Internal typedef for NvAPI function pointers */

		typedef int* (*NvAPI_QueryInterface_t)(unsigned int offset);
		typedef int(*NvAPI_Initialize_t)();
		typedef int(*NvAPI_EnumPhysicalGPUs_t)(int** handles, int* count);
		typedef int(*NvAPI_GPU_GetUsages_t)(int* handle, unsigned int* usages);

		bool m_NvAPIReady = false;

		/* nvapi.dll internal function pointers */

		NvAPI_QueryInterface_t m_NvAPI_QueryInterface = nullptr;
		NvAPI_Initialize_t m_NvAPI_Initialize = nullptr;
		NvAPI_EnumPhysicalGPUs_t m_NvAPI_EnumPhysicalGPUs = nullptr;
		NvAPI_GPU_GetUsages_t m_NvAPI_GPU_GetUsages = nullptr;

		int m_gpuCount = 0;
		int* m_gpuHandles[64] = { nullptr };
		unsigned int gpuUsages[34] = { 0 };

	};

	struct HardwareReport final
	{
		float CPULoad;
		float GPULoad;
		float RAMUsed;
		float RAMFree;
		float RAMMax;
	};

	class HardwareInfo final
	{
	public:
		
		HardwareInfo(double p_timeInterval = 1.0);

		HardwareReport GenerateReport();

		void Tick();

	private:

		void Update();

		double timeInterval;
		double timer;

		std::chrono::steady_clock::time_point previousTime;
		std::chrono::steady_clock::time_point currentTime;
		std::chrono::duration<double> elapsedTime;

		float cpuUsage;
		float gpuUsage;

		CPUInfo cpuInfo;
		GPUInfo gpuInfo;
		RAMInfo ramInfo;

	};

}

// CPUInfo

float AnalyticsHardware::CPUInfo::CalculateCPULoad()
{
	FILETIME idleTime, kernelTime, userTime;
	return GetSystemTimes(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime) + FileTimeToInt64(userTime)) : -100.0f;
}

float AnalyticsHardware::CPUInfo::CalculateCPULoad(uint64_t p_idleTicks, uint64_t p_totalTicks)
{
	uint64_t totalTicksSinceLastTime = p_totalTicks - m_cpuPreviousTotalTicks;
	uint64_t idleTicksSinceLastTime = p_idleTicks - m_cpuPreviousIdleTicks;

	float ret = 1.0f - ((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime) / totalTicksSinceLastTime : 0);

	m_cpuPreviousTotalTicks = p_totalTicks;
	m_cpuPreviousIdleTicks = p_idleTicks;

	return ret * 100.0f;
}

uint64_t AnalyticsHardware::CPUInfo::FileTimeToInt64(const FILETIME& ft)
{
	return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime);
}

// RAMInfo

void AnalyticsHardware::RAMInfo::Update()
{
	m_statex.dwLength = sizeof(m_statex);
	GlobalMemoryStatusEx(&m_statex);
}

float AnalyticsHardware::RAMInfo::GetUsedRAM()
{
	return GetMaxRAM() - m_statex.ullAvailPhys / 1048576.0f;
}

float AnalyticsHardware::RAMInfo::GetFreeRAM()
{
	return m_statex.ullAvailPhys / 1048576.0f;
}

float AnalyticsHardware::RAMInfo::GetMaxRAM()
{
	return m_statex.ullTotalPhys / 1048576.0f;
}

// GPUInfo

AnalyticsHardware::GPUInfo::GPUInfo()
{
#if _WIN32
	HMODULE hmod = LoadLibraryA("nvapi.dll");
#elif _WIN64
	HMODULE hmod = LoadLibraryA("nvapi64.dll");
#else
	HMODULE hmod = nullptr;
#endif

	/* Handle "Couldn't find nvapi.dll" */
	if (hmod)
	{
		m_NvAPI_QueryInterface = (NvAPI_QueryInterface_t)GetProcAddress(hmod, "nvapi_QueryInterface");
		m_NvAPI_Initialize = (NvAPI_Initialize_t)(*m_NvAPI_QueryInterface)(0x0150E828);
		m_NvAPI_EnumPhysicalGPUs = (NvAPI_EnumPhysicalGPUs_t)(*m_NvAPI_QueryInterface)(0xE5AC921F);
		m_NvAPI_GPU_GetUsages = (NvAPI_GPU_GetUsages_t)(*m_NvAPI_QueryInterface)(0x189A1FDF);

		if (m_NvAPI_Initialize && m_NvAPI_EnumPhysicalGPUs && m_NvAPI_GPU_GetUsages)
		{
			(*m_NvAPI_Initialize)();

			gpuUsages[0] = (136) | 0x10000;

			(*m_NvAPI_EnumPhysicalGPUs)(m_gpuHandles, &m_gpuCount);

			m_NvAPIReady = true;
		}
	}
}

float AnalyticsHardware::GPUInfo::CalculateGPULoad()
{
	if (!m_NvAPIReady) return -100.0f;

	(*m_NvAPI_GPU_GetUsages)(m_gpuHandles[0], gpuUsages);

	return static_cast<float>(gpuUsages[3]);
}

// HardwareInfo

AnalyticsHardware::HardwareInfo::HardwareInfo(double p_timeInterval) :
	timeInterval(p_timeInterval), timer(0.0f), cpuUsage(0.0f), gpuUsage(0.0f)
{

}

AnalyticsHardware::HardwareReport AnalyticsHardware::HardwareInfo::GenerateReport()
{
	return HardwareReport
	{
		cpuUsage,
		gpuUsage,
		ramInfo.GetUsedRAM(),
		ramInfo.GetFreeRAM(),
		ramInfo.GetMaxRAM()
	};
}

void AnalyticsHardware::HardwareInfo::Tick()
{
	currentTime = std::chrono::steady_clock::now();
	elapsedTime = currentTime - previousTime;

	timer += elapsedTime.count();

	if (timer >= timeInterval)
	{
		while (timer >= timeInterval)
			timer -= timeInterval;

		Update();
	}

	previousTime = currentTime;
}

void AnalyticsHardware::HardwareInfo::Update()
{
	cpuUsage = cpuInfo.CalculateCPULoad();
	gpuUsage = gpuInfo.CalculateGPULoad();

	ramInfo.Update();
}


#endif
