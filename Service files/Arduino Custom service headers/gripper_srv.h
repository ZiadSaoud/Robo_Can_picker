#ifndef _ROS_SERVICE_gripper_srv_h
#define _ROS_SERVICE_gripper_srv_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace hello_world
{

static const char GRIPPER_SRV[] = "hello_world/gripper_srv";

  class gripper_srvRequest : public ros::Msg
  {
    public:
      typedef uint16_t _angle_type;
      _angle_type angle;

    gripper_srvRequest():
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

    const char * getType(){ return GRIPPER_SRV; };
    const char * getMD5(){ return "ffcccce422d555eeedf1005a7637b109"; };

  };

  class gripper_srvResponse : public ros::Msg
  {
    public:
      typedef bool _done_type;
      _done_type done;

    gripper_srvResponse():
      done(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_done;
      u_done.real = this->done;
      *(outbuffer + offset + 0) = (u_done.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->done);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_done;
      u_done.base = 0;
      u_done.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->done = u_done.real;
      offset += sizeof(this->done);
     return offset;
    }

    const char * getType(){ return GRIPPER_SRV; };
    const char * getMD5(){ return "89bb254424e4cffedbf494e7b0ddbfea"; };

  };

  class gripper_srv {
    public:
    typedef gripper_srvRequest Request;
    typedef gripper_srvResponse Response;
  };

}
#endif
