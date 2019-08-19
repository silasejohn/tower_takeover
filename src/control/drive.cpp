#include "main.h"

#include "config/motor.h"
#include "config/io.h"

#include "control/drive.h"
#include "control/macro.h"

double current, error, last, derivative, output;

void reset() {
  current = 0; error = 0; last = 0; derivative = 0; output = 0;

  LF.tare_position();
  LB.tare_position();
  RF.tare_position();
  RB.tare_position();
  left(0);
  right(0);
}

void drive(double target, int speed, double rate) {
  const double kP = 0.4;
  const double kD = 0.6;

  double deltaL, deltaR;
  double slewOutput = 0;

  reset();

  while(target > 0) {
    deltaL = (LF.get_position() + LB.get_position()) / 2;
    deltaR = (RF.get_position() + RB.get_position()) / 2;
    current = ( deltaL + deltaR ) / 2;
    error = target - current;

    output = pTerm(target, current, kP) + dTerm(error, last) * kD;

    last = error;

    if(output > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = output;
    }

    if(slewOutput > speed) slewOutput = speed;

    if(isSettled(error, 9)) break;

    left(slewOutput - slop());
    right(slewOutput + slop());

    std::cout << LF.get_position() << ", " << RF.get_position() << std::endl;
    wait(20);
  }

  while(target < 0) {
    deltaL = (LF.get_position() + LB.get_position()) / 2;
    deltaR = (RF.get_position() + RB.get_position()) / 2;
    current = ( deltaL + deltaR ) / 2;
    error = target - current;

    output = pTerm(target, current, kP) + dTerm(error, last) * kD;

    last = error;

    if(abs(output) > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = abs(output);
    }

    if(slewOutput > speed) slewOutput = speed;

    if(isSettled(abs(error), 9)) break;

    left(-slewOutput - slop());
    right(-slewOutput + slop());

    std::cout << LF.get_position() << ", " << RF.get_position() << std::endl;
    wait(20);
  }

  reset();
}

void turn(double target, int speed, double rate) {
  const double kP = 0.6;
  const double kD = 0.6;

  double deltaL, deltaR;
  double lastSlew = 0, nowSlew = 0, slewOutput = 0;

  reset();

  while(target > 0) { // Turn Right
    deltaL = (LF.get_position() + LB.get_position()) / 2;
    deltaR = (RF.get_position() + RB.get_position()) / 2;
    current = ( deltaL + abs(deltaR) ) / 2;
    error = target - current;

    output = pTerm(target, current, kP) + dTerm(error, last) * kD;

    last = error;

    if(output > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = output;
    }

    if(slewOutput > speed) slewOutput = speed;

    if(isSettled(error, 6)) break;

    left(slewOutput);
    right(-slewOutput);

    std::cout << LF.get_position() << ", " << RF.get_position() << std::endl;
    wait(20);
  }

  while(target < 0) { // Turn Right
    deltaL = (LF.get_position() + LB.get_position()) / 2;
    deltaR = (RF.get_position() + RB.get_position()) / 2;
    current = ( abs(deltaL) + deltaR ) / 2;
    error = target + current;

    output = pTerm(target, -current, kP) + dTerm(error, last) * kD;

    last = error;

    if(abs(output) > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = abs(output);
    }

    if(slewOutput > speed) slewOutput = speed;

    if(isSettled(error, 6)) break;

    left(-slewOutput);
    right(slewOutput);

    std::cout << LF.get_position() << ", " << RF.get_position() << std::endl;
    wait(20);
  }

  reset();
}

