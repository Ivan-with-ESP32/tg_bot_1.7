#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <GSON.h>
#include <GyverHTTP.h>
#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, 5, 16, 17> oled;

#define WIFI_SSID "TP-Link_BF4A"
#define WIFI_PASW "25659085"
#define BOT_TOKEN "7313695908:AAGA5_KXnj2mK4SAPjXN7-1SEcccVNkNXHQ"
#define ADMIN_ID "6231436511"

#include <GyverBME280.h>
GyverBME280 bme;

float temps[24];
float winds[24];
uint8_t codes[24];

float temp_min[4];
float temp_max[4];
float wind_mid[4];
uint8_t code_max[4];
uint8_t weth_code;
uint8_t w_code[3];
String wether_name[3];

String tasks[10][5];
String user_ID;
String tasks_users_ID[5];
String message_admin;
String function_message;
String command_message;
String message_task;
int comand;
int num_task;
bool task_go;

String message_users;
bool message_users_bool = 0;
String sticker_users;
bool sticker_users_bool = 0;

String HTTP = "https://api.open-meteo.com/v1/forecast?latitude=50.5206&longitude=26.2425&hourly=apparent_temperature,weather_code,wind_speed_10m&wind_speed_unit=ms&timezone=Europe%2FMoscow&forecast_days=1";
String message;
unsigned long timer;
String time_event;
String sticker_all;
byte event;
String users_ID_m[5] = {
  "6231436511",
  "5469714610",
  "1801244168"
};
String users_ID = "6231436511,5469714610";
String morning[4] = {
  "CAACAgIAAxkBAAEMqrtmwauEsV_ZyXMCtSVb4Tc_QMc0gAACwBYAAlDu0Eg5ZDBweGOjDzUE",
  "CAACAgIAAxkBAAEMqqVmwasNLX_pTd9Z76nycap2OSOoUAACPhoAAhlJiEkO2MQeYyo1njUE",
  "CAACAgIAAxkBAAEMqnFmwaoAAafuN5zdl0Wg2xOnHZja45IAAiEAAxDusDHckovBKcVWODUE",
  "CAACAgIAAxkBAAEMqslmwawwYpbKouK9mbf-tN1oeD6P3gACyRwAAvOW-EqIBJ04rz-qdzUE"
};
String evning[4] = {
  "CAACAgIAAxkBAAEMqkRmwaCYRvZ3CHmn3PbQSaYSkMbv7gACWiEAAmy4qEnRik64U9w4izUE",
  "CAACAgIAAxkBAAEMqodmwaoyqU5BAtHfjoGRdxWjbfwqDgACoSMAAptfIUoKEZP7xpUohzUE",
  "CAACAgIAAxkBAAEMqqlmwasnkNQHcdBDv_n0ujvd8f_99QACMB0AApePcUtiq8m-Iy-gUDUE",
  "CAACAgIAAxkBAAEMquxmwcFoURYqBGHvunOInWvWBdgfawACqTwAAjxW4El4IJOZJP77MTUE"
};
String eating[2] = {
  "CAACAgIAAxkBAAEMqm1mwan77Ji3E7QccJPRpYJUQ6SnVQACMQADTb8jFRxgzaoQr6pLNQQ",
  "CAACAgIAAxkBAAEMqqtmwasqzuecSu7L_d9ixznBF5tMCQACNSIAAsRTgUs08Tn70onzwDUE"
};
String mess_1[2] = {
  "Хм... задрімав, ось ",
  "Звичайно, ось "
};
String hello[5] = {
  "CAACAgIAAxkBAAEM68RnAQAB8il8-DrADC94REXX4d7mjEYAAkg9AALSyfhLyWlDIIYTtrA2BA",
  "CAACAgIAAxkBAAEM68hnAQMdAh-TJSZRnhEiXwABf-eTDCAAAmkuAAJqstlL1twVjM1Ntjk2BA",
  "CAACAgIAAxkBAAEM68pnAQMl3rl3NRCe7c82VTnvTU1wHgACN1oAApGSUEpLQStpxGi6oDYE",
  "CAACAgIAAxkBAAEM68xnAQMxsZ-EZGrEVGiJmn2K-F79YgACyRwAAvOW-EqIBJ04rz-qdzYE",
  "CAACAgIAAxkBAAEM685nAQOGWpJ9D9VLFtx_ORg2V6C23QACAUQAAt1xQUpJ0XyAWctRpDYE"
};

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void task(){
  tasks[num_task][0] = message_task;
  message = "";
  message = message + "Завдання №" + num_task + " записано";
  bot.sendMessage(message, user_ID);
  num_task++;
}

