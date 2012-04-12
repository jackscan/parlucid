#include "../GrammarGrammar.hpp"

namespace GrammarGrammar
{

	static const int body0[3] = { 6, 7,  };
	static const int body1[2] = { 7,  };
	static const int body2[7] = { 5, 8, 9, 10, 11, 12,  };
	static const int body3[7] = { 13, 5, 14, 8, 4, 12,  };
	static const int body5[3] = { 10, 15,  };
	static const int body6[6] = { 5, 8, 16, 17, 12,  };
	static const int body7[6] = { 5, 8, 19, 17, 12,  };
	static const int body8[6] = { 19, 8, 16, 17, 12,  };
	static const int body9[3] = { 20, 12,  };
	static const int body10[4] = { 5, 21, 3,  };
	static const int body11[4] = { 5, 21, 4,  };
	static const int body12[4] = { 16, 22, 23,  };
	static const int body13[2] = { 23,  };
	static const int body14[3] = { 23, 24,  };
	static const int body15[2] = { 24,  };
	static const int body16[2] = { 26,  };
	static const int body17[4] = { 27, 16, 28,  };
	static const int body18[4] = { 9, 16, 11,  };
	static const int body19[4] = { 29, 16, 30,  };
	static const int body20[2] = { 5,  };
	static const int body21[2] = { 3,  };
	static const int body22[2] = { 4,  };
	static const int body23[2] = { 31,  };
	static const int body25[3] = { 32, 33,  };
	static const int body26[2] = { 34,  };
	static const int body27[4] = { 33, 35, 34,  };
	static const int body28[4] = { 5, 29, 30,  };
	static const int body29[5] = { 5, 29, 36, 30,  };
	static const int body30[6] = { 37, 21, 5, 29, 30,  };
	static const int body31[7] = { 37, 21, 5, 29, 36, 30,  };
	static const int body32[4] = { 37, 21, 38,  };
	static const int body33[2] = { 38,  };
	static const int body34[4] = { 36, 35, 38,  };
	static const int body35[2] = { 38,  };
	static const int body36[4] = { 38, 39, 5,  };
	static const int body37[2] = { 5,  };
	static const int body38[2] = { 2,  };
	static const int body39[2] = { 1,  };
	static const int body40[3] = { 0, 6,  };

	static const Production productions[41] =
	{
		{ 6, 2, body0 },
		{ 6, 1, body1 },
		{ 7, 6, body2 },
		{ 7, 6, body3 },
		{ 10, 0, 0 },
		{ 10, 2, body5 },
		{ 15, 5, body6 },
		{ 15, 5, body7 },
		{ 15, 5, body8 },
		{ 15, 2, body9 },
		{ 20, 3, body10 },
		{ 20, 3, body11 },
		{ 16, 3, body12 },
		{ 16, 1, body13 },
		{ 23, 2, body14 },
		{ 23, 1, body15 },
		{ 24, 1, body16 },
		{ 24, 3, body17 },
		{ 24, 3, body18 },
		{ 24, 3, body19 },
		{ 26, 1, body20 },
		{ 26, 1, body21 },
		{ 26, 1, body22 },
		{ 26, 1, body23 },
		{ 17, 0, 0 },
		{ 17, 2, body25 },
		{ 33, 1, body26 },
		{ 33, 3, body27 },
		{ 34, 3, body28 },
		{ 34, 4, body29 },
		{ 34, 5, body30 },
		{ 34, 6, body31 },
		{ 34, 3, body32 },
		{ 37, 1, body33 },
		{ 36, 3, body34 },
		{ 36, 1, body35 },
		{ 38, 3, body36 },
		{ 38, 1, body37 },
		{ 0, 1, body38 },
		{ 0, 1, body39 },
		{ 0, 2, body40 },
	};

	const Production& getProduction(int prod)
	{
		return productions[prod];
	}

}

