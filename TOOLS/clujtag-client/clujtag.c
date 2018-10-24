/*
 *  Lib(X)SVF  -  A library for implementing SVF and XSVF JTAG players
 *
 *  Copyright (C) 2009  RIEGL Research ForschungsGmbH
 *  Copyright (C) 2009  Clifford Wolf <clifford@clifford.at>
 *  Copyright (C) 2014  Cluster <clusterrr@clusterrr.com>
 *  
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */
#ifdef __MINGW32__
#define WINDOWS
#endif

#include "libxsvf.h"
#include "defines.h"
#include "jtag_commands.h"

#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <stdint.h>
#include <fcntl.h>
#ifdef WINDOWS
#include <windows.h>
#else
#include <termios.h>
#endif

char comPort[512];
#ifdef WINDOWS
HANDLE portHandle;
#else
int portHandle;
#endif
FILE *f;
int verbose;
int tck_count;
unsigned char tck_data[5096];
int write_buffer_count = 0;
unsigned char write_buffer[ACK_STEP+16];
int byte_counter = 0;
int scan_mode = 0;

#ifdef WINDOWS
static uint8_t read_port()
{
	uint8_t buffer;
	DWORD read = 0;
	if (ReadFile(portHandle, &buffer, 1, &read, NULL) && (read == 1)) return buffer;
	fprintf(stderr, "Read error: %d\r\n", (int)GetLastError());
	exit(1);
}
#else
static uint8_t read_port()
{
	uint8_t data;
	int res, t = 0;
	const int timeout = 5000;
	do
	{
		res = read(portHandle, &data, 1);
		t++;
		if (res == 0) usleep(10);
	}
	while ((res == 0) && (t < timeout));
	if (t == timeout)
	{
		printf("Read timeout\n");
		exit(1);
	}
	if (res == 1) return data;
	perror("Read error");
	exit(1);
}
#endif

void check_result()
{
	uint8_t result = read_port();
	if (verbose >= 4) {
		fprintf(stderr, "[RESULT: %d]\n", result);
	}
	
	if (!result)
	{
		fprintf(stderr, "JTAG error\r\n");
		exit(2);
	}
}

#ifdef WINDOWS
static void flush_data()
{
	if (!write_buffer_count) return;
	DWORD writed = 0;
	if (WriteFile(portHandle, write_buffer, write_buffer_count, &writed, NULL) && (writed == write_buffer_count))
	{
		if (verbose >= 4) {
			fprintf(stderr, "[WRITED: %d]\n", write_buffer_count);
		}
		write_buffer_count = 0;
		return;
	}
	int error = GetLastError();
	fprintf(stderr, "Write error: %d\r\n", error);
	exit(1);
}
#else
static void flush_data()
{
	if (!write_buffer_count) return;
	int res;
	do
	{
	    res  = write(portHandle, write_buffer, write_buffer_count);
	    if (res == -1) usleep(10);
	} while (res == -1 && errno == EAGAIN);
	if (res == -1)
	{
	    perror("Write error");
	    exit(1);
	}
	if (verbose >= 4) {
		fprintf(stderr, "[WRITED: %d]\n", write_buffer_count);
	}
	write_buffer_count = 0;
}
#endif

static void write_port(uint8_t data)
{
	write_buffer[write_buffer_count++] = data;
	byte_counter++;
	if (byte_counter >= ACK_STEP)
	{
		flush_data();
		check_result();
		byte_counter = 0;
	}
}

static int h_setup(struct libxsvf_host *h)
{
	if (verbose >= 1) {
		fprintf(stderr, "[SETUP]\n");
		fflush(stderr);
	}

	if (!strlen(comPort))
	{
		fprintf(stderr, "You '-p' parameter to specify the serial port\n");
		return -1;
	}
#ifdef WINDOWS

	char devicePath[50];
	sprintf(devicePath, "\\\\.\\%s", comPort);

	HANDLE mHandle = CreateFile(devicePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,/* FILE_FLAG_OVERLAPPED*/0, NULL);
	if (mHandle == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Can't open serial port %s\r\n", comPort);
		return -1;
	}

	DCB dcb;
	FillMemory(&dcb, sizeof(dcb), 0);
	if (!GetCommState(mHandle, &dcb))     // get current DCB
	{
		fprintf(stderr, "Can't get serial port settings\r\n");
		return -1;
	}	
	dcb.BaudRate = CBR_256000;			
	if (!SetCommState(mHandle, &dcb))
	{
		fprintf(stderr, "Can't set serial port settings\r\n");
		return -1;
	}
	
	COMMTIMEOUTS timeouts;
	FillMemory(&timeouts, sizeof(timeouts), 0);	
	if (!GetCommTimeouts(mHandle, &timeouts))
	{
		fprintf(stderr, "Can't get serial port timeouts\r\n");
		return -1;
	}
	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutMultiplier = 10000;
	timeouts.WriteTotalTimeoutConstant = 10000;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	if (!SetCommTimeouts(mHandle, &timeouts))
	{
		fprintf(stderr, "Can't set serial port timeouts\r\n");
		return -1;
	}

	portHandle = mHandle;

#else

	int fd;
	struct termios options;
	fd = open(comPort, O_RDWR | O_NOCTTY);
	if (fd == -1)
	{
		perror("Can't open serial port");
		return -1;
	}

	fcntl(fd, F_SETFL, FNDELAY);

	bzero(&options, sizeof(options));
	options.c_cflag = B230400 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	cfsetispeed(&options, B230400);
	cfsetospeed(&options, B230400);
	tcsetattr(fd, TCSANOW, &options);
	tcflush(fd, TCIFLUSH);
	portHandle = fd;
	
#endif

	// Setup JTAG
	int i;
	byte_counter = 0;
	for (i = 0; i < 16; i++)
		write_port(JTAG_SETUP);
	byte_counter = 0;

	return 0;
}

