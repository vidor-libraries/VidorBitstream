/*
 * ov5647.h
 *
 *  Created on: Apr 11, 2018
 *      Author: max
 */

#ifndef OV5647_H_
#define OV5647_H_


int ov5647Detect(void);
int ov5647SensorPower(int on);
int ov5647StreamOn(void);
int ov5647StreamOff(void);


#endif /* OV5647_H_ */
