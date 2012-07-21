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

#include "../RegexGrammar.hpp"

namespace RegexGrammar
{

	static const int body0[2] = { 2,  };
	static const int body1[4] = { 2, 4, 2,  };
	static const int body2[4] = { 2, 5, 6,  };
	static const int body3[2] = { 6,  };
	static const int body4[3] = { 6, 7,  };
	static const int body5[2] = { 7,  };
	static const int body6[4] = { 8, 2, 9,  };
	static const int body7[3] = { 7, 10,  };
	static const int body8[3] = { 7, 11,  };
	static const int body9[3] = { 7, 12,  };
	static const int body10[2] = { 13,  };
	static const int body11[4] = { 14, 15, 16,  };
	static const int body12[5] = { 14, 17, 15, 16,  };
	static const int body13[2] = { 13,  };
	static const int body14[4] = { 18, 4, 13,  };
	static const int body15[3] = { 15, 13,  };
	static const int body16[5] = { 15, 18, 4, 13,  };
	static const int body17[2] = { 13,  };
	static const int body18[2] = { 1,  };
	static const int body19[3] = { 20, 19,  };
	static const int body20[3] = { 0, 3,  };

	static const Production productions[21] =
	{
		{ 3, 1, body0 },
		{ 3, 3, body1 },
		{ 2, 3, body2 },
		{ 2, 1, body3 },
		{ 6, 2, body4 },
		{ 6, 1, body5 },
		{ 7, 3, body6 },
		{ 7, 2, body7 },
		{ 7, 2, body8 },
		{ 7, 2, body9 },
		{ 7, 1, body10 },
		{ 7, 3, body11 },
		{ 7, 4, body12 },
		{ 15, 1, body13 },
		{ 15, 3, body14 },
		{ 15, 2, body15 },
		{ 15, 4, body16 },
		{ 18, 1, body17 },
		{ 13, 1, body18 },
		{ 13, 2, body19 },
		{ 0, 2, body20 },
	};

	const Production& getProduction(int prod)
	{
		return productions[prod];
	}

}

