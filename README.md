# GS-EDGE
# Projeto de Monitoramento de Temperatura em Ambiente Hospitalar

## Descrição do Problema de Saúde
O projeto aborda a necessidade de monitorar a temperatura em ambientes hospitalares para prevenir a propagação de infecções, garantindo um ambiente seguro para pacientes e profissionais de saúde.

## Visão Geral da Solução Proposta
O sistema utiliza um sensor de temperatura DHT22 para medir a temperatura ambiente. Os dados são enviados para um servidor MQTT (broker) para monitoramento remoto. Além disso, incorpora um Buzzer para alertar quando a temperatura ultrapassa um limite considerado indesejado.

## Configuração e Execução

### Hardware Necessário
- ESP32 (ou similar) com suporte a WiFi
- Sensor de temperatura DHT22
- Buzzer

### Configuração do Projeto
1. Clone este repositório.
2. Instale as bibliotecas necessárias: `DHTesp` e `PubSubClient`.
3. Configure as credenciais WiFi e MQTT no código (`SSID`, `PASSWORD`, `BROKER_MQTT`, `BROKER_PORT`, `ID_MQTT`).

### Execução
1. Carregue o código para o seu ESP32.
2. Monitore a saída serial para obter informações sobre a temperatura e a conectividade MQTT.

## Simulação no Wokwi
Acesse a simulação do projeto no Wokwi [aqui](link_para_a_simulacao_no_wokwi).

## Estrutura do Código
- O código-fonte principal está no arquivo `main.ino`.
- A pasta `src` contém outros arquivos de código-fonte, como `DHTWrapper.cpp` e `DHTWrapper.h`.
