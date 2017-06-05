 /*
 * Red Neuronal Arduino
 * Código por Moisés Díaz Malagón
 * Revisión 1.1
 */
#define ETA_MAX 8 //define el step
#define SGN 1 //define el signo de respuesta del proceso
#define REF_PIN 0 //A3
#define CONTROL_PINP 10

float v[3]; //PESOS DE tres entradas a neurona de salida (3 intermedias)
float w[3][3]; //neurona j, entrada i
float x[3];//vector de resultados de capa intermedia a ser comparados con pesos v
float e[3] = { 0,0,0 }, suma; //vector de entradas (error, error anterior, error anterior 2)
float u; //señal de control de neurona de salida
float ETA = ETA_MAX;
float error, delta, delta2; //variables para retro
float ref = 2. * 1023./5.0;

void inizializacion() { //inicializamos con pesos aleatorios, entre -0.1 y 0.1 sin tomar 0
  for (int i = 0; i<3; i++) {
    v[i] = (random(-10, 10) >= 0) ? 0.1:-0.1;
  }
  for (int i = 0; i<3; i++) {
    for (int j = 0; j<3; j++) {
      w[i][j] = (random(-10, 10) >= 0) ? 0.1:-0.1;
    }
  }
}

void run_forward() {
  //calcula el error en este instante leyendo pines anal[ogicos]
  e[0] = 1.;

  for (int j = 0; j<3; j++) { //itera sobre neuronas de capa intermedia
    suma = 0; //suma ponderada de cada neurona
    for (int i = 0; i<3; i++) { //itera sobre entradas de cada neurona
      suma += w[j][i] * e[i];
    }
    //sumatoria se pasa por funcion de activacion y llena vector de resultados
    x[j] = 1.0 / (1.0 + exp(-suma));
  }


  //para neurona final
  suma = 0;
  for (int i = 0; i<3; i++) { //itera sobre entradas de neurona de salida
    suma += v[i] * x[i];
  }
  //sumatoria se pasa por funcion de activacion y saca señal de control
  u = 1.0 / (1.0 + exp(-suma)); //MAXIMO 100% DE DUTY CYCLE
  float control = u * 255;
  analogWrite(CONTROL_PINP, (int)control);
}

void run_backward() {
  //actualizacion de errores
  e[2] = e[1];
  e[1] = (float)(ref - analogRead(REF_PIN)) / 1023;
  
  error = e[1]
  
  delta = error * u * (1 - u);
  for (int i = 0; i<3; i++) {
    v[i] += ETA*delta*x[i]*SGN; //ultimo es salida de la anterior, el signo de "1" es directamente proporcional la señal de control a la respuesta del proceso
  }

  for (int j = 0; j<3; j++) { //itera sobre neuronas de capa intermedia
    delta2 = delta * v[j] * x[j] * (1 - x[j]); //suma ponderada de cada neurona
    for (int i = 0; i<3; i++) { //itera sobre entradas de cada neurona
      w[j][i] += ETA*delta2*e[i]*SGN; //el signo de "1" es directamente proporcional la señal de control a la respuesta del proceso
    }
  }

}

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(REF_PIN, INPUT);
  pinMode(CONTROL_PINP, OUTPUT);

  inizializacion(); //inicializamos pesos de neuronas
  Serial.begin(9600);
  Serial.setTimeout(10000);
}

void loop()
{
  // turn the LED on (HIGH is the voltage level)
  run_forward();
  delayMicroseconds(1000);
  run_backward();

  if (Serial.available()) {
    byte input = Serial.read();
    if (input == 'h' || input == 'H') {
      Serial.println("Neural Network Control: ");
      Serial.println("1) Set learning to 100%");
      Serial.println("2) Set learning to 50%");
      Serial.println("3) Set learning to 10%");
      Serial.println("4) Set learning to 1%");
      Serial.println("8) Write setpoint");
      Serial.println("9) Set learning to 0%");
    }
    else {
      switch (input)
      {
      case '1':
        ETA = ETA_MAX;
        Serial.print("Learning Set to ");
        Serial.println(ETA);
        break;
      case '2':
        Serial.print("Learning Set to ");
        Serial.println(ETA);
        ETA = ETA_MAX*0.5;
        break;
      case '3':
        Serial.print("Learning Set to ");
        Serial.println(ETA);
        ETA = ETA_MAX*0.1;
        break;
      case '4':
        Serial.print("Learning Set to ");
        Serial.println(ETA);
        ETA = ETA_MAX*0.01;
        break;
      case '8':
        char input[2];
        Serial.print('\n');
        Serial.print("Reference XX -> X.X >");
        Serial.readBytesUntil('\n', input, 2);
        Serial.print(" ");
        Serial.print(input[0]);
        Serial.print(".");
        Serial.print(input[1]);
        Serial.print(" \n");
        ref = (input[0]-48) + (input[1] - 48)*0.1;
        ref = ref * 1023. / 5.;
        break;
      case '9':
        ETA = 0;
        Serial.print("Learning Set to ");
        Serial.println(ETA);
        break;
      default:
        break;
      }
      
    }

  }
}

