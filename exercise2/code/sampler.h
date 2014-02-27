#ifndef SAMPLER_H_
#define SAMPLER_H_

/* initialize sampler */
extern void
sampler_init();

/* set sampler mode */
extern void
sampler_set_mode(int mode);

/* get sample */
extern int
sampler_get();

typedef struct sample
{
	float hz;
	int ms;
} sample_t;



#endif
