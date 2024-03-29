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

#include "../LuaTemplateTokenizer.hpp"
#include "../LuaTemplateTokens.hpp"
#include <cstring>

const LuaTemplateTokenizer::State ERROR_STATE = -1;

struct Transition
{
	const char symbol;
	const LuaTemplateTokenizer::State next;
	const int tokens[2];
};

static const Transition transitions0[29] =
{
	{ '\x80', 1, { 0x340, } },
	{ '\t', 2, { 0x342, } },
	{ '\n', 3, { 0x208, } },
	{ '\v', 1, { 0x340, } },
	{ '\r', 4, { 0x208, } },
	{ '\xe', 1, { 0x340, } },
	{ ' ', 2, { 0x342, } },
	{ '!', 1, { 0x340, } },
	{ '"', 5, { 0x2C0, } },
	{ '#', 1, { 0x340, } },
	{ '$', 5, { 0x40240, } },
	{ '%', 6, { 0x240, } },
	{ '&', 1, { 0x340, } },
	{ '-', 7, { 0x740, } },
	{ '0', 7, { 0x750, } },
	{ '1', 7, { 0x740, } },
	{ ':', 1, { 0x340, } },
	{ '@', 8, { 0x240, } },
	{ 'A', 9, { 0x744, } },
	{ '[', 1, { 0x340, } },
	{ '\\', 10, { 0x740, } },
	{ ']', 1, { 0x340, } },
	{ '_', 11, { 0x744, } },
	{ '`', 1, { 0x340, } },
	{ 'a', 9, { 0x744, } },
	{ '{', 12, { 0x80000140, } },
	{ '|', 1, { 0x340, } },
	{ '}', 13, { 0x00000140, 0x1, } },
	{ '~', 1, { 0x340, } },
};

static const Transition transitions1[18] =
{
	{ '\x80', 1, { 0x340, } },
	{ '\n', 3, { 0x200, } },
	{ '\v', 1, { 0x340, } },
	{ '\r', 3, { 0x200, } },
	{ '\xe', 1, { 0x340, } },
	{ '"', 5, { 0x240, } },
	{ '#', 1, { 0x340, } },
	{ '$', 5, { 0x240, } },
	{ '%', 14, { 0x200, } },
	{ '&', 1, { 0x340, } },
	{ '@', 15, { 0x240, } },
	{ 'A', 1, { 0x340, } },
	{ '\\', 16, { 0x340, } },
	{ ']', 1, { 0x340, } },
	{ '{', 17, { 0x100, } },
	{ '|', 1, { 0x340, } },
	{ '}', 18, { 0x140, } },
	{ '~', 1, { 0x340, } },
};

static const Transition transitions2[21] =
{
	{ '\x80', 1, { 0x340, } },
	{ '\t', 2, { 0x342, } },
	{ '\n', 3, { 0x200, } },
	{ '\v', 1, { 0x340, } },
	{ '\r', 3, { 0x200, } },
	{ '\xe', 1, { 0x340, } },
	{ ' ', 2, { 0x342, } },
	{ '!', 1, { 0x340, } },
	{ '"', 5, { 0x240, } },
	{ '#', 1, { 0x340, } },
	{ '$', 5, { 0x240, } },
	{ '%', 14, { 0x200, } },
	{ '&', 1, { 0x340, } },
	{ '@', 15, { 0x240, } },
	{ 'A', 1, { 0x340, } },
	{ '\\', 16, { 0x340, } },
	{ ']', 1, { 0x340, } },
	{ '{', 17, { 0x100, } },
	{ '|', 1, { 0x340, } },
	{ '}', 18, { 0x140, } },
	{ '~', 1, { 0x340, } },
};

static const Transition transitions3[5] =
{
	{ '\x80', 3, { 0x200, } },
	{ '{', -1, { 0 } },
	{ '|', 3, { 0x200, } },
	{ '}', -1, { 0 } },
	{ '~', 3, { 0x200, } },
};

