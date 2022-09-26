/*
 * fileio.c
 * File I/O source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 26/09/2022
 */

#include <stdio.h>

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/memory/allocator.h"
#include "sticky/util/fileio.h"

Schar *
S_file_read_all(const Schar *filename,
                Sint64 *len)
{
	FILE *fp;
	Sbool fail;
	Sint64 size;
	Schar *buf;

	fail = 0;
	size = 0;
	buf = NULL;

	if (!(fp = fopen(filename, "rb")))
	{
		perror("fopen");
		_S_SET_ERROR(S_IO_ERROR, "S_file_read_all");
		return NULL; /* file is not yet open, so return immediately */
	}
	if (fseek(fp, 0, SEEK_END) != 0)
	{
		perror("fseek");
		_S_SET_ERROR(S_IO_ERROR, "S_file_read_all");
		fail = 1;
		goto l_call_fclose;
	}
	if ((size = ftell(fp)) == -1L)
	{
		perror("ftell");
		_S_SET_ERROR(S_IO_ERROR, "S_file_read_all");
		fail = 1;
		goto l_call_fclose;
	}
	rewind(fp);
	buf = (Schar *) S_memory_new(size + 1);
	if (fread(buf, size, 1, fp) != 1)
	{
		perror("fread");
		_S_SET_ERROR(S_IO_ERROR, "S_file_read_all");
		fail = 1;
		goto l_call_fclose;
	}
l_call_fclose:
	if (fclose(fp) == EOF)
	{
		perror("fclose");
		_S_SET_ERROR(S_IO_ERROR, "S_file_read_all");
		/* the file should have already been read, so this isn't a true fail */
		/*fail = 1;*/
	}
	if (!fail)
	{
		*len = size;
		return buf;
	}
	return NULL;
}

