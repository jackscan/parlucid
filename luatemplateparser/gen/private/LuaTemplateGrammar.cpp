/*
 * Copyright 2004-2012 Mathias Fiedler. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "../LuaTemplateGrammar.hpp"

namespace LuaTemplateGrammar
{

	static const int body0[3] = { 11, 12,  };
	static const int body1[2] = { 12,  };
	static const int body2[5] = { 14, 5, 16, 17,  };
	static const int body3[3] = { 14, 6,  };
	static const int body5[3] = { 20, 3,  };
	static const int body6[2] = { 20,  };
	static const int body7[4] = { 21, 23, 24,  };
	static const int body8[4] = { 29, 9, 30,  };
	static const int body9[2] = { 36,  };
	static const int body10[2] = { 10,  };
	static const int body11[2] = { 1,  };
	static const int body13[4] = { 40, 23, 24,  };
	static const int body14[2] = { 20,  };
	static const int body15[3] = { 20, 3,  };
	static const int body16[3] = { 23, 41,  };
	static const int body18[3] = { 42, 3,  };
	static const int body19[5] = { 42, 20, 43, 3,  };
	static const int body20[5] = { 42, 20, 44, 3,  };
	static const int body21[3] = { 42, 20,  };
	static const int body23[2] = { 7,  };
	static const int body24[2] = { 8,  };
	static const int body25[2] = { 28,  };
	static const int body26[2] = { 1,  };
	static const int body28[4] = { 13, 14, 3,  };
	static const int body29[4] = { 13, 14, 4,  };
	static const int body30[3] = { 18, 2,  };
	static const int body31[6] = { 25, 22, 35, 22, 27,  };
	static const int body32[4] = { 25, 22, 27,  };
	static const int body33[5] = { 18, 31, 9, 32,  };
	static const int body34[4] = { 29, 9, 32,  };
	static const int body35[4] = { 37, 38, 7,  };
	static const int body36[3] = { 20, 43,  };
	static const int body37[3] = { 20, 44,  };
	static const int body38[3] = { 0, 11,  };

	static const Production productions[39] =
	{
		{ 11, 2, body0 },
		{ 11, 1, body1 },
		{ 14, 4, body2 },
		{ 14, 2, body3 },
		{ 14, 0, 0 },
		{ 21, 2, body5 },
		{ 22, 1, body6 },
		{ 22, 3, body7 },
		{ 25, 3, body8 },
		{ 35, 1, body9 },
		{ 38, 1, body10 },
		{ 13, 1, body11 },
		{ 13, 0, 0 },
		{ 16, 3, body13 },
		{ 16, 1, body14 },
		{ 40, 2, body15 },
		{ 23, 2, body16 },
		{ 23, 0, 0 },
		{ 41, 2, body18 },
		{ 41, 4, body19 },
		{ 41, 4, body20 },
		{ 24, 2, body21 },
		{ 20, 0, 0 },
		{ 43, 1, body23 },
		{ 43, 1, body24 },
		{ 44, 1, body25 },
		{ 42, 1, body26 },
		{ 42, 0, 0 },
		{ 12, 3, body28 },
		{ 12, 3, body29 },
		{ 44, 2, body30 },
		{ 28, 5, body31 },
		{ 28, 3, body32 },
		{ 44, 4, body33 },
		{ 44, 3, body34 },
		{ 44, 3, body35 },
		{ 20, 2, body36 },
		{ 20, 2, body37 },
		{ 0, 2, body38 },
	};

	const Production& getProduction(int prod)
	{
		return productions[prod];
	}

}

