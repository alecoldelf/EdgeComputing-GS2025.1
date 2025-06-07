# Projeto Edge Computing – Monitoramento Ambiental

## Descrição
Este projeto implementa uma solução de edge computing utilizando um **ESP32**, sensor **DHT22** (temperatura e umidade) e sensor ultrassônico **HC-SR04** para monitorar condições ambientais. Os dados são exibidos em um display LCD I2C, enviados para a plataforma **TagoIO** via HTTP e publicados via **MQTT** em um broker público.

## Componentes de Hardware
- ESP32  
- Sensor DHT22  
- Sensor ultrassônico HC-SR04  
- Display LCD 16×2 I2C (endereço 0x27)  
- Jumpers e protoboard

## Requisitos de Software
- Wokwi (simulador online)  
- Bibliotecas Arduino:
  - WiFi.h  
  - HTTPClient.h  
  - PubSubClient.h  
  - DHT.h  
  - Wire.h  
  - LiquidCrystal_I2C.h  

## Guia de Configuração (Opcional)

### Credenciais Wi-Fi
```cpp
const char* ssid     = "SEU_SSID";
const char* password = "SUA_SENHA";
```

### Token TagoIO e Broker MQTT
```cpp
const char* TAGO_TOKEN  = "SEU_TOKEN_TAGO";
const char* mqtt_server = "test.mosquitto.org";
const int   mqtt_port   = 1883;
const char* mqtt_topic  = "esp32/sensores";
```

## Instalação e Uso
1. Clone ou abra este projeto no Wokwi:  
   https://wokwi.com/projects/432973411615565825  
2. Instale as bibliotecas listadas em `libraries.txt` no seu ambiente Arduino.  
3. (Opcional) Edite as variáveis de Wi-Fi e TagoIO/MQTT no sketch.  
4. Carregue o sketch (`sketch.ino`) no ESP32 e inicie a simulação.  
5. Observe:
   - **Serial Monitor**: exibição de temperatura, umidade e distância.  
   - **LCD I2C**: atualização em tempo real.  
   - **TagoIO**: recebimento de dados via HTTP.  
   - **Broker MQTT**: dados publicados no tópico configurado.

## Estrutura de Arquivos
- `sketch.ino`       – Código principal em C++ para ESP32  
- `diagram.json`     – Descrição do circuito no Wokwi  
- `libraries.txt`    – Lista de bibliotecas usadas  

## Simulação
1. Acesse o link do projeto no Wokwi.  
2. Clique em **Run**.  
3. Monitore:
   - Logs no console serial.  
   - Atualização do display LCD.  
   - Requisições HTTP para TagoIO.  
   - Publicações MQTT.

## Explicação em Vídeo
Assista à demonstração completa no YouTube:  
https://youtu.be/Y0t1gFIB4Uc

## Integrantes
- Alexandre Colvet Delfino — RM 560059  
- Enzo Luciano Barros de Oliveira — RM 559557  
- Lívia Pereira Dias Correa — RM 559414  

## Licença
Distribuído sob a licença **MIT**.

## Imagens Tago e Wokwi
![image](https://github.com/user-attachments/assets/90b9fde4-b12e-46e9-9ce9-48900b799164)
![image](https://github.com/user-attachments/assets/f8a9cbe0-6a7c-4d0e-a455-c7ec3fc427eb)

