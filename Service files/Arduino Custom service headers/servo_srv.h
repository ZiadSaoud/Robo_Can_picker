#ifndef _ROS_SERVICE_servo_srv_h
#define _ROS_SERVICE_servo_srv_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace hello_world
{

static const char SERVO_SRV[] = "hello_world/servo_srv";

  class servo_srvRequest : public ros::Msg
  {
    public:
      typedef uint16_t _angle_type;
      _angle_type angle;

    servo_srvRequest():
      angle(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->angle >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->angle >> (8 * 1)) & 0xFF;
      offset += sizeof(this->angle);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->angle =  ((uint16_t) (*(inbuffer + offset)));
      this->angle |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->angle);
     return offset;
    }

    const char * getType(){ return SERVO_SRV; };
    const char * getMD5(){ return "ffcccce422d555eeedf1005a7637b109"; };

  };

  class servo_srvResponse : public ros::Msg
  {
    public:
      typedef uint16_t _position_type;
      _position_type position;

    servo_srvResponse():
      position(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->position >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->position >> (8 * 1)) & 0xFF;
      offset += sizeof(this->position);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->position =  ((uint16_t) (*(inbuffer + offset)));
      this->position |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->position);
     return offset;
    }

    const char * getType(){ return SERVO_SRV; };
    const char * getMD5(){ return "e6fa9d1b49571c7fff8bae5e451e5e59"; };

  };

  class servo_srv {
    public:
    typedef servo_srvRequest Request;
    typedef servo_srvResponse Response;
  };

}
#endif
