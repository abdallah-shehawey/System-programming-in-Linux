#include "communication.h"

void Spi::init(int number_of_size_data)
{
  this->numberOfData = number_of_size_data;
  for (int i = 0; i < number_of_size_data; i++)
  {
    this->arrayofdata[i] = 's';
  }
}

int Communication::read(char *buffer, int size)
{
  if ((cursor + numberOfData) > size)
  {
    return -1;
  }

  for (int i = 0; i < numberOfData; i++)
  {
    buffer[i] = arrayofdata[cursor++];
  }
  return 0;
}

void Uart::init(int number_of_size_data)
{
  this->numberOfData = number_of_size_data;
  for (int i = 0; i < number_of_size_data; i++)
  {
    this->arrayofdata[i] = 'u';
  }
}

void I2c::init(int number_of_size_data)
{
  this->numberOfData = number_of_size_data;
  for (int i = 0; i < number_of_size_data; i++)
  {
    this->arrayofdata[i] = 'i';
  }
}
