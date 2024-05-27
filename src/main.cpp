#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

// Access Point credentials
const char* ssid = "ESP8266-Access-Point";
const char* password = "12345678";

AsyncWebServer server(80);

void deleteUserById(const char* uid);
String getUsers();
void addUser(const char* name, const char* uid);

void setup() {
    Serial.begin(115200);
    
    if(!LittleFS.begin()){
        Serial.println("An error has occurred while mounting LittleFS");
        return;
    }

    // Initialize Access Point
    WiFi.softAP(ssid, password);
    Serial.println("Access Point started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/users", HTTP_GET, [](AsyncWebServerRequest *request){
        String users = getUsers();
        request->send(200, "application/json", users);
    });

    server.on("/user", HTTP_DELETE, [](AsyncWebServerRequest *request){
        Serial.println("DELETE request received");
        int params = request->params();
        for(int i=0;i<params;i++){
            AsyncWebParameter* p = request->getParam(i);
            Serial.print("Param name: ");
            Serial.println(p->name());
            Serial.print("Param value: ");
            Serial.println(p->value());
        }
        if (request->hasParam("uid")) {
            String uid = request->getParam("uid")->value();
            Serial.print("UID to delete: ");
            Serial.println(uid);
            deleteUserById(uid.c_str());
            request->send(200, "application/json", "{\"message\":\"User deleted\"}");
        } else {
            request->send(400, "application/json", "{\"message\":\"Bad Request\"}");
        }
    });

    server.on("/user", HTTP_POST, [](AsyncWebServerRequest *request){
        if (request->hasParam("name", true) && request->hasParam("uid", true)) {
            String name = request->getParam("name", true)->value();
            String uid = request->getParam("uid", true)->value();
            Serial.print("Adding user: ");
            Serial.print(name);
            Serial.print(" with UID: ");
            Serial.println(uid);
            addUser(name.c_str(), uid.c_str());
            request->send(200, "application/json", "{\"message\":\"User added\"}");
        } else {
            request->send(400, "application/json", "{\"message\":\"Bad Request\"}");
        }
    });

    server.begin();

    File file = LittleFS.open("/user.json", "r");

    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }
    
    size_t size = file.size();
    if(size > 1024){
        Serial.println("File size is too large");
        return;
    }
    
    std::unique_ptr<char[]> buf(new char[size]);
    file.readBytes(buf.get(), size);
    
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, buf.get());
    
    if(error){
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    
    for(JsonObject user : doc.as<JsonArray>()) {
        const char* name = user["name"];
        const char* uid = user["uid"];
        
        Serial.print("Name: ");
        Serial.print(name);
        Serial.print(" - UID: ");
        Serial.println(uid);
    }

    file.close();
}

void deleteUserById(const char* uid) {
    File file = LittleFS.open("/user.json", "r");

    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    size_t size = file.size();
    std::unique_ptr<char[]> buf(new char[size]);
    file.readBytes(buf.get(), size);

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, buf.get());
    
    if(error){
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    file.close();
    
    JsonArray users = doc.as<JsonArray>();
    for (JsonArray::iterator it = users.begin(); it != users.end(); ++it) {
        if (strcmp((*it)["uid"], uid) == 0) {
            users.remove(it);
            break;
        }
    }

    file = LittleFS.open("/user.json", "w");
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    
    serializeJson(doc, file);
    file.close();
    
    Serial.print("User with UID ");
    Serial.print(uid);
    Serial.println(" deleted.");
}

void addUser(const char* name, const char* uid) {
    File file = LittleFS.open("/user.json", "r");

    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    size_t size = file.size();
    std::unique_ptr<char[]> buf(new char[size]);
    file.readBytes(buf.get(), size);

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, buf.get());

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    file.close();

    JsonArray users = doc.as<JsonArray>();
    JsonObject newUser = users.createNestedObject();
    newUser["name"] = name;
    newUser["uid"] = uid;

    file = LittleFS.open("/user.json", "w");
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }

    serializeJson(doc, file);
    file.close();

    Serial.print("User with name ");
    Serial.print(name);
    Serial.print(" and UID ");
    Serial.println(uid);
    Serial.println(" added.");
}

String getUsers() {
    File file = LittleFS.open("/user.json", "r");

    if (!file) {
        Serial.println("Failed to open file for reading");
        return "";
    }

    size_t size = file.size();
    std::unique_ptr<char[]> buf(new char[size]);
    file.readBytes(buf.get(), size);

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, buf.get());

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return "";
    }

    file.close();

    String users;
    serializeJson(doc, users);
    return users;
}

void loop() {
}
