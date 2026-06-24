{\rtf1\ansi\ansicpg1254\cocoartf2870
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fmodern\fcharset0 CourierNewPSMT;\f1\froman\fcharset0 TimesNewRomanPSMT;}
{\colortbl;\red255\green255\blue255;\red0\green0\blue0;}
{\*\expandedcolortbl;;\cssrgb\c0\c0\c0;}
\paperw11900\paperh16840\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\deftab720
\pard\pardeftab720\sl177\qj\partightenfactor0

\f0\fs21\fsmilli10667 \cf2 \expnd0\expndtw0\kerning0
#include <Wire.h>
\f1\fs32 \

\f0\fs21\fsmilli10667 #include <LiquidCrystal_I2C.h>
\f1\fs32 \

\f0\fs21\fsmilli10667 #include <Keypad.h>
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 LiquidCrystal_I2C lcd(0x27, 16, 2);
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 // pinler
\f1\fs32 \

\f0\fs21\fsmilli10667 const int BUZZER = 10;
\f1\fs32 \

\f0\fs21\fsmilli10667 const int LED_YESIL = 11;
\f1\fs32 \

\f0\fs21\fsmilli10667 const int LED_KIRMIZI = 12;
\f1\fs32 \

\f0\fs21\fsmilli10667 const int TRIG = A1;
\f1\fs32 \

\f0\fs21\fsmilli10667 const int ECHO = A2;
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 // keypad
\f1\fs32 \

\f0\fs21\fsmilli10667 const byte SATIR = 4;
\f1\fs32 \

\f0\fs21\fsmilli10667 const byte SUTUN = 4;
\f1\fs32 \

\f0\fs21\fsmilli10667 char tuslar[SATIR][SUTUN] = \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \{'1','2','3','A'\},
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \{'4','5','6','B'\},
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \{'7','8','9','C'\},
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \{'*','0','#','D'\}
\f1\fs32 \

\f0\fs21\fsmilli10667 \};
\f1\fs32 \

\f0\fs21\fsmilli10667 byte satirPin[SATIR] = \{9, 8, 7, 6\};
\f1\fs32 \

\f0\fs21\fsmilli10667 byte sutunPin[SUTUN] = \{5, 4, 3, 2\};
\f1\fs32 \

\f0\fs21\fsmilli10667 Keypad keypad = Keypad(makeKeymap(tuslar), satirPin, sutunPin, SATIR, SUTUN);
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 // sifreler
\f1\fs32 \

\f0\fs21\fsmilli10667 String dogruSifre = "1357";
\f1\fs32 \

\f0\fs21\fsmilli10667 String sahteSifre = "0000";
\f1\fs32 \

\f0\fs21\fsmilli10667 int sifreUzunlugu = 4;
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 // oyun degiskenleri
\f1\fs32 \

\f0\fs21\fsmilli10667 String girilen = "";
\f1\fs32 \

\f0\fs21\fsmilli10667 int kalanSure = 120;
\f1\fs32 \

\f0\fs21\fsmilli10667 unsigned long sonTik = 0;
\f1\fs32 \

\f0\fs21\fsmilli10667 bool oyunBitti = false;
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 // admin modu
\f1\fs32 \

\f0\fs21\fsmilli10667 int dSayaci = 0;
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 // mesafe sensoru ipucu
\f1\fs32 \

\f0\fs21\fsmilli10667 int acilanHane = 1;
\f1\fs32 \

\f0\fs21\fsmilli10667 unsigned long sonIpucu = 0;
\f1\fs32 \

\f0\fs21\fsmilli10667 const unsigned long IPUCU_BEKLEME = 15000;
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 void setup() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 Serial.begin(9600);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 randomSeed(analogRead(A0));
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 pinMode(BUZZER, OUTPUT);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 pinMode(LED_YESIL, OUTPUT);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 pinMode(LED_KIRMIZI, OUTPUT);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 pinMode(TRIG, OUTPUT);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 pinMode(ECHO, INPUT);
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 lcd.init();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.backlight();
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 zorlukSec();
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 void loop() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 if (oyunBitti) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 char tus = keypad.getKey();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 if (tus == '*') \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 oyunuSifirla();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 \}
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 return;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 geriSayim();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 tusOku();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 mesafeIpucu();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 durumLed();
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 // secilen moda gore kurala uygun sifre uretir
\f1\fs32 \

