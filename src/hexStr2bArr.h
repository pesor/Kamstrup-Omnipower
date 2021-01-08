void hexStr2bArr(uint8_t *dest, const char *source, int bytes_n)
{
  uint8_t *dst = dest;
  uint8_t *end = dest + sizeof(bytes_n);
  unsigned int u;

  while (dest < end && sscanf(source, "%2x", &u) == 1)
  {
    *dst++ = u;
    source += 2;
  }
}