typedef struct {
	float x,y,z;
	float vx,vy,vz;
	float ox,oy,oz;
	unsigned char type;
	float size;
	float created;
	float fade;
	unsigned int color;
	boolean alive;
} Particle;

extern Particle* particles;
extern float* particles_vertices;
extern unsigned char* particles_colors;
#define PARTICLES_MAX 8192

void particle_init(void);
void particle_update(float dt);
int particle_render(void);
void particle_create_casing(struct Player* p);
void particle_create(unsigned int color, float x, float y, float z, float velocity, float velocity_y, int amount, float min_size, float max_size);
