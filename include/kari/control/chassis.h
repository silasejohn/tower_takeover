#include "main.h"
#include "kari/util/misc.h"

extern pros::Motor LF, LB, RF, RB;
extern pros::ADIUltrasonic Ultrasonic;

#define DRIVING_POINT 1
#define DRIVING_DIST 2
#define DRIVING_ULTRASONIC 3
#define TURNING 4
#define STRAFING 5
#define STRAFING_SMART 6

struct Vector2 {
  double x;
  double y;
};

struct ChassisTarget {
  double x;
  double y;
  double theta;
  double direction;
  double drivespeed;
  double turnspeed;
  int speed;
  double rate;
  double rate2;
  bool reverse;
};

class Chassis {
  public:
    Chassis();
    Chassis(double * odomL_, double * odomR_, double * theta_, double * posX_, double * posY_);
    ~Chassis();

    // Constant Settings
    Chassis& withGain(double kP = 0.9, double kI = 0, double kD = 3.3, bool isTurn = false);
    Chassis& withTol(double tolerance_ = 1);
    Chassis& withSlop(double offset_ = 0, double amp_ = 0.2);

    // Movement Settings
    Chassis& withoutOdom();
    Chassis& withGyro(double theta_);
    Chassis& withPoint(Vector2 point, int speed_, double rate_ = 4, bool reverse_ = false);
    Chassis& withTarget(double target_, double theta_, int speed_, double rate_ = 4, bool reverse_ = false);

    // Actuators
    Chassis& drive();
    Chassis& drive(Vector2 point, int speed_, int rate_ = 4, bool reverse = false);
    Chassis& drive(double target_, int speed_, double rate_ = 4, int rate2_ =4);
    Chassis& driveUltrasonic(double target_, int speed_, int rate_ = 4);
    Chassis& turn(Vector2 point, int speed_, int rate_ = 4);
    Chassis& turn(double theta_, int speed_, int rate_ = 4);
    Chassis& strafe(double target_, int speed_, int rate_ = 4);
    Chassis& smartstrafe(double direction_, double theta_, double drivespeed_ = 80, double turnspeed_ = 50, double rate_ = 4, double rate2_ = 4);

    void waitUntilSettled();

    // Gyro
    Chassis& calibrateGyro();
    Chassis& tareGyro();

    double * getGyro();

    // Chassis Motor Settings
    void tarePos();
    void reset();

    void lock();
    void unlock();

    // Process Related
    int getMode();
    void setMode(int mode);

    void clearArr();

    // Task
    static void start(void* ignore);
    void run();
    void stop();

    void left(int speed);
    void right(int speed);

  private:
    static bool isRunning;
    static bool isSettled;
    static int mode;

    static double kP_drive, kI_drive, kD_drive, kP_turn, kI_turn, kD_turn;
    static double tolerance, amp, offset;
    static std::vector<ChassisTarget> target;
    static int currTarget;
    static bool isUsingOdom, isUsingGyro, isTurnToPoint;

    static double *odomL, *odomR, *theta, *posX, *posY;

    static double current, gyroOffset, thetaRel, initL, initR, deltaL, deltaR;
    static double driveError, driveIntegral, driveLast, turnError, turnIntegral, turnLast;
    static double driveOutput, driveOutput2, driveOutput3, driveOutput4, turnOutput, driveSlewOutput, driveSlewOutput2, driveSlewOutput3, driveSlewOutput4, turnSlewOutput;
    static double totOutputL, totOutputR;

    double slop(int mode = 0);
};