static const Transition transitions4[7] =
{
	{ '\x80', 3, { 0x200, } },
	{ '\n', 3, { 0x208, } },
	{ '\v', 3, { 0x200, } },
	{ '{', -1, { 0 } },
	{ '|', 3, { 0x200, } },
	{ '}', -1, { 0 } },
	{ '~', 3, { 0x200, } },
};

static const Transition transitions5[11] =
{
	{ '\x80', 5, { 0x240, } },
	{ '\n', 3, { 0x200, } },
	{ '\v', 5, { 0x240, } },
	{ '\r', 3, { 0x200, } },
	{ '\xe', 5, { 0x240, } },
	{ '%', 3, { 0x200, } },
	{ '&', 5, { 0x240, } },
	{ '{', -1, { 0 } },
	{ '|', 5, { 0x240, } },
	{ '}', 19, { 0x40, } },
	{ '~', 5, { 0x240, } },
};

static const Transition transitions6[16] =
{
	{ '\x80', 1, { 0x340, } },
	{ '\n', 3, { 0x200, } },
	{ '\v', 1, { 0x340, } },
	{ '\r', 3, { 0x200, } },
	{ '\xe', 1, { 0x340, } },
	{ '"', 5, { 0x240, } },
	{ '#', 1, { 0x340, } },
	{ '$', 5, { 0x240, } },
	{ '%', 20, { 0x300, } },
	{ '&', 1, { 0x340, } },
	{ '@', 5, { 0x240, } },
	{ 'A', 1, { 0x340, } },
	{ '{', 17, { 0x100, } },
	{ '|', 1, { 0x340, } },
	{ '}', 19, { 0x20040, } },
	{ '~', 1, { 0x340, } },
};

static const Transition transitions7[24] =
{
	{ '\x80', 1, { 0x340, } },
	{ '\n', 3, { 0x200, } },
	{ '\v', 1, { 0x340, } },
	{ '\r', 3, { 0x200, } },
	{ '\xe', 1, { 0x340, } },
	{ '"', 5, { 0x240, } },
	{ '#', 1, { 0x340, } },
	{ '$', 5, { 0x240, } },
	{ '%', 14, { 0x200, } },
	{ '&', 1, { 0x340, } },
	{ '-', 7, { 0x740, } },
	{ ':', 1, { 0x340, } },
	{ '@', 15, { 0x240, } },
	{ 'A', 7, { 0x740, } },
	{ '[', 1, { 0x340, } },
	{ '\\', 10, { 0x740, } },
	{ ']', 1, { 0x340, } },
	{ '_', 7, { 0x740, } },
	{ '`', 1, { 0x340, } },
	{ 'a', 7, { 0x740, } },
	{ '{', 17, { 0x100, } },
	{ '|', 1, { 0x340, } },
	{ '}', 18, { 0x140, } },
	{ '~', 1, { 0x340, } },
};

static const Transition transitions8[16] =
{
	{ '\x80', 1, { 0x340, } },
	{ '\n', 20, { 0x300, } },
	{ '\v', 1, { 0x340, } },
	{ '\r', 20, { 0x300, } },
	{ '\xe', 1, { 0x340, } },
	{ '"', 5, { 0x00000240, 0x20, } },
	{ '#', 1, { 0x340, } },
	{ '%', 20, { 0x300, } },
	{ '&', 1, { 0x340, } },
	{ ':', 5, { 0x00000240, 0x10, } },
	{ ';', 5, { 0x8000240, } },
	{ '<', 1, { 0x340, } },
	{ '{', 21, { 0x20000000, } },
	{ '|', 1, { 0x340, } },
	{ '}', 18, { 0x140, } },
	{ '~', 1, { 0x340, } },
};