\f0\fs21\fsmilli10667 String sifreUret(int uzunluk) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 String sifre = "";
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 int oncekiRakam = 0;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 bool serbest = false;
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 for (int i = 0; i < uzunluk; i++) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 int rakam;
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 if (serbest) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 int tekler[] = \{1, 3, 5, 7, 9\};
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 rakam = tekler[random(0, 5)];
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 serbest = false;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 \} else \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 int kalanHane = uzunluk - i - 1;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 int ustSinir = 9 - kalanHane;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 if (ustSinir < oncekiRakam + 1) ustSinir = oncekiRakam + 1;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 if (ustSinir > 9) ustSinir = 9;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 rakam = random(oncekiRakam + 1, ustSinir + 1);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 sifre += String(rakam);
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 if (rakam == 9) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 serbest = true;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 oncekiRakam = 0;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 \} else \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 oncekiRakam = rakam;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 \}
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 return sifre;
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 void zorlukSec() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.clear();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.setCursor(0, 0);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print("Zorluk sec:");
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 String mesaj = "A=Kolay B=Orta C=Zor\'a0\'a0\'a0 ";
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 int scrollPos = 0;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 unsigned long sonKayma = 0;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 char secim = 0;
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 while (secim != 'A' && secim != 'B' && secim != 'C') \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 secim = keypad.getKey();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 if (millis() - sonKayma >= 300) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 sonKayma = millis();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 lcd.setCursor(0, 1);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 String gosterilecek = mesaj.substring(scrollPos) + mesaj.substring(0, scrollPos);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 lcd.print(gosterilecek.substring(0, 16));
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 scrollPos++;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 if (scrollPos >= mesaj.length()) scrollPos = 0;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 \}
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 if (secim == 'A') \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 kalanSure = 120;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 sifreUzunlugu = 4;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 dogruSifre = sifreUret(4);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \} else if (secim == 'B') \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 kalanSure = 90;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 sifreUzunlugu = 5;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 dogruSifre = sifreUret(5);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \} else \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 kalanSure = 60;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 sifreUzunlugu = 6;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 dogruSifre = sifreUret(6);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 bilgilendirmeGoster();
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 acilanHane = 1;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 sonIpucu = 0;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 dSayaci = 0;
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 ipucuGoster(1);
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 lcd.clear();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print("Basliyor...");
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 delay(1000);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 sonTik = millis();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.clear();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 ekraniYenile();
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 void bilgilendirmeGoster() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 String mesaj = "Sifrenin her basamagi bir oncekinden buyuk olmali. Eger bir basamak 9 gelirse sonraki basamak tek sayi olmali.";
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 while (true) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 kaydir(mesaj);
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 lcd.clear();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 lcd.setCursor(0, 0);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 lcd.print("Tekrar oku : 1");
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 lcd.setCursor(0, 1);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 lcd.print("Oyuna basla: 2");
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 char s = 0;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 while (s != '1' && s != '2') \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 s = keypad.getKey();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 if (s == '2') break;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \}
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 // yaziyi soldan saga kaydirir
\f1\fs32 \

\f0\fs21\fsmilli10667 void kaydir(String mesaj) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 String full = "\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0 " + mesaj + "\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0 ";
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.clear();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.setCursor(0, 0);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print("BILGILENDIRME");
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 for (int pos = 0; pos + 16 <= (int)full.length(); pos++) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 lcd.setCursor(0, 1);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 lcd.print(full.substring(pos, pos + 16));
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 delay(250);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \}
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 void ipucuGoster(int haneNo) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 int haneIndex = haneNo - 1;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.clear();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.setCursor(0, 0);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print("Ipucu");
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.setCursor(0, 1);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print(haneNo);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print(". hane : ");
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print(dogruSifre.charAt(haneIndex));
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 delay(5000);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.clear();
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 // 5 cm den yakin algilarsa siradaki haneyi acar
\f1\fs32 \

