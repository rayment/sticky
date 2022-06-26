/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * tcp_single_block.c
 * Single blocking client/server TCP net test suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 26/06/2022
 */

#include "test_common.h"

#define BUFLEN      64
#define PORT        9041
#define CLI_TIMEOUT 5000  /* millis */
#define SRV_TIMEOUT 10000 /* millis */

struct
thread_data_s
{
	Senum family;
	Sbool response;
	Sbool last;
};

void *
func_cli(void *data)
{
	struct thread_data_s *cli_data;
	char buf[BUFLEN];
	Ssocket *socket;
	Sssize_t rb;

	cli_data = (struct thread_data_s *) data;

	/* let the server prepare itself */
	S_thread_msleep(CLI_TIMEOUT / 2);

	/* attempt connect */
	TEST(
		if (cli_data->family == S_FAMILY_IPV4)
			socket = S_tcp_connect(cli_data->family, "127.0.0.1", PORT, S_TRUE);
		else
			socket = S_tcp_connect(cli_data->family, "::1", PORT, S_TRUE);
	, socket
	, "cli: S_tcp_connect");

	/* attempt receive */
	TEST(
		rb = S_tcp_recv(socket, buf, BUFLEN);
	, S_tcp_get_state(socket) == S_SOCK_READY &&
	  rb == 3 && buf[0] == 'S' && buf[1] == 'R' && buf[2] == 'V'
	, "cli: S_tcp_recv");

	/* send response */
	TEST(
		S_tcp_send(socket, "CLI", 3);
	, 1
	, "cli: S_tcp_send");

	if (cli_data->last)
	{
		/* attempt to naively receive */
		TEST(
			rb = S_tcp_recv(socket, buf, BUFLEN);
		, rb == 0 && S_tcp_get_state(socket) == S_SOCK_DIED
		, "cli: S_tcp_get_state");
	}

	/* close socket */
	TEST(
		S_tcp_close(socket);
	, SERRNO == S_NO_ERROR
	, "cli: S_tcp_close");

	cli_data->response = S_TRUE;
	return data;
}

void *
func_srv(void *data)
{
	struct thread_data_s *srv_data;
	char buf[BUFLEN];
	Ssocket *socket, *client;
	Sssize_t rb;

	srv_data = (struct thread_data_s *) data;

	/* bind to port */
	TEST(
		socket = S_tcp_bind(srv_data->family, PORT, S_TRUE);
	, socket
	, "srv: S_tcp_bind");

	/* listen */
	TEST(
		S_tcp_listen(socket, 1);
	, SERRNO == S_NO_ERROR
	, "srv: S_tcp_listen");

	/* accept connection */
	TEST(
		client = S_tcp_accept(socket);
	, client
	, "srv: S_tcp_accept");

	/* send welcome message */
	TEST(
		S_tcp_send(client, "SRV", 3);
	, 1
	, "src: S_tcp_send");

	/* receive response */
	TEST(
		rb = S_tcp_recv(client, buf, BUFLEN);
	, S_tcp_get_state(client) == S_SOCK_READY &&
	  rb == 3 && buf[0] == 'C' && buf[1] == 'L' && buf[2] == 'I'
	, "src: S_tcp_recv");

	if (srv_data->last)
	{
		/* attempt to naively receive */
		TEST(
			rb = S_tcp_recv(client, buf, BUFLEN);
		, rb == 0 && S_tcp_get_state(client) == S_SOCK_DIED
		, "src: S_tcp_get_state");
	}

	/* close sockets */
	TEST(
		S_tcp_close(client);
		S_tcp_close(socket);
	, SERRNO == S_NO_ERROR
	, "srv: S_tcp_close");

	srv_data->response = S_TRUE;
	return data;
}

int
run(Senum family)
{
	Sthread cli_thread, srv_thread;
	struct thread_data_s cli_data, srv_data;

	if (family != S_FAMILY_IPV4 && family != S_FAMILY_IPV6)
		return EXIT_FAILURE;

	cli_data.family = family;
	srv_data.family = family;

	/* server closes and client detects closure */
	cli_data.last = S_TRUE;
	srv_data.last = S_FALSE;
	cli_thread = S_thread_new(&func_cli, &cli_data);
	srv_thread = S_thread_new(&func_srv, &srv_data);

	S_thread_join(cli_thread);
	S_thread_join(srv_thread);

	/* client closes and server detects closure */
	cli_data.last = S_FALSE;
	srv_data.last = S_TRUE;
	cli_thread = S_thread_new(&func_cli, &cli_data);
	srv_thread = S_thread_new(&func_srv, &srv_data);

	S_thread_join(cli_thread);
	S_thread_join(srv_thread);

	if (cli_data.response && srv_data.response)
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

int
main(void)
{
	INIT();

	if (run(S_FAMILY_IPV4) != EXIT_SUCCESS)
		return EXIT_FAILURE;
	if (run(S_FAMILY_IPV6) != EXIT_SUCCESS)
		return EXIT_FAILURE;

	FREE();

	return EXIT_SUCCESS;
}

