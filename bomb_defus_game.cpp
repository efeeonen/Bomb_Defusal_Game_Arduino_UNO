#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===================== PINLER =====================
const int BUZZER      = 10;
const int LED_YESIL   = 11;
const int LED_KIRMIZI = 12;
const int TRIG        = A1;
const int ECHO        = A2;

// ===================== KEYPAD =====================
const byte SATIR = 4;
const byte SUTUN = 4;

char tuslar[SATIR][SUTUN] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte satirPin[SATIR] = {9, 8, 7, 6};
byte sutunPin[SUTUN] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(tuslar), satirPin, sutunPin, SATIR, SUTUN);

// ===================== SIFRELER =====================
String dogruSifre = "1357";
String sahteSifre = "0000";
int sifreUzunlugu = 4;

// ===================== OYUN DEGISKENLERI =====================
String girilen = "";
int kalanSure = 120;
unsigned long sonTik = 0;
bool oyunBitti = false;

// ===================== ADMIN MODU =====================
int dSayaci = 0;

// ===================== MESAFE SENSORU IPUCU =====================
int acilanHane = 1;
unsigned long sonIpucu = 0;
const unsigned long IPUCU_BEKLEME = 15000;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));

  pinMode(BUZZER, OUTPUT);
  pinMode(LED_YESIL, OUTPUT);
  pinMode(LED_KIRMIZI, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  lcd.init();
  lcd.backlight();

  zorlukSec();
}

void loop() {
  if (oyunBitti) {
    char tus = keypad.getKey();
    if (tus == '*') {
      oyunuSifirla();
    }
    return;
  }

  geriSayim();
  tusOku();
  mesafeIpucu();
  durumLed();
}

// secilen moda gore kurala uygun sifre uretir
String sifreUret(int uzunluk) {
  String sifre = "";
  int oncekiRakam = 0;
  bool serbest = false;

  for (int i = 0; i < uzunluk; i++) {
    int rakam;

    if (serbest) {
      int tekler[] = {1, 3, 5, 7, 9};
      rakam = tekler[random(0, 5)];
      serbest = false;
    } else {
      int kalanHane = uzunluk - i - 1;
      int ustSinir = 9 - kalanHane;
      if (ustSinir < oncekiRakam + 1) ustSinir = oncekiRakam + 1;
      if (ustSinir > 9) ustSinir = 9;
      rakam = random(oncekiRakam + 1, ustSinir + 1);
    }

    sifre += String(rakam);

    if (rakam == 9) {
      serbest = true;
      oncekiRakam = 0;
    } else {
      oncekiRakam = rakam;
    }
  }

  return sifre;
}

void zorlukSec() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Zorluk sec:");

  String mesaj = "A=Kolay B=Orta C=Zor    ";
  int scrollPos = 0;
  unsigned long sonKayma = 0;
  char secim = 0;

  while (secim != 'A' && secim != 'B' && secim != 'C') {
    secim = keypad.getKey();
    if (millis() - sonKayma >= 300) {
      sonKayma = millis();
      lcd.setCursor(0, 1);
      String gosterilecek = mesaj.substring(scrollPos) + mesaj.substring(0, scrollPos);
      lcd.print(gosterilecek.substring(0, 16));
      scrollPos++;
      if (scrollPos >= mesaj.length()) scrollPos = 0;
    }
  }

  if (secim == 'A') {
    kalanSure = 120;
    sifreUzunlugu = 4;
    dogruSifre = sifreUret(4);
  } else if (secim == 'B') {
    kalanSure = 90;
    sifreUzunlugu = 5;
    dogruSifre = sifreUret(5);
  } else {
    kalanSure = 60;
    sifreUzunlugu = 6;
    dogruSifre = sifreUret(6);
  }

  bilgilendirmeGoster();

  acilanHane = 1;
  sonIpucu = 0;
  dSayaci = 0;

  ipucuGoster(1);

  lcd.clear();
  lcd.print("Basliyor...");
  delay(1000);
  sonTik = millis();
  lcd.clear();
  ekraniYenile();
}

void bilgilendirmeGoster() {
  String mesaj = "Sifrenin her basamagi bir oncekinden buyuk olmali. Eger bir basamak 9 gelirse sonraki basamak tek sayi olmali.";

  while (true) {
    kaydir(mesaj);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tekrar oku : 1");
    lcd.setCursor(0, 1);
    lcd.print("Oyuna basla: 2");

    char s = 0;
    while (s != '1' && s != '2') {
      s = keypad.getKey();
    }

    if (s == '2') break;
  }
}

// yaziyi soldan saga kaydirir
void kaydir(String mesaj) {
  String full = "                " + mesaj + "                ";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BILGILENDIRME");
  for (int pos = 0; pos + 16 <= (int)full.length(); pos++) {
    lcd.setCursor(0, 1);
    lcd.print(full.substring(pos, pos + 16));
    delay(250);
  }
}

void ipucuGoster(int haneNo) {
  int haneIndex = haneNo - 1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ipucu");
  lcd.setCursor(0, 1);
  lcd.print(haneNo);
  lcd.print(". hane : ");
  lcd.print(dogruSifre.charAt(haneIndex));
  delay(5000);
  lcd.clear();
}

