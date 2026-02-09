#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdio.h>

class Communication
{
public:
  char arrayofdata[100] = {0};

  virtual void init(int number_of_size_data) = 0;
  int read(char *buffer, int size);

  int cursor = 0;
  int numberOfData = 0;
};

class Spi : public Communication
{
public:
  void init(int number_of_size_data) override;
};

class Uart : public Communication
{
public:
  void init(int number_of_size_data) override;
};

class I2c : public Communication
{
public:
  void init(int number_of_size_data) override;
};

#endif