static const Transition transitions9[26] =
{
	{ '\x80', 1, { 0x340, } },
	{ '\n', 3, { 0x200, } },
	{ '\v', 1, { 0x340, } },
	{ '\r', 3, { 0x200, } },
	{ '\xe', 1, { 0x340, } },
	{ '"', 5, { 0x240, } },
	{ '#', 1, { 0x340, } },
	{ '$', 5, { 0x240, } },
	{ '%', 14, { 0x200, } },
	{ '&', 1, { 0x340, } },
	{ '-', 7, { 0x740, } },
	{ '0', 11, { 0x744, } },
	{ ':', 22, { 0x340, } },
	{ ';', 1, { 0x340, } },
	{ '@', 15, { 0x240, } },
	{ 'A', 11, { 0x744, } },
	{ '[', 1, { 0x340, } },
	{ '\\', 10, { 0x740, } },
	{ ']', 1, { 0x340, } },
	{ '_', 11, { 0x744, } },
	{ '`', 1, { 0x340, } },
	{ 'a', 11, { 0x744, } },
	{ '{', 17, { 0x100, } },
	{ '|', 1, { 0x340, } },
	{ '}', 18, { 0x140, } },
	{ '~', 1, { 0x340, } },
};

static const Transition transitions10[23] =
{
	{ '\x80', 1, { 0x340, } },
	{ '\n', 3, { 0x200, } },
	{ '\v', 1, { 0x340, } },
	{ '\r', 3, { 0x200, } },
	{ '\xe', 1, { 0x340, } },
	{ '"', 5, { 0x240, } },
	{ '#', 1, { 0x340, } },
	{ '%', 14, { 0x200, } },
	{ '&', 1, { 0x340, } },
	{ '-', 7, { 0x740, } },
	{ ':', 1, { 0x340, } },
	{ '@', 23, { 0x340, } },
	{ 'A', 7, { 0x740, } },
	{ '[', 1, { 0x340, } },
	{ '\\', 10, { 0x740, } },
	{ ']', 1, { 0x340, } },
	{ '_', 7, { 0x740, } },
	{ '`', 1, { 0x340, } },
	{ 'a', 7, { 0x740, } },
	{ '{', 17, { 0x100, } },
	{ '|', 1, { 0x340, } },
	{ '}', 18, { 0x140, } },
	{ '~', 1, { 0x340, } },
};

static const Transition transitions11[25] =
{
	{ '\x80', 1, { 0x340, } },
	{ '\n', 3, { 0x200, } },
	{ '\v', 1, { 0x340, } },
	{ '\r', 3, { 0x200, } },
	{ '\xe', 1, { 0x340, } },
	{ '"', 5, { 0x240, } },
	{ '#', 1, { 0x340, } },
	{ '$', 5, { 0x240, } },
	{ '%', 14, { 0x200, } },
	{ '&', 1, { 0x340, } },
	{ '-', 7, { 0x740, } },
	{ '0', 11, { 0x744, } },
	{ ':', 1, { 0x340, } },
	{ '@', 15, { 0x240, } },
	{ 'A', 11, { 0x744, } },
	{ '[', 1, { 0x340, } },
	{ '\\', 10, { 0x740, } },
	{ ']', 1, { 0x340, } },
	{ '_', 11, { 0x744, } },
	{ '`', 1, { 0x340, } },
	{ 'a', 11, { 0x744, } },
	{ '{', 17, { 0x100, } },
	{ '|', 1, { 0x340, } },
	{ '}', 18, { 0x140, } },
	{ '~', 1, { 0x340, } },
};

static const Transition transitions12[16] =
{
	{ '\x80', 18, { 0x140, } },
	{ '\n', -1, { 0 } },
	{ '\v', 18, { 0x140, } },
	{ '\r', -1, { 0 } },
	{ '\xe', 18, { 0x140, } },
	{ '"', 19, { 0x40, } },
	{ '#', 18, { 0x140, } },
	{ '$', 19, { 0x40, } },
	{ '%', 24, { 0x20, } },
	{ '&', 18, { 0x140, } },
	{ '@', 25, { 0x40, } },
	{ 'A', 18, { 0x140, } },
	{ '\\', 26, { 0x140, } },
	{ ']', 18, { 0x140, } },
	{ '{', 17, { 0x100, } },
	{ '|', 18, { 0x140, } },
};

