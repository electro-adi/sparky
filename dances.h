void backandfourth() {

  pca9685.setChannelServoPulseDuration(pan_1, 2280);
  pca9685.setChannelServoPulseDuration(pan_2, 2280);
  pca9685.setChannelServoPulseDuration(pan_3, 2280);
  pca9685.setChannelServoPulseDuration(pan_4, 2280);

  for (int pos = 100; pos <= 400; pos += 2) {
    pca9685.setChannelServoPulseDuration(tilt_1, map(pos, 100, 400, pulse_duration_min, pulse_duration_max));
    pca9685.setChannelServoPulseDuration(tilt_2, map(pos, 100, 400, pulse_duration_min, pulse_duration_max));
    pca9685.setChannelServoPulseDuration(tilt_3, map(pos, 100, 400, pulse_duration_min, pulse_duration_max));
    pca9685.setChannelServoPulseDuration(tilt_4, map(pos, 100, 400, pulse_duration_min, pulse_duration_max));
    delay(5);
  }

  for (int pos = 400; pos >= 100; pos -= 2) {
    pca9685.setChannelServoPulseDuration(tilt_1, map(pos, 100, 400, pulse_duration_min, pulse_duration_max));
    pca9685.setChannelServoPulseDuration(tilt_2, map(pos, 100, 400, pulse_duration_min, pulse_duration_max));
    pca9685.setChannelServoPulseDuration(tilt_3, map(pos, 100, 400, pulse_duration_min, pulse_duration_max));
    pca9685.setChannelServoPulseDuration(tilt_4, map(pos, 100, 400, pulse_duration_min, pulse_duration_max));
    delay(5);
  }
}

void backandfourth2() {

  pca9685.setChannelServoPulseDuration(pan_1, 1537);
  pca9685.setChannelServoPulseDuration(pan_2, 1940);
  pca9685.setChannelServoPulseDuration(pan_3, 740);
  pca9685.setChannelServoPulseDuration(pan_4, 913);

  int pos1 = 2400;
  int pos2 = 500;

  while(pos1 > 1200)
  {
    pos1 -= 5;
    pos2 += 5;

    pca9685.setChannelServoPulseDuration(tilt_1, pos1);
    pca9685.setChannelServoPulseDuration(tilt_2, pos1);
    pca9685.setChannelServoPulseDuration(tilt_3, pos2);
    pca9685.setChannelServoPulseDuration(tilt_4, pos2);
  }

  while(pos1 < 2400)
  {
    pos1 += 5;
    pos2 -= 5;

    pca9685.setChannelServoPulseDuration(tilt_1, pos1);
    pca9685.setChannelServoPulseDuration(tilt_2, pos1);
    pca9685.setChannelServoPulseDuration(tilt_3, pos2);
    pca9685.setChannelServoPulseDuration(tilt_4, pos2);
  }
}

void leftandright() {

  pca9685.setChannelServoPulseDuration(pan_1, 1320);
  pca9685.setChannelServoPulseDuration(pan_2, 1320);
  pca9685.setChannelServoPulseDuration(pan_3, 1320);
  pca9685.setChannelServoPulseDuration(pan_4, 1320);

  for (int pos = 100; pos <= 400; pos += 2) {
    pca9685.setChannelServoPulseDuration(tilt_1, map(pos, 100, 400, pulse_duration_min, pulse_duration_max));
    pca9685.setChannelServoPulseDuration(tilt_2, map(pos, 100, 400, pulse_duration_min, pulse_duration_max));
    pca9685.setChannelServoPulseDuration(tilt_3, map(pos, 100, 400, pulse_duration_min, pulse_duration_max));
    pca9685.setChannelServoPulseDuration(tilt_4, map(pos, 100, 400, pulse_duration_min, pulse_duration_max));
    delay(5);
  }

  for (int pos = 400; pos >= 100; pos -= 2) {
    pca9685.setChannelServoPulseDuration(tilt_1, map(pos, 100, 400, pulse_duration_min, pulse_duration_max));
    pca9685.setChannelServoPulseDuration(tilt_2, map(pos, 100, 400, pulse_duration_min, pulse_duration_max));
    pca9685.setChannelServoPulseDuration(tilt_3, map(pos, 100, 400, pulse_duration_min, pulse_duration_max));
    pca9685.setChannelServoPulseDuration(tilt_4, map(pos, 100, 400, pulse_duration_min, pulse_duration_max));
    delay(5);
  }
}

void leftandright2() {

  pca9685.setChannelServoPulseDuration(pan_1, 1537);
  pca9685.setChannelServoPulseDuration(pan_2, 1940);
  pca9685.setChannelServoPulseDuration(pan_3, 740);
  pca9685.setChannelServoPulseDuration(pan_4, 913);

  int pos1 = 2400;
  int pos2 = 500;

  while(pos1 > 1200)//down to up
  {
    pos1 -= 5;
    pos2 += 5;



    pca9685.setChannelServoPulseDuration(tilt_1, pos1);
    pca9685.setChannelServoPulseDuration(tilt_2, pos1);
    pca9685.setChannelServoPulseDuration(tilt_3, pos2);
    pca9685.setChannelServoPulseDuration(tilt_4, pos2);

    pca9685.setChannelServoPulseDuration(pan_1, pos1);
    pca9685.setChannelServoPulseDuration(pan_2, pos1);
    pca9685.setChannelServoPulseDuration(pan_3, pos2);
    pca9685.setChannelServoPulseDuration(pan_4, pos2);
  }

  while(pos1 < 2400)//up to down
  {
    pos1 += 5;
    pos2 -= 5;

    pca9685.setChannelServoPulseDuration(tilt_1, pos1);
    pca9685.setChannelServoPulseDuration(tilt_2, pos1);
    pca9685.setChannelServoPulseDuration(tilt_3, pos2);
    pca9685.setChannelServoPulseDuration(tilt_4, pos2);

    pca9685.setChannelServoPulseDuration(pan_1, pos1);
    pca9685.setChannelServoPulseDuration(pan_2, pos1);
    pca9685.setChannelServoPulseDuration(pan_3, pos2);
    pca9685.setChannelServoPulseDuration(pan_4, pos2);
  }
}

