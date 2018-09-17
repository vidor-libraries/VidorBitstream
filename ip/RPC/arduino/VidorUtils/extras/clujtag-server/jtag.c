#include "Arduino.h"
#include "jtag.h"
#include "jtag_commands.h"

static uint8_t jtag_current_command = 0;
static uint16_t jtag_pos = 0;
static uint32_t jtag_tck_delay_value = 0;
static uint32_t jtag_num_tck = 0;
static uint32_t jtag_usecs = 0;
static uint16_t jtag_multi_count = 0;
static uint16_t jtag_multi_pos = 0;
static uint16_t jtag_multi_multi = 0;

static int byte_counter = 0;
static uint8_t ok = 0;
static uint8_t setup_counter = 0;

static volatile uint32_t *outport, *inport; // assume one port contains all out pins
static uint32_t tckpinmask, tmspinmask, tdipinmask, tdopinmask;

static uint8_t jtag_read(void)
{
  return tdopinmask & *inport ? 1 : 0;
}

static void tck_delay(uint8_t tms, uint32_t num_tck, uint32_t usecs)
{
  if (tms)
    *outport |= tmspinmask;
  else
    *outport &= ~tmspinmask;

  *outport |= tckpinmask;
  int i;
  for (i = 0; i < num_tck; i++)
  {
    *outport &= ~tckpinmask;
    *outport |= tckpinmask;
  }
  delayMicroseconds(usecs);
}

static uint8_t pulse_tck(int tms, int tdi, int tdo)
{
  if (tms)
    *outport |= tmspinmask;
  else
    *outport &= ~tmspinmask;

  if (tdi >= 0)
  {
    if (tdi)
      *outport |= tdipinmask;
    else
      *outport &= ~tdipinmask;
  }

  *outport &= ~tckpinmask;
  *outport |= tckpinmask;

  if (tdo < 0) return 1;
  return (jtag_read() == tdo);
}

int jtag_parse_byte(uint8_t data)
{
  int i, tms, tdi, tdo;
  switch (jtag_current_command)
  {
    case 0:
      jtag_current_command = data;
      jtag_pos = 0;
      break;

    case JTAG_PULSE_TCK_DELAY:
      if (jtag_pos == 0)
      {
        jtag_num_tck = 0;
        jtag_usecs = 0;
        jtag_tck_delay_value = data;
      } else {
        if ((jtag_pos < 4) && (jtag_tck_delay_value & 0b10)) // num_tck
        {
          jtag_num_tck |= (uint32_t)data << (8 * (jtag_pos - 1));
        } else if ((jtag_pos < 4) && !(jtag_tck_delay_value & 0b10)) // usecs
        {
          jtag_usecs |= (uint32_t)data << (8 * (jtag_pos - 1));
        } else {
          jtag_usecs |= (uint32_t)data << (8 * (jtag_pos - 5));
        }
      }
      if ( ((jtag_tck_delay_value & 0b110) == 0)
           || (((((jtag_tck_delay_value & 0b110) == 0b100) || (jtag_tck_delay_value & 0b110) == 0b010)) && jtag_pos == 4)
           || (((jtag_tck_delay_value & 0b110) == 0b110) && jtag_pos == 8)
         )
      {
        tck_delay(jtag_tck_delay_value & 1, jtag_num_tck, jtag_usecs);
        jtag_current_command = 0;
      }
      jtag_pos++;
      break;

    case JTAG_PULSE_TCK_MULTI:
      if (jtag_pos == 0)
      {
        jtag_multi_count = data;
        jtag_multi_pos = 0;
        jtag_multi_multi = 0;
      } else if (jtag_pos == 1)
      {
        jtag_multi_count |= (uint16_t)data << 8;
      } else {
        if (!jtag_multi_multi && !(data & 0x80))
        {
          jtag_multi_multi = data;
        } else {
          if (!jtag_multi_multi) jtag_multi_multi = 1;
          tms = data & 1;
          tdi = (data >> 1) & 1;
          tdo = -1;
          if (data & (1 << 2))
          {
            tdo = (data >> 3) & 1;
          }
          for (i = 0; i < jtag_multi_multi; i++)
          {
            if (!pulse_tck(tms, tdi, tdo)) return 0;
            jtag_multi_pos++;
          }
          jtag_multi_multi = 0;
        }
        if (jtag_multi_pos >= jtag_multi_count)
          jtag_current_command = 0;
      }
      jtag_pos++;
      break;

    default:
      return 0;
  }
  return 1;
}

void jtag_setup(void)
{
  pinMode(TDO, INPUT);
  pinMode(TDI, OUTPUT);
  pinMode(TMS, OUTPUT);
  pinMode(TCK, OUTPUT);
  jtag_current_command = 0;
  outport = portOutputRegister(digitalPinToPort(TCK));
  tckpinmask = digitalPinToBitMask(TCK);
  tmspinmask = digitalPinToBitMask(TMS);
  tdipinmask = digitalPinToBitMask(TDI);
  inport = portInputRegister(digitalPinToPort(TDO));
  tdopinmask = digitalPinToBitMask(TDO);
}

void jtag_shutdown(void)
{
  pinMode(TDO, INPUT);
  pinMode(TDI, INPUT);
  pinMode(TMS, INPUT);
  pinMode(TCK, INPUT);
}

int jtag_execute(uint8_t data)
{
  if (ok)
    LED_ON;
  if (data == JTAG_SETUP)
  {
    setup_counter++;
    if (setup_counter >= 16)
    {
      jtag_setup();
      ok = 1;
      byte_counter = 0;
      return -1;
    }
  } else setup_counter = 0;

  if (jtag_current_command == 0 && data == JTAG_SHUTDOWN)
  {
    jtag_shutdown();
    byte_counter = ACK_STEP;
    LED_OFF;
  } else if (jtag_current_command == 0 && data == JTAG_TDO_REQUEST)
  {
    byte_counter = 0;
    return jtag_read();
  } else if (ok && !jtag_parse_byte(data))
  {
    ok = 0;
    jtag_shutdown();
  }

  byte_counter++;
  if (byte_counter >= ACK_STEP)
  {
    byte_counter = 0;
    return ok;
  }

  LED_OFF;
  return -1;
}
