/*
 * This file is part of VidorLib
 *
 *
 * Copyright 2018 Arduino SA (http://www.arduino.cc/)
 */

package main

import (
	"crypto/sha256"
	"encoding/binary"
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"os"
	"strconv"
	"strings"
)

var offset int = 0x80000
var fileType int = 1
var relocateBinaryOffset int = 384 // in kilobytes

func main() {

	var ttfBitsreamFilename = flag.String("ttf", "../vidor_s.ttf", "filename of ttf produced by Quartus")
	var invert = flag.Bool("invert", false, "bitwise inverts ttf")
	var bin = flag.Bool("binary", false, "output a binary file instead than ttf")
	var niosElfFilename = flag.String("elf", "", "standalone nios elf")
	var niosBinFilename = flag.String("bin", "../nios_app.bin", "standalone nios bin (already preprocessed with objdump)")
	var outputFilename = flag.String("out", "../app.", "output filename")

	flag.Parse()

	ttf, err := ioutil.ReadFile(*ttfBitsreamFilename)
	if err != nil {
		panic(err)
	}
	// file format is %u, , so convert it to this format

	arr := strings.Split(string(ttf), ",")

	var outBuffer []byte

	for _, el := range arr {
		el = strings.TrimSpace(el)
		i, err := strconv.Atoi(el)
		i_b := byte(i)
		if el != "" && err == nil {
			if *invert {
				i_b = reverse(i_b)
			}
			outBuffer = append(outBuffer, i_b)
		}
	}

	// Pad to 384KB
	for len(outBuffer) < (relocateBinaryOffset * 1024) {
		outBuffer = append(outBuffer, 0xFF)
	}

	if *niosElfFilename != "" {
		// do all the magic with make_app.sh
	} else {
		niosBin, err := ioutil.ReadFile(*niosBinFilename)
		if err != nil {
			panic(err)
		}
		i := 0
		for i < len(niosBin) {
			//niosBin[i] = reverse(niosBin[i])
			i++
		}
		outBuffer = append(outBuffer, niosBin...)
	}

	if *bin {
		*outputFilename = *outputFilename + "bin"
		ioutil.WriteFile(*outputFilename, outBuffer, 0666)
	} else {
		*outputFilename = *outputFilename + "ttf"
		outFile, _ := os.Create(*outputFilename)
		defer outFile.Close()
		i := 0
		for i < len(outBuffer) {
			if len(outBuffer)-i >= 16 {
				fmt.Fprintf(outFile, "%3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d,", outBuffer[i], outBuffer[i+1], outBuffer[i+2], outBuffer[i+3], outBuffer[i+4], outBuffer[i+5], outBuffer[i+6], outBuffer[i+7], outBuffer[i+8], outBuffer[i+9], outBuffer[i+10], outBuffer[i+11], outBuffer[i+12], outBuffer[i+13], outBuffer[i+14], outBuffer[i+15])
			} else {
				// Last line
				for i < len(outBuffer) {
					fmt.Fprintf(outFile, "%3d, ", outBuffer[i])
					i++
				}
			}
			fmt.Fprintf(outFile, "\n")
			i += 16
		}
		//ioutil.WriteFile(*outputFilename, []byte(outString), 0666)
	}

	// Generate info for application, to paste into __attribute__ ((used, section(".fpga_bitstream_signature"))) section
	bs := make([]byte, 4)
	binary.LittleEndian.PutUint32(bs, uint32(offset))
	fmt.Printf("0x%02x, 0x%02x, 0x%02x, 0x%02x,\n", bs[0], bs[1], bs[2], bs[3])

	binary.LittleEndian.PutUint32(bs, uint32(len(outBuffer)))
	fmt.Printf("0x%02x, 0x%02x, 0x%02x, 0x%02x,\n", bs[0], bs[1], bs[2], bs[3])

	f, err := os.Open(*outputFilename)
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()

	h := sha256.New()
	if _, err := io.Copy(h, f); err != nil {
		log.Fatal(err)
	}

	shasum := h.Sum(nil)
	i := 0

	for i < len(shasum) {
		fmt.Printf("0x%02x, ", shasum[i])
		i++
	}
	fmt.Println("")

	binary.LittleEndian.PutUint32(bs, uint32(fileType))
	fmt.Printf("0x%02x, 0x%02x, 0x%02x, 0x%02x,\n", bs[0], bs[1], bs[2], bs[3])
}

func reverse(b uint8) uint8 {
	b = (b&0xF0)>>4 | (b&0x0F)<<4
	b = (b&0xCC)>>2 | (b&0x33)<<2
	b = (b&0xAA)>>1 | (b&0x55)<<1
	return b
}