void leftandright3() {

  pca9685.setChannelServoPulseDuration(tilt_1, 1800);
  pca9685.setChannelServoPulseDuration(tilt_2, 1800);
  pca9685.setChannelServoPulseDuration(tilt_3, 1000);
  pca9685.setChannelServoPulseDuration(tilt_4, 1000);

  int pan1 = 500;//up to 2400
  int pan2 = 2400;//down to 500

  while(pan1 <= 2400)
  {
    if(pan1 <= 2400) pan1 += 8;
    if(pan2 >= 500) pan2 -= 8;

    pca9685.setChannelServoPulseDuration(pan_1, pan1);
    pca9685.setChannelServoPulseDuration(pan_2, pan1);
    pca9685.setChannelServoPulseDuration(pan_3, pan2);
    pca9685.setChannelServoPulseDuration(pan_4, pan2);
  }

  while(pan1 >= 500)
  {
    if(pan1 >= 500) pan1 -= 8;
    if(pan2 <= 2400) pan2 += 8;

    pca9685.setChannelServoPulseDuration(pan_1, pan1);
    pca9685.setChannelServoPulseDuration(pan_2, pan1);
    pca9685.setChannelServoPulseDuration(pan_3, pan2);
    pca9685.setChannelServoPulseDuration(pan_4, pan2);
  }
}

void onebyone() {

  pca9685.setChannelServoPulseDuration(pan_1, 2400);
  pca9685.setChannelServoPulseDuration(pan_2, 2400);
  pca9685.setChannelServoPulseDuration(pan_3, 2400);
  pca9685.setChannelServoPulseDuration(pan_4, 2400);

  pca9685.setChannelServoPulseDuration(tilt_1, 2400);

  delay(300);

  pca9685.setChannelServoPulseDuration(tilt_2, 2400);

  delay(300);

  pca9685.setChannelServoPulseDuration(tilt_3, 2400);

  delay(300);

  pca9685.setChannelServoPulseDuration(tilt_4, 2400);

  delay(300);

  pca9685.setChannelServoPulseDuration(tilt_1, 1320);

  delay(300);

  pca9685.setChannelServoPulseDuration(tilt_2, 1320);

  delay(300);

  pca9685.setChannelServoPulseDuration(tilt_3, 1320);

  delay(300);

  pca9685.setChannelServoPulseDuration(tilt_4, 1320);

  delay(300);

}

void cross() {

    for (int pos = pulse_duration_min; pos <= pulse_duration_max; pos += 3) {
      pca9685.setChannelServoPulseDuration(tilt_1, pos);
      pca9685.setChannelServoPulseDuration(pan_1, pos);

      delay(1);

      pca9685.setChannelServoPulseDuration(tilt_2, pos);
      pca9685.setChannelServoPulseDuration(pan_2, pos);

      delay(1);

      pca9685.setChannelServoPulseDuration(tilt_3, pos);
      pca9685.setChannelServoPulseDuration(pan_3, pos);

      delay(1);

      pca9685.setChannelServoPulseDuration(tilt_4, pos);
      pca9685.setChannelServoPulseDuration(pan_4, pos);
      delay(1);
    }

    for (int pos = pulse_duration_max; pos >= pulse_duration_min; pos -= 3) {
      pca9685.setChannelServoPulseDuration(tilt_1, pos);
      pca9685.setChannelServoPulseDuration(pan_1, pos);

      delay(1);

      pca9685.setChannelServoPulseDuration(tilt_2, pos);
      pca9685.setChannelServoPulseDuration(pan_2, pos);

      delay(1);

      pca9685.setChannelServoPulseDuration(tilt_3, pos);
      pca9685.setChannelServoPulseDuration(pan_3, pos);

      delay(1);

      pca9685.setChannelServoPulseDuration(tilt_4, pos);
      pca9685.setChannelServoPulseDuration(pan_4, pos);
      delay(1);
    }
}

void fast_rotate() {

  int pos;
  for (pos = 0; pos <= 200; pos += 2) {
    for (int i = 0; i != 8; i++) {
      pca9685.setChannelServoPulseDuration(i, map(pos, 0, 200, pulse_duration_min, pulse_duration_max));
    }
    delay(1);
  }
  for (pos = 200; pos >= 0; pos -= 2) {
    for (int i = 0; i != 8; i++) {
      pca9685.setChannelServoPulseDuration(i, map(pos, 0, 200, pulse_duration_min, pulse_duration_max));
    }
    delay(1);
  }
}