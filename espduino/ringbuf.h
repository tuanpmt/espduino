#ifndef _RING_BUF_H_
#define _RING_BUF_H_

#include <stdint.h>
#include <stdlib.h>
#ifdef	__cplusplus
extern "C" {
#endif

typedef struct{
	uint8_t* p_o;				/**< Original pointer */
	uint8_t* volatile p_r;		/**< Read pointer */
	uint8_t* volatile p_w;		/**< Write pointer */
	volatile int32_t fill_cnt;	/**< Number of filled slots */
	int32_t size;				/**< Buffer size */
}RINGBUF;

int16_t RINGBUF_Init(RINGBUF *r, uint8_t* buf, int32_t size);
int16_t RINGBUF_Put(RINGBUF *r, uint8_t c);
int16_t RINGBUF_Get(RINGBUF *r, uint8_t* c);

#ifdef	__cplusplus
}
#endif
#endif
