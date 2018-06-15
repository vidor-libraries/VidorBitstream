/*
 * This file is part of VidorLib
 *
 *
 * Copyright 2018 Arduino SA (http://www.arduino.cc/)
 */

package main

import (
	"bytes"
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"os/exec"
	"strconv"
	"time"

	serial "github.com/tarm/goserial"
)

func main() {

	var svfBitsreamFilename = flag.String("svf", "vidor_s.svf", "filename of svf produced by Quartus")
	var openocdBasePath = flag.String("openocd", "vidor_s.svf", "OpenOCD base path")
	var portName = flag.String("port", "COM1", "UART port name")

	portNumber := 3335

	flag.Parse()

	fmt.Printf("Starting serial-port to IP converter\n")
	fmt.Printf("Com port: %s\n", *portName)

	serConfig := serial.Config{Name: *portName, Baud: 115200}
	serPort, err := serial.OpenPort(&serConfig)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Can not open serial port: %s\n", err)
		return
	}
	defer serPort.Close()

	listener, err := net.Listen("tcp", ":"+strconv.Itoa(portNumber))
	if err != nil {
		fmt.Fprintf(os.Stderr, "Can not open socket server: %s\n", err)
		return
	}
	defer listener.Close()

	ser2ipBuf := make([]byte, 1024)
	ip2serBuf := make([]byte, 1024)

	serPortReadChan := make(chan readResult)
	serPortReadMore := make(chan bool)
	go readProc(serPort, ser2ipBuf, serPortReadChan, serPortReadMore)

	ipReadChan := make(chan readResult)

	acceptChan := make(chan acceptResult)
	acceptMore := make(chan bool)
	go acceptProc(listener, acceptChan, acceptMore)

	// Things that belong to the current connection
	var currentCon net.Conn = nil
	var currentReadMore chan bool = nil
	var connErr error = nil
	var serialErr error = nil

	go startOpenOCD(*openocdBasePath, *svfBitsreamFilename)

	for {
		select {
		case readResult := <-serPortReadChan:
			if readResult.err != nil {
				serialErr = readResult.err
			} else {
				if currentCon != nil {
					_, connErr = currentCon.Write(ser2ipBuf[0:readResult.bytesRead])
				}
				// Read more
				serPortReadMore <- true
			}
		case readResult := <-ipReadChan:
			if readResult.err != nil {
				// Error reading from ip connection
				connErr = readResult.err
			} else {
				_, serialErr = serPort.Write(ip2serBuf[0:readResult.bytesRead])
				if serialErr == nil {
					// Read more
					currentReadMore <- true
				}
			}
		case acceptResult := <-acceptChan:
			if acceptResult.err != nil {
				fmt.Fprintf(os.Stderr, "Can not accept connection: %s\n", acceptResult.err)
				return
			} else {
				currentCon = acceptResult.conn
				currentReadMore = make(chan bool)
				go readProc(currentCon, ip2serBuf, ipReadChan, currentReadMore)
			}
		}

		if serialErr != nil {
			fmt.Fprintf(os.Stderr, "Error reading from serial port: %s\n", serialErr)
			if currentCon != nil {
				currentCon.Close()
				return
			}
		}

		if currentCon != nil && connErr != nil {
			// Close the connection and accept a new one
			currentCon.Close()
			currentCon = nil
			connErr = nil
			acceptMore <- true
		}
	}
}

func startOpenOCD(openocdBasePath, svfBitsreamFilename string) {
	//-d2 -s /home/martino/.arduino15/packages/arduino/tools/openocd/0.9.0-arduino6-static/share/openocd/scripts/ -c "interface remote_bitbang; remote_bitbang_host localhost; remote_bitbang_port 3335; jtag newtap tb276 tap -expected-id 0x020f20dd -irlen 10; init; scan_chain; svf -tap tb276.tap /tmp/project.svf; shutdown"

	commandLine := []string{openocdBasePath + "bin/openocd", "-d2", "-s " + openocdBasePath + "/share/openocd/scripts/", "-c interface remote_bitbang", "-c remote_bitbang_host localhost", "-c remote_bitbang_port 3335", "-c jtag newtap tb276 tap -expected-id 0x020f20dd -irlen 10", "-c init", "-c scan_chain", "-c svf -tap tb276.tap " + svfBitsreamFilename, "-c shutdown"}

	time.Sleep(1000 * time.Millisecond)

	var stdoutBuf, stderrBuf bytes.Buffer
	cmd := exec.Command(commandLine[0], commandLine[1:]...)

	stdoutIn, _ := cmd.StdoutPipe()
	stderrIn, _ := cmd.StderrPipe()

	var errStdout, errStderr error
	stdout := io.MultiWriter(os.Stdout, &stdoutBuf)
	stderr := io.MultiWriter(os.Stderr, &stderrBuf)
	err := cmd.Start()
	if err != nil {
		log.Fatalf("cmd.Start() failed with '%s'\n", err)
	}

	go func() {
		_, errStdout = io.Copy(stdout, stdoutIn)
	}()

	go func() {
		_, errStderr = io.Copy(stderr, stderrIn)
	}()

	err = cmd.Wait()
	if err != nil {
		log.Fatalf("cmd.Run() failed with %s\n", err)
	}
	if errStdout != nil || errStderr != nil {
		log.Fatal("failed to capture stdout or stderr\n")
	}
	outStr, errStr := string(stdoutBuf.Bytes()), string(stderrBuf.Bytes())
	fmt.Printf("\nout:\n%s\nerr:\n%s\n", outStr, errStr)
}

type readResult struct {
	bytesRead int
	err       error
}

type acceptResult struct {
	conn net.Conn
	err  error
}

// Reads from a reader and returns the results in a channel
// After that reading will be stopped until readMore is signaled to give the
// receiver a chance to work with everything in the buffer before we overwrite it
func readProc(src io.Reader, buf []byte, result chan readResult, readMore chan bool) {
	for {
		n, err := src.Read(buf)
		result <- readResult{bytesRead: n, err: err}

		_, ok := <-readMore
		if !ok {
			return
		}
	}
}

// Accepts connections in the goroutine
// After accepting a single connection accepting will be stopped until acceptMore is signaled
func acceptProc(listener net.Listener, result chan acceptResult, acceptMore chan bool) {
	for {
		conn, err := listener.Accept()
		result <- acceptResult{conn: conn, err: err}

		_, ok := <-acceptMore
		if !ok {
			return
		}
	}
}
