void blueSolid() {
  HsvToRgb(226, 1, brightness, rgb);
  analogWrite(R, 255 - rgb[0]);
  analogWrite(G, 255 - rgb[1]);
  analogWrite(B, 255 - rgb[2]);
  
  switchLEDs();
}

void whiteSolid() {
  HsvToRgb(0, 0, brightness, rgb);
  analogWrite(R, 255 - rgb[0]);
  analogWrite(G, 255 - rgb[1]);
  analogWrite(B, 255 - rgb[2]);
  
  switchLEDs();
}

void orangeSolid() {
  HsvToRgb(24, 1, brightness, rgb);
  analogWrite(R, 255 - rgb[0]);
  analogWrite(G, 255 - rgb[1]);
  analogWrite(B, 255 - rgb[2]);
  
  switchLEDs();
}