static const Transition transitions13[17] =
{
	{ '\x80', 18, { 0x140, } },
	{ '\n', -1, { 0 } },
	{ '\v', 18, { 0x140, } },
	{ '\r', -1, { 0 } },
	{ '\xe', 18, { 0x140, } },
	{ '"', 19, { 0x40, } },
	{ '#', 18, { 0x140, } },
	{ '$', 19, { 0x40, } },
	{ '%', 24, { 0 } },
	{ '&', 18, { 0x140, } },
	{ '?', 18, { 0x40000140, } },
	{ '@', 25, { 0x40, } },
	{ 'A', 18, { 0x140, } },
	{ '\\', 26, { 0x140, } },
	{ ']', 18, { 0x140, } },
	{ '{', 17, { 0x100, } },
	{ '|', 18, { 0x140, } },
};

static const Transition transitions14[15] =
{
	{ '\x80', 20, { 0x300, } },
	{ '\n', 3, { 0x200, } },
	{ '\v', 20, { 0x300, } },
	{ '\r', 3, { 0x200, } },
	{ '\xe', 20, { 0x300, } },
	{ '"', 3, { 0x200, } },
	{ '#', 20, { 0x300, } },
	{ '$', 3, { 0x200, } },
	{ '%', 20, { 0x300, } },
	{ '@', 3, { 0x200, } },
	{ 'A', 20, { 0x300, } },
	{ '{', 17, { 0x100, } },
	{ '|', 20, { 0x300, } },
	{ '}', -1, { 0 } },
	{ '~', 20, { 0x300, } },
};

static const Transition transitions15[15] =
{
	{ '\x80', 1, { 0x340, } },
	{ '\n', 20, { 0x300, } },
	{ '\v', 1, { 0x340, } },
	{ '\r', 20, { 0x300, } },
	{ '\xe', 1, { 0x340, } },
	{ '"', 5, { 0x240, } },
	{ '#', 1, { 0x340, } },
	{ '%', 20, { 0x300, } },
	{ '&', 1, { 0x340, } },
	{ ':', 5, { 0x240, } },
	{ '<', 1, { 0x340, } },
	{ '{', -1, { 0 } },
	{ '|', 1, { 0x340, } },
	{ '}', 18, { 0x140, } },
	{ '~', 1, { 0x340, } },
};

static const Transition transitions16[17] =
{
	{ '\x80', 1, { 0x340, } },
	{ '\n', 3, { 0x200, } },
	{ '\v', 1, { 0x340, } },
	{ '\r', 3, { 0x200, } },
	{ '\xe', 1, { 0x340, } },
	{ '"', 5, { 0x240, } },
	{ '#', 1, { 0x340, } },
	{ '%', 14, { 0x200, } },
	{ '&', 1, { 0x340, } },
	{ '@', 23, { 0x340, } },
	{ 'A', 1, { 0x340, } },
	{ '\\', 16, { 0x340, } },
	{ ']', 1, { 0x340, } },
	{ '{', 17, { 0x100, } },
	{ '|', 1, { 0x340, } },
	{ '}', 18, { 0x140, } },
	{ '~', 1, { 0x340, } },
};

static const Transition transitions17[14] =
{
	{ '\x80', 17, { 0x100, } },
	{ '\n', -1, { 0 } },
	{ '\v', 17, { 0x100, } },
	{ '\r', -1, { 0 } },
	{ '\xe', 17, { 0x100, } },
	{ '"', -1, { 0 } },
	{ '#', 17, { 0x100, } },
	{ '$', -1, { 0 } },
	{ '%', 24, { 0 } },
	{ '&', 17, { 0x100, } },
	{ '@', 27, { 0 } },
	{ 'A', 17, { 0x100, } },
	{ '\\', 28, { 0x100, } },
	{ ']', 17, { 0x100, } },
};

static const Transition transitions18[16] =
{
	{ '\x80', 18, { 0x140, } },
	{ '\n', -1, { 0 } },
	{ '\v', 18, { 0x140, } },
	{ '\r', -1, { 0 } },
	{ '\xe', 18, { 0x140, } },
	{ '"', 19, { 0x40, } },
	{ '#', 18, { 0x140, } },
	{ '$', 19, { 0x40, } },
	{ '%', 24, { 0 } },
	{ '&', 18, { 0x140, } },
	{ '@', 25, { 0x40, } },
	{ 'A', 18, { 0x140, } },
	{ '\\', 26, { 0x140, } },
	{ ']', 18, { 0x140, } },
	{ '{', 17, { 0x100, } },
	{ '|', 18, { 0x140, } },
};

