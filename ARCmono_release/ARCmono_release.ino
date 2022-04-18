const int led = 13;
const int grating_indicator = 4;
const int motor_indicator = 5;
const int dirPin = 2;
const int stepPin = 3;

long int n1 = 0; // current quantity of steps
long int n2 = 0; // previous quantity of steps

int sped = 30;
byte dr = 0;

int steps = 0;
int magic_init = 5555;


byte ind_1_state = 0;
byte ind_1_prestate = 0;
byte ind_2_state = 0;
byte ind_2_prestate = 0;

void setup()
{
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(grating_indicator, INPUT);
  pinMode(motor_indicator, INPUT);
  digitalWrite(dirPin, LOW);
  digitalWrite(led, LOW);
  Serial.begin(9600);
}
void loop()
{
  steps = Serial.parseInt();
  if (steps == magic_init) {goto_zero(); steps = 0; n1 = 0; n2 = 0;}
  
  if (steps < 0) dr = 1; 
  if (steps > 0) dr = 0; 

  digitalWrite(dirPin, dr);
  steps = abs(steps);



  while (steps)
  {
    makeStep();
    steps--;
    if (!dr) n1++; else n1--;
  }

  if (n1 != n2)
  {
    Serial.println(n1);
    n2 = n1;
  }


}

void goto_zero()
{

  digitalWrite(dirPin, 0);

  // Move motor to end of grating indicator
  ind_1_state = !digitalRead(grating_indicator);
  ind_1_prestate = ind_1_state;

  while (true)
  {
    makeStep();

    ind_1_state = !digitalRead(grating_indicator);
    if ((!ind_1_state) && ind_1_prestate)
    {
      makeStep();
      if (!ind_1_state) break;
      else
      {
        while (ind_1_state) makeStep();
        break;
      }

    }
    ind_1_prestate = ind_1_state;

  }
  // Move motor to end of motor indicator
  sped *= 10; //low speed...
  ind_2_state = digitalRead(motor_indicator);
  ind_2_prestate = ind_1_state;

  while (true)
  {
    makeStep();

    ind_2_state = digitalRead(motor_indicator);
    if ((!ind_2_state) && ind_2_prestate)
    {
      makeStep();
      if (!ind_2_state) break;
      else
      {
        while (ind_2_state) makeStep();
        break;
      }

    }
    ind_2_prestate = ind_2_state;
  }

  sped /= 10; // return high speed
}

inline void makeStep()
{
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(sped);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(sped);
}
