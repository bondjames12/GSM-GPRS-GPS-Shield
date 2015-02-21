#include "SIM900.h"
#include <SoftwareSerial.h>
//If not used, is better to exclude the HTTP library,
//for RAM saving.
//If your sketch reboots itself proprably you have finished,
//your memory available.
//#include "inetGSM.h"

//If you want to use the Arduino functions to manage SMS, uncomment the lines below.
#include "sms.h"
SMSGSM sms;

//To change pins for Software Serial, use the two lines in GSM.cpp.

//GSM Shield for Arduino
//www.open-electronics.org
//this code is based on the example of Arduino Labs.

//Simple sketch to send and receive SMS.
char number[]="3921234567";
char message[180];
char pos;
char *p;

void setup()
{
  Serial.begin(19200);
  if (gsm.begin(19200))
    Serial.println("\nstatus=READY");
  else 
    Serial.println("\nstatus=IDLE");
};

void loop()
{
	int networkQ = gsm.NetworkCheck();
	Serial.println(networkQ);


  pos = sms.IsSMSPresent(SMS_UNREAD);
  Serial.println((int)pos);
  if ((int)pos > 0 && (int)pos <= 20) {
    Serial.print("New Message, POS=");
    Serial.println((int)pos);
    message[0] = '\0';
    sms.GetSMS((int)pos, number, message, 180);
    p = strstr(message, "testpwd");
    if (p) {
      Serial.println("PSWD OK");
      p = strstr(message, "LEDON");
      if (p) {
        Serial.println("LED ON");
        digitalWrite(13, HIGH);
      }
      else {
        p = strstr(message, "LEDOFF");
        if (p) {
          Serial.println("LED OFF");
          digitalWrite(13, LOW);
        }
      }
    }
    sms.DeleteSMS((int)pos);
  }
  delay(5000);
};

// like std c "strstr" function but not case sensitive
char *strucasestr(char *str1, char *str2)
{
  // local vars  
  char *a, *b;
  // compare str 1 and 2 (not case sensitive)
  while (*str1++) {   
    a = str1;
    b = str2;
    while((*a++ | 32) == (*b++ | 32))
      if(!*b)
        return (str1);
   }
   return 0;
}

// use "AT+CSQ" for retrieve RSSI
// 
// return O if data not available or command fault
int get_RSSI(void)
{
  // local vars
  int g_rssi;  
  int g_ber;
  char* rssistring;
  gsm.getRSSI(rssistring);
  //example response string +CSQ: 13,0
  if (rssistring) {
	  Serial.println("Got RSSI:");
	  Serial.println(rssistring);
    //g_rssi = gsm_modem.parseInt();  
    //g_ber  = gsm_modem.parseInt();
    // rssi "99" -> data not yet available
    //if (g_rssi != 99) 
      // convert to dBm (0..31 -> -113..-51 dbm)
    //  g_rssi = (2 * g_rssi) - 113;
   // else return 0; 
  } else return 0;
  //gsm_modem.find("\r\nOK\r\n");
  return g_rssi;
}

// DEBUG
// see  http://www.nongnu.org/avr-libc/user-manual/malloc.html
void print_RAM_map(void) 
{
  // local var
  char stack = 1;
  // external symbol
  extern char *__data_start;
  extern char *__data_end;
  extern char *__bss_start;
  extern char *__bss_end;
  extern char *__heap_start;
  extern char *__heap_end;
  // sizes compute
  int data_size   = (int)&__data_end - (int)&__data_start;
  int bss_size    = (int)&__bss_end - (int)&__data_end;
  int heap_end    = (int)&stack - (int)&__malloc_margin;
  int heap_size   = heap_end - (int)&__bss_end;
  int stack_size  = RAMEND - (int)&stack + 1;
  int free_memory = (RAMEND - (int)&__data_start + 1) - (data_size + bss_size + heap_size + stack_size);
  // print MAP
  printf_P(PSTR("RAM map\n"));
  printf_P(PSTR("-------\n\n"));  
  printf_P(PSTR("+----------------+  __data_start  = %d\n"), (int)&__data_start);
  printf_P(PSTR("+      data      +\n"));
  printf_P(PSTR("+    variables   +  data_size     = %d\n"), data_size);
  printf_P(PSTR("+   (with init)  +\n"));
  printf_P(PSTR("+----------------+  __data_end    = %d\n"), (int)&__data_end);
  printf_P(PSTR("+----------------+  __bss_start   = %d\n"), (int)&__bss_start);
  printf_P(PSTR("+       bss      +\n"));
  printf_P(PSTR("+    variables   +  bss_size      = %d\n"), bss_size);
  printf_P(PSTR("+    (no init)   +\n"));
  printf_P(PSTR("+----------------+  __bss_end     = %d\n"), (int)&__bss_end);
  printf_P(PSTR("+----------------+  __heap_start  = %d\n"), (int)&__heap_start);
  printf_P(PSTR("+                +\n"));
  printf_P(PSTR("+      heap      +  heap_size     = %d\n"), heap_size);
  printf_P(PSTR("+    (dyn var)   +\n"));
  printf_P(PSTR("+----------------+  heap_end      = %d\n"), heap_end);
  printf_P(PSTR("+                +\n"));
  printf_P(PSTR("+    free mem    +  free          = %d\n"), free_memory);
  printf_P(PSTR("+                +\n"));
  printf_P(PSTR("+----------------+  Current STACK = %d\n"), (int)&stack);
  printf_P(PSTR("+      stack     +\n"));
  printf_P(PSTR("+    (sub arg,   +  stack_size    = %d\n"), stack_size);
  printf_P(PSTR("+     loc var)   +\n"));
  printf_P(PSTR("+----------------+  RAMEND        = %d\n"), RAMEND);  
  printf_P(PSTR("\n\n"));
}