static const Transition transitions19[9] =
{
	{ '\x80', 19, { 0x40, } },
	{ '\n', -1, { 0 } },
	{ '\v', 19, { 0x40, } },
	{ '\r', -1, { 0 } },
	{ '\xe', 19, { 0x40, } },
	{ '%', -1, { 0 } },
	{ '&', 19, { 0x40, } },
	{ '{', -1, { 0 } },
	{ '|', 19, { 0x40, } },
};

static const Transition transitions20[18] =
{
	{ '\x80', 20, { 0x300, } },
	{ '\n', 3, { 0x200, } },
	{ '\v', 20, { 0x300, } },
	{ '\r', 3, { 0x200, } },
	{ '\xe', 20, { 0x300, } },
	{ '"', 3, { 0x200, } },
	{ '#', 20, { 0x300, } },
	{ '$', 3, { 0x200, } },
	{ '%', 14, { 0x200, } },
	{ '&', 20, { 0x300, } },
	{ '@', 29, { 0x200, } },
	{ 'A', 20, { 0x300, } },
	{ '\\', 30, { 0x300, } },
	{ ']', 20, { 0x300, } },
	{ '{', 17, { 0x100, } },
	{ '|', 20, { 0x300, } },
	{ '}', 17, { 0x100, } },
	{ '~', 20, { 0x300, } },
};

static const Transition transitions22[20] =
{
	{ '\x80', 1, { 0x340, } },
	{ '\n', 3, { 0x200, } },
	{ '\v', 1, { 0x340, } },
	{ '\r', 3, { 0x200, } },
	{ '\xe', 1, { 0x340, } },
	{ '"', 5, { 0x240, } },
	{ '#', 1, { 0x340, } },
	{ '$', 5, { 0x240, } },
	{ '%', 14, { 0x200, } },
	{ '&', 1, { 0x340, } },
	{ '/', 7, { 0x740, } },
	{ '0', 1, { 0x340, } },
	{ '@', 15, { 0x240, } },
	{ 'A', 1, { 0x340, } },
	{ '\\', 10, { 0x740, } },
	{ ']', 1, { 0x340, } },
	{ '{', 17, { 0x100, } },
	{ '|', 1, { 0x340, } },
	{ '}', 18, { 0x140, } },
	{ '~', 1, { 0x340, } },
};

static const Transition transitions23[17] =
{
	{ '\x80', 1, { 0x340, } },
	{ '\n', 20, { 0x300, } },
	{ '\v', 1, { 0x340, } },
	{ '\r', 20, { 0x300, } },
	{ '\xe', 1, { 0x340, } },
	{ '"', 5, { 0x240, } },
	{ '#', 1, { 0x340, } },
	{ '%', 31, { 0x300, } },
	{ '&', 1, { 0x340, } },
	{ '@', 23, { 0x340, } },
	{ 'A', 1, { 0x340, } },
	{ '\\', 16, { 0x340, } },
	{ ']', 1, { 0x340, } },
	{ '{', 17, { 0x100, } },
	{ '|', 1, { 0x340, } },
	{ '}', 18, { 0x140, } },
	{ '~', 1, { 0x340, } },
};

static const Transition transitions24[13] =
{
	{ '\x80', 17, { 0x100, } },
	{ '\n', -1, { 0 } },
	{ '\v', 17, { 0x100, } },
	{ '\r', -1, { 0 } },
	{ '\xe', 17, { 0x100, } },
	{ '"', -1, { 0 } },
	{ '#', 17, { 0x100, } },
	{ '$', -1, { 0 } },
	{ '%', 17, { 0x100, } },
	{ '@', -1, { 0 } },
	{ 'A', 17, { 0x100, } },
	{ '}', -1, { 0 } },
	{ '~', 17, { 0x100, } },
};

