# irrigationSys
<h1>SISTEMA DE IRRIGAÇÃO INTELIGENTE </h1>
Este é um projeto para a matéria de objetos inteligentes conectados da Universidade Presbiteriana Mazkenzie

O sistema consiste em uma placa ESP32 com integração ao WiFi que gerencia um sensor de umidade do solo e uma bomba de água, 
conectada ao sistema por meio de um módulo relê.
O script realiza a conexão com a internet por WiFi e o protocolo MQTT, que permite enviar e receber informações do esp32 
em um servidos broker. 
O sistema conta com funcionamento automático, implementado na função automatic() do script. Nele, a placa verifica a umidade do solo, 
e se ela estiver abaixo do desejado, há a ativação do módulo relê.
O sistema também conta com acionamento remoto, acessado e testado no aplicativo para mobile mqtt dash.

