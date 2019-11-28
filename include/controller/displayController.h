#pragma once
#include "pros/apix.h"

// Image Declaration
LV_IMG_DECLARE(intro);
LV_IMG_DECLARE(logo);

LV_IMG_DECLARE(michael1);
LV_IMG_DECLARE(michael2);

class Display {
  public:
    Display();

    void main();
    void auton();
    void sensor();
    void camera();
    void setting();
    
    void run();
    static void start(void* ignore);

    void setRemoteText(std::string text_);

  private:
    static std::string setText, lastText;

    void remoteUpdate();

    // Macros
    lv_obj_t * createLabel(int x, int y, std::string text, lv_obj_t * parent);
    lv_obj_t * createButton(int id, int x, int y, int width, int height, std::string text, lv_obj_t * parent, lv_action_t action);
    lv_obj_t * drawRectangle(int x, int y, int width, int height, lv_color_t stroke, lv_color_t fill, lv_obj_t * parent);
};