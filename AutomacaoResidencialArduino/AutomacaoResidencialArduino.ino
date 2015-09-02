#include <IRremote.h>   // Adiciona a biblioteca do Infravermelho no algoritmo 
IRsend irsend;

int LAMP = 5;   // Define o pino digital 5 como controlador do MOC3011
int dimming = 120; // O dimmer começa com a menor potência fornecida, ou seja, com a carga fora de funcionamento.
int i = 0 , val6 = 0 , val7 = 0 , val8 = 0;  // Define as variáveis do algoritmo
int x = 0 , y, z=0, remap, key, val, n1, contador1;

void setup()
{
  Serial.begin(9600); //Iniciando a Serial
  delay(1000); // Espera um tempo de 1 segundo.
  pinMode(8, OUTPUT); // Define o pino digital 8 do Arduino como Saída.
  pinMode(7, OUTPUT); // Define o pino digital 7 do Arduino como Saída.
  pinMode(6, OUTPUT); // Define o pino digital 6 do Arduino como Saída.
  pinMode(LAMP, OUTPUT); // Define o pino digital 5 do Arduino como Saída.
  attachInterrupt(0, zero_crosss_int, RISING); //Inicio da interrupção quando a carga está conectada à rede doméstica.
}

void loop()
{
   while(!Serial.available()); // O programa espera receber alguma variável pela serial que será enviada pelo bluetooth
   if (Serial.available())
   {
     key=Serial.read();   // Faz a leitura dos Números enviados pela seria via bluetooth
     int y1 = key;
   
    // ==================== Controle do Dimmer com Botões
    
    if (key == 20){ // Se o botão pressionado no aplicativo for o 20..... então acione o Dimmer 
                    while(!Serial.available());
                    val=Serial.read();  // Define que a variável "val" é igual ao valor recebido pela Serial
                              
                    if (val > key){
                                    n1 = val; 
                                  }
                    else if (val < key){
                                         n1 = key; 
                                       }  
                    remap = map (n1, 0, 255, 120, 0);  // Simula um potenciômetro e inverte a sequência dos números 
                    delay(1);
                    if(remap > 100){                  // Se a variável "remap" for maior que 111.... então desative o Dimmer                                     
                                    digitalWrite(LAMP, LOW);
                                    delay(1);
                                    contador1 = 1;          // Quando a variável "contador1" for 1, então o Dimmer estará desativado
                                   }
                    else if(remap <= 100){     //  Quando a variável "remap" for menor ou igual a 111.... então Ative o Dimmer
                                          dimming = remap; 
                                          contador1 = 2;          //  Quando a variável "contador1" for 2, então o Dimmer estará ativado
                                         }
                  }
    
    
    // ==================== Controle do Relé com Botões
    
      if(key == 126){    // Se o botão pressionado no aplicativo for o LED1...... então acione o relé 1.
                      val6 = !val6;     // Sempre que o botão for pressionado mudará o estado do relé entre ligado e desligado.
                      digitalWrite(6,val6);
                    }
      if(key == 127){    // Se o botão pressionado no aplicativo for o LED2...... então acione o relé 2.
                      val7 = !val7 ;    // Sempre que o botão for pressionado mudará o estado do relé entre ligado e desligado.
                      digitalWrite(7,val7);
                    }
      if(key == 128){    // Se o botão pressionado no aplicativo for o LED3...... então acione o relé 3.
                      val8 = !val8 ;    // Sempre que o botão for pressionado mudará o estado do relé entre ligado e desligado.
                      digitalWrite(8,val8);
                    }

    
    // ==================== Controle do Infravermelho  
   
   
    if (y1 == 121) {   // Se o botão pressionado no aplicativo for o Ligar/Desligar, envie o dado pelo infravermelho
                      irsend.sendSAMSUNG(0xE0E040BF, 32);
                      delay(100); 
                   }
  
   if (y1 == 122) {   // Se o botão pressionado no aplicativo for o Volume (+), envie o dado pelo infravermelho
                      irsend.sendSAMSUNG(0xE0E0E01F, 32);
                      delay(100);
                  }
  
   if (y1 == 123) {   // Se o botão pressionado no aplicativo for o Volume (-), envie o dado pelo infravermelho
                      irsend.sendSAMSUNG(0xE0E0D02F, 32);
                      delay(100);
                  }
  
   if (y1 == 124) {   // Se o botão pressionado no aplicativo for o Canal (+), envie o dado pelo infravermelho
                      irsend.sendSAMSUNG(0xE0E048B7, 32);   
                      delay(100);
                  }
  
   if (y1 == 125) {   // Se o botão pressionado no aplicativo for o Canal (-), envie o dado pelo infravermelho
                      irsend.sendSAMSUNG(0xE0E008F7, 32);
                      delay(100);
                  }
    
  }
  delay(1);
}

void zero_crosss_int() // Código para Funcionamento do Dimmer
{
  if(contador1 ==2){
                    int dimtime = (65 * dimming);  // Após passar o tempo calculado para o ângulo de disparo   
                    delayMicroseconds(dimtime);    // O algoritmo envia um pulso de 8,33 microssegundos para o gate do TRIAC 
                    digitalWrite(LAMP, HIGH);
                    delayMicroseconds(8.33);        
                    digitalWrite(LAMP, LOW);  
                   } 
}