static int h_shutdown(struct libxsvf_host *h)
{
	if (verbose >= 1) {
		fprintf(stderr, "[SHUTDOWN]\n");
		fflush(stderr);
	}
	
	if (portHandle)
	{
		write_port(JTAG_SHUTDOWN);
		flush_data();
		check_result(portHandle);
#ifdef WINDOWS
		CloseHandle(portHandle);
#else
		close(portHandle);
#endif
	}

	return 0;
}

static int h_pulse_tck_multi(struct libxsvf_host *h, unsigned char* data, unsigned int count)
{
	write_port(JTAG_PULSE_TCK_MULTI);
	write_port(count & 0xff);
	write_port((count >> 8) & 0xff);
	int i;
	unsigned char r = 0;
	int last_v = data[0];
	for (i = 0; i < count; i++)
	{
		if (data[i] != last_v || r == 0x7f)
		{
			if (r > 0)
			{
				if (r > 1)
				{
					write_port(r);
					write_port(last_v & 0xff);
				} else {
					write_port((last_v & 0xff) | 0x80);
				}
			}
			r = 0;
			last_v = data[i];
		}
		r++;
	}
	
	if (r > 0)
	{
		if (r > 1)
		{
			write_port(r);
			write_port(last_v & 0xff);
		} else {
			write_port((last_v & 0xff) | 0x80);
		}
	}
			
	if (verbose >= 3) {
		fprintf(stderr, "[MULTI TCK: %d bits]\n", count);
		fprintf(stderr, "[TMS:%d, TDI:%d, TDO:%d]\n", data[i]&1, (data[i]>>1)&1 ? (data[i]>>2)&1 : -1, (data[i]>>3)&1 ? (data[i]>>4)&1 : -1);
	}
	return 0;
}

static void flush_tck(struct libxsvf_host *h)
{
	if (tck_count > 0)
		h_pulse_tck_multi(h, tck_data, tck_count);
	tck_count = 0;
}

static void h_udelay(struct libxsvf_host *h, long usecs, int tms, long num_tck)
{
	flush_tck(h);
	if (verbose >= 2) {
		fprintf(stderr, "[DELAY:%ld, TMS:%d, NUM_TCK:%ld]\n", usecs, tms, num_tck);
	}

	write_port(JTAG_PULSE_TCK_DELAY);
	write_port((tms&1) | ((num_tck > 0) ? 0b10 : 0) | ((usecs > 0) ? 0b100 : 0));
	if (num_tck > 0)
	{
		write_port(num_tck & 0xff);
		write_port((num_tck >> 8) & 0xff);
		write_port((num_tck >> 16) & 0xff);
		write_port((num_tck >> 24) & 0xff);
	}
		
	if (usecs > 0)
	{
		write_port(usecs & 0xff);
		write_port((usecs >> 8) & 0xff);
		write_port((usecs >> 16) & 0xff);
		write_port((usecs >> 24) & 0xff);
	}
}

static int h_getbyte(struct libxsvf_host *h)
{
	return fgetc(f);
}

static int h_pulse_tck(struct libxsvf_host *h, int tms, int tdi, int tdo, int rmask, int sync)
{
	if (verbose >= 3) {
		fprintf(stderr, "[PULSE TCK TMS: %d, TDI: %d, TDO: %d]\n", tms, tdi, tdo);
	}

	uint8_t data = 0;
	if (tms) data |= 1;
	if (tdi) data |= (1<<1);
	if (tdo >= 0)
	{
		data |= (1<<2); // need check!
		data |= (tdo << 3); // must be same!
	}
	
	tck_data[tck_count] = data;
	tck_count++;
	if (tck_count == sizeof(tck_data) || (tdo < 0 && scan_mode))
		flush_tck(h);
	
	if (tdo < 0 && scan_mode)
	{
		write_port(JTAG_TDO_REQUEST);
		flush_data();
		byte_counter = 0;
		int res = read_port();
		if (verbose >= 3) {
			fprintf(stderr, "[TDO STATE: %d]\n", res);
		}
		return res;
	}
	
	return 0;
}

