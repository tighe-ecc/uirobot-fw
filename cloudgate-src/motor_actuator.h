#ifndef MOTOR_ACTUATOR_H
#define MOTOR_ACTUATOR_H

void configureMotor();
void moveActuatorToPosition(int CANid, int position);
void closeActuatorStream(int CANid);

#endif // MOTOR_ACTUATOR_H
