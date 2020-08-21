## HC-SR04

Segundo o datasheet, o sensor ultrassônico HC-SR04 é disparado com um pulso de 10us no terminal trigger. Após isso, o sensor responderá com um pulso de largura proporcional ao tempo de ida e volta do som. O diagrama de tempo é mostrado na imagem abaixo:

![diagrama_tempo](docs/imgs/diagrama_tempo.PNG)

A distância pode ser determinada pela equação:

$$
 d = v * (t/2)
$$

Onde v é a velocidade do som ~= 340m/s
E t é a largura do pulso no pino ECHO, que correspondeao tempo de ida e volta.

## Esquemático

O diagrama abaixo mostra o esquema de ligação elétrica entre o stm32 e o hc-sr04

![esquematico](docs/imgs/Schematic_stm32-hcsr04_2020-08-20_21-40-31.PNG)

PA0 - Conectado ao pino TRIGGER, gerando um pulso de 10us.
PA3 - Conectado ao pino ECHO, interrupção borda de subida.
PA4 - Conectado ao pino ECHO, interrupção borda de descida.

## STM32CubeMX

Para gerar o sinal de trigger, o timer2 foi utilizado na configuração PWM. Este TIM2 gera um pulso de 10us em PA0 a cada 100ms.

Para medir o sinal no pino ECHO do sensor, o pino PA3 inicia a contagem do timer 3, quando ocorre uma interrupção pela borda de subida do sinal. Quando ocorre a borda de descida em PA4, a contagem é interrompida e também é obtido o valor do contador do timer. Este timer (TIM3), foi configurado com um preescaler de 72, para obter uma contagem a cada 1us.

### Configuração TIM2

![tim2_config1](docs/imgs/tim2_config1.PNG)
![tim2_config2](docs/imgs/tim2_config2.PNG)

### Configuração TIM3

![tim3_config1](docs/imgs/tim3_config1.PNG)
![tim3_config2](docs/imgs/tim3_config2.PNG)

### GPIO

![gpio](docs/imgs/gpio.PNG)
![gpio1](docs/imgs/gpio1.PNG)
![gpio2](docs/imgs/gpio2.PNG)
![gpio3](docs/imgs/gpio3.PNG)