// 5 cm den yakin algilarsa siradaki haneyi acar
void mesafeIpucu() {
  if (acilanHane >= (int)dogruSifre.length()) return;
  if (sonIpucu != 0 && (millis() - sonIpucu < IPUCU_BEKLEME)) return;

  long mesafe = mesafeOlc();
  if (mesafe > 0 && mesafe < 5) {
    acilanHane++;
    sonIpucu = millis();
    tone(BUZZER, 1200, 150);
    ipucuGoster(acilanHane);
    ekraniYenile();
  }
}

long mesafeOlc() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long sure = pulseIn(ECHO, HIGH, 30000);
  if (sure == 0) return -1;
  return sure * 0.034 / 2;
}

void geriSayim() {
  if (millis() - sonTik >= 1000) {
    sonTik = millis();
    kalanSure--;
    tone(BUZZER, 1000, 50);
    ekraniYenile();
    if (kalanSure <= 0) patla();
  }
}

void ekraniYenile() {
  lcd.setCursor(0, 0);
  lcd.print("Sure: ");
  lcd.print(kalanSure);
  lcd.print("s   ");
  lcd.setCursor(0, 1);
  lcd.print("Kod: ");
  lcd.print(girilen);
  lcd.print("        ");
}

void tusOku() {
  char tus = keypad.getKey();
  if (!tus) return;

  // 4 kez D ile admin modu
  if (tus == 'D') {
    dSayaci++;
    if (dSayaci >= 4) {
      dSayaci = 0;
      adminModu();
      return;
    }
    return;
  } else {
    dSayaci = 0;
  }

  if (tus == '#') {
    if (girilen == dogruSifre) {
      defuse();
    } else if (girilen == sahteSifre) {
      sahteSifreCezasi();
    } else {
      yanlisSifre();
    }
    girilen = "";
  }
  else if (tus == '*') {
    girilen = "";
    ekraniYenile();
  }
  else {
    if ((int)girilen.length() < sifreUzunlugu) girilen += tus;
    tone(BUZZER, 1500, 30);
    ekraniYenile();
  }
}

void durumLed() {
  if (kalanSure <= 5 && kalanSure > 0) {
    digitalWrite(LED_KIRMIZI, (millis() / 200) % 2);
  } else {
    digitalWrite(LED_KIRMIZI, LOW);
  }
}

void defuse() {
  oyunBitti = true;
  digitalWrite(LED_KIRMIZI, LOW);
  digitalWrite(LED_YESIL, HIGH);
  lcd.clear();
  lcd.print("BOMBA IMHA EDILDI!");

  tone(BUZZER, 2000);
  delay(2000);
  noTone(BUZZER);
  delay(200);

  int notalar[] = {
    294, 294, 294, 392, 587, 523, 494, 440, 784, 587,
    523, 494, 440, 784, 587, 523, 494, 523, 440
  };
  int sureler[] = {
    150, 150, 150, 900, 900, 150, 150, 150, 900, 450,
    150, 150, 150, 900, 450, 150, 150, 150, 900
  };
  int notaSayisi = sizeof(notalar) / sizeof(notalar[0]);
  for (int i = 0; i < notaSayisi; i++) {
    tone(BUZZER, notalar[i]);
    delay(sureler[i]);
    noTone(BUZZER);
    delay(50);
  }

  digitalWrite(LED_YESIL, LOW);
  oyunSonuMesaji();
}

void patla() {
  oyunBitti = true;
  digitalWrite(LED_YESIL, LOW);
  digitalWrite(LED_KIRMIZI, HIGH);
  lcd.clear();
  lcd.print("BOOM! PATLADI");

  for (int i = 0; i < 5; i++) {
    tone(BUZZER, 400, 200);
    delay(250);
  }

  delay(3800);
  digitalWrite(LED_KIRMIZI, LOW);
  oyunSonuMesaji();
}

void oyunSonuMesaji() {
  delay(4000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Oyun bitti");
  lcd.setCursor(0, 1);
  lcd.print("Yeniden baslat:*");
}

void yanlisSifre() {
  kalanSure -= 5;
  digitalWrite(LED_KIRMIZI, HIGH);
  lcd.clear();
  lcd.print("YANLIS! -5sn");
  tone(BUZZER, 300, 400);
  delay(600);
  digitalWrite(LED_KIRMIZI, LOW);
  lcd.clear();
  ekraniYenile();
}

void sahteSifreCezasi() {
  kalanSure -= 10;
  digitalWrite(LED_KIRMIZI, HIGH);
  lcd.clear();
  lcd.print("TUZAK KOD! -10sn");
  tone(BUZZER, 200, 600);
  delay(700);
  digitalWrite(LED_KIRMIZI, LOW);
  lcd.clear();
  ekraniYenile();
}

void adminModu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ADMIN - Sifre:");
  lcd.setCursor(0, 1);
  lcd.print(dogruSifre);
  tone(BUZZER, 1800, 300);
  delay(5000);
  lcd.clear();
  ekraniYenile();
}

void oyunuSifirla() {
  oyunBitti = false;
  girilen = "";
  sonIpucu = 0;
  acilanHane = 1;
  dSayaci = 0;
  digitalWrite(LED_YESIL, LOW);
  digitalWrite(LED_KIRMIZI, LOW);
  tone(BUZZER, 1000, 200);
  delay(300);
  zorlukSec();
}
