/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * exception.c
 * Exception handling test suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 26/09/2022
 */

#include "test_common.h"

struct
thread_ptr_s
{
	Senum in, out;
};

void *
thread_thrower(void *ptr)
{
	struct thread_ptr_s *e;

	e = (struct thread_ptr_s *) ptr;

	e->out = 0;
	S_TRY
	{
		S_THROW(e->in);
	}
	S_CATCH(S_INVALID_OPERATION)
	{
		e->out = S_INVALID_OPERATION;
	}
	S_CATCH(S_INVALID_ENUM)
	{
		e->out = S_INVALID_ENUM;
	}
	S_ENDTRY;
	return ptr;
}

int
main(void)
{
	Sbool except;
	Sthread at, bt;
	Suint8 i;
	struct thread_ptr_s av, bv;

	INIT();

	except = S_FALSE;
	TEST(
		S_TRY
		{
			/* do nothing */
		}
		S_CATCH(S_INVALID_VALUE)
		{
			except = S_TRUE;
			S_error("Incorrect catch!\n");
		}
		S_ENDTRY;
	, !except
	, "S_TRY (single catch, no error)"
	);

	except = S_FALSE;
	TEST(
		S_TRY
		{
			S_THROW(S_INVALID_VALUE);
		}
		S_CATCH(S_INVALID_VALUE)
		{
			except = S_TRUE;
		}
		S_CATCH(S_INVALID_ENUM)
		{
			except = S_TRUE;
			S_error("Incorrect catch!\n");
		}
		S_ENDTRY;
	, except
	, "S_TRY (double catch, single error)");

	TEST(
		av.in = S_INVALID_ENUM;
		bv.in = S_INVALID_OPERATION;
		for (i = 0; i < 100; ++i)
		{
			at = S_thread_new(thread_thrower, &av);
			bt = S_thread_new(thread_thrower, &bv);
			S_thread_join(at);
			S_thread_join(bt);
			if (av.out != S_INVALID_ENUM || bv.out != S_INVALID_OPERATION)
			{
				S_error("Thread storage corruption!");
			}
		}
		SERRNO = S_NO_ERROR;
	, 1
	, "S_TRY (double catch, double error in 2 threads, 100 attempts)");

	FREE();

	return EXIT_SUCCESS;
}