void strafe(double target, int speed, double rate) {

  const double kP = 0.6;
  const double kD = 0.6;

  double deltaL, deltaR;
  double lastSlew = 0, nowSlew = 0, slewOutput = 0;

  while(target > 0) {
    deltaL = (LF.get_position() - LB.get_position()) / 2;
    deltaR = (RB.get_position() - RF.get_position()) / 2;
    current = (deltaL + deltaR) / 2;
    error = target - current;

    output = pTerm(target, current, kP) + dTerm(error, last) * kD;

    last = error;

    if(output > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = output;
    }

    if(slewOutput > speed) slewOutput = speed;

    LF.move_velocity(slewOutput + slop(2, 0));
    LB.move_velocity(-slewOutput + slop(2, 0));
    RF.move_velocity(-slewOutput - slop(2, 0));
    RB.move_velocity(slewOutput - slop(2, 0));

    if(isSettled(error, 6)) break;
    wait(20);
  }

  while(target < 0) {
    deltaL = (LF.get_position() - LB.get_position()) / 2;
    deltaR = (RB.get_position() - RF.get_position()) / 2;
    current = (deltaL + deltaR) / 2;
    error = target - current;

    output = pTerm(target, current, kP) + dTerm(error, last) * kD;

    last = error;

    if(abs(output) > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = abs(output);
    }

    if(slewOutput > speed) slewOutput = speed;

    LF.move_velocity(-slewOutput + slop(2, 0));
    LB.move_velocity(slewOutput + slop(2, 0));
    RF.move_velocity(slewOutput - slop(2, 0));
    RB.move_velocity(-slewOutput - slop(2, 0));

    if(isSettled(abs(error), 6)) break;
    wait(20);
  }

  reset();
}

void strafe(double target, int speed, double rate, double sturn) {

  const double kP = 0.6;
  const double kD = 0.6;

  double deltaL, deltaR;
  double lastSlew = 0, nowSlew = 0, slewOutput = 0;

  while(target > 0) {
    deltaL = (LF.get_position() - LB.get_position()) / 2;
    deltaR = (RB.get_position() - RF.get_position()) / 2;
    current = (deltaL + deltaR) / 2;
    error = target - current;

    output = pTerm(target, current, kP) + dTerm(error, last) * kD;

    last = error;

    if(output > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = output;
    }

    if(slewOutput > speed) slewOutput = speed;

    LF.move_velocity(slewOutput + slop(2, sturn));
    LB.move_velocity(-slewOutput + slop(2, sturn));
    RF.move_velocity(-slewOutput - slop(2, sturn));
    RB.move_velocity(slewOutput - slop(2, sturn));

    if(isSettled(error, 6)) break;
    wait(20);
  }

  while(target < 0) {
    deltaL = (LF.get_position() - LB.get_position()) / 2;
    deltaR = (RB.get_position() - RF.get_position()) / 2;
    current = (deltaL + deltaR) / 2;
    error = target - current;

    output = pTerm(target, current, kP) + dTerm(error, last) * kD;

    last = error;

    if(abs(output) > slewOutput + rate) {
      slewOutput += rate;
    } else {
      slewOutput = abs(output);
    }

    if(slewOutput > speed) slewOutput = speed;

    LF.move_velocity(-slewOutput + slop(2, -sturn));
    LB.move_velocity(slewOutput + slop(2, -sturn));
    RF.move_velocity(slewOutput - slop(2, -sturn));
    RB.move_velocity(-slewOutput - slop(2, -sturn));

    if(isSettled(abs(error), 6)) break;
    wait(20);
  }

  reset();
}


void align(double target, double tolerance) {

  double errorL, errorR, lastL, lastR, outputL, outputR;
  double now, last, elapsed;

  double kP = 0.7, kI = 1, kD = 8;

  reset();

  while(true) {
    elapsed = now - last;
    now = clock();

    errorL = target - ultraL.get_value();
    errorR = target - ultraR.get_value();

    outputL = pTerm(target, ultraL.get_value(), kP) + kD * dTerm(errorL, lastL) * elapsed;
    outputR = pTerm(target, ultraR.get_value(), kP) + kD * dTerm(errorR, lastR) * elapsed;

    left(outputL);
    right(outputR);

    lastL = errorL;
    lastR = errorR;

    last = clock();

    if(isSettled(outputL, tolerance) && isSettled(outputR, tolerance)) break;
  }

  reset();
}

void left(int speed) {
  LF.move_velocity(speed);
  LB.move_velocity(speed);
}

void right(int speed) {
  RF.move_velocity(speed);
  RB.move_velocity(speed);
}
