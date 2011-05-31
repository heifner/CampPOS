// Copyright 2011 Kevin Heifner.  All rights reserved.
#ifndef CAMPER_H
#define CAMPER_H
#pragma once

#include <string>

typedef long long FPKey;

/// Camper storage
class Camper {
public:

  Camper(
    FPKey id, 
    const std::string& firstName,
    const std::string& lastName,
    float amount = 0.0f) 
    : id_(id)
    , firstName_(firstName)
    , lastName_(lastName)
    , amount_(amount)
  {}

  FPKey getId() const { return id_; }
  std::string getFirstName() const { return firstName_; }
  std::string getLastName() const { return lastName_; }

  float getAmount() const { return amount_; }
  void setAmount(float amount) { amount_ = amount; }

private:
  Camper(const Camper&); // disable
  Camper& operator=(const Camper&); // disable

private:
  const FPKey id_;
  std::string firstName_;
  std::string lastName_;
  float amount_;
};

#endif // CAMPER_H
