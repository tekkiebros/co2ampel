void setColor(int coWert){

  if(coWert>=300 && coWert<700){
    strip.setPixelColor(0, green);
    strip.show();
  }
  if(coWert>=700 && coWert<1000){
    strip.setPixelColor(0, yellow);
    strip.show();
  }
  if(coWert>=1000 && coWert<2000){
    strip.setPixelColor(0, red);
    strip.show();
  }
  if(coWert>=2000){
    for(int k=0;k<=5; k++){
      strip.setPixelColor(0, red);
      strip.show();
      delay(500);
      strip.setPixelColor(0,0,0,0);
      strip.show();
      delay(500);
    }
    return;
  }
  
}
