#include <simplecpp>
#include "shooter.h"
#include "bubble.h"
#include <string>
/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int LEFT_ENDMSG_MARGIN=250;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);
const int SCORE_POSITION=400;
const int TIMER_X=400,TIMER_Y=TOP_MARGIN-5;
const int HEALTH_X=70,HEALTH_Y=TOP_MARGIN-5;
int HEALTH=100;
double TIMER=0,TOTAL_TIME=100; 							//TIMER inititalised

int closest_distance(Bubble bubble, Bullet bullet){ 		//returns closest distance between bullet and bubble
  double x1=bubble.get_center_x(),y1=bubble.get_center_y();
  double x2=bullet.get_center_x(),y2=bullet.get_center_y();
  double w=bullet.get_width(),h=bullet.get_height();
  double xl=x2-w/2,xr=x2+w/2,yu=y2-h/2,yd=y2+h/2;
  
  if (x1<=xl && y1<=yu) return sqrt(pow(x1-xl,2)+pow(y1-yu,2));
  else if (x1>xl && x1<xr && y1<=yu) return yu-y1;
  else if (x1>=xr && y1<=yu) return sqrt(pow(x1-xr,2)+pow(y1-yu,2));
  else if (x1<=xl && y1>yu && y1<yd) return xl-x1;
  else if (x1>xl && x1<xr && y1>yu && y1<yd) return 0;
  else if (x1>=xr && y1>yu && y1<yd) return x1-xr;
  else if (x1<=xl && y1>=yd) return sqrt(pow(x1-xl,2)+pow(y1-yd,2));
  else if (x1>xl && x1<xr && y1>=yd) return y1-yd;
  else if (x1>=xr && y1>=yd) return sqrt(pow(x1-xr,2)+pow(y1-yd,2));
  
  else return 0; //for the sake of the compiler
}

int closest_shooter_body_distance(Bubble bubble, Shooter shooter){ 		//returns closest distance between bullet and shooter body
  double x1=bubble.get_center_x(),y1=bubble.get_center_y();
  double x2=shooter.get_body_center_x(),y2=shooter.get_body_center_y();
  double w=shooter.get_body_width(),h=shooter.get_body_height();
  double xl=x2-w/2,xr=x2+w/2,yu=y2-h/2,yd=y2+h/2;
  
  if (x1<=xl && y1<=yu) return sqrt(pow(x1-xl,2)+pow(y1-yu,2));
  else if (x1>xl && x1<xr && y1<=yu) return yu-y1;
  else if (x1>=xr && y1<=yu) return sqrt(pow(x1-xr,2)+pow(y1-yu,2));
  else if (x1<=xl && y1>yu && y1<yd) return xl-x1;
  else if (x1>xl && x1<xr && y1>yu && y1<yd) return 0;
  else if (x1>=xr && y1>yu && y1<yd) return x1-xr;
  else if (x1<=xl && y1>=yd) return sqrt(pow(x1-xl,2)+pow(y1-yd,2));
  else if (x1>xl && x1<xr && y1>=yd) return y1-yd;
  else if (x1>=xr && y1>=yd) return sqrt(pow(x1-xr,2)+pow(y1-yd,2));
  
  else return 0; //for the sake of the compiler
}

bool hit(Bubble bubble, Bullet bullet){ 			//returns if a bullet has hit a bubble
  if (closest_distance(bubble,bullet)<bubble.get_radius()) return true;
  else return false;
}

bool hit_shooter(Bubble bubble, Shooter shooter){ 			//returns if a bullet has hit shooter
  double x1=bubble.get_center_x(),y1=bubble.get_center_y();
  double x2=shooter.get_head_center_x(),y2=shooter.get_head_center_y();
  double r1=bubble.get_radius(),r2=shooter.get_head_radius(),rsum=r1+r2;
  double dist=sqrt(pow(x1-x2,2)+pow(y1-y2,2));    		//distance between shooter head and bubble centres
  if (closest_shooter_body_distance(bubble,shooter)<bubble.get_radius() || dist<rsum) return true;
  else return false;
}

void move_bullets(vector<Bullet> &bullets, vector<Bubble> &bubbles,int &score){
    vector<Bubble> add_bubbles;
    
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
    for(unsigned int i=0; i<bullets.size(); i++){
      for (unsigned int j=0; j < bubbles.size(); j++){
        if (hit(bubbles[j],bullets[i])){
          bullets.erase(bullets.begin()+i);
          score+=1;
          if (bubbles[j].get_radius()>BUBBLE_DEFAULT_RADIUS){
            double x=bubbles[j].get_center_x(), y=bubbles[j].get_center_y(),r=bubbles[j].get_radius(),vy=bubbles[j].get_vy();
            bubbles.erase(bubbles.begin()+j);
            add_bubbles.push_back(Bubble(x, y, r/2.0 , -BUBBLE_DEFAULT_VX, vy, COLOR(255,105,180)));
            add_bubbles.push_back(Bubble(x, y, r/2.0 , BUBBLE_DEFAULT_VX, vy, COLOR(255,105,180)));           
          }
          
          else {
            bubbles.erase(bubbles.begin()+j);
          }
        }
      }
    }
    
    bubbles.insert( bubbles.end(), add_bubbles.begin(), add_bubbles.end() );
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME,PLAY_Y_HEIGHT);
    }
}

