int counter = 0;
int delta = 1;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.write(constrain(counter, 0, 255));
  counter += delta;
  if (counter >= 255 || counter <= 0)
  {
    delta *= -1;
  }
}
