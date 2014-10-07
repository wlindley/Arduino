#include <MicroView.h>

int i = 0;

MicroViewWidget* widget;

void setup() {
    uView.begin();
    uView.clear(PAGE);
    //uView.display();
    
    widget = new MicroViewGauge(32, 24, 0, 100);
}

void loop() {
    //uView.clear(PAGE);
    
    //uView.setCursor(0, 0);
    //uView.setFontType(3);
    //uView.print(i);
    
    //uView.circleFill(32, 24, i);
    
    widget->setValue(i);
    
    uView.display();
    
    //if (i < 24)
    if (i < 100)
        i++;
    delay(100);
}