static const Transition transitions25[13] =
{
	{ '\x80', 18, { 0x140, } },
	{ '\n', 17, { 0x100, } },
	{ '\v', 18, { 0x140, } },
	{ '\r', 17, { 0x100, } },
	{ '\xe', 18, { 0x140, } },
	{ '"', 19, { 0x40, } },
	{ '#', 18, { 0x140, } },
	{ '%', 17, { 0x100, } },
	{ '&', 18, { 0x140, } },
	{ ':', 19, { 0x40, } },
	{ '<', 18, { 0x140, } },
	{ '{', -1, { 0 } },
	{ '|', 18, { 0x140, } },
};

static const Transition transitions26[15] =
{
	{ '\x80', 18, { 0x140, } },
	{ '\n', -1, { 0 } },
	{ '\v', 18, { 0x140, } },
	{ '\r', -1, { 0 } },
	{ '\xe', 18, { 0x140, } },
	{ '"', 19, { 0x40, } },
	{ '#', 18, { 0x140, } },
	{ '%', 24, { 0 } },
	{ '&', 18, { 0x140, } },
	{ '@', 32, { 0x140, } },
	{ 'A', 18, { 0x140, } },
	{ '\\', 26, { 0x140, } },
	{ ']', 18, { 0x140, } },
	{ '{', 17, { 0x100, } },
	{ '|', 18, { 0x140, } },
};

static const Transition transitions27[7] =
{
	{ '\x80', 17, { 0x100, } },
	{ '"', -1, { 0 } },
	{ '#', 17, { 0x100, } },
	{ ':', -1, { 0 } },
	{ '<', 17, { 0x100, } },
	{ '{', -1, { 0 } },
	{ '|', 17, { 0x100, } },
};

static const Transition transitions28[13] =
{
	{ '\x80', 17, { 0x100, } },
	{ '\n', -1, { 0 } },
	{ '\v', 17, { 0x100, } },
	{ '\r', -1, { 0 } },
	{ '\xe', 17, { 0x100, } },
	{ '"', -1, { 0 } },
	{ '#', 17, { 0x100, } },
	{ '%', 24, { 0 } },
	{ '&', 17, { 0x100, } },
	{ '@', 33, { 0x100, } },
	{ 'A', 17, { 0x100, } },
	{ '\\', 28, { 0x100, } },
	{ ']', 17, { 0x100, } },
};

static const Transition transitions29[9] =
{
	{ '\x80', 20, { 0x300, } },
	{ '"', 3, { 0x200, } },
	{ '#', 20, { 0x300, } },
	{ ':', 3, { 0x200, } },
	{ '<', 20, { 0x300, } },
	{ '{', -1, { 0 } },
	{ '|', 20, { 0x300, } },
	{ '}', 17, { 0x100, } },
	{ '~', 20, { 0x300, } },
};

static const Transition transitions30[17] =
{
	{ '\x80', 20, { 0x300, } },
	{ '\n', 3, { 0x200, } },
	{ '\v', 20, { 0x300, } },
	{ '\r', 3, { 0x200, } },
	{ '\xe', 20, { 0x300, } },
	{ '"', 3, { 0x200, } },
	{ '#', 20, { 0x300, } },
	{ '%', 14, { 0x200, } },
	{ '&', 20, { 0x300, } },
	{ '@', 34, { 0x300, } },
	{ 'A', 20, { 0x300, } },
	{ '\\', 30, { 0x300, } },
	{ ']', 20, { 0x300, } },
	{ '{', 17, { 0x100, } },
	{ '|', 20, { 0x300, } },
	{ '}', 17, { 0x100, } },
	{ '~', 20, { 0x300, } },
};

