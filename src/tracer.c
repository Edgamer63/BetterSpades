#include "common.h"

struct Tracer* tracers;

void tracer_add(unsigned char type, char scoped, float x, float y, float z, float dx, float dy, float dz) {
    int a = (type==WEAPON_SHOTGUN)?5:1;
    for(int k=0;k<TRACER_MAX;k++) {
        if(!tracers[k].used) {
            float spread = 0.0F;
            switch(type) {
                case WEAPON_RIFLE:
                    tracers[k].type = 0;
                    spread = 0.0060000001F;
                    break;
                case WEAPON_SMG:
                    tracers[k].type = 1;
                    spread = 0.012F;
                    break;
                case WEAPON_SHOTGUN:
                    tracers[k].type = 2;
                    spread = 0.024F;
                    break;
            }
            float basex = (rand()-rand())/16383.0F*spread*(scoped?0.5F:1.0F);
            float basey = (rand()-rand())/16383.0F*spread*(scoped?0.5F:1.0F);
            float basez = (rand()-rand())/16383.0F*spread*(scoped?0.5F:1.0F);
            dx += basex;
            dy += basey;
            dz += basez;
            tracers[k].r.origin.x = x+dx*3.0F;
            tracers[k].r.origin.y = y+dy*3.0F;
            tracers[k].r.origin.z = z+dz*3.0F;
            tracers[k].r.direction.x = dx;
            tracers[k].r.direction.y = dy;
            tracers[k].r.direction.z = dz;
            tracers[k].created = glfwGetTime();
            tracers[k].used = 1;
            a--;
            if(a==0) {
                break;
            }
        }
    }
}

void tracer_render() {
    struct kv6_t* m[3] = {&model_semi_tracer,&model_smg_tracer,&model_shotgun_tracer};
    for(int k=0;k<TRACER_MAX;k++) {
        if(tracers[k].used) {
            matrix_push();
            matrix_translate(tracers[k].r.origin.x,tracers[k].r.origin.y,tracers[k].r.origin.z);
            matrix_pointAt(tracers[k].r.direction.x,tracers[k].r.direction.y,tracers[k].r.direction.z);
            matrix_rotate(90.0F,0.0F,1.0F,0.0F);
            matrix_upload();
            kv6_render(m[tracers[k].type],TEAM_SPECTATOR);
            matrix_pop();
        }
    }
}

void tracer_update(float dt) {
    for(int k=0;k<TRACER_MAX;k++) {
        if(tracers[k].used) {
            if(glfwGetTime()-tracers[k].created>0.5F) {
                tracers[k].used = 0;
            } else {
                struct Camera_HitType hit;
                camera_hit(&hit,-1,
                           tracers[k].r.origin.x,tracers[k].r.origin.y,tracers[k].r.origin.z,
                           tracers[k].r.direction.x,tracers[k].r.direction.y,tracers[k].r.direction.z,
                           256.0F*dt);
                if(hit.type==CAMERA_HITTYPE_BLOCK) {
                    sound_create(NULL,SOUND_WORLD,&sound_impact,tracers[k].r.origin.x,tracers[k].r.origin.y,tracers[k].r.origin.z);
                    tracers[k].used = 0;
                } else {
                    tracers[k].r.origin.x += tracers[k].r.direction.x*256.0F*dt;
                    tracers[k].r.origin.y += tracers[k].r.direction.y*256.0F*dt;
                    tracers[k].r.origin.z += tracers[k].r.direction.z*256.0F*dt;
                }
            }
        }
    }
}

void tracer_init() {
    tracers = calloc(sizeof(struct Tracer)*TRACER_MAX,1);
}
