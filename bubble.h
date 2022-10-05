#include <simplecpp>

/* Bubble Vars */
const int BUBBLE_START_X = 250;
const int BUBBLE_START_Y = 50;
const int BUBBLE_DEFAULT_RADIUS = 10;
const int BUBBLE_RADIUS_THRESHOLD = 10;
const int BUBBLE_DEFAULT_VX = 100;
const int BUBBLE_DEFAULT_VY = 0;
const int GRAVITY=60;
class Bubble
{
private:
    Circle circle;  // the circle representing the bubble
    double vx, vy;  // velocity in x and y direction
    //double radius;  // radius of the bubble
    //double bubble_x, bubble_y; //center of the bubble
    Color color;    // color of the bubble

public:
    Bubble(double cx, double cy, double r, double vx_=BUBBLE_DEFAULT_VX, double vy_=BUBBLE_DEFAULT_VY, Color color_=COLOR(0, 0, 255))
    {
        // Bubble constructor
        color = color_;
        circle = Circle(cx, cy, r);
        circle.setColor(color);
        circle.setFill(true);
        //radius = r;
        vx = vx_;
        vy = vy_;
        //bubble_x = cx;
        //bubble_y = cy;
    }

    void nextStep(double t,int PLAY_Y_HEIGHT)
    {
        // move the bubble
        double new_x = circle.getX() + vx*t;
        double new_y = circle.getY() + vy*t;
	vy+=GRAVITY*t;
        if ((vx < 0 && new_x < (0 + circle.getRadius())) // bounce along X direction at left border
            ||
            (vx > 0 && new_x > (WINDOW_X - circle.getRadius()))) // bounce along X direction at right border
        {
            
            vx = -vx;
            new_x = circle.getX() + vx*t;
            
        }
	if (vy>0 && new_y > (PLAY_Y_HEIGHT-circle.getRadius())) //bounce along y direction at bottom line
	{
	  vy=-vy;
	  new_y=circle.getY() + vy*t;
	}
        circle.moveTo(new_x, new_y);
    }

    void move_to_start(double x){           //moves to starting position
        circle.moveTo(x, BUBBLE_START_Y);
        vy = BUBBLE_DEFAULT_VY;
    }

    double get_radius()
    {
        // return the radius of the bubble
        return circle.getRadius();
    }

    double get_center_x()
    {
        // return the x coordinate of the center of the bubble
        return circle.getX();
    }

    double get_center_y()
    {
        // return the y coordinate of the center of the bubble
        return circle.getY();
    }

    double get_vx()
    {
        // return the x velocity of the bubble
        return vx;
    }
    double get_vy()
    {
        // return the y velocity of the bubble
        return vy;
    }

    Color get_color()
    {
        // return the color of the bubble
        return color;
    }
};
