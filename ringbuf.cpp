#include "ringbuf.h"

RINGBUF::RINGBUF(I32 size)
{
  _size = size;
  _buf = new U8[_size];
  _pr = _pw = 0;
}
RINGBUF::RINGBUF(U8 *buf, I32 size)
{
  _size = size;
  _buf = buf;
  _pr = _pw = 0;
}
RINGBUF::~RINGBUF()
{
  delete []_buf;
}
I32 RINGBUF::Push(U8 b)
{
  I32 next;
  next = (_pw + 1)%_size;
  if(next == _pr) return 0;
  _buf[_pw] = b;
  _pw = next;
  return 1;
}
I32 RINGBUF::Push(U8 *bs, I32 len)
{
  I32 next, i;
  for(i = 0; i < len; i++) {
    next = (_pw + 1)%_size;
    if(next == _pr) break;
    _buf[_pw] = bs[i];
    _pw = next;
  }
  return i;
}
I32 RINGBUF::Pop(U8 *b)
{
  if(_pr == _pw) return 0;
  *b = _buf[_pr];
  _pr = (_pr + 1) % _size;
  return 1;
}

I32 RINGBUF::Pop(U8 *bs, I32 len)
{
  I32 i;
  for(i = 0; i < len; i ++) {
    if(_pr == _pw) break;
    bs[i] = _buf[_pr];
    _pr = (_pr + 1) % _size;
  }
  return i;
}

I32 RINGBUF::IsEmpty(void)
{
  return (_pw == _pr);
}

void RINGBUF::Clear(void)
{
  _pr = _pw = 0;
}

