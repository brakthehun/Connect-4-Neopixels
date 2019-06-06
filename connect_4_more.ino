#include <Adafruit_NeoPixel.h>
#define pix 8
#define height 6
#define plr (player*3)
#define drop digitalRead(buttons[plr+1])
#define lft digitalRead(buttons[plr])
#define rht digitalRead(buttons[plr+2])
Adafruit_NeoPixel strip[height]={
  Adafruit_NeoPixel(pix, 2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(pix, 3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(pix, 4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(pix, 5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(pix, 6, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(pix, 7, NEO_GRB + NEO_KHZ800)};
  
  boolean field[2][height][pix];
  boolean player;
  boolean update=1;
  boolean win;

  byte buttons[6]={10,11,12,14,15,16};
  int color[2]={127,654};
  int pos=0;
  byte xwin;
  byte ywin;
  byte twin;
  int dir=1;
  
void setup() {
  for(int a=0;a<height;a++){
    strip[a].begin();
    //for(int b=0;b<pix;b++){
      //setPixel(a,b,random(768),255);
      //strip[a].show();
      //delay(25);
      //dimPixel(a,b,0);
    //}
    strip[a].show();
  }
  for(int a=0;a<6;a++){
    pinMode(buttons[a],INPUT_PULLUP);
  }
  //Serial.begin(9600);
}

void loop() {
  for(int a=0;a<2;a++){
    player=a;
    update=1;
    while(digitalRead(buttons[plr+1])){
      if(!lft){
        delay(25);
        color[a]+=36;
        update=1;
      }
      if(!rht){
        delay(25);
        color[a]-=18;
        update=1;
      }
      if(update){
        color[a]%=768;
        update=0;
        for(int b=0;b<pix;b++){
          setPixel(0,b,color[a],255);
        }
        strip[0].show();
        while(!rht||!lft){
          delay(1);
        }
        delay(25);
      }
    }
  }
  dimAll();
  
  pos=0;
  win=0;
  while(!win){
    player=!player;
    update=1;
    pos-=dir;
    while(drop){
      if(!lft){
        delay(25);
        dimPixel(0,pos,0);
        dir=-1;
        update=1;
      }
      if(!rht){
        delay(25);
        dimPixel(0,pos,0);
        dir=1;
        update=1;
      }
      if(update){
        update=0;
        pos+=dir;
        pos%=pix;
        while(field[0][0][pos]||field[1][0][pos]){
          pos+=dir;
          pos%=768;
        }
        setPixel(0,pos,color[player],255);
        strip[0].show();
        while(!rht||!lft){
          delay(1);
        }
        delay(25);
      }
    }
    byte drops=0;
    while(!field[0][drops+1][pos]&&!field[1][drops+1][pos]&&drops<(height-1)){
      dimPixel(drops,pos,0);
      strip[drops].show();
      drops++;
      setPixel(drops,pos,color[player],255);
      strip[drops].show();
      delay(120);
    }
    field[player][drops][pos]=1;
    testwin();
  }
  if(twin<4){
    for(int d=255;d>=0;d--){
      for(int a=0;a<height;a++){
        for(int b=0;b<pix;b++){
          for(int c=0;c<2;c++){
            if(field[c][a][b]){setPixel(a,b,color[c],d);}
          }
        }
      }
      showWin(twin,color[player]);
      for(int a=0;a<height;a++){
        strip[a].show();
      }
      delay(6);
    }
    dimAll();
    for(int a=0;a<1536;a++){
      color[player]++;
      color[player]%=768;
      showWin(twin,color[player]);
      for(int b=0;b<height;b++){
        strip[b].show();
      }
      delay(2);
    }
    delay(1000);
  }
  if(twin==4){
    int dimfield[height][pix];
    for(int a=0;a<height;a++){
      for(int b=0;b<pix;b++){
        dimfield[a][b]=random(2000);
      }
    }
    for(int a=0;a<2000;a++){
      for(int b=0;b<height;b++){
        for(int c=0;c<pix;c++){
          dimfield[b][c]=max(0,dimfield[b][c]-1);
          for(int d=0;d<2;d++){
            setPixel(b,c,color[d],min(255,dimfield[b][c]));
          }
        }
        strip[b].show();
      }
    }
  }
}

void showWin(int winz, int colorz){
  for(int a=0;a<4;a++){
    if(twin==0){setPixel(ywin,xwin+a,color[player],255);}
    if(twin==1){setPixel(ywin+a,xwin,color[player],255);}
    if(twin==2){setPixel(ywin+a,xwin+a,color[player],255);}
    if(twin==3){setPixel(ywin+a,(xwin+3)-a,color[player],255);}
  }
}

void testwin(){
  
  for(int a=0;a<height;a++){
    for(int b=0;b<pix-4;b++){
      byte count=0;
      for(int c=0;c<4;c++){
        count+=field[player][a][b+c];
      }
      if(count==4){
        win=1;
        xwin=b;
        ywin=a;
        twin=0;
      }
    }
  }
  
  for(int a=0;a<height-4;a++){
    for(int b=0;b<pix;b++){
      byte count=0;
      for(int c=0;c<4;c++){
        count+=field[player][a+c][b];
      }
      if(count==4){
        win=1;
        xwin=b;
        ywin=a;
        twin=1;
      }
    }
  }
  
  for(int a=0;a<height-3;a++){
    for(int b=0;b<pix-3;b++){
      byte count=0;
      for(int c=0;c<4;c++){
        count+=field[player][a+c][b+c];
      }
      if(count==4){
        win=1;
        xwin=b;
        ywin=a;
        twin=2;
      }
      count=0;
      for(int c=0;c<4;c++){
        count+=field[player][a+c][(b+3)-c];
      }
      if(count==4){
        win=1;
        xwin=b;
        ywin=a;
        twin=3;
      }
    }
  }
  
  for(int a=0;a<pix;a++){
    byte count=0;
    for(int b=0;b<pix;b++){
      count+=field[0][0][b];
      count+=field[1][0][b];
    }
    if(count==pix){
      win=1;
      twin=4;
    }
  }
}

void setPixel(int stripz, int pixelz, int colorz, long unsigned fadez){
  colorz%=768;
  pixelz%=pix;
  fadez*=fadez;
  fadez/=255;
  
  if(colorz<256){
    colorz=(colorz*fadez)/255;
    strip[stripz].setPixelColor(pixelz,colorz,0,fadez-colorz);
  }
  if(colorz>=256&&colorz<512){
    colorz-=256;
    colorz=(colorz*fadez)/255;
    strip[stripz].setPixelColor(pixelz,fadez-colorz,colorz,0);
  }
  if(colorz>=512){
    colorz-=512;
    colorz=(colorz*fadez)/255;
    strip[stripz].setPixelColor(pixelz,0,fadez-colorz,colorz);
  }
}

void dimPixel(int stripz, int pixelz, long unsigned fadez){
  fadez*=fadez;
  fadez/=255;
  pixelz%=pix;
  strip[stripz].setPixelColor(pixelz,fadez,fadez,fadez);
}

void dimAll(){
  for(int a=0;a<height;a++){
    for(int b=0;b<pix;b++){
      dimPixel(a,b,0);
      field[0][a][b]=0;
      field[1][a][b]=0;
    }
    strip[a].show();
  }
}