# ESP8266 User Management API

Dieses Projekt bietet eine einfache Benutzerverwaltungs-API, die auf einem ESP8266 läuft. Es ermöglicht das Hinzufügen, Auflisten und Löschen von Benutzern über HTTP-Anfragen. Benutzerinformationen werden in einer JSON-Datei auf dem LittleFS-Dateisystem gespeichert.

## Features

- **Access Point**: Der ESP8266 fungiert als Access Point.
- **Benutzerverwaltung**: Benutzer können hinzugefügt, aufgelistet und gelöscht werden.
- **JSON-basierte Speicherung**: Benutzerinformationen werden in einer JSON-Datei gespeichert.

## API-Endpunkte

### GET /users

Listet alle Benutzer auf.

**Antwort**:

```json
[
  { "name": "Alice", "uid": "1234" },
  { "name": "Bob", "uid": "5678" },
  { "name": "Charlie", "uid": "9012" }
]
```

### POST /user

Fügt einen neuen Benutzer hinzu.

**Parameter**:

- `name` (string): Der Name des Benutzers.
- `uid` (string): Die eindeutige ID des Benutzers.

**Beispiel**:

```sh
curl -X POST http://<IP-Adresse>/user -d "name=Fritz&uid=1234"
```

**Antwort**:

```json
{ "message": "User added" }
```

### DELETE /user

Löscht einen Benutzer anhand der UID.

**Parameter**:

- `uid` (string): Die eindeutige ID des Benutzers.

**Beispiel**:

```sh
curl -X DELETE http://<IP-Adresse>/user?uid=1234
```

**Antwort**:

```json
{ "message": "User deleted" }
```

## Installation

1. **PlatformIO Projekt erstellen**:

   - Erstellen Sie ein neues PlatformIO-Projekt für den ESP8266.
   - Fügen Sie die notwendigen Bibliotheken (`ArduinoJson`, `LittleFS`, `ESPAsyncWebServer`) zu Ihrer `platformio.ini` Datei hinzu.

2. **Dateistruktur**:

   - Platzieren Sie die `main.cpp` Datei im `src` Ordner.
   - Platzieren Sie die `user.json` Datei im `data` Ordner mit folgendem Inhalt:

   ```json
   [
     { "name": "Alice", "uid": "1234" },
     { "name": "Bob", "uid": "5678" },
     { "name": "Charlie", "uid": "9012" }
   ]
   ```

3. **LittleFS-Daten hochladen**:

   ```sh
   pio run --target uploadfs
   ```

4. **Sketch hochladen**:

   ```sh
   pio run --target upload
   ```

5. **Seriellen Monitor öffnen**:
   - Überprüfen Sie die IP-Adresse des Access Points im seriellen Monitor.

## To-Dos

- [ ] **Authentifizierung**:
  - Implementieren Sie eine einfache Authentifizierung, um sicherzustellen, dass nur autorisierte Benutzer Änderungen vornehmen können.
- [ ] **Datenvalidierung**:
  - Fügen Sie eine Validierung hinzu, um sicherzustellen, dass die Daten (z.B. UID und Name) in einem erwarteten Format sind, bevor sie gespeichert werden.
- [ ] **Datenkonsistenz**:
  - Verhindern Sie das Hinzufügen von Benutzern mit doppelten UIDs.
- [ ] **Erweiterung der API**:
  - Fügen Sie Endpunkte hinzu, um Benutzerinformationen zu aktualisieren.
  - Implementieren Sie eine Suche nach Benutzern basierend auf verschiedenen Kriterien (z.B. Name oder UID).
- [ ] **Fehlerbehandlung**:
  - Verbessern Sie die Fehlerbehandlung und geben Sie aussagekräftigere Fehlermeldungen zurück.
- [ ] **Optimierung der Speicherplatznutzung**:
  - Überprüfen Sie, wie viel Speicherplatz auf dem ESP8266 noch verfügbar ist, und optimieren Sie die JSON-Verarbeitung entsprechend.

## Lizenz

Dieses Projekt ist unter der MIT-Lizenz lizenziert.
