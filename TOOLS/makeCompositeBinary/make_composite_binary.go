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

type filesFlag []string

func (h *filesFlag) String() string {
	return fmt.Sprint(*h)
}

func (h *filesFlag) Set(csv string) error {
	var values []string
	values = strings.Split(csv, ",")
	for _, value := range values {
		value = strings.TrimSpace(value)
		*h = append(*h, value)
	}
	return nil
}

func main() {

	var filenames filesFlag
	compositeOffset := -1

	flag.Var(&filenames, "i", "Comma separated list of files to be merged; format is filename:isBitstream:offset")
	var fileType = flag.Int("t", 1, "Filetype to generate (boot=0 , app=1, data=2)")
	var outputFilename = flag.String("o", "../app.ttf", "output filename")

	flag.Parse()

	var outBuffer []byte

	for _, element := range filenames {

		splits := strings.Split(element, ":")
		if len(splits) != 3 {
			panic("Filenames not in the right format!")
		}
		filename := splits[0]
		isBitstream, _ := strconv.Atoi(splits[1])
		offset, _ := strconv.Atoi(splits[2]) // expressed in kilobytes

		if compositeOffset == -1 {
			compositeOffset = offset
		}

		// Pad to offset
		for len(outBuffer) < ((offset - compositeOffset) * 1024) {
			outBuffer = append(outBuffer, 0xFF)
		}

		if isBitstream != 0 {

			ttf, err := ioutil.ReadFile(filename)
			if err != nil {
				panic(err)
			}
			// file format is %u, , so convert it to this format

			arr := strings.Split(string(ttf), ",")

			for _, el := range arr {
				el = strings.TrimSpace(el)
				i, err := strconv.Atoi(el)
				i_b := byte(i)
				if el != "" && err == nil {
					i_b = reverse(i_b)
					outBuffer = append(outBuffer, i_b)
				}
			}
		} else {
			bin, err := ioutil.ReadFile(filename)
			if err != nil {
				panic(err)
			}
			outBuffer = append(outBuffer, bin...)
		}
	}

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

	printInfo(compositeOffset*1024, *fileType, outBuffer, *outputFilename)

}

func printInfo(offset, fileType int, outBuffer []byte, filename string) {
	// Generate info for application, to paste into __attribute__ ((used, section(".fpga_bitstream_signature"))) section

	if fileType != 0 {
		fmt.Printf("NO_BOOTLOADER,\n")
	}

	if fileType == 2 {
		fmt.Printf("NO_APP,\n")
	}

	bs := make([]byte, 4)
	binary.LittleEndian.PutUint32(bs, uint32(offset))
	fmt.Printf("0x%02x, 0x%02x, 0x%02x, 0x%02x,\n", bs[0], bs[1], bs[2], bs[3])

	binary.LittleEndian.PutUint32(bs, uint32(len(outBuffer)))
	fmt.Printf("0x%02x, 0x%02x, 0x%02x, 0x%02x,\n", bs[0], bs[1], bs[2], bs[3])

	f, err := os.Open(filename)
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
	fmt.Printf("0x%02x, 0x%02x, 0x%02x, 0x%02x, \n", bs[0], bs[1], bs[2], bs[3])

	fmt.Printf("0x00, 0x00, 0x00, 0x00, // force flag, change to 1 if needed\n")

	if fileType == 0 {
		fmt.Printf("NO_APP,\n")
	}

	if fileType != 2 {
		fmt.Printf("NO_USER_DATA,\n")
	}

}

func reverse(b uint8) uint8 {
	b = (b&0xF0)>>4 | (b&0x0F)<<4
	b = (b&0xCC)>>2 | (b&0x33)<<2
	b = (b&0xAA)>>1 | (b&0x55)<<1
	return b
}
