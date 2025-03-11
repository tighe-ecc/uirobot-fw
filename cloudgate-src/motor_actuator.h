#ifndef MOTOR_ACTUATOR_H
#define MOTOR_ACTUATOR_H

void configureMotors();
void moveActuatorToPosition(int CANid, int position);
void startActuatorMotion();
void closeActuatorStream(int CANid);

#endif // MOTOR_ACTUATOR_H
