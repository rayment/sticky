/*
 * random.c
 * Random number generator suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 17/02/2022
 */

#include "test_common.h"

#define SEED        0xdeadbe3f
#define GENERATIONS 1024
#define MIN         32
#define MAX         64

#define GEN_DATA(var,func)                                     \
	for (i = 0; i < GENERATIONS; ++i)                          \
		*(var+i) = S_random_next_##func();

#define GEN_MAX_DATA(var,func)                                 \
	for (i = 0; i < GENERATIONS; ++i)                          \
		*(var+i) = S_random_range_##func(MIN, MAX);

#define TEST_NEXT(var,func)                                    \
	TEST(                                                      \
		b = S_TRUE;                                            \
		for (i = 0; i < GENERATIONS; ++i)                      \
		{                                                      \
			if (*(var+i) != S_random_next_##func())            \
				b = S_FALSE;                                   \
		}                                                      \
	, b                                                        \
	, "S_random_next_"#func);

#define TEST_RANGE(var,func)                                   \
	TEST(                                                      \
		b = S_TRUE;                                            \
		for (i = 0; i < GENERATIONS; ++i)                      \
		{                                                      \
			if (*(var+i) != S_random_range_##func(MIN, MAX) || \
			    *(var+i) < MIN || *(var+i) > MAX)              \
				b = S_FALSE;                                   \
		}                                                      \
	, b                                                        \
	, "S_random_range_"#func);

int
main(void)
{
	Suint32 i;
	Sbool b;
	Sbool bs[GENERATIONS];
	Schar cs[GENERATIONS];
	Sfloat fs[GENERATIONS];
	Sdouble ds[GENERATIONS];
	Suint8 u8s[GENERATIONS];
	Suint16 u16s[GENERATIONS];
	Suint32 u32s[GENERATIONS];
	Suint64 u64s[GENERATIONS];
	Sint8 i8s[GENERATIONS];
	Sint16 i16s[GENERATIONS];
	Sint32 i32s[GENERATIONS];
	Sint64 i64s[GENERATIONS];

	INIT();

	TEST(
		S_random_set_seed(SEED);
	, S_random_get_seed() == SEED
	, "S_random_set_seed and S_random_get_seed");

	GEN_DATA(bs, bool);
	GEN_DATA(cs, char);
	GEN_DATA(fs, float);
	GEN_DATA(ds, double);
	GEN_DATA(u8s, uint8);
	GEN_DATA(u16s, uint16);
	GEN_DATA(u32s, uint32);
	GEN_DATA(u64s, uint64);
	GEN_DATA(i8s, int8);
	GEN_DATA(i16s, int16);
	GEN_DATA(i32s, int32);
	GEN_DATA(i64s, int64);

	S_random_set_seed(SEED);

	TEST_NEXT(bs, bool);
	TEST_NEXT(cs, char);
	TEST_NEXT(fs, float);
	TEST_NEXT(ds, double);
	TEST_NEXT(u8s, uint8);
	TEST_NEXT(u16s, uint16);
	TEST_NEXT(u32s, uint32);
	TEST_NEXT(u64s, uint64);
	TEST_NEXT(i8s, int8);
	TEST_NEXT(i16s, int16);
	TEST_NEXT(i32s, int32);
	TEST_NEXT(i64s, int64);

	S_random_set_seed(SEED);

	GEN_MAX_DATA(cs, char);
	GEN_MAX_DATA(fs, float);
	GEN_MAX_DATA(ds, double);
	GEN_MAX_DATA(u8s, uint8);
	GEN_MAX_DATA(u16s, uint16);
	GEN_MAX_DATA(u32s, uint32);
	GEN_MAX_DATA(u64s, uint64);
	GEN_MAX_DATA(i8s, int8);
	GEN_MAX_DATA(i16s, int16);
	GEN_MAX_DATA(i32s, int32);
	GEN_MAX_DATA(i64s, int64);

	S_random_set_seed(SEED);

	TEST_RANGE(cs, char);
	TEST_RANGE(fs, float);
	TEST_RANGE(ds, double);
	TEST_RANGE(u8s, uint8);
	TEST_RANGE(u16s, uint16);
	TEST_RANGE(u32s, uint32);
	TEST_RANGE(u64s, uint64);
	TEST_RANGE(i8s, int8);
	TEST_RANGE(i16s, int16);
	TEST_RANGE(i32s, int32);
	TEST_RANGE(i64s, int64);

	FREE();

	return EXIT_SUCCESS;
}