static void h_report_device(struct libxsvf_host *h, unsigned long idcode)
{
	printf("Device found: idcode=0x%08lx, revision=0x%01lx, part=0x%04lx, manufactor=0x%03lx\n", idcode,
			(idcode >> 28) & 0xf, (idcode >> 12) & 0xffff, (idcode >> 1) & 0x7ff);
}


static void h_report_status(struct libxsvf_host *h, const char *message)
{
	if (verbose >= 1) {
		fprintf(stderr, "[STATUS] %s\n", message);
	}
}

static void h_report_error(struct libxsvf_host *h, const char *file, int line, const char *message)
{
	fprintf(stderr, "[%s:%d] %s\n", file, line, message);
}

static void *h_realloc(struct libxsvf_host *h, void *ptr, int size, enum libxsvf_mem which)
{
	if (verbose >= 2) {
		fprintf(stderr, "[REALLOC:%s:%d]\n", libxsvf_mem2str(which), size);
	}
	return realloc(ptr, size);
}

static struct libxsvf_host h = {
	.udelay = h_udelay,
	.setup = h_setup,
	.shutdown = h_shutdown,
	.getbyte = h_getbyte,
	.pulse_tck = h_pulse_tck,
	.pulse_sck = NULL,
	.set_trst = NULL,
	.set_frequency = NULL,
	.report_tapstate = NULL,
	.report_device = h_report_device,
	.report_status = h_report_status,
	.report_error = h_report_error,
	.realloc = h_realloc,
};

const char *progname;

static void copyleft()
{
	static int already_printed = 0;
	if (already_printed)
		return;
	fprintf(stderr, "clujtag\n");
	fprintf(stderr, "Copyright (C) 2009  RIEGL Research ForschungsGmbH\n");
	fprintf(stderr, "Copyright (C) 2009  Clifford Wolf <clifford@clifford.at>\n");
	fprintf(stderr, "Copyright (C) 2015  Cluster <clusterrr@clusterrr.com>\n");
	fprintf(stderr, "Lib(X)SVF is free software licensed under the ISC license.\n");
	already_printed = 1;
}

static void help()
{
	copyleft();
	fprintf(stderr, "\n");
	fprintf(stderr, "Usage: %s [ -v ... ] -p portname { -s svf-file | -x xsvf-file | -c }\n", progname);
	fprintf(stderr, "\n");
	fprintf(stderr, "   -p portname\n");
	fprintf(stderr, "          Use specified serial port for communication with programmer (COMxx for Windows and /dev/ttyxxx for Linux)\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "   -v, -vv, -vvv, -vvvv\n");
	fprintf(stderr, "          Verbose, more verbose and even more verbose\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "   -s svf-file\n");
	fprintf(stderr, "          Play the specified SVF file\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "   -x xsvf-file\n");
	fprintf(stderr, "          Play the specified XSVF file\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "   -c\n");
	fprintf(stderr, "          List devices in JTAG chain\n");
	fprintf(stderr, "\n");
	exit(1);
}

int main(int argc, char **argv)
{
	copyleft();
	
	int rc = 0;
	int gotaction = 0;
	int opt;

	tck_count = 0;
	verbose = 0;
	comPort[0] = 0;
	portHandle = 0;
	char filename[1024];

	progname = argc >= 1 ? argv[0] : "xvsftool";
	while ((opt = getopt(argc, argv, "vp:x:s:c")) != -1)
	{
		switch (opt)
		{
		case 'v':
			verbose++;
			break;
		case 'p':
			strncpy(comPort, optarg, sizeof(comPort)-1);
			comPort[sizeof(comPort)-1] = 0;
			break;
		case 'x':
		case 's':
			gotaction = opt;
			strncpy(filename, optarg, sizeof(filename));
			filename[sizeof(filename)-1] = 0;
			break;
		case 'c':
			gotaction = opt;
			break;
		default:
			help();
			break;
		}
	}
	
	switch (gotaction)
	{
		case 'x':
		case 's':
			if (verbose)
				fprintf(stderr, "Playing %s file `%s'.\n", opt == 's' ? "SVF" : "XSVF", filename);
			if (!strcmp(filename, "-"))
				f = stdin;
			else
				f = fopen(filename, "rb");
			if (f == NULL) {
				fprintf(stderr, "Can't open %s file `%s': %s\n", opt == 's' ? "SVF" : "XSVF", filename, strerror(errno));
			rc = 1;
			}
			if (libxsvf_play(&h, gotaction == 's' ? LIBXSVF_MODE_SVF : LIBXSVF_MODE_XSVF) < 0) {
				fprintf(stderr, "Error while playing %s file `%s'.\n", opt == 's' ? "SVF" : "XSVF", filename);
				rc = 1;
			}
			if (strcmp(filename, "-"))
				fclose(f);
			break;
			
		case 'c':
			scan_mode = 1;
			if (libxsvf_play(&h, LIBXSVF_MODE_SCAN) < 0) {
				fprintf(stderr, "Error while scanning JTAG chain.\n");
				rc = 1;
			}
			break;
			
		default:
			help();
			break;
	}

	if (rc)
	{
		fprintf(stderr, "Finished with errors!\n");
	}

	exit(rc);
}

