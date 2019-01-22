/*++

Copyright (C) 2018 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

UnitTest_Wrapper.cpp: Defines Unittests for the Wrapper classes
operations

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"


namespace Lib3MF
{

	TEST(Wrapper, GetLibraryVersion)
	{
		Lib3MF_uint32 nMajor, nMinor, nMicro;
		CLib3MFWrapper::GetLibraryVersion(nMajor, nMinor, nMicro);

		ASSERT_EQ(nMajor, LIB3MF_VERSION_MAJOR);
		ASSERT_EQ(nMinor, LIB3MF_VERSION_MINOR);
		ASSERT_EQ(nMicro, LIB3MF_VERSION_MICRO);
	}
	
	TEST(Wrapper, GetSpecificationVersion)
	{
		Lib3MF_uint32 nMajor, nMinor, nMicro;
		bool bIsSupported;

		CLib3MFWrapper::GetSpecificationVersion("BogusSpecification", bIsSupported, nMajor, nMinor, nMicro);
		ASSERT_FALSE(bIsSupported);

		CLib3MFWrapper::GetSpecificationVersion("http://schemas.microsoft.com/3dmanufacturing/core/2015/02", bIsSupported, nMajor, nMinor, nMicro);
		ASSERT_TRUE(bIsSupported);
		ASSERT_EQ(nMajor, 1);
		ASSERT_EQ(nMinor, 2);
		ASSERT_EQ(nMicro, 3);

		CLib3MFWrapper::GetSpecificationVersion("http://schemas.microsoft.com/3dmanufacturing/material/2015/02", bIsSupported, nMajor, nMinor, nMicro);
		ASSERT_TRUE(bIsSupported);
		ASSERT_EQ(nMajor, 1);
		ASSERT_EQ(nMinor, 0);
		ASSERT_EQ(nMicro, 0);

		CLib3MFWrapper::GetSpecificationVersion("http://schemas.microsoft.com/3dmanufacturing/production/2015/06", bIsSupported, nMajor, nMinor, nMicro);
		ASSERT_TRUE(bIsSupported);
		ASSERT_EQ(nMajor, 1);
		ASSERT_EQ(nMinor, 0);
		ASSERT_EQ(nMicro, 0);

		CLib3MFWrapper::GetSpecificationVersion("http://schemas.microsoft.com/3dmanufacturing/beamlattice/2017/02", bIsSupported, nMajor, nMinor, nMicro);
		ASSERT_TRUE(bIsSupported);
		ASSERT_EQ(nMajor, 1);
		ASSERT_EQ(nMinor, 0);
		ASSERT_EQ(nMicro, 0);

		CLib3MFWrapper::GetSpecificationVersion("http://schemas.microsoft.com/3dmanufacturing/slice/2015/07", bIsSupported, nMajor, nMinor, nMicro);
		ASSERT_TRUE(bIsSupported);
		ASSERT_EQ(nMajor, 1);
		ASSERT_EQ(nMinor, 0);
		ASSERT_EQ(nMicro, 0);
	}

	TEST(Wrapper, CreateModel)
	{
		auto model = CLib3MFWrapper::CreateModel();
	}

	TEST(Wrapper, CheckError)
	{
		CLib3MFWrapper::CheckError(nullptr, 0);
		try {
			CLib3MFWrapper::CheckError(nullptr, 1);
			ASSERT_TRUE(false);
		}
		catch (ELib3MFException &e) {
			ASSERT_EQ(e.getErrorCode(), 1);
		}
	}

	TEST(Wrapper, Color)
	{
		sLib3MFColor c1, c2;
		c1.m_Red = 123;
		c1.m_Green = 0;
		c1.m_Blue = 255;
		c1.m_Alpha = 001;

		Lib3MF_single fR, fG, fB, fA;
		Lib3MF_uint8 nR, nG, nB, nA;

		CLib3MFWrapper::ColorToRGBA(c1, nR, nG, nB, nA);
		ASSERT_EQ(c1.m_Red, nR);
		ASSERT_EQ(c1.m_Green, nG);
		ASSERT_EQ(c1.m_Blue, nB);
		ASSERT_EQ(c1.m_Alpha, nA);

		double EPS = 1e-6;
		CLib3MFWrapper::ColorToFloatRGBA(c1, fR, fG, fB, fA);
		ASSERT_EQ(c1.m_Red, round(fR*255));
		ASSERT_EQ(c1.m_Green, round(fG * 255));
		ASSERT_EQ(c1.m_Blue, round(fB * 255));
		ASSERT_EQ(c1.m_Alpha, round(fA * 255));

		c2 = CLib3MFWrapper::RGBAToColor(nR, nG, nB, nA);
		ASSERT_EQ(c2.m_Red, nR);
		ASSERT_EQ(c2.m_Green, nG);
		ASSERT_EQ(c2.m_Blue, nB);
		ASSERT_EQ(c2.m_Alpha, nA);

		c2 = CLib3MFWrapper::FloatRGBAToColor(fR, fG, fB, fA);
		ASSERT_EQ(c1.m_Red, c2.m_Red);
		ASSERT_EQ(c1.m_Green, c2.m_Green);
		ASSERT_EQ(c1.m_Blue, c2.m_Blue);
		ASSERT_EQ(c1.m_Alpha, c2.m_Alpha);
	}

}