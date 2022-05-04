#include <FlexiTimer2.h>

const int led = 13;
const int grating_indicator = 4;
const int motor_indicator = 5;
const int dirPin = 2;
const int stepPin = 3;

long int current_step_num = 0; // current quantity of steps
long int needed_step_num = 0;

int sped = 30;
byte dr = 0;

long int steps = 0;



byte ind_1_state = 0;
byte ind_1_prestate = 0;
byte ind_2_state = 0;
byte ind_2_prestate = 0;
char command;

bool initialising = false;
bool stopRegim = false;

void setup()
{
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(grating_indicator, INPUT);
  pinMode(motor_indicator, INPUT);
  digitalWrite(dirPin, LOW);
  digitalWrite(led, LOW);
  Serial.begin(115200);
  FlexiTimer2::set(60000, timerInterupt);
}

void loop()
{
  if (Serial.available())
  {
    if (!isalpha(Serial.peek())) Serial.flush(); // отримано невідому команду - очистка порту

    command = Serial.read();

    if (command == 'i') // наказ ініціалізуватись
    {
      goto_zero();
      steps = 0;
      current_step_num = 0;
      Serial.print('i');
    }
    else if (command == 'd') //поїхати на якусь кількість кроків в якийсь бік
    {

      steps = Serial.parseInt();
      motor_run_nsteps();
      Serial.println(current_step_num);

    }
    else if (command == 'a') //перевірка чи підключений
    {

      Serial.print('a');

    }
    else if (command == 'g') //поїхати конкретно на крок номер...
    {
      needed_step_num = Serial.parseInt();
      steps = needed_step_num - current_step_num;
      motor_run_nsteps();
      Serial.println(current_step_num);
    }
  }
}


inline void motor_run_nsteps()
{
  digitalWrite(led, HIGH);
  if (steps < 0) dr = 1; else dr = 0;
  digitalWrite(dirPin, dr);
  steps = abs(steps);
  while (steps)
  {
    makeStep();
    steps--;
    if (!dr) current_step_num++; else current_step_num--;
    if (current_step_num > 576000) current_step_num -= 576000;
    if (current_step_num < 0) current_step_num += 576000;
  }
  digitalWrite(led, LOW);
}



void goto_zero()
{
  digitalWrite(led, HIGH);
  digitalWrite(dirPin, 0);

  // Move motor to end of grating indicator
  ind_1_state = !digitalRead(grating_indicator);
  ind_1_prestate = ind_1_state;

  initialising = true;
  FlexiTimer2::start();

  while (true)
  {
    if (stopRegim)
    {
      digitalWrite(led, LOW);
      initialising = false;
      stopRegim = false;
      FlexiTimer2::stop();
      return;
    }
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
    if (stopRegim)
    {
      digitalWrite(led, LOW);
      initialising = false;
      stopRegim = false;
      FlexiTimer2::stop();
      return;
    }
    makeStep();

    ind_2_state = digitalRead(motor_indicator);
    if ((!ind_2_state) && ind_2_prestate)
    {
      if (stopRegim)
      {
        digitalWrite(led, LOW);
        initialising = false;
        stopRegim = false;
        FlexiTimer2::stop();
        return;
      }
      makeStep();
      if (!ind_2_state) break;
      else
      {
        while (ind_2_state)
        {
          if (stopRegim)
          {
            digitalWrite(led, LOW);
            initialising = false;
            stopRegim = false;
            FlexiTimer2::stop();
            return;
          }
          makeStep();
        }
        break;
      }

    }
    ind_2_prestate = ind_2_state;
  }

  sped /= 10; // return high speed
  digitalWrite(led, LOW);
  initialising = false;
  stopRegim = false;
  FlexiTimer2::stop();
}

inline void makeStep()
{
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(sped);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(sped);
}


void  timerInterupt()
{
  if (initialising) stopRegim = true;
}
