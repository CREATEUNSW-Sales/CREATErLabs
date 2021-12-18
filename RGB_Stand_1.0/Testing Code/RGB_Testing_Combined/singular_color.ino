void blueSolid() {
  analogWrite(R, 255);
  analogWrite(G, 195);
  analogWrite(B, 0);
  
  switchLEDs();
}

void whiteSolid() {
  analogWrite(R, 0);
  analogWrite(G, 0);
  analogWrite(B, 0);
  
  switchLEDs();
}

void orangeSolid() {
  analogWrite(R, 0);
  analogWrite(G, 145);
  analogWrite(B, 255);
  
  switchLEDs();
}