static const Transition transitions31[18] =
{
	{ '\x80', 20, { 0x300, } },
	{ '\n', 3, { 0x200, } },
	{ '\v', 20, { 0x300, } },
	{ '\r', 3, { 0x200, } },
	{ '\xe', 20, { 0x300, } },
	{ '"', 3, { 0x200, } },
	{ '#', 20, { 0x300, } },
	{ '$', 3, { 0x200, } },
	{ '%', 31, { 0x300, } },
	{ '&', 20, { 0x300, } },
	{ '@', 29, { 0x200, } },
	{ 'A', 20, { 0x300, } },
	{ '\\', 30, { 0x300, } },
	{ ']', 20, { 0x300, } },
	{ '{', 17, { 0x100, } },
	{ '|', 20, { 0x300, } },
	{ '}', 17, { 0x100, } },
	{ '~', 20, { 0x300, } },
};

static const Transition transitions32[15] =
{
	{ '\x80', 18, { 0x140, } },
	{ '\n', 17, { 0x100, } },
	{ '\v', 18, { 0x140, } },
	{ '\r', 17, { 0x100, } },
	{ '\xe', 18, { 0x140, } },
	{ '"', 19, { 0x40, } },
	{ '#', 18, { 0x140, } },
	{ '%', 35, { 0x100, } },
	{ '&', 18, { 0x140, } },
	{ '@', 32, { 0x140, } },
	{ 'A', 18, { 0x140, } },
	{ '\\', 26, { 0x140, } },
	{ ']', 18, { 0x140, } },
	{ '{', 17, { 0x100, } },
	{ '|', 18, { 0x140, } },
};

static const Transition transitions33[9] =
{
	{ '\x80', 17, { 0x100, } },
	{ '"', -1, { 0 } },
	{ '#', 17, { 0x100, } },
	{ '%', 35, { 0x100, } },
	{ '&', 17, { 0x100, } },
	{ '@', 33, { 0x100, } },
	{ 'A', 17, { 0x100, } },
	{ '\\', 28, { 0x100, } },
	{ ']', 17, { 0x100, } },
};

static const Transition transitions34[13] =
{
	{ '\x80', 20, { 0x300, } },
	{ '"', 3, { 0x200, } },
	{ '#', 20, { 0x300, } },
	{ '%', 31, { 0x300, } },
	{ '&', 20, { 0x300, } },
	{ '@', 34, { 0x300, } },
	{ 'A', 20, { 0x300, } },
	{ '\\', 30, { 0x300, } },
	{ ']', 20, { 0x300, } },
	{ '{', 17, { 0x100, } },
	{ '|', 20, { 0x300, } },
	{ '}', 17, { 0x100, } },
	{ '~', 20, { 0x300, } },
};

static const Transition transitions35[14] =
{
	{ '\x80', 17, { 0x100, } },
	{ '\n', -1, { 0 } },
	{ '\v', 17, { 0x100, } },
	{ '\r', -1, { 0 } },
	{ '\xe', 17, { 0x100, } },
	{ '"', -1, { 0 } },
	{ '#', 17, { 0x100, } },
	{ '$', -1, { 0 } },
	{ '%', 35, { 0x100, } },
	{ '&', 17, { 0x100, } },
	{ '@', 27, { 0 } },
	{ 'A', 17, { 0x100, } },
	{ '\\', 28, { 0x100, } },
	{ ']', 17, { 0x100, } },
};

static const Transition deadEnd = { '\x80', ERROR_STATE, { 0 } };


