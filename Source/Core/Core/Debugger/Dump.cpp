// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <cstdio>

#include "Common/Common.h"
#include "Common/FileUtil.h"

#include "Core/Debugger/Dump.h"

CDump::CDump(const char* _szFilename) :
	m_pData(nullptr)
{
	File::IOFile pStream(_szFilename, "rb");
	if (pStream)
	{
		m_size = (size_t)pStream.GetSize();

		m_pData = new u8[m_size];

		pStream.ReadArray(m_pData, m_size);
	}
}

CDump::~CDump(void)
{
	if (m_pData != nullptr)
	{
		delete[] m_pData;
		m_pData = nullptr;
	}
}

int CDump::GetNumberOfSteps(void)
{
	return (int)(m_size / STRUCTUR_SIZE);
}

u32 CDump::GetGPR(int _step, int _gpr)
{
	u32 offset = _step * STRUCTUR_SIZE;

	if (offset >= m_size)
		return -1;

	return Read32(offset + OFFSET_GPR + (_gpr * 4));
}

u32 CDump::GetPC(int _step)
{
	u32 offset = _step * STRUCTUR_SIZE;

	if (offset >= m_size)
		return -1;

	return Read32(offset + OFFSET_PC);
}

u32 CDump::Read32(u32 _pos)
{
	u32 result = (m_pData[_pos+0] << 24) |
	             (m_pData[_pos+1] << 16) |
	             (m_pData[_pos+2] << 8)  |
	             (m_pData[_pos+3] << 0);

	return result;
}
