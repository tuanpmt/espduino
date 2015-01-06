/**
 * \file
 *       Circular buffer class
 * \author
 *       Tuan PM <tuanpm@live.com>
 */
#ifndef _RINGBUF_H_
#define _RINGBUF_H_
#include "typedef.h"
class RINGBUF
{
public:
  /**
   * \brief
   *       Circular buffer class
   * \param
   *       size: size in byte of buffer
   */
  RINGBUF(I32 size);

  /**
   * \brief
   *       Circular buffer class
   * \param
   *       buf: user defined buffer
   *       size: size in byte of buffer
   */

  RINGBUF(U8 *buf, I32 size);
  ~RINGBUF();

  /**
   * \brief
   *       Put a byte to buffer
   * \param
   *       b: byte value
   * \return
   *       0 if buffer is fully
   *       1 if success
   */
  I32 Push(U8 b);

  /**
   * \brief
   *       Put bytes array to buffer
   * \param
   *       bs: point to array
   *       len: length of array
   * \return
   *       0 if buffer is fully
   *       lenght of array if success
   */
  I32 Push(U8 *bs, I32 len);

  /**
   * \brief
   *       pop a byte from buffer
   * \param
   *       b: point to 
   * \return
   *       0 if buffer is empty
   *       1 if success
   */
  I32 Pop(U8 *b);

  I32 Pop(U8 *bs, I32 len);
  void Clear(void);
  I32 IsEmpty(void);
private:
  U8 *_buf;
  I32 _size;
  I32 _pw, _pr;
};
#endif