const struct StateTransitions
{
	const int size;
	const int output[2];
	const Transition* transitions;
}
transitionTable[36] =
{
	{ 29, { 0xE80607FE, 0x31, }, transitions0 },
	{ 18, { 0x340, }, transitions1 },
	{ 21, { 0x342, }, transitions2 },
	{ 5, { 0x200, }, transitions3 },
	{ 7, { 0x208, }, transitions4 },
	{ 11, { 0x240, }, transitions5 },
	{ 16, { 0x20340, }, transitions6 },
	{ 24, { 0x740, }, transitions7 },
	{ 16, { 0x28000340, 0x30, }, transitions8 },
	{ 26, { 0x744, }, transitions9 },
	{ 23, { 0x740, }, transitions10 },
	{ 25, { 0x744, }, transitions11 },
	{ 16, { 0x160, }, transitions12 },
	{ 17, { 0x40000140, }, transitions13 },
	{ 15, { 0x300, }, transitions14 },
	{ 15, { 0x340, }, transitions15 },
	{ 17, { 0x340, }, transitions16 },
	{ 14, { 0x100, }, transitions17 },
	{ 16, { 0x140, }, transitions18 },
	{ 9, { 0x40, }, transitions19 },
	{ 18, { 0x300, }, transitions20 },
	{ 1, { 0 }, &deadEnd },
	{ 20, { 0x740, }, transitions22 },
	{ 17, { 0x340, }, transitions23 },
	{ 13, { 0x100, }, transitions24 },
	{ 13, { 0x140, }, transitions25 },
	{ 15, { 0x140, }, transitions26 },
	{ 7, { 0x100, }, transitions27 },
	{ 13, { 0x100, }, transitions28 },
	{ 9, { 0x300, }, transitions29 },
	{ 17, { 0x300, }, transitions30 },
	{ 18, { 0x300, }, transitions31 },
	{ 15, { 0x140, }, transitions32 },
	{ 9, { 0x100, }, transitions33 },
	{ 13, { 0x300, }, transitions34 },
	{ 14, { 0x100, }, transitions35 },
};


static inline const Transition& getTransition(LuaTemplateTokenizer::State state, char sym, const int context[2])
{
	const StateTransitions& entry = transitionTable[state];

	int anyoutput = 0;
	for (int c = 0; c < 2; ++c)
		anyoutput |= (entry.output[c] & context[c]);

	if (anyoutput)
	{
		int lower = 0;
		int upper = entry.size;
		while (lower < upper - 1)
		{
			int index = (upper + lower) / 2;
			if (sym < entry.transitions[index].symbol) upper = index;
			else lower = index;
		}
		return sym < entry.transitions[lower].symbol ? deadEnd : entry.transitions[lower];
	}
	else return deadEnd;
}

void LuaTemplateTokenizer::begin(unsigned icontext)
{
	const int contexts[][2] =
	{
		{ 0x787B, }, // 0
		{ 0x4078, }, // 1
		{ 0x3A770188, 0x1938, }, // 2
		{ 0x3B96018A, 0x1E30, }, // 3
		{ 0xC0000004, 0x1, }, // 4
		{ 0x00000200, 0x1, }, // 5
		{ 0x00000400, 0x40, }, // 6
	};

	if (icontext < 7)
	{
		this->context[0] = contexts[icontext][0];
		this->context[1] = contexts[icontext][1];
	}
	else
	{
		this->context[0] = 0x0;
		this->context[1] = 0x0;
	}
	this->state = 0;
	this->token = sizeof(int) * 8;
	this->length = 0;
	this->pos = 0;
}

int LuaTemplateTokenizer::end(int& token)
{
	token = this->length > 0 ? this->token : LuaTemplateTokens::TOK_END;
	return this->length;
}

bool LuaTemplateTokenizer::process(const char* symbols, int len)
{
	int i = 0;
	int tok[2] = { 0 };
	int tokenIndex = 2 - 1;
	while(i < len)
	{
		const Transition& trans = getTransition(state, symbols[i++], context);
		state = trans.next;

		if (state < 0) break;

		int anytok = 0;
		for (int c = 0; c <= tokenIndex; ++c)
			anytok |= (tok[c] = (trans.tokens[c] & context[c]));

		if (anytok != 0)
		{
			const int setsize = sizeof(int) * 8;
			int newIndex = 0;
			for (; newIndex < tokenIndex && 0 == tok[newIndex]; ++newIndex)
				;
			if (newIndex != tokenIndex || tok[tokenIndex] != (1 << (token - tokenIndex * setsize)))
			{
				tokenIndex = newIndex;
				token = tokenIndex * setsize;
				for (int ts = tok[tokenIndex]; 0 == (ts & 1); ++token, ts >>= 1)
					;
				// update context to not accept any tokens with lower prio (i.e. greater tokens)
				context[tokenIndex] &= ~(-1 << (1 + token - tokenIndex * setsize));
				for (int c = tokenIndex + 1; c < 2; ++c) context[c] = 0x0;
			}
			length = pos + i;
		}
	}

	pos += len;

	return state >= 0;
}
