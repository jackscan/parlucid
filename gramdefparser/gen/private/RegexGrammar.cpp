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