void reset_bubbles(vector<Bubble> &bubbles){

    wait(5);    
    //reset all bubbles to position x remains same, position y is initial.
    for (unsigned int i=0; i< bubbles.size(); i++)
    {
        bubbles[i].move_to_start(bubbles[i].get_center_x());
    }
}

vector<Bubble> create_bubbles()
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X*0.1, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    bubbles.push_back(Bubble(WINDOW_X*0.3, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    bubbles.push_back(Bubble(WINDOW_X*0.4, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    bubbles.push_back(Bubble(WINDOW_X*0.6, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*2, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    bubbles.push_back(Bubble(WINDOW_X*0.8, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    //bubbles.push_back(Bubble(WINDOW_X*0.4, BUBBLE_START_Y*1.5, BUBBLE_DEFAULT_RADIUS*2, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    //bubbles.push_back(Bubble(WINDOW_X*0.7, BUBBLE_START_Y*1.6, BUBBLE_DEFAULT_RADIUS*2, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    return bubbles;
}


int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));
    
    string msg_cmd("Input: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);
    
    string end_msg("");
    Text end(LEFT_ENDMSG_MARGIN, BOTTOM_MARGIN, end_msg);

    string hit_msg("");
    Text hit(LEFT_ENDMSG_MARGIN, BOTTOM_MARGIN, hit_msg);
    
    //Initialise the score
    int score=0;
    string score_msg="SCORE : " + to_string(score);
    Text score_str(SCORE_POSITION, BOTTOM_MARGIN, score_msg);
    
    string time="TIME REMAINING" +to_string(TOTAL_TIME-TIMER);
    Text time_str(TIMER_X, TIMER_Y, time);
    
    string health="HEALTH : "+to_string(HEALTH);
    Text health_str(HEALTH_X, HEALTH_Y, health);
    
    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, PLAY_Y_HEIGHT-TOP_MARGIN , SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    // Main game loop
    while (true)
    {
        
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {   
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);
            
            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }
        //checks if the shooter has been hit
        for (unsigned int i=0; i < bubbles.size(); i++)
    	{
    	  if (hit_shooter(bubbles[i],shooter)){
            double x=bubbles[i].get_center_x(), y=bubbles[i].get_center_y(),r=bubbles[i].get_radius(),vy=bubbles[i].get_vy();
    	    
            HEALTH-=(bubbles[i].get_radius()/BUBBLE_DEFAULT_RADIUS)*20;             			//If bubble comes in contact for STEP_TIME health reduces by 5
            bubbles.erase(bubbles.begin()+i);   //bubble disappears after hitting 

            if (r > BUBBLE_DEFAULT_RADIUS){
                bubbles.push_back(Bubble(x, y, r/2.0 , -BUBBLE_DEFAULT_VX, vy, COLOR(255,105,180)));
                bubbles.push_back(Bubble(x, y, r/2.0 , BUBBLE_DEFAULT_VX, vy, COLOR(255,105,180)));
            }
            if (HEALTH > 0 && bubbles.size()>0){
                hit.setMessage("You have been hit!");
                string health="HEALTH : "+to_string(HEALTH);
                health_str.setMessage(health);
                reset_bubbles( bubbles );   //bubbles reset to starting positions
                break;
            }  
            else if (HEALTH <=0){
                string health="HEALTH : "+to_string(0);
                health_str.setMessage(health);
                msg_cmd="";
                charPressed.setMessage(msg_cmd);
                end_msg="GAME OVER! Click anywhere to exit";
                end.setMessage(end_msg);
                getClick();
                return 0;
            } 
                                                	  
          }
    	}
        //check for loss/time run out
        if (TIMER>=TOTAL_TIME || HEALTH<=0){
            if (HEALTH<0) HEALTH=0;
          string health="HEALTH : "+to_string(HEALTH);
          health_str.setMessage(health);
          msg_cmd="";
          charPressed.setMessage(msg_cmd);
          end_msg="GAME OVER! Click anywhere to exit";
          end.setMessage(end_msg);
          getClick();
          return 0;
        }
        //check for win
        if (bubbles.size()==0){
            string health="HEALTH : "+to_string(HEALTH);
            health_str.setMessage(health);
            msg_cmd="";
            charPressed.setMessage(msg_cmd);
            end_msg="You have WON! Click anywhere to exit";
            end.setMessage(end_msg);
            getClick();
            return 0;
        }

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets,bubbles,score);
	
    	//Updates the score
        score_str.setMessage("SCORE : " + to_string(score));
    	
    	//Updates the time
    	string new_time="TIME REMAINING : "+ to_string(TOTAL_TIME-TIMER);
    	time_str.setMessage(new_time.substr(0,22));
    	TIMER+=0.1;
        
        //Updates the health
        string health="HEALTH : "+to_string(HEALTH);
        health_str.setMessage(health);

        //resets hit to blank
        hit.setMessage("");

        wait(STEP_TIME);
    }
}