void function(){
  comand = function_message.indexOf(' ');
  command_message = function_message.substring(0, comand);
  if(command_message == "/admin"){
    message_admin = function_message.substring(comand, function_message.length());
    bot.sendMessage(message_admin, ADMIN_ID);
  }else if(command_message == "/task"){
    task_go = 1;
    tasks_users_ID[0] = user_ID;
    message_task = function_message.substring(comand, function_message.length());
    task();
  }else if(command_message == "/task_off"){
    task_go = 0;
  }else if(command_message == "/user_mess"){
    message_users = function_message.substring(comand++, function_message.length());
    message_users_bool = 1;
    bot.sendMessage("Виберіть кому надіслати повідомлення", ADMIN_ID);
    bot.showMenu("Мені \n Мамі \n Тимощук Ані", ADMIN_ID);
  }else if(command_message == "/user_sticker"){
    sticker_users = function_message.substring(comand += 1, function_message.length());
    sticker_users_bool = 1;
    bot.sendMessage("Виберіть кому надіслати стікер", ADMIN_ID);
    bot.showMenu("Мені \n Мамі \n Тимощук Ані", ADMIN_ID);
  }
}

void newMsg(FB_msg &msg){
  if (msg.text == "/start"){
    message = "";
    message = message + "Новий користувач: " + msg.username + ", " + msg.userID;
    bot.sendMessage(message, ADMIN_ID);
    bot.sendMessage("Вітаю. Я Бот для керування системою 'Розумний дім'. Я працюю на мікроконтроллері ESP32. Мій розробник запрограмував та навчив всього, що зараз я вмію.", msg.chatID);
    bot.sendMessage("А вмію я багато, передусім надсилати Вам температуру (/temperature), вологість (/humidity) та атмосферний тиск (/pressure)", msg.chatID);
    bot.sendMessage("До речі, я можу надсилати Вам прогноз погоди. Поки що тільки для Здолбунова, яле невдовзі навчуся і для інших міст. Надсилати прогноз погоди я буду о 7 годині ранку, а також за командою /weather", msg.chatID);
    bot.sendMessage("Для початку познайомимось. Вкажіть, будь ласка, свій рід", msg.chatID);
    bot.sendSticker("CAACAgIAAxkBAAEMqs9mwaw9M2mTo55Pwr9bhGGo1GRxIgACdhgAAgQfAAFL8Vn_RwhWXfA1BA", msg.chatID);
    bot.showMenu("Жіночий \n Чоловічий", msg.chatID);
  }else if(msg.text == "Жіночий"){
    bot.closeMenu(msg.chatID);
    message = "";
    message = message + "Приємно познайомитися, пані " + msg.username;
    bot.sendMessage(message, msg.chatID);
    bot.sendSticker(hello[random(0, 4)], msg.chatID);
    bot.sendMessage("Також Ви можете надсилати сповіщення адміністранції напряму. Для цього використовуйте команду /admin, \n приклад: /admin як тебе звати?", msg.chatID);
    bot.sendMessage("Відтепер ви можете створювати завдання на день, про які я нагадаю. \nДля цього викортстовуйте команду /task завдання; час \n Час, будь ласка вказуйте в форматі 6:15 або 18:00", msg.chatID);
  }else if(msg.text == "Чоловічий"){
    bot.closeMenu(msg.chatID);
    message = "";
    message = message + "Приємно познайомитися, пане " + msg.username;
    bot.sendMessage(message, msg.chatID);
    bot.sendSticker(hello[random(0, 4)], msg.chatID);
    bot.sendMessage("Також Ви можете надсилати сповіщення адміністранції напряму. Для цього використовуйте команду /admin, \n приклад: /admin як тебе звати?", msg.chatID);
    bot.sendMessage("Відтепер ви можете створювати завдання на день, про які я нагадаю. \nДля цього викортстовуйте команду /task завдання; час \n Час, будь ласка вказуйте в форматі 6:15 або 18:00", msg.chatID);
  }else if(msg.text == "/temperature"){
    message = "";
    message = message + mess_1[random(0, 1)] + "температура в кімнаті: " + bme.readTemperature() + " *C";
    bot.sendMessage(message, msg.chatID);
  }else if(msg.text == "/humidity"){
    message = "";
    message = message + mess_1[random(0, 1)] + "вологість в кімнаті: " + bme.readHumidity() + " %";
    bot.sendMessage(message, msg.chatID);
  }else if(msg.text == "/pressure"){
    message = "";
    message = message + mess_1[random(0, 1)] + "атмосферний тиск в Здолбунові дорівнює: " + pressureToMmHg(bme.readPressure()) + " мм р. ст";
    bot.sendMessage(message, msg.chatID);
  }else if(msg.text == "Привіт" || msg.text == "привіт"){
    bot.sendSticker(hello[random(0, 4)], msg.chatID);
    bot.sendMessage("Привіт, чим я можу Вам допомогти? \n Сподіваюся не дарма прокидався", msg.chatID);
  }else if(msg.text == "/time"){
    bot.sendSticker("CAACAgIAAxkBAAEMqm9mwan-JY9ugPyLK1iwh61RlXj-ugACHAADEO6wMb93HJBsI7JCNQQ", msg.chatID);
    bot.sendMessage(time_event, msg.chatID);
  }else if(msg.text == "Дякую" || msg.text == "дякую"){
    bot.sendSticker("CAACAgIAAxkBAAEMqvBmwc9929ekzPMMFYBIEcfljmnmlgACfRMAAlUCuUmMFsQmzwjuWzUE", msg.chatID);
  }else if(msg.text == "Мені" && message_users_bool == 1){
    bot.sendMessage(message_users, users_ID_m[0]);
    message_users_bool = 0;
  }else if(msg.text == "Мамі" && message_users_bool == 1){
    bot.sendMessage(message_users, users_ID_m[1]);
    message_users_bool = 0;
  }else if(msg.text == "Тимощук Ані" && message_users_bool == 1){
    bot.sendMessage(message_users, users_ID_m[2]);
    message_users_bool = 0;
  }else if(msg.text == "Мені" && sticker_users_bool == 1){
    bot.sendSticker(sticker_users, users_ID_m[0]);
    Serial.println(sticker_users);
    sticker_users_bool = 0;
  }else if(msg.text == "Мамі" && sticker_users_bool == 1){
    bot.sendSticker(sticker_users, users_ID_m[1]);
    sticker_users_bool = 0;
  }else if(msg.text == "Тимощук Ані" && sticker_users_bool == 1){
    bot.sendSticker(sticker_users, users_ID_m[2]);
    sticker_users_bool = 0;
  }else if(msg.text == "/weather"){
    ghttp::EspInsecureClient http("api.open-meteo.com", 443);
    if (http.request(HTTP)) {
      ghttp::Client::Response resp = http.getResponse();
      if (resp) {
        String s = resp.body().readString();
        gson::Parser json;
        if (json.parse(s)) {
          json["hourly"]["apparent_temperature"].parseTo(temps);
          json["hourly"]["wind_speed_10m"].parseTo(winds);
          json["hourly"]["weather_code"].parseTo(codes);
          json["hourly"].stringify(Serial);
        } else {
          Serial.println("parse error");
        }
      } else {
          Serial.println("response error");
      }
    }
  temp_min[0] = (temps[6] += temps[7] += temps[8] += temps[9] += temps[10] += temps[11]) / 6;
  temp_min[1] = (temps[12] += temps[13] += temps[14] += temps[15] += temps[16] += temps[17]) / 6;
  temp_min[2] = (temps[18] += temps[19] += temps[20] += temps[21] += temps[22] += temps[23]) / 6;

  wind_mid[0] = (winds[6] + winds[7] + winds[8] + winds[9] + winds[10] + winds[11]) / 6;
  wind_mid[1] = (winds[12] + winds[13] + winds[14] + winds[15] + winds[16] + winds[17]) / 6;
  wind_mid[2] = (winds[18] + winds[19] + winds[20] + winds[21] + winds[22] + winds[23]) / 6;

  weth_code = winds[6];
  for(int i = 6; i < 12; i++){
    weth_code = max(weth_code, codes[i]);
  }
  code_max[0] = weth_code;

  weth_code = winds[12];
  for(int i = 12; i < 17; i++){
    weth_code = max(weth_code, codes[i]);
  }
  code_max[1] = weth_code;

  weth_code = winds[18];
  for(int i = 18; i < 23; i++){
    weth_code = max(weth_code, codes[i]);
  }
  code_max[2] = weth_code;
  for(int i = 0; i < 3; i++){
        switch(code_max[i]){
            case 0: wether_name[i] = "чисте небо";
            break;
            case 1: wether_name[i] = "переважно хмарно";
            break;
            case 2: wether_name[i] = "мінлива хмарність";
            break;
            case 3: wether_name[i] = "похмуро";
            break;
            case 45: wether_name[i] = "туман";
            break;
            case 48: wether_name[i] = "відкладання інею";
            break;
            case 51: wether_name[i] = "слабка мряка";
            break;
            case 53: wether_name[i] = "помірна мряка";
            break;
            case 55: wether_name[i] = "сильна мряка";
            break;
            case 56: wether_name[i] = "легкий крижаний дощ";
            break;
            case 57: wether_name[i] = "щільний крижаний дощ";
            break;
            case 61: wether_name[i] = "слабкий дощ";
            break;
            case 63: wether_name[i] = "помірний дощ";
            break;
            case 65: wether_name[i] = "сильний дощ";
            break;
            case 66: wether_name[i] = "слабкий крижаний дощ";
            break;
            case 67: wether_name[i] = "сильний крижаний дощ";
            break;
            case 71: wether_name[i] = "слабкий снігопад";
            break;
            case 73: wether_name[i] = "помірний снігопад";
            break;
            case 75: wether_name[i] = "сильний снігопад";
            break;
            case 77: wether_name[i] = "сніжна крупа";
            break;
            case 80: wether_name[i] = "невеликий дощ";
            break;
            case 81: wether_name[i] = "помірний дощ";
            break;
            case 82: wether_name[i] = "сильний дощ";
            break;
            case 85: wether_name[i] = "невеликий снігопад";
            break;
            case 86: wether_name[i] = "сильний снігопад";
            break;
            case 95: wether_name[i] = "гроза";
            break;
            case 96: wether_name[i] = "гроза з невеликим градом";
            break;
            case 99: wether_name[i] = "гроза з сильнийм градом";
            break;
        }
    }

  Serial.print(temp_min[0]);
  Serial.print('\t');
  Serial.print(temp_min[1]);
  Serial.print('\t');
  Serial.print(temp_min[2]);
  Serial.println('\n');
  Serial.print(wind_mid[0]);
  Serial.print('\t');
  Serial.print(wind_mid[1]);
  Serial.print('\t');
  Serial.print(wind_mid[2]);
  Serial.println('\n');
  Serial.print(wether_name[0]);
  Serial.print('\t');
  Serial.print(wether_name[1]);
  Serial.print('\t');
  Serial.print(wether_name[2]);
  
  message = "";
  message = message + "Середня температура в Здолбунові зранку дорівнює " + temp_min[0] + "*C, " + "\n" + "вдень (12-18): " + temp_min[1] + "*C, " + "\n" + "а ввечері: " + temp_min[2];
  bot.sendMessage(message, msg.chatID);
  message = "";
  message = message + "Протягом ранку швидкість вітру буде становити " + wind_mid[0] + " м/с, " + "\n" + "вдень - " + wind_mid[1] + " м/с, " "\n" + "і ввечері " + wind_mid[2];
  bot.sendMessage(message, msg.chatID);
  message = "";
  message = message + "Зранку очікується " + wether_name[0] + ", з 12 до 17 години - " + wether_name[1] + ", а ввечері - " + wether_name[2];
  bot.sendMessage(message, msg.chatID);
  }else{
    function_message = msg.text;
    user_ID = msg.chatID;
    Serial.println(function_message);
    function();
  }

  if(event == 1){
    bot.sendSticker(morning[random(0, 4)], users_ID);
    event = 0;
  }else if(event == 2){
    bot.sendSticker(evning[random(0, 4)], users_ID);
    event = 0;
  }else if(event == 3){
    bot.sendSticker(eating[random(0, 1)], users_ID);
    event = 0;
  }else if(event == 4){
    bot.sendSticker("CAACAgIAAxkBAAEMqsFmwau04Q0Qo8UR8-i4jt-YK3B_eAAChz4AAvqhUEp2ZLn-5zzd0DUE", users_ID);
    event = 0;
  }else if(event == 5){
    bot.sendSticker("CAACAgIAAxkBAAEMqmlmwan1mYqlmidxYLxKfWW8vbfgMgACIQADTb8jFSvG1AdPnBN8NQQ", users_ID);
    event = 0;
  }else if(event == 6){
    bot.sendSticker("CAACAgIAAxkBAAEMqmFmwanouvQD22ricj2C0ISkLQL8GgACEAADTb8jFQt1oXSLBgKiNQQ", users_ID);
    event = 0;
  }
}



void setup(){
  bme.begin();
  oled.init();
  oled.clear();
  oled.setScale(2);
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASW);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println(WiFi.localIP());
  bot.attach(newMsg);
  bot.setChatID(0);
}

void loop(){
  bot.tick();
  if(millis() - timer >= 1000){
    FB_Time t = bot.getTime(3);
    time_event = t.timeString();
    if(time_event == "06:30:01"){
      event = 1;
    }
    if(time_event == "21:00:01"){
      event = 2;
    }
    if(time_event == "13:00:01"){
      event = 3;
    }
    if(time_event == "10:00:01"){
      event = 4;
    }
    if(time_event == "22:00:01"){
      event = 5;
    }
    if(time_event == "17:30:01"){
      event = 6;
    }
    if(WiFi.status() == WL_CONNECTED){
      oled.clear();
      oled.home();
      oled.print("conected");
      oled.println("\n");
      oled.print(time_event);
      oled.update();
    }else{
      oled.clear();
      oled.home();
      oled.print("not conected");
      oled.println("\n");
      oled.print(time_event);
      oled.update();
    }
  }
  
}
