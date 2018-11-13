/*
  This sketch shows how to control QR code markers recognition on a MKRVidor4000.
  Connect the camera to the strip header and show it a printed qr code.
  Function vcam.qrrec.readQRCode() will report the coordinates of found markers (if any).
  
  Circuit:
  - MKR Vidor 4000;
  - Raspberry Pi Camera V1 (OV5647) connected to strip header
  - microHDMI to HDMI cable
  - an HDMI capable display

  created 25 July 2018
  by Riccardo Rizzo
*/

#include "VidorGraphics.h"
#include "VidorCamera.h"

VidorCamera vcam;

#define MAXDIM 10

static uint16_t x[QR_PT_DET_NUM], y[QR_PT_DET_NUM];

struct qrPtn {
  uint16_t x[QR_PT_DET_NUM];
  uint16_t y[QR_PT_DET_NUM];
};

static qrPtn qrBufferPtn[MAXDIM];

uint16_t count = 0, last;

void setup() {
  Serial.begin(9600);

  if (!FPGA.begin()) {
    Serial.println("Initialization failed!");
    while (1) {}
  }

  // initialize the video subsystem
  vcam.vgfx.begin();

  /**
    begin() enable the I2C communication and initialize the display for the camera
  */
  if (!vcam.begin()) {
    Serial.println("Camera begin failed");
    while (1) {}
  }

  /**
      qrrec.begin(); enable the QR code recognition
  */
  vcam.qrrec.begin();
  vcam.qrrec.draw();
  Serial.println("Power ON");
}

void loop()  {
  /**
     qrrec.readQRCode(); get, if available, the coordinates of the QR code in the screen
  */
  vcam.qrrec.readQRCode();
  for (int i = 0; i < QR_PT_DET_NUM; i++) {
    if (vcam.qrrec.qr.pt[i].valid) {
      x[i] = (vcam.qrrec.qr.pt[i].xs + vcam.qrrec.qr.pt[i].xe) / 2;
      y[i] = (vcam.qrrec.qr.pt[i].ys + vcam.qrrec.qr.pt[i].ye) / 2;
      vcam.vgfx.Cross(x[i], y[i], 65535);
    }
  }

  last = count % MAXDIM;
  for (int i = 0; i < QR_PT_DET_NUM; i++) {
    vcam.vgfx.Cross(qrBufferPtn[last].x[i], qrBufferPtn[last].y[i], 0, 0);
    qrBufferPtn[last].x[i] = x[i];
    qrBufferPtn[last].y[i] = y[i];
  }
  count++;
}