\f0\fs21\fsmilli10667 void mesafeIpucu() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 if (acilanHane >= (int)dogruSifre.length()) return;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 if (sonIpucu != 0 && (millis() - sonIpucu < IPUCU_BEKLEME)) return;
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 long mesafe = mesafeOlc();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 if (mesafe > 0 && mesafe < 5) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 acilanHane++;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 sonIpucu = millis();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 tone(BUZZER, 1200, 150);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 ipucuGoster(acilanHane);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 ekraniYenile();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \}
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 long mesafeOlc() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 digitalWrite(TRIG, LOW);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 delayMicroseconds(2);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 digitalWrite(TRIG, HIGH);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 delayMicroseconds(10);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 digitalWrite(TRIG, LOW);
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 long sure = pulseIn(ECHO, HIGH, 30000);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 if (sure == 0) return -1;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 return sure * 0.034 / 2;
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 void geriSayim() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 if (millis() - sonTik >= 1000) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 sonTik = millis();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 kalanSure--;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 tone(BUZZER, 1000, 50);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 ekraniYenile();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 if (kalanSure <= 0) patla();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \}
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 void ekraniYenile() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.setCursor(0, 0);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print("Sure: ");
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print(kalanSure);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print("s\'a0\'a0 ");
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.setCursor(0, 1);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print("Kod: ");
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print(girilen);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print("\'a0\'a0\'a0\'a0\'a0\'a0\'a0 ");
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 void tusOku() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 char tus = keypad.getKey();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 if (!tus) return;
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 // 4 kez D ile admin modu
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 if (tus == 'D') \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 dSayaci++;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 if (dSayaci >= 4) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 dSayaci = 0;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 adminModu();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 return;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 \}
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 return;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \} else \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 dSayaci = 0;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 if (tus == '#') \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 if (girilen == dogruSifre) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 defuse();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 \} else if (girilen == sahteSifre) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 sahteSifreCezasi();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 \} else \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0\'a0\'a0 yanlisSifre();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 \}
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 girilen = "";
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \}
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 else if (tus == '*') \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 girilen = "";
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 ekraniYenile();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \}
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 else \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 if ((int)girilen.length() < sifreUzunlugu) girilen += tus;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 tone(BUZZER, 1500, 30);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 ekraniYenile();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \}
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 void durumLed() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 if (kalanSure <= 5 && kalanSure > 0) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 digitalWrite(LED_KIRMIZI, (millis() / 200) % 2);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \} else \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 digitalWrite(LED_KIRMIZI, LOW);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \}
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 void defuse() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 oyunBitti = true;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 digitalWrite(LED_KIRMIZI, LOW);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 digitalWrite(LED_YESIL, HIGH);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.clear();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print("BOMBA IMHA EDILDI!");
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 tone(BUZZER, 2000);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 delay(2000);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 noTone(BUZZER);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 delay(200);
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 int notalar[] = \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 294, 294, 294, 392, 587, 523, 494, 440, 784, 587,
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 523, 494, 440, 784, 587, 523, 494, 523, 440
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \};
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 int sureler[] = \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 150, 150, 150, 900, 900, 150, 150, 150, 900, 450,
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 150, 150, 150, 900, 450, 150, 150, 150, 900
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \};
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 int notaSayisi = sizeof(notalar) / sizeof(notalar[0]);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 for (int i = 0; i < notaSayisi; i++) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 tone(BUZZER, notalar[i]);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 delay(sureler[i]);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 noTone(BUZZER);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 delay(50);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 digitalWrite(LED_YESIL, LOW);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 oyunSonuMesaji();
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 void patla() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 oyunBitti = true;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 digitalWrite(LED_YESIL, LOW);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 digitalWrite(LED_KIRMIZI, HIGH);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.clear();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print("BOOM! PATLADI");
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 for (int i = 0; i < 5; i++) \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 tone(BUZZER, 400, 200);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0\'a0\'a0 delay(250);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 \'a0 delay(3800);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 digitalWrite(LED_KIRMIZI, LOW);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 oyunSonuMesaji();
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 void oyunSonuMesaji() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 delay(4000);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.clear();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.setCursor(0, 0);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print("Oyun bitti");
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.setCursor(0, 1);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print("Yeniden baslat:*");
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 void yanlisSifre() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 kalanSure -= 5;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 digitalWrite(LED_KIRMIZI, HIGH);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.clear();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print("YANLIS! -5sn");
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 tone(BUZZER, 300, 400);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 delay(600);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 digitalWrite(LED_KIRMIZI, LOW);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.clear();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 ekraniYenile();
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 void sahteSifreCezasi() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 kalanSure -= 10;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 digitalWrite(LED_KIRMIZI, HIGH);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.clear();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print("TUZAK KOD! -10sn");
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 tone(BUZZER, 200, 600);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 delay(700);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 digitalWrite(LED_KIRMIZI, LOW);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.clear();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 ekraniYenile();
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 void adminModu() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.clear();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.setCursor(0, 0);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print("ADMIN - Sifre:");
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.setCursor(0, 1);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.print(dogruSifre);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 tone(BUZZER, 1800, 300);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 delay(5000);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 lcd.clear();
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 ekraniYenile();
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
\'a0\

\f0\fs21\fsmilli10667 void oyunuSifirla() \{
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 oyunBitti = false;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 girilen = "";
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 sonIpucu = 0;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 acilanHane = 1;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 dSayaci = 0;
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 digitalWrite(LED_YESIL, LOW);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 digitalWrite(LED_KIRMIZI, LOW);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 tone(BUZZER, 1000, 200);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 delay(300);
\f1\fs32 \

\f0\fs21\fsmilli10667 \'a0 zorlukSec();
\f1\fs32 \

\f0\fs21\fsmilli10667 \}
\f1\fs32 \
}